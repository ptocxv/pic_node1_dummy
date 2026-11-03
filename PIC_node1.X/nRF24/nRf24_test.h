/* 
 * File:   nRf24_test.h
 * Author: Lenovo
 *
 * Created on November 2, 2026, 7:14 PM
 */

#ifndef NRF24_TEST_H
#define	NRF24_TEST_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>

uint8_t NRF24_TestReadRegister(
    uint8_t registerAddress
);

void NRF24_TestWriteRegister(
    uint8_t registerAddress,
    uint8_t value
);

uint8_t NRF24_TestGetStatus(void);

bool NRF24_TestSpi(void);

#endif	/* NRF24_TEST_H */

