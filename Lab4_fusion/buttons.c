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
extern int setAlarm, blinkDigits;
extern int secs, mins, hrs;
int buttonData;
volatile int pf4_button = 1;
volatile int pe0_button = 0;
volatile int pe1_button = 0;
int alarmComplete = 0;
int time[] = {0,0,0};
int index_time = 2;
int alarmActivatedFlag = 0;
int hr_alarm;
int min_alarm;
int sec_alarm;
int displayAlarmActivatedFlag;
int startInputingDigits = 0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void Buttons_Init(void) {
//	SYSCTL_RCGCGPIO_R |= 0x10;        // 1) activate clock for Port E
//  while((SYSCTL_PRGPIO_R&0x10)==0); // allow time for clock to start
//                                    // 2) no need to unlock PE0-3
//  GPIO_PORTE_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
//  GPIO_PORTE_AMSEL_R &= ~0x0F;      // 4) disable analog function on PE0-3
//                                    // 5) no pullup for external switches
//  GPIO_PORTE_DIR_R &= ~0x0F;        // 5) set direction to output
//  GPIO_PORTE_AFSEL_R &= ~0x0F;      // 6) regular port function
//  GPIO_PORTE_DEN_R |= 0x0F;         // 7) enable digital port
	SYSCTL_RCGCGPIO_R |= 0x10;  //INITIALIZE PORT E
	while((SYSCTL_PRGPIO_R&0x10) == 0){};			//PORT E
	GPIO_PORTE_DIR_R &= 0xE0;								//PE0-4 ARE INPUTS
	GPIO_PORTE_AFSEL_R &= 0xE2;							//AFSEL OFF
	GPIO_PORTE_DEN_R |= 0x1B;								//ENABLE PE0,1,3,4n
}
	
	


void checkButtonPressed(void){
	if (PF4 == 0){
		setAlarm = 1;
	}
}

// displays to lcd
void setAlarmTime(int in_t[]){
	DisableInterrupts();
	if (in_t[0]>=24)
		in_t[0] = 0;
	if (in_t[1]>=60)
		in_t[1] = 0;
	if (in_t[2]>=60)
		in_t[2] = 0;
	
	ST7735_SetCursor(6,4);
	if (in_t[0]<10)
		ST7735_OutUDec(0);
	ST7735_OutUDec(in_t[0]);
	ST7735_OutChar(':');
	if (in_t[1]<10) 
		ST7735_OutUDec(0);
	ST7735_OutUDec(in_t[1]);
	ST7735_OutChar(':');
	if (in_t[2]<10)
		ST7735_OutUDec(0);
	ST7735_OutUDec(in_t[2]);
	EnableInterrupts();
}

// func will keep loop till pf4 is pressed confirming the clock time
// intilaizes variables to 0 for hr, min, sec
void inputClockTime(void){ 
	startInputingDigits = 1;
	setAlarm = 1;
	alarmComplete = 0;
	time[0] = 0;
	time[1] = 0;
	time[2] = 0;
	index_time = 0;
	while(!alarmComplete){		
		if (!(GPIO_PORTF_DATA_R&0x10)){		// Complete clock setup
			while (!(GPIO_PORTF_DATA_R&0x10)){}
			alarmComplete = 1;
			startInputingDigits = 0;
			hrs = time[0];
			mins = time[1];
			secs = time[2];
//			alarmActivatedFlag = 1;					// flag allows interrupt to compare time with alarm time
			break;
		}
	}
}


// func will keep loop till pf4 is pressed confirming the alarm time
// intilaizes variables to 0 for hr, min, sec
void inputAlarmTime(void){ 
	startInputingDigits = 1;
	alarmComplete = 0;
	time[0] = 0;
	time[1] = 0;
	time[2] = 0;
	index_time = 0;
	
	while(!alarmComplete){		
		if (!(GPIO_PORTF_DATA_R&0x10)){		// Complete alarm setup
			while (!(GPIO_PORTF_DATA_R&0x10)){}
			alarmComplete = 1;
			startInputingDigits = 0;
			hr_alarm = time[0];
			min_alarm = time[1];
			sec_alarm = time[2];
			alarmActivatedFlag = 1;					// flag allows interrupt to compare time with alarm time				
			break;
		}
	}
}


void blinkHours(void){
	ST7735_SetCursor(6,4);
	if (blinkDigits == 1){
		ST7735_OutString("  ");
	}
	else{
		if (time[0]<10)
			ST7735_OutUDec(0);
		ST7735_OutUDec(time[0]);
	}
}

void blinkMins(void){
	ST7735_SetCursor(9,4);
	if (blinkDigits == 1){
		ST7735_OutString("  ");
	}
	else{
		if (time[1]<10)
			ST7735_OutUDec(0);
		ST7735_OutUDec(time[1]);
	}
}

void blinkSecs(void){
	ST7735_SetCursor(12,4);
	if (blinkDigits == 1){
		ST7735_OutString("  ");
	}
	else{
		if (time[2]<10)
			ST7735_OutUDec(0);
		ST7735_OutUDec(time[2]);
	}
}

void removeBlinkEffect(int i){
	if(i == 0){
		ST7735_SetCursor(6,4);
		if (time[0]<10)
			ST7735_OutUDec(0);
		ST7735_OutUDec(time[0]);
	}
	
	if(i == 1){
		ST7735_SetCursor(9,4);
		if (time[1]<10)
			ST7735_OutUDec(0);
		ST7735_OutUDec(time[1]);
	}
	
	else if (i==2){
		ST7735_SetCursor(12,4);
		if (time[2]<10)
			ST7735_OutUDec(0);
		ST7735_OutUDec(time[2]);
	}
}

int pe0Flag = 0;
int pe1Flag = 0;

// Uses interrupt to check if a button has been pressed
// if pf0 is pressed, rotate between hr,min,sec
// if pe1 is pressed, increment number
void buttonStatus(int x, int y, int z){
	pe0_button = x;
	pe1_button = z;
	if(pe1_button == 0)
		pe1Flag=0;
	if(pe0_button == 0)
		pe0Flag=0;

	
	if ((pe1_button == 1) & (pe1Flag==0)){			// Increase digit
		pe1Flag = 1;
		time[index_time]+=1;
		setAlarmTime(time);
	}
		
	if ((pe0_button ==1) & (pe0Flag == 0)){			// Rotate between hr,min,sec
		pe0Flag = 1;
		removeBlinkEffect(index_time);
		index_time+=1;
		if (index_time > 2)
			index_time=0;
	}
	
	if (index_time == 0)
		blinkHours();
	if (index_time == 1)
		blinkMins();
	if (index_time == 2)
		blinkSecs();
}
