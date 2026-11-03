/* 
 * File:   mfrc522_core.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 12:51 AM
 */

#ifndef MFRC522_CORE_H
#define	MFRC522_CORE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>

#include "mfrc522_registers.h"

// Non-blocking MFRC522 initialization states.
typedef enum
{
    MFRC522_INIT_NOT_STARTED = 0,
    MFRC522_INIT_HARD_RESET_LOW,
    MFRC522_INIT_HARD_RESET_WAIT,
    MFRC522_INIT_SOFT_RESET_START,
    MFRC522_INIT_SOFT_RESET_WAIT,
    MFRC522_INIT_CONFIGURE,
    MFRC522_INIT_READY,
    MFRC522_INIT_ERROR
} mfrc522_init_state_t;

// Low-level MFRC522 register access.
void MFRC522_CoreWriteRegister(
    uint8_t registerAddress,
    uint8_t value
);

uint8_t MFRC522_CoreReadRegister(
    uint8_t registerAddress
);

void MFRC522_CoreSetRegisterBits(
    uint8_t registerAddress,
    uint8_t mask
);

void MFRC522_CoreClearRegisterBits(
    uint8_t registerAddress,
    uint8_t mask
);

// Reader diagnostics and antenna control.
uint8_t MFRC522_CoreGetVersion(void);

uint8_t MFRC522_CoreGetDetectedVersion(void);

void MFRC522_CoreAntennaOn(void);

void MFRC522_CoreAntennaOff(void);

// Non-blocking MFRC522 initialization.
void MFRC522_CoreInitializeStart(void);

void MFRC522_CoreInitializeUpdate(void);

bool MFRC522_CoreIsReady(void);

bool MFRC522_CoreInitializationFailed(void);

mfrc522_init_state_t MFRC522_CoreGetInitializationState(void);

#endif	/* MFRC522_CORE_H */

