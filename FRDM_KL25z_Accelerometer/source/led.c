/*-------------------------------------------------------------------------------------------------
 * led.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *   File brief : Controlling LED using PWM,
 *   			Digital angle gauge function implementaiton and PWM LED control
 *-------------------------------------------------------------------------------------------------
 */

#include "led.h"
#include "MKL25Z4.h"
#include "TPM.h"
#include "LOG.h"
#include "systick.h"

#include "UART1.h"


//To convert Angle Ranges to PWM
#define OLDRANGE (180)
#define NEWRANGE (48000)

#define MASK(x) (1UL << (x))

//LEDs positions 
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)		// on port B
#define BLUE_LED_POS (1)		// on port D


extern double cal_roll, cal_pitch;

#define PWM_RANGE	FULL_BRIGHTNESS
#define FULL_BRIGHTNESS (48000)

#define RED_PWM 	TPM2->CONTROLS[0].CnV
#define GREEN_PWM 	TPM2->CONTROLS[1].CnV
#define BLUE_PWM 	TPM0->CONTROLS[1].CnV

 // Function :  Disables the red LED
#define LED_RedOn()		TPM2->CONTROLS[0].CnV = 0;

 //Function : Enables the red LED
#define LED_RedOff()	TPM2->CONTROLS[0].CnV = FULL_BRIGHTNESS;

 //Function : Enables the green LED
#define LED_GreenOn()	TPM0->CONTROLS[1].CnV = 0;

 //Disables the green LED
#define LED_GreenOff()	TPM0->CONTROLS[1].CnV = FULL_BRIGHTNESS;

 //Function : Enables the blue LED
#define LED_BlueOn()	TPM2->CONTROLS[1].CnV = 0;

//Function : Disables the blue LED
#define LED_BlueOff()	TPM2->CONTROLS[1].CnV = FULL_BRIGHTNESS;



//volatile static int log_frq_div = 4;
int log_frq_div = 4;
void Init_LED()
{
	// Enable clock to ports B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;;

	// Make 3 pins read for TPM/PWM Pins
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(3);
	
	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(4);

	// Set ports to outputs
	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);

	// Initialize to 0
	RED_PWM = 0;
	GREEN_PWM = 0;
	BLUE_PWM = 0;

	LOG("\n\rInitialized PWM functionalities for LED");
}

void LED_Red()
{
	/* Turn on RED LEd,
	 * Turn off Green LED
	 * Turn off Blue LEd*/
	LED_RedOn();
	LED_GreenOff();
	LED_BlueOff();
}

void LED_Green()
{
	/* Turn off Red LEd,
	 * Turn on Green LED
	 * Turn off Blue LEd*/
	LED_RedOff();
	LED_GreenOn();
	LED_BlueOff();
}

void LED_Blue()
{
	/* Turn off RED LEd,
	 * Turn off Green LED
	 * Turn on Blue LEd*/
	LED_RedOff();
	LED_GreenOff();
	LED_BlueOn();
}


void LED_White()
{
	/* Turn on All LEd */
	LED_RedOn();
	LED_GreenOn();
	LED_BlueOn();
}

void LED_Off()
{
	/* Turn off LEd */
	LED_RedOff();
	LED_GreenOff();
	LED_BlueOff();
}

/*
 * Function : Digital Angle Gauge mode to print angle value and Control RGB LED
 *   	Digital Angle Gauge to print angle value and Control RGB LED
 *   	here using roll angle and Controlling PWM value LED based on based on roll
 *  Parameters :
 * ​			 mma8451q_acc_t *acc: Pointer to MMA8451Q based structure
​ *
​ * ​ Returns :
 * ​ 		none
 */
void Anglegauge_LED(mma8451q_acc_t *acc)
{
	double roll = 0.0,  pitch = 0.0;
	static int log_num,  prev_log_num =0;
	static int send_log_num = 0;
	static int prev_send_log_num = 0;

	read_full_xyz(acc);								// Read Acceleration Data from MMA8451Q
	convert_xyz_to_roll_pitch(acc, &roll, &pitch);	// Convert acceleration data to Roll and Pitch

	// Convert 0-90 degree (Pitch and Roll) to PWM Range of (0-48000)

	roll = roll - cal_roll;
	pitch= pitch - cal_pitch;

	//detect and write angle for specific 30 60 90 ...
	int roll_v = (int)roll;
	int w_f= 0;
	switch(roll_v)
	{
		case  30 :
		case -30 :

				RED_PWM = 0;
				GREEN_PWM = PWM_RANGE;
				BLUE_PWM =0;
				w_f= 1;
				break;


		case  45 :
		case -45 :
				RED_PWM = 0;
				GREEN_PWM = PWM_RANGE;
				BLUE_PWM = PWM_RANGE;
				w_f= 1;
				break;
		case 60 :
		case -60 :
				RED_PWM = PWM_RANGE;
				GREEN_PWM = 0;
				BLUE_PWM =0;
				w_f= 1;
				break;


		case  75 :
		case -75 :
				RED_PWM = PWM_RANGE;
				GREEN_PWM = PWM_RANGE;
				BLUE_PWM =0;
				w_f= 1;
				break;


		case 90 :
		case -90 :
				RED_PWM = PWM_RANGE;
				GREEN_PWM = PWM_RANGE;
				BLUE_PWM =PWM_RANGE;
				w_f= 1;
				break;
		case 120 :
		case -120 :
				RED_PWM = PWM_RANGE;
				GREEN_PWM = 0;
				BLUE_PWM =PWM_RANGE;
				w_f= 1;
				break;
		case  150 :
		case -150 :
				RED_PWM = 0;
				GREEN_PWM = 0;
				BLUE_PWM =PWM_RANGE;
				w_f= 1;
				break;

		case  180 :
		case -180 :
				RED_PWM = PWM_RANGE;
				GREEN_PWM = PWM_RANGE;
				BLUE_PWM =PWM_RANGE;
				break;
		default :
			RED_PWM = 0;
			GREEN_PWM = 0;
			BLUE_PWM =0;
			break;
	}
	if(w_f==1)
	{
		LOG("\n\rAngle Detected: %d\n\r",roll_v);
	}


	if(now()%(log_frq_div) == 0)
	{
		log_num = (now());
		if(log_num != prev_log_num)
		{
			printf("\n\rRoll: %d 	 Pitch: %d", (int)roll, (int)pitch);
			prev_log_num = log_num;
		}
	}

	if((now()%120) ==0)
	{
		send_log_num=(now());

		if(prev_send_log_num !=send_log_num)
		{
			char stu[100]={'\0'};
			sprintf(stu,"Roll: %d 	 Pitch: %d \n\r", (int)roll, (int)pitch);

			int len = strlen(stu);
			UART1_Tx_string(&stu[0],len);
			prev_send_log_num = send_log_num;
		}
	}
}


