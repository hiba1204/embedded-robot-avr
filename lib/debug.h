#pragma once

#include "uart.h"

#ifdef DEBUG
#define PRINT_DEBUG(x) printDebug(x)
#else
#define PRINT_DEBUG(x) ((void)0)
#endif

void printDebug(const char *message);
