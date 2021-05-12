/*--------------------------------------------------------------------------------------------------
 * UART1.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief : UART 1 Initialization & function for communication over UART1
 *
 *  Reference/Source: https://github.com/alexander-g-dean/ESF/tree/master/NXP/
 *
 *  --------------------------------------------------------------------------------------------------
 */
#ifndef UART1_H_
#define UART1_H_

#include "MKL25Z4.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "LOG.h"
#include "sysclock.h"
#include "cbfifo.h"

#include "UART0.h"
#include "MKL25Z4.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/*
 * Initializing the UART2 for BAUD_RATE: 115200, Data Size: 8, Parity: None, Stop Bits: 2
 *
 * Parameters:
 *   baud_rate: uint32_t for the requested baud rate
 *
 * Returns:
 *   void
 */
void Init_UART1();

/*
 * Function :
 * 		Reads one character from the serial connection UART 1 and Returns it.
 * Returns :
 *  	-1 if no data is available to be read.
 * */
int readchar_U1(void);

/*
 * Function  : receive nbytes  UART
 * Parameters:
 *  	: str 	 : Pass string onto which
 * 		: nbytes : requesting nbytes
 *
 * Returns no of bytes received i.e. successfully dequeued bytes
 **/
size_t UART1_Rx_string(void* str, size_t nbytes);



/*
 * Function : transmit one character over UART 1
 *
 * Parameters:
 * 		enqueue one character to be Transmit onto Tx circular buffer
 * Returns:
 *   void
 **/

int UART1_Tx_char(char ch);


/*
 * Function : Transmits String over to UART
 * Parameters:
 * 	 str	:  Pass string pointer which you want to Transmit over UART
 *	 nbytes : The Length of the String to transmit
 *
 * Returns:
 *   void
 */
void UART1_Tx_string(const char* str, size_t nbytes);
#endif /* UART1_H_ */
