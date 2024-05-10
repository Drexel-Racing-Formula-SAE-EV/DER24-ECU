/**
 * @file board.h
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BOARD_H_
#define __BOARD_H_

#include <stdbool.h>

#include "ext_drivers/stm32f767.h"
#include "ext_drivers/poten.h"
#include "ext_drivers/pressure_sensor.h"
#include "ext_drivers/canbus.h"
#include "ext_drivers/cli.h"
#include "ext_drivers/mpu6050.h"
#include "ext_drivers/dashboard.h"
#include "ext_drivers/flow_sensor.h"
#include "ext_drivers/ntc.h"
#include "ext_drivers/pwm.h"
#include "ext_drivers/ams.h"

// 0.12V - 1.8V * (3/2) resistor divider => 0.18V - 2.7V
#define BSE1_MIN 155//Brake emulator min: 155 //Theoretical value (ADC max): 339
#define BSE1_MAX 2238 //Brake emulator max: 2240 //Theoretical value (ADC max): 1900
// 0.14V -1.8V *(3/2) => 0.21V - 2.7V
#define BSE2_MIN 175 //Brake emulator min: 175 //Theoretical value (ADC max): 810
#define BSE2_MAX 2250 //Brake emulator max: 2250 //Theoretical value (ADC max): 2158
// TODO: Calibrate
// 0.5V-4.5V Sensor output * 2/3 VDiv = 0.33V-3V * 4095 / 3.3V = 413Ct-3723Ct
#define COOL_PRESS_MIN 413
#define COOL_PRESS_MAX 3723

#define BSE1_ADC_CH 13
#define BSE2_ADC_CH 9
#define COOL_PRESS_ADC_CH 7
#define COOL_TEMP1_CH 15
#define COOL_TEMP2_CH 14

#define APPS1_0 85
#define APPS1_100 195
#define APPS2_0 95
#define APPS2_100 305

#define CANBUS_ISR 	0x2	// Notification bit value for ISR messages
#define CANBUS_APPS	0x1 // Notification bit value for APPS messages 

#define MTR_CMD_ID 0x0C0

typedef struct {
	stm32f767_device_t stm32f767;
	poten_t apps1;
	poten_t apps2;
	pressure_sensor_t bse1;
	pressure_sensor_t bse2;
	pressure_sensor_t cool_pressure;
 	flow_sensor_t cool_flow;
 	ntc_t cool_temp1;
 	ntc_t cool_temp2;
 	pwm_device_t cool_pump;
 	canbus_device_t canbus_device;
	cli_device_t cli;
	mpu6050_device_t mpu6050;
	dashboard_t dashboard;
	pwm_device_t ssa;
	ams_t ams;
} board_t;

void board_init(board_t *dev);

#endif
