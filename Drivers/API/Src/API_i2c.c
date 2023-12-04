/*
 * API_i2c.c
 *
 *  Created on: Dec 3, 2023
 *      Author: hollerller
 */

#include "API_delay.h"
#include "API_i2c.h"
#include "API_uart.h"

#define TIMEOUT 1000

I2C_HandleTypeDef hi2c1;

bool_t i2c_Init() {

	bool_t i2cInitOk = false;// Variable to save the UART initialization status

	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c1) == HAL_OK) {
		i2cInitOk = true;

	} else {

		// Error handling: Print an error message over UART

		uint8_t errorMessage[] = "I2C Initialization Failed \r\n";

		uartSendString(errorMessage);

	}

	return i2cInitOk;

}

void i2c_Master_Trasmit(uint8_t *pData, uint8_t devAddress, uint16_t size) {

	  HAL_I2C_Master_Transmit(&hi2c1, devAddress, pData, size, TIMEOUT);

}


void i2c_Mem_Write(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t *pData, uint16_t size){

	HAL_I2C_Mem_Write(&hi2c1, devAddress, memAddress, memAddSize , pData, size, TIMEOUT);


}

void i2c_Mem_Read(uint16_t devAddress, uint16_t memAddress, uint16_t memAddSize, uint8_t *pData, uint16_t size){

	HAL_I2C_Mem_Read(&hi2c1, devAddress, memAddress, memAddSize , pData, size, TIMEOUT);
}
