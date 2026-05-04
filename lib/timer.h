#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint16_t g_isInRoomEntrance;

class Timer
{
public:
    void startTimer0forSound(uint8_t ocrValue, uint8_t prescalerBits);
    void stopTimer0forSound();

    void initializePwmTimer1();
    void adjustPwmLeftWheel(uint8_t dutyCycle);
    void adjustPwmRightWheel(uint8_t dutyCycle);
    void stopPwmLeftWheel();
    void stopPwmRightWheel();

    void startTimer2();
    void stopTimer2();
};
