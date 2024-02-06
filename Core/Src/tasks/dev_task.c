/*
 * dev_task.c
 *
 *  Created on: Mar 14, 2023
 *      Author: colebardin
 */

#include "tasks/dev_task.h"

void dev_task_fn(void *args);

TaskHandle_t dev_task_start(struct app_data *data)
{
    TaskHandle_t handle;
    xTaskCreate(dev_task_fn, "Development Task", 128, (void *)data, tskIDLE_PRIORITY + 1, &handle);
    return handle;
}

void dev_task_fn(void *args)
{
    struct app_data *data = (struct app_data *)args;

    while(1){
    	/* digitize the analog signal of the Brake pressure Transducers*/

	}

    // Get rid of unused warning for dev task.
    (void)data;
}
