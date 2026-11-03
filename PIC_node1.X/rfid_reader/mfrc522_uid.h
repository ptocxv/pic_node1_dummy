/* 
 * File:   mfrc522_uid.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 12:53 AM
 */

#ifndef MFRC522_UID_H
#define	MFRC522_UID_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>

#include "mfrc522_registers.h"

/*
 * UID returned by the MFRC522 UID state machine.
 *
 * This implementation supports:
 *
 * 4-byte UID
 * 7-byte UID
 *
 * The array also leaves space for possible future
 * 10-byte UID support.
 */
typedef struct
{
    uint8_t length;
    uint8_t bytes[MFRC522_UID_MAX_LENGTH];
} mfrc522_uid_t;

/*
 * Non-blocking UID-reading states.
 */
typedef enum
{
    MFRC522_UID_IDLE = 0,

    MFRC522_UID_REQA_START,
    MFRC522_UID_REQA_WAIT,

    MFRC522_UID_ANTICOLLISION_CL1_START,
    MFRC522_UID_ANTICOLLISION_CL1_WAIT,

    MFRC522_UID_SELECT_CL1_START,
    MFRC522_UID_SELECT_CL1_WAIT,

    MFRC522_UID_ANTICOLLISION_CL2_START,
    MFRC522_UID_ANTICOLLISION_CL2_WAIT,

    MFRC522_UID_SELECT_CL2_START,
    MFRC522_UID_SELECT_CL2_WAIT,

    MFRC522_UID_READY,
    MFRC522_UID_NO_TAG,
    MFRC522_UID_ERROR
} mfrc522_uid_state_t;

/*
 * Start a new non-blocking UID-reading operation.
 *
 * Returns false if:
 *
 * The MFRC522 core is not ready.
 * A UID operation is already in progress.
 */
bool MFRC522_UidStart(void);

/*
 * Progress the UID-reading state machine.
 *
 * Call repeatedly from the main scheduler.
 */
void MFRC522_UidUpdate(void);

/*
 * Return true while the UID-reading operation
 * is in progress.
 */
bool MFRC522_UidIsBusy(void);

/*
 * Return true when a complete UID is available.
 */
bool MFRC522_UidAvailable(void);

/*
 * Copy the detected UID into the caller-owned structure.
 *
 * Returns false if no UID is currently available.
 */
bool MFRC522_UidGet(
    mfrc522_uid_t *uid
);

/*
 * Return true if no tag answered the REQA command.
 */
bool MFRC522_UidNoTag(void);

/*
 * Return true if the UID operation failed.
 */
bool MFRC522_UidHasError(void);

/*
 * Reset a completed UID operation to IDLE.
 *
 * This does not interrupt an active operation.
 */
void MFRC522_UidReset(void);

/*
 * Cancel the active UID operation.
 */
void MFRC522_UidCancel(void);

/*
 * Return the current UID state.
 */
mfrc522_uid_state_t MFRC522_UidGetState(void);

#endif	/* MFRC522_UID_H */

