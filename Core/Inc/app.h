/**
* @file app.h
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-03-13
*
* @copyright Copyright (c) 2023
*
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "board.h"

#define PLAUSIBILITY_THRESH 10
#define BRAKE_LIGHT_THRESH 5
#define BPPC_BSE_THRESH 10
#define BPPC_APPS_H_THRESH 25
#define BPPC_APPS_L_THRESH 5
#define RTD_BSE_THRESH 10

#define APPS_FREQ 200
#define ERROR_FREQ 50
#define BSE_FREQ 50
#define BPPC_FREQ 20

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

#define BAMOCAR_CANBUS_RXID 0x201
#define BAMOCAR_CANBUS_TORQUE_CMD 0x90

struct app_data {
	float throttlePercent;
	float brakePercent;

	bool rtdFlag;

	bool hardSystemFault;
	bool softSystemFault;
	
	bool appsFaultFlag;
	bool bseFaultFlag;
	bool bppcFaultFlag;
	
	bool brakeLightState;

	struct board board;

	TaskHandle_t dev_task;
	TaskHandle_t cli_task;
	TaskHandle_t rtd_task;
	TaskHandle_t error_task;
	TaskHandle_t apps_task;
	TaskHandle_t bse_task;
	TaskHandle_t bppc_task;
	TaskHandle_t canbus_task;
};

void app_create();

void cli_putline(char *line);
