#include "navigation.h"
#include <util/delay.h>

volatile uint16_t g_isInRoomEntrance = 0;

ISR(TIMER2_COMPA_vect)
{
    g_isInRoomEntrance++;
}

Navigation::Navigation(Motor &motor, Sound &sound, InstructionData &data, LineSensor &lineSensor, DistanceSensor &distanceSensor, Instruction &instruction)
    : motor_(motor),
      sound_(sound),
      data_(data),
      lineSensor_(lineSensor),
      distanceSensor_(distanceSensor),
      instruction_(instruction),
      led_(&DDRC, &PORTC, PC2, PC3),
      dir_(data.isClockwise
               ? DirectionConfig{lineSensor.S1, lineSensor.S2, 100, 120, lineSensor.S5}
               : DirectionConfig{lineSensor.S5, lineSensor.S4, 110, 90, lineSensor.S1})
{
}

void Navigation::playAlertSequence()
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
void Navigation::flashGreen() { instruction_.playStartUpSequence(); }

bool Navigation::isAtIntersection()
{
    return lineSensor_.isAllOnBlackLine();
}

bool Navigation::isInParkingArea()
{
    return lineSensor_.isOnBlackLine(dir_.spotSensor);
}

bool Navigation::isAtRoomEntrance()
{
    bool noInnerWall = !lineSensor_.isOnBlackLine(dir_.insideWallSensor);

    if (noInnerWall && !roomEntranceCheckInProgress_)
    {
        roomEntranceCheckInProgress_ = true;
        timer_.startTimer2();
    }
    else if (!noInnerWall && roomEntranceCheckInProgress_)
    {
        timer_.stopTimer2();
        roomEntranceCheckInProgress_ = false;
    }

    if (roomEntranceCheckInProgress_ && g_isInRoomEntrance >= ROOM_ENTRANCE_DEBOUNCE_MS)
    {
        timer_.stopTimer2();
        roomEntranceCheckInProgress_ = false;
        motor_.stop();
        return true;
    }
    return false;
}

void Navigation::followOutsideWall()
{
    if (!lineSensor_.isOnBlackLine(dir_.outsideWallSensor))
        motor_.testMoveForward(dir_.leftSpeed, dir_.rightSpeed);
    else
        motor_.testMoveForward(dir_.rightSpeed, dir_.leftSpeed);
}

void Navigation::followInsideWall()
{
    if (!lineSensor_.isOnBlackLine(dir_.insideWallSensor))
        motor_.testMoveForward(dir_.rightSpeed, dir_.leftSpeed);
    else
        motor_.testMoveForward(dir_.leftSpeed, dir_.rightSpeed);
    _delay_ms(TURN_PULSE_MS);
    motor_.stop();
    _delay_ms(TURN_PAUSE_MS);
}

void Navigation::turn90DegreesFollowingDirection()
{
    if (data_.isClockwise)
        motor_.turnRight90Degrees();
    else
        motor_.turnLeft90Degrees();
}

void Navigation::reverse90DegreesFollowingDirection()
{
    if (data_.isClockwise)
        motor_.turnLeft90Degrees();
    else
        motor_.turnRight90Degrees();
}

void Navigation::turnFollowingDirection()
{
    if (data_.isClockwise)
        motor_.turnRight(LEFT_TURN_SPEED, RIGHT_TURN_SPEED);
    else
        motor_.turnLeft(LEFT_TURN_SPEED, RIGHT_TURN_SPEED);
    _delay_ms(TURN_PULSE_MS);
    motor_.stop();
    _delay_ms(TURN_PAUSE_MS);
}

void Navigation::turnOppositeDirection()
{
    if (data_.isClockwise)
        motor_.turnLeft(LEFT_TURN_SPEED, RIGHT_TURN_SPEED);
    else
        motor_.turnRight(LEFT_TURN_SPEED, RIGHT_TURN_SPEED);
    _delay_ms(TURN_PULSE_MS);
    motor_.stop();
    _delay_ms(TURN_PAUSE_MS);
}

void Navigation::turnIntersectionOuterWall()
{
    motor_.testMoveForward(INTERSECTION_FORWARD_SPEED, INTERSECTION_FORWARD_SPEED);
    _delay_ms(INTERSECTION_FORWARD_DELAY);
    do
    {
        turnFollowingDirection();
        _delay_ms(INTERSECTION_SETTLE_DELAY);
    } while (!lineSensor_.isOnBlackLine(dir_.outsideWallSensor) || lineSensor_.isOnBlackLine(dir_.spotSensor));
}

void Navigation::turnIntersectionInnerWall()
{
    motor_.testMoveForward(INTERSECTION_FORWARD_SPEED, INTERSECTION_FORWARD_SPEED);
    _delay_ms(INTERSECTION_FORWARD_DELAY);
    do
    {
        turnFollowingDirection();
        _delay_ms(INTERSECTION_SETTLE_DELAY);
    } while (!lineSensor_.isOnBlackLine(dir_.insideWallSensor));
    do
    {
        turnFollowingDirection();
        _delay_ms(INTERSECTION_SETTLE_DELAY);
    } while (lineSensor_.isOnBlackLine(dir_.insideWallSensor));
    do
    {
        turnFollowingDirection();
        _delay_ms(INTERSECTION_SETTLE_DELAY);
    } while (!lineSensor_.isOnBlackLine(dir_.insideWallSensor));
}

