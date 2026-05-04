#pragma once
#include <avr/io.h>
#include "timer.h"

class Motor
{
public:
    Motor();
    void moveForward(uint8_t speed);
    void moveBackward(uint8_t speed);
    void turnLeft(uint8_t leftSpeed, uint8_t rightSpeed);
    void turnRight(uint8_t leftSpeed, uint8_t rightSpeed);
    void stop();
    void reverseRight(uint8_t speed);
    void reverseLeft(uint8_t speed);

    void turnRight90Degrees();
    void turnLeft90Degrees();
    void reverseLeft90Degrees();
    void reverseRight90Degrees();
    void testMoveForward(uint8_t leftSpeed, uint8_t rightSpeed);

private:
    uint8_t PWM_LEFT_WHEEL;        // uses PD4 (OC1B)
    uint8_t PWM_RIGHT_WHEEL;       // uses PD5 (OC1A)
    uint8_t DIRECTION_LEFT_WHEEL;  // uses PD6
    uint8_t DIRECTION_RIGHT_WHEEL; // uses PD7
    Timer timer_;

    void leftWheelForward(uint8_t speed);
    void rightWheelForward(uint8_t speed);

    void leftWheelBackward(uint8_t speed);
    void rightWheelBackward(uint8_t speed);

    void stopLeftWheel();
    void stopRightWheel();

    uint8_t static constexpr TURN_RIGHT_90_DEGREES_SPEED = 150;
    uint16_t static constexpr TURN_RIGHT_90_DEGREES_DELAY = 800;
    uint8_t static constexpr TURN_LEFT_90_DEGREES_SPEED = 150;
    uint16_t static constexpr TURN_LEFT_90_DEGREES_DELAY = 980;
    uint8_t static constexpr TURN_90_PULSES = 10;
    uint8_t static constexpr TURN_90_PAUSE_MS = 20;
};