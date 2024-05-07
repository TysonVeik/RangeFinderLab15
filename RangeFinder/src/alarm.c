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
bool alarm_activator = false;
int alarm_counter = 0;
bool alarm_sounded = false;

void alarm_interrupt_handler(void) {
    alarm_counter++;
    if (mode == CONTINUOUS_TONE) {
        if (alarm_activator) {
            digitalWrite(BUZZER, HIGH);
        } else {
            digitalWrite(BUZZER, LOW);
        }
        alarm_activator = !alarm_activator;
    }
    if (alarm_counter >= total_period) {
        alarm_counter = 0;
    }
    if (alarm_sounded) {
        if (alarm_counter < on_period) {
            digitalWrite(BUZZER, HIGH);
            cowpi_illuminate_right();
            
        } else if (alarm_counter > on_period){
            alarm_sounded = false;
            cowpi_deluminate_right_led();
        }
    }
}

void initialize_alarm(void) {
  register_timer_ISR(1, 100, alarm_interrupt_handler);
}

void manage_alarm(void) {

}
