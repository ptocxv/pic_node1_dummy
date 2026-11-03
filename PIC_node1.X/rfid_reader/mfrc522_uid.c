#include <stddef.h>

#include "mfrc522_uid.h"

#include "mfrc522_core.h"
#include "mfrc522_transceive.h"

/*
 * Current UID-reading state.
 */
static mfrc522_uid_state_t uidState =
    MFRC522_UID_IDLE;

/*
 * Final detected UID.
 */
static mfrc522_uid_t detectedUid;

/*
 * Response received from the most recently completed
 * Transceive operation.
 */
static uint8_t responseBuffer[
    MFRC522_MAX_FRAME_SIZE
];

/*
 * Anticollision responses.
 *
 * Bytes 0 to 3:
 * UID bytes or cascade tag and UID bytes.
 *
 * Byte 4:
 * BCC checksum.
 */
static uint8_t level1Response[
    MFRC522_ANTICOLLISION_LENGTH
];

static uint8_t level2Response[
    MFRC522_ANTICOLLISION_LENGTH
];

/*
 * Complete SELECT frame:
 *
 * Cascade command
 * NVB
 * Four UID/cascade bytes
 * BCC
 * CRC low
 * CRC high
 */
static uint8_t selectFrame[
    MFRC522_SELECT_FRAME_LENGTH
];

/*
 * SAK response:
 *
 * SAK
 * CRC low
 * CRC high
 */
static uint8_t sakResponse[
    MFRC522_SAK_RESPONSE_LENGTH
];

/*
 * Private helper declarations.
 */
static void MFRC522_UidClearResult(void);

static void MFRC522_UidSetError(void);

static bool MFRC522_UidReadResponse(
    uint8_t *destination,
    uint8_t expectedLength
);

static bool MFRC522_UidCheckBcc(
    const uint8_t response[ MFRC522_ANTICOLLISION_LENGTH ]
);

static void MFRC522_UidBuildSelectFrame(
    uint8_t cascadeCommand,
    const uint8_t anticollisionResponse[ MFRC522_ANTICOLLISION_LENGTH ]
);

static bool MFRC522_UidCheckSakCrc(
    const uint8_t response[ MFRC522_SAK_RESPONSE_LENGTH ]
);

static void MFRC522_UidCalculateCrcA(
    const uint8_t *data,
    uint8_t length,
    uint8_t result[2]
);


/*
 * Start a new non-blocking UID-reading operation.
 */
bool MFRC522_UidStart(void)
{
    if (!MFRC522_CoreIsReady())
    {
        return false;
    }

    if (MFRC522_UidIsBusy())
    {
        return false;
    }

    MFRC522_UidClearResult();

    MFRC522_TransceiveReset();

    uidState = MFRC522_UID_REQA_START;

    return true;
}


/*
 * Progress the non-blocking UID-reading state machine.
 */
