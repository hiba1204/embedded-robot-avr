#include "interrupt.h"
#include "button.h"

void Interrupt::initialization(void)
{
    // cli is a routine that blocks all interrupts.
    cli();

    // this procedure adjusts the ATmega324PA's EIMSK register to enable external interrupts
    EIMSK |= (1 << INT0);

    // make external interrupts sensitive to changes of the push-button by configuring the EICRA register
    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |= (1 << ISC00);

    // sei allows interrupts to be received again.
    sei();
};