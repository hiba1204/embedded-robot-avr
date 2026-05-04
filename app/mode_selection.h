#pragma once
#include <avr/io.h>
#include "led.h"
#include "button.h"

enum class Mode
{
    INSTRUCTION,
    EXECUTION,
    RAPPORT
};

class ModeSelector
{
public:
    ModeSelector();
    Mode selectMode();

private:
    LED led_;
    Button button_;
    bool waitForButtonDuring();
    bool checkModeGreen();
    bool checkModeRed();
    static constexpr uint16_t STARTUP_DELAY = 2000;
    static constexpr uint16_t LED_ON_DELAY = 2000;
    static constexpr uint16_t LED_OFF_DELAY = 1000;
    static constexpr uint8_t POLL_INTERVAL = 10;
};