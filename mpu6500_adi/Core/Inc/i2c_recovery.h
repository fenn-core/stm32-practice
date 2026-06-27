/*
 * i2c_recovery.h
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */

#ifndef INC_I2C_RECOVERY_H_
#define INC_I2C_RECOVERY_H_

#define MPU6500_I2C_TIMEOUT_MS  20U


HAL_StatusTypeDef i2c_bus_recover(
    I2C_HandleTypeDef *hi2c,
    GPIO_TypeDef *scl_port,
    uint16_t scl_pin,
    GPIO_TypeDef *sda_port,
    uint16_t sda_pin
);



#endif /* INC_I2C_RECOVERY_H_ */
