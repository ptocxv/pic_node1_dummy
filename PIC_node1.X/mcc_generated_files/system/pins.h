/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.1
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RB0 aliases
#define RFID_IRQ_TRIS                 TRISBbits.TRISB0
#define RFID_IRQ_LAT                  LATBbits.LATB0
#define RFID_IRQ_PORT                 PORTBbits.RB0
#define RFID_IRQ_WPU                  WPUBbits.WPUB0
#define RFID_IRQ_OD                   ODCONBbits.ODCB0
#define RFID_IRQ_ANS                  ANSELBbits.ANSELB0
#define RFID_IRQ_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RFID_IRQ_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RFID_IRQ_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RFID_IRQ_GetValue()           PORTBbits.RB0
#define RFID_IRQ_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RFID_IRQ_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RFID_IRQ_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define RFID_IRQ_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define RFID_IRQ_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define RFID_IRQ_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define RFID_IRQ_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RFID_IRQ_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)
#define RB0_SetInterruptHandler  RFID_IRQ_SetInterruptHandler

// get/set RB1 aliases
#define nRF_IRQ_TRIS                 TRISBbits.TRISB1
#define nRF_IRQ_LAT                  LATBbits.LATB1
#define nRF_IRQ_PORT                 PORTBbits.RB1
#define nRF_IRQ_WPU                  WPUBbits.WPUB1
#define nRF_IRQ_OD                   ODCONBbits.ODCB1
#define nRF_IRQ_ANS                  ANSELBbits.ANSELB1
#define nRF_IRQ_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define nRF_IRQ_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define nRF_IRQ_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define nRF_IRQ_GetValue()           PORTBbits.RB1
#define nRF_IRQ_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define nRF_IRQ_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define nRF_IRQ_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define nRF_IRQ_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define nRF_IRQ_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define nRF_IRQ_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define nRF_IRQ_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define nRF_IRQ_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)
#define RB1_SetInterruptHandler  nRF_IRQ_SetInterruptHandler

// get/set RC3 aliases
#define IO_RC3_TRIS                 TRISCbits.TRISC3
#define IO_RC3_LAT                  LATCbits.LATC3
#define IO_RC3_PORT                 PORTCbits.RC3
#define IO_RC3_WPU                  WPUCbits.WPUC3
#define IO_RC3_OD                   ODCONCbits.ODCC3
#define IO_RC3_ANS                  ANSELCbits.ANSELC3
#define IO_RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define IO_RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define IO_RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define IO_RC3_GetValue()           PORTCbits.RC3
#define IO_RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define IO_RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define IO_RC3_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define IO_RC3_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define IO_RC3_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define IO_RC3_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define IO_RC3_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define IO_RC3_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 aliases
#define IO_RC4_TRIS                 TRISCbits.TRISC4
#define IO_RC4_LAT                  LATCbits.LATC4
#define IO_RC4_PORT                 PORTCbits.RC4
#define IO_RC4_WPU                  WPUCbits.WPUC4
#define IO_RC4_OD                   ODCONCbits.ODCC4
#define IO_RC4_ANS                  ANSELCbits.ANSELC4
#define IO_RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_RC4_GetValue()           PORTCbits.RC4
#define IO_RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_RC4_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_RC4_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define IO_RC4_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define IO_RC4_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define IO_RC4_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define IO_RC4_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 aliases
#define IO_RC5_TRIS                 TRISCbits.TRISC5
#define IO_RC5_LAT                  LATCbits.LATC5
#define IO_RC5_PORT                 PORTCbits.RC5
#define IO_RC5_WPU                  WPUCbits.WPUC5
#define IO_RC5_OD                   ODCONCbits.ODCC5
#define IO_RC5_ANS                  ANSELCbits.ANSELC5
#define IO_RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define IO_RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define IO_RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define IO_RC5_GetValue()           PORTCbits.RC5
#define IO_RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define IO_RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define IO_RC5_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define IO_RC5_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define IO_RC5_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define IO_RC5_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define IO_RC5_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define IO_RC5_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RD3 aliases
#define nRF_CE_TRIS                 TRISDbits.TRISD3
#define nRF_CE_LAT                  LATDbits.LATD3
#define nRF_CE_PORT                 PORTDbits.RD3
#define nRF_CE_WPU                  WPUDbits.WPUD3
#define nRF_CE_OD                   ODCONDbits.ODCD3
#define nRF_CE_ANS                  ANSELDbits.ANSELD3
#define nRF_CE_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define nRF_CE_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define nRF_CE_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define nRF_CE_GetValue()           PORTDbits.RD3
#define nRF_CE_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define nRF_CE_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define nRF_CE_SetPullup()          do { WPUDbits.WPUD3 = 1; } while(0)
#define nRF_CE_ResetPullup()        do { WPUDbits.WPUD3 = 0; } while(0)
#define nRF_CE_SetPushPull()        do { ODCONDbits.ODCD3 = 0; } while(0)
#define nRF_CE_SetOpenDrain()       do { ODCONDbits.ODCD3 = 1; } while(0)
#define nRF_CE_SetAnalogMode()      do { ANSELDbits.ANSELD3 = 1; } while(0)
#define nRF_CE_SetDigitalMode()     do { ANSELDbits.ANSELD3 = 0; } while(0)

