/* 
 * File:   nRF24_irq.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 3:40 PM
 */

#ifndef NRF24_IRQ_H
#define	NRF24_IRQ_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdbool.h>

void NRF24_IRQ_Callback(void);

bool NRF24_IRQ_TakePendingEvent(void);

void NRF24_IRQ_ClearPendingEvent(void);

#endif	/* NRF24_IRQ_H */

