#include "nrf24_test.h"
#include "mcc_generated_files/system/system.h"

#define NRF24_CMD_R_REGISTER     0x00U
#define NRF24_CMD_W_REGISTER     0x20U
#define NRF24_CMD_NOP            0xFFU

#define NRF24_REGISTER_MASK      0x1FU
#define NRF24_REG_RF_CH          0x05U

#define NRF24_TEST_CHANNEL       42U

uint8_t NRF24_TestReadRegister(
    uint8_t registerAddress
)
{
    uint8_t value;

    /*
     * Ensure the MFRC522 is not selected.
     */
    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    (void)SPI1_ByteExchange(
        (uint8_t)(
            NRF24_CMD_R_REGISTER
            | (registerAddress
               & NRF24_REGISTER_MASK)
        )
    );

    value = SPI1_ByteExchange(
        NRF24_CMD_NOP
    );

    nRF_CSN_SetHigh();

    return value;
}

void NRF24_TestWriteRegister(
    uint8_t registerAddress,
    uint8_t value
)
{
    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    (void)SPI1_ByteExchange(
        (uint8_t)(
            NRF24_CMD_W_REGISTER
            | (registerAddress
               & NRF24_REGISTER_MASK)
        )
    );

    (void)SPI1_ByteExchange(value);

    nRF_CSN_SetHigh();
}

uint8_t NRF24_TestGetStatus(void)
{
    uint8_t status;

    RFID_CS_SetHigh();

    nRF_CSN_SetLow();

    status = SPI1_ByteExchange(
        NRF24_CMD_NOP
    );

    nRF_CSN_SetHigh();

    return status;
}

bool NRF24_TestSpi(void)
{
    uint8_t originalChannel;
    uint8_t readbackChannel;

    originalChannel =
        NRF24_TestReadRegister(
            NRF24_REG_RF_CH
        );

    NRF24_TestWriteRegister(
        NRF24_REG_RF_CH,
        NRF24_TEST_CHANNEL
    );

    readbackChannel =
        NRF24_TestReadRegister(
            NRF24_REG_RF_CH
        );

    /*
     * Restore the original channel.
     */
    NRF24_TestWriteRegister(
        NRF24_REG_RF_CH,
        originalChannel
    );

    return (
        readbackChannel
        == NRF24_TEST_CHANNEL
    );
}
