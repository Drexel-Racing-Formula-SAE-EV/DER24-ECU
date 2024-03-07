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
	ret = 0;
}

