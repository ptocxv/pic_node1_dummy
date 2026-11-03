/* 
 * File:   nRF24_registers.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 8:12 PM
 */

#ifndef NRF24_REGISTERS_H
#define	NRF24_REGISTERS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>

/*
 * =========================================================
 * SPI commands
 * =========================================================
 */

#define NRF24_CMD_R_REGISTER             0x00U
#define NRF24_CMD_W_REGISTER             0x20U
#define NRF24_CMD_R_RX_PAYLOAD           0x61U
#define NRF24_CMD_W_TX_PAYLOAD           0xA0U
#define NRF24_CMD_FLUSH_TX               0xE1U
#define NRF24_CMD_FLUSH_RX               0xE2U
#define NRF24_CMD_REUSE_TX_PAYLOAD       0xE3U
#define NRF24_CMD_NOP                    0xFFU

#define NRF24_REGISTER_MASK              0x1FU

/*
 * =========================================================
 * Register addresses
 * =========================================================
 */

#define NRF24_REG_CONFIG                 0x00U
#define NRF24_REG_EN_AA                  0x01U
#define NRF24_REG_EN_RXADDR              0x02U
#define NRF24_REG_SETUP_AW               0x03U
#define NRF24_REG_SETUP_RETR             0x04U
#define NRF24_REG_RF_CH                  0x05U
#define NRF24_REG_RF_SETUP               0x06U
#define NRF24_REG_STATUS                 0x07U
#define NRF24_REG_OBSERVE_TX             0x08U
#define NRF24_REG_RPD                    0x09U
#define NRF24_REG_RX_ADDR_P0             0x0AU
#define NRF24_REG_RX_ADDR_P1             0x0BU
#define NRF24_REG_RX_ADDR_P2             0x0CU
#define NRF24_REG_RX_ADDR_P3             0x0DU
#define NRF24_REG_RX_ADDR_P4             0x0EU
#define NRF24_REG_RX_ADDR_P5             0x0FU
#define NRF24_REG_TX_ADDR                0x10U
#define NRF24_REG_RX_PW_P0               0x11U
#define NRF24_REG_RX_PW_P1               0x12U
#define NRF24_REG_RX_PW_P2               0x13U
#define NRF24_REG_RX_PW_P3               0x14U
#define NRF24_REG_RX_PW_P4               0x15U
#define NRF24_REG_RX_PW_P5               0x16U
#define NRF24_REG_FIFO_STATUS            0x17U
#define NRF24_REG_DYNPD                  0x1CU
#define NRF24_REG_FEATURE                0x1DU

/*
 * =========================================================
 * CONFIG register bits
 * =========================================================
 */

#define NRF24_CONFIG_MASK_RX_DR           0x40U
#define NRF24_CONFIG_MASK_TX_DS           0x20U
#define NRF24_CONFIG_MASK_MAX_RT          0x10U
#define NRF24_CONFIG_EN_CRC               0x08U
#define NRF24_CONFIG_CRCO                 0x04U
#define NRF24_CONFIG_PWR_UP               0x02U
#define NRF24_CONFIG_PRIM_RX              0x01U

/*
 * Primary transmitter:
 *
 * CRC enabled
 * Two-byte CRC
 * Powered up
 * PRIM_RX cleared
 */
#define NRF24_CONFIG_PTX                  \
    ((uint8_t)(                           \
        NRF24_CONFIG_EN_CRC               \
        | NRF24_CONFIG_CRCO               \
        | NRF24_CONFIG_PWR_UP             \
    ))

/*
 * =========================================================
 * EN_AA register bits
 * =========================================================
 */

#define NRF24_EN_AA_P0                    0x01U

/*
 * =========================================================
 * EN_RXADDR register bits
 * =========================================================
 */

#define NRF24_EN_RXADDR_P0                0x01U

/*
 * =========================================================
 * SETUP_AW register values
 * =========================================================
 */

#define NRF24_SETUP_AW_3_BYTES            0x01U
#define NRF24_SETUP_AW_4_BYTES            0x02U
#define NRF24_SETUP_AW_5_BYTES            0x03U

/*
 * =========================================================
 * SETUP_RETR fields
 * =========================================================
 *
 * ARD:
 * Automatic retransmission delay.
 *
 * ARC:
 * Automatic retransmission count.
 */

#define NRF24_RETR_DELAY_500_US            0x10U
#define NRF24_RETR_COUNT_5                 0x05U

#define NRF24_SETUP_RETR_DEFAULT           \
    ((uint8_t)(                            \
        NRF24_RETR_DELAY_500_US            \
        | NRF24_RETR_COUNT_5               \
    ))

/*
 * =========================================================
 * RF_SETUP register bits
 * =========================================================
 */

#define NRF24_RF_SETUP_CONT_WAVE           0x80U
#define NRF24_RF_SETUP_RF_DR_LOW           0x20U
#define NRF24_RF_SETUP_PLL_LOCK            0x10U
#define NRF24_RF_SETUP_RF_DR_HIGH          0x08U
#define NRF24_RF_SETUP_RF_PWR_HIGH         0x04U
#define NRF24_RF_SETUP_RF_PWR_LOW          0x02U

/*
 * 1 Mbps and 0 dBm.
 *
 * RF_DR_LOW  = 0
 * RF_DR_HIGH = 0
 * RF_PWR     = 11
 */
#define NRF24_RF_SETUP_1MBPS_0DBM          \
    ((uint8_t)(                            \
        NRF24_RF_SETUP_RF_PWR_HIGH         \
        | NRF24_RF_SETUP_RF_PWR_LOW        \
    ))

/*
 * =========================================================
 * STATUS register bits
 * =========================================================
 */

#define NRF24_STATUS_RX_DR                 0x40U
#define NRF24_STATUS_TX_DS                 0x20U
#define NRF24_STATUS_MAX_RT                0x10U
#define NRF24_STATUS_RX_P_NO_MASK          0x0EU
#define NRF24_STATUS_TX_FULL               0x01U

#define NRF24_STATUS_IRQ_FLAGS             \
    ((uint8_t)(                            \
        NRF24_STATUS_RX_DR                 \
        | NRF24_STATUS_TX_DS               \
        | NRF24_STATUS_MAX_RT              \
    ))

/*
 * =========================================================
 * FIFO_STATUS register bits
 * =========================================================
 */

#define NRF24_FIFO_STATUS_TX_REUSE         0x40U
#define NRF24_FIFO_STATUS_TX_FULL          0x20U
#define NRF24_FIFO_STATUS_TX_EMPTY         0x10U
#define NRF24_FIFO_STATUS_RX_FULL          0x02U
#define NRF24_FIFO_STATUS_RX_EMPTY         0x01U

/*
 * =========================================================
 * Driver settings
 * =========================================================
 */

#define NRF24_CHANNEL                      42U
#define NRF24_ADDRESS_WIDTH                5U
#define NRF24_PAYLOAD_SIZE                 4U

/*
 * Allow five milliseconds after PWR_UP before transmission.
 */
#define NRF24_POWER_UP_WAIT_MS             5U

/*
 * Software safeguard for an unfinished transmission.
 */
#define NRF24_TX_TIMEOUT_MS                20U

#endif	/* NRF24_REGISTERS_H */

