
#pragma once
#define F_CPU 8000000UL
#include <avr/io.h>
#include "led.h"
#include "uart.h"
#include "data.h"
#include "bytecode_interpreter.h"

class Report
{
public:
    Report();
    void run();

private:
    void signalStart();
    void readFromEeprom();
    void sendReport();

    void writeString(const char *str);
    void writeNumber(uint8_t number);
    void writeNewline();
    void writeLine(const char *location,
                   uint8_t count,
                   const char *caseSingular,
                   const char *casePlural);

    static constexpr uint8_t RED_FLASH_DELAY  = 125;
    static constexpr uint8_t RED_FLASH_CYCLES = 8;

    LED led_;
    Uart uart_;
    BytecodeInterpreter interpreter_;
    ReportData reportData_;
};

