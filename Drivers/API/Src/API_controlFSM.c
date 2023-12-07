/*
 * API_controlFSM.c
 *
 *  Created on: Dec 1, 2023
 *      Author: hollerller
 */

#include "driver_BME280.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"
#include "main.h"
#include "API_controlFSM.h"

/*
 * Enumeration
 * Define variables with the FMS states
 *
 */
typedef enum {
	TH_NORMAL,	// State for normal values for Temperature and Humidity
	T_ALERT,	// State for temperature outside thresholds
	H_ALERT,	// State for humidity outside thresholds
	TH_ALERT,	// State for temperature and humidity outside thresholds

} controlFSM_state_t;

/*
 * Delay used to control the blinking led when there's a temperature alert
 */
#define BME_HAL_TDELAY 100

/*
 * Delay used to control the blinking led when there's a humidity alert
 */
#define BME_HAL_HDELAY 500

static const uint8_t DEBOUNCETIME = 40; // Debounce delay constant

static delay_t debounceDelay;		// Create a variable type delay_t
static tick_t initialDelay = DEBOUNCETIME; // Set the initial time of the delay (40ms)

// Variable to store the current state of the FSM
static controlFSM_state_t controlFSM_state;

static float temp;
static float hum;

// Functions to control the LED depending on the FSM states

static void ledOff();
static void ledTalert();
static void ledTHalert();
static void ledHalert();

// Initializes the control FSM
void controlFSM_init() {
	// Set the initial state to TH_NORMAL and turn off the led alert
	controlFSM_state = TH_NORMAL;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	delayInit(&debounceDelay, initialDelay);	// Initialize the delay

}

/*
 * Function to update the FSM based on the values of
 * temperature temp and humidity hum read by the sensor
 *
 */

void controlFSM_update() {

	// **** Define the temperature and humidity thresholds ****
	const float minTemp = 20.0;
	const float maxTemp = 26.0;

	const float minHum = 35.0;
	const float maxHum = 65.0;

	// ****

	// Get current values of temperature and humidity

	temp = BME280_getTemp();
	hum = BME280_getHum();

	switch (controlFSM_state) {

	case TH_NORMAL:	// State when temperature and humidity are within thresholds

		ledOff();	// Calls function to turn off the led

		if (delayRead(&debounceDelay)) {

			// If temperature and humidity are outside thresholds update state to TH_ALERT
			if ((temp < minTemp || temp > maxTemp)
					&& (hum < minHum || hum > maxHum)) {

				controlFSM_state = TH_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If temperature is outside thresholds update state to T_ALERT

			} else if (temp < minTemp || temp > maxTemp) {

				controlFSM_state = T_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If humidity is outside thresholds update state to H_ALERT

			} else if (hum < minHum || hum > maxHum) {

				controlFSM_state = H_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

			}
		}
		break;

	case T_ALERT:// State when temperature is outside thresholds but humidity is ok

		ledTalert();

		if (delayRead(&debounceDelay)) {

			// If temperature and humidity are within thresholds update state to TH_NORMAL

			if ((temp >= minTemp && temp <= maxTemp)
					&& (hum >= minHum && hum <= maxHum)) {

				controlFSM_state = TH_NORMAL;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If humidity is outside thresholds update state to H_ALERT

			} else if ((temp >= minTemp && temp <= maxTemp)
					&& (hum < minHum || hum > maxHum)) {

				controlFSM_state = H_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If temperature and humidity are outside thresholds update state to TH_ALERT
			} else if ((temp < minTemp || temp > maxTemp)
					&& (hum < minHum || hum > maxHum)) {

				controlFSM_state = TH_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

			}
		}
		break;

	case H_ALERT:// State when humidity is outside thresholds but temperature is ok

		ledHalert();

		if (delayRead(&debounceDelay)) {

			// If temperature and humidity are within thresholds update state to TH_NORMAL

			if ((temp >= minTemp && temp <= maxTemp)
					&& (hum >= minHum && hum <= maxHum)) {

				controlFSM_state = TH_NORMAL;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If temperature is outside thresholds update state to T_ALERT

			} else if ((temp < minTemp || temp > maxTemp)
					&& (hum >= minHum && hum <= maxHum)) {

				controlFSM_state = T_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If temperature and humidity are outside thresholds update state to TH_ALERT
			} else if ((temp < minTemp || temp > maxTemp)
					&& (hum < minHum || hum > maxHum)) {

				controlFSM_state = TH_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

			}
		}
		break;

	case TH_ALERT:// State when temperature and humidity are outside thresholds

		ledTHalert();

		if (delayRead(&debounceDelay)) {

			// If temperature and humidity are within thresholds update state to TH_NORMAL

			if ((temp >= minTemp && temp <= maxTemp)
					&& (hum >= minHum && hum <= maxHum)) {

				controlFSM_state = TH_NORMAL;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If temperature is outside thresholds update state to T_ALERT

			} else if (hum >= minHum && hum <= maxHum) {

				controlFSM_state = T_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

				// If humidity is outside thresholds update state to H_ALERT

			} else if (temp >= minTemp && temp <= maxTemp) {

				controlFSM_state = H_ALERT;
				delayInit(&debounceDelay, initialDelay);// Initialize the delay for debounce

			}

		}
		break;

	default:
		// default state for the FSM
		controlFSM_state = TH_NORMAL;

		break;

	}
}

// Function to turn off the led
void ledOff() {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

// Function to blink the led when there's a temperature alert TDELAY = 100

void ledTalert() {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(BME_HAL_TDELAY);

}

// Function to blink the led when there's a humidity alert HDELAY = 500

void ledHalert() {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_Delay(BME_HAL_HDELAY);

}

// Function to turn on the led
void ledTHalert() {
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

}
