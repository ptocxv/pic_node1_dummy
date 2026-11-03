#include "nRF24_irq.h"
#include "mcc_generated_files/system/system.h"

static volatile bool nrfIrqPending = false;

void NRF24_IRQ_Callback(void)
{
    nrfIrqPending = true;
}

bool NRF24_IRQ_TakePendingEvent(void)
{
    bool pending;
    uint8_t interruptsWereEnabled;

    interruptsWereEnabled = INTCON0bits.GIE;

    INTERRUPT_GlobalInterruptDisable();

    pending = nrfIrqPending;
    nrfIrqPending = false;

    if (interruptsWereEnabled != 0U)
    {
        INTERRUPT_GlobalInterruptEnable();
    }

    return pending;
}

void NRF24_IRQ_ClearPendingEvent(void)
{
    uint8_t interruptsWereEnabled;

    interruptsWereEnabled = INTCON0bits.GIE;

    INTERRUPT_GlobalInterruptDisable();

    nrfIrqPending = false;

    if (interruptsWereEnabled != 0U)
    {
        INTERRUPT_GlobalInterruptEnable();
    }
}