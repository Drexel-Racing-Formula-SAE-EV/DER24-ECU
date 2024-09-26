/*
 * mpu6050.h
 *
 *  Created on: Feb 9, 2024
 *      Author: Cole Bardin & Azmain Yousuf
 */

#ifndef ___MPU6050_H_
#define ___MPU6050_H_

#include "stm32f7xx_hal.h"

#define MPU6050_ADDR0 0x68
#define MPU6050_ADDR1 0x69

#define REG_CONFIG 0x1A
#define REG_SMPLRT_DIV 0x19
#define REG_CONFIG_GYRO 27
#define REG_CONFIG_ACC 28
#define REG_USR_CTRL 107
#define REG_DATA 59
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H_REG 0x43

typedef enum
{
	EXT_SYNC_DISABLE,
	EXT_SYNC_TEMP_OUT_L0,
	EXT_SYNC_GYRO_XOUT_L0,
	EXT_SYNC_GYRO_YOUT_L0,
	EXT_SYNC_GYRO_ZOUT_L0,
	EXT_SYNC_ACC_XOUT_L0,
	EXT_SYNC_ACC_YOUT_L0,
	EXT_SYNC_ACC_ZOUT_L0,
} EXT_SYNC_SET;

typedef enum
{
	DLPF_260HZ_BW,
	DLPF_184HZ_BW,
	DLPF_94HZ_BW,
	DLPF_44HZ_BW,
	DLPF_21HZ_BW,
	DLPF_10HZ_BW,
	DLPF_5HZ_BW,
} DLPF_CFG;

typedef enum
{
	FS_SEL_250,
	FS_SEL_500,
	FS_SEL_1000,
	FS_SEL_2000,
} FS_SEL;

typedef enum
{
	AFS_SEL_2,
	AFS_SEL_4,
	AFS_SEL_8,
	AFS_SEL_16,
} AFS_SEL;

typedef enum
{
	CLKSEL_INT_8MHZ,
	CLKSEL_XGYRO,
	CLKSEL_YGYRO,
	CLKSEL_ZGYRO,
	CLKSEL_EXT_32KHZ,
	CLKSEL_EXT_19MHZ,
	CLKSEL_STOP,
} CLKSEL;

typedef struct
{
	uint8_t addr_7bit;
	uint8_t sample_rate_divisor;
	EXT_SYNC_SET external_sync;
	DLPF_CFG lowpass_filter;
	FS_SEL gyro_scale;
	AFS_SEL acc_scale;
	CLKSEL clock;
} mpu6050_config_t;

typedef struct
{
	I2C_HandleTypeDef *hi2c;
	uint8_t addr_7bit;
	float temp;
	float x_acc;
	float y_acc;
	float z_acc;
	float x_gyro;
	float y_gyro;
	float z_gyro;
	float acc_div;
	float gyro_div;
	HAL_StatusTypeDef error;
} mpu6050_t;

int mpu6050_init(mpu6050_t *dev, mpu6050_config_t *conf, I2C_HandleTypeDef *hi2c);

int mpu6050_read(mpu6050_t *dev);

#endif
