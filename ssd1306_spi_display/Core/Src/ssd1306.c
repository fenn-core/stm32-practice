/*
 * ssd1306.c
 *
 *  Created on: Apr 21, 2026
 *      Author: pusana
 */

#include "main.h"


extern SPI_HandleTypeDef hspi1;

void hardware_reset(void){
	HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

void send_spi_command(uint8_t cmd){
	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void send_spi_data(uint8_t data){
	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void ssd1306_init(void){
	hardware_reset();
	send_spi_command(0xAE); // display off

	send_spi_command(0x20); // set memory addressing mode
	send_spi_command(0x00); // to horizontal addressing

	send_spi_command(0xA8); // set multiplex ratio to
	send_spi_command(0x3F); // 64 mux lines

	send_spi_command(0xD3); // set display offset
	send_spi_command(0x00); // to 0

	send_spi_command(0x40); // set display start line to zero

	send_spi_command(0xA1); // set segment remap for left/right

	send_spi_command(0xC8); // COM output scan direction

	send_spi_command(0xDA); // COM pins hardware configuration
	send_spi_command(0x12); // for a typical 128×64 panel

	send_spi_command(0x81); // set contrast
	send_spi_command(0x7F); // to this safe value

	send_spi_command(0xA4); // resume display from RAM content

	send_spi_command(0xA6); // set normal display

	send_spi_command(0xD5); // set display clock divide ratio / oscillator frequency to
	send_spi_command(0x80); // a common default-ish startup value

	send_spi_command(0x8D); // enable the charge pump for the
	send_spi_command(0x8D); // common internal-charge-pump style modules

	send_spi_command(0xD9); // set pre-charge period to a
	send_spi_command(0xF1); // common internal-VCC/charge-pump-friendly value

	send_spi_command(0xDB); // set VCOMH deselect level
	send_spi_command(0x40); //  another standard startup value

	send_spi_command(0x2E); // deactivate scroll

	send_spi_command(0xAF); // display on
}

void ssd1306_set_full_window(void)
{
    // Set column address
    send_spi_command(0x21); // command
    send_spi_command(0x00); // start column
    send_spi_command(0x7F); // end column (127)

    // Set page address
    send_spi_command(0x22); // command
    send_spi_command(0x00); // start page
    send_spi_command(0x07); // end page (7)
}

void ssd1306_set_fully_lit(void){
	ssd1306_set_full_window();
	send_spi_command(0x40); // set display start line to zero

	for (uint16_t i = 0; i < 1024; i++){
		send_spi_data(0xFF);
	}
}