// get/set RD4 aliases
#define nRF_CSN_TRIS                 TRISDbits.TRISD4
#define nRF_CSN_LAT                  LATDbits.LATD4
#define nRF_CSN_PORT                 PORTDbits.RD4
#define nRF_CSN_WPU                  WPUDbits.WPUD4
#define nRF_CSN_OD                   ODCONDbits.ODCD4
#define nRF_CSN_ANS                  ANSELDbits.ANSELD4
#define nRF_CSN_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define nRF_CSN_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define nRF_CSN_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define nRF_CSN_GetValue()           PORTDbits.RD4
#define nRF_CSN_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define nRF_CSN_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define nRF_CSN_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define nRF_CSN_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define nRF_CSN_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define nRF_CSN_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define nRF_CSN_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define nRF_CSN_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set RD6 aliases
#define RFID_RST_TRIS                 TRISDbits.TRISD6
#define RFID_RST_LAT                  LATDbits.LATD6
#define RFID_RST_PORT                 PORTDbits.RD6
#define RFID_RST_WPU                  WPUDbits.WPUD6
#define RFID_RST_OD                   ODCONDbits.ODCD6
#define RFID_RST_ANS                  ANSELDbits.ANSELD6
#define RFID_RST_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define RFID_RST_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define RFID_RST_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define RFID_RST_GetValue()           PORTDbits.RD6
#define RFID_RST_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define RFID_RST_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define RFID_RST_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define RFID_RST_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define RFID_RST_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define RFID_RST_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define RFID_RST_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define RFID_RST_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set RD7 aliases
#define RFID_CS_TRIS                 TRISDbits.TRISD7
#define RFID_CS_LAT                  LATDbits.LATD7
#define RFID_CS_PORT                 PORTDbits.RD7
#define RFID_CS_WPU                  WPUDbits.WPUD7
#define RFID_CS_OD                   ODCONDbits.ODCD7
#define RFID_CS_ANS                  ANSELDbits.ANSELD7
#define RFID_CS_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define RFID_CS_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define RFID_CS_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define RFID_CS_GetValue()           PORTDbits.RD7
#define RFID_CS_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define RFID_CS_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define RFID_CS_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define RFID_CS_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define RFID_CS_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define RFID_CS_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define RFID_CS_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define RFID_CS_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

// get/set RF0 aliases
#define IO_RF0_TRIS                 TRISFbits.TRISF0
#define IO_RF0_LAT                  LATFbits.LATF0
#define IO_RF0_PORT                 PORTFbits.RF0
#define IO_RF0_WPU                  WPUFbits.WPUF0
#define IO_RF0_OD                   ODCONFbits.ODCF0
#define IO_RF0_ANS                  ANSELFbits.ANSELF0
#define IO_RF0_SetHigh()            do { LATFbits.LATF0 = 1; } while(0)
#define IO_RF0_SetLow()             do { LATFbits.LATF0 = 0; } while(0)
#define IO_RF0_Toggle()             do { LATFbits.LATF0 = ~LATFbits.LATF0; } while(0)
#define IO_RF0_GetValue()           PORTFbits.RF0
#define IO_RF0_SetDigitalInput()    do { TRISFbits.TRISF0 = 1; } while(0)
#define IO_RF0_SetDigitalOutput()   do { TRISFbits.TRISF0 = 0; } while(0)
#define IO_RF0_SetPullup()          do { WPUFbits.WPUF0 = 1; } while(0)
#define IO_RF0_ResetPullup()        do { WPUFbits.WPUF0 = 0; } while(0)
#define IO_RF0_SetPushPull()        do { ODCONFbits.ODCF0 = 0; } while(0)
#define IO_RF0_SetOpenDrain()       do { ODCONFbits.ODCF0 = 1; } while(0)
#define IO_RF0_SetAnalogMode()      do { ANSELFbits.ANSELF0 = 1; } while(0)
#define IO_RF0_SetDigitalMode()     do { ANSELFbits.ANSELF0 = 0; } while(0)

