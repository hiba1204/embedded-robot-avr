#include "report.h"
#include <util/delay.h>

Report::Report()
    : led_(&DDRC, &PORTC, PC2, PC3),
      uart_(),
      interpreter_(),
      reportData_()
{
}

void Report::run()
{
    signalStart();
    readFromEeprom();
    sendReport();
}

void Report::signalStart()
{
    for (uint8_t i = 0; i < RED_FLASH_CYCLES; i++)
        led_.blinkRed(RED_FLASH_DELAY);
}

void Report::readFromEeprom()
{
    reportData_ = interpreter_.loadReport();
}

void Report::writeString(const char *str)
{
    while (*str)
        uart_.writeUart(*str++);
}

void Report::writeNumber(uint8_t number)
{
    if (number >= 10)
        uart_.writeUart('0' + number / 10);
    uart_.writeUart('0' + number % 10);
}

void Report::writeNewline()
{
    uart_.writeUart('\r');
    uart_.writeUart('\n');
}

void Report::writeLine(const char *location,
                       uint8_t count,
                       const char *caseSingular,
                       const char *casePlural)
{
    writeString(location);

    if (count == 0)
    {
        writeString("oui");
        writeNewline();
        return;
    }

    writeString("non           ");
    writeNumber(count);
    writeString(" ");

    if (count == 1)
        writeString(caseSingular);
    else
        writeString(casePlural);

    writeNewline();
}

void Report::sendReport()
{
    _delay_ms(300);
    writeString("Rapport de conformite");
    writeNewline();
    writeNewline();

    writeString("Emplacement      conformite   detail");
    writeNewline();
    writeString("------------------------------------------------------------");
    writeNewline();
    writeNewline();

    writeLine("Local A          ", reportData_.roomAEvacuated,   "personne",        "personnes");
    writeLine("Local B          ", reportData_.roomBObjects,    "objet",           "objets");
    writeLine("Local C          ", reportData_.roomCObjects,    "objet",           "objets");
    writeLine("Local D          ", reportData_.roomDEvacuated,   "personne",        "personnes");
    writeLine("Couloir OUEST    ", reportData_.westCorridorDamage, "zone endommagee", "zones endommagees");
    writeLine("Couloir EST      ", reportData_.eastCorridorDamage,   "zone endommagee", "zones endommagees");
}


