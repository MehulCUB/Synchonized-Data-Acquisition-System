/*----------------------------------------------------------------------------------------------------
 * TPM.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *  File brief : Initialization / configuration of TPM modules TPM0 and TPM1
 *--------------------------------------------------------------------------------------------------- */
#include "TPM.h"
#include "sysclock.h"
#include "MKL25Z4.h"
#include "stdint.h"
#include "LOG.h"


#define XTAL_FREQ			(8000000u) 	// 8 MHz xtal FRDM-KL25Z has an on-board
#define XTAL_PDIVIDE		(4u)		// divide by 4 (8 MHz --> 2 MHz)
#define XTAL_PUPSCALE		(24u)		// scale up by 24 (2 MHz --> 48 MHz)
#define CORE_CLOCK			(XTAL_FREQ/XTAL_PDIVIDE*XTAL_PUPSCALE) // Hz

#define PWM_PERIOD 			(48000)		//Period for Full PWM Brighness
#define FULL_ON 			(PWM_PERIOD)
#define FULL_OFF			(0)


/*
 * Function :
 *  Initializes the TPM module for LED PWM control
 *
 * 	Parameters  :
 *  			 None
 * 	Returns :
 *  	 		 None
 */
void InitTPM()
{
	// CLock Gating TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;

	//set clock source for tpm: 48 MHz
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	//load the counter and mod
	TPM0->MOD = PWM_PERIOD-1;
	TPM2->MOD = PWM_PERIOD-1;

	// Prescalar set to 1, no division
	TPM0->SC |= (TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));
	TPM2->SC |= (TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));

	// Set channel 1 to edge-aligned low-true PWM
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	// Clock prescaler is 7 (PWM clock devided by 128)
	// This makes PWM clock as 48000000/128 = 375000 Hz (375Khz)
	TPM0->SC |= TPM_SC_PS(7);
	TPM2->SC |= TPM_SC_PS(7);

	// Setting Initial Duty cycle to 0
	TPM2->CONTROLS[0].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;
	TPM0->CONTROLS[1].CnV = 0;
}
