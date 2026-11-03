#include <stddef.h>

#include "microswitch.h"
#include "timebase.h"
#include "mcc_generated_files/system/system.h"

#define MICROSWITCH_SAMPLE_INTERVAL_MS  10U
#define MICROSWITCH_STABLE_SAMPLES      3U

static bool stableState = false;
static bool candidateState = false;

static uint8_t equalSampleCount = 0U;

static bool eventPending = false;
static bool eventState = false;

static uint32_t nextSampleTime = 0U;

static bool Microswitch_ReadInput(void)
{
    /*
     * Active-low input:
     *
     * LOW  = pressed = ON
     * HIGH = released = OFF
     */
    return (
        MICROSWITCH_GetValue() == 0U
    );
}

void Microswitch_Initialize(void)
{
    stableState =
        Microswitch_ReadInput();

    candidateState = stableState;

    equalSampleCount = 0U;

    eventPending = false;
    eventState = stableState;

    nextSampleTime =
        Timebase_GetMilliseconds();
}

void Microswitch_Update(void)
{
    uint32_t currentTime;
    bool rawState;

    currentTime =
        Timebase_GetMilliseconds();

    if (!Timebase_DeadlineReached(
            currentTime,
            nextSampleTime))
    {
        return;
    }

    nextSampleTime =
        currentTime
        + MICROSWITCH_SAMPLE_INTERVAL_MS;

    rawState =
        Microswitch_ReadInput();

    if (rawState != candidateState)
    {
        candidateState = rawState;
        equalSampleCount = 1U;

        return;
    }

    if (equalSampleCount
        < MICROSWITCH_STABLE_SAMPLES)
    {
        equalSampleCount++;
    }

    if ((equalSampleCount
         >= MICROSWITCH_STABLE_SAMPLES)
        && (candidateState != stableState))
    {
        stableState = candidateState;

        eventState = stableState;
        eventPending = true;
    }
}

bool Microswitch_EventAvailable(void)
{
    return eventPending;
}

bool Microswitch_GetEvent(
    bool *switchIsOn
)
{
    if (switchIsOn == NULL)
    {
        return false;
    }

    if (!eventPending)
    {
        return false;
    }

    *switchIsOn = eventState;

    eventPending = false;

    return true;
}

bool Microswitch_GetState(void)
{
    return stableState;
}