void MFRC522_UidUpdate(void)
{
    uint8_t command[2];
    uint8_t responseLength;
    uint8_t sak;

    /*
     * Progress the currently active RFID frame exchange.
     */
    MFRC522_TransceiveUpdate();

    switch (uidState)
    {
        case MFRC522_UID_IDLE:
            break;

        /*
         * =================================================
         * REQA
         * =================================================
         */

        case MFRC522_UID_REQA_START:
            command[0] = PICC_CMD_REQA;

            if (MFRC522_TransceiveStart(
                    command,
                    1U,
                    MFRC522_REQA_VALID_BITS,
                    MFRC522_TRANSCEIVE_TIMEOUT_MS))
            {
                uidState = MFRC522_UID_REQA_WAIT;
            }
            break;

        case MFRC522_UID_REQA_WAIT:
            if (MFRC522_TransceiveResponseAvailable())
            {
                responseLength = MFRC522_TransceiveGetResponseLength();

                /*
                 * ATQA must contain exactly two bytes.
                 */
                if (responseLength != MFRC522_ATQA_LENGTH)
                {
                    MFRC522_UidSetError();
                    break;
                }

                /*
                 * ATQA proves that an ISO/IEC 14443A
                 * tag answered. The ATQA bytes are not
                 * part of the UID, so they do not need
                 * to be retained here.
                 */
                MFRC522_TransceiveReset();

                uidState = MFRC522_UID_ANTICOLLISION_CL1_START;
            }
            else if (MFRC522_TransceiveNoResponse())
            {
                MFRC522_TransceiveReset();

                uidState = MFRC522_UID_NO_TAG;
            }
            else if (MFRC522_TransceiveHasError())
            {
                MFRC522_UidSetError();
            }
            break;

        /*
         * =================================================
         * Cascade level 1 anticollision
         * =================================================
         */

        case MFRC522_UID_ANTICOLLISION_CL1_START:
            /*
             * 0x93:
             * Cascade-level-1 anticollision command.
             *
             * 0x20:
             * NVB requesting the complete UID fragment.
             */
            command[0] = PICC_CMD_ANTICOLL_CL1;
            command[1] = 0x20U;

            if (MFRC522_TransceiveStart(
                    command,
                    2U,
                    MFRC522_FULL_BYTE_VALID_BITS,
                    MFRC522_TRANSCEIVE_TIMEOUT_MS))
            {
                uidState = MFRC522_UID_ANTICOLLISION_CL1_WAIT;
            }
            break;

        case MFRC522_UID_ANTICOLLISION_CL1_WAIT:
            if (MFRC522_TransceiveResponseAvailable())
            {
                if (!MFRC522_UidReadResponse(
                        level1Response,
                        MFRC522_ANTICOLLISION_LENGTH))
                {
                    MFRC522_UidSetError();
                    break;
                }

                if (!MFRC522_UidCheckBcc(
                        level1Response))
                {
                    MFRC522_UidSetError();
                    break;
                }

                MFRC522_TransceiveReset();

                uidState = MFRC522_UID_SELECT_CL1_START;
            }
            else if (MFRC522_TransceiveNoResponse())
            {
                MFRC522_TransceiveReset();

                uidState = MFRC522_UID_NO_TAG;
            }
            else if (MFRC522_TransceiveHasError())
            {
                MFRC522_UidSetError();
            }
            break;

        /*
         * =================================================
         * Cascade level 1 SELECT
         * =================================================
         */

        case MFRC522_UID_SELECT_CL1_START:
            MFRC522_UidBuildSelectFrame(
                PICC_CMD_ANTICOLL_CL1,
                level1Response
            );

            if (MFRC522_TransceiveStart(
                    selectFrame,
                    MFRC522_SELECT_FRAME_LENGTH,
                    MFRC522_FULL_BYTE_VALID_BITS,
                    MFRC522_TRANSCEIVE_TIMEOUT_MS))
            {
                uidState = MFRC522_UID_SELECT_CL1_WAIT;
            }
            break;

        case MFRC522_UID_SELECT_CL1_WAIT:
            if (MFRC522_TransceiveResponseAvailable())
            {
                if (!MFRC522_UidReadResponse(
                        sakResponse,
                        MFRC522_SAK_RESPONSE_LENGTH))
                {
                    MFRC522_UidSetError();
                    break;
                }

                if (!MFRC522_UidCheckSakCrc(
                        sakResponse))
                {
                    MFRC522_UidSetError();
                    break;
                }

                sak = sakResponse[0];

                MFRC522_TransceiveReset();

                /*
                 * If the first anticollision byte is the
                 * cascade tag, the UID continues in
                 * cascade level 2.
                 */
                if (level1Response[0] == PICC_CASCADE_TAG)
                {
                    /*
                     * The SAK cascade bit must be set
                     * because another UID level follows.
                     */
                    if ((sak & PICC_SAK_CASCADE_BIT) == 0U)
                    {
                        MFRC522_UidSetError();
                        break;
                    }

                    /*
                     * Do not include the 0x88 cascade tag
                     * in the final UID.
                     */
                    detectedUid.bytes[0] = level1Response[1];

                    detectedUid.bytes[1] = level1Response[2];

                    detectedUid.bytes[2] = level1Response[3];

                    uidState = MFRC522_UID_ANTICOLLISION_CL2_START;
                }
                else
                {
                    /*
                     * No cascade tag means a four-byte UID.
                     *
                     * The SAK cascade bit must therefore
                     * be clear.
                     */
                    if ((sak & PICC_SAK_CASCADE_BIT) != 0U)
                    {
                        MFRC522_UidSetError();
                        break;
                    }

                    detectedUid.bytes[0] = level1Response[0];

                    detectedUid.bytes[1] = level1Response[1];

                    detectedUid.bytes[2] = level1Response[2];

                    detectedUid.bytes[3] = level1Response[3];

                    detectedUid.length = MFRC522_UID_SINGLE_LENGTH;

                    uidState = MFRC522_UID_READY;
                }
            }
            else if (MFRC522_TransceiveNoResponse() || MFRC522_TransceiveHasError())
            {
                MFRC522_UidSetError();
            }
            break;

        /*
         * =================================================
         * Cascade level 2 anticollision
         * =================================================
         */

        case MFRC522_UID_ANTICOLLISION_CL2_START:
            /*
             * 0x95:
             * Cascade-level-2 anticollision command.
             *
             * 0x20:
             * NVB requesting the complete UID fragment.
             */
            command[0] = PICC_CMD_ANTICOLL_CL2;
            command[1] = 0x20U;

            if (MFRC522_TransceiveStart(
                    command,
                    2U,
                    MFRC522_FULL_BYTE_VALID_BITS,
                    MFRC522_TRANSCEIVE_TIMEOUT_MS))
            {
                uidState = MFRC522_UID_ANTICOLLISION_CL2_WAIT;
            }
            break;

        case MFRC522_UID_ANTICOLLISION_CL2_WAIT:
            if (MFRC522_TransceiveResponseAvailable())
            {
                if (!MFRC522_UidReadResponse(
                        level2Response,
                        MFRC522_ANTICOLLISION_LENGTH))
                {
                    MFRC522_UidSetError();
                    break;
                }

                if (!MFRC522_UidCheckBcc(
                        level2Response))
                {
                    MFRC522_UidSetError();
                    break;
                }

                MFRC522_TransceiveReset();

                uidState = MFRC522_UID_SELECT_CL2_START;
            }
            else if (MFRC522_TransceiveNoResponse() || MFRC522_TransceiveHasError())
            {
                MFRC522_UidSetError();
            }
            break;

        /*
         * =================================================
         * Cascade level 2 SELECT
         * =================================================
         */

        case MFRC522_UID_SELECT_CL2_START:
            MFRC522_UidBuildSelectFrame(
                PICC_CMD_ANTICOLL_CL2,
                level2Response
            );

            if (MFRC522_TransceiveStart(
                    selectFrame,
                    MFRC522_SELECT_FRAME_LENGTH,
                    MFRC522_FULL_BYTE_VALID_BITS,
                    MFRC522_TRANSCEIVE_TIMEOUT_MS))
            {
                uidState = MFRC522_UID_SELECT_CL2_WAIT;
            }
            break;

        case MFRC522_UID_SELECT_CL2_WAIT:
            if (MFRC522_TransceiveResponseAvailable())
            {
                if (!MFRC522_UidReadResponse(sakResponse, MFRC522_SAK_RESPONSE_LENGTH))
                {
                    MFRC522_UidSetError();
                    break;
                }

                if (!MFRC522_UidCheckSakCrc(sakResponse))
                {
                    MFRC522_UidSetError();
                    break;
                }

                sak = sakResponse[0];

                /*
                 * A set cascade bit would mean that
                 * cascade level 3 is required.
                 *
                 * This implementation currently supports
                 * only four-byte and seven-byte UIDs.
                 */
                if ((sak & PICC_SAK_CASCADE_BIT) != 0U)
                {
                    MFRC522_UidSetError();
                    break;
                }

                detectedUid.bytes[3] = level2Response[0];

                detectedUid.bytes[4] = level2Response[1];

                detectedUid.bytes[5] = level2Response[2];

                detectedUid.bytes[6] = level2Response[3];

                detectedUid.length = MFRC522_UID_DOUBLE_LENGTH;

                MFRC522_TransceiveReset();

                uidState = MFRC522_UID_READY;
            }
            else if (MFRC522_TransceiveNoResponse()
                     || MFRC522_TransceiveHasError())
            {
                MFRC522_UidSetError();
            }
            break;

        /*
         * =================================================
         * Completed states
         * =================================================
         */

        case MFRC522_UID_READY: break;

        case MFRC522_UID_NO_TAG: break;

        case MFRC522_UID_ERROR: break;

        default:
            MFRC522_UidSetError();
            break;
    }
}


