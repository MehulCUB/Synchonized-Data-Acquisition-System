/*--------------------------------------------------------------------------------------------------
 * cap_touch.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief :  Implementation of function for capacitive Touch Module  initialization
 * 				 and touch scan function to detect Touch slider
 *
 * --------------------------------------------------------------------------------------------------
 */

#ifndef CAP_TOUCH_H_
#define CAP_TOUCH_H_


#include "board.h"
#include "pin_mux.h"
#include "MKL25Z4.h"

/*
 * This function initializes touch sensor of FRDMKL25Z board
 * Parameters
 * 			: none
 * Returns
 * 			: none
 */

int Touch_Scan_LH(void);

/*
 * This function scans touch slider capacitance value
 * 	Parameters
 * 			: None
 * 	Returns
 * 			:returns scanned capacitance value
 */
void Touch_Init(void);

#endif /* CAP_TOUCH_H_ */
