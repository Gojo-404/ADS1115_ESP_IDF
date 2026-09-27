#include "ADS1115.h" //The include for our header file.

//next up, the main attraction of the show, the read_analog.
int16_t read_analog(i2c_master_dev_handle_t i2c_dev_handle, int16_t timeout){ //returns a 16 bit integer.
    uint8_t buff[2] = {0}; //allocating the buffer for out 16 bit integer, but as 2 8bit integers instead becasue of how I2C works. In practice, dosent matter for you.
    uint8_t reg = CONVERSION_REG; //declearing a variable for holding the address of out Conversion Register as we cannot just directly send #defines.
    uint8_t reg2 = CONFIG_REG; //same as previous but for the Configuration register.
    
    bool isRunning = true; //a simeple IsRunning for our while loop
    uint8_t buff_temp[2] = {0}; //a temporary buffer created to hold the raw values before its a integer, as a two's complement output
    int16_t maxtime = 0; //variabkle for our time.
    //i2c_master_transmit(i2c_dev_handle, &reg, 1, timeout); //transmitting to the Conversion register. this is kinda unnecessary, as it dosent do anything. idk why i kept it.
    while(isRunning){ //the while loop
        i2c_master_transmit(i2c_dev_handle, &reg2, 1, timeout); //the transmit for the Config register
        i2c_master_receive(i2c_dev_handle, buff_temp, 2, timeout); // receiver for the Done or Not-Done state.
        if(buff_temp[0] >= 128){ //a simple hack to check if the MSB is high or not.
            isRunning = false; //exit the loop
        }
        else if(maxtime == timeout * 40){ //time out condition
            break; //break the loop
            return NULL;    //returning NULL
        };
        else{
            maxtime++; //increasing the maxtime variable in case we got Not-Done state
            vTaskDelay(pdMS_TO_TICKS(20)); //a delay of 20 ms
            continue; //continue
        }
    };
    i2c_master_transmit(i2c_dev_handle, &reg, 1, timeout); //the transmit to the Conversion register. you gotta poke it before it returns anything
    i2c_master_receive(i2c_dev_handle, buff, 2, timeout); //catching the results
    int16_t raw = (buff[0] << 8) | buff[1]; //converting the raw value into a 16 bit integer
    return raw; //outputting the ressulting 16 bti integer
        
     
    }

    


void config_ADC(i2c_master_dev_handle_t i2c_dev_handle, ads_com *Alpha){ //sending out comm configuration to the ads.

    uint8_t byte_part_1 = (Alpha->os_bit<<7) | (Alpha->mux_bits << 4) | (Alpha->range_bits << 1) | (Alpha->mode); //Just some simple bit manipulation.
    uint8_t byte_part_2 = (Alpha->speed <<5) | (Alpha->comp_mode<< 4) | (Alpha->alert_mode << 3) | (Alpha->latch << 2) | (Alpha->assert_bits); //more bit manipulation

    uint8_t one_peice[3] = { //the final array holding our values
        CONFIG_REG,
        byte_part_1,
        byte_part_2,
    };

    i2c_master_transmit(i2c_dev_handle, one_peice, 3, -1); //transmitting the values. notice the 3. it just means we are sending 3 bytes of data between one start and stop
    vTaskDelay(pdMS_TO_TICKS(5)); //a slight delay in case the user dosent allocate some time after configuring which can result in some porblems.
}
