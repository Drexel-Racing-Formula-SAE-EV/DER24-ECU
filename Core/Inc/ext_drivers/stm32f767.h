/**
* @file stm32f767.h
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-03-13
*
* @copyright Copyright (c) 2023
*
*/

#ifndef __STM32F767_H_
#define __STM32F767_H_

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

typedef struct {
	ADC_HandleTypeDef hadc1;
	ADC_HandleTypeDef hadc2;
	ADC_HandleTypeDef hadc3;

	CAN_HandleTypeDef hcan1;

	I2C_HandleTypeDef hi2c2;

	RTC_HandleTypeDef hrtc;

	SPI_HandleTypeDef hspi6;

	TIM_HandleTypeDef htim5;

	UART_HandleTypeDef huart7;
	UART_HandleTypeDef huart3;

	osMutexId_t can1_mutex;
	osMutexId_t i2c2_mutex;
	osMutexId_t spi4_mutex;
	osMutexId_t spi6_mutex;
	osMutexId_t	uart3_mutex;
	osMessageQueueId_t can1_mq;

	CAN_TxHeaderTypeDef can1_txheader;
} stm32f767_device_t;

void stm32f767_init(stm32f767_device_t *dev);

#endif
