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

#pragma once

#include "cmsis_os.h"
#include <stm32f7xx_hal.h>

#define DATALEN 8

typedef struct {
    uint32_t id;
    uint8_t data[DATALEN];
} canbus_packet;

struct canbus_device {
    CAN_HandleTypeDef *hcan;
    CAN_TxHeaderTypeDef *tx_header;
    uint32_t tx_mailbox;
    canbus_packet rx_packet;
};

void canbus_device_init(struct canbus_device *dev, CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *tx_header);
