#pragma once
#include <avr/io.h>
#include "memoire_24.h"
#include "uart.h"
#include "data.h"

class BytecodeInterpreter
{
public:
    BytecodeInterpreter();
    void saveToMemory();
    void loadFromMemory();
    InstructionData instructionSet();
    InstructionData getData() const { return data_; }

    void saveReport(const ReportData &report);
    ReportData loadReport();

private:
    uint16_t readProgramSize();
    bool isValidProgramSize(uint16_t programSize);
    void readInstructions(uint8_t opcodes[], uint8_t operands[]);
    bool parseInstructions();
    void fillInstructionData(const uint8_t opcodes[], const uint8_t operands[]);

    Uart uart_;
    InstructionData data_;
    Memoire24CXXX memory_;

    uint8_t static constexpr INSTRUCTION_COUNT = 7;
    uint8_t static constexpr DIRECTION_OPCODE_IDX = 1;
    uint8_t static constexpr FIRST_MIDI_NOTE_OPERAND_IDX = 2;
    uint8_t static constexpr SECOND_MIDI_NOTE_OPERAND_IDX = 3;
    uint8_t static constexpr THIRD_MIDI_NOTE_OPERAND_IDX = 4;
    uint8_t static constexpr PARKING_SPOT_OPERAND_IDX = 5;
    uint8_t static constexpr WRITING_DELAY = 10;

    uint16_t static constexpr DIRECTION_ADDR = 0x0000;
    uint16_t static constexpr FIRST_MIDI_NOTE_ADDR = 0x0001;
    uint16_t static constexpr SECOND_MIDI_NOTE_ADDR = 0x0002;
    uint16_t static constexpr THIRD_MIDI_NOTE_ADDR = 0x0003;
    uint16_t static constexpr PARKING_SPOT_ADDR = 0x0004;

    uint16_t static constexpr WEST_CORRIDOR_ADDR = 0x0010;
    uint16_t static constexpr EAST_CORRIDOR_ADDR = 0x0011;
    uint16_t static constexpr ROOM_A_ADDR = 0x0012;
    uint16_t static constexpr ROOM_B_ADDR = 0x0013;
    uint16_t static constexpr ROOM_C_ADDR = 0x0014;
    uint16_t static constexpr ROOM_D_ADDR = 0x0015;
};
