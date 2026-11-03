#include "mfrc522_core.h"

#include "timebase.h"
#include "rfid_irq.h"
#include "mcc_generated_files/system/system.h"

// Current initialization state.
static mfrc522_init_state_t initializationState = MFRC522_INIT_NOT_STARTED;

// Deadline used by the non-blocking reset sequence.
static uint32_t initializationDeadline = 0U;

// Version value read after initialization.
static uint8_t detectedVersion = 0U;

// Write one byte to an MFRC522 register.
void MFRC522_CoreWriteRegister(
    uint8_t registerAddress,
    uint8_t value
)
{
    uint8_t addressByte;

    // MFRC522 SPI write-address
    addressByte = (uint8_t)(
        ((uint16_t)registerAddress << 1U)
        & 0x7EU
    );

    // The MFRC522 and nRF24 share the SPI bus. The nRF24 is deselected before selecting the MFRC522.
    nRF_CSN_SetHigh();

    RFID_CS_SetLow();

    (void)SPI1_ByteExchange(addressByte);
    (void)SPI1_ByteExchange(value);

    RFID_CS_SetHigh();
}


// Read one byte from an MFRC522 register.
uint8_t MFRC522_CoreReadRegister(
    uint8_t registerAddress
)
{
    uint8_t addressByte;
    uint8_t registerValue;

    // MFRC522 SPI read-address format:
    addressByte = (uint8_t)(
        (
            ((uint16_t)registerAddress << 1U)
            & 0x7EU
        )
        | 0x80U
    );

    // The MFRC522 and nRF24 share the SPI bus. The nRF24 is deselected before selecting the MFRC522.
    nRF_CSN_SetHigh();

    RFID_CS_SetLow();

    // Tell the MFRC522 which register should be read.
    (void)SPI1_ByteExchange(addressByte);

    // The dummy byte generates eight SPI clock pulses.
    // The requested register value is returned over MISO.
    registerValue = SPI1_ByteExchange(0x00U);

    RFID_CS_SetHigh();

    return registerValue;
}


// Set selected bits in an MFRC522 register while preserving all other register bits.
void MFRC522_CoreSetRegisterBits(
    uint8_t registerAddress,
    uint8_t mask
)
{
    uint8_t registerValue;

    registerValue = MFRC522_CoreReadRegister(
        registerAddress
    );

    registerValue = (uint8_t)(
        registerValue | mask
    );

    MFRC522_CoreWriteRegister(
        registerAddress,
        registerValue
    );
}


// Clear selected bits in an MFRC522 register while preserving all other register bits.
void MFRC522_CoreClearRegisterBits(
    uint8_t registerAddress,
    uint8_t mask
)
{
    uint8_t registerValue;

    registerValue = MFRC522_CoreReadRegister(
        registerAddress
    );

    registerValue = (uint8_t)(
        registerValue
        & (uint8_t)(~mask)
    );

    MFRC522_CoreWriteRegister(
        registerAddress,
        registerValue
    );
}


/*
 Read the MFRC522 VersionReg register.
 0x91 = MFRC522 version 1.0
 0x92 = MFRC522 version 2.0
 0x00 or 0xFF = power, RST, CS, SPI or wiring problem.
 */
uint8_t MFRC522_CoreGetVersion(void)
{
    return MFRC522_CoreReadRegister(
        MFRC522_VERSION_REG
    );
}


// Return the version that was detected during the most recent initialization attempt.
uint8_t MFRC522_CoreGetDetectedVersion(void)
{
    return detectedVersion;
}


// Enable both MFRC522 antenna transmitter outputs.
void MFRC522_CoreAntennaOn(void)
{
    uint8_t registerValue;

    registerValue = MFRC522_CoreReadRegister(
        MFRC522_TX_CONTROL_REG
    );

    if ((registerValue
         & MFRC522_TX_CONTROL_ANTENNA_MASK)
        != MFRC522_TX_CONTROL_ANTENNA_MASK)
    {
        MFRC522_CoreSetRegisterBits(
            MFRC522_TX_CONTROL_REG,
            MFRC522_TX_CONTROL_ANTENNA_MASK
        );
    }
}


// Disable both MFRC522 antenna transmitter outputs.
void MFRC522_CoreAntennaOff(void)
{
    MFRC522_CoreClearRegisterBits(
        MFRC522_TX_CONTROL_REG,
        MFRC522_TX_CONTROL_ANTENNA_MASK
    );
}


/*
 Start the non-blocking initialization sequence.
 
 Initialization may be started from:
    MFRC522_INIT_NOT_STARTED
    MFRC522_INIT_ERROR
 
 Calling this function while initialization is already running or completed has no effect.
*/
void MFRC522_CoreInitializeStart(void)
{
    if ((initializationState != MFRC522_INIT_NOT_STARTED)
        && (initializationState != MFRC522_INIT_ERROR))
    {
        return;
    }

    detectedVersion = 0U;

    // Deselect both SPI devices.
    RFID_CS_SetHigh();
    nRF_CSN_SetHigh();

    // Assert the MFRC522 hardware reset.
    RFID_RST_SetLow();

    // Store the time at which reset may be released.
    initializationDeadline = Timebase_GetMilliseconds() + MFRC522_HARD_RESET_LOW_MS;

    initializationState = MFRC522_INIT_HARD_RESET_LOW;
}


