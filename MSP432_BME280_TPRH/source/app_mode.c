/*---------------------------------------------------------------------------------------------------
 * app_mode.c
 *  File brief : to switch between different application mode, like command line, trasnmit message, ..
 *
 *  Created on: May 3, 2021
 *      Author: Mehul
 *
 * ---------------------------------------------------------------------------------------------------
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

int tx_msg_flag =0;
int cli_mode_on =0;
int time_print_on=0;
bool farenheit_on =0;

//variable for For reading  BME280 values
s32 returnRslt;
s32 g_s32ActualTemp   = 0;
u32 g_u32ActualPress  = 0;
u32 g_u32ActualHumity = 0;


void time_r(void);

//Function : software delay
void delay(void)
{
    int i=500000;
    while(i--)
    {
        __asm(" nop");
    }
}



void Application_mode(void)
{

    // Initialize bme280 sensor
    bme280_data_readout_template();
    returnRslt = bme280_set_power_mode(BME280_SLEEP_MODE);
    returnRslt = bme280_set_power_mode(BME280_NORMAL_MODE);



     while(1)
     {
         if(time_print_on)
         {
             print_rtc_time();
         }

         if(cli_mode_on)
         {
             puts_u0("\r\n#Command Processor Mode#\n\r");
             puts_u0("Enter command starting with \n\r @, e.g.@help,@unit\n\r");
             puts_u0("?");
             cli_mode_on = Cli();
         }
         if(tx_msg_flag)
         {
             puts_u0("\r\nEnter message you want to send leading with '$'\n\r");
             puts_u0("\n\rE.g. '$Hi \n\r");
             puts_u0("\n\rE.g. '$Hi,Frdm-board \n\r");
             tx_msg_flag = Cli();
         }

         read_tprh();
         delay();
         delay();
         delay();
         delay();
         delay();
     }
}


/*
 * Function : Read temperature, pressure and humidity values and print the log
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void read_tprh(void)
{
    static int ci =0;
	
    returnRslt = bme280_read_pressure_temperature_humidity(&g_u32ActualPress, &g_s32ActualTemp, &g_u32ActualHumity);
    float rh = ((float)g_u32ActualHumity/1024);
    float pr = (((float)g_u32ActualPress*2.95)/10000);  // to convert into  inhg
    float tmp = ((float)g_s32ActualTemp/100);
	
	
    char st[120];
    if(farenheit_on==1)
    {
        float tmpF = (tmp * 9 / 5) + 32; //conversion : fahrenheit = (celsius * 9 / 5) + 32;

        sprintf(st,"$humid :%.02f %s  press:%.02f inHg   temp:%.02f F \n\r", rh,"%",pr,tmpF);
    }
    else
    {
        sprintf(st,"$humid :%.02f %s  press:%.02f inHg   temp:%.02f C \n\r", rh,"%",pr,tmp);
    }

    puts_u0(&st[1]);
	
    ci++;
    if((ci%5==0))
    {
        send_stringu2(st);
    }
	
    memset(&st[0],0,sizeof(st));
}
