#include <stddef.h>

#include "nRF24.h"
#include "nRF24_irq.h"
#include "timebase.h"
#include "mcc_generated_files/system/system.h"

/*
 * Current radio state.
 */
static nrf24_state_t radioState =
    NRF24_STATE_UNINITIALIZED;

/*
 * Power-up or transmission deadline.
 */
static uint32_t radioDeadline = 0U;

/*
 * Fixed wireless address.
 *
 * The ESP32 receiver must use the same five bytes
 * for receive pipe 0.
 */
static const uint8_t transmitAddress[
    NRF24_ADDRESS_WIDTH
] =
{
    'P', 'I', 'C', '0', '1'
};

/*
 * Write a raw SPI command and return the STATUS byte.
 */
static uint8_t NRF24_ExecuteCommand(
    uint8_t command
)
{
    uint8_t status;

    /*
     * Ensure the MFRC522 is deselected from the
     * shared SPI bus.
     */
    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    status =
        SPI1_ByteExchange(command);

    nRF_CSN_SetHigh();

    return status;
}

/*
 * Write one payload into the TX FIFO.
 */
static bool NRF24_WritePayload(
    const uint8_t *payload,
    uint8_t length
)
{
    uint8_t index;
    uint8_t status;

    if (payload == NULL)
    {
        return false;
    }

    if ((length == 0U) ||
        (length > NRF24_PAYLOAD_SIZE))
    {
        return false;
    }

    /*
     * Deselect the RFID reader before using the
     * shared SPI bus.
     */
    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    status =
        SPI1_ByteExchange(
            NRF24_CMD_W_TX_PAYLOAD
        );

    /*
     * Do not write another payload when the TX FIFO
     * is already full.
     */
    if ((status & NRF24_STATUS_TX_FULL) != 0U)
    {
        nRF_CSN_SetHigh();

        return false;
    }

    for (index = 0U;
         index < length;
         index++)
    {
        (void)SPI1_ByteExchange(
            payload[index]
        );
    }

    /*
     * The radio uses a fixed four-byte payload.
     * Fill any missing bytes with zero.
     */
    for (index = length;
         index < NRF24_PAYLOAD_SIZE;
         index++)
    {
        (void)SPI1_ByteExchange(0x00U);
    }

    nRF_CSN_SetHigh();

    return true;
}

/*
 * Read one nRF24 register.
 */
uint8_t NRF24_ReadRegister(
    uint8_t registerAddress
)
{
    uint8_t value;

    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    (void)SPI1_ByteExchange(
        (uint8_t)(
            NRF24_CMD_R_REGISTER |
            (registerAddress &
             NRF24_REGISTER_MASK)
        )
    );

    value =
        SPI1_ByteExchange(
            NRF24_CMD_NOP
        );

    nRF_CSN_SetHigh();

    return value;
}

/*
 * Write one nRF24 register.
 */
void NRF24_WriteRegister(
    uint8_t registerAddress,
    uint8_t value
)
{
    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    (void)SPI1_ByteExchange(
        (uint8_t)(
            NRF24_CMD_W_REGISTER |
            (registerAddress &
             NRF24_REGISTER_MASK)
        )
    );

    (void)SPI1_ByteExchange(value);

    nRF_CSN_SetHigh();
}

/*
 * Write several bytes to an nRF24 register.
 *
 * Used for TX_ADDR and RX_ADDR_P0.
 */
void NRF24_WriteRegisterBuffer(
    uint8_t registerAddress,
    const uint8_t *data,
    uint8_t length
)
{
    uint8_t index;

    if ((data == NULL) ||
        (length == 0U))
    {
        return;
    }

    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    (void)SPI1_ByteExchange(
        (uint8_t)(
            NRF24_CMD_W_REGISTER |
            (registerAddress &
             NRF24_REGISTER_MASK)
        )
    );

    for (index = 0U;
         index < length;
         index++)
    {
        (void)SPI1_ByteExchange(
            data[index]
        );
    }

    nRF_CSN_SetHigh();
}

/*
 * Read the current STATUS register.
 *
 * Every SPI command returns STATUS as its first byte.
 */
uint8_t NRF24_GetStatus(void)
{
    uint8_t status;

    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    status =
        SPI1_ByteExchange(
            NRF24_CMD_NOP
        );

    nRF_CSN_SetHigh();

    return status;
}

/*
 * Clear the TX FIFO.
 */
void NRF24_FlushTx(void)
{
    (void)NRF24_ExecuteCommand(
        NRF24_CMD_FLUSH_TX
    );
}

/*
 * Clear the RX FIFO.
 */
void NRF24_FlushRx(void)
{
    (void)NRF24_ExecuteCommand(
        NRF24_CMD_FLUSH_RX
    );
}

/*
 * Clear selected IRQ flags.
 *
 * STATUS IRQ bits are cleared by writing one.
 */
