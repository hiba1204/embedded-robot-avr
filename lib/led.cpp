#include "led.h"
#define F_CPU 8000000
#include <util/delay.h>

LED::LED(volatile uint8_t *DDRX, volatile uint8_t *PORTX, uint8_t greenPin, uint8_t redPin)
{
    GREEN_LED_MASK = (1 << greenPin);
    RED_LED_MASK = (1 << redPin);
    FREE_LED_MASK = RED_LED_MASK | GREEN_LED_MASK;

    port_ = PORTX;
    ddr_ = DDRX;
    *ddr_ |= FREE_LED_MASK;
}

void LED::off() { *port_ &= ~FREE_LED_MASK; }

void LED::greenOn()
{
    *port_ &= ~FREE_LED_MASK;
    *port_ |= GREEN_LED_MASK;
}

void LED::redOn()
{
    *port_ &= ~FREE_LED_MASK;
    *port_ |= RED_LED_MASK;
}

void LED::amberOn()
{
    greenOn();
    _delay_ms(1);
    redOn();
    _delay_ms(1);
}

void LED::delayMultipleOf25Ms(uint8_t operand)
{
    for (uint8_t i = 0; i < operand; i++)
        _delay_ms(1);
}

void LED::blinkGreen(uint8_t operand)
{
    greenOn();
    delayMultipleOf25Ms(operand);
    off();
    delayMultipleOf25Ms(operand);
}

void LED::blinkRed(uint8_t operand)
{
    redOn();
    delayMultipleOf25Ms(operand);
    off();
    delayMultipleOf25Ms(operand);
}
