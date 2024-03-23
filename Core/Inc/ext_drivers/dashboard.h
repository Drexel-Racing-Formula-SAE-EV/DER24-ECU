/*
 * dashboard.h
 *
 *  Created on: Mar 23, 2024
 *      Author: cole
 */

#ifndef __DASHBOARD_H_
#define __DASHBOARD_H_

#include "stm32f7xx_hal.h"

#define DASH_LINESZ 64

typedef struct
{
	UART_HandleTypeDef *huart;
	HAL_StatusTypeDef ret;
	char line[DASH_LINESZ];
} dashboard_t;

int dashboard_init(dashboard_t *dev, UART_HandleTypeDef *huart);
HAL_StatusTypeDef dashboard_write(dashboard_t *dev, char *str);

#endif /* INC_EXT_DRIVERS_DASHBOARD_H_ */
