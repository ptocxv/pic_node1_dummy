#include "mcc_generated_files/system/system.h"

#include <stdbool.h>
#include <stdint.h>

#include "timebase.h"
#include "console.h"
#include "microswitch.h"
#include "nrf24.h"
#include "nrf24_irq.h"
#include "nrf24_test.h"

static uint8_t resetCause = 0U;

int main(void)
{
    bool switchIsOn = false;
    bool packetPending = false;

    uint8_t payload[NRF24_PAYLOAD_SIZE] =
    {
        0U,
        0U,
        0U,
        0U
    };

    resetCause = PCON0;
    PCON0 = 0x3FU;

    SYSTEM_Initialize();

    /*
     * Ensure both shared SPI devices are initially deselected.
     */
    RFID_CS_SetHigh();

    nRF_CSN_SetHigh();
    nRF_CE_SetLow();

    Console_Initialize();
    Microswitch_Initialize();

    Console_WriteString(
        "\r\nRESET PCON0=0x"
    );

    Console_WriteHexByte(resetCause);
    Console_WriteString("\r\n");

    /*
     * Register the 1 ms timebase callback.
     */
    TMR0_OverflowCallbackRegister(
        Timebase_TimerCallback
    );

    /*
     * Register the active-low nRF24 IRQ callback.
     */
    nRF_IRQ_SetInterruptHandler(
        NRF24_IRQ_Callback
    );

    /*
     * Open the SPI peripheral shared by the nRF24 and RFID reader.
     */
    if (!SPI1_Open(HOST_CONFIG))
    {
        Console_WriteString(
            "SPI open failed\r\n"
        );

        while (1)
        {
            Console_Update();
        }
    }

    Console_WriteString(
        "nRF SPI test\r\n"
    );

    if (NRF24_TestSpi())
    {
        Console_WriteString(
            "nRF SPI OK\r\n"
        );
    }
    else
    {
        Console_WriteString(
            "nRF SPI FAIL\r\n"
        );
    }

    Console_WriteString(
        "STATUS=0x"
    );

    Console_WriteHexByte(
        NRF24_TestGetStatus()
    );

    Console_WriteString("\r\n");

    /*
     * Start non-blocking nRF24 transmitter initialization.
     */
    NRF24_InitializeStart();

    /*
     * Enable interrupts after all callbacks have been registered.
     */
    INTERRUPT_GlobalInterruptEnable();

    while (1)
    {
        Console_Update();
        Microswitch_Update();
        NRF24_Update();

        /*
         * Stop normal operation if the local radio driver enters
         * an unrecoverable error state.
         */
        if (NRF24_HasError())
        {
            Console_WriteString(
                "nRF driver error\r\n"
            );

            while (1)
            {
                Console_Update();
            }
        }

        /*
         * Handle a successfully acknowledged transmission.
         */
        if (NRF24_SendSucceeded())
        {
            Console_WriteString(
                "nRF TX success\r\n"
            );

            NRF24_ClearResult();
        }
        /*
         * MAX_RT means the transmitter did not receive an
         * acknowledgement from the ESP32-side nRF24.
         */
        else if (NRF24_SendMaxRetries())
        {
            Console_WriteString(
                "nRF TX max retries\r\n"
            );

            NRF24_ClearResult();
        }

        /*
         * Save a new microswitch state when an event occurs.
         */
        if (Microswitch_GetEvent(
                &switchIsOn))
        {
            if (switchIsOn)
            {
                Console_WriteString(
                    "Switch: ON\r\n"
                );

                payload[0] = 1U;
            }
            else
            {
                Console_WriteString(
                    "Switch: OFF\r\n"
                );

                payload[0] = 0U;
            }

            /*
             * The radio uses a fixed four-byte payload.
             */
            payload[1] = 0U;
            payload[2] = 0U;
            payload[3] = 0U;

            packetPending = true;
        }

        /*
         * Send only when a packet is pending and the radio has
         * finished initialization or its previous transmission.
         */
        if (packetPending &&
            NRF24_IsReady())
        {
            if (NRF24_SendStart(
                    payload,
                    sizeof(payload)))
            {
                packetPending = false;

                Console_WriteString(
                    "nRF TX started\r\n"
                );
            }
        }
    }
}