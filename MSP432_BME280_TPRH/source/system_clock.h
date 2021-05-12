/*----------------------------------------------------------------------------------------------------------------
 *
 * system_clock.h
 * File brief  : system_clock.h
 *
 *      Author: mehul
 *----------------------------------------------------------------------------------------------------------------
 */
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "i2c_driver.h"
#include "demo_sysctl.h"


#ifndef SOURCE_SYSTEM_CLOCK_H_
#define SOURCE_SYSTEM_CLOCK_H_

/*
 * Function :  The following function is responsible for starting XT1 in the
   MSP432 that is used to source the internal FLL that drives the
   MCLK and SMCLK.
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void startCrystalOscillator(void);
/*
 * Function : The following function is responsible for setting up the system
   clock at a specified frequency.
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void setSystemClock(uint32_t CPU_Frequency);





#endif /* SOURCE_SYSTEM_CLOCK_H_ */
