/*
 * API_controlFSM.c
 *
 *  Created on: Dec 1, 2023
 *      Author: hollerller
 */

#include <driver_BME280.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "API_controlFSM.h"

// Private declarations

typedef enum{
NORMAL_TH,
T_ALERT,
H_ALERT,
TH_ALERT,

} controlFSM_state_t ;		// Define a variable with the FMS states

static controlFSM_state_t controlFSM_state;


void controlFSM_init(){			// Initialize the FMS

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);



}

void controlFSM_update(){

	//uint8_t buttonState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin); // Read the button state (pressed or released)

	float minTemp = 20.0;
	float maxTemp = 25.0;

	float minHum = 35.0;
	float maxHum = 70.0;

/*	switch (controlFSM_state){

	case NORMAL_TH:		// State when button is released

			ledOff();

			if (temp < minTemp || temp > maxTemp) {

				controlFSM_state = T_ALERT;

			}

		break;

		case T_ALERT:

			ledTalert();

						if (temp >= minTemp && temp <= maxTemp) {

							controlFSM_state = NORMAL_TH;

						}

					break;

		default:

			controlFSM_state = NORMAL_TH;

			break;

	}*/


	switch (controlFSM_state){

	case NORMAL_TH:		// State when button is released

		ledOff();

		if ((temp < minTemp || temp > maxTemp) && (hum < minHum || hum > maxHum)) {

			controlFSM_state = TH_ALERT;

		} else if (temp < minTemp || temp > maxTemp)  {

			controlFSM_state = T_ALERT;

		} else if (hum < minHum || hum > maxHum) {

			controlFSM_state = H_ALERT;

		}

	break;

	case T_ALERT:

		ledTalert();

		if ((temp >= minTemp && temp <= maxTemp) && (hum >= minHum && hum <= maxHum)) {

					controlFSM_state = NORMAL_TH;

				} else if ((temp >= minTemp && temp <= maxTemp) && (hum < minHum || hum > maxHum)) {

					controlFSM_state = H_ALERT;

				} else if ((temp < minTemp || temp > maxTemp) && (hum < minHum || hum > maxHum)) {

					controlFSM_state = TH_ALERT;

				}

	case H_ALERT:


		ledHalert();

		if ((temp >= minTemp && temp <= maxTemp) && (hum >= minHum && hum <= maxHum)) {

					controlFSM_state = NORMAL_TH;

		} else if ((temp < minTemp || temp > maxTemp) && (hum >= minHum && hum <= maxHum)) {

			controlFSM_state = T_ALERT;

		} else if ((temp < minTemp || temp > maxTemp) && (hum < minHum || hum > maxHum)) {

			controlFSM_state = TH_ALERT;

		}

		break;

	case TH_ALERT:	// State for the rising edge


		ledTHalert();


		if ((temp >= minTemp && temp <= maxTemp) && (hum >= minHum && hum <= maxHum)) {

					controlFSM_state = NORMAL_TH;

		} else if ((temp < minTemp && temp > maxTemp) && (hum >= minHum && hum <= maxHum)) {

			controlFSM_state = T_ALERT;

		} else if ((temp >= minTemp && temp <= maxTemp) && (hum < minHum || hum > maxHum)) {

			controlFSM_state = H_ALERT;

		}

		break;

	default:

		controlFSM_state = NORMAL_TH;

		break;

	}
}

void ledOff(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}


void ledTalert(){
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(250);

}


void ledHalert(){
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(500);

}


void ledTHalert(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

}