void NRF24_ClearInterruptFlags(
    uint8_t flags
)
{
    NRF24_WriteRegister(
        NRF24_REG_STATUS,
        (uint8_t)(
            flags &
            NRF24_STATUS_IRQ_FLAGS
        )
    );
}

/*
 * Configure the radio as a primary transmitter.
 */
void NRF24_InitializeStart(void)
{
    uint8_t channelReadback;

    /*
     * Keep CE LOW during configuration.
     */
    nRF_CE_SetLow();
    nRF_CSN_SetHigh();

    /*
     * Deselect the RFID reader from the shared SPI bus.
     */
    RFID_CS_SetHigh();

    NRF24_IRQ_ClearPendingEvent();

    /*
     * Power down while configuring.
     *
     * Enable CRC with a two-byte CRC.
     */
    NRF24_WriteRegister(
        NRF24_REG_CONFIG,
        (uint8_t)(
            NRF24_CONFIG_EN_CRC |
            NRF24_CONFIG_CRCO
        )
    );

    /*
     * Enable automatic acknowledgement on pipe 0.
     */
    NRF24_WriteRegister(
        NRF24_REG_EN_AA,
        NRF24_EN_AA_P0
    );

    /*
     * Enable receive pipe 0 because automatic
     * acknowledgement uses pipe 0.
     */
    NRF24_WriteRegister(
        NRF24_REG_EN_RXADDR,
        NRF24_EN_RXADDR_P0
    );

    /*
     * Use five-byte addresses.
     */
    NRF24_WriteRegister(
        NRF24_REG_SETUP_AW,
        NRF24_SETUP_AW_5_BYTES
    );

    /*
     * Configure automatic retries.
     */
    NRF24_WriteRegister(
        NRF24_REG_SETUP_RETR,
        NRF24_SETUP_RETR_DEFAULT
    );

    /*
     * Use radio channel 42.
     */
    NRF24_WriteRegister(
        NRF24_REG_RF_CH,
        NRF24_CHANNEL
    );

    /*
     * Use 1 Mbps and 0 dBm.
     */
    NRF24_WriteRegister(
        NRF24_REG_RF_SETUP,
        NRF24_RF_SETUP_1MBPS_0DBM
    );

    /*
     * The TX address and pipe-0 RX address must
     * match when automatic acknowledgement is used.
     */
    NRF24_WriteRegisterBuffer(
        NRF24_REG_TX_ADDR,
        transmitAddress,
        NRF24_ADDRESS_WIDTH
    );

    NRF24_WriteRegisterBuffer(
        NRF24_REG_RX_ADDR_P0,
        transmitAddress,
        NRF24_ADDRESS_WIDTH
    );

    /*
     * Configure the fixed payload size.
     */
    NRF24_WriteRegister(
        NRF24_REG_RX_PW_P0,
        NRF24_PAYLOAD_SIZE
    );

    /*
     * Disable dynamic payloads.
     */
    NRF24_WriteRegister(
        NRF24_REG_DYNPD,
        0x00U
    );

    NRF24_WriteRegister(
        NRF24_REG_FEATURE,
        0x00U
    );

    /*
     * Remove old packets and clear old IRQ flags.
     */
    NRF24_FlushTx();
    NRF24_FlushRx();

    NRF24_ClearInterruptFlags(
        NRF24_STATUS_IRQ_FLAGS
    );

    /*
     * Verify one writable register to confirm that
     * PIC-to-nRF24 SPI communication works.
     */
    channelReadback =
        NRF24_ReadRegister(
            NRF24_REG_RF_CH
        );

    if (channelReadback != NRF24_CHANNEL)
    {
        radioState =
            NRF24_STATE_ERROR;

        return;
    }

    /*
     * Power up in primary-transmitter mode.
     */
    NRF24_WriteRegister(
        NRF24_REG_CONFIG,
        NRF24_CONFIG_PTX
    );

    radioDeadline =
        Timebase_GetMilliseconds() +
        NRF24_POWER_UP_WAIT_MS;

    radioState =
        NRF24_STATE_POWER_UP_WAIT;
}

/*
 * Begin one asynchronous payload transmission.
 */
