/**
* @file dashboard_task.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-09-28
*
* @copyright Copyright (c) 2023
*
*/
#include <stdio.h>

#include "tasks/dashboard_task.h"
#include "main.h"

#define NEWLINE "\r\n"

/**
* @brief Actual Dashboard task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void dashboard_task_fn(void *arg);

TaskHandle_t dashboard_task_start(app_data_t *data)
{
   TaskHandle_t handle;
   xTaskCreate(dashboard_task_fn, "Dashboard task", 128, (void *)data, DASH_PRIO, &handle);
   return handle;
}

void dashboard_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;
    dashboard_t *dash = &data->board.dashboard;
    uint32_t entry;
    HAL_StatusTypeDef ret;

	for(;;)
	{
		entry = osKernelGetTickCount();
		
		ret = HAL_OK;
		snprintf(dash->line, DASH_LINESZ, "throttle %d" NEWLINE, data->throttle);
		ret |= dashboard_write(dash, dash->line);
		snprintf(dash->line, DASH_LINESZ, "brake %d" NEWLINE, data->brake);
		ret |= dashboard_write(dash, dash->line);
		snprintf(dash->line, DASH_LINESZ, "rtd %d" NEWLINE, data->rtd_mode);
		ret |= dashboard_write(dash, dash->line);
		snprintf(dash->line, DASH_LINESZ, "bms_fail %d" NEWLINE, data->bms_fail);
		ret |= dashboard_write(dash, dash->line);
		snprintf(dash->line, DASH_LINESZ, "imd_fail %d" NEWLINE, data->imd_fail);
		ret |= dashboard_write(dash, dash->line);
		snprintf(dash->line, DASH_LINESZ, "ecu_fault %d" NEWLINE, data->hard_fault);
		ret |= dashboard_write(dash, dash->line);
		// TODO: add rest of metrics after testing
		data->dashboard_fault = (ret != HAL_OK);

		osDelayUntil(entry + (1000 / DASH_FREQ));
	}
}
