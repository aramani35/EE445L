// MAX5353TestMain.c
// Runs on LM4F120/TM4C123
// Test the functions provided in MAX5353.c by outputting
// a sine wave at a particular frequency.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Figure 7.19 for complete schematic

#include "..//inc//tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include "DAC.h"
#include "SysTick.h"
#include "Music.h"
#include "Timer0A.h"
#include "Switches.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


// 12-bit 32-element sine wave
// multiply each value by 2 to shift into bits 12:1 of SSI packet
// three control bits in 15:13 are all zero for immediate DAC update
// book figure shows MAX5353 in unipolar rail-to-rail configuration
// that means when wave[n] = 0x0000 (LSB = 0), output = 0
//                 wave[n] = 0x1000 (LSB = 0), output = Vref
//                 wave[n] = 0x1FFE (LSB = 0), output = 2*Vref


void PortF_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x20;				// activate port F
	while((SYSCTL_PRGPIO_R&0x0020) == 0){};// ready?
	GPIO_PORTF_DIR_R |= 0x0E;					// make PF3-1 output (PF3-1 built-in LEDs)
	GPIO_PORTF_AFSEL_R &= ~0x0E;			// disable alt funct on PF3-1
	GPIO_PORTF_DEN_R |= 0x0E;					// enable digital I/O on PF3-1
																		// configure PF3-1 as GPIO
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
	GPIO_PORTF_AMSEL_R = 0;						// disable analog functionality on PF
}

//const Song song = {90, mary_lamb};
//const Song Prelude = {128, ff7_pre};
//bool Play_mode = false;
//void Pause(void);


int main(void){
	DisableInterrupts();
//  uint32_t i=0;
	PortF_Init();
	Switches_Init();
  DAC_Init(0x1000);                  // initialize with command: Vout = Vref
  SysTick_Init();
	Timer0A_Init(&Song_PlayHandler, F16HZ);
	EnableInterrupts();
	
//	Song_PlayInit(); // need song
	Pause();
  while(1){
		switch(Switches_In()) {
		  case 1  : TogglePlayPause(); break;	// If PE0 pressed, play C note  
			case 2  : Rewind(); break;    // If PE1 pressed, play D note
			case 4  : TIMER0_TAILR_R = F64HZ-1; break;    // If PE2 pressed, play E note
			case 8  : TIMER0_TAILR_R = F16HZ-1; break;    // If PE3 pressed, play G note
		}
	}
}
