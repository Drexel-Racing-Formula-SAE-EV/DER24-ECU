/**
* @file pressure_sensor.h
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-04-24
*
* @copyright Copyright (c) 2023
*
*/

#ifndef __PRESSURE_SENSOR_H_
#define __PRESSURE_SENSOR_H_

#include <stdint.h>

#include "stm32f7xx_hal.h"

typedef struct {
	uint16_t min;
	uint16_t max;

	uint16_t count;
	float percent;

	void *handle;
	uint32_t channelNum;

	uint16_t(*read_count)(void *hadc);
} pressure_sensor_t;

void pressure_sensor_init(pressure_sensor_t *sensor, uint16_t min, uint16_t max, void *handle, uint8_t channelNum, uint16_t(*read_count)(void *hadc));

float pressure_sensor_get_percent(pressure_sensor_t *root);

uint8_t pressure_sensor_check_implausibility(float L, float R, int thresh, int count);

uint8_t switchChannelADC(pressure_sensor_t *root);

#endif
