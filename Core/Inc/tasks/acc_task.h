/**
 * @file acc_task.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __ACC_TASK_H_
#define __ACC_TASK_H_

#include "app.h"

#include "cmsis_os.h"

/**
 * @brief Starts the ACC task
 * 
 * @param data App data structure pointer
 * @return TaskHandle_t Handle used for task
 */
TaskHandle_t acc_task_start(app_data_t *data);

#endif
