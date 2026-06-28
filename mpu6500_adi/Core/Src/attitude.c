/*
 * attitude.c
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */


#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "attitude.h"

#define RAD2DEG(x) ((x) * (57.2957795f))
#define DEG2RAD(x) ((x) * (0.01745329252f))



void attitude_compute_pitch_roll(attitude_t *attitude,
		const vec3f_t accel_vals,
		const vec3f_t gyro_vals,
		float dt)
	{

	float accel_values[3] = {
	accel_vals.x,
	accel_vals.y,
	accel_vals.z};

	float gyro_values[3] = {
	gyro_vals.x,
	gyro_vals.y,
	gyro_vals.z};


	float accel_x = (accel_values[attitude->orientation.body_x] * attitude->orientation.sign_x);
	float accel_y = (accel_values[attitude->orientation.body_y] * attitude->orientation.sign_y);
	float accel_z = (accel_values[attitude->orientation.body_z] * attitude->orientation.sign_z);


	float accel_roll = RAD2DEG(atan2f(accel_y, accel_z));

	float accel_pitch = RAD2DEG(atan2f(-accel_x,
			sqrtf(accel_y * accel_y + accel_z * accel_z)
			));


	if (!(attitude->initialized)){
		attitude->roll_deg = accel_roll;
		attitude->pitch_deg = accel_pitch;
		attitude->initialized = true;
		return;
	}


	float predicted_roll  = attitude->roll_deg +
			gyro_values[attitude->orientation.body_x] * attitude->orientation.sign_x * dt;

	float roll_rad = DEG2RAD(attitude->roll_deg);

	float pitch_rate = attitude->orientation.body_y * cosf(roll_rad)
		- attitude->orientation.body_z * sinf(roll_rad);

	float predicted_pitch = attitude->pitch_deg + pitch_rate * dt;


	if (predicted_roll > 180){
		predicted_roll -= 360;
	}
	else if (predicted_roll < -180){
		predicted_roll += 360;
	}

	float alpha = 0.5f / (0.5f + dt);

	float fused_roll =
		alpha * predicted_roll
		+ (1.0f - alpha) * accel_roll;

	float fused_pitch =
		alpha * predicted_pitch
		+ (1.0f - alpha) * accel_pitch;


	attitude->roll_deg = fused_roll;
	attitude->pitch_deg = fused_pitch;


}
