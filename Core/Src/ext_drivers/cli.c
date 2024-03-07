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
#include <string.h>

#include "cmsis_os.h"

#include "ext_drivers/cli.h"

void cli_device_init(cli_device_t *dev, UART_HandleTypeDef *huart)
{
    dev->huart = huart;
    dev->index = 0;
    dev->msg_pending = false;
    dev->msg_count = 0;
    dev->msg_proc = 0;
    dev->msg_valid = 0;
    dev->ret = 0;
}

int cli_printline(cli_device_t *dev, char *line)
{
	static char nl[] = "\r\n";
	HAL_StatusTypeDef ret = 0;

	if(xPortIsInsideInterrupt())
	{
		ret |= HAL_UART_Transmit_IT(dev->huart, (uint8_t *)line, strlen(line));
		ret |= HAL_UART_Transmit_IT(dev->huart, (uint8_t*)nl, strlen(nl));
	}
	else
	{
		//while(osMutexAcquire(app.board.stm32f767.uart3_mutex, 0) != osOK) osDelay(5);
		ret |= HAL_UART_Transmit(dev->huart, (uint8_t *)line, strlen(line), HAL_MAX_DELAY);
		ret |= HAL_UART_Transmit(dev->huart, (uint8_t *)nl, strlen(nl), HAL_MAX_DELAY);
		//osMutexRelease(app.board.stm32f767.uart3_mutex);
	}
	return ret;
}

int tokenize(char *s, char *toks[], int maxtoks, char *delim)
{
	int i = 0;

	toks[i] = (char *)strtok(s, delim);
	while(toks[i++] != NULL)
	{
		if(i >= maxtoks - 1) toks[i] = NULL;
		else toks[i] = (char *)strtok(NULL, delim);
	}
	return i - 1;
}
