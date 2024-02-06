/**
 * @file cli.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-10-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ext_drivers/cli.h"

void cli_device_init(cli_device_t *dev, UART_HandleTypeDef *huart){
    dev->huart = huart;
    dev->index = 0;
    dev->msg_pending = false;
}
