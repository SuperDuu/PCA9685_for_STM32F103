/*
 * pca9685.h
 *
 *  Created on: Oct 13, 2024
 *      Author: Vu Duc Du
 */

#ifndef PCA9685_H_
#define PCA9685_H_

#include "stm32f1xx_hal.h"

#define PCA9685_I2C_ADDRESS_1 0x80  //0x40 << 1
//#define PCA9685_I2C_ADDRESS_2 0xFE  //0x7F << 1
#define FREQUENCY_OSCILLATOR 25000000

#define MODE1_RESTART 0x80
#define MODE1_SLEEP 0x10
#define MODE1_AI 0x20
#define MODE1_EXTCLK 0x40
#define MODE2_OUTDRV 0x04
#define PRESCALE_MIN 3
#define PRESCALE_MAX 255

void PCA9685_Init(I2C_HandleTypeDef *hi2c, uint8_t addr);
void PCA9685_SetPWMFreq(I2C_HandleTypeDef *hi2c, uint8_t addr, float freq);
void PCA9685_SetPWM(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t num, uint16_t on, uint16_t off);
void PCA9685_Sleep(I2C_HandleTypeDef *hi2c, uint8_t addr);
void PCA9685_Wakeup(I2C_HandleTypeDef *hi2c, uint8_t addr);
void PCA9685_Reset(I2C_HandleTypeDef *hi2c, uint8_t addr);
void PCA9685_SetPin(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t num, uint16_t val, uint8_t invert);

#endif
