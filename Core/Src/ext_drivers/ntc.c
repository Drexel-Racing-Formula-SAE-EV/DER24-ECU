/*
 * ntc.c
 *
 *  Created on: Mar 27, 2024
 *      Author: cole
 */

#include <math.h>
#include "ext_drivers/ntc.h"

void ntc_init(ntc_t *dev, ADC_HandleTypeDef *hadc, uint32_t channel)
{
	dev->hadc = hadc;
	dev->channel = channel;
	dev->count = 0;
	dev->temp = 0.0;
}
