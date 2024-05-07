/**************************************************************************/ /**
*
* @file user_controls.c
*
* @author (Tyson Veik)
* @author (Colman Scharff)
*
* @brief Code to get inputs from the user.
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
#include <string.h>
#include <stdlib.h>
#include "user_controls.h"
#include "shared_variables.h"
#include "outputs.h"
#include "interrupt_support.h"

int mode;
bool ping_requested;

void initialize_controls(void)
{
    ping_requested = false;
}

void manage_controls(void)
{
    if (cowpi_left_switch_is_in_left_position())
    {
        if (cowpi_right_switch_is_in_left_position())
        {
            mode = NORMAL_OPERATION;
        }
        else
        {
            mode = CONTINUOUS_TONE;
        }
    }
    else
    {
        if (cowpi_right_switch_is_in_left_position())
        {
            mode = SINGLE_PULSE;
            if (cowpi_left_button_is_pressed())
            {
                ping_requested = true;
            }
        }
        else
        {
            mode = THRESHOLD_ADJUSTMENT;
            display_string(1, "Enter Threshold Range");
            char key1;
            char key2;
            char key3;
            char[3] threshold_string;
            key1 = cowpi_get_keypress();
            threshold_string[0] = key1;
            display_string(2, threshold_string);
            key2 = cowpi_get_keypress();
            threshold_string[1] = key2;
            display_string(2, threshold_string);
            key3 = cowpi_get_keypress();
            threshold_string[2] = key3;
            display_string(3, threshold_string);
            threshold_range = atoi(threshold_string);
        }
    }
}

