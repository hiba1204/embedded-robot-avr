#define F_CPU 8000000UL
#include "execution.h"
#include <util/delay.h>

Execution::Execution()
    : motor_(),
      sound_(),
      lineSensor_(),
      distanceSensor_(),
      interpreter_(),
      instruction_()
{
}

bool Execution::isInStorageRoom(uint8_t roomCount_)
{
    return roomCount_ == 1 || roomCount_ == 2;
}

bool Execution::isInWestCorridor()
{
    if (data_.isClockwise)
        return corridorCount_ == 0;
    else
        return corridorCount_ == 1;
}

void Execution::storeCorridorData(uint8_t damage)
{
    if (isInWestCorridor())
        report_.westCorridorDamage = damage;
    else
        report_.eastCorridorDamage = damage;
}

void Execution::storeRoomData(uint8_t value, uint8_t roomCount_)
{
    if (data_.isClockwise)
    {
        switch (roomCount_)
        {
        case 0:
            report_.roomAEvacuated = value;
            break;
        case 1:
            report_.roomBObjects = value;
            break;
        case 2:
            report_.roomCObjects = value;
            break;
        case 3:
            report_.roomDEvacuated = value;
            break;
        }
    }

    else
    {
        switch (roomCount_)
        {
        case 0:
            report_.roomDEvacuated = value;
            break;
        case 1:
            report_.roomCObjects = value;
            break;
        case 2:
            report_.roomBObjects = value;
            break;
        case 3:
            report_.roomAEvacuated = value;
            break;
        }
    }
}

void Execution::runRoute(Navigation &nav)
{
    switch (state_)
    {
    case State::INIT:
        nav.exitParking();
        state_ = State::SOUTH_TO_CORRIDOR;
        break;

    case State::SOUTH_TO_CORRIDOR:
        nav.followOutsideWall();
        if (nav.isAtIntersection())
        {
            nav.turnIntersectionOuterWall();
            motor_.stop();
            _delay_ms(1000);
            for (uint16_t elapsed = 0; elapsed < 1000; elapsed += 10)
            {
                nav.followOutsideWall();
                _delay_ms(10);
            }
            state_ = State::IN_CORRIDOR;
        }
        break;

    case State::IN_CORRIDOR:
        nav.followOutsideWall();
        damageCount_ += nav.inspectCorridor();
        if (nav.isAtIntersection())
        {
            storeCorridorData(damageCount_);
            damageCount_ = 0;

            corridorCount_++;
            if (corridorCount_ == 1)
            {
                nav.turnIntersectionInnerWall();
                state_ = State::NORTH_WALL;
            }
            else
            {
                nav.turnIntersectionOuterWall();
                state_ = State::ENTER_PARKING;
            }
        }
        break;

    case State::NORTH_WALL:
        nav.followInsideWall();
        if (nav.isAtRoomEntrance())
        {
            if (roomCount_ < 4)
                state_ = State::ROOM_TASK;
        }
        if (nav.isAtIntersection())
        {
            nav.turnIntersectionOuterWall();
            state_ = State::IN_CORRIDOR;
        }
        break;

    case State::ROOM_TASK:
    {
        nav.enterRoom();
        uint8_t detectionCount;
        if (isInStorageRoom(roomCount_))
            detectionCount = nav.inspectStorageRoom();
        else
            detectionCount = nav.evacuateRoom();
        storeRoomData(detectionCount, roomCount_);
        roomCount_++;
        state_ = State::NORTH_WALL;
        break;
    }

    case State::ENTER_PARKING:
        nav.enterParking();
        state_ = State::DONE;
        break;

    case State::DONE:
        break;
    }
}

void Execution::playExecutionMode()
{
    interpreter_.loadFromMemory();
    data_ = interpreter_.getData();

    Navigation nav(motor_, sound_, data_, lineSensor_, distanceSensor_, instruction_);
    ReportData report_;

    while (state_ != State::DONE)
    {
        runRoute(nav);
    }
    interpreter_.saveReport(report_);
}
