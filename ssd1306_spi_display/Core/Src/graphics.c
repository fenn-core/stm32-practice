/*
 * graphics.c
 *
 *  Created on: Apr 24, 2026
 *      Author: pusana
 */

#include "main.h"
#include "ssd1306.h"
#include <stdint.h>
#include <stdbool.h>

void bouncing_pixel(uint8_t x, uint8_t y, uint8_t delay, uint32_t animation_time){
	 uint32_t start_time = HAL_GetTick();
	 uint32_t last_update_time = start_time;
	 bool x_direction = true;
	 bool y_direction = true;
	 while ((HAL_GetTick() - start_time) < animation_time){
		 uint32_t current_time = HAL_GetTick();
	     if (current_time - last_update_time >= delay){
	         last_update_time = current_time;

			 ssd1306_clear_buffer();
			 draw_pixel(x, y, true);
			 ssd1306_update();

			 if ((x == 0 && !x_direction) ||(x == 127 && x_direction)){
				 x_direction = ! x_direction;
			 }
			 if ((y == 0 && !y_direction) || (y == 63 && y_direction)){
				 y_direction = ! y_direction;
			 }
			 if (x_direction){
				 x++;
			 }
			 else{
				 x--;
			 }
			 if (y_direction){
				 y++;
			 }
			 else{
				 y--;
			 }
		 }
	 }
	 ssd1306_clear_buffer();
}

void draw_hline(uint8_t x, uint8_t y, uint8_t length){
	 for(uint8_t i = 0; i < length; i++) {
	  	 draw_pixel(x+i, y, true);
	 }
 }

void draw_vline(uint8_t x, uint8_t y, uint8_t length){
	 for(uint8_t i = 0; i < length; i++) {
	  	draw_pixel(x, y+i, true);
	 }
 }

 void draw_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
     draw_vline(x1, y1, y2 - y1 + 1);
     draw_vline(x2, y1, y2 - y1 + 1);
     draw_hline(x1, y1, x2 - x1 + 1);
     draw_hline(x1, y2, x2 - x1 + 1);
 }
