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

extern I2C_HandleTypeDef hi2c1;


uint8_t bme280_read_chip_id(void){
	uint8_t chip_id = 0;

	if (HAL_I2C_Mem_Read(&hi2c1, BME280_ADDR, BME280_REG_CHIP_ID, 1, &chip_id, 1, HAL_MAX_DELAY) != HAL_OK){
		return 0xFF; // error
	}

	return chip_id;
}
