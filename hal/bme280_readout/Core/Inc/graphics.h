/*
 * graphics.h
 *
 *  Created on: Apr 24, 2026
 *      Author: pusana
 */

#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_
#include "ssd1306.h"
#include <stdint.h>
#include <stdbool.h>

void bouncing_pixel(ssd1306_spi_t *display, uint8_t x, uint8_t y, uint8_t delay, uint32_t animation_time);

void draw_hline(ssd1306_spi_t *display,uint8_t x, uint8_t y, uint8_t length);

void draw_vline(ssd1306_spi_t *display,uint8_t x, uint8_t y, uint8_t length);

void draw_rect(ssd1306_spi_t *display,uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void draw_rect_filled(ssd1306_spi_t *display, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void draw_char(ssd1306_spi_t *display,uint8_t x, uint8_t y, char letter);

void draw_string(ssd1306_spi_t *display, uint8_t x, uint8_t y, const char *str);

#endif /* INC_GRAPHICS_H_ */
