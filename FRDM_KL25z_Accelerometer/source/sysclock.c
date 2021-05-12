/*-------------------------------------------------------------------------------------------------------------
 * sysclock.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 * File Brief:
 *   Reference :  https://learningmicro.wordpress.com/configuring-device-clock-and-using-systick-system-tick-timer-module-to-generate-software-timings
 *------------------------------------------------------------------------------------------------------------
 */

#include "MKL25Z4.h"
#include "TPM.h"
#include "sysclock.h"


#define SYSCLOCK_FREQUENCY (48000000U)

void sysclock_init()
{
	// MCG_C1: CLKS (bit 7-6) = 00
	MCG->C1 |= MCG_C1_CLKS(0);  // Select PLL/FLL as clock source

	// MCG_C1: IREFS (bit 2)  = 1
	MCG->C1 |= MCG_C1_IREFS(1); // Select Internal Reference clock
								// source for FLL

	// MCG_C4: DRST_DRS (bit 6-5) = 01
	MCG->C4 |= MCG_C4_DRST_DRS(1); // Select DCO range as Mid range
								   // DCO generates the FLL Clock

	// MCG_C4: DMX32 (bit 7) = 1
	MCG->C4 |= MCG_C4_DMX32(1);    // Select DCO frequency as 48Mhz
}
