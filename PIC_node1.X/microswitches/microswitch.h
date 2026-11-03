/* 
 * File:   microswitch.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 3:31 PM
 */

#ifndef MICROSWITCH_H
#define	MICROSWITCH_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif


#include <stdbool.h>

void Microswitch_Initialize(void);

void Microswitch_Update(void);

bool Microswitch_EventAvailable(void);

bool Microswitch_GetEvent(
    bool *switchIsOn
);

bool Microswitch_GetState(void);

#endif	/* MICROSWITCH_H */

