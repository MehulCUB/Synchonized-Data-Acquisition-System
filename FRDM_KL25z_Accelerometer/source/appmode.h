/*-------------------------------------------------------------------------------------------------------------
 * appmode.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *  File brief :  Header file for interface for function for swtiching
 *  			  between the applicaiton mode between calibration, commandline mode and Angle gauge mode
 *
 *-------------------------------------------------------------------------------------------------------------
 */

#ifndef APPMODE_H_
#define APPMODE_H_


#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "stdint.h"
#include "stdbool.h"


/*
 * Function : calibration of accelerometer 	 ​
 *
 * ​ Parameters: ​ none
​ *
​ * ​ ​Returns: ​ none
 */

void calibrate_accelerometer(void);

/*
 * Function :  Application mode to switch between states
 * 			To switch between modes
 * 				Digital Angle gauge with controlling RGB LED
 * 				Calibrate
 * 				Touch Based calibration  	 ​
 *
 * ​ Parameters: 
 *		 ​ none
​ *
​ * ​ ​Returns: 
 *		 ​ none
 */
void application_mode(void);

#endif /* APPMODE_H_ */
