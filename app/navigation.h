#pragma once
#include <avr/io.h>
#include "motor.h"
#include "data.h"
#include "sensor.h"
#include "sound.h"
#include "led.h"
#include "instruction.h"
#include "timer.h"

class Navigation
{
public:
    Navigation(Motor &motor, Sound &sound, InstructionData &data, LineSensor &lineSensor, DistanceSensor &distanceSensor, Instruction &instruction);

    void followOutsideWall();
    void followInsideWall();
    void playAlertSequence();
    void turnFollowingDirection();
    void turnOppositeDirection();
    void turnIntersectionOuterWall();
    void turnIntersectionInnerWall();

    void turn90DegreesFollowingDirection();
    void reverse90DegreesFollowingDirection();
    void exitStorageRoom();
    void exitEvacuationRoom();

    void enterRoom();
    void exitParking();
    void enterParking();

    uint8_t inspectCorridor();
    uint8_t evacuateRoom();
    uint8_t inspectStorageRoom();

    bool isInParkingArea();
    bool isAtIntersection();
    bool isAtRoomEntrance();

private:
    struct DirectionConfig
    {
        uint8_t outsideWallSensor; // for line following (S1 or S5)
        uint8_t spotSensor;        // for parking spots (S2 or S4)
        uint8_t leftSpeed;
        uint8_t rightSpeed;
        uint8_t insideWallSensor; // for line following (S5 or S1)
    };

    void crossParkingSpots();
    void turnIntoParking();
    void ajustParking();
    void executionModeCompleted();

    void flashGreen();

    // void turn90DegreesFollowingDirection();
    // void reverse90DegreesFollowingDirection();

    // void exitEvacuationRoom();
    //  void exitStorageRoom();

    Motor &motor_;
    Sound &sound_;
    InstructionData &data_;
    LineSensor &lineSensor_;
    DistanceSensor &distanceSensor_;
    Instruction &instruction_;
    LED led_;
    DirectionConfig dir_;
    Timer timer_;

    static constexpr uint8_t NOTE_DELAY = 250;
    static constexpr uint8_t SILENCE_DELAY = 125;
    static constexpr uint8_t FORWARD_SPEED = 100;
    static constexpr uint8_t LEFT_TURN_SPEED = 120;
    static constexpr uint8_t RIGHT_TURN_SPEED = 120;
    static constexpr uint8_t TURN_PULSE_MS = 100;
    static constexpr uint8_t TURN_PAUSE_MS = 30;
    static constexpr uint8_t INTERSECTION_FORWARD_SPEED = 80;
    static constexpr uint16_t INTERSECTION_FORWARD_DELAY = 1600;
    static constexpr uint16_t INTERSECTION_SETTLE_DELAY = 30;
    static constexpr uint8_t BACKWARD_SPEED = 100;

    static constexpr uint16_t PARKING_REVERSE_DELAY = 3300;
    static constexpr uint16_t PARKING_ENTRY_DELAY = 1000;
    static constexpr uint8_t LOW_NOTE = 45;
    static constexpr uint16_t LOW_NOTE_DELAY = 1000;

    static constexpr uint8_t POLE_THRESHOLD = 28;
    static constexpr uint8_t POLE_SAMPLES = 5;
    static constexpr uint16_t ALERT_WAIT_DELAY = 2000;
    static constexpr uint16_t ROOM_ENTRY_FORWARD_DELAY = 3000;
    static constexpr uint16_t ROOM_ENTRANCE_DEBOUNCE_MS = 2000;
    static constexpr uint16_t ROOM_SCAN_SETTLE_DELAY = 500;
    static constexpr uint16_t ROOM_SWEEP_STEP_DELAY = 200;
    static constexpr uint8_t ROOM_SWEEP_SPEED = 120;
    static constexpr uint8_t ROOM_FORWARD_SPEED = 100;
    static constexpr uint16_t STORAGE_SCAN_FORWARD_DELAY = 3500;

    bool wasOnDamage_ = false;
    bool lastTurnRight_ = false;
    bool roomEntranceCheckInProgress_ = false;
};
