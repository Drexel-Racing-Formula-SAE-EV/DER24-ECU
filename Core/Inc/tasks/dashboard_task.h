/**
* @file dashboard_task.h
* @author Cole Bardin (cab572@drexel.edu)
* @brief
* @version 0.1
* @date 2024-03-23
*
* @copyright Copyright (c) 2023
*
*/

#ifndef __DASHBOARD_TASK_H_
#define __DASHBOARD_TASK_H_

#include "app.h"

#include "cmsis_os.h"

/**
* @brief Starts the Dashboard task
*
* @param data App data structure pointer
* @return TaskHandle_t Handle used for task
*/
TaskHandle_t dashboard_task_start(app_data_t *data);

#endif
