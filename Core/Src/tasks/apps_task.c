/**
 * @file apps_task.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "tasks/apps_task.h"
#include "ext_drivers/canbus.h"

#define TO_LSB(x) (x & 0xff)
#define TO_MSB(x) (x >> 8)

/**
 * @brief Actual APPS task function
 *
 * @param arg App_data struct pointer converted to void pointer
 */
void apps_task_fn(void *arg);

TaskHandle_t apps_task_start(struct app_data *data) {
    TaskHandle_t handle;
    xTaskCreate(apps_task_fn, "APPS task", 128, (void *)data, 7, &handle);
    return handle;
}

void apps_task_fn(void *arg) {
    struct app_data *data = (struct app_data *)arg;
    struct poten *apps1 = &data->board.apps1;
    struct poten *apps2 = &data->board.apps2;
    osMessageQueueId_t canbus_mq = data->board.stm32f767.can1_mq;

    uint16_t throttleHex;
    canbus_packet TxPacket;
    uint32_t entryTicksCount;

    // Initialize all CANBus data values to 0
    for(uint8_t i = 0; i < 8; i++){
        TxPacket.data[i] = 0x00;
    }

    // Set CANBus Receiving ID in header
    TxPacket.id = BAMOCAR_CANBUS_RXID;
    // Set the command identifier to be Torque Command
    TxPacket.data[0] = BAMOCAR_CANBUS_TORQUE_CMD;

    while(1){
        entryTicksCount = osKernelGetTickCount();

        // Read throttle
        apps1->count = apps1->read_count(apps1->handle);
        apps2->count = apps2->read_count(apps2->handle);
        apps1->percent = potenGetPercent(apps1);
        apps1->percent = potenGetPercent(apps2);

        // T.4.2.5 (2022)
        if(!potenCheckImplausability(apps1->percent, apps2->percent, PLAUSIBILITY_THRESH, APPS_FREQ / 10)){
            data->appsFaultFlag = true;
        }

        data->throttlePercent = (apps1->percent + apps2->percent) / 2;

        if(data->hardSystemFault || data->softSystemFault){
            TxPacket.data[1] = 0x00;
            TxPacket.data[2] = 0x00;
        }else{
            throttleHex = percentToThrottleHex(data->throttlePercent);
            TxPacket.data[1] = TO_LSB(throttleHex);
            TxPacket.data[2] = TO_MSB(throttleHex);
        }

        if(data->rtdFlag){
            // Give torque command to CANBus Task
            osMessageQueuePut(canbus_mq, &TxPacket, 0, HAL_MAX_DELAY);
            xTaskNotify(data->canbus_task, CANBUS_APPS, eSetBits);
        }

        osDelayUntil(entryTicksCount + (1000 / APPS_FREQ));
    }
}
