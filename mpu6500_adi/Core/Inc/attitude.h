/*
 * attitude.h
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */

#ifndef INC_ATTITUDE_H_
#define INC_ATTITUDE_H_


typedef struct{
	float x;
	float y;
	float z;

}vec3f_t;


typedef enum{
	IMU_AXIS_X = 0,
	IMU_AXIS_Y = 1,
	IMU_AXIS_Z = 2,
	IMU_AXIS_COUNT

}imu_axis_t;


typedef enum{
	IMU_POSITIVE_SIGN = 1,
	IMU_NEGATIVE_SIGN = -1

}imu_sign_t;


typedef struct{
	imu_axis_t body_x;
	imu_axis_t body_y;
	imu_axis_t body_z;

	imu_sign_t sign_x;
	imu_sign_t sign_y;
	imu_sign_t sign_z;

}attitude_orientatiton_t;


typedef struct{
	float roll_deg;
	float pitch_deg;

	attitude_orientatiton_t orientation;

}attitude_t;


void attitude_compute_pitch_roll(attitude_t *attitude, const vec3f_t accel_vals);


#endif /* INC_ATTITUDE_H_ */
