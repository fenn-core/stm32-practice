/*
 * mpu6050.h
 *
 *  Created on: May 9, 2026
 *      Author: pusana
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include <stdbool.h>
#include "main.h"

#define MPU6050_BUFFER_SIZE 14

typedef struct{
	uint8_t mpu6050_addr;
	I2C_HandleTypeDef *handle;
}mpu6050_config_t;

typedef struct{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
}mpu6050_data_t;

typedef struct{
	mpu6050_config_t config;
	uint8_t chip_id;
	uint8_t read_buffer[MPU6050_BUFFER_SIZE];
	mpu6050_data_t mpu6050_data;
}mpu6050_t;


HAL_StatusTypeDef mpu6050_read_chip_id(mpu6050_t *sensor);

HAL_StatusTypeDef mpu6050_init(mpu6050_t *sensor, I2C_HandleTypeDef *passed_handle, uint8_t passed_address);

void mpu6050_compute_values(mpu6050_t *sensor, I2C_HandleTypeDef *passed_handle, uint8_t passed_address);


#endif /* INC_MPU6050_H_ */