/*
 * Return true while the UID operation is active.
 */
bool MFRC522_UidIsBusy(void)
{
    return (
        (uidState != MFRC522_UID_IDLE)
        && (uidState != MFRC522_UID_READY)
        && (uidState != MFRC522_UID_NO_TAG)
        && (uidState != MFRC522_UID_ERROR)
    );
}


/*
 * Return true when the final UID is available.
 */
bool MFRC522_UidAvailable(void)
{
    return (
        uidState == MFRC522_UID_READY
    );
}


/*
 * Copy the final UID into the caller-owned structure.
 */
bool MFRC522_UidGet(
    mfrc522_uid_t *uid
)
{
    uint8_t index;

    if (uid == NULL)
    {
        return false;
    }

    if (!MFRC522_UidAvailable())
    {
        return false;
    }

    uid->length = detectedUid.length;

    for (index = 0U; index < detectedUid.length; index++)
    {
        uid->bytes[index] = detectedUid.bytes[index];
    }

    /*
     * Clear unused bytes in the caller's UID structure.
     */
    for (index = detectedUid.length; index < MFRC522_UID_MAX_LENGTH; index++)
    {
        uid->bytes[index] = 0U;
    }

    return true;
}


/*
 * Return true if no tag answered REQA.
 */
bool MFRC522_UidNoTag(void)
{
    return (
        uidState == MFRC522_UID_NO_TAG
    );
}


