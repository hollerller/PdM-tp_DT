/* Private function prototypes
 *
 *  */

// Includes

#include "API_delay.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"

// Implement the function delayInit to establish the initial state

void delayInit( delay_t *delay, tick_t duration ) {

	// check if the duration is valid ( non negative ):

	if (duration >= 0) {
		delay->duration = duration;
		delay->running = false;
	} else {
		printf("Error: Negative duration is not valid.");
		delay->duration = 0;
		delay->running = false;
	}
}

// Check if the delay has finished

bool_t delayRead(delay_t *delay){

	tick_t currentTime = HAL_GetTick();

	if (delay->running) {

		if (currentTime - delay->startTime >= delay->duration) {
			delay->running = false;
			return true;
		}
		else {
			return false;
		}

	}
	else {
		delay->running = true;
		delay->startTime = currentTime;
		return false;
	}
}

// Update the delay duration


void delayWrite( delay_t * delay, tick_t duration ){
	delay->duration = duration;
}

