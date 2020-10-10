/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include "FreeRTOS.h"
#include "task.h"
#include "penControl.h"

static void prvSetupHardware(void){
	SystemCoreClockUpdate();
	Board_Init();
}

extern "C" {

void vConfigureTimerForRunTimeStats( void ) {
	Chip_SCT_Init(LPC_SCTSMALL1);
	LPC_SCTSMALL1->CONFIG = SCT_CONFIG_32BIT_COUNTER;
	LPC_SCTSMALL1->CTRL_U = SCT_CTRL_PRE_L(255) | SCT_CTRL_CLRCTR_L; // set prescaler to 256 (255 +1), and start timer
}

}

static void buttonTask (void * pvParameters){
	penControl jaska = penControl(LPC_SCT0, SWM_SCT0_OUT0_O, 0, 10);

	vTaskDelay(100);

	while(1){
		jaska.setPenPosition(90);
		DEBUGOUT("%d\r\n",jaska.getPenPosition());
		vTaskDelay(1000);
		jaska.setPenPosition(180);
		DEBUGOUT("%d\r\n",jaska.getPenPosition());
		vTaskDelay(1000);
		jaska.setPenPosition(140);
		DEBUGOUT("%d\r\n",jaska.getPenPosition());
		vTaskDelay(1000);
		jaska.setPenPosition(255);
		DEBUGOUT("%d\r\n",jaska.getPenPosition());
		vTaskDelay(1000);


	}
}

int main(void)
{
	prvSetupHardware();

	xTaskCreate(buttonTask, "Button",
					configMINIMAL_STACK_SIZE + 128, NULL, (tskIDLE_PRIORITY + 1UL),
					(TaskHandle_t *) NULL);

	vTaskStartScheduler();

	return 1;
}
