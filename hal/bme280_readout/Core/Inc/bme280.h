/*
 * bme280.h
 *
 *  Created on: Apr 27, 2026
 *      Author: pusana
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#define BME280_BUFFER_SIZE 18

typedef struct{
	  uint16_t dig_T1;
	  int16_t dig_T2;
	  int16_t dig_T3;

	  uint16_t dig_P1;
	  int16_t dig_P2;
	  int16_t dig_P3;
	  int16_t dig_P4;
	  int16_t dig_P5;
	  int16_t dig_P6;
	  int16_t dig_P7;
	  int16_t dig_P8;
	  int16_t dig_P9;

	  uint8_t dig_H1;
	  int16_t dig_H2;
	  uint8_t dig_H3;
	  int16_t dig_H4;
	  int16_t dig_H5;
	  int8_t dig_H6;
}bme280_calib_t;

typedef struct{
	int32_t temperature_centi_c;
	uint32_t pressure_hpa;
	uint8_t humidity_percent;
}bme280_data_t;

typedef struct{
	uint8_t bme280_addr;
	uint8_t chip_id;
	I2C_HandleTypeDef *handle;
}bme280_setup_t;

typedef struct{
	bme280_setup_t setup_vals;
	bme280_calib_t calib_vals;
	bme280_data_t bme280_data;
	uint8_t read_buffer[BME280_BUFFER_SIZE];
	int32_t t_fine;
}bme280_t;


HAL_StatusTypeDef bme280_read_chip_id(bme280_t *sensor, uint8_t *value);

HAL_StatusTypeDef bme280_init(bme280_t *sensor, I2C_HandleTypeDef *passed_handle, uint8_t passed_address);

HAL_StatusTypeDef bme280_read_values(bme280_t *sensor);

#endif /* INC_BME280_H_ */
