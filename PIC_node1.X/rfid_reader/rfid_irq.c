#include "rfid_irq.h"
#include "mcc_generated_files/system/system.h"

static volatile bool rfidIrqPending = false;

void RFID_IRQ_Callback(void)
{
    // This function runs inside the IOC interrupt.
    // Do not perform SPI communication here.
    // Only record that the MFRC522 generated an event.
    rfidIrqPending = true;
}

bool RFID_IRQ_TakePendingEvent(void)
{
    bool pending;
    uint8_t interruptsWereEnabled;

    // Save the current global interrupt state.
    interruptsWereEnabled = INTCON0bits.GIE;

    // Protect the shared flag while reading and clearing it.
    INTERRUPT_GlobalInterruptDisable();

    pending = rfidIrqPending;
    rfidIrqPending = false;

    // Restore the previous interrupt state.
    if (interruptsWereEnabled != 0U)
    {
        INTERRUPT_GlobalInterruptEnable();
    }

    return pending;
}

void RFID_IRQ_ClearPendingEvent(void)
{
    uint8_t interruptsWereEnabled;

    interruptsWereEnabled = INTCON0bits.GIE;

    INTERRUPT_GlobalInterruptDisable();

    rfidIrqPending = false;

    if (interruptsWereEnabled != 0U)
    {
        INTERRUPT_GlobalInterruptEnable();
    }
}
