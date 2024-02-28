/**
 * @file poten.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <math.h>

#include "ext_drivers/poten.h"
#include "ext_drivers/map.h"

void poten_init(poten_t *poten, uint16_t min, uint16_t max, ADC_HandleTypeDef *handle)
{
	poten->min = min;
	poten->max = max;
	poten->handle = handle;
}

float poten_get_percent(poten_t *root) {
	float percent = (float)map(root->count, root->min, root->max, 100, 0);
	if(percent > 100.0) return 100.0;
	else if(percent < 0.0) return 0.0;
	else return percent;
}

uint16_t poten_percent_to_hex(float percent)
{
    if (percent > 100) percent = 100.0;
    if (percent < 0) percent = 0.0;
    return (uint16_t)percent * 0x5555;
}

uint8_t poten_check_plausibility(float L, float R, int thresh, int count)
{
    static unsigned int counts = 0;

	// Check if APPS1 and APPS2 are more than 10% different
	if(fabs(L - R) > thresh)
	{
		counts++;

		// If there are consecutive errors for more than 100ms, error
		return counts <= count;
	}
	else
	{
		// If potentiometers are within spec, reset count
		counts = 0;
		return 1;
	}
}
