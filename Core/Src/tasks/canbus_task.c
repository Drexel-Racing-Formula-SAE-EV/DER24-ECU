/**
 * @file canbus_task.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "tasks/canbus_task.h"
#include "ext_drivers/canbus.h"

/**
 * @brief CANBus task function
 * 
 * @param arg App_data struct pointer converted to void pointer
 */
void canbus_task_fn(void *arg);

TaskHandle_t canbus_task_start(app_data_t *data) {
    TaskHandle_t handle;
    xTaskCreate(canbus_task_fn, "CANBus Task", 512, (void *)data, 14, &handle);
    return handle;
}

void canbus_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;

    canbus_device_t *canbus = &data->board.canbus_device;
    CAN_HandleTypeDef *hcan = canbus->hcan;
    CAN_TxHeaderTypeDef *tx_header = data->board.canbus_device.tx_header;
    canbus_packet_t can_packet;
    HAL_StatusTypeDef can_status;
    osStatus_t mq_status;
    uint32_t task_notification;

    for(;;)
    {
    	xTaskNotifyWait(CANBUS_APPS, 0, &task_notification, HAL_MAX_DELAY);
		if(task_notification & CANBUS_APPS)
		{
			taskENTER_CRITICAL();
			for(int i = 0; i < 8; i++) can_packet.data[i] = canbus->tx_packet.data[i];
			tx_header->StdId = canbus->tx_packet.id;
			for(int i = 0; i < 8; i++) canbus->tx_packet.data[i] = 0;
			taskEXIT_CRITICAL();
			can_status = HAL_CAN_AddTxMessage(hcan, tx_header, can_packet.data, &canbus->tx_mailbox);
			data->canbus_fault = (can_status != HAL_OK);
		}
	}
    data->mq_fault = (mq_status != osOK);
}


