/**
 * @file board.c
 * @author Cole Bardin (cab572@drexel.edu)
 * @brief 
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "board.h"
#include "main.h"

void board_init(board_t *dev)
{
	stm32f767_init(&dev->stm32f767);

	poten_init(&dev->apps1, APPS1_0, APPS1_100, &dev->stm32f767.hadc1);
	poten_init(&dev->apps2, APPS2_0, APPS2_100, &dev->stm32f767.hadc2);
	pressure_sensor_init(&dev->bse1, BSE1_MIN, BSE1_MAX, &dev->stm32f767.hadc3, BSE1_ADC_CH);
	pressure_sensor_init(&dev->bse2, BSE2_MIN, BSE2_MAX, &dev->stm32f767.hadc3, BSE2_ADC_CH);
	pressure_sensor_init(&dev->cool_pressure, COOL_PRESS_MIN, COOL_PRESS_MAX, &dev->stm32f767.hadc3, COOL_PRESS_ADC_CH);
	flow_sensor_init(&dev->cool_flow, 54000000, &dev->stm32f767.htim5, TIM5, TIM_CHANNEL_2, TIM_CHANNEL_1);
	ntc_init(&dev->cool_temp1, &dev->stm32f767.hadc3, COOL_TEMP1_CH);
	ntc_init(&dev->cool_temp2, &dev->stm32f767.hadc3, COOL_TEMP2_CH);
	pwm_device_init(&dev->cool_pump, TIM4, &dev->stm32f767.htim4, 65535, &TIM4->CCR3, 3);

	canbus_device_init(&dev->canbus_device, &dev->stm32f767.hcan1, &dev->stm32f767.can1_txheader);

	cli_device_init(&dev->cli, &dev->stm32f767.huart3);

	pwm_device_init(&dev->ssa, TIM3, &dev->stm32f767.htim3, 65535, &TIM3->CCR4, 4);

	mpu6050_config_t mpu6050_conf = {0};
	mpu6050_conf.addr_7bit = MPU6050_ADDR1;
	mpu6050_conf.sample_rate_divisor = 0;
	mpu6050_conf.external_sync = EXT_SYNC_DISABLE;
	mpu6050_conf.lowpass_filter = DLPF_260HZ_BW;
	mpu6050_conf.gyro_scale = FS_SEL_250;
	mpu6050_conf.acc_scale = AFS_SEL_2;
	mpu6050_conf.clock = CLKSEL_INT_8MHZ;
	mpu6050_init(&dev->mpu6050, &mpu6050_conf, &dev->stm32f767.hi2c2);

	dashboard_init(&dev->dashboard, &dev->stm32f767.huart7);
}
