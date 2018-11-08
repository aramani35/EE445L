// main.c
// Runs on LM4F120/TM4C123
// UART runs at 115,200 baud rate 
// Daniel Valvano
// May 3, 2015

/* This example accompanies the books
  "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

"Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
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

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


#include <stdint.h> // C99 variable types
//#include "ADCSWTrigger.h"
#include "ADCT0ATrigger.h"
#include "calib.h"
#include "ST7735.h"
#include "uart.h"
#include "PLL.h"


extern int count;
int old_point = 0;;

int main(void){ int32_t data;
	DisableInterrupts();
  PLL_Init(Bus80MHz);   // 80 MHz
  UART_Init();              // initialize UART device
	ST7735_InitR(INITR_REDTAB);
	ADC0_InitTimer0ATriggerSeq3(9, 80000);
	
	EnableInterrupts();

  while(1){
		ST7735_SetCursor(0, 0);
		int currentTemperature = ConvertToTemperature(ADC_FIFO_CurrentValue());
		ST7735_OutString("Temperature: ");
		ST7735_OutUDec(currentTemperature / 100);
		ST7735_OutString(".");
		ST7735_OutUDec(currentTemperature % 100);
		ST7735_SetCursor(0, 2);

//	for(int i = 0; i < FIFO_SIZE; i += 1) {	
//		int adc_data = 128 - ConvertToTemperature(ADC_FIFO_Get()[i]) * 2 / 100;
//		ST7735_PlotLine(adc_data); 
//		ST7735_PlotNext();
//	}
		
		for(int i = 0; i < FIFO_SIZE; i += 1) {
			ST7735_DrawPixel(i+1, old_point+1, ST7735_BLACK);
			ST7735_DrawPixel(i+1, old_point, ST7735_BLACK);
			ST7735_DrawPixel(i, old_point+1, ST7735_BLACK);
			ST7735_DrawPixel(i, old_point, ST7735_BLACK);
			int32_t point = 128 - ConvertToTemperature(ADC_FIFO_Get()[i]) * 2 / 100;
			old_point = point;
			ST7735_DrawPixel(i+1, point+1, ST7735_RED);
			ST7735_DrawPixel(i+1, point, ST7735_RED);
			ST7735_DrawPixel(i, point+1, ST7735_RED);
			ST7735_DrawPixel(i, point, ST7735_RED);
		}
		WaitForInterrupt();
	}
	
}




//int main(void){ int32_t data;
//  PLL_Init(Bus80MHz);   // 80 MHz
//  UART_Init();              // initialize UART device
////  ADC0_InitSWTriggerSeq3_Ch9();
//	ADC0_InitTimer0ATriggerSeq3(9, 80000);
//	EnableInterrupts();

//  while(1){
////    data = ADC0_InSeq3();
////    UART_OutString("\n\rADC data =");
////    UART_OutUDec(data);
////		UART_OutString("\n\rADC data = ");
//		if (count >=100){
//			DisableInterrupts();
//			break;
//		}
//  }
//}


