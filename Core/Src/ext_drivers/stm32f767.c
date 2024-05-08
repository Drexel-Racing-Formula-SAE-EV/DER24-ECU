/**
* @file stm32f767.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-03-13
*
* @copyright Copyright (c) 2023
*
*/

#include "main.h"
#include "ext_drivers/stm32f767.h"
#include "ext_drivers/canbus.h"

const osMutexAttr_t can1_mutex_attr = {
	.name = "CAN Bus Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t i2c2_mutex_attr = {
	.name = "MPU6050 Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t spi6_mutex_attr = {
	.name = "SD Card Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t uart3_mutex_attr = {
	.name = "CLI Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t uart7_mutex_attr = {
	.name = "Dashboard Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

void stm32f767_init(stm32f767_device_t *dev)
{
	extern ADC_HandleTypeDef hadc1;
	extern ADC_HandleTypeDef hadc2;
	extern ADC_HandleTypeDef hadc3;

	extern CAN_HandleTypeDef hcan1;

	extern I2C_HandleTypeDef hi2c2;

	extern RTC_HandleTypeDef hrtc;

	extern SPI_HandleTypeDef hspi6;

	extern TIM_HandleTypeDef htim3;
	extern TIM_HandleTypeDef htim4;
	extern TIM_HandleTypeDef htim5;

	extern UART_HandleTypeDef huart7;
	extern UART_HandleTypeDef huart3;

	dev->hadc1 = hadc1;
	dev->hadc2 = hadc2;
	dev->hadc3 = hadc3;
	dev->hcan1 = hcan1;
	dev->hi2c2 = hi2c2;
	dev->hrtc = hrtc;
	dev->hspi6 = hspi6;
	dev->htim3 = htim3;
	dev->htim4 = htim4;
	dev->htim5 = htim5;
	dev->huart3 = huart3;
	dev->huart7 = huart7;

	dev->can1_mutex = osMutexNew(&can1_mutex_attr);
	assert(dev->can1_mutex);

	dev->i2c2_mutex = osMutexNew(&i2c2_mutex_attr);
	assert(dev->i2c2_mutex);

	dev->spi6_mutex = osMutexNew(&spi6_mutex_attr);
	assert(dev->spi6_mutex);

	dev->uart3_mutex = osMutexNew(&uart3_mutex_attr);
	assert(dev->uart3_mutex);

	dev->uart7_mutex = osMutexNew(&uart7_mutex_attr);
	assert(dev->uart7_mutex);
}

uint16_t stm32f767_adc_read(ADC_HandleTypeDef *hadc)
{
	uint16_t count;

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
	count = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);
	return count;
}

HAL_StatusTypeDef stm32f767_adc_switch_channel(ADC_HandleTypeDef *hadc, uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	return HAL_ADC_ConfigChannel(hadc, &sConfig);
}
