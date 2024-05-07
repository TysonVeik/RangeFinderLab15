/**************************************************************************//**
 *
 * @file alarm.c
 *
 * @author (Tyson Veik)
 * @author (Colman Scharff)
 *
 * @brief Code to manage the piezodisc and LEDs.
 *
 ******************************************************************************/

/*
 * RangeFinder GroupLab assignment and starter code (c) 2023 Christopher A. Bohn
 * RangeFinder solution (c) the above-named students
 */

#include <CowPi.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "alarm.h"
#include "shared_variables.h"
#include "outputs.h"
#include "interrupt_support.h"

const unsigned int on_period = 500;
volatile unsigned int total_period = 50000;


void alarm_interrupt_handler(void) 
{
    // every other invocation will place a 1 on the BUZZER pin and will place a 0 on the alternate invocations
}

void initialize_alarm(void)
{
    register_timer_ISR(1,100,alarm_interrupt_handler);
}

void manage_alarm(void)
{

}
