/*
 * bme280.h
 *
 *  Created on: Apr 27, 2026
 *      Author: pusana
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

HAL_StatusTypeDef bme280_read_chip_id(uint8_t *value);

HAL_StatusTypeDef bme280_init(I2C_HandleTypeDef *passed_handle, uint8_t passed_address);

void bme280_read_values(void);

#endif /* INC_BME280_H_ */
