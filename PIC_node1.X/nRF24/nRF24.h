/* 
 * File:   nRF24.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 7:48 PM
 */

#ifndef NRF24_H
#define	NRF24_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>

#include "nrf24_registers.h"

/*
 * Internal radio states.
 */
typedef enum
{
    NRF24_STATE_UNINITIALIZED = 0,
    NRF24_STATE_POWER_UP_WAIT,
    NRF24_STATE_READY,
    NRF24_STATE_TX_WAIT,
    NRF24_STATE_TX_SUCCESS,
    NRF24_STATE_TX_MAX_RETRIES,
    NRF24_STATE_ERROR
} nrf24_state_t;

/*
 * Low-level register operations.
 */
uint8_t NRF24_ReadRegister(
    uint8_t registerAddress
);

void NRF24_WriteRegister(
    uint8_t registerAddress,
    uint8_t value
);

void NRF24_WriteRegisterBuffer(
    uint8_t registerAddress,
    const uint8_t *data,
    uint8_t length
);

uint8_t NRF24_GetStatus(void);

void NRF24_FlushTx(void);

void NRF24_FlushRx(void);

void NRF24_ClearInterruptFlags(
    uint8_t flags
);

/*
 * Radio initialization.
 *
 * Call NRF24_InitializeStart() after the nRF24 has been
 * powered for approximately 100 ms.
 *
 * Then call NRF24_Update() repeatedly.
 */
void NRF24_InitializeStart(void);

bool NRF24_IsReady(void);

/*
 * Start one non-blocking payload transmission.
 */
bool NRF24_SendStart(
    const uint8_t *payload,
    uint8_t length
);

/*
 * Progress initialization and transmission.
 *
 * Call repeatedly from the main loop.
 */
void NRF24_Update(void);

/*
 * Transmission result queries.
 */
bool NRF24_SendSucceeded(void);

bool NRF24_SendMaxRetries(void);

bool NRF24_HasError(void);

/*
 * Clear a completed transmission result and return
 * the radio to READY.
 */
void NRF24_ClearResult(void);

nrf24_state_t NRF24_GetState(void);

#endif	/* NRF24_H */

