//// SysTick.h
//// Runs on LM4F120/TM4C123
//// Provide functions that initialize the SysTick module, wait at least a
//// designated number of clock cycles, and wait approximately a multiple
//// of 10 milliseconds using busy wait.  After a power-on-reset, the
//// LM4F120 gets its clock from the 16 MHz precision internal oscillator,
//// which can vary by +/- 1% at room temperature and +/- 3% across all
//// temperature ranges.  If you are using this module, you may need more
//// precise timing, so it is assumed that you are using the PLL to set
//// the system clock to 50 MHz.  This matters for the function
//// SysTick_Wait10ms(), which will wait longer than 10 ms if the clock is
//// slower.
//// Daniel Valvano
//// September 11, 2013

///* This example accompanies the book
//   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
//   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
//   Program 2.11, Section 2.6

// Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
//    You may use, edit, run or distribute this file
//    as long as the above copyright notice remains
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// For more information about my classes, my research, and my books, see
// http://users.ece.utexas.edu/~valvano/
// */
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define LEDS      (*((volatile uint32_t *)0x40025038))
#define RED       0x02
#define BLUE      0x04
#define GREEN     0x08
extern unsigned long Sound_Period;

//---------------------------TEST1--------------------------------------
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Music.h"
#include "DAC.h"

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 500000-1;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000;
  NVIC_ST_CTRL_R = 0x07;
	Sound_Period = 0;
}

uint32_t voiceIndex = 0;
Note *previousNote = 0;

int long long count = 50000;
void SysTick_Handler(void) {
	count--;
	if (count == 0){
		LEDS ^= BLUE;
		count = 50000;
	}
	if (Sound_Period){
		voiceIndex = ((voiceIndex+1)&0x1F);
		DAC_Out(Sinwave[voiceIndex]);
	}
}


//---------------------------TEST2--------------------------------------

//#include <stdint.h>
//#include "../inc/tm4c123gh6pm.h"
//#include "Music.h"
//#include "DAC.h"

//#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
//#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
//#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
//#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
//#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

//// Initialize SysTick with busy wait running at bus clock.
//void SysTick_Init(void){
//  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
//  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
//  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
//                                        // enable SysTick with core clock
//  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
//}

//uint32_t voiceIndex = 0;
//Note *previousNote = 0;
//void SysTick_Handler(void) {
//	if(voiceIndex > 2000)
//		DAC_Out((Instrument_CurrentVoltage(voiceIndex) - 2048) /* * Instrument_EnvelopeMultiplier(voiceIndex) / ENVELOPE_SCALE */ + 2048);
//	Note *newNote = Song_CurrentNote();
//	if(newNote != previousNote) {
//		previousNote = newNote;
//		if(newNote->pitch == 0)
//			NVIC_ST_RELOAD_R = 0;
//		else
//			NVIC_ST_RELOAD_R = 800000 / newNote->pitch;
//		voiceIndex %= 64;
//	}
//	voiceIndex += 1;
//}




//---------------------------TEST3--------------------------------------

//#include <stdint.h>
//#include "../inc/tm4c123gh6pm.h"
//#include "Music.h"
//#include "DAC.h"

//#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
//#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
//#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
//#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
//#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value
//#define TRUE										1
//#define FALSE										0

//// Initialize SysTick with busy wait running at bus clock.
//void SysTick_Init(void){
//  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
//  NVIC_ST_RELOAD_R = 500;  // maximum reload value
//  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
//                                        // enable SysTick with core clock
//	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000;
//  //NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
//	NVIC_ST_CTRL_R = 0x07;
//}

//// SysTick Interupt Handler
//extern uint32_t Systick_one_sec;

//uint32_t voiceIndex = 0;
//Note *previousNote = 0;
//void SysTick_Handler(void) {
//	if(voiceIndex > 2000)
//		DAC_Out((Instrument_CurrentVoltage(voiceIndex) - 2048) /* * Instrument_EnvelopeMultiplier(voiceIndex) / ENVELOPE_SCALE */ + 2048);
//	Note *newNote = Song_CurrentNote();
//	if(newNote != previousNote) {
//		previousNote = newNote;
//		if(newNote->pitch == 0)
//			NVIC_ST_RELOAD_R = 0;
//		else
//			NVIC_ST_RELOAD_R = 800000 / newNote->pitch;
//		voiceIndex %= 64;
//	}
//	voiceIndex += 1;
//}

//// Time delay using busy wait.
//// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)
//void SysTick_Wait(uint32_t delay){
//  volatile uint32_t elapsedTime;
//  uint32_t startTime = NVIC_ST_CURRENT_R;
//  do{
//    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
//  }
//  while(elapsedTime <= delay);
//}

//// Time delay using busy wait.
//// This assumes 50 MHz system clock.
//void SysTick_Wait10ms(uint32_t delay){
//  uint32_t i;
//  for(i=0; i<delay; i++){
//    SysTick_Wait(500000);  // wait 10ms (assumes 50 MHz clock)
//  }
//}

