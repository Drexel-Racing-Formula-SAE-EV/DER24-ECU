/**
 * @file poten.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __POTEN_H_
#define __POTEN_H_

#include <stdint.h>
#include "stm32f7xx_hal.h"

#define HISTSZ 50

typedef struct {
	uint16_t min;
	uint16_t max;

	uint16_t count;
	float percent;
	float hist[HISTSZ];

	ADC_HandleTypeDef *handle;
} poten_t;

void poten_init(poten_t *poten, uint16_t min, uint16_t max, ADC_HandleTypeDef *handle);

float poten_get_percent(poten_t *root);

uint16_t poten_percent_to_hex(float percent);

uint8_t poten_check_plausibility(float L, float R, int thresh, int count);

#endif
