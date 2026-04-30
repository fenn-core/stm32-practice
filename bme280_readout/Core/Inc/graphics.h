/*
 * graphics.h
 *
 *  Created on: Apr 24, 2026
 *      Author: pusana
 */

#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_
#include <stdint.h>
#include <stdbool.h>

void bouncing_pixel(uint8_t x, uint8_t y, uint8_t delay, uint32_t animation_time);

void draw_hline(uint8_t x, uint8_t y, uint8_t length);

void draw_vline(uint8_t x, uint8_t y, uint8_t length);

void draw_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void draw_rect_filled(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void draw_char(uint8_t x, uint8_t y, char letter);

void draw_string(uint8_t x, uint8_t y, char *str);

#endif /* INC_GRAPHICS_H_ */
