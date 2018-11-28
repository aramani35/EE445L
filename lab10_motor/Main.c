#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Timer.h"
#include "Buttons.h"
#include "Display.h"
#include "Sound.h"

#include <stdio.h>

uint8_t mode;
uint16_t current_time;
uint16_t alarm_time;
uint8_t set_time;
uint8_t set_alarm;
uint8_t alarm_status;
uint8_t button_value = 0x00; // bit0 is set for PB0, bit1 is set for PB1, etc... bit 4 is set for PF4
//char* color = "ST7735_BLACK";
uint16_t color = 0x0000;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void DelayWait10ms_(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

void testSound(){
	SoundInit();
	AlarmOn();
	DelayWait10ms_(200);	// wait 2 seconds before turning off
	AlarmOff();
	// observe PB7 for square wave
}

void testButtons(){
	
}

void testDisplay(){
	Output_Init();
	
	InitDisplay();
	for(int h = 0; h < 12; h++){
		for(int i = 0; i < 60; i++){
			SetTime_Analog(h, i);
			DelayWait10ms_(10);
		}
	}

	// Test Digital clock
	Init_Digital();
	for(int h = 0; h < 24; h++){
		for(int i = 0; i < 60; i++){
			SetTime_Digital(h, i);
			DelayWait10ms_(15);
		}
	}
	
}

void testTimer(){
	ClockInit();
	// Check watch logs to see if time is updated.
}

/* Set Alarm Button */
void PB0_handler(void){
	
	/* Set hour of alarm */
	if((set_alarm == 0) && (set_time == 0)){
		set_alarm++;
		ST7735_SetCursor(0, 2);												/* Tweak when testing */
		ST7735_OutString("Set Hour");
	}
	/* Set minute of alarm */
	else if (set_alarm == 1){
		set_alarm++;
		//ST7735_FillRect(26, 54, 10, 5, ST7735_BLACK);		/* Tweak when testing */
		ST7735_SetCursor(0, 2);												/* Tweak when testing */
		ST7735_OutString("Set Minute");
	}
	/* Finish editing alarm */
	else if (set_alarm == 2){
		set_alarm = 0;
		ST7735_SetCursor(0, 2);
		ST7735_OutString("          ");	// Clear out text
		//ST7735_FillRect(26, 54, 10, 5, ST7735_BLACK);		/* Tweak when testing */
	}
	GPIO_PORTB_ICR_R |= 0x0F;				// Clear all flags								
	GPIO_PORTB_IM_R |= 0x0F;				// Rearm everything
}

/* Set Time Button */
void PB1_handler(void){
	GPIO_PORTB_IM_R &= ~0x02;													/* Disarm PB1 */
		
	/* Set hour of time */
	if((set_alarm == 0) && (set_time == 0)){
		set_time++;
		ST7735_SetCursor(0, 2);												/* Tweak when testing */
		ST7735_OutString("Set Hour");
	}
	/* Set minute of time */
	else if (set_time == 1){
		set_time++;
		//ST7735_FillRect(26, 54, 10, 5, ST7735_BLACK);		/* Tweak when testing */
		ST7735_SetCursor(0, 2);												/* Tweak when testing */
		ST7735_OutString("Set Minute");
	}
	/* Finish editing time */
	else if (set_time == 2){
		set_time = 0;
		ST7735_SetCursor(0, 2);	
		ST7735_OutString("          ");	// Clear out text
		//ST7735_FillRect(26, 54, 10, 5, ST7735_BLACK);		/* Tweak when testing */
	}
	
	GPIO_PORTB_ICR_R |= 0x0F;				// Clear all flags								
	GPIO_PORTB_IM_R |= 0x0F;				// Rearm everything
}

/* Mode and + Button */
void PB2_handler(void){
	
	/* Incrementing hour of alarm/currentTime */
	if((set_time == 1) || (set_alarm == 1)){
		uint16_t time_copy = set_time ? current_time : alarm_time;
		
		if(((time_copy & 0xFF00) >> 8) == 23){
			time_copy &= 0x00FF;
		}
		else{
			time_copy += 0x100;
		}
		
		if(mode && set_time){
			SetTime_Digital((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else if (!mode && set_time){
			SetTime_Analog((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else{
			Display_Alarm((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		
		set_time ? (current_time = time_copy) : (alarm_time = time_copy);
	}
	
	/* Incrementing minute of alarm/currentTime */
	else if((set_time == 2) || (set_alarm == 2)){
		uint16_t time_copy = set_time ? current_time : alarm_time;
		
		if((time_copy & 0x00FF) == 59){
			time_copy &= 0xFF00;
		}
		else{
			time_copy += 1;
		}
		
		if(mode && set_time){
			SetTime_Digital((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else if (!mode && set_time){
			SetTime_Analog((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else{
			Display_Alarm((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}		
		
		set_time ? (current_time = time_copy) : (alarm_time = time_copy);
	}
	
	/* Changing the mode */
	else{
		mode ^= 1;
		if(mode){
			Init_Digital();
			SetTime_Digital((current_time & 0xFF00) >> 8, current_time & 0xFF);
		}
		else{
			Init_Analog();
			SetTime_Analog((current_time & 0xFF00) >> 8, current_time & 0xFF);
		}
	}
	GPIO_PORTB_ICR_R |= 0x0F;				// Clear all flags								
	GPIO_PORTB_IM_R |= 0x0F;				// Rearm everything
}

void PF4_handler(void){
	if(mode){
		SetTime_Digital((current_time & 0xFF00) >> 8, current_time & 0xFF);
	}
	else{
		SetTime_Analog((current_time & 0xFF00) >> 8, current_time & 0xFF);
	}
}

/* Alarm On/Off and - Button */
void PB3_handler(void){
	
	/* Decrementing the hour of alarm/currentTime */
	if((set_time == 1) || (set_alarm == 1)){
		uint16_t time_copy = set_time ? current_time : alarm_time;
		
		if(((time_copy & 0xFF00) >> 8) == 0x00){
			time_copy |= 0x1700;
		}
		else{
			time_copy -= 0x100;
		}
		
		if(mode && set_time){
			SetTime_Digital((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else if (!mode && set_time){
			SetTime_Analog((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else{
			Display_Alarm((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		
		set_time ? (current_time = time_copy) : (alarm_time = time_copy);
	}
	
	/* Decrementing the minute of alarm/currentTime */
	else if((set_time == 2) || (set_alarm == 2)){
		uint16_t time_copy = set_time ? current_time : alarm_time;
		
		if((time_copy & 0x00FF) == 00){
			time_copy |= 59;
		}
		else{
			time_copy -= 1;
		}
		
		if(mode && set_time){
			SetTime_Digital((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else if (!mode && set_time){
			SetTime_Analog((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		else{
			Display_Alarm((time_copy & 0xFF00) >> 8, time_copy & 0xFF);
		}
		
		set_time ? (current_time = time_copy) : (alarm_time = time_copy);
	}
	
	/* Turning on/off the alarm */
	else{
		alarm_status ^= 1; 
		Display_Alarm_Status();
	}
	GPIO_PORTB_ICR_R |= 0x0F;				// Clear all flags								
	GPIO_PORTB_IM_R |= 0x0F;				// Rearm everything
}

int main(void){
	PLL_Init(Bus80MHz);
	//testDisplay();
	
	mode = 0;
	current_time = 0x0000;
	alarm_time = 0;
	set_time = 0;
	set_alarm = 0;
	alarm_status = 0;
	
	SoundInit();
	InitDisplay();
	ButtonInit();
	ClockInit();
	EnableInterrupts();
	
	uint16_t previousTime = 0xFFFF;
	uint8_t previousAlarmS = 0xFF;
	
	while(1){
		if((alarm_time == current_time) && (alarm_status == 1)){	// Current time hits alarm time and alarm is turned ON
			AlarmOn();
		}
		if(alarm_status == 0){
			AlarmOff();
		}
		
		/*
		if(alarm_status != previousAlarmS){	// if alarm status changed, display new status
			Display_Alarm_Status();
		}
		*/
		if(current_time != previousTime){	// If time changed since last time, display the new time
			if(mode){
				SetTime_Digital((current_time & 0xFF00) >> 8, current_time & 0xFF);
			}
			else{
				SetTime_Analog((current_time & 0xFF00) >> 8, current_time & 0xFF);
			}
		}
		
		if(button_value == 1){
			button_value = 0;
			PB0_handler();
		}
		if(button_value == 2){
			button_value = 0;
			PB1_handler();
		}
		if(button_value == 4){
			button_value = 0;
			PB2_handler();
		}
		if(button_value == 8){
			button_value = 0;
			PB3_handler();
		}
		if(button_value == 16){
			button_value = 0;
			PF4_handler();
		}
		
		previousTime = current_time;
		//previousAlarmS = alarm_status;
	}
}
