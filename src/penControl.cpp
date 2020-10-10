/*
 * penControl.cpp
 *
 *  Created on: 8 Oct 2020
 *      Author: Joel
 */

#include "penControl.h"

penControl::penControl(LPC_SCT_T * pSCT, CHIP_SWM_PIN_MOVABLE pSWM, int port, int pin) {
	this->pSCT = pSCT;
	Chip_SCT_Init(pSCT);

	multiplier = float(1000) / (float)255;

	pSCT->CONFIG					|= (1 << 17);				// Two 16-bit timers, auto limit
	pSCT->CTRL_L					|= ((72-1) << 5);			// Set clock time to 72MHz/72=1MHz

	pSCT->MATCHREL[0].L 			= 20000-1;					// Set the frequency 1MHz/20000 = 50Hz match frequency
	pSCT->MATCHREL[1].L         	= 1000 + 180 * multiplier;	// Sets the duty cycle

	pSCT->EVENT[0].STATE 			= 0xFFFFFFFF;				// Event 0 happens in all states
	pSCT->EVENT[0].CTRL 			= (1 << 12);				// Match 0 condition

	pSCT->EVENT[1].STATE 			= 0xFFFFFFFF;				// Event 1 happens in all states
	pSCT->EVENT[1].CTRL				= (1 << 0) | (1 << 12);		// Match 1 condition

	pSCT->OUT[0].SET				= (1 << 0);					// Event 0 sets current as high
	pSCT->OUT[0].CLR				= (1 << 1);					// Event 1 sets current as low

	Chip_SWM_MovablePortPinAssign(pSWM, port, pin);				// Assign SCT to Pen port and pin 0,10

	pSCT->CTRL_L					&= ~(1 << 2);				// Start the timer
}

penControl::~penControl() {
	// TODO Auto-generated destructor stub
}

/*Set pen position, should be 0 - 255*/
void penControl::setPenPosition(uint8_t position){
	savedPenPos = position;
	pSCT->MATCHREL[1].L = 1000 + position * multiplier;			// Change duty cycle to change pen position
	vTaskDelay(100);
}

/*Return pen position, should be 0 - 255*/
uint8_t penControl::getPenPosition(){
	//savedPenPos = (pSCT->MATCHREL[1].L - 1000) / multiplier;	//Get current duty cycle
	return savedPenPos;
}
