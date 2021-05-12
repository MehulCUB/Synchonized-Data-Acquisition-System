/* ----------------------------------------------------------------------------------------------------
 * Author  :  Mehul Patel
 * Term    :  Spring 2021
 * Org     :  University of Colorado Boulder
 * Topic 	: Final Project
 *--------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------
 * appmode.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *  File brief :  function for swtiching between the applicaiton mode calibration,
 *  			  commandline mode and Angle gauge mode
 *----------------------------------------------------------------------------------------------------------
 */

#include <appmode.h>
#include <Init_MMA.h>
#include <MMA8451Q.h>
#include "assert.h"
#include "TPM.h"
#include "systick.h"
#include "math.h"
#include "cli.h"
#include "i2c.h"
#include "led.h"
#include "cap_touch.h"
#include "UART0.h"




#define MMA_ISR_PIN 14
#define ROLL_PITCH_RANGE 180

double cal_roll=0, cal_pitch=0;
mma8451q_acc_t acc;
int volatile cli_mode_on = 0;
int volatile tx_msg_flag = 0;

//Function : calibration of accelerometer
void calibrate_accelerometer(void)
{
	read_full_xyz(&acc);		// Convert acc to Roll and Pitch

	convert_xyz_to_roll_pitch(&acc, &cal_roll, &cal_pitch);
	LOG("\n\r Calibrated Roll = %d, Pitch = %d\n\r",(int)cal_roll, (int)cal_pitch);

	LED_White();				// BLINKING WHITE LEDs to indicate change in calibration
	delay_ms(200);

	LED_Off();
	delay_ms(200);

	LED_White();
	delay_ms(200);

	LED_Off();
	return;
}
//Application mode to switch between states To switch between modes		
void application_mode(void)
{
  mma8451q_acc_t acc; 			// To hold Acceleration data
  MMA8451Q_InitializeData( &acc);	// Sets it to default which is Zero
  
  
  LOG("\n\r---------------------------------------------------------------------\n\r");	delay_ms(100);
  LOG("\n\r---------------------------------------------------------------------\n\r");	delay_ms(100);
  LOG("\n\r		Initializing Digital Angle gauge 							  \n\r");	delay_ms(100);
  LOG("\n\r		Rotate device to view Change in Pitch and Roll				  \n\r");	delay_ms(100);
  LOG("\n\r---------------------------------------------------------------------\n\r");	delay_ms(100);
  LOG("\n\r---------------------------------------------------------------------\n\r");	delay_ms(100);
  LOG("\n\rPress 'm'or 'M' and enter and send msg to MSP boar 		\n\r");				delay_ms(100);
  LOG("\n\rPress 'C'or 'c' and and enter the commands				\n\r");				delay_ms(100);
  //enter tx msg to send the message

  LED_Off();

	while (1)
	{
			if(tx_msg_flag==1)
			{
				LOG("\n\rSend msg mode::\n\r");
				printf("Enter msg to send to MSP:");
				cli_mode_on= 1;
				Cli();
				cli_mode_on = 0;
				tx_msg_flag = 0;
			}
			if( cli_mode_on==1)
			{
				tx_msg_flag = 0;
				LOG("\n\rCommand mode\n\r");
				printf("? ");
				delay_ms(1000);
				Cli();
				delay_ms(2000);
				cli_mode_on = 0; // return
			}

			else if(Touch_Scan_LH())
			{
				LOG("\rCapcitive touch detected - calibrating accelerometer\n\r");
				LOG("\rHold accelerometer at angle You want to Calibrate\n\r");
				delay_ms(5000);
				calibrate_accelerometer();
				delay_ms(5000);
			}

			

			Anglegauge_LED(&acc);
	}
}
