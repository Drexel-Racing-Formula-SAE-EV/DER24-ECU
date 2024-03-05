/**
 * @file cli.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-10-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CLI_H_
#define __CLI_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f7xx_hal.h"

#define CLI_LINE_SIZE 128

typedef struct {
    uint8_t c;
    unsigned int index;
	UART_HandleTypeDef *huart;
    bool msg_pending;
    unsigned int msg_count;
    unsigned int msg_proc;
    unsigned int msg_valid;
    char line[CLI_LINE_SIZE];
} cli_device_t;

typedef struct {
    char *name;
    void (*func)(char *arg);
    char *desc;
} command_t;

void cli_device_init(cli_device_t *dev, UART_HandleTypeDef *huart);

#endif
