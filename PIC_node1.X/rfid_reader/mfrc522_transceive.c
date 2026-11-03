#include <stddef.h>

#include "mfrc522_transceive.h"

#include "mfrc522_core.h"
#include "timebase.h"
#include "rfid_irq.h"

/*
 * Current Transceive state.
 */
static mfrc522_transceive_state_t transceiveState =
    MFRC522_TRANSCEIVE_IDLE;

/*
 * Software timeout deadline.
 */
static uint32_t transceiveDeadline = 0U;

/*
 * Received response bytes.
 */
static uint8_t transceiveResponse[
    MFRC522_MAX_FRAME_SIZE
];

static uint8_t transceiveResponseLength = 0U;


/*
 * Place the MFRC522 and software state into a clean
 * completed-error condition.
 */
static void MFRC522_TransceiveSetError(
    mfrc522_transceive_state_t errorState
);


/*
 * Stop the active MFRC522 Transceive command and clear
 * the associated reader flags.
 */
static void MFRC522_TransceiveStopHardware(void);


/*
 * Start one non-blocking Transceive operation.
 */
bool MFRC522_TransceiveStart(
    const uint8_t *sendData,
    uint8_t sendLength,
    uint8_t validLastBits,
    uint32_t timeoutMs
)
{
    uint8_t index;

    /*
     * The reader must be initialized before starting
     * contactless communication.
     */
    if (!MFRC522_CoreIsReady())
    {
        return false;
    }

    if (sendData == NULL)
    {
        return false;
    }

    if ((sendLength == 0U)
        || (sendLength > MFRC522_MAX_FRAME_SIZE))
    {
        return false;
    }

    if (validLastBits > 7U)
    {
        return false;
    }

    if (timeoutMs == 0U)
    {
        return false;
    }

    /*
     * Do not start another operation while the previous
     * operation is still active.
     */
    if (transceiveState
        == MFRC522_TRANSCEIVE_WAITING)
    {
        return false;
    }

    transceiveResponseLength = 0U;

    /*
     * Stop any previously running MFRC522 command.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_COMMAND_REG,
        MFRC522_CMD_IDLE
    );

    /*
     * Clear old MFRC522 communication interrupt flags.
     *
     * Writing one clears the corresponding interrupt bit.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_COM_IRQ_REG,
        MFRC522_COM_IRQ_ALL
    );

    /*
     * Clear any previous PIC-side IOC event before
     * starting the new reader operation.
     */
    RFID_IRQ_ClearPendingEvent();

    /*
     * Flush the MFRC522 FIFO.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_FIFO_LEVEL_REG,
        MFRC522_FIFO_LEVEL_FLUSH
    );

    /*
     * Copy the outgoing frame into the MFRC522 FIFO.
     */
    for (index = 0U;
         index < sendLength;
         index++)
    {
        MFRC522_CoreWriteRegister(
            MFRC522_FIFO_DATA_REG,
            sendData[index]
        );
    }

    /*
     * Configure the number of valid bits in the final
     * transmitted byte.
     *
     * RxAlign is cleared because this implementation
     * expects received data to start at bit zero.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_BIT_FRAMING_REG,
        (uint8_t)(
            validLastBits
            & MFRC522_BIT_FRAMING_TX_LAST_BITS
        )
    );

    /*
     * Select Transceive mode.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_COMMAND_REG,
        MFRC522_CMD_TRANSCEIVE
    );

    /*
     * Setting StartSend begins wireless transmission.
     */
    MFRC522_CoreSetRegisterBits(
        MFRC522_BIT_FRAMING_REG,
        MFRC522_BIT_FRAMING_START_SEND
    );

    /*
     * The physical IRQ is the normal completion path.
     *
     * This software deadline prevents the operation from
     * remaining stuck if the IRQ wire, IOC configuration,
     * or reader operation fails.
     */
    transceiveDeadline =
        Timebase_GetMilliseconds()
        + timeoutMs;

    transceiveState =
        MFRC522_TRANSCEIVE_WAITING;

    return true;
}


/*
 * Progress the active Transceive operation.
 *
 * This function checks the IRQ event once and returns.
 * No busy-wait loop is used.
 */
