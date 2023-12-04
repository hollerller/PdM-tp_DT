/*
 * API_i2c.h
 *
 *  Created on: Dec 3, 2023
 *      Author: hollerller
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_


// Define the function prototypes for I2C

bool_t i2c_Init(); 		// Initializes the I2C settings

// Function to transmit from mater to slave device
void i2c_Master_Trasmit(uint8_t * pData, uint8_t deviceAddress, uint16_t size);


// Function to write an ammount of data to a specific memory address using I2C
void i2c_Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t *pData, uint16_t size);


// Function to read an ammount of data to a specific memory address using I2C
void i2c_Mem_Read(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t *pData, uint16_t size);



#endif /* API_INC_API_I2C_H_ */