bool NRF24_SendStart(
    const uint8_t *payload,
    uint8_t length
)
{
    if (radioState != NRF24_STATE_READY)
    {
        return false;
    }

    if (payload == NULL)
    {
        return false;
    }

    if ((length == 0U) ||
        (length > NRF24_PAYLOAD_SIZE))
    {
        return false;
    }

    /*
     * CE must be LOW while configuring PTX and
     * loading the TX FIFO.
     */
    nRF_CE_SetLow();

    /*
     * Ensure PTX mode and power-up remain selected.
     */
    NRF24_WriteRegister(
        NRF24_REG_CONFIG,
        NRF24_CONFIG_PTX
    );

    NRF24_IRQ_ClearPendingEvent();

    NRF24_ClearInterruptFlags(
        NRF24_STATUS_IRQ_FLAGS
    );

    NRF24_FlushTx();

    if (!NRF24_WritePayload(
            payload,
            length))
    {
        radioState =
            NRF24_STATE_ERROR;

        return false;
    }

    /*
     * A CE pulse of at least 10 microseconds starts
     * the transmission.
     *
     * This is a short bounded hardware timing pulse,
     * not a wait for an external event.
     */
    nRF_CE_SetHigh();

    __delay_us(15);

    nRF_CE_SetLow();

    radioDeadline =
        Timebase_GetMilliseconds() +
        NRF24_TX_TIMEOUT_MS;

    radioState =
        NRF24_STATE_TX_WAIT;

    return true;
}

/*
 * Progress radio initialization and transmission.
 *
 * This function must be called repeatedly from main.
 */
void NRF24_Update(void)
{
    uint32_t currentTime;
    uint8_t status;

    currentTime =
        Timebase_GetMilliseconds();

    switch (radioState)
    {
        case NRF24_STATE_UNINITIALIZED:
            break;

        case NRF24_STATE_POWER_UP_WAIT:
            if (Timebase_DeadlineReached(
                    currentTime,
                    radioDeadline))
            {
                radioState =
                    NRF24_STATE_READY;
            }
            break;

        case NRF24_STATE_READY:
            break;

        case NRF24_STATE_TX_WAIT:
            /*
             * Normally wait for the active-low IRQ event.
             */
            if (!NRF24_IRQ_TakePendingEvent())
            {
                /*
                 * Apply a software timeout in case the IRQ
                 * connection or interrupt configuration fails.
                 */
                if (!Timebase_DeadlineReached(
                        currentTime,
                        radioDeadline))
                {
                    return;
                }
            }

            status =
                NRF24_GetStatus();

            /*
             * TX_DS indicates that the packet was transmitted
             * and acknowledged by the receiver.
             */
            if ((status &
                 NRF24_STATUS_TX_DS) != 0U)
            {
                NRF24_ClearInterruptFlags(
                    NRF24_STATUS_TX_DS
                );

                radioState =
                    NRF24_STATE_TX_SUCCESS;

                return;
            }

            /*
             * MAX_RT indicates that the automatic retry limit
             * was reached without receiving an acknowledgement.
             */
            if ((status &
                 NRF24_STATUS_MAX_RT) != 0U)
            {
                NRF24_ClearInterruptFlags(
                    NRF24_STATUS_MAX_RT
                );

                /*
                 * MAX_RT does not remove the payload from
                 * the TX FIFO, so flush the FIFO.
                 */
                NRF24_FlushTx();

                radioState =
                    NRF24_STATE_TX_MAX_RETRIES;

                return;
            }

            /*
             * The software timeout expired without a useful
             * radio result.
             */
            if (Timebase_DeadlineReached(
                    currentTime,
                    radioDeadline))
            {
                NRF24_FlushTx();

                NRF24_ClearInterruptFlags(
                    NRF24_STATUS_IRQ_FLAGS
                );

                radioState =
                    NRF24_STATE_ERROR;
            }
            break;

        case NRF24_STATE_TX_SUCCESS:
            break;

        case NRF24_STATE_TX_MAX_RETRIES:
            break;

        case NRF24_STATE_ERROR:
            break;

        default:
            radioState =
                NRF24_STATE_ERROR;
            break;
    }
}

/*
 * Return true when initialization is complete and
 * the radio is able to start a transmission.
 */
bool NRF24_IsReady(void)
{
    return (
        radioState ==
        NRF24_STATE_READY
    );
}

/*
 * Return true after an acknowledged transmission.
 */
bool NRF24_SendSucceeded(void)
{
    return (
        radioState ==
        NRF24_STATE_TX_SUCCESS
    );
}

/*
 * Return true after the automatic retry limit.
 *
 * This usually means that no receiver acknowledgement
 * was received.
 */
bool NRF24_SendMaxRetries(void)
{
    return (
        radioState ==
        NRF24_STATE_TX_MAX_RETRIES
    );
}

/*
 * Return true after a local driver, SPI, or timeout error.
 */
bool NRF24_HasError(void)
{
    return (
        radioState ==
        NRF24_STATE_ERROR
    );
}

/*
 * Clear a completed transmission result and return
 * the radio to READY.
 */
void NRF24_ClearResult(void)
{
    if ((radioState ==
         NRF24_STATE_TX_SUCCESS) ||
        (radioState ==
         NRF24_STATE_TX_MAX_RETRIES))
    {
        radioState =
            NRF24_STATE_READY;
    }
}

/*
 * Return the current internal radio state.
 */
nrf24_state_t NRF24_GetState(void)
{
    return radioState;
}