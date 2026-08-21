/*
 * adi.c
 *
 *  Created on: Jun 28, 2026
 *      Author: pusana
 */


#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "adi.h"
#include "ssd1306.h"
#include "graphics.h"
#include "attitude.h"

#define DEG2RAD(x) ((x) * (0.01745329252f))


void adi_draw_128x64(adi_t *adi, ssd1306_spi_t *display, attitude_t *attitude, float pixels_per_deg){
	if (!(adi->initialized)){
		adi->display_pitch = attitude->pitch_deg;
		adi->display_roll = attitude->roll_deg;
		adi->initialized = true;
	}
	else{
	    adi->display_pitch += adi->pitch_alpha * (attitude->pitch_deg - adi->display_pitch);
	}

	float roll_error = attitude->roll_deg - adi->display_roll;
	if (roll_error > 180){
		roll_error -= 360;
	}
	else if (roll_error < -180){
		roll_error += 360;
	}
	adi->display_roll += adi->roll_alpha * roll_error;


	int16_t pitch_offset = roundf(adi->display_pitch * pixels_per_deg);

	float dir_x = cosf(DEG2RAD(adi->display_roll));
	float dir_y = sinf(DEG2RAD(adi->display_roll));

	float center_x = 64.0f + pitch_offset * -dir_y;
	float center_y = 32.0f + pitch_offset * dir_x;


	float half_length = 160.0f;  // An overkill value to cover the whole diagonal

	int16_t x1 = lroundf(center_x - dir_x * half_length);
	int16_t y1 = lroundf(center_y - dir_y * half_length);

	int16_t x2 = lroundf(center_x + dir_x * half_length);
	int16_t y2 = lroundf(center_y + dir_y * half_length);


	ssd1306_clear_buffer(display);
	draw_line(display, x1, y1, x2, y2);
	ssd1306_update(display);

}
