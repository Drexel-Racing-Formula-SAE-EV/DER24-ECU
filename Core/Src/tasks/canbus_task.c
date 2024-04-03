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
    xTaskCreate(canbus_task_fn, "CANBus Task", 512, (void *)data, 8, &handle);
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
    ams_data_packet_t ams_data_dest[] = {
    		{0,  &ams->air_status,        &ams->imd_status,        &ams->current},
			{1,  &ams->max_temp,          &ams->min_volt,          &ams->max_volt},
    		{2,  &ams->segs[0].volts[0],  &ams->segs[0].volts[1],  &ams->segs[0].volts[2]},
			{3,  &ams->segs[0].volts[3],  &ams->segs[0].volts[4],  &ams->segs[0].volts[5]},
			{4,  &ams->segs[0].volts[6],  &ams->segs[0].volts[7],  &ams->segs[0].volts[8]},
			{5,  &ams->segs[0].volts[9],  &ams->segs[0].volts[10], &ams->segs[0].volts[11]},
			{6,  &ams->segs[0].volts[12], &ams->segs[0].volts[13], NULL},
			{7,  &ams->segs[1].volts[0],  &ams->segs[1].volts[1],  &ams->segs[1].volts[2]},
			{8,  &ams->segs[1].volts[3],  &ams->segs[1].volts[4],  &ams->segs[1].volts[5]},
			{9,  &ams->segs[1].volts[6],  &ams->segs[1].volts[7],  &ams->segs[1].volts[8]},
			{10, &ams->segs[1].volts[9],  &ams->segs[1].volts[10], &ams->segs[1].volts[11]},
			{11, &ams->segs[1].volts[12], &ams->segs[1].volts[13], NULL},
			{12, &ams->segs[2].volts[0],  &ams->segs[2].volts[1],  &ams->segs[2].volts[2]},
			{13, &ams->segs[2].volts[3],  &ams->segs[2].volts[4],  &ams->segs[2].volts[5]},
			{14, &ams->segs[2].volts[6],  &ams->segs[2].volts[7],  &ams->segs[2].volts[8]},
			{15, &ams->segs[2].volts[9],  &ams->segs[2].volts[10], &ams->segs[2].volts[11]},
			{16, &ams->segs[2].volts[12], &ams->segs[2].volts[13], NULL},
			{17, &ams->segs[3].volts[0],  &ams->segs[3].volts[1],  &ams->segs[3].volts[2]},
			{18, &ams->segs[3].volts[3],  &ams->segs[3].volts[4],  &ams->segs[3].volts[5]},
			{19, &ams->segs[3].volts[6],  &ams->segs[3].volts[7],  &ams->segs[3].volts[8]},
			{20, &ams->segs[3].volts[9],  &ams->segs[3].volts[10], &ams->segs[3].volts[11]},
			{21, &ams->segs[3].volts[12], &ams->segs[3].volts[13], NULL},
			{22, &ams->segs[4].volts[0],  &ams->segs[4].volts[1],  &ams->segs[4].volts[2]},
			{23, &ams->segs[4].volts[3],  &ams->segs[4].volts[4],  &ams->segs[4].volts[5]},
			{24, &ams->segs[4].volts[6],  &ams->segs[4].volts[7],  &ams->segs[4].volts[8]},
			{25, &ams->segs[4].volts[9],  &ams->segs[4].volts[10], &ams->segs[4].volts[11]},
			{26, &ams->segs[4].volts[12], &ams->segs[4].volts[13], NULL},
			{27, &ams->segs[0].temps[0],  &ams->segs[0].temps[1],  &ams->segs[0].temps[2]},
			{28, &ams->segs[0].temps[3],  &ams->segs[0].temps[4],  &ams->segs[0].temps[5]},
			{29, &ams->segs[0].temps[6],  &ams->segs[0].temps[7],  &ams->segs[0].temps[8]},
			{30, &ams->segs[0].temps[9],  &ams->segs[0].temps[10], &ams->segs[0].temps[11]},
			{31, &ams->segs[0].temps[12], &ams->segs[0].temps[13], &ams->segs[0].temps[14]},
			{32, &ams->segs[0].temps[15], &ams->segs[0].temps[16], NULL},
			{33, &ams->segs[1].temps[0],  &ams->segs[1].temps[1],  &ams->segs[1].temps[2]},
			{34, &ams->segs[1].temps[3],  &ams->segs[1].temps[4],  &ams->segs[1].temps[5]},
			{35, &ams->segs[1].temps[6],  &ams->segs[1].temps[7],  &ams->segs[1].temps[8]},
			{36, &ams->segs[1].temps[9],  &ams->segs[1].temps[10], &ams->segs[1].temps[11]},
			{37, &ams->segs[1].temps[12], &ams->segs[1].temps[13], &ams->segs[1].temps[14]},
			{38, &ams->segs[1].temps[15], &ams->segs[1].temps[16], NULL},
			{39, &ams->segs[2].temps[0],  &ams->segs[2].temps[1],  &ams->segs[2].temps[2]},
			{40, &ams->segs[2].temps[3],  &ams->segs[2].temps[4],  &ams->segs[2].temps[5]},
			{41, &ams->segs[2].temps[6],  &ams->segs[2].temps[7],  &ams->segs[2].temps[8]},
			{42, &ams->segs[2].temps[9],  &ams->segs[2].temps[10], &ams->segs[2].temps[11]},
			{43, &ams->segs[2].temps[12], &ams->segs[2].temps[13], &ams->segs[2].temps[14]},
			{44, &ams->segs[2].temps[15], &ams->segs[2].temps[16], NULL},
			{45, &ams->segs[3].temps[0],  &ams->segs[3].temps[1],  &ams->segs[3].temps[2]},
			{46, &ams->segs[3].temps[3],  &ams->segs[3].temps[4],  &ams->segs[3].temps[5]},
			{47, &ams->segs[3].temps[6],  &ams->segs[3].temps[7],  &ams->segs[3].temps[8]},
			{48, &ams->segs[3].temps[9],  &ams->segs[3].temps[10], &ams->segs[3].temps[11]},
			{49, &ams->segs[3].temps[12], &ams->segs[3].temps[13], &ams->segs[3].temps[14]},
			{50, &ams->segs[3].temps[15], &ams->segs[3].temps[16], NULL},
			{51, &ams->segs[4].temps[0],  &ams->segs[4].temps[1],  &ams->segs[4].temps[2]},
			{52, &ams->segs[4].temps[3],  &ams->segs[4].temps[4],  &ams->segs[4].temps[5]},
			{53, &ams->segs[4].temps[6],  &ams->segs[4].temps[7],  &ams->segs[4].temps[8]},
			{54, &ams->segs[4].temps[9],  &ams->segs[4].temps[10], &ams->segs[4].temps[11]},
			{55, &ams->segs[4].temps[12], &ams->segs[4].temps[13], &ams->segs[4].temps[14]},
			{56, &ams->segs[4].temps[15], &ams->segs[4].temps[16], NULL},
			{57, &ams->fans[0],           &ams->fans[1],           &ams->fans[2]},
			{58, &ams->fans[3],           &ams->fans[4],           &ams->fans[5]},
			{59, &ams->fans[6],           &ams->fans[7],           &ams->fans[8]},
			{60, &ams->fans[9],           NULL,                    NULL}
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
            	ams_data_packet_t packet_locs = ams_data_dest[header];
            	uint16_t packet_header = packet_locs.header;
            	if(packet_header == header)
            	{
            		if(packet_locs.d0) *packet_locs.d0 = data0;
            		if(packet_locs.d1) *packet_locs.d1 = data1;
            		if(packet_locs.d2) *packet_locs.d2 = data2;
            	}
            	// TODO: Implement data logging function
            }
        }
        data->mq_fault = (mq_status != osOK);
    }
}