// get/set RF1 aliases
#define IO_RF1_TRIS                 TRISFbits.TRISF1
#define IO_RF1_LAT                  LATFbits.LATF1
#define IO_RF1_PORT                 PORTFbits.RF1
#define IO_RF1_WPU                  WPUFbits.WPUF1
#define IO_RF1_OD                   ODCONFbits.ODCF1
#define IO_RF1_ANS                  ANSELFbits.ANSELF1
#define IO_RF1_SetHigh()            do { LATFbits.LATF1 = 1; } while(0)
#define IO_RF1_SetLow()             do { LATFbits.LATF1 = 0; } while(0)
#define IO_RF1_Toggle()             do { LATFbits.LATF1 = ~LATFbits.LATF1; } while(0)
#define IO_RF1_GetValue()           PORTFbits.RF1
#define IO_RF1_SetDigitalInput()    do { TRISFbits.TRISF1 = 1; } while(0)
#define IO_RF1_SetDigitalOutput()   do { TRISFbits.TRISF1 = 0; } while(0)
#define IO_RF1_SetPullup()          do { WPUFbits.WPUF1 = 1; } while(0)
#define IO_RF1_ResetPullup()        do { WPUFbits.WPUF1 = 0; } while(0)
#define IO_RF1_SetPushPull()        do { ODCONFbits.ODCF1 = 0; } while(0)
#define IO_RF1_SetOpenDrain()       do { ODCONFbits.ODCF1 = 1; } while(0)
#define IO_RF1_SetAnalogMode()      do { ANSELFbits.ANSELF1 = 1; } while(0)
#define IO_RF1_SetDigitalMode()     do { ANSELFbits.ANSELF1 = 0; } while(0)

// get/set RF2 aliases
#define BUZZER_TRIS                 TRISFbits.TRISF2
#define BUZZER_LAT                  LATFbits.LATF2
#define BUZZER_PORT                 PORTFbits.RF2
#define BUZZER_WPU                  WPUFbits.WPUF2
#define BUZZER_OD                   ODCONFbits.ODCF2
#define BUZZER_ANS                  ANSELFbits.ANSELF2
#define BUZZER_SetHigh()            do { LATFbits.LATF2 = 1; } while(0)
#define BUZZER_SetLow()             do { LATFbits.LATF2 = 0; } while(0)
#define BUZZER_Toggle()             do { LATFbits.LATF2 = ~LATFbits.LATF2; } while(0)
#define BUZZER_GetValue()           PORTFbits.RF2
#define BUZZER_SetDigitalInput()    do { TRISFbits.TRISF2 = 1; } while(0)
#define BUZZER_SetDigitalOutput()   do { TRISFbits.TRISF2 = 0; } while(0)
#define BUZZER_SetPullup()          do { WPUFbits.WPUF2 = 1; } while(0)
#define BUZZER_ResetPullup()        do { WPUFbits.WPUF2 = 0; } while(0)
#define BUZZER_SetPushPull()        do { ODCONFbits.ODCF2 = 0; } while(0)
#define BUZZER_SetOpenDrain()       do { ODCONFbits.ODCF2 = 1; } while(0)
#define BUZZER_SetAnalogMode()      do { ANSELFbits.ANSELF2 = 1; } while(0)
#define BUZZER_SetDigitalMode()     do { ANSELFbits.ANSELF2 = 0; } while(0)

