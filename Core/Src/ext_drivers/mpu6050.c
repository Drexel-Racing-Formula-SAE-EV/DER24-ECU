/*
 * mpu6050.c
 *
 *  Created on: Feb 19, 2024
 *      Author: Cole Bardin & Azmain Yousuf
 */

#include "ext_drivers/mpu6050.h"

const float GRYO_DIVS[4] = {131.0, 65.5, 32.8, 16.4};
const float ACC_DIVS[4] = {16384.0, 8192.0, 4096.0, 2048.0};

int mpu6050_init(mpu6050_device_t *dev, mpu6050_config_t *conf, I2C_HandleTypeDef *hi2c){
	uint8_t temp_data;
	HAL_StatusTypeDef ret = 0;

	dev->addr_7bit = conf->addr_7bit;
	dev->hi2c = hi2c;
	dev->temp = 0.0;
	dev->x_acc = 0.0;
	dev->y_acc = 0.0;
	dev->z_acc = 0.0;
	dev->x_gyro = 0.0;
	dev->y_gyro = 0.0;
	dev->z_gyro = 0.0;
	dev->gyro_div = GRYO_DIVS[conf->gyro_scale];
	dev->acc_div = ACC_DIVS[conf->acc_scale];

	ret |= HAL_I2C_IsDeviceReady(dev->hi2c, (dev->addr_7bit << 1), 100, 100);

	temp_data = conf->sample_rate_divisor;
	ret |= HAL_I2C_Mem_Write(dev->hi2c, (dev->addr_7bit << 1), REG_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &temp_data, 1, 200);

	temp_data = conf->lowpass_filter;
	temp_data |= conf->external_sync << 3;
	ret |= HAL_I2C_Mem_Write(dev->hi2c, (dev->addr_7bit << 1), REG_CONFIG, 1, &temp_data, 1, 200);

	temp_data = conf->gyro_scale << 3;
	ret |= HAL_I2C_Mem_Write(dev->hi2c, (dev->addr_7bit << 1), REG_CONFIG_GYRO, 1, &temp_data, 1, 200);

	temp_data = conf->acc_scale << 3;
	ret |= HAL_I2C_Mem_Write(dev->hi2c, (dev->addr_7bit << 1), REG_CONFIG_ACC, 1, &temp_data, 1, 200);

	temp_data = conf->clock;
	ret |= HAL_I2C_Mem_Write(dev->hi2c, (dev->addr_7bit << 1), REG_USR_CTRL, 1, &temp_data, 1, 200);

	return ret;
}

int mpu6050_read(mpu6050_device_t *dev){
	HAL_StatusTypeDef ret;
	uint8_t data[14] = {0};

	ret = HAL_I2C_Mem_Read(dev->hi2c, (dev->addr_7bit << 1), ACCEL_XOUT_H, 1, data, 14, 200);
	dev->error = ret;
    int x_accR = ((int)data[0] << 8) | data[1];
    int y_accR = ((int)data[2] << 8) | data[3];
    int z_accR = ((int)data[4] << 8) | data[5];
    int tempR  = ((int)data[6] << 8) | data[7];
    int x_gyroR = ((int)data[8] << 8) | data[9];
    int y_gyroR = ((int)data[10] << 8) | data[11];
    int z_gyroR = ((int)data[12] << 8) | data[13];
    dev->x_acc = (float)x_accR/ dev->acc_div;
    dev->y_acc = (float)y_accR / dev->acc_div;
    dev->z_acc = (float)z_accR / dev->acc_div;
    dev->x_gyro = (float)x_gyroR / dev->gyro_div;
    dev->y_gyro = (float)y_gyroR / dev->gyro_div;
    dev->z_gyro = (float)z_gyroR / dev->gyro_div;
    dev->temp = ((float)(tempR) / 340.0) + 36.53;

    return ret;
}
