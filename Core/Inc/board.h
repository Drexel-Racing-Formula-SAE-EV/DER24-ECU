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

#pragma once

#include <stdbool.h>

#include "ext_drivers/pressTrans.h"
#include "ext_drivers/stm32f767.h"
#include "ext_drivers/poten.h"
#include "ext_drivers/canbus.h"
#include "ext_drivers/cli.h"

#define CANBUS_ISR 	0x2	// Notification bit value for ISR messages
#define CANBUS_APPS	0x1 // Notification bit value for APPS messages 

struct board {
	// Physical devices on the board
	struct stm32f767_device stm32f767;
	struct pressTrans bse1;
	struct pressTrans bse2;
	struct poten apps1;
	struct poten apps2;
 	struct canbus_device canbus_device;
	struct cli_device cli;
};

void board_init(struct board* dev);

uint16_t ADC_read_count(void *hadc);

void setBrakeLight(bool state);
void setMotorEn(bool state);
