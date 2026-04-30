/*
 * bme280.c
 *
 *  Created on: Apr 27, 2026
 *      Author: pusana
 */

#include "main.h"
#include "bme280.h"
#include <stdint.h>

#define BME280_REG_CHIP_ID 0xD0

// Chip Registers
#define CTRL_HUM  0xF2
#define CTRL_MEAS 0xF4
#define CONFIG 	  0xF5


HAL_StatusTypeDef bme280_read_register(bme280_t *sensor, uint8_t reg, uint8_t *addr){
	return HAL_I2C_Mem_Read(sensor->setup_vals.handle, sensor->setup_vals.bme280_addr, reg, 1, addr, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef bme280_write_register(bme280_t *sensor, uint8_t reg, uint8_t value){
	return HAL_I2C_Mem_Write(sensor->setup_vals.handle, sensor->setup_vals.bme280_addr, reg, 1, &value, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef bme280_read_chip_id(bme280_t *sensor, uint8_t *value){
	return bme280_read_register(sensor, BME280_REG_CHIP_ID, value);
}

HAL_StatusTypeDef bme280_read_registers(bme280_t *sensor, uint8_t start_reg, uint8_t length){
	return HAL_I2C_Mem_Read(sensor->setup_vals.handle, sensor->setup_vals.bme280_addr, start_reg, 1, sensor->read_buffer, length, HAL_MAX_DELAY);
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

void bme280_read_calibration_data(bme280_t *sensor){
	bme280_calib_t *cal = &sensor->calib_vals;


	bme280_read_registers(sensor, 0x88, 2);
	cal->dig_T1 = (((uint16_t)sensor->read_buffer[1] << 8) | (uint16_t)sensor->read_buffer[0]);

	bme280_read_registers(sensor, 0x8A, 2);
	cal->dig_T2 = (int16_t)(((uint16_t)sensor->read_buffer[1] << 8) | (uint16_t)sensor->read_buffer[0]);

	bme280_read_registers(sensor, 0x8C, 2);
	cal->dig_T3 = (int16_t)(((uint16_t)sensor->read_buffer[1] << 8) | (uint16_t)sensor->read_buffer[0]);


	bme280_read_registers(sensor, 0x8E, 18);
	cal->dig_P1 = (((uint16_t)sensor->read_buffer[1] << 8) | (uint16_t)sensor->read_buffer[0]);

	cal->dig_P2 = (int16_t)(((uint16_t)sensor->read_buffer[3] << 8) | (uint16_t)sensor->read_buffer[2]);

	cal->dig_P3 = (int16_t)(((uint16_t)sensor->read_buffer[5] << 8) | (uint16_t)sensor->read_buffer[4]);

	cal->dig_P4 = (int16_t)(((uint16_t)sensor->read_buffer[7] << 8) | (uint16_t)sensor->read_buffer[6]);

	cal->dig_P5 = (int16_t)(((uint16_t)sensor->read_buffer[9] << 8) | (uint16_t)sensor->read_buffer[8]);

	cal->dig_P6 = (int16_t)(((uint16_t)sensor->read_buffer[11] << 8) | (uint16_t)sensor->read_buffer[10]);

	cal->dig_P7 = (int16_t)(((uint16_t)sensor->read_buffer[13] << 8) | (uint16_t)sensor->read_buffer[12]);

	cal->dig_P8 = (int16_t)(((uint16_t)sensor->read_buffer[15] << 8) | (uint16_t)sensor->read_buffer[14]);

	cal->dig_P9 = (int16_t)(((uint16_t)sensor->read_buffer[17] << 8) | (uint16_t)sensor->read_buffer[16]);


	bme280_read_register(sensor, 0xA1, &cal->dig_H1);

	bme280_read_registers(sensor, 0xE1, 2);
	cal->dig_H2 = (int16_t)(((uint16_t)sensor->read_buffer[1] << 8) | (uint16_t)sensor->read_buffer[0]);

	bme280_read_register(sensor, 0xE3, &cal->dig_H3);

	bme280_read_registers(sensor, 0xE4, 3);
	cal->dig_H4 = (int16_t)(((uint16_t)sensor->read_buffer[0] << 4) | ((uint16_t)sensor->read_buffer[1] & 0x0F));
	cal->dig_H5 = (int16_t)(((uint16_t)sensor->read_buffer[2] << 4) | ((uint16_t)sensor->read_buffer[1] >> 4));

	bme280_read_register(sensor, 0xE7, sensor->read_buffer);
	cal->dig_H6 = (int8_t)(sensor->read_buffer[0]);
}

HAL_StatusTypeDef bme280_init(bme280_t *sensor, I2C_HandleTypeDef *passed_handle, uint8_t passed_address){
	bme280_setup_t *setup = &sensor->setup_vals;
	setup->handle = passed_handle;
	setup->bme280_addr = (passed_address << 1);


	if (bme280_read_register(sensor, BME280_REG_CHIP_ID, &setup->chip_id) != HAL_OK){
		return HAL_ERROR;
	}
	if (setup->chip_id != 0x60){
		return HAL_ERROR;
	}
	bme280_write_register(sensor, CTRL_HUM, 0x01);  // humidity oversampling x1
	bme280_write_register(sensor, CTRL_MEAS, 0x27); // temp x1, pressure x1, normal mode
	bme280_write_register(sensor, CONFIG, 0x00);    // filter off, short standby/default-ish
	bme280_read_calibration_data(sensor);
	return HAL_OK;
}

void bme280_compute_values(bme280_t *sensor){
	bme280_calib_t *cal = &sensor->calib_vals;
	bme280_data_t *out = &sensor->bme280_data;
	sensor->t_fine = 0;


	uint32_t adc_P = process_raw_data_20bit(sensor->read_buffer);
	uint32_t adc_T = process_raw_data_20bit(sensor->read_buffer + 3);
	uint16_t adc_H = process_raw_data_16bit(sensor->read_buffer + 6);


	int32_t var1_T, var2_T;

	var1_T = (((((adc_T >> 3) - (cal->dig_T1 << 1))) * cal->dig_T2) >> 11);

	var2_T = ((((((adc_T >> 4) - cal->dig_T1) *
	          ((adc_T >> 4) - cal->dig_T1)) >> 12) *
			    cal->dig_T3) >> 14);

	sensor->t_fine = var1_T + var2_T;

	int32_t temperature = ((sensor->t_fine * 5 + 128) >> 8);

	out->temperature_centi_c = temperature;


	int64_t var1_P, var2_P, p;

	var1_P = (int64_t)sensor->t_fine - 128000;

	if (var1_P == 0){
		return;
	}
	var2_P = var1_P * var1_P * cal->dig_P6;
	var2_P = var2_P + ((var1_P * cal->dig_P5) << 17);
	var2_P = var2_P + ((int64_t)cal->dig_P4 << 35);

	var1_P = ((var1_P * var1_P * cal->dig_P3) >> 8) + ((var1_P * cal->dig_P2) << 12);
	var1_P = (((((int64_t)1 << 47) + var1_P)) * cal->dig_P1) >> 33;

	p = 1048576 - adc_P;
	p = (((p << 31) - var2_P) * 3125) / var1_P;

	var1_P = ((int64_t)cal->dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2_P = ((int64_t)cal->dig_P8 * p) >> 19;

	p = ((p + var1_P + var2_P) >> 8) + (cal->dig_P7 << 4);

	out->pressure_hpa = ((p >> 8)/100);


	 int32_t v_x1 = sensor->t_fine - 76800;

	v_x1 = (((((adc_H << 14) - (cal->dig_H4 << 20) - (cal->dig_H5 * v_x1)) + 16384) >> 15)
	        * (((((((v_x1 * cal->dig_H6) >> 10) * (((v_x1 * cal->dig_H3) >> 11) + 32768)) >> 10) + 2097152)
	        * cal->dig_H2 + 8192) >> 14));

	v_x1 = v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * cal->dig_H1) >> 4);

	if (v_x1 < 0){
	    v_x1 = 0;
	}
	if (v_x1 > 419430400){
	    v_x1 = 419430400;
	}

	uint32_t humidity = (v_x1 >> 12);
	out->humidity_percent = (humidity / 1024);
}


HAL_StatusTypeDef bme280_read_values(bme280_t *sensor){
	if (bme280_read_registers(sensor, 0xF7, 8) != HAL_OK){
		return HAL_ERROR;
	}
	bme280_compute_values(sensor);

	return HAL_OK;
}






