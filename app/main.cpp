#include <avr/io.h>
#define F_CPU 8000000UL
#include "mode_selection.h"
#include "instruction.h"
#include "bytecode_interpreter.h"
#include "navigation.h"
#include "sensor.h"
#include <util/delay.h>
#include "led.h"
#include "execution.h"
#include "report.h"

Motor motor;
Sound sound;
InstructionData data;
LineSensor lineSensor;
DistanceSensor distanceSensor;
Instruction instruction;
LED led(&DDRC, &PORTC, PC2, PC3);
Navigation navigation(motor, sound, data, lineSensor, distanceSensor, instruction);

int main()
{
    ModeSelector selector;
    Mode mode = selector.selectMode();

    if (mode == Mode::INSTRUCTION)
    {
        Instruction instruction;
        instruction.playInstructionMode();
    }
    else if (mode == Mode::RAPPORT)
    {
        Report report;
        report.run();
    }
    else
    {
        Execution execution;
        execution.playExecutionMode();
    }
    return 0;
}
