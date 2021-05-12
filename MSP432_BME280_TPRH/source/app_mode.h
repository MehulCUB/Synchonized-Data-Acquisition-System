/*--------------------------------------------------------------------------------------------------
 * app_mode.h
 *   File brief : header File for interface of functionality
 *                to switch between different application mode,
 *                          like command line, transmit message, ..
 *
 *  Created on: May 3, 2021
 *      Author: Mehul
 *--------------------------------------------------------------------------------------------------
 */

#ifndef SOURCE_APP_MODE_H_
#define SOURCE_APP_MODE_H_


/*
 * Function : software delay function    ​
 *
 * ​ Parameters: ​ none
​ *
​ * ​ ​Returns: ​ none
 */

void delay(void);


/*
 * Function :  Application mode to switch between different mode 
 * 
 *   To switch between modes command line , transmit message, or simply print the log of the sensor data
 *
 * ​ Parameters: ​ none
​ *
​ * ​ ​Returns: ​ none
 */
 
void Application_mode(void);


/*
 * Function : Read temperature, pressure and humidity values and print the log
 *
 * Parameters : none
 *
 * Returns  : None
 *
 */
void read_tprh(void);

#endif /* SOURCE_APP_MODE_H_ */
