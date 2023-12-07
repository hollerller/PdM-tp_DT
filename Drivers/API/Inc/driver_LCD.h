/*
 * API_LCD.h
 *
 *  Created on: Dec 3, 2023
 *      Author: hollerller
 */

#ifndef API_INC_DRIVER_LCD_H_
#define API_INC_DRIVER_LCD_H_

void LCD_init();	// Function used to initialize the LCD
void LCD_sendCMD(char cmd);	// Function used to send a comand to the LCD
void LCD_sendChar(char caracter);	// Function used to send a character to the LCD
void LCD_sendString(char * text);	// Function used to send a string to the LCD
void LCD_clear();	// Function used to clear the LCD display


#endif /* API_INC_DRIVER_LCD_H_ */
