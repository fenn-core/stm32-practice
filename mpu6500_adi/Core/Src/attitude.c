/*
 * attitude.c
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */


#include <stdint.h>
#include <math.h>
#include "attitude.h"

#define RAD2DEG(x) (x * 57.2957795f)


void attitude_compute_pitch_roll(attitude_t *attitude, const vec3f_t accel_vals){
	float values[3] = {
	accel_vals.x,
	accel_vals.y,
	accel_vals.z};

	float accel_x = (values[attitude->orientation.body_x] * attitude->orientation.sign_x);
	float accel_y = (values[attitude->orientation.body_y] * attitude->orientation.sign_y);
	float accel_z = (values[attitude->orientation.body_z] * attitude->orientation.sign_z);


	float roll = RAD2DEG(atan2f(accel_y, accel_z));

	float pitch = RAD2DEG(atan2f(accel_x,
			sqrtf(accel_y * accel_y + accel_z * accel_z)
			));

	attitude->roll_deg = roll;
	attitude->pitch_deg = pitch;

}
