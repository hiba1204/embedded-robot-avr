#include "button.h"
#define F_CPU 8000000
#include <util/delay.h>

Button::Button(volatile uint8_t *DDRX, volatile uint8_t *PINX, uint8_t pin)
{
    BUTTON_MASK = (1 << pin);
    *DDRX &= ~(BUTTON_MASK); // entrée
    pin_ = PINX;
}

bool Button::isPressed()
{
    if (*pin_ & BUTTON_MASK)
    {
        _delay_ms(DEBOUNCE_DELAY);
        return (*pin_ & BUTTON_MASK);
    }
    return false;
}