// get/set RF3 aliases
#define MICROSWITCH_TRIS                 TRISFbits.TRISF3
#define MICROSWITCH_LAT                  LATFbits.LATF3
#define MICROSWITCH_PORT                 PORTFbits.RF3
#define MICROSWITCH_WPU                  WPUFbits.WPUF3
#define MICROSWITCH_OD                   ODCONFbits.ODCF3
#define MICROSWITCH_ANS                  ANSELFbits.ANSELF3
#define MICROSWITCH_SetHigh()            do { LATFbits.LATF3 = 1; } while(0)
#define MICROSWITCH_SetLow()             do { LATFbits.LATF3 = 0; } while(0)
#define MICROSWITCH_Toggle()             do { LATFbits.LATF3 = ~LATFbits.LATF3; } while(0)
#define MICROSWITCH_GetValue()           PORTFbits.RF3
#define MICROSWITCH_SetDigitalInput()    do { TRISFbits.TRISF3 = 1; } while(0)
#define MICROSWITCH_SetDigitalOutput()   do { TRISFbits.TRISF3 = 0; } while(0)
#define MICROSWITCH_SetPullup()          do { WPUFbits.WPUF3 = 1; } while(0)
#define MICROSWITCH_ResetPullup()        do { WPUFbits.WPUF3 = 0; } while(0)
#define MICROSWITCH_SetPushPull()        do { ODCONFbits.ODCF3 = 0; } while(0)
#define MICROSWITCH_SetOpenDrain()       do { ODCONFbits.ODCF3 = 1; } while(0)
#define MICROSWITCH_SetAnalogMode()      do { ANSELFbits.ANSELF3 = 1; } while(0)
#define MICROSWITCH_SetDigitalMode()     do { ANSELFbits.ANSELF3 = 0; } while(0)

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handler for the RFID_IRQ pin functionality
 * @param none
 * @return none
 */
void RFID_IRQ_ISR(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt Handler Setter for RFID_IRQ pin interrupt-on-change functionality.
 *        Allows selecting an interrupt handler for RFID_IRQ at application runtime
 * @pre Pins intializer called
 * @param InterruptHandler function pointer.
 * @return none
 */
void RFID_IRQ_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup  pinsdriver
 * @brief Dynamic Interrupt Handler for RFID_IRQ pin.
 *        This is a dynamic interrupt handler to be used together with the RFID_IRQ_SetInterruptHandler() method.
 *        This handler is called every time the RFID_IRQ ISR is executed and allows any function to be registered at runtime.
 * @pre Pins intializer called
 * @param none
 * @return none
 */
extern void (*RFID_IRQ_InterruptHandler)(void);

/**
 * @ingroup  pinsdriver
 * @brief Default Interrupt Handler for RFID_IRQ pin. 
 *        This is a predefined interrupt handler to be used together with the RFID_IRQ_SetInterruptHandler() method.
 *        This handler is called every time the RFID_IRQ ISR is executed. 
 * @pre Pins intializer called
 * @param none
 * @return none
 */
void RFID_IRQ_DefaultInterruptHandler(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handler for the nRF_IRQ pin functionality
 * @param none
 * @return none
 */
void nRF_IRQ_ISR(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt Handler Setter for nRF_IRQ pin interrupt-on-change functionality.
 *        Allows selecting an interrupt handler for nRF_IRQ at application runtime
 * @pre Pins intializer called
 * @param InterruptHandler function pointer.
 * @return none
 */
void nRF_IRQ_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup  pinsdriver
 * @brief Dynamic Interrupt Handler for nRF_IRQ pin.
 *        This is a dynamic interrupt handler to be used together with the nRF_IRQ_SetInterruptHandler() method.
 *        This handler is called every time the nRF_IRQ ISR is executed and allows any function to be registered at runtime.
 * @pre Pins intializer called
 * @param none
 * @return none
 */
extern void (*nRF_IRQ_InterruptHandler)(void);

/**
 * @ingroup  pinsdriver
 * @brief Default Interrupt Handler for nRF_IRQ pin. 
 *        This is a predefined interrupt handler to be used together with the nRF_IRQ_SetInterruptHandler() method.
 *        This handler is called every time the nRF_IRQ ISR is executed. 
 * @pre Pins intializer called
 * @param none
 * @return none
 */
void nRF_IRQ_DefaultInterruptHandler(void);


#endif // PINS_H
/**
 End of File
*/