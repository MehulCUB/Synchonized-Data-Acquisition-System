/*----------------------------------------------------------------------------------------------------
 * systick.h
 *
 *  File brief : Systick timer to manage the timing calculation for different functionalities in the project
 *  Created on: April 29, 2021
 *
 *      Author: mehul
 *----------------------------------------------------------------------------------------------------
 */

 
#include "MKL25Z4.h"
#include "stdio.h"
#include "stdint.h"
#include "sysclock.h"
#include "LOG.h"



// tick at every 62.5ms => frequency 16 Hz
typedef uint32_t ticktime_t;


/*
 * fn:systick timer initialization function
 *  Parameter:
 * 		none
 *  Returns  : none
 */
void Init_SysTick(void);


/*
 * fn:get time in terms of tick - multiple of 1/16 of second since start of system program
 *  Parameter:
 * 		none
 *  Returns  : time in multiple of 1/16 of second since start of system program
 */

ticktime_t now();

/*
 * fn:load value of state timer start time for state timing calculations
 *  Parameter:
 * 		none
 *  Returns  :
 *		time in multiple of 1/16 of second since start of system program
 */

void reset_timer();

 /*
 * fn:number of tick of state  no of ticks from starting from particular state
 *  Parameter:
 * 		none
 *  Returns  :
 *		no of ticks from starting from particular state
 */
ticktime_t get_timer();

  /*
 * fn: delay_ms function to get delay in nmsec
 *
 * 			    as timer tick is in multiple of 62.5ms,  it will generate exact delay for nmsec in multiples of 62.5ms,
 * 			    otherwise it will give approx ms delay
 *  Parameter:
 *   		  nmsec : n miliseconds
 *
 *  Returns  : none
 */
void delay_ms(uint32_t nmsec);
