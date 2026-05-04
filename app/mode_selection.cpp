#include "mode_selection.h"
#include <util/delay.h>

ModeSelector::ModeSelector()
    : led_(&DDRC, &PORTC, PC2, PC3),
      button_(&DDRD, &PIND, PD2)
{
}

bool ModeSelector::waitForButtonDuring()
{
    for (uint16_t i = 0; i < LED_ON_DELAY / POLL_INTERVAL; i++)
    {
        if (button_.isPressed())
            return true;
        _delay_ms(POLL_INTERVAL);
    }
    return false;
}

bool ModeSelector::checkModeGreen()
{
    led_.greenOn();
    if (waitForButtonDuring())
    {
        led_.off();
        return true;
    }
    led_.off();
    _delay_ms(LED_OFF_DELAY);
    return false;
}

bool ModeSelector::checkModeRed()
{
    led_.redOn();
    if (waitForButtonDuring())
    {
        led_.off();
        return true;
    }
    led_.off();
    _delay_ms(LED_OFF_DELAY);
    return false;
}

Mode ModeSelector::selectMode()
{
    _delay_ms(STARTUP_DELAY);

    if (checkModeGreen())
        return Mode::INSTRUCTION;

    _delay_ms(POLL_INTERVAL);

    if (checkModeRed())
        return Mode::RAPPORT;

    return Mode::EXECUTION;
}