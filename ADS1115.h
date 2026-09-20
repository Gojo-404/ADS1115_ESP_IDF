#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define OS_START    0b1
#define OS_STOP     0b0

#define IN0_TO_GND  0b100
#define IN1_TO_GND  0b101
#define IN2_TO_GND  0b110
#define IN3_TO_GND  0b111

#define FSR_6    0b000
#define FSR_4    0b001
#define FSR_2    0b010
#define FSR_1    0b011
#define FSR_0_512 0b100
#define FSR_0_256 0b101

#define CONTINIOUS  0b0
#define ONE_SHOT    0b1

#define FULL_SPEED  0b111
#define SPEED_475   0b110
#define SPEED_250   0b101
#define SPEED_DEAFAULT 0b100
#define SPEED_64    0b011
#define SPEED_32    0b010
#define SPEED_16    0b001
#define SPEED_8     0b000

#define TRADITIONAL 0b0
#define WINDOW      0b1

#define ACTIVE_LOW  0b0
#define ACTIVE_HIGH 0b1

#define NO_LATCH    0b0
#define YES_LATCH   0b1

#define ASSERT_AFTER_1  0b00
#define ASERT_AFTER_2   0b01
#define ASERT_AFTER_4   0b10
#define NO_ASSERT       0b11

#define CONFIG_REG  0x01
#define CONVERSION_REG 0x00

typedef struct{
    uint8_t os_bit;
    uint8_t mux_bits;
    uint8_t range_bits;
    uint8_t mode;
    uint8_t speed;
    uint8_t comp_mode;
    uint8_t alert_mode;
    uint8_t latch;
    uint8_t assert_bits;
}ads_com;

int16_t read_analog(i2c_master_dev_handle_t i2c_dev_handle, int16_t timeout);
void config_ADC(i2c_master_dev_handle_t i2c_dev_handle, ads_com *Alpha);

#endif