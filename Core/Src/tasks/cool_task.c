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
    pressure_sensor_t *press = &data->board.cool_pressure;
    flow_sensor_t *flow = &data->board.cool_flow;
    ntc_t *temp1 = &data->board.cool_temp1;
    ntc_t *temp2 = &data->board.cool_temp2;

    uint32_t entry;

	for(;;)
	{
		entry = osKernelGetTickCount();

		// TODO: Calibrate coolant sensors
		stm32f767_adc_switch_channel(press->handle, press->channel);
		press->count = stm32f767_adc_read(press->handle);
		press->percent = pressure_sensor_get_percent(press);
		data->coolant_pressure = (float)press->count * 3.3 / 4096.0;

		data->coolant_flow = flow->freq;

		stm32f767_adc_switch_channel(temp1->hadc, temp1->channel);
		temp1->count = stm32f767_adc_read(temp1->hadc);
		data->coolant_temp_in = ntc_convert(temp1);

		stm32f767_adc_switch_channel(temp2->hadc, temp2->channel);
		temp2->count = stm32f767_adc_read(temp2->hadc);
		data->coolant_temp_out = ntc_convert(temp2);

		osDelayUntil(entry + (1000 / COOL_FREQ));
	}
}
