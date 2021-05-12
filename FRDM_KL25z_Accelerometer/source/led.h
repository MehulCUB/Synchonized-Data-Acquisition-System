/*-------------------------------------------------------------------------------------------------
 *
 * led.h
 *
 * Created on: May 1, 2021
 *      Author: mehul
 *
 * File: Header file for Instantiation and functionalities for LED to interact with the PWM
 *-------------------------------------------------------------------------------------------------
 */

#ifndef LED_H_
#define LED_H_

#include "MKL25Z4.h"
#include "TPM.h"
#include "mma8451q.h"
#include "systick.h"


/*
 * Function : 
 *
 *  Sets up the GPIOs for LED driving
 *
 */
void Init_LED();

/*
 * Function : 
 *  	Lights the red LED
 *
 *  Parameters : 
 * ​	 	none
​ *
​ * ​ Returns : 
 * ​ 	none
 */
void LED_Red();

/*
 * Function : 
 *  		Lights the Green LED
 *  Parameters : 
 *  ​ 		none
​ *
​ * ​ Returns : 
 * ​ 		none
 */
void LED_Green();

/*
 * Function : 
 *  		Lights the Blue LED
 *  Parameters : 
 *  ​	 	none
​ *
​ * ​ Returns : 
 * ​ 		none
 */
void LED_Blue();

/*
 * Function : 
 *  		Lights all LEDs
 *  Parameters : 
 *  ​	 none
​ *
​ * ​ Returns : 
 * ​ 	none
 */
void LED_White();

/*
 * Function : 
 *  	Disables all LEDs
 *  Parameters : 
 *  ​ 	none
​ *
​ * ​ Returns : 
 * ​ 	none
 */
void LED_Off();


/*
 * Function :
 *   	Digital Angle Gauge to print angle value and Control RGB LED
 *   	here using roll angle and Controlling PWM value LED based on based on roll
 *  Parameters :
 * ​			 mma8451q_acc_t *acc: Pointer to MMA8451Q based structure
​ *
​ * ​ Returns :
 * ​ 		none
 */
void Anglegauge_LED(mma8451q_acc_t *acc);


#endif /* LED_H_ */
