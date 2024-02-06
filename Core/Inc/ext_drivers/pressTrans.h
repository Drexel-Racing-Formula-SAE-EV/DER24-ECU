/**
* @file pressTrans.h
* @author Cole Bardin (cab572@drexel.edu) & Benedict Hofmockel
* @brief
* @version 0.1
* @date 2023-04-24
*
* @copyright Copyright (c) 2023
*
*/

#pragma once

#include <stdint.h>
#include <stm32f7xx_hal.h>

struct pressTrans {
	uint16_t min;
	uint16_t max;

	uint16_t count;
	float percent;

	void *handle; //ADC handle goes here
	uint32_t channelNum; //ADC channel number

	uint16_t(*read_count)(void *hadc); //this is a function pointer in C, kinda weird ain't it.
};

void pressTrans_init(struct pressTrans *pressTrans, uint16_t min, uint16_t max, void *handle, uint8_t channelNum, uint16_t(*read_count)(void *hadc));

float presstransGetPercent(struct pressTrans *root);

uint8_t presstransCheckImplausability(float L, float R, int thresh, int count);

uint8_t switchChannelADC (struct pressTrans *root);
