/* 
 * File:   mfrc522_registers.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 12:54 AM
 */

#ifndef MFRC522_REGISTERS_H
#define	MFRC522_REGISTERS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>

// Page 0: Command and status registers
#define MFRC522_COMMAND_REG              0x01U
#define MFRC522_COM_I_EN_REG             0x02U
#define MFRC522_DIV_I_EN_REG             0x03U
#define MFRC522_COM_IRQ_REG              0x04U
#define MFRC522_DIV_IRQ_REG              0x05U
#define MFRC522_ERROR_REG                0x06U
#define MFRC522_STATUS1_REG              0x07U
#define MFRC522_STATUS2_REG              0x08U
#define MFRC522_FIFO_DATA_REG            0x09U
#define MFRC522_FIFO_LEVEL_REG           0x0AU
#define MFRC522_WATER_LEVEL_REG          0x0BU
#define MFRC522_CONTROL_REG              0x0CU
#define MFRC522_BIT_FRAMING_REG          0x0DU
#define MFRC522_COLL_REG                 0x0EU

// Page 1: Communication configuration registers
#define MFRC522_MODE_REG                 0x11U
#define MFRC522_TX_MODE_REG              0x12U
#define MFRC522_RX_MODE_REG              0x13U
#define MFRC522_TX_CONTROL_REG           0x14U
#define MFRC522_TX_ASK_REG               0x15U
#define MFRC522_TX_SEL_REG               0x16U
#define MFRC522_RX_SEL_REG               0x17U
#define MFRC522_RX_THRESHOLD_REG         0x18U
#define MFRC522_DEMOD_REG                0x19U
#define MFRC522_MF_TX_REG                0x1CU
#define MFRC522_MF_RX_REG                0x1DU
#define MFRC522_SERIAL_SPEED_REG         0x1FU

// Page 2: CRC, modulation and timer registers
#define MFRC522_CRC_RESULT_H_REG         0x21U
#define MFRC522_CRC_RESULT_L_REG         0x22U
#define MFRC522_MOD_WIDTH_REG            0x24U
#define MFRC522_RF_CFG_REG               0x26U
#define MFRC522_GS_N_REG                 0x27U
#define MFRC522_CW_GS_P_REG              0x28U
#define MFRC522_MOD_GS_P_REG             0x29U
#define MFRC522_T_MODE_REG               0x2AU
#define MFRC522_T_PRESCALER_REG          0x2BU
#define MFRC522_T_RELOAD_H_REG           0x2CU
#define MFRC522_T_RELOAD_L_REG           0x2DU
#define MFRC522_T_COUNTER_VALUE_H_REG    0x2EU
#define MFRC522_T_COUNTER_VALUE_L_REG    0x2FU

// Page 3: Test and version registers
#define MFRC522_TEST_SEL1_REG            0x31U
#define MFRC522_TEST_SEL2_REG            0x32U
#define MFRC522_TEST_PIN_EN_REG          0x33U
#define MFRC522_TEST_PIN_VALUE_REG       0x34U
#define MFRC522_TEST_BUS_REG             0x35U
#define MFRC522_AUTO_TEST_REG            0x36U
#define MFRC522_VERSION_REG              0x37U
#define MFRC522_ANALOG_TEST_REG          0x38U
#define MFRC522_TEST_DAC1_REG            0x39U
#define MFRC522_TEST_DAC2_REG            0x3AU
#define MFRC522_TEST_ADC_REG             0x3BU


// Commands written to CommandReg
#define MFRC522_CMD_IDLE                 0x00U
#define MFRC522_CMD_MEM                  0x01U
#define MFRC522_CMD_GENERATE_RANDOM_ID   0x02U
#define MFRC522_CMD_CALC_CRC             0x03U
#define MFRC522_CMD_TRANSMIT             0x04U
#define MFRC522_CMD_NO_CMD_CHANGE        0x07U
#define MFRC522_CMD_RECEIVE              0x08U
#define MFRC522_CMD_TRANSCEIVE           0x0CU
#define MFRC522_CMD_MF_AUTHENT           0x0EU
#define MFRC522_CMD_SOFT_RESET           0x0FU


