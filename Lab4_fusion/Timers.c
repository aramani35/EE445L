#include <stdint.h>
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "buttons.h"
#include <math.h>
	
#define PF1             (*((volatile uint32_t *)0x40025008))
extern int secs, mins, hrs, secFlag, setAlarm, alarmActivatedFlag, displayAlarmActivatedFlag;
extern int hr_alarm, min_alarm, sec_alarm, startInputingDigits, mainSelection;
int alarmBeep = 1;
int ringTheAlarm = 0;
int displayTheAlarm = 0;
int blinkDigits = 0;
int PF0=0;
int PE0=0;
int PE1=0;
volatile int x;
int eraseAlarm = 0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);   // user function


void printDigitalTime(int sec, int min, int hr){
//	ST7735_SetCursor(0,0);
	if (hr<10)
		ST7735_OutUDec(0);
	ST7735_OutUDec(hr);
	ST7735_OutChar(':');
	if (min<10) 
		ST7735_OutUDec(0);
	ST7735_OutUDec(min);
	ST7735_OutChar(':');
	if (sec<10)
		ST7735_OutUDec(0);
	ST7735_OutUDec(sec);
//		ST7735_OutChar('\n');
//	ST7735_SetCursor(9,0);
//	ST7735_OutString(__DATE__);
}

void printAlarmTime(void){
	ST7735_SetCursor(0,15);
	ST7735_OutString("Alarm: ");
	printDigitalTime(sec_alarm, min_alarm, hr_alarm);
}

//void Timer0A_Init100HzInt(void){
//  volatile uint32_t delay;
//  DisableInterrupts();
//  // **** general initialization ****
//  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
//  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
//  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
//  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
//  // **** timer0A initialization ****
//   

     // configure for periodic mode
//  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
//  TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
//  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
//  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
//  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
//  // **** interrupt initialization ****
//                                   // Timer0A=priority 2
//  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
//  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
//}
//void Timer0A_Handler(void){
void clockEventsHandler(void){
//	timeBuff[buffIndex] = TIMER1_TAR_R;
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
	checkButtonPressed();
	blinkDigits ^= 0x1;
	if (setAlarm & startInputingDigits){
		x = GPIO_PORTE_DATA_R;
		PF0 = (!(GPIO_PORTF_DATA_R&0x1));
//		PE0 = (GPIO_PORTE_DATA_R&&0x1);
		PE1 = (GPIO_PORTE_DATA_R&0x2) == 0x2;
		buttonStatus(PF0, PE0, PE1);
//		ST7735_SetCursor(3,0);
		
	}
	if (alarmActivatedFlag){
		if((hrs == hr_alarm) & (mins == min_alarm) & (secs >= sec_alarm & secs <= sec_alarm+2)){
			ringTheAlarm = 1;
		}
		if (!(GPIO_PORTF_DATA_R&0x1) & (mainSelection == 0)){				// Complete alarm setup
			while (!(GPIO_PORTF_DATA_R&0x1)){}
			alarmActivatedFlag = 0;
			displayAlarmActivatedFlag = 0;
			eraseAlarm = 1;
			displayTheAlarm = 0;
			ringTheAlarm = 0;
			PWM0_ENABLE_R &= 0xFFFFFFFE; 
		}
		if(ringTheAlarm==1){
//			PWM0_ENABLE_R |= 0x00000001; 
			if (alarmBeep) {	
				PWM0_ENABLE_R |= 0x00000001; 
			}
			else {
				PWM0_ENABLE_R &= 0xFFFFFFFE;
			}
		}
	}
	
//	if(displayAlarmActivatedFlag == 1){
//		if (alarmActivatedFlag == 1){
//			ST7735_SetCursor(0,1);
//			ST7735_OutString("Alarm: ");
//			printDigitalTime(sec_alarm, min_alarm, hr_alarm);
//		}
//	}
	
}





// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init(uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
//  PeriodicTask = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}


void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  secs+=1;
	secFlag = 1;
	alarmBeep ^= 0x01;
	PF1^=0x02;
	
	if(secs > 59){
		secs = 0;
		mins += 1;
	}
	else if(mins > 59){
		mins = 0;
		hrs += 1;
	}
	else if(hrs > 23){
		hrs = 0;
	}
	
	ST7735_SetCursor(0,0);
	printDigitalTime(secs, mins, hrs);
//	ST7735_OutString(__DATE__);
	
//	if (alarmActivatedFlag == 1 & mainSelection ==0){
//		ST7735_SetCursor(0,1);
//		ST7735_OutString("Alarm: ");
//		printDigitalTime(sec_alarm, min_alarm, hr_alarm);
//	}
//	if (alarmActivatedFlag == 1){
//		ST7735_SetCursor(0,15);
//		ST7735_OutString("Alarm: ");
//		printDigitalTime(sec_alarm, min_alarm, hr_alarm);
//	}
	if(eraseAlarm == 1){
		eraseAlarm = 0;
		ST7735_SetCursor(0,15);
		ST7735_OutString("               ");
	}

}

