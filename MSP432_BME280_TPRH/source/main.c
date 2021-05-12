/**----------------------------------------------------------------------------------------------------------------
*  Synchronized Data Acquisition System    
*
*     Author : Mehul Patel
*
*     University of Colorado Boulder
*---------------------------------------------------------------------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------------------------------------------------------------------
 * Hardware : Using MSP432P401R development board
 *
 * I have implemented 
	UART 0, with circular buffer implementation
	UART 1
	I2C driver 
	MSP432 BME 280 sensor interfacing 
	UART command processor and
	sending the data over UART
	---------------------------------------------------------------------------------------------------------------------------------------
*/
/*---------------------------------------------------------------------------------------------------------------------------------------
 * I have used and modified  bme280 sensor file for project requirement
 *
 * Environmental sensor with pressure, ambient temperature and humidity
 * I have used modified version BME280 sensor support files, as it required to have compensatated values for accurate readings
 *
 * I verified  temperature, ambient pressure and humidity readings as follow
 *
 * temperature used hot water, ice bag - ice cubes near by
 * for humidity blowed humid air from mouth
 * we verify pressure value with Your smart phone weather app
 * ---------------------------------------------------------------------------------------------------------------------------------------
 */

/*
 * File brief  : main.c
 *
 *      Author: mehul
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <string.h>  
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>



#include "app_mode.h"
#include "bme280_support.h"
#include "cli.h"
#include "demo_sysctl.h"
#include "i2c_driver.h"
#include "RTC.h"
#include "stdio.h"
#include "system_clock.h"
#include "systick.h"
#include "UART0.h"
#include "UART2.h"

#define CPU_FREQ                    (48000000)


/*
 *  BME Sensor connection
 *      I2C_SCL(P6.5)
 *      I2C_SDA(P6.4)
*/


/*
 * Function :   main function for calling initialization routines
 *					and switching to application mode
 * 
 * Parameters : None
 *
 * Returns  : None
 *
 */

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //  // Stop WDT and
	
    MAP_Interrupt_disableMaster();  //disabling master interrupts

    // Configure clocks
    startCrystalOscillator();
    setSystemClock(CPU_FREQ);
	
	
    Init_UART0();		// Initialize UART 0 
    Init_UART2();		// Initialize UART 2
    Init_RTC();			// Initialize RTC 
	Init_SysTick();		// Initialize Systick timer 
	
    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
	
    volatile uint32_t index;
	
    // Enabling SRAM Bank Retention
    SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKEN_BNK7_EN;
	
    for (index = 0;index < 100;index++);
	
	Init_I2C();    			// Initialize i2c
	
    MAP_Interrupt_enableMaster();
	
    Application_mode();
}