// ISO/IEC 14443A PICC commands
#define PICC_CMD_REQA                    0x26U
#define PICC_CMD_WUPA                    0x52U

#define PICC_CMD_ANTICOLL_CL1            0x93U
#define PICC_CMD_ANTICOLL_CL2            0x95U
#define PICC_CMD_ANTICOLL_CL3            0x97U

#define PICC_CMD_SELECT                  0x70U
#define PICC_CMD_HLTA                    0x50U

#define PICC_CASCADE_TAG                 0x88U


// ComIEnReg masks
#define MFRC522_COM_I_EN_IRQ_INVERT      0x80U
#define MFRC522_COM_I_EN_TX              0x40U
#define MFRC522_COM_I_EN_RX              0x20U
#define MFRC522_COM_I_EN_IDLE            0x10U
#define MFRC522_COM_I_EN_HI_ALERT        0x08U
#define MFRC522_COM_I_EN_LO_ALERT        0x04U
#define MFRC522_COM_I_EN_ERROR           0x02U
#define MFRC522_COM_I_EN_TIMER           0x01U

#define MFRC522_COM_I_EN_TRANSCEIVE      \
    ((uint8_t)(                          \
        MFRC522_COM_I_EN_IRQ_INVERT      \
        | MFRC522_COM_I_EN_RX            \
        | MFRC522_COM_I_EN_IDLE          \
        | MFRC522_COM_I_EN_ERROR         \
        | MFRC522_COM_I_EN_TIMER         \
    ))


// DivIEnReg masks
#define MFRC522_DIV_I_EN_IRQ_PUSH_PULL   0x80U
#define MFRC522_DIV_I_EN_MFIN_ACT        0x10U
#define MFRC522_DIV_I_EN_CRC             0x04U


// ComIrqReg masks
#define MFRC522_COM_IRQ_SET1             0x80U
#define MFRC522_COM_IRQ_TX               0x40U
#define MFRC522_COM_IRQ_RX               0x20U
#define MFRC522_COM_IRQ_IDLE             0x10U
#define MFRC522_COM_IRQ_HI_ALERT         0x08U
#define MFRC522_COM_IRQ_LO_ALERT         0x04U
#define MFRC522_COM_IRQ_ERROR            0x02U
#define MFRC522_COM_IRQ_TIMER            0x01U

#define MFRC522_COM_IRQ_ALL              0x7FU

#define MFRC522_COM_IRQ_TRANSCEIVE       \
    ((uint8_t)(                          \
        MFRC522_COM_IRQ_RX               \
        | MFRC522_COM_IRQ_IDLE           \
        | MFRC522_COM_IRQ_ERROR          \
        | MFRC522_COM_IRQ_TIMER          \
    ))


// DivIrqReg masks
#define MFRC522_DIV_IRQ_SET2             0x80U
#define MFRC522_DIV_IRQ_MFIN_ACT         0x10U
#define MFRC522_DIV_IRQ_CRC              0x04U


// ErrorReg masks
#define MFRC522_ERROR_WRITE              0x80U
#define MFRC522_ERROR_TEMP               0x40U
#define MFRC522_ERROR_BUFFER_OVERFLOW    0x10U
#define MFRC522_ERROR_COLLISION          0x08U
#define MFRC522_ERROR_CRC                0x04U
#define MFRC522_ERROR_PARITY             0x02U
#define MFRC522_ERROR_PROTOCOL           0x01U

// Errors treated as fatal during the current UID-reading implementation.
#define MFRC522_ERROR_TRANSCEIVE_MASK    \
    ((uint8_t)(                          \
        MFRC522_ERROR_BUFFER_OVERFLOW    \
        | MFRC522_ERROR_COLLISION        \
        | MFRC522_ERROR_PARITY           \
        | MFRC522_ERROR_PROTOCOL         \
    ))


