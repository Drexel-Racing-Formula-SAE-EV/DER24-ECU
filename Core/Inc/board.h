/**
 * @file board.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BOARD_H_
#define __BOARD_H_

#include <ext_drivers/pressure_sensor.h>
#include <stdbool.h>

#include "ext_drivers/stm32f767.h"
#include "ext_drivers/poten.h"
#include "ext_drivers/canbus.h"
#include "ext_drivers/cli.h"

#define CANBUS_ISR 	0x2	// Notification bit value for ISR messages
#define CANBUS_APPS	0x1 // Notification bit value for APPS messages 

typedef struct {
	// Physical devices on the board
	stm32f767_device_t stm32f767;
	pressure_sensor_t bse1;
	pressure_sensor_t bse2;
	poten_t apps1;
	poten_t apps2;
 	canbus_device_t canbus_device;
	cli_device_t cli;
} board_t;

void board_init(board_t *dev);

uint16_t ADC_read_count(void *hadc);

void setBrakeLight(bool state);
void setMotorEn(bool state);

#endif
