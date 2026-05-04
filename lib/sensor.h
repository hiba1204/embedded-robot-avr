#pragma once
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "can.h"

class LineSensor
{
public:
    LineSensor(); 
    bool isOnBlackLine(uint8_t pin);
    bool isAllOnBlackLine();
    
    uint8_t S1;     // uses PA0
    uint8_t S2;     // uses PA1
    uint8_t S3;     // uses PA2
    uint8_t S4;     // uses PA3
    uint8_t S5;     // uses PA4
};


class DistanceSensor
{
public:
    explicit DistanceSensor();
    
    uint16_t readRawValue();
    bool detect(uint8_t threshold, uint8_t sampleCount);

private:
    uint8_t PIN;    // uses PA5
    uint16_t last_ = 0; 
    static can adc_;
    uint8_t convertToDistance(uint16_t value);
    static constexpr uint8_t BITSHIFT = 2;
};
