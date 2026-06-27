/*
 * i2c_recovery.c
 *
 *  Created on: Jun 27, 2026
 *      Author: pusana
 */


#include "stm32f4xx_hal.h"
#include "i2c_recovery.h"


HAL_StatusTypeDef i2c_bus_recover(
    I2C_HandleTypeDef *hi2c,
    GPIO_TypeDef *scl_port,
    uint16_t scl_pin,
    GPIO_TypeDef *sda_port,
    uint16_t sda_pin){

	HAL_StatusTypeDef status = HAL_I2C_DeInit(hi2c);
	if (status != HAL_OK){
		return status;
	}

	if (hi2c->Instance == I2C1) {
	    __HAL_RCC_I2C1_FORCE_RESET();
	    __NOP();
	    __HAL_RCC_I2C1_RELEASE_RESET();
	}
	else if (hi2c->Instance == I2C2) {
	    __HAL_RCC_I2C2_FORCE_RESET();
	    __NOP();
	    __HAL_RCC_I2C2_RELEASE_RESET();
	}
	else if (hi2c->Instance == I2C3) {
	    __HAL_RCC_I2C3_FORCE_RESET();
	    __NOP();
	    __HAL_RCC_I2C3_RELEASE_RESET();
	}
	else {
	    return HAL_ERROR;
	}

	GPIO_InitTypeDef gpio = {0};

	gpio.Mode  = GPIO_MODE_OUTPUT_OD;
	gpio.Pull  = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;

	gpio.Pin = scl_pin;
	HAL_GPIO_Init(scl_port, &gpio);

	gpio.Pin = sda_pin;
	HAL_GPIO_Init(sda_port, &gpio);


	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_SET);

	HAL_Delay(1);

	if (HAL_GPIO_ReadPin(scl_port, scl_pin) != GPIO_PIN_SET) {
	    return HAL_ERROR;
	}

	if (HAL_GPIO_ReadPin(sda_port, sda_pin) != GPIO_PIN_SET) {
	    for (uint8_t pulse = 0; pulse < 9U; ++pulse) {
	        HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET);
	        HAL_Delay(1);

	        HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);
	        HAL_Delay(1);

	        if (HAL_GPIO_ReadPin(scl_port, scl_pin) != GPIO_PIN_SET) {
	            return HAL_ERROR;
	        }

	        if (HAL_GPIO_ReadPin(sda_port, sda_pin) == GPIO_PIN_SET) {
	            break;
	        }
	    }

	    if (HAL_GPIO_ReadPin(sda_port, sda_pin) != GPIO_PIN_SET) {
	        return HAL_ERROR;
	    }
	}

	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_RESET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(scl_port, scl_pin, GPIO_PIN_SET);
	HAL_Delay(1);

	if (HAL_GPIO_ReadPin(scl_port, scl_pin) != GPIO_PIN_SET) {
	    return HAL_ERROR;
	}

	HAL_GPIO_WritePin(sda_port, sda_pin, GPIO_PIN_SET);
	HAL_Delay(1);

	if (HAL_GPIO_ReadPin(scl_port, scl_pin) != GPIO_PIN_SET ||
	    HAL_GPIO_ReadPin(sda_port, sda_pin) != GPIO_PIN_SET) {
	    return HAL_ERROR;
	}

	status = HAL_I2C_Init(hi2c);
	if (status != HAL_OK) {
	    return status;
	}

	return HAL_OK;


}
