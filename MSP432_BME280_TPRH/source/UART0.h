/*--------------------------------------------------------------------------------------------------
 *  UART0.h
 * 
 * File brief  : UART0 
 *
 *  Created on: Apr 25, 2021
 *      Author: mehul
 *--------------------------------------------------------------------------------------------------
 */
#ifndef SOURCE_UART0_H_
#define SOURCE_UART0_H_

#include "cbfifo.h"


/*
 * Function :  init uart
 * Initializes the EUSCI A0 UART and UART circular cbuffer - i.e FIFOs
 *
 * Parameters :
 *
 * Returns  :
 *
 */
void Init_UART0(void);




/* uart_putc
 * Function : Send a character directly out the UART
 *
 * Parameters :
 *
 * Returns  : None
 *
 */

void uart0_putch(char c);

/*
Function: uart_send
    Queues a character for UART transmission
*/
/*
 * Function : send one char
 *
 * Parameters :
 *
 * Returns  : None
 *
 */

void putch_u0(char c);

/*
 * Function :   send  - ueues a null-terminated string for UART transmission
 *
 * Parameters :
 *
 * Returns  : None
 *
 */

void puts_u0(char *str);



#endif /* SOURCE_UART0_H_ */
