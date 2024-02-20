/**
 * @file acc_task.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief
 * @version 0.1
 * @date 2024-02-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "tasks/acc_task.h"

/**
 * @brief Actual ACC task function
 *
 * @param arg App_data struct pointer converted to void pointer
 */
void acc_task_fn(void *arg);

TaskHandle_t acc_task_start(app_data_t *data)
{
    TaskHandle_t handle;
    xTaskCreate(acc_task_fn, "ACC task", 128, (void *)data, 5, &handle);
    return handle;
}

void acc_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;

    uint32_t entryTicksCount;

    for(;;)
    {
        entryTicksCount = osKernelGetTickCount();

        osDelayUntil(entryTicksCount + (1000 / ACC_FREQ));
    }
}
