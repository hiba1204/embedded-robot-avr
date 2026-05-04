#include "sensor.h"

LineSensor::LineSensor()
    : S1(1 << PA0),
      S2(1 << PA1),
      S3(1 << PA2),
      S4(1 << PA3),
      S5(1 << PA4)
{
    DDRA &= ~(S1| S2 | S3 | S4 | S5);
    PORTA &= ~(S1| S2 | S3 | S4 | S5);
}

bool LineSensor::isOnBlackLine(uint8_t pin)
{
    return (PINA & pin) != 0;
}

bool LineSensor::isAllOnBlackLine()
{
    return isOnBlackLine(S1) && isOnBlackLine(S2) && isOnBlackLine(S3) && isOnBlackLine(S4) && isOnBlackLine(S5);
}

can DistanceSensor::adc_;

DistanceSensor::DistanceSensor()
    : PIN(PA5)
{
    DDRA &= ~(1 << PA5);
    PORTA &= ~(1 << PA5);
}

uint16_t DistanceSensor::readRawValue()
{
    uint16_t rawValue = adc_.lecture(PIN);
    return rawValue >> BITSHIFT;
}

bool DistanceSensor::detect(uint8_t threshold, uint8_t sampleCount)
{
    if (sampleCount == 0)
    {
        return false;
    }

    uint32_t sum = 0;
    for (uint8_t i = 0; i < sampleCount; ++i)
    {
        sum += readRawValue();
        _delay_ms(5);
    }

    return ((sum / sampleCount) > threshold);
}

uint8_t DistanceSensor::convertToDistance(uint16_t value)
{
    if (value == 0)
    {
        return 255;
    }

    if (value > 255)
    {
        return 255;
    }

    return static_cast<uint8_t>(value);
}
