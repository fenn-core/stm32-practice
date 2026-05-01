/*
 * ssd1306.h
 *
 *  Created on: Apr 21, 2026
 *      Author: pusana
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_
#define SSD1306_BUFFER_SIZE 1024

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct{
	SPI_HandleTypeDef *handle;

	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;

	GPIO_TypeDef *dc_port;
	uint16_t dc_pin;

	GPIO_TypeDef *rst_port;
	uint16_t rst_pin;
}ssd1306_spi_config_t;

typedef struct{
	SPI_HandleTypeDef *handle;
	uint8_t framebuffer[SSD1306_BUFFER_SIZE];

	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;

	GPIO_TypeDef *dc_port;
	uint16_t dc_pin;

	GPIO_TypeDef *rst_port;
	uint16_t rst_pin;

}ssd1306_spi_t;

void ssd1306_set_full_buffer(ssd1306_spi_t *display);

void ssd1306_clear_buffer(ssd1306_spi_t *display);

void ssd1306_init(ssd1306_spi_t *display, ssd1306_spi_config_t *config);

void draw_pixel(ssd1306_spi_t *display, uint8_t x, uint8_t y, bool state);

void ssd1306_update(ssd1306_spi_t *display);

#endif /* INC_SSD1306_H_ */
