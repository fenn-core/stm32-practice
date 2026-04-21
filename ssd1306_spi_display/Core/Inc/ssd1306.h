/*
 * ssd1306.h
 *
 *  Created on: Apr 21, 2026
 *      Author: pusana
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

void hardware_reset(void);

void send_spi_command(uint8_t cmd);

void send_spi_data(uint8_t data);

void ssd1306_init(void);

void ssd1306_set_full_window(void);

void ssd1306_set_fully_lit(void);

#endif /* INC_SSD1306_H_ */
