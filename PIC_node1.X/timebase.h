/* 
 * File:   timebase.h
 * Author: Lenovo
 *
 * Created on November 1, 2026, 11:18 PM
 */

#ifndef TIMEBASE_H
#define	TIMEBASE_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>

void Timebase_TimerCallback(void);

uint32_t Timebase_GetMilliseconds(void);

bool Timebase_DeadlineReached(
    uint32_t currentTime,
    uint32_t deadline
);

#endif	/* TIMEBASE_H */

