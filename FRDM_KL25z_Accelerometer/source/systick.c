/*----------------------------------------------------------------------------------------------------
 * systick.c
 *
 *  File brief : Systick timer to manage the timing calculation for different functionalities in the project
 *  Created on: April 29, 2021
 *
 *      Author: mehul
 *----------------------------------------------------------------------------------------------------
 */

#include "systick.h"
volatile ticktime_t Timer_Tick;
volatile ticktime_t g_program_start;
volatile ticktime_t state_timer_start_tick;


#define COUNTER_PER_TICK_VALUE 2999999
 /*
  *
  *  Calculation for SysTick->LOAD count per tick
  *  processor clock is at 48MHz
  *  to get tick at 1/16th of second = 62.5ms = 16Hz
  *  SysTick->LOAD = (48Mhz / 16 Hz) - 1 = 2999999
  */
//function initializes the systick timer - with 62.5ms tick time
void Init_SysTick(void)
{
	SysTick->LOAD = (COUNTER_PER_TICK_VALUE);	//load number counter per tick value 2999999

	NVIC_SetPriority(SysTick_IRQn, 3); 			// NVIC Interrupt Priority // 3
	NVIC_ClearPendingIRQ(SysTick_IRQn); 		// Clear Pending IRq's
	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick->VAL = 0; 							// Clear Timer value

	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | 	// Mask to Initialize TIcks,
						SysTick_CTRL_ENABLE_Msk | //Enable CTRL Mask
						SysTick_CTRL_CLKSOURCE_Msk ;  // use Processer CLock Source of 48 Mhz


	Timer_Tick = 0; //main clock => 0 = start ticks from 0
	g_program_start = state_timer_start_tick = 0;
}

//returns time in multiple of 1/16 of second since start of system program
ticktime_t now()
{
	return Timer_Tick - g_program_start;
}

//load value of state timer start time for state timing calculations
void reset_timer()
{
	state_timer_start_tick = Timer_Tick;
	//load the state timer state_timer_start_tick value for reference for calculation of timing
}

//returns - number of tick of state  -no of ticks from starting from particular state
ticktime_t get_timer()
{
	return (Timer_Tick - state_timer_start_tick);
}

// SysTick interrupt handler, update the main tick value at every interrupt
void SysTick_Handler()
{
	Timer_Tick++; 	//main system timer  - incremented at every 1/16 seconds  - ticking at 1/16 second
}

//delay_ms function to get delay in nmsec
void delay_ms(uint32_t nmsec)
{
	uint32_t delay_Start_tick = Timer_Tick;
	while (((Timer_Tick - delay_Start_tick)*62.5) < nmsec);
}
