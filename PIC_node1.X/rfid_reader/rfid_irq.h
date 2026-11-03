/* 
 * File:   rfid_irq.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 12:12 AM
 */

#ifndef RFID_IRQ_H
#define	RFID_IRQ_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdbool.h>

void RFID_IRQ_Callback(void);

bool RFID_IRQ_TakePendingEvent(void);

void RFID_IRQ_ClearPendingEvent(void);

#endif	/* RFID_IRQ_H */

