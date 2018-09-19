#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
//#include "Bool.h"
#include "buttons.h"
#include "Timers.h"
#include "ST7735.h"

#define PE0 (*((volatile uint32_t *)0x40024004))
#define PE1 (*((volatile uint32_t *)0x40024008))
#define PE2 (*((volatile uint32_t *)0x40024010))// ADC CONVERTER
#define PE3 (*((volatile uint32_t *)0x40024020))
#define PE4 (*((volatile uint32_t *)0x40024040))
#define PF4 (*((volatile uint32_t *)0x40025040))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF0 (*((volatile uint32_t *)0x40025004))
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
	buttonData = PF0;
	int x;
	x+=5;
	
	if (PF4 == 0){
		setAlarm = 1;
	}
}

void setAlarmTime(int in_t, int limit){
	if (in_t >=limit){
		in_t = 0;
	}
		
	if (in_t < 10)
		ST7735_OutUDec(0);
	ST7735_OutUDec(in_t);
	ST7735_OutString(":00:00");
}


void inputAlarmTime(void){
	int secNum = 0;
	int minNum = 0;
	int hrNum = 0;
	int time[] = {0,0,0};
	int i = 0;
	while(1){
		if (GPIO_PORTE_DATA_R&0x2){
			while (GPIO_PORTE_DATA_R&0x2){}
			time[i]+=1;
			ST7735_SetCursor(6,4);
			if(i==0)
				setAlarmTime(time[i], 24);
			if(i==2)
				setAlarmTime(time[i], 60);
			if(i==2)
				setAlarmTime(time[i], 60);
		}
		if (GPIO_PORTE_DATA_R&0x1){
			while (GPIO_PORTE_DATA_R&0x1){}
			i+=1;
			if (i >2)
				i=0;
	
		}
	}
}
