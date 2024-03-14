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

app_data_t app = {0};

void app_create()
{
	app.rtdFlag = false;

	app.hardSystemFault = false;
	app.softSystemFault = false;

	app.appsFaultFlag = false;
	app.bseFaultFlag = false;
	app.bppcFaultFlag = false;
	app.accFaultFlag = false;
	app.cliFaultFlag = false;

	app.brakeLightState = false;

	app.throttlePercent = 0;
	app.brakePercent = 0;

	board_init(&app.board);

	/*
	app.datetime.second = 0;
	app.datetime.minute = 0;
	app.datetime.hour = 0;
	app.datetime.day = 1;
	app.datetime.month = 1;
	app.datetime.year = 24;
	write_time();
	*/

	HAL_UART_Receive_IT(app.board.cli.huart, &app.board.cli.c, 1);

	assert(app.cli_task = cli_task_start(&app));
	assert(app.rtd_task = rtd_task_start(&app));
	assert(app.error_task = error_task_start(&app));
	assert(app.canbus_task = canbus_task_start(&app));
	assert(app.bse_task = bse_task_start(&app));
	assert(app.apps_task = apps_task_start(&app));
	assert(app.bppc_task = bppc_task_start(&app));
	assert(app.acc_task = acc_task_start(&app));
}

void cli_putline(char *line)
{
	static char nl[] = "\r\n";
	
	HAL_StatusTypeDef ret = 0;

	if(xPortIsInsideInterrupt())
	{
		ret |= HAL_UART_Transmit_IT(app.board.cli.huart, (uint8_t *)line, strlen(line));
		ret |= HAL_UART_Transmit_IT(app.board.cli.huart, (uint8_t*)nl, strlen(nl));
	}
	else
	{
		//while(osMutexAcquire(app.board.stm32f767.uart3_mutex, 0) != osOK) osDelay(5);
		ret |= HAL_UART_Transmit(app.board.cli.huart, (uint8_t *)line, strlen(line), HAL_MAX_DELAY);
		ret |= HAL_UART_Transmit(app.board.cli.huart, (uint8_t *)nl, strlen(nl), HAL_MAX_DELAY);
		//osMutexRelease(app.board.stm32f767.uart3_mutex);
	}
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
	HAL_RTCEx_BKUPWrite(&app.board.stm32f767.hrtc, RTC_BKP_DR0, 0x32);

	return ret;
}
