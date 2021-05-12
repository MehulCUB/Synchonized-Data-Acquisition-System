/*----------------------------------------------------------------------------------------------------
 * TPM.h
 *
 * Created on: May 1, 2021
 *      Author: mehul
 *
 *  File brief : Initialization / configuration of TPM modules TPM0 and TPM1
 *--------------------------------------------------------------------------------------------------- */

#ifndef TPM_H_
#define TPM_H_



/*
 * To Initialize system clock to 48Mhz
 *
 * Parameters:
 * 		void
 * Returns:
 *   void
 */
void InitClock();

/*
 * Function : to Set up TPM0 and TPM
 *
 * Parameters:
 * 		void
 * Returns:
 *   void
 **/
void InitTPM();

#endif /* TPM_H_ */
