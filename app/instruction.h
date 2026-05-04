#pragma once
#include <avr/io.h>
#include "led.h"
#include "bytecode_interpreter.h"
#include "sound.h"

class Instruction
{
public:
    Instruction();
    void playInstructionMode();
    void playMidiNotes();
    void playStartUpSequence();
    void receiveInstructions();

private:
    LED led_;
    BytecodeInterpreter bytecodeInterpreter_;
    InstructionData data_;
    Sound sound_;

    void instructionConfirmation();
    void blinkAmber();
    void ledOn();
    static constexpr uint16_t GREEN_FLASH_DELAY = 125;
    static constexpr uint8_t GREEN_FLASH_CYCLE = 8;
    static constexpr uint8_t NOTE_DELAY = 250;
    static constexpr uint8_t SILENCE_DELAY = 125;
    static constexpr uint16_t AMBER_DELAY = 500;
    static constexpr uint16_t INSTRUCTION_DELAY = 2000;
    static constexpr uint16_t LED_ON_DELAY = 2000;
};
