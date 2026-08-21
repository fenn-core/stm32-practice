/*
 * mpu6500.h
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */

#ifndef INC_MPU6500_H_
#define INC_MPU6500_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define MPU6500_DEFAULT_CALIBRATION_SAMPLES  1000U
#define MPU6500_DEFAULT_CALIBRATION_DELAY    2U


typedef struct{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;

	int16_t temperature;

	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;

}mpu6500_raw_t;


typedef struct{
    float accel_x_g;
    float accel_y_g;
    float accel_z_g;

    float temperature_c;

    float gyro_x_dps;
    float gyro_y_dps;
    float gyro_z_dps;

}mpu6500_data_t;


typedef struct{
	float gyro_x_bias_raw;
	float gyro_y_bias_raw;
	float gyro_z_bias_raw;

}mpu6050_calib_t;


typedef struct{
		I2C_HandleTypeDef *handle;
		uint16_t address;
		uint8_t chip_id;

		uint8_t readbuffer[14];

		mpu6500_raw_t raw;
		mpu6500_data_t data;
		mpu6050_calib_t calib;

}mpu6500_t;


HAL_StatusTypeDef mpu6500_init(mpu6500_t *sensor, I2C_HandleTypeDef *handle, uint8_t address);

HAL_StatusTypeDef mpu6500_read_data(mpu6500_t *sensor);

HAL_StatusTypeDef mpu6050_calibrate_gyro(mpu6500_t *sensor, uint16_t sample_count, uint8_t sample_delay);


#endif /* INC_MPU6500_H_ */
