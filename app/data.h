#pragma once
#include <avr/io.h>

struct InstructionData
{
    bool isClockwise = false;
    uint8_t midiNotes[3] = {0, 0, 0};
    uint8_t parkingSpot = 0;
};

struct ReportData
{
    uint8_t westCorridorDamage = 0;
    uint8_t eastCorridorDamage = 0;
    uint8_t roomAEvacuated = 0;
    uint8_t roomBObjects = 0;
    uint8_t roomCObjects = 0;
    uint8_t roomDEvacuated = 0;
};
