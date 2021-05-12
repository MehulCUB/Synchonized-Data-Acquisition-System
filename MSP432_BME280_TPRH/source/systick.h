/*----------------------------------------------------------------------------------------------------------------

 * systick.h
 * File brief  : systick.h
 *
 *      Author: mehul
 *
 * implementation of  systick timer and helper functions to calculate state timing
 *----------------------------------------------------------------------------------------------------------------
 */

#ifndef SOURCE_SYSTICK_H_
#define SOURCE_SYSTICK_H_

#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "stdint.h"
#include "stdio.h"
#include "stdint.h"
#include "UART0.h"
#include "UART2.h"

// tick at every 62.5ms => frequency 16 Hz
// As per assignment suggestion
typedef uint32_t ticktime_t;


/*
 * fn:systick timer initialization function
 *  Parameter:
 *      none
 *  Returns  : none
 */


void Init_SysTick(void);


/*
 * fn:get time in terms of tick - multiple of 1/16 of second since start of system program
 *  Parameter:
 *      none
 *  Returns  : time in multiple of 1/16 of second since start of system program
 */


ticktime_t now();

/*
 * fn:load value of state timer start time for state timing calculations
 *  Parameter:
 *      none
 *  Returns  :
 *      time in multiple of 1/16 of second since start of system program
 */


void reset_timer();

 /*
 * fn:
 *  Parameter:
 *      none
 *  Returns  :
 *      no of ticks from starting from particular state
 */

ticktime_t get_timer();

  /*
 * fn: delay_ms function to get delay in nmsec
 *
 *              as timer tick is in multiple of 62.5ms,  it will generate exact delay for nmsec in multiples of 62.5ms,
 *              otherwise it will give approx ms delay
 *  Parameter:
 *            nmsec : n miliseconds
 *
 *  Returns  : none
 */


void delay_ms(uint32_t nmsec);



#endif /* SOURCE_SYSTICK_H_ */
