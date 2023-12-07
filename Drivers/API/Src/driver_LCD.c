/*
 * API_LCD.c
 *
 *  Created on: Dec 3, 2023
 *      Author: hollerller
 */

#include "stm32f4xx_hal.h"

#include <stdint.h>
#include "driver_LCD.h"
#include "API_delay.h"
#include "API_i2c.h"
#include <string.h>

#define INITIALDELAY 20



void LCD_init(){

	HAL_Delay(20);
	LCD_sendCMD(0x30);
	HAL_Delay(10);
	LCD_sendCMD(0x30);
	HAL_Delay(1);
	LCD_sendCMD(0x30);
	LCD_sendCMD(0x20);

	// Initial settings

	LCD_sendCMD(0x28); 	// Function set DL = 0 (4 bit mode) N = 1 (2 Lines) F = - (5x7 style)
	LCD_sendCMD(0x08);	// Display Switch off
	LCD_sendCMD(0x01);	// Clear display
	HAL_Delay(2);
	LCD_sendCMD(0x06);	// Entry mode
	LCD_sendCMD(0x0C);	// Display Switch on
	LCD_sendCMD(0x01);	// Clear display
	HAL_Delay(2);


}


void LCD_sendCMD(char command){

	uint8_t cmdMSB = (command & 0xF0);
	uint8_t cmdLSB = ((command << 4) & 0xF0);
	uint8_t enableOn = 0x0C;
	uint8_t enableOff = 0x08;

	uint8_t sendCMD[4] = {
			cmdMSB | enableOn,
			cmdMSB | enableOff,
			cmdLSB | enableOn,
			cmdLSB | enableOff};

	i2c_Master_Trasmit(sendCMD, 0x4E, 4);

}

void LCD_sendChar(char caracter){

	uint8_t charMSB = (caracter & 0xF0);
	uint8_t charLSB = ((caracter << 4) & 0xF0);
	uint8_t enableOn = 0x0D;
	uint8_t enableOff = 0x09;

	uint8_t sendChar[4] = {
			charMSB | enableOn,
			charMSB | enableOff,
			charLSB | enableOn,
			charLSB | enableOff};

	i2c_Master_Trasmit(sendChar, 0x4E, 4);


}


void LCD_sendString(char * text){

	uint16_t size = strlen(text);

	for (uint8_t i = 0 ; i < size; i ++) {
		LCD_sendChar(*text);
		text++;
	}

}

void LCD_clear(){
	LCD_sendCMD(0x01);	// Clear display
	HAL_Delay(2);
}

//void LCD_sendByte(char data);
