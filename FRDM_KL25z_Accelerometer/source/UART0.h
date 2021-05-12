/*---------------------------------------------------------------------------------------------------
 * UART0.h
 *
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief: UART 0 Initialization & function for communication over UART0
 * ---------------------------------------------------------------------------------------------------
 */

#ifndef UART0_H_
#define UART0_H_



#include "MKL25Z4.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "LOG.h"
#include "sysclock.h"
#include "cbfifo.h"
		

/*
 * Initializing the UART0
 *
 *
 * Returns:
 *   		void
 */
void Init_UART0(void);


/*
 * Transmits String over to UART
 *
 * Parameters:
 *   		str: String to Transmit over UART
 *	 		count: The Length of the String to transmit
 * Returns:
 *   		void
 */
void Send_String(const void* str, size_t count);


/*
 * Receive the Data from UART to Receive Buffer to store
 *
 * Parameters:
 *  		 str: String to Transmit over UART
 *	 		count: The Length of the String to transmit
 * Returns:
 *  		 void
 */
size_t Receive_String(void* str, size_t count);



#endif /* UART0_H_ */
