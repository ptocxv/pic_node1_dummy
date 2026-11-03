#include <stddef.h>

#include "console.h"
#include "mcc_generated_files/system/system.h"

/*
 * Number of bytes that can wait for transmission.
 *
 * The usable capacity is one byte less than this value
 * because equal read and write positions represent an
 * empty queue.
 */
#define CONSOLE_TX_BUFFER_SIZE 128U

static char transmitBuffer[
    CONSOLE_TX_BUFFER_SIZE
];

static uint8_t transmitWriteIndex = 0U;
static uint8_t transmitReadIndex = 0U;

static bool transmitOverflow = false;


/*
 * Convert a value from 0 to 15 into one hexadecimal
 * character.
 */
static char Console_ToHexCharacter(uint8_t value)
{
    value &= 0x0FU;

    if (value < 10U)
    {
        return (char)('0' + value);
    }

    return (char)(
        'A' + (value - 10U)
    );
}


/*
 * Calculate the next circular-buffer index.
 */
static uint8_t Console_GetNextIndex(
    uint8_t currentIndex
)
{
    currentIndex++;

    if (currentIndex >= CONSOLE_TX_BUFFER_SIZE)
    {
        currentIndex = 0U;
    }

    return currentIndex;
}


/*
 * Initialize the console software queue.
 *
 * UART1 itself is initialized by SYSTEM_Initialize().
 */
void Console_Initialize(void)
{
    transmitWriteIndex = 0U;
    transmitReadIndex = 0U;
    transmitOverflow = false;
}


/*
 * Progress UART transmission.
 *
 * This function never waits. If UART1 is not ready,
 * the function returns immediately.
 *
 * Call Console_Update() repeatedly from the main loop.
 */
void Console_Update(void)
{
    char character;

    /*
     * An equal read and write index means that the
     * transmit queue is empty.
     */
    if (transmitReadIndex == transmitWriteIndex)
    {
        return;
    }

    /*
     * Do not wait for UART1.
     */
    if (!UART1_IsTxReady())
    {
        return;
    }

    character =
        transmitBuffer[transmitReadIndex];

    transmitReadIndex =
        Console_GetNextIndex(
            transmitReadIndex
        );

    UART1_Write(
        (uint8_t)character
    );
}


/*
 * Add one character to the transmit queue.
 *
 * Returns false if the queue is full.
 */
bool Console_WriteCharacter(char character)
{
    uint8_t nextWriteIndex;

    nextWriteIndex =
        Console_GetNextIndex(
            transmitWriteIndex
        );

    /*
     * If advancing the write index would reach the
     * read index, the circular queue is full.
     */
    if (nextWriteIndex == transmitReadIndex)
    {
        transmitOverflow = true;

        return false;
    }

    transmitBuffer[transmitWriteIndex] =
        character;

    transmitWriteIndex =
        nextWriteIndex;

    return true;
}


/*
 * Add a null-terminated string to the transmit queue.
 *
 * The function returns false if the queue becomes full.
 * Characters already queued remain available for
 * transmission.
 */
bool Console_WriteString(const char *text)
{
    if (text == NULL)
    {
        return false;
    }

    while (*text != '\0')
    {
        if (!Console_WriteCharacter(*text))
        {
            return false;
        }

        text++;
    }

    return true;
}


/*
 * Queue one byte as two uppercase hexadecimal digits.
 *
 * Example:
 *
 * 0x04 becomes "04"
 * 0xA7 becomes "A7"
 */
bool Console_WriteHexByte(uint8_t value)
{
    char highCharacter;
    char lowCharacter;

    highCharacter =
        Console_ToHexCharacter(
            (uint8_t)(value >> 4U)
        );

    lowCharacter =
        Console_ToHexCharacter(value);

    if (!Console_WriteCharacter(highCharacter))
    {
        return false;
    }

    if (!Console_WriteCharacter(lowCharacter))
    {
        return false;
    }

    return true;
}


/*
 * Queue a UID in hexadecimal format.
 *
 * Example output:
 *
 * UID: 04:A7:3C:92:1B:6F:80
 */
bool Console_WriteUid(
    const uint8_t *uidBytes,
    uint8_t uidLength
)
{
    uint8_t index;

    if (uidBytes == NULL)
    {
        return false;
    }

    if (uidLength == 0U)
    {
        return false;
    }

    if (!Console_WriteString("UID: "))
    {
        return false;
    }

    for (index = 0U;
         index < uidLength;
         index++)
    {
        if (!Console_WriteHexByte(
                uidBytes[index]))
        {
            return false;
        }

        if ((index + 1U) < uidLength)
        {
            if (!Console_WriteCharacter(':'))
            {
                return false;
            }
        }
    }

    return Console_WriteString("\r\n");
}


/*
 * Return true while characters remain queued.
 */
bool Console_IsBusy(void)
{
    return (
        transmitReadIndex
        != transmitWriteIndex
    );
}


/*
 * Return true if a previous write could not fit in
 * the transmit queue.
 */
bool Console_HasOverflowed(void)
{
    return transmitOverflow;
}


/*
 * Clear the console overflow indicator.
 */
void Console_ClearOverflow(void)
{
    transmitOverflow = false;
}
