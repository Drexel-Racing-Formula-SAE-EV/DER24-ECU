/**
* @file app.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-03-19
*
* @copyright Copyright (c) 2023
*
*/
#include <string.h>

#include "app.h"
#include "tasks/bse_task.h"
#include "tasks/rtd_task.h"
#include "tasks/error_task.h"
#include "tasks/bse_task.h"
#include "tasks/bppc_task.h"
#include "tasks/apps_task.h"
#include "tasks/canbus_task.h"
#include "tasks/cli_task.h"
#include "tasks/acc_task.h"
#include "tasks/dashboard_task.h"
#include "tasks/cool_task.h"

app_data_t app = {0};

void app_create()
{
	app.throttle = 0;
	app.brake = 0;

	app.rtd_state = false;

	app.hard_fault = false;
	app.soft_fault = false;

	app.coolant_fault = false;
	app.apps_fault = false;
	app.bse_fault = false;
	app.bppc_fault = false;
	app.acc_fault = false;
	app.cli_fault = false;
	app.canbus_fault = false;
	app.dashboard_fault = false;
	app.mq_fault = false;

	app.fw_state = false;
	app.tsal = false;
	app.rtd_button = false;
	app.cascadia_ok = true;
	app.cascadia_error = false;
	app.cascadia_en = false;
	app.imd_fail = false;
	app.bms_fail = false;
	app.bspd_fail = false;

	app.brakelight = false;

	app.coolant_pressure = 0.0;
	app.coolant_flow = 0.0;
	app.coolant_temp_in = 0.0;
	app.coolant_temp_out = 0.0;

	app.throttle = 0;
	app.brake = 0;

	board_init(&app.board);
    set_cascadia_enable(1);

	HAL_UART_Receive_IT(app.board.cli.huart, &app.board.cli.c, 1);
	HAL_CAN_ActivateNotification(app.board.canbus.hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	assert(app.cli_task = cli_task_start(&app));
	assert(app.rtd_task = rtd_task_start(&app));
	assert(app.error_task = error_task_start(&app));
	assert(app.canbus_task = canbus_task_start(&app));
	assert(app.bse_task = bse_task_start(&app));
	assert(app.apps_task = apps_task_start(&app));
	assert(app.bppc_task = bppc_task_start(&app));
	assert(app.acc_task = acc_task_start(&app));
	assert(app.dashboard_task = dashboard_task_start(&app));
	assert(app.cool_task = cool_task_start(&app));
}

HAL_StatusTypeDef read_time(){
	RTC_TimeTypeDef rTime;
	RTC_DateTypeDef rDate;
	HAL_StatusTypeDef ret = 0;

	ret |= HAL_RTC_GetTime(&app.board.stm32f767.hrtc, &rTime, RTC_FORMAT_BIN);
	ret |= HAL_RTC_GetDate(&app.board.stm32f767.hrtc, &rDate, RTC_FORMAT_BIN);

	app.datetime.second = rTime.Seconds;
	app.datetime.minute = rTime.Minutes;
	app.datetime.hour = rTime.Hours;
	app.datetime.day = rDate.Date;
	app.datetime.month = rDate.Month;
	app.datetime.year = rDate.Year;

	return ret;
}

HAL_StatusTypeDef write_time(){
	RTC_TimeTypeDef rTime;
	RTC_DateTypeDef rDate;
	HAL_StatusTypeDef ret = 0;

	rTime.Seconds = HEX2DEC(app.datetime.second);
	rTime.Minutes = HEX2DEC(app.datetime.minute);
	rTime.Hours = HEX2DEC(app.datetime.hour);
	rDate.Date = HEX2DEC(app.datetime.day);
	rDate.Month = HEX2DEC(app.datetime.month);
	rDate.Year = HEX2DEC(app.datetime.year);
	rDate.WeekDay = RTC_WEEKDAY_MONDAY;

	ret |= HAL_RTC_SetTime(&app.board.stm32f767.hrtc, &rTime, RTC_FORMAT_BCD);
	ret |= HAL_RTC_SetDate(&app.board.stm32f767.hrtc, &rDate, RTC_FORMAT_BCD);
	HAL_PWR_EnableBkUpAccess();
	HAL_RTCEx_BKUPWrite(&app.board.stm32f767.hrtc, RTC_BKP_DR1, 0x32F2);
    HAL_PWR_DisableBkUpAccess();

	return ret;
}

void set_fw(bool state)
{
	app.fw_state = state;
	HAL_GPIO_WritePin(Firmware_Ok_GPIO_Port, Firmware_Ok_Pin, state);
}

void set_buzzer(bool state)
{
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, state);
}

void set_cascadia_enable(bool state)
{
	app.cascadia_en = state;
	HAL_GPIO_WritePin(MTR_EN_GPIO_Port, MTR_EN_Pin, state);
}

void set_brakelight(bool state)
{
	app.brakelight = state;
	HAL_GPIO_WritePin(Brake_Light_GPIO_Port, Brake_Light_Pin, state);
}

void set_ssa(int duty)
{
	if(duty > 100) duty = 100;
	else if(duty < 0) duty = 0;
	pwm_set_percent(&app.board.ssa, duty);
}
