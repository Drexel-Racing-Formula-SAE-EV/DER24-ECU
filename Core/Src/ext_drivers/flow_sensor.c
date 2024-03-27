/*
 * flow_sensor.c
 *
 *  Created on: Mar 26, 2024
 *      Author: cole
 */

#include "ext_drivers/flow_sensor.h"

void flow_sensor_init(flow_sensor_t *dev, uint32_t clock_freq, TIM_HandleTypeDef *htim, TIM_TypeDef *tim, HAL_TIM_ActiveChannel high_channel, HAL_TIM_ActiveChannel total_channel)
{
	dev->clock_freq = clock_freq;
	dev->htim = htim;
	dev->tim = tim;
	dev->high_channel = high_channel;
	dev->total_channel = total_channel;
	dev->duty = 0;
	dev->freq = 0;
	dev->high_count = 0;
	dev->total_count = 0;
	dev->ret = 0;
	HAL_TIM_Base_Start(htim);
	HAL_TIM_IC_Start_IT(htim, total_channel);
	HAL_TIM_IC_Start(htim, high_channel);
}

int flow_sensor_read(flow_sensor_t *dev)
{
	dev->total_count = HAL_TIM_ReadCapturedValue(dev->htim, dev->total_channel);
	if (dev->total_count != 0)
	{
		// calculate the Duty Cycle
		dev->duty = (float)(HAL_TIM_ReadCapturedValue(dev->htim, dev->high_channel) * 100) / (float)dev->total_count;
		dev->freq = (float)dev->clock_freq / (float)dev->total_count;
	}
	return 0;
}
