#pragma once
#include <avr/io.h>

class LED
{
public:
    LED(volatile uint8_t *DDRX, volatile uint8_t *PORTx, uint8_t greenPin, uint8_t redPin);

    void off();
    void redOn();
    void greenOn();
    void amberOn();
    void blinkRed(uint8_t operand);
    void blinkGreen(uint8_t operand);
    void delayMultipleOf25Ms(uint8_t operand);

private:
    volatile uint8_t *ddr_;
    volatile uint8_t *port_;

    uint8_t GREEN_LED_MASK;
    uint8_t RED_LED_MASK;
    uint8_t FREE_LED_MASK;
};
