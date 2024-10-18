/**
* @file rtd_task.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-09-28
*
* @copyright Copyright (c) 2023
*
*/

#include "tasks/rtd_task.h"
#include "main.h"

/**
* @brief Actual RTD task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void rtd_task_fn(void *arg);

TaskHandle_t rtd_task_start(app_data_t *data)
{
   TaskHandle_t handle;
   xTaskCreate(rtd_task_fn, "RTD task", 128, (void *)data, 20, &handle);
   return handle;
}

void rtd_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;
    uint32_t delay;

	for(;;)
	{
		data->tsal = HAL_GPIO_ReadPin(TSAL_HV_SIG_GPIO_Port, TSAL_HV_SIG_Pin);
		data->rtd_button = HAL_GPIO_ReadPin(RTD_Go_GPIO_Port, RTD_Go_Pin);
		data->cascadia_ok = !HAL_GPIO_ReadPin(MTR_Ok_GPIO_Port, MTR_Ok_Pin);

		// state machine (as described in Teams -> Electrical - Firmware -> Files -> RTD_FSM.pptx)
		switch(data->rtd_mode)
		{
			case TSAL_AWAIT:
				if(data->tsal)
				{
					data->rtd_mode = RTD_BUTTON_AWAIT;
				}
				break;
			
			case RTD_BUTTON_AWAIT:
				if(data->rtd_button)
				{
					data->rtd_mode = RTD_CONDITIONS_AWAIT;
				}
				break;

			case RTD_CONDITIONS_AWAIT:
				if(data->cascadia_ok && data->brakelight && data->rtd_button)
				{
					data->rtd_mode = RTD_ENABLED;
					set_buzzer(1);
					osDelay(3000);
					set_buzzer(0);
				}
				break;

			case RTD_ENABLED:
				if(!data->rtd_button)
				{
					data->rtd_mode = RTD_CONDITIONS_AWAIT;
				}
				break;
		}
		
		// from any state
		if(!data->tsal)
		{
			data->rtd_mode = TSAL_AWAIT;
		}

		// Trip Shutdown circuit
		if(!data->hard_fault && data->tsal)
		{
			set_fw(0);
			osDelay(delay);
			set_fw(1);
		}
		osDelay(delay);
	}
}
