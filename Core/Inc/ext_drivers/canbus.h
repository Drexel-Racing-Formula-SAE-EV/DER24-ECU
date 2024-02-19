/**
 * @file canbus.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CANBUS_H_
#define __CANBUS_H_

#include "cmsis_os.h"
#include "stm32f7xx_hal.h"

#define DATALEN 8

typedef struct {
    uint32_t id;
    uint8_t data[DATALEN];
} canbus_packet_t;

typedef struct {
    CAN_HandleTypeDef *hcan;
    CAN_TxHeaderTypeDef *tx_header;
    uint32_t tx_mailbox;
    canbus_packet_t rx_packet;
} canbus_device_t;

void canbus_device_init(canbus_device_t *dev, CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *tx_header);

#endif
