/*
 * flow_sensor.h
 *
 *  Created on: Mar 26, 2024
 *      Author: cole
 */

#ifndef __FLOW_SENSOR_H_
#define __FLOW_SENSOR_H_

#include <stdint.h>
#include "stm32f7xx_hal.h"

typedef struct
{
	uint32_t clock_freq;
	TIM_HandleTypeDef *htim;
	TIM_TypeDef *tim;
	HAL_TIM_ActiveChannel high_channel; // TIM_CHANNEL_2
	HAL_TIM_ActiveChannel total_channel; // TIM_CHANNEL_1
	uint32_t high_count;
	uint32_t total_count;
	float duty;
	float freq;
	int ret;
} flow_sensor_t;

void flow_sensor_init(flow_sensor_t *dev, uint32_t clock_freq, TIM_HandleTypeDef *htim, TIM_TypeDef *tim, HAL_TIM_ActiveChannel high_channel, HAL_TIM_ActiveChannel total_channel);
int flow_sensor_read(flow_sensor_t *dev);

#endif /* __FLOW_SENSOR_H_ */