/*
 * Return true if the UID operation failed.
 */
bool MFRC522_UidHasError(void)
{
    return (
        uidState == MFRC522_UID_ERROR
    );
}


/*
 * Reset a completed operation to IDLE.
 */
void MFRC522_UidReset(void)
{
    if (MFRC522_UidIsBusy())
    {
        return;
    }

    MFRC522_TransceiveReset();

    MFRC522_UidClearResult();

    uidState = MFRC522_UID_IDLE;
}


/*
 * Cancel an active UID operation.
 */
void MFRC522_UidCancel(void)
{
    MFRC522_TransceiveCancel();

    MFRC522_UidClearResult();

    uidState = MFRC522_UID_IDLE;
}


/*
 * Return the current UID state.
 */
mfrc522_uid_state_t MFRC522_UidGetState(void)
{
    return uidState;
}


/*
 * Clear the internally stored UID.
 */
static void MFRC522_UidClearResult(void)
{
    uint8_t index;

    detectedUid.length = 0U;

    for (index = 0U; index < MFRC522_UID_MAX_LENGTH; index++)
    {
        detectedUid.bytes[index] = 0U;
    }

    for (index = 0U; index < MFRC522_MAX_FRAME_SIZE; index++)
    {
        responseBuffer[index] = 0U;
    }

    for (index = 0U; index < MFRC522_ANTICOLLISION_LENGTH; index++)
    {
        level1Response[index] = 0U;
        level2Response[index] = 0U;
    }

    for (index = 0U; index < MFRC522_SELECT_FRAME_LENGTH; index++)
    {
        selectFrame[index] = 0U;
    }

    for (index = 0U; index < MFRC522_SAK_RESPONSE_LENGTH; index++)
    {
        sakResponse[index] = 0U;
    }
}


/*
 * Place the UID operation in the error state.
 */
