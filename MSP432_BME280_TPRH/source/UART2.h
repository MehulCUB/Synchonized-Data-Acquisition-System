/*--------------------------------------------------------------------------------------------------
 * UART2.h
 *
 * File brief  : UART2.h
 * Implementation of 2nd UART on MSP432
 *
 *      Author: mehul
 *      Created on: Apr 28, 2021
 *--------------------------------------------------------------------------------------------------
 */
#ifndef SOURCE_UART2_H_
#define SOURCE_UART2_H_

#include "stdint.h"

/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void Init_UART2(void);


/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
char getch_u2(void);
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void putch_u2(char c);

/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void send_stringu2(char*s);

/*
 * Function : software delay
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void sdelay(int n);


#endif /* SOURCE_UART2_H_ */