// FIFOLevelReg masks
#define MFRC522_FIFO_LEVEL_FLUSH         0x80U
#define MFRC522_FIFO_LEVEL_COUNT_MASK    0x7FU


// ControlReg masks
#define MFRC522_CONTROL_RX_LAST_BITS     0x07U


// BitFramingReg masks
#define MFRC522_BIT_FRAMING_START_SEND   0x80U
#define MFRC522_BIT_FRAMING_RX_ALIGN     0x70U
#define MFRC522_BIT_FRAMING_TX_LAST_BITS 0x07U

// REQA is transmitted as a seven-bit command.
#define MFRC522_REQA_VALID_BITS          0x07U

// A value of zero means that the final byte contains all eight valid bits.
#define MFRC522_FULL_BYTE_VALID_BITS     0x00U


// CollReg masks
#define MFRC522_COLL_VALUES_AFTER_COLL   0x80U
#define MFRC522_COLL_POSITION_NOT_VALID  0x20U
#define MFRC522_COLL_POSITION_MASK       0x1FU


// Status2Reg masks
#define MFRC522_STATUS2_TEMP_SENS_CLEAR  0x80U
#define MFRC522_STATUS2_I2C_FORCE_HS     0x40U
#define MFRC522_STATUS2_MF_CRYPTO1_ON    0x08U
#define MFRC522_STATUS2_MODEM_STATE_MASK 0x07U


// TxControlReg masks
#define MFRC522_TX_CONTROL_TX2_RF_EN     0x02U
#define MFRC522_TX_CONTROL_TX1_RF_EN     0x01U

#define MFRC522_TX_CONTROL_ANTENNA_MASK  \
    ((uint8_t)(                          \
        MFRC522_TX_CONTROL_TX2_RF_EN     \
        | MFRC522_TX_CONTROL_TX1_RF_EN   \
    ))


// TxASKReg masks
#define MFRC522_TX_ASK_FORCE_100_ASK     0x40U


// ModeReg values and masks
#define MFRC522_MODE_MSB_FIRST           0x80U
#define MFRC522_MODE_TX_WAIT_RF          0x20U
#define MFRC522_MODE_POL_MFIN            0x08U
#define MFRC522_MODE_CRC_PRESET_MASK     0x03U

// Common ISO/IEC 14443A configuration value.
#define MFRC522_MODE_ISO14443A           0x3DU


// Timer configuration values
#define MFRC522_T_MODE_DEFAULT           0x80U
#define MFRC522_T_PRESCALER_DEFAULT      0xA9U
#define MFRC522_T_RELOAD_H_DEFAULT       0x03U
#define MFRC522_T_RELOAD_L_DEFAULT       0xE8U


// UID and frame limits
#define MFRC522_UID_MAX_LENGTH           10U
#define MFRC522_MAX_FRAME_SIZE           18U
#define MFRC522_ATQA_LENGTH              2U
#define MFRC522_ANTICOLLISION_LENGTH     5U
#define MFRC522_SELECT_FRAME_LENGTH      9U
#define MFRC522_SAK_RESPONSE_LENGTH      3U
#define MFRC522_UID_CL1_PART_LENGTH      3U
#define MFRC522_UID_CL2_PART_LENGTH      4U
#define MFRC522_UID_SINGLE_LENGTH        4U
#define MFRC522_UID_DOUBLE_LENGTH        7U
#define MFRC522_UID_TRIPLE_LENGTH        10U


// SAK masks
#define PICC_SAK_CASCADE_BIT             0x04U


// Driver timing values
#define MFRC522_HARD_RESET_LOW_MS        5U
#define MFRC522_HARD_RESET_WAIT_MS       50U
#define MFRC522_SOFT_RESET_WAIT_MS       50U
#define MFRC522_TRANSCEIVE_TIMEOUT_MS    30U

// CRC_A configuration
#define MFRC522_CRC_A_INITIAL_VALUE      0x6363U

#endif /* MFRC522_REGISTERS_H */

