// -------------------------------------------------------------------
// File name: Blynk.c
// Description: This code is used to bridge the TM4C123 board and the Blynk Application
//              via the ESP8266 WiFi board
// Author: Mark McDermott and Andrew Lynch (Arduino source)
// Converted to EE445L style Jonathan Valvano
// Orig gen date: May 21, 2018
// Last update: Sept 20, 2018
//
// Download latest Blynk library here:
//   https://github.com/blynkkk/blynk-library/releases/latest
//
//  Blynk is a platform with iOS and Android apps to control
//  Arduino, Raspberry Pi and the likes over the Internet.
//  You can easily build graphic interfaces for all your
//  projects by simply dragging and dropping widgets.
//
//   Downloads, docs, tutorials: http://www.blynk.cc
//   Sketch generator:           http://examples.blynk.cc
//   Blynk community:            http://community.blynk.cc
//
//------------------------------------------------------------------------------

// TM4C123       ESP8266-ESP01 (2 by 4 header)
// PE5 (U5TX) to Pin 1 (Rx)
// PE4 (U5RX) to Pin 5 (TX)
// PE3 output debugging
// PE2 nc
// PE1 output    Pin 7 Reset
// PE0 input     Pin 3 Rdy IO2
//               Pin 2 IO0, 10k pullup to 3.3V  
//               Pin 8 Vcc, 3.3V (separate supply from LaunchPad 
// Gnd           Pin 4 Gnd  
// Place a 4.7uF tantalum and 0.1 ceramic next to ESP8266 3.3V power pin
// Use LM2937-3.3 and two 4.7 uF capacitors to convert USB +5V to 3.3V for the ESP8266
// http://www.ti.com/lit/ds/symlink/lm2937-3.3.pdf
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "PLL.h"
#include "Timer2.h"
#include "Timer3.h"
#include "UART.h"
#include "PortF.h"
#include "esp8266.h"
#include "PWM.h"
#include "tach.h"

void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

uint32_t LED;      // VP1
uint32_t LastF;    // VP74


uint32_t Period =100000000;				// 24-bit 12.5 ns units
int32_t Done;						// mailbox status set each rising
int32_t kp1, kp2, ki1, ki2, I, desired_speed;	// variables needed for control equation


// These 6 variables contain the most recent Blynk to TM4C123 message
// Blynk to TM4C123 uses VP0 to VP15
char serial_buf[64];
char Pin_Number[2]   = "99";       // Initialize to invalid pin number
char Pin_Integer[8]  = "0000";     //
char Pin_Float[8]    = "0.0000";   //
uint32_t pin_num; 
uint32_t pin_int;
 
// ----------------------------------- TM4C_to_Blynk ------------------------------
// Send data to the Blynk App
// It uses Virtual Pin numbers between 70 and 99
// so that the ESP8266 knows to forward the data to the Blynk App
void TM4C_to_Blynk(uint32_t pin,uint32_t value){
  if((pin < 70)||(pin > 99)){
    return; // ignore illegal requests
  }
// your account will be temporarily halted if you send too much data
  ESP8266_OutUDec(pin);       // Send the Virtual Pin #
  ESP8266_OutChar(',');
  ESP8266_OutUDec(value);      // Send the current value
  ESP8266_OutChar(',');
  ESP8266_OutString("0.0\n");  // Null value not used in this example
}
 
 
// -------------------------   Blynk_to_TM4C  -----------------------------------
// This routine receives the Blynk Virtual Pin data via the ESP8266 and parses the
// data and feeds the commands to the TM4C.
void Blynk_to_TM4C(void){int j; char data;
// Check to see if a there is data in the RXD buffer
  if(ESP8266_GetMessage(serial_buf)){  // returns false if no message
    // Read the data from the UART5
#ifdef DEBUG1
    j = 0;
    do{
      data = serial_buf[j];
      UART_OutChar(data);        // Debug only
      j++;
    }while(data != '\n');
    UART_OutChar('\r');        
#endif
           
// Rip the 3 fields out of the CSV data. The sequence of data from the 8266 is:
// Pin #, Integer Value, Float Value.
    strcpy(Pin_Number, strtok(serial_buf, ","));
    strcpy(Pin_Integer, strtok(NULL, ","));       // Integer value that is determined by the Blynk App
    strcpy(Pin_Float, strtok(NULL, ","));         // Not used
    pin_num = atoi(Pin_Number);     // Need to convert ASCII to integer
    pin_int = atoi(Pin_Integer);  
		
		// check pin_int for button press
    if(pin_num == 0x2 && pin_int){				// VP 2: KP1
			kp1 = pin_int;
    }
    
    else if (pin_num == 0x3 && pin_int){	// VP 3: KP2
			kp2 = pin_int;
    }
    
    else if(pin_num == 0x4 && pin_int){		// VP 4: KI1
			ki1 = pin_int;
    }
    
    else if (pin_num == 0x5 && pin_int){	// VP 5: KI2
			ki2 = pin_int;

    }
		else if (pin_num == 0x6 && pin_int){	// VP 6: Desired speed
			desired_speed = pin_int;
		}
		else if (pin_num == 0x7 && pin_int){
			// tau here
		}
		
		
  // ---------------------------- VP #1 ----------------------------------------
  // This VP is the LED select button
    if(pin_num == 0x01)  {  
      LED = pin_int;
      PortF_Output(LED<<2); // Blue LED
#ifdef DEBUG3
      Output_Color(ST7735_CYAN);
      ST7735_OutString("Rcv VP1 data=");
      ST7735_OutUDec(LED);
      ST7735_OutChar('\n');
#endif
    }                               // Parse incoming data        
#ifdef DEBUG1
    UART_OutString(" Pin_Number = ");
    UART_OutString(Pin_Number);
    UART_OutString("   Pin_Integer = ");
    UART_OutString(Pin_Integer);
    UART_OutString("   Pin_Float = ");
    UART_OutString(Pin_Float);
    UART_OutString("\n\r");
#endif
  }  
	TM4C_to_Blynk(77, (200000000/Period)/10);
}

