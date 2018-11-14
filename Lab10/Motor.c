/** hardware connections **
 * DC Motor goes on PB6
 */
 
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "Timer.h"
#include "Tach.h"

// 80MHZ PWM frequencies with /16 divider
#define MOTOR_1Hz 4999999
#define MOTOR_10Hz 499999
#define MOTOR_100Hz 49999
#define MOTOR_500Hz  9999
#define MOTOR_1000Hz 4999
#define MOTOR_5000Hz  999


// port init for Motor
static void PWM_Init(uint32_t period, uint32_t duty);
static void PortB_Init(void); 

// motor speed
static uint32_t period, duty;
static uint16_t desired, actual; // desired speed

// min and max rps
static uint16_t min, max;
	
/** Motor_Init() **
 * Activate the Motor for Output Loop.
 * Outputs: none
 */
void Motor_Init(uint32_t _period, uint16_t _min, uint16_t _max){
	period = _period;
	min = _min; max = _max;
	duty = period / 2;
	PortB_Init();
	PWM_Init(period, duty);
	Tachometer_Init();
	// interrupt at 10x faster than the time constant
	Timer0A_Init(MOTOR_100Hz * 2, 0x01);
}

/** Motor_Start() **
 * Start spinning the motor
 * Kick off the PWM on PB6
 * Motor current will flow.
 */
void Motor_Start(){
  PWM0_ENABLE_R |= 0x00000001;// enable PB6/M0PWM0
	Timer0A_Start();
}

/** Motor_Stop() **
 * Stop the motor
 * Motor current will stop ABRUPTLY
 */
void Motor_Stop(){
	Timer0A_Stop();
	PWM0_ENABLE_R &= ~0x00000001;  // disable PB6/M0PWM0
}

/** Motor_SetDuty() **
 * Update the motor's duty cycle to run at the new speed.
 */
void Motor_SetDuty(uint32_t _duty){
	if(_duty < period){
		PWM0_0_CMPA_R = _duty;
		duty = _duty;
	}
}

/** Motor_Duty() **
 * Read the motor's duty cycle
 */
uint32_t Motor_Duty(){
  return duty;
}

/** Motor_Desired() **
 * Read the motor's desired speed
 */
uint16_t Motor_Desired(){
  return desired;
}

/** Motor_SetDesired() **
 * Read the motor's desired speed
 */
void Motor_SetDesired(uint16_t _desired){
  desired = _desired;
}

/** Motor_Actual() **
 * Read the motor's actual speed
 */
uint16_t Motor_Actual(){
  return actual;
}

// update motor
#define COEFF  1 / 2
void Timer0A_Handler(){
	// acknowledge
	Timer0A_Acknowledge();
	
	// calculate speed error.
	period = Tachometer_Read();
	actual = 200000000 / period; // speed with 0.1 resolution
	int32_t error = desired - actual;
	
	// update response;
	static int32_t response;
	response = duty + (error * COEFF);
	
	// response filter
	if(response < min){
		response = min;
	} else if(response > max * 99 / 100){
		response = max * 99 / 100;
	} 
	
	if(desired == 0){
		response = 0;
		actual = 0;
	} Motor_SetDuty(response);
}

/** PWM_Init() **
 * period is 16-bit number of PWM clock cycles in one period (3<=period)
 * period for PB6 and PB7 must be the same
 * duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
 * PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
 *                = BusClock/2
 *                = 80 MHz/2 = 40 MHz (in this example)
 * Output on PB6/M0PWM0
 */
static void PWM_Init(uint32_t period, uint32_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      ((SYSCTL_RCC_R & (~0x000E0000)) | 0x00060000);   //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0xC8;                 // PB6 goes low on LOAD and high on CMPA down
  PWM0_0_LOAD_R = period;           // 5) cycles needed to count down to 0
  PWM0_0_CMPA_R = duty;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
}

/** PortB_Init() **
 * Initialize PortB for Motor Output
 * Speaker output on PB6
 * Outputs: none
 */
static void PortB_Init(){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;    		 // turn on port B
	delay = SYSCTL_RCGCTIMER_R; 			 // allow time to finish activating
	GPIO_PORTB_AFSEL_R |= 0x40;        // enable alt funct on PB6
  GPIO_PORTB_PCTL_R &= ~0x0F000000;  // configure PB6 as PWM0
  GPIO_PORTB_PCTL_R |= 0x04000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;       // disable analog functionality on PB6
  GPIO_PORTB_DEN_R |= 0x40;          // enable digital I/O on PB6
}