void Navigation::crossParkingSpots()
{
    uint8_t target = data_.parkingSpot;
    uint8_t spotsToCross = data_.isClockwise ? (6 - target) : (target + 1);

    for (uint8_t i = 0; i < spotsToCross - 1; ++i)
    {
        while (!lineSensor_.isOnBlackLine(dir_.spotSensor))
            followOutsideWall();
        while (lineSensor_.isOnBlackLine(dir_.spotSensor))
            followOutsideWall();
    }
    turn90DegreesFollowingDirection();
    motor_.stop();
}

void Navigation::ajustParking()
{
    do
    {
        motor_.moveForward(FORWARD_SPEED);
    } while (!lineSensor_.isAllOnBlackLine());
    motor_.moveBackward(BACKWARD_SPEED);
    _delay_ms(PARKING_ENTRY_DELAY);
    motor_.stop();
}

void Navigation::executionModeCompleted()
{
    sound_.play(LOW_NOTE);
    _delay_ms(LOW_NOTE_DELAY);
    _delay_ms(50);
    sound_.stop();
}

void Navigation::exitEvacuationRoom()
{
    do
    {
        turnOppositeDirection();
    } while (!lineSensor_.isOnBlackLine(dir_.insideWallSensor));
    motor_.stop();
    _delay_ms(1000);
}

void Navigation::exitStorageRoom()
{
    reverse90DegreesFollowingDirection();
    reverse90DegreesFollowingDirection();
    while (!isAtIntersection())
        motor_.moveForward(FORWARD_SPEED);
    turnIntersectionInnerWall();
}

void Navigation::exitParking()
{
    motor_.moveBackward(BACKWARD_SPEED);
    _delay_ms(PARKING_REVERSE_DELAY);
    reverse90DegreesFollowingDirection();
    followOutsideWall();
}

uint8_t Navigation::inspectCorridor()
{
    uint8_t damageCount = 0;
    bool wasOnDamage = false;

    bool isOnDamage = (lineSensor_.isOnBlackLine(lineSensor_.S2) && !lineSensor_.isOnBlackLine(lineSensor_.S4)) || (lineSensor_.isOnBlackLine(lineSensor_.S4) && !lineSensor_.isOnBlackLine(lineSensor_.S2));
    if (isOnDamage && !wasOnDamage)
    {
        _delay_ms(100);
        isOnDamage = (lineSensor_.isOnBlackLine(lineSensor_.S2) && !lineSensor_.isOnBlackLine(lineSensor_.S4)) || (lineSensor_.isOnBlackLine(lineSensor_.S4) && !lineSensor_.isOnBlackLine(lineSensor_.S2));
        if (isOnDamage)
        {
            damageCount++;
            led_.redOn();
        }
    }
    else if (!isOnDamage)
        led_.off();

    wasOnDamage = isOnDamage;
    return damageCount;
}

void Navigation::enterRoom()
{
    do
    {
        motor_.testMoveForward(85, 80);
        _delay_ms(400);
        motor_.stop();
        _delay_ms(40);
    } while (!lineSensor_.isOnBlackLine(dir_.insideWallSensor) && !lineSensor_.isOnBlackLine(dir_.spotSensor));
    motor_.testMoveForward(85, 100);
    _delay_ms(1000);
    motor_.stop();
    do
    {
        turnFollowingDirection();
        _delay_ms(INTERSECTION_SETTLE_DELAY);
    } while (lineSensor_.isOnBlackLine(dir_.insideWallSensor));
    for (uint8_t i = 0; i < 10; i++)
    {
        turnFollowingDirection();
    }
    motor_.moveBackward(BACKWARD_SPEED);
    _delay_ms(1000);
    do
    {
        turnFollowingDirection();
    } while (!lineSensor_.isOnBlackLine(dir_.insideWallSensor));
    motor_.stop();
}

uint8_t Navigation::evacuateRoom()
{
    uint8_t peopleEvacuated = 0;

    while (!lineSensor_.isOnBlackLine(dir_.outsideWallSensor))
    {
        turnOppositeDirection();
        if (distanceSensor_.detect(POLE_THRESHOLD, POLE_SAMPLES))
        {
            do
            {
                playAlertSequence();
                _delay_ms(ALERT_WAIT_DELAY);
            } while (distanceSensor_.detect(POLE_THRESHOLD, POLE_SAMPLES));
            flashGreen();

            peopleEvacuated++;
        }
    }
    exitEvacuationRoom();
    return peopleEvacuated;
}

uint8_t Navigation::inspectStorageRoom()
{
    reverse90DegreesFollowingDirection();
    uint8_t objectCount = 0;
    bool wasOnObject = false;
    motor_.stop();
    _delay_ms(500);
    for (uint16_t elapsed = 0; elapsed < STORAGE_SCAN_FORWARD_DELAY; elapsed += 25)
    {
        motor_.moveForward(ROOM_FORWARD_SPEED);
        uint8_t sensorsOn = lineSensor_.isOnBlackLine(lineSensor_.S1) + lineSensor_.isOnBlackLine(lineSensor_.S2) + lineSensor_.isOnBlackLine(lineSensor_.S3) + lineSensor_.isOnBlackLine(lineSensor_.S4) + lineSensor_.isOnBlackLine(lineSensor_.S5);
        bool isOnObject = sensorsOn >= 3;
        if (isOnObject)
            led_.redOn();
        else
            led_.off();
        if (isOnObject && !wasOnObject)
            objectCount++;

        wasOnObject = isOnObject;
        _delay_ms(25);
    }
    motor_.stop();
    led_.off();
    _delay_ms(1000);

    exitStorageRoom();
    return objectCount;
}

void Navigation::enterParking()
{
    crossParkingSpots();
    ajustParking();
    executionModeCompleted();
}