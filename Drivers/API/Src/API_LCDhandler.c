/*
 * API_LCDhandler.c
 *
 *  Created on: Dec 5, 2023
 *      Author: hollerller
 */

#include "stm32f4xx_hal.h"
#include "API_LCDhandler.h"
#include "driver_BME280.h"
#include "driver_LCD.h"
#include <stdio.h>
#include "main.h"


/*
 * Enumeration
 * Define variables with the LCD states
 *
 */

typedef enum {
	TH_CURRENT,	// State for normal values for Temperature and Humidity
	TH_MAX,	// State for temperature outside thresholds
	TH_MIN,	// State for humidity outside thresholds

} LCDstate_t;

// Variable to store the current state of the FSM
static LCDstate_t LCDstate;

// Declares the initial values for maximum and minimum temp and hum

static float maxTemp = -40;
static float minTemp = 85;
static float maxHum = 0;
static float minHum = 100;

// Save the message string to be sent via LCD

char messageTemp[16] = "";
char messageHum[16] = "";


//

void LCDhandlerInit(){

	LCDstate = TH_CURRENT;

}


void LCDhandlerFSM(){

	// Calls the functions

	tempMax();
	tempMin();
	humMax();
	humMin();

	uint8_t buttonState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin); // Read the button state (pressed or released)

	switch (LCDstate) {

		case TH_CURRENT:	// State to send current measures to LCD


			if (buttonState){
			LCD_clear();
			LCD_sendCMD(0x80);
			sprintf(messageTemp, "Temp: %.2f", temp);
			LCD_sendString(messageTemp);

			LCD_sendCMD(0xC0);
			sprintf(messageHum, "Hum:  %.2f", hum);
			LCD_sendString(messageHum);
		} else {
			LCDstate = TH_MAX;
		}


			break;

		case TH_MAX:// State to send maximum values

			if (buttonState){
			LCD_clear();
			LCD_sendCMD(0x80);
			sprintf(messageTemp, "Max Temp: %.2f", maxTemp);
			LCD_sendString(messageTemp);

			LCD_sendCMD(0xC0);
			sprintf(messageHum, "Max Hum:  %.2f", maxHum);
			LCD_sendString(messageHum);

			} else {
						LCDstate = TH_MIN;
					}

			break;

		case TH_MIN:// State to send minimum values

			if (buttonState){
				LCD_clear();
			LCD_sendCMD(0x80);
			sprintf(messageTemp, "Min Temp: %.2f", minTemp);
			LCD_sendString(messageTemp);

			LCD_sendCMD(0xC0);
			sprintf(messageHum, "Min Hum:  %.2f", minHum);
			LCD_sendString(messageHum);

			} else {
									LCDstate = TH_CURRENT;
								}

			break;

		default:
			// default state for the FSM
			LCDstate = TH_CURRENT;

			break;

		}


}


/*
 * Functions to update the maximum and minimum values of temperature and humidity
 * read and calculated from the sensor
 *
 */


void tempMax(){

	if (temp > maxTemp){
		maxTemp = temp;
	}

}

void tempMin(){

	if (temp < minTemp){
		minTemp = temp;
	}

}

void humMax(){

	if (hum > maxHum){
		maxHum = hum;
	}

}

void humMin(){

	if (hum < minHum){
		minHum = hum;
	}

}

