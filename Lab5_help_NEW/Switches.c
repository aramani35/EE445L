
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include <stdbool.h>
#include "Switches.h"
#include "Timer1A.h"
#include "Music.h"

void Switches_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x10;        // 1) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10)==0); // allow time for clock to start
                                    // 2) no need to unlock PE0-3
  GPIO_PORTE_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
  GPIO_PORTE_AMSEL_R &= ~0x0F;      // 4) disable analog function on PE0-3
                                    // 5) no pullup for external switches
  GPIO_PORTE_DIR_R &= ~0x0F;        // 5) set direction to output
  GPIO_PORTE_AFSEL_R &= ~0x0F;      // 6) regular port function
  GPIO_PORTE_DEN_R |= 0x0F;         // 7) enable digital port
	
	Timer1_Init();
}
uint16_t debounce[4];

#define waitingCycle 5

void Switches_Read(void) {
	int32_t data = GPIO_PORTE_DATA_R;
	if((data & 0x01) != 0)
		debounce[0] = (data & 0x01)*waitingCycle;
	if((data & 0x02) != 0)
		debounce[1] = ((data & 0x2) >> 1)*waitingCycle;
	if((data & 0x04) != 0)
		debounce[2] = ((data & 0x4) >> 2)*waitingCycle;
	if((data & 0x08) != 0)
		debounce[3] = ((data & 0x8) >> 3)*waitingCycle;
}

extern bool songPlaying;
bool songSpeed = true;
void Pause(void);
void Play(void);
void Rewind(void);
void toggle_play_pause(void);
void toggle_speed_slow(void);
void change_music(void);

void Switches_Pressed(uint32_t button) {	
	switch(button) {
		case 0: toggle_play_pause(); break;
		case 1: Rewind(); break;
		case 2: toggle_speed_slow(); break;
		case 3: change_music(); break;
		default: 0; break;
	}
}
void Switches_10ms_Handler(void) {
	Switches_Read();
	for(int ind = 0; ind < 4; ind += 1) {
		if(debounce[ind] > 0) {
			debounce[ind] -= 1;
			if(debounce[ind] == 0) {
				Switches_Pressed(ind);
			}
		}
	}
}

extern const Song GFF;
extern const Song SOS;
extern const Song test;
int songNum = 0;

void change_music(void) {
	songNum++;
	if (songNum >2) songNum = 0;
	
	switch(songNum) {
		case 0: Pause(); Song_PlayInit(GFF); Play(); break;
		case 1: Pause(); Song_PlayInit(SOS); Play(); break;
		case 2: Pause(); Song_PlayInit(test); Play(); break;
		default: 0; break;
	}
}


void toggle_speed_slow(void) {
	if (songSpeed) {
		TIMER0_TAILR_R = F64HZ-1;
		songSpeed = !songSpeed;
	} 
	else {
		TIMER0_TAILR_R = F24HZ-1;
		songSpeed = !songSpeed;
	}
		
}


void toggle_play_pause(void){
	if (songPlaying)
		Pause();
	else
		Play();
}

void Play(void) {
	songPlaying = true;
	NVIC_ST_CTRL_R = 0x07;
	TIMER0_CTL_R = 0x00000001;
	NVIC_ST_RELOAD_R = 500;
}

void Pause(void) {
	songPlaying = false;
	NVIC_ST_CTRL_R = 0;
	TIMER0_CTL_R = 0x00000000;
}



void Rewind(void) {
	Pause();
		switch(songNum) {
			case 0: Song_PlayInit(GFF); break;
			case 1: Song_PlayInit(SOS); break;
			case 2: Song_PlayInit(test); break;
			default: 0; break;
		}
}
