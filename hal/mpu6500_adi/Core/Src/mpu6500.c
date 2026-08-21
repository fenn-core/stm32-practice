/*
 * mpu6500.c
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */


#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "mpu6500.h"
#include "i2c_recovery.h"

#define MPU6500_WHO_AM_I      0x75
#define MPU6500_PWR_MGMT_1    0x6B
#define MPU6500_CONFIG        0x1A
#define MPU6500_GYRO_CONFIG   0x1B
#define MPU6500_ACCEL_CONFIG  0x1C
#define MPU6500_ACCEL_XOUT_H  0x3B


static HAL_StatusTypeDef mpu6500_read_register(mpu6500_t *sensor, uint8_t reg, uint8_t *value){
	return HAL_I2C_Mem_Read(sensor->handle, sensor->address, reg, I2C_MEMADD_SIZE_8BIT, value, 1, MPU6500_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef mpu6500_read_registers(mpu6500_t *sensor, uint8_t reg, uint8_t length){
	return HAL_I2C_Mem_Read(sensor->handle, sensor->address, reg, I2C_MEMADD_SIZE_8BIT, sensor->readbuffer, length, MPU6500_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef mpu6500_write_register(mpu6500_t *sensor, uint8_t reg, uint8_t value){
	return HAL_I2C_Mem_Write(sensor->handle, sensor->address, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, MPU6500_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef mpu6500_write_and_verify(mpu6500_t *sensor, uint8_t reg, uint8_t value){
	HAL_StatusTypeDef status = mpu6500_write_register(sensor, reg, value);
	if (status != HAL_OK){
		return status;
		}
	uint8_t register_value;
	status = mpu6500_read_register(sensor, reg, &register_value);
	if (status != HAL_OK){
		return status;
		}
	if (register_value != value){
		return HAL_ERROR;
	}

	return HAL_OK;

}


HAL_StatusTypeDef mpu6500_init(mpu6500_t *sensor, I2C_HandleTypeDef *handle, uint8_t address){
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t register_value = 0x00;

	sensor->handle = handle;
	sensor->address = (uint16_t)(address << 1);

	HAL_Delay(50);

	status = mpu6500_read_register(sensor, MPU6500_WHO_AM_I, &register_value);
	if (status != HAL_OK){
		return status;
		}

	if (register_value != 0x70){
		return HAL_ERROR;
		}

	sensor->chip_id = register_value;

	// Reset then set PWR_MGMT_1 to ON

	status = mpu6500_write_register(sensor, MPU6500_PWR_MGMT_1, 0x80);
	if (status != HAL_OK){
		return status;
		}

	HAL_Delay(50); // Wait after hardware reset

	status = mpu6500_write_and_verify(sensor, MPU6500_PWR_MGMT_1, 0x01);
	if (status != HAL_OK){
		return status;
	}

	// Set config values to:
	// CONFIG = 0x03
	// DLPF_CFG = 3
	// moderate low-pass filtering

	status = mpu6500_write_and_verify(sensor, MPU6500_CONFIG, 0x03);
	if (status != HAL_OK){
		return status;
	}
	status = mpu6500_write_and_verify(sensor, MPU6500_GYRO_CONFIG, 0x00);
	if (status != HAL_OK){
		return status;
	}
	status = mpu6500_write_and_verify(sensor, MPU6500_ACCEL_CONFIG, 0x00);
	if (status != HAL_OK){
		return status;
	}

		return HAL_OK;

}


static HAL_StatusTypeDef mpu6500_read_raw_data(mpu6500_t *sensor){
	HAL_StatusTypeDef status = mpu6500_read_registers(sensor, MPU6500_ACCEL_XOUT_H, 14);
	if (status != HAL_OK){
		return status;
	}

	sensor->raw.accel_x = (int16_t)((((uint16_t)sensor->readbuffer[0]) << 8) | sensor->readbuffer[1]);
	sensor->raw.accel_y = (int16_t)((((uint16_t)sensor->readbuffer[2]) << 8) | sensor->readbuffer[3]);
	sensor->raw.accel_z = (int16_t)((((uint16_t)sensor->readbuffer[4]) << 8) | sensor->readbuffer[5]);

	sensor->raw.temperature = (int16_t)((((uint16_t)sensor->readbuffer[6]) << 8) | sensor->readbuffer[7]);

	sensor->raw.gyro_x = (int16_t)((((uint16_t)sensor->readbuffer[8]) << 8) | sensor->readbuffer[9]);
	sensor->raw.gyro_y = (int16_t)((((uint16_t)sensor->readbuffer[10]) << 8) | sensor->readbuffer[11]);
	sensor->raw.gyro_z = (int16_t)((((uint16_t)sensor->readbuffer[12]) << 8) | sensor->readbuffer[13]);


	return HAL_OK;

}


static void mpu6500_process_raw_data(mpu6500_t *sensor){
	sensor->data.accel_x_g = sensor->raw.accel_x / 16384.0f;
	sensor->data.accel_y_g = sensor->raw.accel_y / 16384.0f;
	sensor->data.accel_z_g = sensor->raw.accel_z / 16384.0f;

	sensor->data.temperature_c = sensor->raw.temperature / 333.87 + 21;

	float unbiased_gyro_x = (sensor->raw.gyro_x - sensor->calib.gyro_x_bias_raw);
	float unbiased_gyro_y = (sensor->raw.gyro_y - sensor->calib.gyro_y_bias_raw);
	float unbiased_gyro_z = (sensor->raw.gyro_z - sensor->calib.gyro_z_bias_raw);

	sensor->data.gyro_x_dps = unbiased_gyro_x / 131.0f;
	sensor->data.gyro_y_dps = unbiased_gyro_y / 131.0f;
	sensor->data.gyro_z_dps = unbiased_gyro_z / 131.0f;

}


HAL_StatusTypeDef mpu6500_read_data(mpu6500_t *sensor){
	HAL_StatusTypeDef status = HAL_OK;

	status = mpu6500_read_raw_data(sensor);
	if (status != HAL_OK){
		return status;
	}

	mpu6500_process_raw_data(sensor);

	return HAL_OK;

}


HAL_StatusTypeDef mpu6050_calibrate_gyro(mpu6500_t *sensor, uint16_t sample_count, uint8_t sample_delay){
	HAL_StatusTypeDef status = HAL_OK;

	int64_t acc_x = 0;
	int64_t acc_y = 0;
	int64_t acc_z = 0;

	for (uint16_t i = 0; i < sample_count; ++i){
		status = mpu6500_read_raw_data(sensor);
		if (status != HAL_OK){
			return status;
		}

		acc_x += sensor->raw.gyro_x;
		acc_y += sensor->raw.gyro_y;
		acc_z += sensor->raw.gyro_z;

		HAL_Delay(sample_delay);

	}

	sensor->calib.gyro_x_bias_raw = (acc_x / sample_count);
	sensor->calib.gyro_y_bias_raw = (acc_y / sample_count);
	sensor->calib.gyro_z_bias_raw = (acc_z / sample_count);

	return HAL_OK;

}







