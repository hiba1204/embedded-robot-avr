#pragma once
#include <avr/io.h>
#include "motor.h"
#include "sound.h"
#include "sensor.h"
#include "bytecode_interpreter.h"
#include "navigation.h"

class Execution
{
public:
    Execution();
    void playExecutionMode();

private:
    Motor motor_;
    Sound sound_;
    LineSensor lineSensor_;
    DistanceSensor distanceSensor_;
    BytecodeInterpreter interpreter_;
    Instruction instruction_;
    InstructionData data_;
    ReportData report_;

    void runRoute(Navigation &nav /*, ReportData &report*/);
    void storeCorridorData(uint8_t damage);
    void storeRoomData(uint8_t value, uint8_t roomCount);
    bool isInStorageRoom(uint8_t roomCount);
    bool isInWestCorridor();

    enum class State
    {
        INIT,
        SOUTH_TO_CORRIDOR,
        IN_CORRIDOR,
        NORTH_WALL,
        ROOM_TASK,
        SOUTH_TO_PARKING,
        ENTER_PARKING,
        DONE
    };

    State state_ = State::INIT;
    uint8_t roomCount_ = 0;
    uint8_t corridorCount_ = 0;
    uint8_t damageCount_ = 0;
};
