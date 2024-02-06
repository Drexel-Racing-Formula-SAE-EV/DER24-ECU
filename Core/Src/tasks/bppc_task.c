/**
* @file bppc_task.c
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-09-28
*
* @copyright Copyright (c) 2023
*
*/

#include "tasks/bppc_task.h"
#include "main.h"

/**
* @brief Actual BPPC task function
*
* @param arg App_data struct pointer converted to void pointer
*/
void bppc_task_fn(void *arg);

TaskHandle_t bppc_task_start(struct app_data *data)
{
   TaskHandle_t handle;
   xTaskCreate(bppc_task_fn, "BPPC task", 128, (void *)data, 8, &handle);
   return handle;
}

void bppc_task_fn(void *arg){
    struct app_data *data = (struct app_data *)arg;
	   
    uint32_t entryTicksCount;
	bool brakesEnganged = false;
	bool throttleEngaged = false;
	bool throttleReleased = true;

	while(1){
		entryTicksCount = osKernelGetTickCount();

		brakesEnganged = (data->brakePercent > BPPC_BSE_THRESH);
		throttleEngaged = (data->throttlePercent > BPPC_APPS_H_THRESH);
		throttleReleased = (data->throttlePercent < BPPC_APPS_L_THRESH);

		// EV.5.7 (2022)
		if(data->bppcFaultFlag == true){
			if(throttleReleased){
				data->bppcFaultFlag = false;
			}
		}else if(brakesEnganged && throttleEngaged){
			data->bppcFaultFlag = true;
		}

		osDelayUntil(entryTicksCount + (1000 / BPPC_FREQ));
	}
}
