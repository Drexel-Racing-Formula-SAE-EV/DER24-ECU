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

#include <math.h>

#include "main.h"
#include "tasks/apps_task.h"
#include "ext_drivers/canbus.h"

#define TO_LSB(x) (x & 0xff)
#define TO_MSB(x) (x >> 8) & 0xff

/**
 * @brief Actual APPS task function
 *
 * @param arg App_data struct pointer converted to void pointer
 */
void apps_task_fn(void *arg);

TaskHandle_t apps_task_start(app_data_t *data)
{
    TaskHandle_t handle;
    xTaskCreate(apps_task_fn, "APPS task", 128, (void *)data, 10, &handle);
    return handle;
}

void apps_task_fn(void *arg)
{
    app_data_t *data = (app_data_t *)arg;
    poten_t *apps1 = &data->board.apps1;
    poten_t *apps2 = &data->board.apps2;
    canbus_packet_t *tx_packet = &data->board.canbus_device.tx_packet;

    float throttle_raw;
    uint16_t throttle_hex;
    uint32_t entry;

    for(uint8_t i = 0; i < 8; i++) tx_packet->data[i] = 0x00;
    tx_packet->id = MTR_CMD_ID;
    xTaskNotify(data->canbus_task, CANBUS_APPS, eSetBits);

    for(;;)
    {
        entry = osKernelGetTickCount();

        apps1->count = stm32f767_adc_read(apps1->handle);
        apps2->count = stm32f767_adc_read(apps2->handle);
        apps1->percent = poten_get_percent(apps1);
        apps2->percent = poten_get_percent(apps2);
        data->throttle_diff = apps1->percent - apps2->percent;

        throttle_raw = (apps1->percent + apps2->percent) / 2;
        data->throttle = (int)throttle_raw;

        // T.4.2.5 (2022)
        if(!poten_check_plausibility(apps1->percent, apps2->percent, PLAUSIBILITY_THRESH, APPS_FREQ / 10))
        {
            data->apps_fault = true;
        }
        if(data->hard_fault || data->apps_fault || !data->rtd_state)
        {
            tx_packet->data[0] = 0;
            tx_packet->data[1] = 0;
            tx_packet->data[2] = 0;
            tx_packet->data[3] = 0;
            tx_packet->data[4] = 1; // Forward direction
            tx_packet->data[5] = 0; // Disable inverter
            tx_packet->data[6] = 0;
            tx_packet->data[7] = 0;
        }
        else
        {
            throttle_hex = (uint16_t)(data->throttle * MAXTRQ / 10.0); // CM CANBus Protocol
            tx_packet->data[0] = TO_LSB(throttle_hex);
            tx_packet->data[1] = TO_MSB(throttle_hex);
            tx_packet->data[2] = 0;
            tx_packet->data[3] = 0;
            tx_packet->data[4] = 1; // Forward direction
            tx_packet->data[5] = 1; // Enable inverter
            tx_packet->data[6] = 0;
            tx_packet->data[7] = 0;
        }
    	xTaskNotify(data->canbus_task, CANBUS_APPS, eSetBits);
        osDelayUntil(entry + (1000 / APPS_FREQ));
    }
}
