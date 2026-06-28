/*
 * adi.c
 *
 *  Created on: Jun 28, 2026
 *      Author: pusana
 */

#ifndef INC_ADI_C_
#define INC_ADI_C_
#define ADI_DEFAULT_PIXELS_PER_DEG  1U

#include <stdint.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "attitude.h"


typedef struct{
	float display_pitch;
	float display_roll;

	float pitch_alpha;
	float roll_alpha;

	bool initialized;

}adi_t;


void adi_draw_128x64(adi_t *adi, ssd1306_spi_t *display, attitude_t *attitude, float pixels_per_deg);


#endif /* INC_ADI_C_ */
