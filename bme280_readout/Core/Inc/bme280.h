/*
 * bme280.h
 *
 *  Created on: Apr 27, 2026
 *      Author: pusana
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

HAL_StatusTypeDef bme280_read_register(uint8_t reg, uint8_t *value);

HAL_StatusTypeDef bme280_write_register(uint8_t reg, uint8_t value);

HAL_StatusTypeDef bme280_read_chip_id(uint8_t *value);

void bme280_init(void);

void read_registers(uint8_t start_reg, uint8_t *buffer, uint8_t length);

uint32_t process_raw_data_20bit(const uint8_t *raw_data);

uint16_t process_raw_data_16bit(const uint8_t *raw_data);

#endif /* INC_BME280_H_ */
