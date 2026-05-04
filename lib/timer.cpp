#include "timer.h"

void Timer::startTimer0forSound(uint8_t ocrValue, uint8_t prescalerBits)
{
    TCNT0 = 0;
    OCR0A = ocrValue;
    TCCR0A = (1 << COM0A0) | (1 << WGM01); // Toggle OC0A on compare match, CTC mode
    TCCR0B = prescalerBits;
    TIMSK0 = 0;
}

void Timer::stopTimer0forSound()
{
    TCCR0A &= ~(1 << COM0A0);
    OCR0A = 0;
}

void Timer::initializePwmTimer1()
{
    // 8-bit Phase Correct PWM on Timer 1, OC1A = PD5, OC1B = PD4
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << CS12); // Prescaler = 256
    TCCR1C = 0;
}

void Timer::adjustPwmLeftWheel(uint8_t dutyCycle) { OCR1B = dutyCycle; }
void Timer::adjustPwmRightWheel(uint8_t dutyCycle) { OCR1A = dutyCycle; }
void Timer::stopPwmLeftWheel() { OCR1B = 0; }
void Timer::stopPwmRightWheel() { OCR1A = 0; }

void Timer::startTimer2()
{
    cli();
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = 124;           // 1ms tick: 8MHz / (64 * (124+1)) = 1000Hz
    TCCR2A = (1 << WGM21); // CTC mode
    TCCR2B = (1 << CS22);  // prescaler 64
    g_isInRoomEntrance = 0;
    TIMSK2 = (1 << OCIE2A);
    sei();
}

void Timer::stopTimer2()
{
    TIMSK2 &= ~(1 << OCIE2A);
    TCCR2B = 0;
}
