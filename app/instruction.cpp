#include "instruction.h"
#include <util/delay.h>

Instruction::Instruction()
    : led_(&DDRC, &PORTC, PC2, PC3),
      bytecodeInterpreter_(),
      data_(),
      sound_()
{
}

void Instruction::playStartUpSequence()
{
    for (uint8_t i = 0; i < GREEN_FLASH_CYCLE; i++)
        led_.blinkGreen(GREEN_FLASH_DELAY);
}

void Instruction::receiveInstructions()
{
    data_ = bytecodeInterpreter_.instructionSet();
}

void Instruction::playMidiNotes()
{
    uint8_t nMidiNotes = 3;
    for (uint8_t i = 0; i < nMidiNotes; i++)
    {
        uint8_t note = data_.midiNotes[i];
        sound_.play(note);
        _delay_ms(NOTE_DELAY);
        sound_.stop();
        _delay_ms(SILENCE_DELAY);
    }
}

void Instruction::blinkAmber()
{
    for (uint8_t i = 0; i < data_.parkingSpot; i++)
    {
        led_.amberOn();
        _delay_ms(AMBER_DELAY);
        led_.off();
        _delay_ms(AMBER_DELAY);
    }
}

void Instruction::ledOn()
{
    if (data_.isClockwise)
        led_.greenOn();
    else
        led_.redOn();

    _delay_ms(LED_ON_DELAY);
    led_.off();
}

void Instruction::instructionConfirmation()
{
    playMidiNotes();
    _delay_ms(INSTRUCTION_DELAY);
    ledOn();
    _delay_ms(INSTRUCTION_DELAY);
    blinkAmber();
}

void Instruction::playInstructionMode()
{
    playStartUpSequence();
    receiveInstructions();
    instructionConfirmation();
}