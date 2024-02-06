/**
* @file bppc_task.h
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2023-09-28
*
* @copyright Copyright (c) 2023
*
*/

#ifndef __BPPC_TASK_H_
#define __BPPC_TASK_H_

#include "cmsis_os.h"
#include "app.h"

/**
* @brief Starts the BPPC task
*
* @param data App data structure pointer
* @return TaskHandle_t Handle used for task
*/
TaskHandle_t bppc_task_start(app_data_t *data);

#endif
