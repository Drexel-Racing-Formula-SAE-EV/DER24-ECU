/*
 * ntc.h
 *
 *  Created on: Mar 27, 2024
 *      Author: cole
 */

#ifndef __NTC_H_
#define __NTC_H_

#include <stdint.h>
#include "stm32f7xx_hal.h"

typedef struct
{
	ADC_HandleTypeDef *hadc;
	uint32_t channel;
	uint16_t count;
	float temp;
} ntc_t;

void ntc_init(ntc_t *dev, ADC_HandleTypeDef *hadc, uint32_t channel);
float ntc_convert(ntc_t *dev);

#endif /* __NTC_H_ */
