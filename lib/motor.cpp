#include "motor.h"
#define F_CPU 8000000
#include <util/delay.h>

Motor::Motor()
{
    PWM_LEFT_WHEEL = (1 << PD4);  // OC1B
    PWM_RIGHT_WHEEL = (1 << PD5); // OC1A
    DIRECTION_LEFT_WHEEL = (1 << PD6);
    DIRECTION_RIGHT_WHEEL = (1 << PD7);

    DDRD |= PWM_LEFT_WHEEL | PWM_RIGHT_WHEEL | DIRECTION_LEFT_WHEEL | DIRECTION_RIGHT_WHEEL;

    timer_.initializePwmTimer1();
}

void Motor::leftWheelForward(uint8_t speed)
{
    PORTD &= ~DIRECTION_LEFT_WHEEL;
    timer_.adjustPwmLeftWheel(speed);
}

void Motor::rightWheelForward(uint8_t speed)
{
    PORTD &= ~DIRECTION_RIGHT_WHEEL;
    timer_.adjustPwmRightWheel(speed);
}

void Motor::moveForward(uint8_t speed)
{
    leftWheelForward(speed);
    rightWheelForward(speed);
}

void Motor::leftWheelBackward(uint8_t speed)
{
    PORTD |= DIRECTION_LEFT_WHEEL;
    timer_.adjustPwmLeftWheel(speed);
}

void Motor::rightWheelBackward(uint8_t speed)
{
    PORTD |= DIRECTION_RIGHT_WHEEL;
    timer_.adjustPwmRightWheel(speed);
}

void Motor::moveBackward(uint8_t speed)
{
    leftWheelBackward(speed);
    rightWheelBackward(speed);
}

void Motor::stopLeftWheel()
{
    timer_.stopPwmLeftWheel();
}

void Motor::stopRightWheel()
{
    timer_.stopPwmRightWheel();
}

void Motor::turnLeft(uint8_t leftSpeed, uint8_t rightSpeed)
{
    rightWheelForward(rightSpeed);
    leftWheelBackward(leftSpeed);
}

void Motor::turnRight(uint8_t leftSpeed, uint8_t rightSpeed)
{
    leftWheelForward(leftSpeed);
    rightWheelBackward(rightSpeed);
}

void Motor::stop()
{
    stopLeftWheel();
    stopRightWheel();
}

// void Motor::reverseLeft(uint8_t speed)
// {
//     leftWheelBackward(speed);
//     rightWheelForward(speed);
// }

// void Motor::reverseRight(uint8_t speed)
// {
//     rightWheelBackward(speed);
//     leftWheelForward(speed);
// }

void Motor::turnLeft90Degrees()
{
    uint16_t pulseMs = TURN_LEFT_90_DEGREES_DELAY / TURN_90_PULSES;
    for (uint8_t i = 0; i < TURN_90_PULSES; i++)
    {
        turnLeft(TURN_LEFT_90_DEGREES_SPEED, TURN_LEFT_90_DEGREES_SPEED);
        _delay_ms(pulseMs);
        stop();
        _delay_ms(TURN_90_PAUSE_MS);
    }
}

void Motor::turnRight90Degrees()
{
    uint16_t pulseMs = TURN_RIGHT_90_DEGREES_DELAY / TURN_90_PULSES;
    for (uint8_t i = 0; i < TURN_90_PULSES; i++)
    {
        turnRight(TURN_RIGHT_90_DEGREES_SPEED, TURN_RIGHT_90_DEGREES_SPEED);
        _delay_ms(pulseMs);
        stop();
        _delay_ms(TURN_90_PAUSE_MS);
    }
}

// void Motor::reverseLeft90Degrees()
// {
//     reverseLeft(REVERSE_90_DEGREES_SPEED);
//     _delay_ms(REVERSE_90_DEGREES_DELAY);
//     stop();
// }

// void Motor::reverseRight90Degrees()
// {
//     reverseRight(TURN_90_DEGREES_SPEED);
//     _delay_ms(TURN_90_DEGREES_DELAY);
//     stop();
// }

void Motor::testMoveForward(uint8_t leftSpeed, uint8_t rightSpeed)
{
    leftWheelForward(leftSpeed);
    rightWheelForward(rightSpeed);
}
