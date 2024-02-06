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

TaskHandle_t bse_task_start(struct app_data *data)
{
   TaskHandle_t handle;
   xTaskCreate(bse_task_fn, "BSE task", 128, (void *)data, 7, &handle);
   return handle;
}

void bse_task_fn(void *arg){
    struct app_data *data = (struct app_data *)arg;
    struct pressTrans *bse1 = &data->board.bse1;
    struct pressTrans *bse2 = &data->board.bse2;
    bool newBrakeLightState;

    uint32_t entryTicksCount;

	while(1){
		entryTicksCount = osKernelGetTickCount();

		// Read ADC channels for each BSE input
		switchChannelADC(bse1);
		bse1->count = bse1->read_count(bse1->handle);
		switchChannelADC(bse2);
		bse2->count = bse2->read_count(bse2->handle);

		// Calculate Percentage
		bse1->percent = presstransGetPercent(bse1);
		bse2->percent = presstransGetPercent(bse2);

		// T.4.3.3 (2022)
		if(!presstransCheckImplausability(bse1->percent, bse2->percent, PLAUSIBILITY_THRESH, BSE_FREQ / 10)){
			data->bseFaultFlag = true;
		}

		// Set average value
		data->brakePercent = (bse1->percent + bse2->percent) / 2;

		// Operate brake light
		newBrakeLightState = (data->brakePercent >= BRAKE_LIGHT_THRESH);
		if(data->brakeLightState != newBrakeLightState){
			data->brakeLightState = newBrakeLightState;
			setBrakeLight(newBrakeLightState);
		}

		osDelayUntil(entryTicksCount + (1000 / BSE_FREQ));
	}
}
