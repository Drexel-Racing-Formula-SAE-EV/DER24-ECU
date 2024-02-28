/**
* @file pressure_sensor.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-04-24
*
* @copyright Copyright (c) 2023
*
*/

#include <math.h>

#include "ext_drivers/pressure_sensor.h"

#include "ext_drivers/map.h"

void pressure_sensor_init(pressure_sensor_t *sensor, uint16_t min, uint16_t max, ADC_HandleTypeDef *handle, uint8_t channel)
{
	sensor->min = min;
	sensor->max = max;
	sensor->handle = handle;
	sensor->channel = channel;
}

float pressure_sensor_get_percent(pressure_sensor_t *root)
{
	float percent = (float)map(root->count, root->min, root->max, 0, 100);
	if(percent > 100.0)
	{
		return 100.0;
	}
	else if(percent < 0.0)
	{
		return 0.0;
	}
	else
	{
		return percent;
	}
}

uint8_t pressure_sensor_check_implausibility(float L, float R, int thresh, int count)
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
