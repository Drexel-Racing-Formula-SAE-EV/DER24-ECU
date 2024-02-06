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

TaskHandle_t canbus_task_start(struct app_data *data) {
    TaskHandle_t handle;
    xTaskCreate(canbus_task_fn, "CANBus Task", 128, (void *)data, 8, &handle);
    return handle;
}

void canbus_task_fn(void *arg) {
    struct app_data *data = (struct app_data *)arg;

    struct canbus_device *canbus_device = &data->board.canbus_device;
    CAN_HandleTypeDef *hcan = canbus_device->hcan;
    CAN_TxHeaderTypeDef *tx_header = data->board.canbus_device.tx_header;
    osMessageQueueId_t canbus_mq = data->board.stm32f767.can1_mq;
    canbus_packet can_packet;
    HAL_StatusTypeDef can_status;
    osStatus_t mq_status;
    uint32_t taskNotification;

    while(1) {
        xTaskNotifyWait(CANBUS_ISR | CANBUS_APPS, 0, &taskNotification, HAL_MAX_DELAY);
        // Pull canbus packet from data queue
        mq_status = osMessageQueueGet(canbus_mq, &can_packet, NULL, HAL_MAX_DELAY);
        // Upon successful message acquisition
        if(mq_status == osOK){
            if(taskNotification & CANBUS_APPS){
            	// Message from APPS task
            	tx_header->StdId = can_packet.id;
            	can_status = HAL_CAN_AddTxMessage(hcan, tx_header, can_packet.data, &canbus_device->tx_mailbox);
            }else if(taskNotification & CANBUS_ISR){
            	// Message from CANBus Received ISR
            	// TODO: Implement data logging function
            	// this is BS line for breakpoint testing, will be removed once logging is setup
            	can_status = 0;
            	// Log received message
            }
        }
    }
}

