#include <stdint.h>

#ifndef __TIMERSINTS_H__ // do not include more than once
#define __TIMERSINTS_H__

void Timer0A_Init100HzInt();
void checkAlarmTime();
void printDigitalTime(int sec, int min, int hr);
void printAlarmTime(void);

// ***************** Timer1_Init ****************
// Activate Timer1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init(uint32_t period);

#endif // __TIMERSINTS_H__
