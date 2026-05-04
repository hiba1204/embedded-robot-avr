#define F_CPU 8000000UL
#include "bytecode_interpreter.h"
#include <util/delay.h>

#define DBT 0x01
#define ATT 0x02
#define SGO 0x48
#define TRD 0x64
#define TRG 0x65
#define FIN 0xFF

#define UNIT_DELAY 25

void delayMultipleOf25Ms(uint8_t operand)
{
    for (uint8_t i = 0; i < operand; i++)
        _delay_ms(UNIT_DELAY);
}

BytecodeInterpreter::BytecodeInterpreter()
    : uart_(),
      memory_()
{
}

uint16_t BytecodeInterpreter::readProgramSize()
{
    uint8_t msb = uart_.readUart();
    uint8_t lsb = uart_.readUart();
    return (static_cast<uint16_t>(msb) << 8) | lsb;
}

void BytecodeInterpreter::readInstructions(uint8_t opcodes[], uint8_t operands[])
{
    for (uint8_t i = 0; i < INSTRUCTION_COUNT; i++)
    {
        opcodes[i] = uart_.readUart();
        operands[i] = uart_.readUart();
    }
}

void BytecodeInterpreter::fillInstructionData(const uint8_t opcodes[], const uint8_t operands[])
{
    data_.isClockwise = (opcodes[DIRECTION_OPCODE_IDX] == TRD);
    data_.midiNotes[0] = operands[FIRST_MIDI_NOTE_OPERAND_IDX];
    data_.midiNotes[1] = operands[SECOND_MIDI_NOTE_OPERAND_IDX];
    data_.midiNotes[2] = operands[THIRD_MIDI_NOTE_OPERAND_IDX];
    data_.parkingSpot = operands[PARKING_SPOT_OPERAND_IDX];
}

bool BytecodeInterpreter::parseInstructions()
{
    data_ = InstructionData{};
    uint16_t programSize = readProgramSize();

    uint8_t opcodes[INSTRUCTION_COUNT];
    uint8_t operands[INSTRUCTION_COUNT];
    readInstructions(opcodes, operands);
    fillInstructionData(opcodes, operands);
    return true;
}

InstructionData BytecodeInterpreter::instructionSet()
{
    while (!parseInstructions())
    {
        // wait
    }
    saveToMemory();
    return data_;
}

void BytecodeInterpreter::saveToMemory()
{
    memory_.ecriture(DIRECTION_ADDR, (uint8_t)data_.isClockwise);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(FIRST_MIDI_NOTE_ADDR, data_.midiNotes[0]);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(SECOND_MIDI_NOTE_ADDR, data_.midiNotes[1]);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(THIRD_MIDI_NOTE_ADDR, data_.midiNotes[2]);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(PARKING_SPOT_ADDR, data_.parkingSpot);
    _delay_ms(WRITING_DELAY);
}

void BytecodeInterpreter::loadFromMemory()
{
    uint8_t clockwise = 0;
    memory_.lecture(DIRECTION_ADDR, &clockwise);
    data_.isClockwise = clockwise;
    memory_.lecture(FIRST_MIDI_NOTE_ADDR, &data_.midiNotes[0]);
    memory_.lecture(SECOND_MIDI_NOTE_ADDR, &data_.midiNotes[1]);
    memory_.lecture(THIRD_MIDI_NOTE_ADDR, &data_.midiNotes[2]);
    memory_.lecture(PARKING_SPOT_ADDR, &data_.parkingSpot);
}

void BytecodeInterpreter::saveReport(const ReportData &report)
{
    memory_.ecriture(WEST_CORRIDOR_ADDR, report.westCorridorDamage);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(EAST_CORRIDOR_ADDR, report.eastCorridorDamage);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(ROOM_A_ADDR, report.roomAEvacuated);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(ROOM_B_ADDR, report.roomBObjects);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(ROOM_C_ADDR, report.roomCObjects);
    _delay_ms(WRITING_DELAY);
    memory_.ecriture(ROOM_D_ADDR, report.roomDEvacuated);
    _delay_ms(WRITING_DELAY);
}

ReportData BytecodeInterpreter::loadReport()
{
    ReportData report;
    memory_.lecture(WEST_CORRIDOR_ADDR, &report.westCorridorDamage);
    memory_.lecture(EAST_CORRIDOR_ADDR, &report.eastCorridorDamage);
    memory_.lecture(ROOM_A_ADDR, &report.roomAEvacuated);
    memory_.lecture(ROOM_B_ADDR, &report.roomBObjects);
    memory_.lecture(ROOM_C_ADDR, &report.roomCObjects);
    memory_.lecture(ROOM_D_ADDR, &report.roomDEvacuated);
    return report;
}