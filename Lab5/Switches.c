// Piano.c
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "Switches.h"
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include <stdbool.h>

bool Play_mode;
bool play_pause = false;

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Switches_Init(void){ 
	volatile uint32_t Delay;
  SYSCTL_RCGC2_R |= 0x10;       // 00010000 PORTE
	Delay = SYSCTL_RCGC2_R;       // Delay
	Delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DIR_R |= ~0x0F;    // PE0-PE3 are inputs  
	GPIO_PORTE_PDR_R |= 0x0F;     // 00001111
  GPIO_PORTE_DEN_R |= 0x0F;     // 00001111
}


void Play(void) {
	Play_mode = true;
	NVIC_ST_CTRL_R = 0x07;
	TIMER0_CTL_R = 0x00000001;
	NVIC_ST_RELOAD_R = 500;
}

void Pause(void) {
	Play_mode = false;
	NVIC_ST_CTRL_R = 0;
	TIMER0_CTL_R = 0x00000000;
}


void TogglePlayPause(){
	play_pause = !play_pause;
	if (play_pause)
		Play(); 
	else
		Pause();
}


//extern const Song song;
//extern const Song Prelude;

//void Rewind(void) {
//	Pause();
//	Song_PlayInit(Prelude);
//}



// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Switches_In(void){

  return GPIO_PORTE_DATA_R&0x0F; // Reads inputs
}
