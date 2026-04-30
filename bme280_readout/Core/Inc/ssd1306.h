/*
 * ssd1306.h
 *
 *  Created on: Apr 21, 2026
 *      Author: pusana
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_
#include <stdint.h>
#include <stdbool.h>

void ssd1306_set_full_buffer(void);

void ssd1306_clear_buffer(void);

void ssd1306_init(void);

void draw_pixel(uint8_t x, uint8_t y, bool state);

void ssd1306_update(void);

#endif /* INC_SSD1306_H_ */
