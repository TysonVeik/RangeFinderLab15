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

bool object_detected;
int object_distance;
int object_approach_rate;
int ADC_register_value = 889;
int state;
adc_t* adc_structure = 0x4004c000;


void handle_sensor_timer(void){

}

void detect_edge_pulse(void){

}

void initialize_sensor(void) {
    state = INITIAL_START;
    register_timer_ISR(1,32768, handle_sensor_timer());
    register_pin_ISR(1L << ECHO, detect_edge_pulse());
    object_detected = false;
    adc_structure->control = adc_structure->control | 1 << 20;
    //adc_structure->control = adc_structure->control || 0100 << 11;
    adc_structure->control = adc_structure->control | 0011;

}

void manage_sensor(void) {

}