/*
 Progress the non-blocking initialization sequence.
 
 Call this function repeatedly from the main scheduler.
 The function performs at most the work required by the current state and then returns.
*/
void MFRC522_CoreInitializeUpdate(void)
{
    uint32_t currentTime;

    currentTime =
        Timebase_GetMilliseconds();

    switch (initializationState)
    {
        case MFRC522_INIT_NOT_STARTED:
            // Initialization has not been requested.
            break;

        case MFRC522_INIT_HARD_RESET_LOW:
            // Keep RST LOW until the hardware-reset duration has elapsed.
            if (Timebase_DeadlineReached(
                    currentTime,
                    initializationDeadline))
            {
                RFID_RST_SetHigh();

                initializationDeadline = currentTime + MFRC522_HARD_RESET_WAIT_MS;

                initializationState = MFRC522_INIT_HARD_RESET_WAIT;
            }
            break;

        case MFRC522_INIT_HARD_RESET_WAIT:
            // Allow the MFRC522 to start after the hardware reset is released.
            if (Timebase_DeadlineReached(
                    currentTime,
                    initializationDeadline))
            {
                initializationState = MFRC522_INIT_SOFT_RESET_START;
            }
            break;

        case MFRC522_INIT_SOFT_RESET_START:
            // Request an internal MFRC522 software reset.
            MFRC522_CoreWriteRegister(
                MFRC522_COMMAND_REG,
                MFRC522_CMD_SOFT_RESET
            );

            initializationDeadline = currentTime + MFRC522_SOFT_RESET_WAIT_MS;

            initializationState = MFRC522_INIT_SOFT_RESET_WAIT;
            break;

        case MFRC522_INIT_SOFT_RESET_WAIT:
            // Wait for the internal reset to finish.
            if (Timebase_DeadlineReached(
                    currentTime,
                    initializationDeadline))
            {
                initializationState = MFRC522_INIT_CONFIGURE;
            }
            break;

        case MFRC522_INIT_CONFIGURE:
            // Configure the MFRC522 internal timer used for contactless communication timeouts.
            MFRC522_CoreWriteRegister(
                MFRC522_T_MODE_REG,
                MFRC522_T_MODE_DEFAULT
            );

            MFRC522_CoreWriteRegister(
                MFRC522_T_PRESCALER_REG,
                MFRC522_T_PRESCALER_DEFAULT
            );

            MFRC522_CoreWriteRegister(
                MFRC522_T_RELOAD_H_REG,
                MFRC522_T_RELOAD_H_DEFAULT
            );

            MFRC522_CoreWriteRegister(
                MFRC522_T_RELOAD_L_REG,
                MFRC522_T_RELOAD_L_DEFAULT
            );

            // Force 100 percent ASK modulation.
            MFRC522_CoreWriteRegister(
                MFRC522_TX_ASK_REG,
                MFRC522_TX_ASK_FORCE_100_ASK
            );

            // Configure the ISO/IEC 14443A CRC preset.
            MFRC522_CoreWriteRegister(
                MFRC522_MODE_REG,
                MFRC522_MODE_ISO14443A
            );

            // Configure the MFRC522 physical IRQ output as a push-pull output.
            MFRC522_CoreWriteRegister(
                MFRC522_DIV_I_EN_REG,
                MFRC522_DIV_I_EN_IRQ_PUSH_PULL
            );

            /*
            Configure active-low IRQ operation.
            Enable:
                RxIRq
                IdleIRq
                ErrIRq
                TimerIRq
            */
            MFRC522_CoreWriteRegister(
                MFRC522_COM_I_EN_REG,
                MFRC522_COM_I_EN_TRANSCEIVE
            );

            // Clear all old MFRC522 communication interrupt flags.
            MFRC522_CoreWriteRegister(
                MFRC522_COM_IRQ_REG,
                MFRC522_COM_IRQ_ALL
            );

            // Clear any old PIC-side IOC event.
            RFID_IRQ_ClearPendingEvent();

            // Enable the reader antenna.
            MFRC522_CoreAntennaOn();

            // Verify SPI communication by reading VersionReg.
            detectedVersion = MFRC522_CoreGetVersion();

            if ((detectedVersion == 0x00U) || (detectedVersion == 0xFFU))
            {
                initializationState = MFRC522_INIT_ERROR;
            }
            else
            {
                initializationState = MFRC522_INIT_READY;
            }
            break;

        case MFRC522_INIT_READY:
            // Initialization completed successfully.
            break;

        case MFRC522_INIT_ERROR:
            // The application may retry by calling MFRC522_CoreInitializeStart().
            break;

        default:
            initializationState = MFRC522_INIT_ERROR;
            break;
    }
}


// Return true after successful initialization.
bool MFRC522_CoreIsReady(void)
{
    return (
        initializationState == MFRC522_INIT_READY
    );
}


// Return true if initialization failed.
bool MFRC522_CoreInitializationFailed(void)
{
    return (
        initializationState == MFRC522_INIT_ERROR
    );
}


// Return the current initialization state.
mfrc522_init_state_t
MFRC522_CoreGetInitializationState(void)
{
    return initializationState;
}
