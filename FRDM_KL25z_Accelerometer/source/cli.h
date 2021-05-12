/*--------------------------------------------------------------------------------------------------
 * cli.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File Brief:header file for function for comamnd line input handling and processing
 *--------------------------------------------------------------------------------------------------
 */

#ifndef CLI_H_
#define CLI_H_

#include "MKL25Z4.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "LOG.h"
#include "sysclock.h"

#include "cbfifo.h"
#include "UART0.h"
/*
 * Handling Command Line Interface between UART and user (terminal)
 *
 * Parameters:
 *  		 void
 * Returns:
 *   		void
 */
int manage_cli(void);
/*
 * function which handles the coordination between UART and Command line interface
 *
 * Parameters:
 *  		 void
 * Returns:
 *  		 void
 */
void Cli(void);

/*
 * Parse Command entered over UART and print appropriate result
 * 
 * Parameters:
 *    		String to Parse
 * Returns:
 *   		None
 */
void process_cmd(char *str);



#endif /* CLI_H_ */
