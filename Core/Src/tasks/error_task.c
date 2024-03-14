/**
 * @file error_task.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief
 * @version 0.1
 * @date 2023-09-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "tasks/error_task.h"

/**
 * @brief Actual ERROR task function
 *
 * @param arg App_data struct pointer converted to void pointer
 */
void error_task_fn(void *arg);

TaskHandle_t error_task_start(app_data_t *data)
{
    TaskHandle_t handle;
    xTaskCreate(error_task_fn, "ERROR task", 128, (void *)data, 10, &handle);
    return handle;
}

void error_task_fn(void *arg)
{
	app_data_t *data = (app_data_t *)arg;
	RTC_HandleTypeDef *hrtc = &data->board.stm32f767.hrtc;

    uint32_t entryTicksCount;

    for(;;)
    {
        entryTicksCount = osKernelGetTickCount();

        //HAL_PWR_EnableBkUpAccess();
        //HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR1, 0x32);

        if(!data->hardSystemFault)
        {
            if(data->appsFaultFlag || data->bseFaultFlag)
            {
                data->hardSystemFault = true;
                setMotorEn(0);
            }
        }
        
        if(data->bppcFaultFlag)
        {
            data->softSystemFault = true;
            setMotorEn(0);
        }
        else
        {
            data->softSystemFault = false;
            setMotorEn(1);
        }

        osDelayUntil(entryTicksCount + (1000 / ERROR_FREQ));
    }
}
