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
#include "ext_drivers/rtc.h"

#define PLAUSIBILITY_THRESH 10
#define BRAKE_LIGHT_THRESH 5
#define BPPC_BSE_THRESH 10
#define BPPC_APPS_H_THRESH 25
#define BPPC_APPS_L_THRESH 5

#define APPS_FREQ 100
#define ACC_FREQ 10
#define ERROR_FREQ 50
#define BSE_FREQ 50
#define BPPC_FREQ 20
#define CLI_FREQ 50
#define DASH_FREQ 5

#define MAXTRQ 100 // maximum nM of toruqe that will be requested from motorcontroller (=100% throttle)

typedef struct {
	int throttle;
	int brake;

	bool rtd_state;

	bool hard_fault;
	bool soft_fault;
	
	bool apps_fault;
	bool bse_fault;
	bool bppc_fault;
	bool acc_fault;
	bool cli_fault;
	bool canbus_fault;
	
	bool tsal;
	bool rtd_button;
	bool cascadia_ok;
	bool cascadia_error;
	bool imd_fail;
	bool bms_fail;
	bool bspd_fail;

	bool brakelight;

	board_t board;
	datetime_t datetime;

	TaskHandle_t dev_task;
	TaskHandle_t cli_task;
	TaskHandle_t rtd_task;
	TaskHandle_t error_task;
	TaskHandle_t apps_task;
	TaskHandle_t bse_task;
	TaskHandle_t bppc_task;
	TaskHandle_t canbus_task;
	TaskHandle_t acc_task;
	TaskHandle_t dashboard_task;
} app_data_t;

void app_create();
void cli_putline(char *line);
HAL_StatusTypeDef read_time();
HAL_StatusTypeDef write_time();
void set_fw(bool state);
void set_buzzer(bool state);
void set_cascadia_enable(bool state);
void set_brakelight(bool state);

#endif
