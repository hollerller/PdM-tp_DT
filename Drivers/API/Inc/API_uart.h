/*
 * API_uart.h
 *
 *  Created on: Nov 19, 2023
 *      Author: hollerller
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_



// Define the function prototypes for UART

bool_t uartInit(); 	// Initializes the UART settings
void uartSendString(uint8_t * pstring); 	// Function to transmit a complete string (until \0 char)
void uartSendStringSize(uint8_t * pstring, uint16_t size);	// Function to send a string and receives also the string size
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);	// Function to receive a string



#endif /* API_INC_API_UART_H_ */
