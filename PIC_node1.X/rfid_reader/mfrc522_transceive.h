/* 
 * File:   mfrc522_transceive.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 12:52 AM
 */

#ifndef MFRC522_TRANSCEIVE_H
#define	MFRC522_TRANSCEIVE_H

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
 * Result state of one MFRC522 Transceive operation.
 */
typedef enum
{
    MFRC522_TRANSCEIVE_IDLE = 0,

    MFRC522_TRANSCEIVE_WAITING,

    MFRC522_TRANSCEIVE_COMPLETE,

    MFRC522_TRANSCEIVE_NO_RESPONSE,

    MFRC522_TRANSCEIVE_ERROR,

    MFRC522_TRANSCEIVE_BUFFER_ERROR
} mfrc522_transceive_state_t;

/*
 * Start one non-blocking Transceive operation.
 *
 * sendData:
 * Bytes to place into the MFRC522 FIFO.
 *
 * sendLength:
 * Number of bytes to transmit.
 *
 * validLastBits:
 * Number of valid bits in the final transmitted byte.
 *
 * Use:
 *
 * MFRC522_REQA_VALID_BITS
 *
 * for the seven-bit REQA command.
 *
 * Use:
 *
 * MFRC522_FULL_BYTE_VALID_BITS
 *
 * for frames containing complete bytes.
 *
 * timeoutMs:
 * Software backup timeout.
 */
bool MFRC522_TransceiveStart(
    const uint8_t *sendData,
    uint8_t sendLength,
    uint8_t validLastBits,
    uint32_t timeoutMs
);

/*
 * Progress the active Transceive operation.
 *
 * Call repeatedly from the main scheduler.
 */
void MFRC522_TransceiveUpdate(void);

/*
 * Return the current Transceive state.
 */
mfrc522_transceive_state_t
MFRC522_TransceiveGetState(void);

/*
 * Return true while the reader is waiting for completion.
 */
bool MFRC522_TransceiveIsBusy(void);

/*
 * Return true when a valid response is available.
 */
bool MFRC522_TransceiveResponseAvailable(void);

/*
 * Return the received response length.
 *
 * Returns zero unless a valid response is available.
 */
uint8_t MFRC522_TransceiveGetResponseLength(void);

/*
 * Copy the received response into a caller-owned buffer.
 *
 * destinationSize must be at least as large as the
 * received response.
 */
bool MFRC522_TransceiveCopyResponse(
    uint8_t *destination,
    uint8_t destinationSize
);

/*
 * Return true if the tag did not answer or the reader's
 * internal timer expired.
 */
bool MFRC522_TransceiveNoResponse(void);

/*
 * Return true if a communication or buffer error occurred.
 */
bool MFRC522_TransceiveHasError(void);

/*
 * Reset a completed operation to IDLE.
 *
 * This function does not cancel an operation that is
 * currently waiting.
 */
void MFRC522_TransceiveReset(void);

/*
 * Immediately stop and reset the current operation.
 */
void MFRC522_TransceiveCancel(void);

#endif	/* MFRC522_TRANSCEIVE_H */