static void MFRC522_UidSetError(void)
{
    if (MFRC522_TransceiveIsBusy())
    {
        MFRC522_TransceiveCancel();
    }
    else
    {
        MFRC522_TransceiveReset();
    }

    uidState = MFRC522_UID_ERROR;
}


/*
 * Copy and validate the response from the Transceive layer.
 */
static bool MFRC522_UidReadResponse(
    uint8_t *destination,
    uint8_t expectedLength
)
{
    uint8_t receivedLength;

    if (destination == NULL)
    {
        return false;
    }

    if (!MFRC522_TransceiveResponseAvailable())
    {
        return false;
    }

    receivedLength =
        MFRC522_TransceiveGetResponseLength();

    if (receivedLength != expectedLength)
    {
        return false;
    }

    return MFRC522_TransceiveCopyResponse(
        destination,
        expectedLength
    );
}


/*
 * Verify an anticollision BCC checksum.
 *
 * BCC = byte0 XOR byte1 XOR byte2 XOR byte3
 */
static bool MFRC522_UidCheckBcc(
    const uint8_t response[ MFRC522_ANTICOLLISION_LENGTH ]
)
{
    uint8_t calculatedBcc;

    calculatedBcc = (uint8_t)(
        response[0] ^ response[1] ^ response[2] ^ response[3]
    );

    return (
        calculatedBcc == response[4]
    );
}


/*
 * Build the complete nine-byte SELECT frame.
 */
static void MFRC522_UidBuildSelectFrame(
    uint8_t cascadeCommand,
    const uint8_t anticollisionResponse[ MFRC522_ANTICOLLISION_LENGTH ]
)
{
    uint8_t crc[2];
    uint8_t index;

    selectFrame[0] = cascadeCommand;

    selectFrame[1] = PICC_CMD_SELECT;

    /*
     * Copy four UID/cascade bytes and BCC.
     */
    for (index = 0U; index < MFRC522_ANTICOLLISION_LENGTH; index++)
    {
        selectFrame[index + 2U] = anticollisionResponse[index];
    }

    /*
     * Calculate CRC_A over:
     *
     * Cascade command
     * NVB
     * Four UID/cascade bytes
     * BCC
     *
     * Total: seven bytes.
     */
    MFRC522_UidCalculateCrcA(
        selectFrame,
        7U,
        crc
    );

    selectFrame[7] = crc[0];
    selectFrame[8] = crc[1];
}


/*
 * Validate the CRC returned with the SAK byte.
 */
static bool MFRC522_UidCheckSakCrc(
    const uint8_t response[ MFRC522_SAK_RESPONSE_LENGTH ]
)
{
    uint8_t calculatedCrc[2];

    MFRC522_UidCalculateCrcA(
        response,
        1U,
        calculatedCrc
    );

    return (
        (calculatedCrc[0] == response[1]) && (calculatedCrc[1] == response[2])
    );
}


/*
 * Calculate ISO/IEC 14443A CRC_A.
 *
 * result[0] = CRC low byte
 * result[1] = CRC high byte
 */
static void MFRC522_UidCalculateCrcA(
    const uint8_t *data,
    uint8_t length,
    uint8_t result[2]
)
{
    uint16_t crc;
    uint8_t dataByte;
    uint8_t index;

    crc = MFRC522_CRC_A_INITIAL_VALUE;

    for (index = 0U; index < length; index++)
    {
        dataByte = (uint8_t)(
            data[index] ^ (uint8_t)(crc & 0x00FFU)
        );

        dataByte = (uint8_t)(
            dataByte ^ (uint8_t)(dataByte << 4U)
        );

        crc = (uint16_t)(
            (crc >> 8U)
            ^ ((uint16_t)dataByte << 8U)
            ^ ((uint16_t)dataByte << 3U)
            ^ ((uint16_t)dataByte >> 4U)
        );
    }

    result[0] = (uint8_t)(crc & 0x00FFU);

    result[1] = (uint8_t)((crc >> 8U) & 0x00FFU);
}
