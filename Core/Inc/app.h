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

#ifndef __APP_H_
#define __APP_H_

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

#define APPS_FREQ 100
#define ACC_FREQ 10
#define ERROR_FREQ 50
#define BSE_FREQ 50
#define BPPC_FREQ 20

#define MAXTRQ 100 // maximum nM of toruqe that will be requested from motorcontroller (=100% throttle)

typedef struct {
	float throttlePercent;
	float brakePercent;

	bool rtdFlag;

	bool hardSystemFault;
	bool softSystemFault;
	
	bool appsFaultFlag;
	bool bseFaultFlag;
	bool bppcFaultFlag;
	bool accFaultFlag;
	
	bool brakeLightState;

	board_t board;

	TaskHandle_t dev_task;
	TaskHandle_t cli_task;
	TaskHandle_t rtd_task;
	TaskHandle_t error_task;
	TaskHandle_t apps_task;
	TaskHandle_t bse_task;
	TaskHandle_t bppc_task;
	TaskHandle_t canbus_task;
	TaskHandle_t acc_task;
} app_data_t;

void app_create();

void cli_putline(char *line);

#endif
