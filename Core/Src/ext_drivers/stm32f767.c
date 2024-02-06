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

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

extern CAN_HandleTypeDef hcan1;

extern I2C_HandleTypeDef hi2c2;

extern RTC_HandleTypeDef hrtc;

extern SPI_HandleTypeDef hspi6;

extern TIM_HandleTypeDef htim5;

extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart3;

const osMutexAttr_t can1_mutex_attr = {
	.name = "CAN Bus 1 Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t i2c2_mutex_attr = {
	.name = "I2C 2 Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t spi4_mutex_attr = {
	.name = "SPI 4 Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t spi6_mutex_attr = {
	.name = "SPI 6 Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

const osMutexAttr_t uart3_mutex_attr = {
	.name = "UART 3 Mutex",
	.attr_bits = osMutexPrioInherit | osMutexRecursive,
	.cb_mem = NULL,
	.cb_size = 0UL,
};

void stm32f767_init(stm32f767_device_t *dev){
	MX_GPIO_Init();
	MX_CAN1_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	MX_I2C2_Init();
	MX_SPI6_Init();
	MX_TIM5_Init();
	MX_USART3_UART_Init();
	MX_UART7_Init();
	MX_RTC_Init();

	dev->hadc1 = hadc1;
	dev->hadc2 = hadc2;
	dev->hadc3 = hadc3;

	dev->hcan1 = hcan1;
	dev->can1_mq = osMessageQueueNew(128, sizeof(canbus_packet), NULL);

	dev->hi2c2 = hi2c2;

	dev->hrtc = hrtc;

	dev->hspi6 = hspi6;

	dev->huart3 = huart3;
	dev->huart3 = huart7;

	dev->can1_mutex = osMutexNew(&can1_mutex_attr);
	assert(dev->can1_mutex);

	dev->i2c2_mutex = osMutexNew(&i2c2_mutex_attr);
	assert(dev->i2c2_mutex);

	dev->spi4_mutex = osMutexNew(&spi4_mutex_attr);
	assert(dev->spi4_mutex);

	dev->spi6_mutex = osMutexNew(&spi6_mutex_attr);
	assert(dev->spi6_mutex);

	dev->uart3_mutex = osMutexNew(&uart3_mutex_attr);
	assert(dev->uart3_mutex);
}
