/*----------------------------------------------------------------------------------------------------------------
 * 
 * RTC.c
 *
 *  Created on: May 6, 2021
 *      Author: Mehul
 * ----------------------------------------------------------------------------------------------------------------
 */


#include "rtc.h"
#include "UART0.h"

#define S_YEAR  0x2021		    //2021
#define S_DATE  0x06			// 6
#define S_MONTH 0x05			//May

#define S_DOW 	0x04			// Day of week = 0x04 = thursday
#define S_HOUR 	0x10			// time hour  = 10 am

#define S_MINUTES  0x00
#define S_SECONDS  0x00
	
/*
* TODO: set time by setting RTC registers using user input 
*/	

/*
 * Function : Converts bcd data to decimal equivalent
 *
 * Parameters : None
 *
 * Returns  : decimal converted integer 
 *
 */
uint8_t BCD2DEC(uint8_t bcd_data)
{
    uint8_t temp;
    temp = (bcd_data & 0xF);
    temp = temp+((bcd_data >> 4) * 10);

   return temp;
}



/*
 * Function : Initialize RTC - real time clock 
 *
 * Parameters :
 *
 * Returns  : None
 *
 */ 
void Init_RTC()
{
    /* RTC_C Key Value for RTC_C write access */
    RTCCTL0_H = RTCKEY_H;

    /* Real-time clock time event interrupt enable */
    RTCCTL0_L |= RTCTEVIE;

    /* Real-time clock time event interrupt flag */
    RTCCTL0_L &= ~(RTCTEVIFG);

    /* Real-time clock BCD select */
    /* Real-time clock hold */
    RTCCTL13 = RTCBCD | RTCHOLD ;

    
	set_rtc_time();
	
    /* Start RTC In Calender Mode */
    RTCCTL1 &= ~(RTCHOLD);

    /* Lock the RTC Register after initialization */
    RTCCTL0_H= 0;
}

/*
 * Function : Set RTC - real time clock Timing and Date registers
 *            change macros  defined Set RTC time time to current date and time
 * Parameters : None
 *
 * Returns  : None
 *
 */
void set_rtc_time()
{
    //change macros  defined Set RTC time time to current date and time
    
	//Year value 
	RTC_C->YEAR = S_YEAR ;
	
	//Date - month and date 
    RTC_C->DATE = (S_MONTH<< RTC_C_DATE_MON_OFS)| (S_DATE | RTC_C_DATE_DAY_OFS);

    // Day of week & Hour
    RTC_C->TIM1 = (S_DOW<< RTC_C_TIM1_DOW_OFS)| (S_HOUR << RTC_C_TIM1_HOUR_OFS);

    // Minute, Seconds 
    RTC_C->TIM0 = (S_MINUTES << RTC_C_TIM0_MIN_OFS)| (S_SECONDS << RTC_C_TIM0_SEC_OFS);
}

/*
 * Function : Calculates the RTC time and print time
 *
 * Parameters : None
 *
 * Returns  : None
 *
 */ 
void print_rtc_time()
{
	char rtctime[100];

	//variables for reading RTC data
    int date,month,year;
    int hour,min,sec;

    //read date, month , year
    date  = BCD2DEC(RTCDAY);    //Day
    month = BCD2DEC(RTCMON);    //Month
    year  = BCD2DEC(RTCYEAR);   //Year

    //read Hour,Min,Sec
    hour = BCD2DEC(RTCHOUR);    //Hour
    min  = BCD2DEC(RTCMIN);     //min
    sec  = BCD2DEC(RTCSEC);     //sec

    // time in  "HH:MM:SS   MM:DD:YY"format
    sprintf(rtctime,"\n\r%02d:%02d:%02d     %02d/%02d/%02d   \n\r",hour,min,sec,date,month,year);
    puts_u0(rtctime);
}
