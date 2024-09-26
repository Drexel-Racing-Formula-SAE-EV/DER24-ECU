/*
 * fans.h
 *
 *  Created on: Mar 28, 2024
 *      Author: Cole Bardin
 */

#ifndef __PWM_H_
#define __PWM_H_

#include <stdint.h>
#include "stm32f7xx_hal.h"

typedef struct
{
	TIM_TypeDef *timer;
	TIM_HandleTypeDef *htim;
	int channel;
	uint64_t max_timer_val;
	volatile uint32_t *CCR;
	float duty_cycle;
} pwm_t;

int pwm_device_init(pwm_t *dev, TIM_TypeDef *timer, TIM_HandleTypeDef *htim, uint64_t max_timer_val, volatile uint32_t *CCR, int channel);
int pwm_set_percent(pwm_t *dev, float percent);

#endif /* __PWM_H_ */
