/*
 * mpu6050.c
 *
 *  Created on: May 9, 2026
 *      Author: pusana
 */

#include <stdbool.h>
#include "mpu6050.h"

#define WHO_AM_I     0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B


HAL_StatusTypeDef read_register(mpu6050_t *sensor, uint8_t reg, uint8_t *addr){
	return HAL_I2C_Mem_Read(sensor->config.handle, sensor->config.mpu6050_addr, reg, 1, addr, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef write_register(mpu6050_t *sensor, uint8_t reg, uint8_t value){
	return HAL_I2C_Mem_Write(sensor->config.handle, sensor->config.mpu6050_addr, reg, 1, &value, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef read_registers(mpu6050_t *sensor, uint8_t start_reg, uint8_t length){
	return HAL_I2C_Mem_Read(sensor->config.handle, sensor->config.mpu6050_addr, start_reg, 1, sensor->read_buffer, length, HAL_MAX_DELAY);
}

HAL_StatusTypeDef mpu6050_read_chip_id(mpu6050_t *sensor){
	return read_register(sensor, WHO_AM_I, &sensor->chip_id);
}

HAL_StatusTypeDef mpu6050_init(mpu6050_t *sensor, I2C_HandleTypeDef *passed_handle, uint8_t passed_address){
	sensor->config.handle = passed_handle;
	sensor->config.mpu6050_addr = passed_address << 1;

	if (mpu6050_read_chip_id(sensor) != HAL_OK){
		return HAL_ERROR;
	}

	if (sensor->chip_id != 0x68 && sensor->chip_id != 0x70 && sensor->chip_id != 0x71){
		return HAL_ERROR;
	}

	if (write_register(sensor, PWR_MGMT_1, 0x00) != HAL_OK){
		return HAL_ERROR;
	}

	return HAL_OK;
}

void mpu6050_compute_values(mpu6050_t *sensor, I2C_HandleTypeDef *passed_handle, uint8_t passed_address){
	mpu6050_data_t *data = &sensor->mpu6050_data;

	read_registers(sensor, ACCEL_XOUT_H, 14);

	data->accel_x = (((uint16_t)sensor->read_buffer[0] << 8) | ((uint16_t)sensor->read_buffer[1]));
	data->accel_y = (((uint16_t)sensor->read_buffer[2] << 8) | ((uint16_t)sensor->read_buffer[3]));
	data->accel_z = (((uint16_t)sensor->read_buffer[4] << 8) | ((uint16_t)sensor->read_buffer[5]));
	data->gyro_x = (((uint16_t)sensor->read_buffer[8] << 8) | ((uint16_t)sensor->read_buffer[9]));
	data->gyro_y = (((uint16_t)sensor->read_buffer[10] << 8) | ((uint16_t)sensor->read_buffer[11]));
	data->gyro_z = (((uint16_t)sensor->read_buffer[12] << 8) | ((uint16_t)sensor->read_buffer[13]));

}
