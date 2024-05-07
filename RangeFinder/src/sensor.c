/**************************************************************************/ /**
*
* @file sensor.c
*
* @author (Tyson Veik)
* @author (Colman Scharff)
*
* @brief Code to manage the distance sensor.
*
******************************************************************************/

/*
 * RangeFinder GroupLab assignment and starter code (c) 2023 Christopher A. Bohn
 * RangeFinder solution (c) the above-named students
 */

#include <CowPi.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sensor.h"
#include "shared_variables.h"
#include "outputs.h"
#include "interrupt_support.h"

adc_t *adc = (adc_t *)0x4004c000;
bool object_detected;
uint64_t object_distance;
uint32_t object_rate_of_approach;
int state;
cowpi_timer_t *timer;
uint32_t lower_32bits;
uint32_t upper_32bits;
uint64_t timer_counter;
uint32_t sensor_start_time;
uint32_t sensor_end_time;

void sensor_timer_interrupt_handler(void)
{
    if (state == INITIAL_START)
    {
        state = POWERING_UP;
    }
    else if (state == POWERING_UP)
    {
        state = READY;
    }
    if (state == ACTIVE_LISTENING)
    {
        object_detected = false;
        state = QUIESCENT;
    }
    else if (state == ACTIVE_DETECTED)
    {
        object_detected = true;
        alarm_requested = true;
        state = QUIESCENT;
    }
    else if (state == QUIESCENT)
    {
        state = READY;
    }
}

void echo_pin_interrupt_handler(void)
{
    reset_timer(1);
    timer = (cowpi_timer_t *)0x40054000;
    state = ACTIVE_LISTENING;
    if (digitalRead(ECHO) == LOW) {
        lower_32bits = timer->lower_word;
        upper_32bits = timer->upper_word;
        timer_counter = ((uint64_t)upper_32bits << 32 | lower_32bits);
        state = ACTIVE_DETECTED;
    }
}

void initialize_sensor(void)
{
    state = INITIAL_START;
    register_timer_ISR(1, 32768, sensor_timer_interrupt_handler);
    register_pin_ISR(1L << ECHO, echo_pin_interrupt_handler);
    object_detected = false;
    adc->control |= (1 << 20);
    adc->control |= (4 << 12);
    adc->control |= 0b11;
}

void manage_sensor(void)
{
    ping_requested = true;
    alarm_sounded = true;
    alarm_counter = 0;
    digitalWrite(TRIGGER, HIGH);
    ping_requested = false;
    uint32_t start_time = timer->lower_word;
    while ((timer->lower_word - start_time) < 10) {

    }
    digitalWrite(TRIGGER, LOW);
    while (state = ACTIVE_LISTENING) {

    }
    if (object_detected) {
        object_distance = ((timer_counter)*(256108888-121907*889));
        object_distance >>= 33;
        char distance_str[20];
        sprintf(distance_str, "Distance: %d cm", (int)(object_distance / 58));
        display_string(1, distance_str);
    } else {
        display_string(1, "No Object");
        display_string(2, "Detected");
    }
}
