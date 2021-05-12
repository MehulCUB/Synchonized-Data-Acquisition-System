/*----------------------------------------------------------------------------------------------------------------
 * RTC.h
 *
 *  Created on: May 6, 2021
 *      Author: Mehul 
 * ----------------------------------------------------------------------------------------------------------------
 */

#ifndef SOURCE_RTC_H_
#define SOURCE_RTC_H_

#include "stdint.h"
#include "stdio.h"
#include <ti/devices/msp432p4xx/inc/msp432.h>



/*
 * Function : Initialize RTC - real time clock 
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void Init_RTC();


/*
 * Function : Set RTC - real time clock Timing and date registers 
 *
 * Parameters : None
 *
 * Returns  : None
 *
 */

void set_rtc_time();

/*
 * Function : Calculates the RTC time and generates it in the string format
 *
 * Parameters : None
 *
 * Returns  : None
 *
 */
 

void print_rtc_time(void);



#endif /* SOURCE_RTC_H_ */
