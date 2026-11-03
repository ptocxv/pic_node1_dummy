/**
 * Generated Driver File
 * 
 * @file pins.c
 * 
 * @ingroup  pinsdriver
 * 
 * @brief This is generated driver implementation for pins. 
 *        This file provides implementations for pin APIs for all pins selected in the GUI.
 *
 * @version Driver Version 3.1.1
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

#include "../pins.h"

void (*RFID_IRQ_InterruptHandler)(void);
void (*nRF_IRQ_InterruptHandler)(void);

void PIN_MANAGER_Initialize(void)
{
   /**
    LATx registers
    */
    LATA = 0x0;
    LATB = 0x0;
    LATC = 0x0;
    LATD = 0xD0;
    LATE = 0x0;
    LATF = 0x0;
    /**
    ODx registers
    */
    ODCONA = 0x0;
    ODCONB = 0x0;
    ODCONC = 0x0;
    ODCOND = 0x0;
    ODCONE = 0x0;
    ODCONF = 0x0;

    /**
    TRISx registers
    */
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xDF;
    TRISD = 0x27;
    TRISE = 0xF;
    TRISF = 0xFA;

    /**
    ANSELx registers
    */
    ANSELA = 0xFF;
    ANSELB = 0xFC;
    ANSELC = 0xC7;
    ANSELD = 0x27;
    ANSELE = 0x7;
    ANSELF = 0xF0;

    /**
    WPUx registers
    */
    WPUA = 0x0;
    WPUB = 0x0;
    WPUC = 0x0;
    WPUD = 0x0;
    WPUE = 0x0;
    WPUF = 0x2;


    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;
    SLRCOND = 0xFF;
    SLRCONE = 0x7;
    SLRCONF = 0xFF;

    /**
    INLVLx registers
    */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xFF;
    INLVLD = 0xFF;
    INLVLE = 0xF;
    INLVLF = 0xFF;

   /**
    RxyI2C | RxyFEAT registers   
    */
    RB1I2C = 0x0;
    RB2I2C = 0x0;
    RC3I2C = 0x0;
    RC4I2C = 0x0;
    /**
    PPS registers
    */
    U1RXPPS = 0x29; //RF1->UART1:RX1;
    SPI1SDIPPS = 0x14; //RC4->SPI1:SDI1;
    RF0PPS = 0x20;  //RF0->UART1:TX1;
    RC5PPS = 0x32;  //RC5->SPI1:SDO1;
    SPI1SCKPPS = 0x13;  //RC3->SPI1:SCK1;
    RC3PPS = 0x31;  //RC3->SPI1:SCK1;

   /**
    IOCx registers 
    */
    IOCAP = 0x0;
    IOCAN = 0x0;
    IOCAF = 0x0;
    IOCBP = 0x0;
    IOCBN = 0x3;
    IOCBF = 0x0;
    IOCCP = 0x0;
    IOCCN = 0x0;
    IOCCF = 0x0;
    IOCEP = 0x0;
    IOCEN = 0x0;
    IOCEF = 0x0;

    RFID_IRQ_SetInterruptHandler(RFID_IRQ_DefaultInterruptHandler);
    nRF_IRQ_SetInterruptHandler(nRF_IRQ_DefaultInterruptHandler);

    // Enable PIE0bits.IOCIE interrupt 
    PIE0bits.IOCIE = 1; 
}
  
void PIN_MANAGER_IOC(void)
{
    // interrupt on change for pin RFID_IRQ
    if(IOCBFbits.IOCBF0 == 1)
    {
        RFID_IRQ_ISR();  
    }
    // interrupt on change for pin nRF_IRQ
    if(IOCBFbits.IOCBF1 == 1)
    {
        nRF_IRQ_ISR();  
    }
}
   
/**
   RFID_IRQ Interrupt Service Routine
*/
void RFID_IRQ_ISR(void) {

    // Add custom RFID_IRQ code

    // Call the interrupt handler for the callback registered at runtime
    if(RFID_IRQ_InterruptHandler)
    {
        RFID_IRQ_InterruptHandler();
    }
    IOCBFbits.IOCBF0 = 0;
}

/**
  Allows selecting an interrupt handler for RFID_IRQ at application runtime
*/
void RFID_IRQ_SetInterruptHandler(void (* InterruptHandler)(void)){
    RFID_IRQ_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for RFID_IRQ
*/
void RFID_IRQ_DefaultInterruptHandler(void){
    // add your RFID_IRQ interrupt custom code
    // or set custom function using RFID_IRQ_SetInterruptHandler()
}
   
/**
   nRF_IRQ Interrupt Service Routine
*/
void nRF_IRQ_ISR(void) {

    // Add custom nRF_IRQ code

    // Call the interrupt handler for the callback registered at runtime
    if(nRF_IRQ_InterruptHandler)
    {
        nRF_IRQ_InterruptHandler();
    }
    IOCBFbits.IOCBF1 = 0;
}

/**
  Allows selecting an interrupt handler for nRF_IRQ at application runtime
*/
void nRF_IRQ_SetInterruptHandler(void (* InterruptHandler)(void)){
    nRF_IRQ_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for nRF_IRQ
*/
void nRF_IRQ_DefaultInterruptHandler(void){
    // add your nRF_IRQ interrupt custom code
    // or set custom function using nRF_IRQ_SetInterruptHandler()
}
/**
 End of File
*/