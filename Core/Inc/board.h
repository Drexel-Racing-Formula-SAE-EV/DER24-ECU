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

#include <stdbool.h>

#include "ext_drivers/stm32f767.h"
#include "ext_drivers/poten.h"
#include <ext_drivers/pressure_sensor.h>
#include "ext_drivers/canbus.h"
#include "ext_drivers/cli.h"

// 0.12V - 1.8V * (3/2) resistor divider => 0.18V - 2.7V
#define BSE1_MIN 155//Brake emulator min: 155 //Theoretical value (ADC max): 339
#define BSE1_MAX 2238 //Brake emulator max: 2240 //Theoretical value (ADC max): 1900
// 0.14V -1.8V *(3/2) => 0.21V - 2.7V
#define BSE2_MIN 175 //Brake emulator min: 175 //Theoretical value (ADC max): 810
#define BSE2_MAX 2250 //Brake emulator max: 2250 //Theoretical value (ADC max): 2158

#define BSE1_ADC_CH 13
#define BSE2_ADC_CH 9

#define APPS1_MIN 339
#define APPS1_MAX 1900
#define APPS2_MIN 810
#define APPS2_MAX 2158

#define CANBUS_ISR 	0x2	// Notification bit value for ISR messages
#define CANBUS_APPS	0x1 // Notification bit value for APPS messages 

#define MTR_CANBUS_RXID 0x201

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
