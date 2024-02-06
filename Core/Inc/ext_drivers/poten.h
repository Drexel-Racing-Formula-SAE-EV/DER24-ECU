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

#pragma once

#include <stdint.h>

struct poten {
	uint16_t min;
	uint16_t max;

	uint16_t count;
	float percent;

	void *handle;

	uint16_t(*read_count)(void *hadc);
};

void poten_init(struct poten *poten, uint16_t min, uint16_t max, void *handle, uint16_t(*read_count)(void *hadc));

float potenGetPercent(struct poten *root);

uint16_t percentToThrottleHex(float percent);

uint8_t potenCheckImplausability(float L, float R, int thresh, int count);
