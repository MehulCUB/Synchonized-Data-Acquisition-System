/*--------------------------------------------------------------------------------------------------
 * cli.h
 *
 * Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief  : Header file for different interafce funcion for command line processing interface
 *--------------------------------------------------------------------------------------------------
 */
#ifndef SOURCE_CLI_H_
#define SOURCE_CLI_H_

/*
 * Handling Command Line Interface between UART and user (terminal)
 *
 * Parameters:
 *           void
 * Returns:
 *          void
 */
int manage_cli(void);
/*
 * function which handles the coordination between UART and Command line interface
 *
 * Parameters:
 *           void
 * Returns:
 *           void
 */
int Cli(void);

/*
 * Parse Command entered over UART and print appropriate result
 *
 * Parameters:
 *          String to Parse
 * Returns:
 *          None
 */
void process_cmd(char *str);



#endif /* SOURCE_CLI_H_ */
