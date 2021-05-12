/*-------------------------------------------------------------------------------------------------
 * LOG.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File Brief:
 * -------------------------------------------------------------------------------------------------
 */

#ifndef LOG_H_
#define LOG_H_


#include "stdio.h"
#include "fsl_debug_console.h"

#ifdef DEBUG
	#define LOG printf  //#define LOG PRINTF
#else
	#define LOG(...)
#endif


#endif /* LOG_H_ */
