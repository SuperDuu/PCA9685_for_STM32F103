/*
 * pca9685.c
 *
 *  Created on: Oct 13, 2024
 *      Author: Vu Duc Du
 */

#include "pca9685.h"
static void PCA9685_Write8(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t data) {
    uint8_t buffer[2] = {reg, data};
    HAL_I2C_Master_Transmit(hi2c, addr, buffer, 2, HAL_MAX_DELAY);
}
static uint8_t PCA9685_Read8(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg) {
    uint8_t result;
    HAL_I2C_Master_Transmit(hi2c, addr, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(hi2c, addr, &result, 1, HAL_MAX_DELAY);
    return result;
}
void PCA9685_Init(I2C_HandleTypeDef *hi2c, uint8_t addr) {
    PCA9685_Reset(hi2c, addr);
    HAL_Delay(10);
}

void PCA9685_Reset(I2C_HandleTypeDef *hi2c, uint8_t addr) {
    PCA9685_Write8(hi2c, addr, 0x00, MODE1_RESTART);
}
void PCA9685_SetPWMFreq(I2C_HandleTypeDef *hi2c, uint8_t addr, float freq) {
    freq *= 0.9;
    float prescaleval = FREQUENCY_OSCILLATOR / (freq * 4096.0) - 1;
    uint8_t prescale = (uint8_t)(prescaleval + 0.5);

    uint8_t oldmode = PCA9685_Read8(hi2c, addr, 0x00);
    uint8_t sleepmode = oldmode | MODE1_SLEEP;
    PCA9685_Write8(hi2c, addr, 0x00, sleepmode);
    PCA9685_Write8(hi2c, addr, 0xFE, prescale);
    PCA9685_Write8(hi2c, addr, 0x00, oldmode);
    PCA9685_Write8(hi2c, addr, 0x00, oldmode | MODE1_RESTART);
}


void PCA9685_SetPWM(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t num, uint16_t on, uint16_t off) {
    PCA9685_Write8(hi2c, addr, 0x06 + 4 * num, on & 0xFF);
    PCA9685_Write8(hi2c, addr, 0x07 + 4 * num, on >> 8);
    PCA9685_Write8(hi2c, addr, 0x08 + 4 * num, off & 0xFF);
    PCA9685_Write8(hi2c, addr, 0x09 + 4 * num, off >> 8);
}


void PCA9685_SetPin(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t num, uint16_t val, uint8_t invert) {
    if (invert) {
        if (val == 0) {
            PCA9685_SetPWM(hi2c, addr, num, 4096, 0);
        } else if (val >= 4095) {
            PCA9685_SetPWM(hi2c, addr, num, 0, 4096);
        } else {
            PCA9685_SetPWM(hi2c, addr, num, 0, 4096 - val);
        }
    } else {
        if (val == 0) {
            PCA9685_SetPWM(hi2c, addr, num, 0, 4096);
        } else if (val >= 4095) {
            PCA9685_SetPWM(hi2c, addr, num, 4096, 0);
        } else {
            PCA9685_SetPWM(hi2c, addr, num, 0, val);
        }
    }
}
