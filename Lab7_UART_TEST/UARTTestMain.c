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
  char string[20];  // global to assist in debugging
  uint32_t n;
	PortF_Init();
  PLL_Init(Bus50MHz);       // 50  MHz
  ST7735_InitR(INITR_REDTAB);
	ST7735_FillScreen(ST7735_BLACK); 
	ST7735_SetCursor(0,0);
	UART_Init();              // initialize UART
	SysTick_Init();
	
	
  unsigned char rx_data;
	int c;
//  OutCRLF();
//  for(ch='A'; ch<='Z'; ch=ch+1){// print the uppercase alphabet
//    UART_OutChar(ch);
//  }
//  OutCRLF();
//  UART_OutChar(' ');
//  for(ch='a'; ch<='z'; ch=ch+1){// print the lowercase alphabet
//    UART_OutChar(ch);
//  }
//  OutCRLF();
//  UART_OutChar('-');
//  UART_OutChar('-');
//  UART_OutChar('>');
	c=0;
  while(1){
		
		rx_data = (UART3_DR_R&0xFF);//UART3_DR_R;
		c=rx_data;
		if (c>0) {
			ST7735_SetCursor(0,0);
			ST7735_OutUDec(c);
			PF2 ^= 0x04;
		}
		if (c == 9|c==39) {
			PF1=0x01;
		}
		if (c==0){
			PF1=0x01;
		}
		else {
			PF1=0;
		}
		
//			for (int i=0; i<1000000; i++){}
//			PF1 ^= 0x02;
//		rx_data = UART3_DR_R;
//		c=rx_data;
//		if (c=='a'){
//			PF3= 0x08;
//		}
//			n=0;
//			n=UART_InUDec();
////			ch = UART_InChar();
//			if (n > 0) { PF1 ^=0x02; }
		
//    UART_OutString("InString: ");
//    UART_InString(string,19);
//    UART_OutString(" OutString="); UART_OutString(string); OutCRLF();

//    UART_OutString("InUDec: ");  n=UART_InUDec();
//    UART_OutString(" OutUDec="); UART_OutUDec(n); OutCRLF();

//    UART_OutString("InUHex: ");  n=UART_InUHex();
//    UART_OutString(" OutUHex="); UART_OutUHex(n); OutCRLF();

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


/*
Input command codes:
Turn on the light: 	28
Turn off the light:	224
Play music:					252
Turn on the TV:			142	
What's the time:		282
Right:							128
Stop:								240
Mode 2:							280

*/