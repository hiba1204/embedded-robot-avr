#define F_CPU 8000000UL
#include <util/delay.h>
#include "sound.h"
#include "debug.h"

// Notes 45-58 : prescaler 256, OCR = 15625/f - 1  (max OCR = 141)
// Notes 59-81 : prescaler 64,  OCR = 62500/f - 1  (max OCR = 252)
const uint8_t Sound::ocrValues[nFrequencies] = {
    141, 133, 126, 118, 112, 105, 99, 94, 88, 83, 79, 74, 70, 66, // 45-58
    252, 238, 224, 212, 200, 189, 178, 168, 158, 149,             // 59-68
    141, 133, 126, 118, 112, 105, 99, 94, 88, 83, 79, 74, 70      // 69-81
};

Sound::Sound()
{
    PWM_MASK = (1 << PB3); // OC0A
    GROUND_MASK = (1 << PB2);

    DDRB |= PWM_MASK | GROUND_MASK;
    PORTB &= ~GROUND_MASK;
}

void Sound::play(uint8_t midiNote)
{
    if (midiNote < 45 || midiNote > 81)
        return;

    uint8_t index = midiNote - 45;
    uint8_t prescalerBits;
    if (index < LOW_FREQ_COUNT)
        prescalerBits = (1 << CS02); // prescaler 256
    else
        prescalerBits = (1 << CS01) | (1 << CS00); // prescaler 64

    timer_.startTimer0forSound(ocrValues[index], prescalerBits);
}

void Sound::stop()
{
    timer_.stopTimer0forSound();
    PORTB &= ~PWM_MASK;
}
