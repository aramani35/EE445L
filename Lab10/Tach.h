#include <stdint.h>
//#include "tm4c123gh6pm.h"
#include "Timer.h"
 

/** Tachometer_Init() **
 * Activate the Tachometer for Input Loop.
 * Outputs: none
 */
void Tachometer_Init(void);

/** Tachometer_Read() **
 * Reads the latest period measurement with 0.1rps resolution
 * Outputs: latest period value
 */
uint32_t Tachometer_Read(void);