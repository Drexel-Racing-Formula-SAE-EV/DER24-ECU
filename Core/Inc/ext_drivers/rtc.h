/**
 * @file rtc.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2024-01-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __RTC_H_
#define __RTC_H_

#include <stdint.h>
#include <stm32f7xx_hal.h>
#include <stm32f7xx_hal_rtc.h>

#define DEC2HEX(x) (10*(x / 16) + (x - 10*(x / 16)))
#define HEX2DEC(x) (16*(x / 10) + (x % 10))

typedef struct {
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
} datetime_t;

#endif
