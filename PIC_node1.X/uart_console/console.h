/* 
 * File:   console.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 1:40 AM
 */

#ifndef CONSOLE_H
#define	CONSOLE_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>

void Console_Initialize(void);

void Console_Update(void);

bool Console_WriteCharacter(char character);

bool Console_WriteString(const char *text);

bool Console_WriteHexByte(uint8_t value);

bool Console_WriteUid(
    const uint8_t *uidBytes,
    uint8_t uidLength
);

bool Console_IsBusy(void);

bool Console_HasOverflowed(void);

void Console_ClearOverflow(void);

#endif	/* CONSOLE_H */

