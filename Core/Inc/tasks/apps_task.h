/**
 * @file apps_task.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __APPS_TASK_H_
#define __APPS_TASK_H_

#include "cmsis_os.h"
#include "app.h"

/**
 * @brief Starts the APPS task
 * 
 * @param data App data structure pointer
 * @return TaskHandle_t Handle used for task
 */
TaskHandle_t apps_task_start(app_data_t *data);

#endif
