// UARTTestMain.c
// Runs on LM4F120/TM4C123
// Used to test the UART.c driver
// Daniel Valvano
// May 30, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1
#include <stdint.h>
#include <String.h>
#include "PLL.h"
#include "UART.h"
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none

#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value
#define TRUE										1
#define FALSE										0

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000;
  //NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
	NVIC_ST_CTRL_R = 0x07;
}

// SysTick Interupt Handler
void SysTick_Handler(void) {
	unsigned char rx_data;
	unsigned char c;
//	PF2 ^= 0x04;
//	c = 0;
//	rx_data = ((char)(UART3_DR_R&0xFF));//UART3_DR_R;
//	c=rx_data;
//	if (c==0) {
//		PF3 ^= 0x08;
//	}
//	if (c=='a')
//		PF1 ^= 0x02;
//	c = UART_InChar();
//	if (c=='a')
//		PF1 ^= 0x02;
}

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

void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}
//debug code
int main(void){
  char ch;
	char prev = '*';
	char prevString[20] = "afhuapf";
  char string[20];  // global to assist in debugging
  uint32_t n = 0;
	uint32_t prevn = 0;
	
	PortF_Init();
  PLL_Init(Bus80MHz);       // 50  MHz
  ST7735_InitR(INITR_REDTAB);
	ST7735_FillScreen(ST7735_BLACK); 
	ST7735_SetCursor(0,0);
	UART_Init();              // initialize UART
	SysTick_Init();
	ST7735_SetCursor(0,0);
	
  unsigned char rx_data;
	int c;
	c=0;
	for (int i=0; i<20; i++) {
		string[i] = prevString[i];
	}
  while(1){
//		n = UART_InUDec();
//		if(n != prevn) {
//			ST7735_SetCursor(0,0);
//			ST7735_OutUDec(n);
//			prevn = n;
//		}
    UART_InString2(string,19);
		if (strcmp(string,prevString) != 0) {
			ST7735_SetCursor(0,0);
			ST7735_OutString(string);
			for (int i=0; i<20; i++) {
				prevString[i] = string[i];
			}
		}
//		ch = UART_InChar();
//		ST7735_SetCursor(0, 0);
//		ST7735_OutUDec(ch);
		
		
		
  }
}

void UART3_Handler(void) {
	int u=0,i=0,lu=0,ru=0;
	unsigned char rx_data;
	unsigned char c;
	PF3= 0x08;
	while(1){
		UART3_INT_CLEAR_R &= ~(0X010);
		rx_data = UART3_DR_R;
		c=rx_data;
//		for (int i=0; i<100000; i++){}
//			PF2 ^= 0x04;
//		if (c=='a') {
			
		
	}
}
