/*
 * API_i2c.h
 *
 *  Created on: Dec 3, 2023
 *      Author: hollerller
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

// Define the function prototypes for I2C

#include <string.h>
#include "stm32f4xx_hal.h"
#include "API_delay.h"

bool_t i2c_Init(); 	// Initializes the I2C settings

void i2c_Master_Trasmit(uint8_t * pData, uint8_t deviceAddress, uint16_t size);

void i2c_Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t *pData, uint16_t size);

void i2c_Mem_Read(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t *pData, uint16_t size);



#endif /* API_INC_API_I2C_H_ */
