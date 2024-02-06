/**
 * @file board.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "board.h"
#include "app.h"

extern struct app_data app;

void board_init(struct board* dev) {
	stm32f767_init(&dev->stm32f767);

	poten_init(&dev->apps1, APPS1_MIN, APPS1_MAX, &dev->stm32f767.hadc1, ADC_read_count);
	poten_init(&dev->apps2, APPS2_MIN, APPS2_MAX, &dev->stm32f767.hadc2, ADC_read_count);
	pressTrans_init(&dev->bse1, BSE1_MIN, BSE1_MAX, &dev->stm32f767.hadc3, BSE1_ADC_CH, ADC_read_count);
	pressTrans_init(&dev->bse2, BSE2_MIN, BSE2_MAX, &dev->stm32f767.hadc3, BSE2_ADC_CH, ADC_read_count);

	canbus_device_init(&dev->canbus_device, &dev->stm32f767.hcan1, &dev->stm32f767.can1_txheader);

	cli_device_init(&dev->cli, &dev->stm32f767.huart7);
}

uint16_t ADC_read_count(void *hadc) {
	uint16_t count;

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
	count = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);

	return count;
}

void setBrakeLight(bool state){
	HAL_GPIO_WritePin(Brake_Light_GPIO_Port, Brake_Light_Pin, state);
}

void setMotorEn(bool state){
	HAL_GPIO_WritePin(MTR_EN_GPIO_Port, MTR_EN_Pin, state);
}
