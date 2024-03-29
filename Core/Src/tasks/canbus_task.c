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
    xTaskCreate(canbus_task_fn, "CANBus Task", 128, (void *)data, 8, &handle);
    return handle;
}

void canbus_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;

    canbus_device_t *canbus_device = &data->board.canbus_device;
    CAN_HandleTypeDef *hcan = canbus_device->hcan;
    CAN_TxHeaderTypeDef *tx_header = data->board.canbus_device.tx_header;
    osMessageQueueId_t canbus_mq = data->board.stm32f767.can1_mq;
    ams_t *ams = &data->board.ams;
    canbus_packet_t can_packet;
    HAL_StatusTypeDef can_status;
    osStatus_t mq_status;
    uint32_t taskNotification;

    // See ECU_AMS_CANBUS_Protocol.xlsx
    uint16_t *d0_locs[] = {
    		&ams->air_status, &ams->max_temp,
    		&ams->segs[0].volts[0], &ams->segs[0].volts[3], &ams->segs[0].volts[6], &ams->segs[0].volts[9],  &ams->segs[0].volts[12],
			&ams->segs[1].volts[0], &ams->segs[1].volts[3], &ams->segs[1].volts[6], &ams->segs[1].volts[9],  &ams->segs[1].volts[12],
			&ams->segs[2].volts[0], &ams->segs[2].volts[3], &ams->segs[2].volts[6], &ams->segs[2].volts[9],  &ams->segs[2].volts[12],
			&ams->segs[3].volts[0], &ams->segs[3].volts[3], &ams->segs[3].volts[6], &ams->segs[3].volts[9],  &ams->segs[3].volts[12],
			&ams->segs[4].volts[0], &ams->segs[4].volts[3], &ams->segs[4].volts[6], &ams->segs[4].volts[9],  &ams->segs[4].volts[12]
    };
    uint16_t *d1_locs[] = {
    		&ams->imd_status, &ams->min_volt,
			&ams->segs[0].volts[1], &ams->segs[0].volts[4], &ams->segs[0].volts[7], &ams->segs[0].volts[10], &ams->segs[0].volts[13],
			&ams->segs[1].volts[1], &ams->segs[1].volts[4], &ams->segs[1].volts[7], &ams->segs[1].volts[10], &ams->segs[1].volts[13],
			&ams->segs[2].volts[1], &ams->segs[2].volts[4], &ams->segs[2].volts[7], &ams->segs[2].volts[10], &ams->segs[2].volts[13],
			&ams->segs[3].volts[1], &ams->segs[3].volts[4], &ams->segs[3].volts[7], &ams->segs[3].volts[10], &ams->segs[3].volts[13],
			&ams->segs[4].volts[1], &ams->segs[4].volts[4], &ams->segs[4].volts[7], &ams->segs[4].volts[10], &ams->segs[4].volts[13]
    };
    uint16_t *d2_locs[] = {
    		&ams->current,    &ams->max_volt,
			&ams->segs[0].volts[2], &ams->segs[0].volts[5], &ams->segs[0].volts[8], &ams->segs[0].volts[11], NULL,
			&ams->segs[1].volts[2], &ams->segs[1].volts[5], &ams->segs[1].volts[8], &ams->segs[1].volts[11], NULL,
			&ams->segs[2].volts[2], &ams->segs[2].volts[5], &ams->segs[2].volts[8], &ams->segs[2].volts[11], NULL,
			&ams->segs[3].volts[2], &ams->segs[3].volts[5], &ams->segs[3].volts[8], &ams->segs[3].volts[11], NULL,
			&ams->segs[4].volts[2], &ams->segs[4].volts[5], &ams->segs[4].volts[8], &ams->segs[4].volts[11], NULL
    };

    for(;;)
    {
        xTaskNotifyWait(CANBUS_ISR | CANBUS_APPS, 0, &taskNotification, HAL_MAX_DELAY);
        mq_status = osMessageQueueGet(canbus_mq, &can_packet, NULL, HAL_MAX_DELAY);
        if(mq_status == osOK)
        {
            if(taskNotification & CANBUS_APPS)
            {
            	tx_header->StdId = can_packet.id;
            	can_status = HAL_CAN_AddTxMessage(hcan, tx_header, can_packet.data, &canbus_device->tx_mailbox);
            	data->canbus_fault = (can_status != HAL_OK);
            }
            else if(taskNotification & CANBUS_ISR)
            {
            	uint16_t header = ((uint16_t)can_packet.data[0] << 8) | can_packet.data[1];
            	uint16_t data0  = ((uint16_t)can_packet.data[2] << 8) | can_packet.data[3];
            	uint16_t data1  = ((uint16_t)can_packet.data[4] << 8) | can_packet.data[5];
            	uint16_t data2  = ((uint16_t)can_packet.data[6] << 8) | can_packet.data[7];
            	// TODO: Implement data logging function
            	*d0_locs[header] = data0;
            	*d1_locs[header] = data1;
            	*d2_locs[header] = data2;
            }
        }
    }
    (void)can_status; // TODO: remove once this variable is removed
}


