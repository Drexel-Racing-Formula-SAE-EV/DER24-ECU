/**
* @file cli_task.h
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-10-24
*
* @copyright Copyright (c) 2023
*
*/

#ifndef __CLI_TASK_H_
#define __CLI_TASK_H_

#include "app.h"

#include "cmsis_os.h"
/**
* @brief Starts the CLI task
*
* @param data App data structure pointer
* @return TaskHandle_t Handle used for task
*/
TaskHandle_t cli_task_start(app_data_t *data);

#endif