void SendInformation(void){
  uint32_t thisF;
  thisF = PortF_Input();
// your account will be temporarily halted if you send too much data
  if(thisF != LastF){
    TM4C_to_Blynk(74, thisF);  // VP74
#ifdef DEBUG3
    Output_Color(ST7735_WHITE);
    ST7735_OutString("Send 74 data=");
    ST7735_OutUDec(thisF);
    ST7735_OutChar('\n');
#endif
  }
  LastF = thisF;
}



/***************** main *****************/ 
int main(void){       
  PLL_Init(Bus80MHz);   // Bus clock at 80 MHz
  DisableInterrupts();  // Disable interrupts until finished with inits
  PortF_Init();
  LastF = PortF_Input();
#ifdef DEBUG3
  Output_Init();        // initialize ST7735
  ST7735_OutString("EE445L Lab 4D\nBlynk example\n");
#endif
#ifdef DEBUG1
  UART_Init(5);         // Enable Debug Serial Port
  UART_OutString("\n\rEE445L Lab 4D\n\rBlynk example");
#endif
  ESP8266_Init();       // Enable ESP8266 Serial Port
  ESP8266_Reset();      // Reset the WiFi module
  ESP8266_SetupWiFi();  // Setup communications to Blynk Server  
  
  Timer2_Init(&Blynk_to_TM4C,800000); 
  // check for receive data from Blynk App every 10ms

  Timer3_Init(&SendInformation,40000000); 
  // Send data back to Blynk App every 1/2 second
	
	ST7735_InitR(INITR_REDTAB); 	
	tach_timer_init();
  PWM0B_Init(40000, 30000);
  EnableInterrupts();
	
	// DEFAULT VALUES
	volatile int32_t duty = 0;
	desired_speed = 1023;
	kp1 = 1;
	kp2 = 1;
	ki1 = 1;
	ki2 = 1;
	volatile int old_duty = 0;
	I = 0;
	volatile int current_speed, prev_speed, P, E;
  while(1) {
		
		//Blynk_to_TM4C();
//		volatile int current_speed, P, E;
		prev_speed = current_speed;
		current_speed = 200000000/Period;				//0.1 rps
//		if (current_speed > 200 && current_speed<10000){
		ST7735_SetCursor(0, 3);
		ST7735_OutUDec(current_speed);
		if (current_speed>1000)
			current_speed=1000;
		if (current_speed-200>0)
			current_speed-=200;
		ST7735_OutString("     ");
		ST7735_SetCursor(0, 3);
		
		E = (desired_speed) - current_speed;
		
		ST7735_SetCursor(0, 2);
		ST7735_OutUDec(desired_speed);

		if(E < 0) 
			E *=20;
		
//		ST7735_SetCursor(0, 4);
//		if (E < 0) {
//			ST7735_OutChar('-');
//			int error_s = E * -1;
//			ST7735_OutUDec(error_s);
//		}
//		else {
//			ST7735_OutUDec(E);
//		}
//		ST7735_SetCursor(0, 8);
//		ST7735_OutUDec(kp1);
//		ST7735_SetCursor(0, 9);
//		ST7735_OutUDec(kp2);
//		ST7735_SetCursor(0, 10);
//		ST7735_OutUDec(ki1);
//		ST7735_SetCursor(0, 11);
//		ST7735_OutUDec(ki2);
		
		P = (kp1*E)/(kp2);
		I = I + (ki1*E)/(ki2);
//		if (I<=0) I=1;
		if (P>10000) P = 10000;
		else if (P<-10000) P = -10000;
		
		if (I>10000) I = 10000;
		else if (I<-10000) I = -10000;
//		old_duty = duty;
		duty += ((P + I) >> 9);
//		int change=duty-old_duty;
//		if(duty-old_duty<0){
//			change*=-1;
//		}
//		if(change > 100)
		
		if (duty < 0) {
			duty = 0;
			I=0;
		}
		if(duty >= 40000){
			duty = 39999;
			I = 0;
		}
		

		PWM0B_Duty(duty);		// This is where we adjust the speed
//  }
}
}