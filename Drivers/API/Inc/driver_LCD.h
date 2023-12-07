/*
 * API_LCD.h
 *
 *  Created on: Dec 3, 2023
 *      Author: hollerller
 */

#ifndef API_INC_DRIVER_LCD_H_
#define API_INC_DRIVER_LCD_H_

void LCD_init();
void LCD_sendCMD(char cmd);
void LCD_sendChar(char caracter);
void LCD_sendString(char * text);
void LCD_clear();


#endif /* API_INC_DRIVER_LCD_H_ */
