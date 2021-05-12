/* ----------------------------------------------------------------------------------------------------
 * Author  :  Mehul Patel
 * Term    :  Spring 2021
 * Org     :  University of Colorado Boulder
 *--------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------
 * Project brief :
 *
 *		  -> Digital Angle Gauge using KL25Z using on-board MMA8451Q accelerometer
 * 		  -> MMA8451Q accelerometer over I2C to implement the logic for a digital angle gauge
 * 		  	  which will change the LED colors in response to the device orientation.
 * 		  -> calculate roll and pitch using  MMA8451Q
 * 		  -> capacitive touch tap to calibrate the 0° setting
 * 		  -> circular buffer for UART
 * 		  -> circular buffer testing for UART
 * 		  -> UART2
 * 		  -> to enter into command line mode - press 'c'
 * 		  -> LED color change implementation
 *        -> Changes the LED colors in response to the device orientation
 *        -> PWM module control
 *--------------------------------------------------------------------------------------------------*/




#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "MKL25Z4.h"
#include "systick.h"
#include "math.h"
#include "assert.h"
#include "UART0.h"
#include "UART1.h"
#include "i2c.h"
#include "led.h"
#include "TPM.h"
#include "cap_touch.h"
#include "LOG.h"
#include <Init_MMA.h>
#include <MMA8451Q.h>
#include <appmode.h>
int main(void)
{
	sysclock_init(); // Initialize Core Clock

    Touch_Init();

	Init_UART0();	//Initialize UART0

    Init_SysTick(); //initialize systick timer
   	delay_ms(500);

   	InitTPM();		//Initialize PWM on LED Ports
   	delay_ms(500);

   	Init_LED();		//initialize the RGB led
   	delay_ms(500);

   	I2C_Init();		//initialize the I2C bus
   	delay_ms(500);
	
	Init_MMA8451Q(); // initialize the Sensor
	delay_ms(1000);
	

	LOG("\n\rAccelerometer based angle gauge on Frdm-KL25z\n\r\n\r\n\r");								delay_ms(100);
	LOG("\n\rOrient/ rotate frdmkl25z to see change in angle, print log  roll & pitch angles");			delay_ms(100);
	LOG("\n\rTouch Capacitive slider To calibrate\n\r");												delay_ms(100);
	LOG("\n\r Or   Press C to go to Command line mode\n\r");											delay_ms(100);
	LOG("\n\rFrdm-KL25z constantly sending data on UART2\n\r");											delay_ms(100);
	
	Init_UART1();		//Initialize UART1
	
    application_mode();  //switch to application_mode



    return 0 ;
}
