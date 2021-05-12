/*--------------------------------------------------------------------------------------------------
 *
 * File brief  : demo_sysctl.c
 *
 * demo_sysctl.c
 */
//****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include "demo_sysctl.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//*****************************************************************************
//
//! Provides a small delay.
//!
//! \param ui32Count is the number of delay loop iterations to perform.
//!
//! This function provides a means of generating a delay by executing a simple
//! 3 instruction cycle loop a given number of times.  It is written in
//! assembly to keep the loop instruction count consistent across tool chains.
//!
//! It is important to note that this function does NOT provide an accurate
//! timing mechanism.  Although the delay loop is 3 instruction cycles long,
//! the execution time of the loop will vary dramatically depending upon the
//! application's interrupt environment (the loop will be interrupted unless
//! run with interrupts disabled and this is generally an unwise thing to do)
//! and also the current system clock rate and flash timings (wait states and
//! the operation of the prefetch buffer affect the timing).
//!
//! For better accuracy, the ROM version of this function may be used.  This
//! version will not suffer from flash- and prefect buffer-related timing
//! variability but will still be delayed by interrupt service routines.
//!
//! For best accuracy, a system timer should be used with code either polling
//! for a particular timer value being exceeded or processing the timer
//! interrupt to determine when a particular time period has elapsed.
//!
//! \return None.
//
//*****************************************************************************
//
// For CCS implement this function in pure assembly.  This prevents the TI
// compiler from doing funny things with the optimizer.
//
#if defined(__TI_ARM__)
__asm("    .sect \".text:SysCtlDelay\"\n"
      "    .clink\n"
      "    .thumbfunc SysCtlDelay\n"
      "    .thumb\n"
      "    .global SysCtlDelay\n"
      "SysCtlDelay:\n"
      "    subs r0, #1\n"
      "    bne.n SysCtlDelay\n"
      "    bx lr\n");
#endif


// #define DelayMs(ulClockMS) {}

//Function :

void DelayMs(uint32_t ulClockMS)
{
	if (ulClockMS == 1)
	{
		ulClockMS = 2;
	}
	SysCtlDelay((ulClockMS/2)*(CS_getMCLK() / (3 * 1000)));
}

