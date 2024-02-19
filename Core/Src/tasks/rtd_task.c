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

    canbus_packet_t TxPacket;
    osMessageQueueId_t canbus_mq = data->board.stm32f767.can1_mq;
	
	bool tsalHV = false;
	bool brakesEngaged = false;
	bool rtdButton = false;
	bool motorOk = false;

	data->rtdFlag = false;

    for(uint8_t i = 0; i < 8; i++)
    {
        TxPacket.data[i] = 0x00;
    }
    TxPacket.id = MTR_CMD_ID;

	for(;;)
	{
		if(!data->rtdFlag)
		{
			// EV.10.4.3
			// TODO: check polarity of sensors & decide if MTR inputs here
			tsalHV = HAL_GPIO_ReadPin(TSAL_HV_SIG_GPIO_Port, TSAL_HV_SIG_Pin);
			brakesEngaged = (data->brakePercent >= RTD_BSE_THRESH);
			rtdButton = HAL_GPIO_ReadPin(RTD_Go_GPIO_Port, RTD_Go_Pin);
			motorOk = !HAL_GPIO_ReadPin(MTR_Ok_GPIO_Port, MTR_Ok_Pin);

		    // Send Inverter Disable
		    osMessageQueuePut(canbus_mq, &TxPacket, 0, HAL_MAX_DELAY);
		    xTaskNotify(data->canbus_task, CANBUS_APPS, eSetBits);

			if(tsalHV && brakesEngaged && rtdButton && motorOk) data->rtdFlag = true;
		}
		else
		{
			data->rtd_task = NULL;
			setMotorEn(1);
			vTaskDelete(NULL);
			while(1) osDelay(1000);
		}
		
		osDelay(10);
	}
}
