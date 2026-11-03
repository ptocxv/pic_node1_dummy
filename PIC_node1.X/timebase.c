#include "timebase.h"
#include "mcc_generated_files/system/system.h"

static volatile uint32_t systemTimeMs = 0U;

void Timebase_TimerCallback(void)
{
    systemTimeMs++;
}

uint32_t Timebase_GetMilliseconds(void)
{
    uint32_t value;
    uint8_t interruptsWereEnabled;

    interruptsWereEnabled = INTCON0bits.GIE;

    INTERRUPT_GlobalInterruptDisable();

    value = systemTimeMs;

    if (interruptsWereEnabled != 0U)
    {
        INTERRUPT_GlobalInterruptEnable();
    }

    return value;
}

bool Timebase_DeadlineReached(
    uint32_t currentTime,
    uint32_t deadline
)
{
    return ((int32_t)(currentTime - deadline) >= 0);
}