void MFRC522_TransceiveUpdate(void)
{
    uint32_t currentTime;

    uint8_t irqFlags;
    uint8_t errorFlags;
    uint8_t fifoLength;
    uint8_t index;

    bool irqEventPending;

    if (transceiveState
        != MFRC522_TRANSCEIVE_WAITING)
    {
        return;
    }

    currentTime =
        Timebase_GetMilliseconds();

    /*
     * Consume the PIC-side IRQ event, if one occurred.
     */
    irqEventPending =
        RFID_IRQ_TakePendingEvent();

    if (!irqEventPending)
    {
        /*
         * No physical IRQ has occurred yet.
         *
         * Return immediately while the software deadline
         * remains in the future.
         */
        if (!Timebase_DeadlineReached(
                currentTime,
                transceiveDeadline))
        {
            return;
        }

        /*
         * The software deadline has expired.
         *
         * Read ComIrqReg once in case the MFRC522 completed
         * but the physical IRQ edge was lost.
         */
        irqFlags =
            MFRC522_CoreReadRegister(
                MFRC522_COM_IRQ_REG
            );

        /*
         * No relevant MFRC522 event occurred before the
         * software deadline.
         */
        if ((irqFlags
             & MFRC522_COM_IRQ_TRANSCEIVE)
            == 0U)
        {
            MFRC522_TransceiveStopHardware();

            transceiveState =
                MFRC522_TRANSCEIVE_NO_RESPONSE;

            return;
        }
    }
    else
    {
        /*
         * The physical MFRC522 IRQ pin became active.
         *
         * Determine the exact reader event by reading
         * ComIrqReg outside the IOC callback.
         */
        irqFlags =
            MFRC522_CoreReadRegister(
                MFRC522_COM_IRQ_REG
            );
    }

    /*
     * Clear StartSend after the Transceive command has
     * completed or terminated.
     */
    MFRC522_CoreClearRegisterBits(
        MFRC522_BIT_FRAMING_REG,
        MFRC522_BIT_FRAMING_START_SEND
    );

    /*
     * TimerIRq means that no valid tag response arrived
     * before the MFRC522 internal timer expired.
     */
    if ((irqFlags
         & MFRC522_COM_IRQ_TIMER)
        != 0U)
    {
        MFRC522_CoreWriteRegister(
            MFRC522_COM_IRQ_REG,
            MFRC522_COM_IRQ_ALL
        );

        transceiveState =
            MFRC522_TRANSCEIVE_NO_RESPONSE;

        return;
    }

    /*
     * Check the communication-error register.
     */
    errorFlags =
        MFRC522_CoreReadRegister(
            MFRC522_ERROR_REG
        );

    if ((errorFlags
         & MFRC522_ERROR_TRANSCEIVE_MASK)
        != 0U)
    {
        MFRC522_CoreWriteRegister(
            MFRC522_COM_IRQ_REG,
            MFRC522_COM_IRQ_ALL
        );

        transceiveState =
            MFRC522_TRANSCEIVE_ERROR;

        return;
    }

    /*
     * A successful operation must provide received data.
     */
    if ((irqFlags & MFRC522_COM_IRQ_RX) == 0U)
    {
        MFRC522_CoreWriteRegister(
            MFRC522_COM_IRQ_REG,
            MFRC522_COM_IRQ_ALL
        );

        transceiveState =
            MFRC522_TRANSCEIVE_NO_RESPONSE;

        return;
    }

    /*
     * Read the number of received FIFO bytes.
     */
    fifoLength =
        MFRC522_CoreReadRegister(
            MFRC522_FIFO_LEVEL_REG
        );

    fifoLength = (uint8_t)(
        fifoLength
        & MFRC522_FIFO_LEVEL_COUNT_MASK
    );

    if (fifoLength == 0U)
    {
        MFRC522_TransceiveSetError(
            MFRC522_TRANSCEIVE_ERROR
        );

        return;
    }

    if (fifoLength > MFRC522_MAX_FRAME_SIZE)
    {
        MFRC522_TransceiveSetError(
            MFRC522_TRANSCEIVE_BUFFER_ERROR
        );

        return;
    }

    /*
     * Copy the received response from the FIFO into the
     * internal persistent response buffer.
     */
    for (index = 0U;
         index < fifoLength;
         index++)
    {
        transceiveResponse[index] =
            MFRC522_CoreReadRegister(
                MFRC522_FIFO_DATA_REG
            );
    }

    transceiveResponseLength =
        fifoLength;

    /*
     * Clear all MFRC522 interrupt flags.
     *
     * This allows the physical active-low IRQ pin to
     * return to its inactive state.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_COM_IRQ_REG,
        MFRC522_COM_IRQ_ALL
    );

    transceiveState =
        MFRC522_TRANSCEIVE_COMPLETE;
}


/*
 * Return the current Transceive state.
 */
mfrc522_transceive_state_t
MFRC522_TransceiveGetState(void)
{
    return transceiveState;
}


/*
 * Return true while waiting for the MFRC522 operation
 * to finish.
 */
bool MFRC522_TransceiveIsBusy(void)
{
    return (
        transceiveState
        == MFRC522_TRANSCEIVE_WAITING
    );
}


/*
 * Return true when the response buffer contains a valid
 * received frame.
 */
bool MFRC522_TransceiveResponseAvailable(void)
{
    return (
        transceiveState
        == MFRC522_TRANSCEIVE_COMPLETE
    );
}


/*
 * Return the received response length.
 */
uint8_t MFRC522_TransceiveGetResponseLength(void)
{
    if (!MFRC522_TransceiveResponseAvailable())
    {
        return 0U;
    }

    return transceiveResponseLength;
}


/*
 * Copy the received response into a caller-owned buffer.
 */
bool MFRC522_TransceiveCopyResponse(
    uint8_t *destination,
    uint8_t destinationSize
)
{
    uint8_t index;

    if (destination == NULL)
    {
        return false;
    }

    if (!MFRC522_TransceiveResponseAvailable())
    {
        return false;
    }

    if (destinationSize
        < transceiveResponseLength)
    {
        return false;
    }

    for (index = 0U;
         index < transceiveResponseLength;
         index++)
    {
        destination[index] =
            transceiveResponse[index];
    }

    return true;
}


/*
 * Return true if no tag response was received.
 */
bool MFRC522_TransceiveNoResponse(void)
{
    return (
        transceiveState
        == MFRC522_TRANSCEIVE_NO_RESPONSE
    );
}


/*
 * Return true if a communication or buffer error occurred.
 */
bool MFRC522_TransceiveHasError(void)
{
    return (
        (transceiveState
         == MFRC522_TRANSCEIVE_ERROR)
        || (transceiveState
            == MFRC522_TRANSCEIVE_BUFFER_ERROR)
    );
}


/*
 * Reset a completed Transceive operation to IDLE.
 *
 * An active operation is not interrupted by this function.
 */
void MFRC522_TransceiveReset(void)
{
    uint8_t index;

    if (transceiveState
        == MFRC522_TRANSCEIVE_WAITING)
    {
        return;
    }

    transceiveResponseLength = 0U;
    transceiveDeadline = 0U;

    for (index = 0U;
         index < MFRC522_MAX_FRAME_SIZE;
         index++)
    {
        transceiveResponse[index] = 0U;
    }

    transceiveState =
        MFRC522_TRANSCEIVE_IDLE;
}


/*
 * Cancel the active Transceive operation.
 */
void MFRC522_TransceiveCancel(void)
{
    uint8_t index;

    MFRC522_TransceiveStopHardware();

    RFID_IRQ_ClearPendingEvent();

    transceiveResponseLength = 0U;
    transceiveDeadline = 0U;

    for (index = 0U;
         index < MFRC522_MAX_FRAME_SIZE;
         index++)
    {
        transceiveResponse[index] = 0U;
    }

    transceiveState =
        MFRC522_TRANSCEIVE_IDLE;
}


/*
 * Enter a completed error state and clear the associated
 * MFRC522 flags.
 */
static void MFRC522_TransceiveSetError(
    mfrc522_transceive_state_t errorState
)
{
    MFRC522_TransceiveStopHardware();

    transceiveResponseLength = 0U;

    transceiveState =
        errorState;
}


/*
 * Stop Transceive and clear reader-side interrupt state.
 */
static void MFRC522_TransceiveStopHardware(void)
{
    /*
     * Stop the current command.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_COMMAND_REG,
        MFRC522_CMD_IDLE
    );

    /*
     * Clear StartSend.
     */
    MFRC522_CoreClearRegisterBits(
        MFRC522_BIT_FRAMING_REG,
        MFRC522_BIT_FRAMING_START_SEND
    );

    /*
     * Clear all communication interrupt flags.
     */
    MFRC522_CoreWriteRegister(
        MFRC522_COM_IRQ_REG,
        MFRC522_COM_IRQ_ALL
    );
}
