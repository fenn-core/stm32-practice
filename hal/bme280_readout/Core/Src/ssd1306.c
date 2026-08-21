/*
 * ssd1306.c
 *
 *  Created on: Apr 21, 2026
 *      Author: pusana
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "ssd1306.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


static void hardware_reset(ssd1306_spi_t *display){
	HAL_GPIO_WritePin(display->rst_port, display->rst_pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(display->rst_port, display->rst_pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

static void send_spi_command(ssd1306_spi_t *display, uint8_t cmd){
	HAL_GPIO_WritePin(display->cs_port, display->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(display->dc_port, display->dc_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(display->handle, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(display->cs_port, display->cs_pin, GPIO_PIN_SET);
}

static void send_spi_data(ssd1306_spi_t *display, uint8_t data){
	HAL_GPIO_WritePin(display->cs_port, display->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(display->dc_port, display->dc_pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(display->handle, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(display->cs_port, display->cs_pin, GPIO_PIN_SET);
}

static void send_spi_framebuffer(ssd1306_spi_t *display, uint8_t *data, uint32_t length){
	HAL_GPIO_WritePin(display->cs_port, display->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(display->dc_port, display->dc_pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(display->handle, data, length, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(display->cs_port, display->cs_pin, GPIO_PIN_SET);
}

static void ssd1306_set_full_window(ssd1306_spi_t *display){
    // Set column address
    send_spi_command(display, 0x21); // command
    send_spi_command(display, 0x00); // start column
    send_spi_command(display, 0x7F); // end column (127)

    // Set page address
    send_spi_command(display, 0x22); // command
    send_spi_command(display, 0x00); // start page
    send_spi_command(display, 0x07); // end page (7)
}

void ssd1306_set_full_buffer(ssd1306_spi_t *display){
	memset(display->framebuffer, 0xFF, SSD1306_BUFFER_SIZE);
}

void ssd1306_clear_buffer(ssd1306_spi_t *display){
	memset(display->framebuffer, 0x00, SSD1306_BUFFER_SIZE);
}

void ssd1306_init(ssd1306_spi_t *display, ssd1306_spi_config_t *config){
	display->handle = config->handle;
	display->cs_pin = config->cs_pin;
	display->cs_port = config->cs_port;
	display->dc_pin = config->dc_pin;
	display->dc_port = config->dc_port;
	display->rst_pin = config->rst_pin;
	display->rst_port = config->rst_port;


	hardware_reset(display);
	send_spi_command(display, 0xAE); // display off

	send_spi_command(display, 0x20); // set memory addressing mode
	send_spi_command(display, 0x00); // to horizontal addressing

	send_spi_command(display, 0xA8); // set multiplex ratio to
	send_spi_command(display, 0x3F); // 64 mux lines

	send_spi_command(display, 0xD3); // set display offset
	send_spi_command(display, 0x00); // to 0

	send_spi_command(display, 0x40); // set display start line to zero

	send_spi_command(display, 0xA1); // set segment remap for left/right

	send_spi_command(display, 0xC8); // COM output scan direction

	send_spi_command(display, 0xDA); // COM pins hardware configuration
	send_spi_command(display, 0x12); // for a typical 128×64 panel

	send_spi_command(display, 0x81); // set contrast
	send_spi_command(display, 0x7F); // to this safe value

	send_spi_command(display, 0xA4); // resume display from RAM content

	send_spi_command(display, 0xA6); // set normal display

	send_spi_command(display, 0xD5); // set display clock divide ratio / oscillator frequency to
	send_spi_command(display, 0x80); // a common default-ish startup value

	send_spi_command(display, 0x8D); // enable the charge pump for the
	send_spi_command(display, 0x14); // common internal-charge-pump style modules

	send_spi_command(display, 0xD9); // set pre-charge period to a
	send_spi_command(display, 0xF1); // common internal-VCC/charge-pump-friendly value

	send_spi_command(display, 0xDB); // set VCOMH deselect level
	send_spi_command(display, 0x40); //  another standard startup value

	send_spi_command(display, 0x2E); // deactivate scroll

	ssd1306_clear_buffer(display); // clear buffer to prevent garbage

	ssd1306_update(display); // update display

	send_spi_command(display, 0xAF); // display on
}

void draw_pixel(ssd1306_spi_t *display, uint8_t x, uint8_t y, bool state){
	if (128 > x && 64 > y){
		uint8_t page = (y / 8);
		uint16_t buffer_index = (page * 128) + x;
		uint8_t bit_position = (y % 8);
		uint8_t mask = 1 << bit_position;
        if (state){
            display->framebuffer[buffer_index] |= mask;
        } else {
        	display->framebuffer[buffer_index] &= ~mask;
		}
	}
}

void ssd1306_update(ssd1306_spi_t *display){
	ssd1306_set_full_window(display);
	send_spi_framebuffer(display, display->framebuffer, SSD1306_BUFFER_SIZE);
}






