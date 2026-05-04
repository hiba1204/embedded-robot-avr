#pragma once

#include <avr/io.h>

class Button
{
public:
    Button(volatile uint8_t *DDRX, volatile uint8_t *PINX, uint8_t pin);
    bool isPressed();

private:
    volatile uint8_t *pin_;
    uint8_t BUTTON_MASK;
    static constexpr uint8_t DEBOUNCE_DELAY = 10; // ms
};
