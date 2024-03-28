/*
 * pwm.c
 *
 *  Created on: Mar 28, 2024
 *      Author: cole
 */

#include "ext_drivers/pwm.h"

int pwm_device_init(pwm_device_t *dev, TIM_TypeDef *timer, TIM_HandleTypeDef *htim, uint64_t max_timer_val, volatile uint32_t *CCR, int channel)
{
	dev->timer = timer;
	dev->htim = htim;
	dev->channel = channel;
	dev->max_timer_val = max_timer_val;
	dev->CCR = CCR;

	HAL_TIM_PWM_Start(htim, (channel - 1) * 4);
	pwm_set_percent(dev, 0.0);

	return 0;
}

int pwm_set_percent(pwm_device_t *dev, float percent)
{
	if(percent > 100.0) percent = 100;
	else if(percent < 0) percent = 0;
	dev->duty_cycle = percent;
	*(dev->CCR) = (volatile uint32_t)dev->max_timer_val * (volatile uint32_t)percent / 100;
	return 0;
}
