/*--------------------------------------------------------------------------------------------------
 * cap_touch.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief : header file for function interface for capacitive Touch Module  initialization
 * 				 and touch scan function to detect Touch slider
 * --------------------------------------------------------------------------------------------------
 */

#include "cap_touch.h"

#define TOUCH_OFFSET 600 //550 							// offset value to be subtracted
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)				//macro for extracting the count from data register



/*
 * This function initializes touch sensor of FRDMKL25Z board
 * Parameters
 * 			: none
 * Returns
 * 			: none
 */
void Touch_Init()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; 					// enabling the clock

	TSI0->GENCS = TSI_GENCS_MODE(0u) | 					//operating in non-noise mode
								TSI_GENCS_REFCHRG(0u) |	//reference oscillator charge and discharge value 500nA
								TSI_GENCS_DVOLT(0u) |	//oscillator voltage rails set to default
								TSI_GENCS_EXTCHRG(0u) | //electrode oscillator charge and discharge value 500nA
								TSI_GENCS_PS(0u) |		// frequency clcok divided by one
								TSI_GENCS_NSCN(31u) |	//scanning the electrode 32 times
								TSI_GENCS_TSIEN_MASK |	//enabling the TSI module
								TSI_GENCS_EOSF_MASK;	// writing one to clear the end of scan flag
}

/*
 * This function scans touch slider capacitance value
 * 	Parameters
 * 			: None
 * 	Returns
 * 			:returns scanned capacitance value
 */

int Touch_Scan_LH(void)
{
	unsigned int scan = 0, scan_val=0;
	TSI0->DATA = TSI_DATA_TSICH(10u);
	TSI0->DATA |= TSI_DATA_SWTS_MASK; 				//software trigger to start the scan
	while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK )) 	// waiting for the scan to complete 32 times
	;
	scan = TOUCH_DATA;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; 			//writing one to clear the end of scan flag
	scan_val = scan - TOUCH_OFFSET;
	if(scan_val <= 100 || scan_val >= 1000 )
		return 0;
	else
		return 1;
}
