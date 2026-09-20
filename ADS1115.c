#include "ADS1115.h"

int16_t read_analog(i2c_master_dev_handle_t i2c_dev_handle, int16_t timeout){
    uint8_t buff[2] = {0};
    uint8_t reg = CONVERSION_REG;
    uint8_t reg2 = CONFIG_REG;
    
    bool isRunning = true;
    uint8_t buff_temp[2] = {0};
    int16_t maxtime = 0;
    i2c_master_transmit(i2c_dev_handle, &reg, 1, timeout);
    while(isRunning){
        i2c_master_transmit(i2c_dev_handle, &reg2, 1, timeout);
        i2c_master_receive(i2c_dev_handle, buff_temp, 2, timeout);
        if(buff_temp[0] >= 128){
            isRunning = false;
        }
        else{
            maxtime++;
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }
    };
    i2c_master_transmit(i2c_dev_handle, &reg, 1, timeout);
    i2c_master_receive(i2c_dev_handle, buff, 2, timeout);
    int16_t raw = (buff[0] << 8) | buff[1];
    return raw;
        
     
    }

    


void config_ADC(i2c_master_dev_handle_t i2c_dev_handle, ads_com *Alpha){

    uint8_t byte_part_1 = (Alpha->os_bit<<7) | (Alpha->mux_bits << 4) | (Alpha->range_bits << 1) | (Alpha->mode);
    uint8_t byte_part_2 = (Alpha->speed <<5) | (Alpha->comp_mode<< 4) | (Alpha->alert_mode << 3) | (Alpha->latch << 2) | (Alpha->assert_bits);

    uint8_t one_peice[3] = {
        CONFIG_REG,
        byte_part_1,
        byte_part_2,
    };

    i2c_master_transmit(i2c_dev_handle, one_peice, 3, -1);
    vTaskDelay(pdMS_TO_TICKS(5));
}