#pragma once
#include "timer.h"
#include <avr/io.h>

class Sound
{
public:
    Sound();
    void play(uint8_t midiNote);
    void stop();

private:
    uint8_t PWM_MASK;    // uses PB3 (OC0A)
    uint8_t GROUND_MASK; // uses PB2

    static const uint8_t nFrequencies = 37;
    static const uint8_t ocrValues[nFrequencies];

    uint8_t static constexpr LOW_FREQ_COUNT = 14; // note 58

    Timer timer_;
};
