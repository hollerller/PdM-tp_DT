/*
 * API_uart.c
 *
 *  Created on: Nov 19, 2023
 *      Author: hollerller
 */

#include <string.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "API_uart.h"

#define TIMEOUT 1000	// Define a timeout value for UART operations


static UART_HandleTypeDef uartHandler;	// Declare a UART_HandleTypeDef handle structure

bool_t uartInit(){

	bool_t uartInitOk = false;	 // Variable to save the UART initialization status

	// Configure the UART settings

	uartHandler.Instance = USART2;
	uartHandler.Init.BaudRate = 115200;	// BaudRate
	uartHandler.Init.WordLength = UART_WORDLENGTH_8B; //Word length
	uartHandler.Init.StopBits = UART_STOPBITS_1;	// Stop bit
	uartHandler.Init.Parity = UART_PARITY_NONE;		//Parity
	uartHandler.Init.Mode = UART_MODE_TX_RX;	// UART Mode
	uartHandler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartHandler.Init.OverSampling = UART_OVERSAMPLING_16;

	  if (HAL_UART_Init(&uartHandler) == HAL_OK)	// Check if the UART was initialized successfuly
	  {

		  uartInitOk = true;

		  // Print an initialization message over UART

	  	  uint8_t initialSettings[] = "UART Initialized at 115200 BaudRate, parity: NONE, Word Length 8 B\r\n";

	  	  uartSendString(initialSettings);


	  }

	  	  return uartInitOk;

}


void uartSendString(uint8_t * pstring){


	uint8_t size = strlen((char * ) pstring);	// Get the size of the string

	HAL_UART_Transmit(&uartHandler, pstring, size, TIMEOUT);	// Transmit the string over UART


}



void uartSendStringSize(uint8_t * pstring, uint16_t size){


	HAL_UART_Transmit(&uartHandler, pstring, size, TIMEOUT);	// Transmit a specific size of the string over UART

}


void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {

	HAL_UART_Receive(&uartHandler, pstring, size, TIMEOUT);	// Receives a specific size of the string over UART

}

