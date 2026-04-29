/*
 * bme280.c
 *
 *  Created on: Apr 27, 2026
 *      Author: pusana
 */

#include "main.h"
#include <stdint.h>

#define BME280_REG_CHIP_ID 0xD0

// Chip Registers
#define CTRL_HUM  0xF2
#define CTRL_MEAS 0xF4
#define CONFIG 	  0xF5

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
	uint8_t calib_buffer[18];
	int32_t t_fine = 0;
}bme280_t;


HAL_StatusTypeDef bme280_read_register(bme280_t sensor, uint8_t reg, uint8_t *addr){
	return HAL_I2C_Mem_Read(sensor.setup_vals.handle, sensor.setup_vals.bme280_addr, reg, 1, addr, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef bme280_write_register(bme280_t sensor, uint8_t reg, uint8_t value){
	return HAL_I2C_Mem_Write(sensor.setup_vals.handle, sensor.setup_vals.bme280_addr, reg, 1, &value, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef bme280_read_chip_id(uint8_t *value){
	return bme280_read_register(BME280_REG_CHIP_ID, value);
}

void bme280_read_registers(bme280_t sensor, uint8_t start_reg, uint8_t *buffer, uint8_t length){
	HAL_I2C_Mem_Read(sensor.setup_vals.handle, sensor.setup_vals.bme280_addr, start_reg, 1, buffer, length, HAL_MAX_DELAY);
}

uint32_t process_raw_data_20bit(const uint8_t *raw_data){
	uint32_t msb  =  raw_data[0];
	uint32_t lsb  =  raw_data[1];
	uint32_t xlsb = (raw_data[2] >> 4);

	return ((msb << 12)| (lsb << 4) | xlsb);
}

uint16_t process_raw_data_16bit(const uint8_t *raw_data){
	uint16_t msb = raw_data[0];
	uint16_t lsb = raw_data[1];

	return ((msb << 8)| lsb);
}

void bme280_read_calibration_data(bme280_t sensor){
	bme280_calib_t calib_vals = sensor.calib_vals;
	uint8_t calib_buffer = sensor.calib_buffer;

	bme280_read_registers(0x88, calib_buffer, 2);
	calib_vals.dig_T1 = (((uint16_t)calib_buffer[1] << 8) | (uint16_t)calib_buffer[0]);

	bme280_read_registers(0x8A, calib_buffer, 2);
	calib_vals.dig_T2 = (int16_t)(((uint16_t)calib_buffer[1] << 8) | (uint16_t)calib_buffer[0]);

	bme280_read_registers(0x8C, calib_buffer, 2);
	calib_vals.dig_T3 = (int16_t)(((uint16_t)calib_buffer[1] << 8) | (uint16_t)calib_buffer[0]);


	bme280_read_registers(0x8E, calib_buffer, 18);
	calib_vals.dig_P1 = (((uint16_t)calib_buffer[1] << 8) | (uint16_t)calib_buffer[0]);

	calib_vals.dig_P2 = (int16_t)(((uint16_t)calib_buffer[3] << 8) | (uint16_t)calib_buffer[2]);

	calib_vals.dig_P3 = (int16_t)(((uint16_t)calib_buffer[5] << 8) | (uint16_t)calib_buffer[4]);

	calib_vals.dig_P4 = (int16_t)(((uint16_t)calib_buffer[7] << 8) | (uint16_t)calib_buffer[6]);

	calib_vals.dig_P5 = (int16_t)(((uint16_t)calib_buffer[9] << 8) | (uint16_t)calib_buffer[8]);

	calib_vals.dig_P6 = (int16_t)(((uint16_t)calib_buffer[11] << 8) | (uint16_t)calib_buffer[10]);

	calib_vals.dig_P7 = (int16_t)(((uint16_t)calib_buffer[13] << 8) | (uint16_t)calib_buffer[12]);

	calib_vals.dig_P8 = (int16_t)(((uint16_t)calib_buffer[15] << 8) | (uint16_t)calib_buffer[14]);

	calib_vals.dig_P9 = (int16_t)(((uint16_t)calib_buffer[17] << 8) | (uint16_t)calib_buffer[16]);


	bme280_read_register(0xA1, &calib_vals.dig_H1);

	bme280_read_registers(0xE1, calib_buffer, 2);
	calib_vals.dig_H2 = (int16_t)(((uint16_t)calib_buffer[1] << 8) | (uint16_t)calib_buffer[0]);

	bme280_read_register(0xE3, &calib_vals.dig_H3);

	bme280_read_registers(0xE4, calib_buffer, 3);
	calib_vals.dig_H4 = (int16_t)(((uint16_t)calib_buffer[0] << 4) | ((uint16_t)calib_buffer[1] & 0x0F));
	calib_vals.dig_H5 = (int16_t)(((uint16_t)calib_buffer[2] << 4) | ((uint16_t)calib_buffer[1] >> 4));

	bme280_read_register(0xE7, calib_buffer);
	calib_vals.dig_H6 = (int8_t)(calib_buffer[0]);
}

HAL_StatusTypeDef bme280_init(I2C_HandleTypeDef *passed_handle, uint8_t passed_address){
	setup_vals.handle = passed_handle;
	setup_vals.bme280_addr = (passed_address << 1);
	bme280_read_register(BME280_REG_CHIP_ID, &setup_vals.chip_id);
	if (setup_vals.chip_id != 0x60){
		return HAL_ERROR;
	}
	bme280_write_register(CTRL_HUM, 0x01);  // humidity oversampling x1
	bme280_write_register(CTRL_MEAS, 0x27); // temp x1, pressure x1, normal mode
	bme280_write_register(CONFIG, 0x00);    // filter off, short standby/default-ish
	bme280_read_calibration_data();
	return HAL_OK;
}

void bme280_compute_values(bme280_t sensor, uint8_t raw_data[8]){
	uint32_t adc_P = process_raw_data_20bit(&raw_data[0]);
	uint32_t adc_T = process_raw_data_20bit(&raw_data[3]);
	uint16_t adc_H = process_raw_data_16bit(&raw_data[6]);


	int32_t var1_T, var2_T;

	var1_T = (((((adc_T >> 3) - (calib_vals.dig_T1 << 1))) * calib_vals.dig_T2) >> 11);

	var2_T = ((((((adc_T >> 4) - calib_vals.dig_T1) *
	          ((adc_T >> 4) - calib_vals.dig_T1)) >> 12) *
			    calib_vals.dig_T3) >> 14);

	t_fine = var1_T + var2_T;

	int32_t temperature = ((t_fine * 5 + 128) >> 8);

	sensor.bme280_data.temperature_centi_c = temperature;


	int64_t var1_P, var2_P, p;

	var1_P = (int64_t)t_fine - 128000;

	if (var1_P == 0){
		return;
	}
	var2_P = var1_P * var1_P * calib_vals.dig_P6;
	var2_P = var2_P + ((var1_P * calib_vals.dig_P5) << 17);
	var2_P = var2_P + ((int64_t)calib_vals.dig_P4 << 35);

	var1_P = ((var1_P * var1_P * calib_vals.dig_P3) >> 8) + ((var1_P * calib_vals.dig_P2) << 12);
	var1_P = (((((int64_t)1 << 47) + var1_P)) * calib_vals.dig_P1) >> 33;

	p = 1048576 - adc_P;
	p = (((p << 31) - var2_P) * 3125) / var1_P;

	var1_P = ((int64_t)calib_vals.dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2_P = ((int64_t)calib_vals.dig_P8 * p) >> 19;

	p = ((p + var1_P + var2_P) >> 8) + (calib_vals.dig_P7 << 4);

	sensor.bme280_data.pressure_hpa = ((p >> 8)/100);


	 int32_t v_x1 = t_fine - 76800;

	v_x1 = (((((adc_H << 14) - (calib_vals.dig_H4 << 20) - (calib_vals.dig_H5 * v_x1)) + 16384) >> 15)
	        * (((((((v_x1 * calib_vals.dig_H6) >> 10) * (((v_x1 * calib_vals.dig_H3) >> 11) + 32768)) >> 10) + 2097152)
	        * calib_vals.dig_H2 + 8192) >> 14));

	v_x1 = v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * calib_vals.dig_H1) >> 4);

	if (v_x1 < 0){
	    v_x1 = 0;
	}
	if (v_x1 > 419430400){
	    v_x1 = 419430400;
	}

	uint32_t humidity = (v_x1 >> 12);
	sensor.bme280_data.humidity_percent = (humidity / 1024);
}


void bme280_read_values(sensor){
	uint8_t raw_data[8];
	bme280_read_registers(0xF7, raw_data, 8);
	bme280_compute_values(sensor, raw_data);
}



