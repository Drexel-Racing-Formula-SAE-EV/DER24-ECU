/**
* @file bse_task.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-8-15
*
* @copyright Copyright (c) 2023
*
*/

#include "tasks/bse_task.h"
#include "main.h"

/**
* @brief Actual BSE task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void bse_task_fn(void *arg);

TaskHandle_t bse_task_start(app_data_t *data)
{
   TaskHandle_t handle;
   xTaskCreate(bse_task_fn, "BSE task", 128, (void *)data, BSE_PRIO, &handle);
   return handle;
}

void bse_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;
    pressure_sensor_t *bse1 = &data->board.bse1;
    pressure_sensor_t *bse2 = &data->board.bse2;
    float brake_raw;
    uint32_t entry;

	for(;;)
	{
		entry = osKernelGetTickCount();

		stm32f767_adc_switch_channel(bse1->handle, bse1->channel);
		bse1->count = stm32f767_adc_read(bse1->handle);
		stm32f767_adc_switch_channel(bse2->handle, bse2->channel);
		bse2->count = stm32f767_adc_read(bse2->handle);
		bse1->percent = pressure_sensor_get_percent(bse1);
		bse2->percent = pressure_sensor_get_percent(bse2);

		// T.4.3.3 (2022)
		if(!pressure_sensor_check_implausibility(bse1->percent, bse2->percent, PLAUSIBILITY_THRESH, BSE_FREQ / 10))
		{
			data->bse_fault = true;
		}

		brake_raw = (bse1->percent + bse2->percent) / 2;
		data->brake = (int)brake_raw;
		set_brakelight((data->brake >= BRAKE_LIGHT_THRESH));

		osDelayUntil(entry + (1000 / BSE_FREQ));
	}
}
