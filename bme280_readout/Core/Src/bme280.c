/*
 * bme280.c
 *
 *  Created on: Apr 27, 2026
 *      Author: pusana
 */

#include "main.h"
#include <stdint.h>

#define BME280_ADDR (0x76 << 1)
#define BME280_REG_CHIP_ID 0xD0

// Chip Registers
#define CTRL_HUM  0xF2
#define CTRL_MEAS 0xF4
#define CONFIG 	  0xF5

extern I2C_HandleTypeDef hi2c1;

typedef struct {


};


HAL_StatusTypeDef bme280_read_register(uint8_t reg, uint8_t *value){
	return HAL_I2C_Mem_Read(&hi2c1, BME280_ADDR, reg, 1, value, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef bme280_write_register(uint8_t reg, uint8_t value){
	return HAL_I2C_Mem_Write(&hi2c1, BME280_ADDR, reg, 1, &value, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef bme280_read_chip_id(uint8_t *value){
	return bme280_read_register(BME280_REG_CHIP_ID, value);
}

void bme280_init(void){
	bme280_write_register(CTRL_HUM, 0x01);  // humidity oversampling x1
	bme280_write_register(CTRL_MEAS, 0x27); // temp x1, pressure x1, normal mode
	bme280_write_register(CONFIG, 0x00);    // filter off, short standby/default-ish
}

void read_registers(uint8_t start_reg, uint8_t *buffer, uint8_t length){
	HAL_I2C_Mem_Read(&hi2c1, BME280_ADDR, start_reg, 1, buffer, length, HAL_MAX_DELAY);
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
