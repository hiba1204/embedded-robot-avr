#include "debug.h"

#ifdef DEBUG

void printDebug(const char *message)
{
    static Uart uart;
    while(*message)
        uart.writeUart(*message++);
}

#endif
