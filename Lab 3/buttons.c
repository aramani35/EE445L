#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
//#include "Bool.h"
#include "buttons.h"
#include "Timers.h"

extern int setAlarm;
int buttonData;

void Buttons_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x10;        // 1) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10)==0); // allow time for clock to start
                                    // 2) no need to unlock PE0-3
  GPIO_PORTE_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
  GPIO_PORTE_AMSEL_R &= ~0x0F;      // 4) disable analog function on PE0-3
                                    // 5) no pullup for external switches
  GPIO_PORTE_DIR_R &= ~0x0F;        // 5) set direction to output
  GPIO_PORTE_AFSEL_R &= ~0x0F;      // 6) regular port function
  GPIO_PORTE_DEN_R |= 0x0F;         // 7) enable digital port
	
	
}

void checkButtonPressed(void){
	buttonData = GPIO_PORTE_DATA_R;
	if (buttonData&0x03){
		setAlarm = 1;
	}
}
	
