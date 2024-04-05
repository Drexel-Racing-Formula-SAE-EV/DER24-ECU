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
    xTaskCreate(error_task_fn, "ERROR task", 128, (void *)data, 14, &handle);
    return handle;
}

void error_task_fn(void *arg)
{
	app_data_t *data = (app_data_t *)arg;
	RTC_HandleTypeDef *hrtc = &data->board.stm32f767.hrtc;

    uint32_t entry;

    for(;;)
    {
        entry = osKernelGetTickCount();

        HAL_PWR_EnableBkUpAccess();
        HAL_RTCEx_BKUPWrite(hrtc, RTC_BKP_DR0, '*');
        HAL_PWR_DisableBkUpAccess();

        data->cascadia_error = HAL_GPIO_ReadPin(MTR_Fault_GPIO_Port, MTR_Fault_Pin);
		data->imd_fail = HAL_GPIO_ReadPin(IMD_Fail_GPIO_Port, IMD_Fail_Pin);
		data->bms_fail = HAL_GPIO_ReadPin(BMS_Fail_GPIO_Port, BMS_Fail_Pin);
		data->bspd_fail = HAL_GPIO_ReadPin(BSPD_Fail_GPIO_Port, BSPD_Fail_Pin);

		set_ssa(data->board.ams.air_state ? 0 : 100);

		// TODO: for next rev of shutdown
		//if(!data->board.ams.air_status && !data->imd_fail && !data->bms_fail && !data->bspd_fail) set_ssa(100);
		//else set_ssa(0);

		data->hard_fault = (data->apps_fault ||
				            data->bse_fault ||
							data->cascadia_error
						    );
        
        data->soft_fault =(data->bppc_fault ||
        				   data->cli_fault ||
						   data->acc_fault ||
						   data->canbus_fault ||
						   data->dashboard_fault
						   );

		if(data->hard_fault)
		{
			set_fw(0);
			set_cascadia_enable(0);
		}

        osDelayUntil(entry + (1000 / ERROR_FREQ));
    }
}
