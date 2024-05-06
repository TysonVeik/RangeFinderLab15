/**************************************************************************//**
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
uint32_t object_distance;
uint32_t object_rate_of_approach;
int state;

void sensor_timer_interrupt_handler(void)
{
    if (state == ACTIVE_LISTENING)
    {
        object_detected = false;
        state = QUIESCENT;
    }
    else if (state == ACTIVE_DETECTED)
    {
        object_detected = true;
        state = QUIESCENT;
    }
    else if (state == QUIESCENT)
    {
        state = READY;
    }
}

void echo_pin_interrupt_handler(void)
{
    static uint32_t start_time = 0;
    if (is_rising_edge(ECHO))
    {
        reset_timer(start_time);
        start_time = get_timer_counter();
        state = ACTIVE_LISTENING;
    }
    else if (is_falling_edge(ECHO) && state == ACTIVE_LISTENING)
    {
        uint32_t pulse_duration = get_timer_counter() - start_time;
        state = ACTIVE_DETECTED;

        if (pulse_duration > 0)
        {
            uint32_t adc_value = 889;
            object_distance = pulse_duration * (256108888 - (121907 * adc_value)) / 233;

        }
    }
}

void initialize_sensor(void)
{
    state = INITIAL_START;
    register_timer_interrupt_handler(sensor_timer_interrupt_handler);
    register_pin_interrupt_handler(TRIGGER, CHANGE, echo_pin_interrupt_handler);
    object_detected = false;
    adc->control |= (1 << 20); 
    adc->control |= (4 << 12);
    adc->control |= 0b11;
}

void manage_sensor(void)
{
    if (ping_requested)
    {
        set_pin_high(TRIGGER);
        busy_wait_us(10);
        set_pin_low(TRIGGER);
        ping_requested = false;
    }
    if (object_detected)
    {
        display_distance(object_distance);
    }
    else
    {
        display_no_object_detected();
    }
}
