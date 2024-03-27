/**
* @file cool_task.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2024-3-26
*
* @copyright Copyright (c) 2023
*
*/

#include "tasks/cool_task.h"
#include "main.h"

/**
* @brief Actual COOL task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void cool_task_fn(void *arg);

TaskHandle_t cool_task_start(app_data_t *data)
{
   TaskHandle_t handle;
   xTaskCreate(cool_task_fn, "COOL task", 128, (void *)data, 3, &handle);
   return handle;
}

void cool_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;
    pressure_sensor_t *cool_press = &data->board.cool_pressure;

    uint32_t entry;

	for(;;)
	{
		entry = osKernelGetTickCount();

		stm32f767_adc_switch_channel(cool_press->handle, cool_press->channel);
		cool_press->count = stm32f767_adc_read(cool_press->handle);
		cool_press->percent = pressure_sensor_get_percent(cool_press);
		//data->coolant_pressure = cool_press->percent;
		data->coolant_pressure = (float)cool_press->count * 3.3 / 4096.0;


		osDelayUntil(entry + (1000 / COOL_FREQ));
	}
}
