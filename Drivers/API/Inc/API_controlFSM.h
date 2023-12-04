/*
 * API_controlFSM.h
 *
 *  Created on: Dec 1, 2023
 *      Author: hollerller
 */

#ifndef API_INC_API_CONTROLFSM_H_
#define API_INC_API_CONTROLFSM_H_

// Declare functions to initialize and update the FMS to control the LED alerts

void controlFSM_init();
void controlFSM_update();

// Functions to control the LED depending on the FSM states

void ledOff();
void ledTalert();
void ledTHalert();
void ledHalert();

#endif /* API_INC_API_CONTROLFSM_H_ */
