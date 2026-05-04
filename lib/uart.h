#pragma once
#include <avr/io.h>

class Uart
{
public:
    Uart();
    void writeUart(uint8_t data);
    uint8_t readUart();
};