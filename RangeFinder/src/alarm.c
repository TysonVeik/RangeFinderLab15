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
volatile unsigned int isr_trigger_count = 0;
volatile bool sound_alarm = false;

void timer_interrupt_handler(void)
{
    isr_trigger_count++;
    if (isr_trigger_count >= total_period)
    {
        isr_trigger_count = 0;
    }

    if (sound_alarm && isr_trigger_count < on_period)
    {

        digitalWrite(BUZZER, HIGH);
        cowpi_illuminate_right_led();
    }
    else
    {
        digitalWrite(BUZZER, LOW);
        cowpi_deluminate_right_led();
        sound_alarm = false;
    }
}

void initialize_alarm(void)
{
    register_timer_interrupt_handler(timer_interrupt_handler);
    configure_timer_interrupt(100);
}

void manage_alarm(void)
{
    if (mode == CONTINUOUS_TONE)
    {
        generate_continuous_tone();
    }
    else
    {
        stop_continuous_tone();
    }
}

void handle_ping_request(void)
{
    sound_alarm = true;
    isr_trigger_count = 0;
    ping_requested = false;
}