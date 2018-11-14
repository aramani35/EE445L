/** hardware connections **
 * DC Motor goes on PB6
 */
 
#include <stdint.h>

// 80MHZ PWM frequencies with /16 divider
#define MOTOR_1Hz 4999999
#define MOTOR_10Hz 499999
#define MOTOR_100Hz 49999
#define MOTOR_500Hz  9999
#define MOTOR_1000Hz 4999
#define MOTOR_5000Hz  999
	
/** Motor_Init() **
 * Activate the Motor for Output Loop.
 * Outputs: none
 */
void Motor_Init(uint32_t _period, uint16_t _min, uint16_t _max);

/** Motor_Start() **
 * Start spinning the motor
 * Kick off the PWM on PB6
 * Motor current will flow.
 */
void Motor_Start(void);

/** Motor_Stop() **
 * Stop the motor
 * Motor current will stop ABRUPTLY
 */
void Motor_Stop(void);

/** Motor_SetDuty() **
 * Update the motor's duty cycle to run at the new speed.
 */
void Motor_SetDuty(uint32_t _duty);

/** Motor_Duty() **
 * Read the motor's duty cycle
 */
uint32_t Motor_Duty(void);

/** Motor_Desired() **
 * Read the motor's desired speed
 */
uint16_t Motor_Desired(void);

/** Motor_Actual() **
 * Read the motor's actual speed
 */
uint16_t Motor_Actual(void);

/** Motor_SetDesired() **
 * Read the motor's desired speed
 */
void Motor_SetDesired(uint16_t _desired);
