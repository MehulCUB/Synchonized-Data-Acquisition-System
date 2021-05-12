/*--------------------------------------------------------------------------------------------------
 * UART2.c
 *
 *
 * File brief  : implementation of 2nd UART on MSP432
 *
 *      Author: mehul
 *      Created on: Apr 28, 2021
 *--------------------------------------------------------------------------------------------------
 */
#include <source/UART2.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <string.h>
#include <stdio.h>
#include "UART0.h"
#include "app_mode.h"


// UART 2 on MSP432
// Pins
// RX P3.2 -> TX
// TX P3.3 -> RX


volatile int c = 0;

char st[100]={'\0'};
volatile int i;
volatile int t=0;


extern int cli_mode_on;
extern int tx_msg_flag;


//Function : UART 2 initlizaiton function
void Init_UART2(void)
{
	// Configure Port 1 pins to select UART module
	// RX : P1.2
	// TX : P1.3
	// Configure port
	P3->SEL0 |= (BIT3 | BIT2);
	P3->SEL1 &= ~(BIT3 | BIT2);

	// Reset device before configuration
	EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;

	// UART Clock = SMCLK (low-speed subsystem master clock) = 3MHz
	EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SSEL__SMCLK | EUSCI_A_CTLW0_SWRST;

	//EUSCI_A2->BRW = 78;   	//9600
	//EUSCI_A2->BRW = 50;     	//14400
	//EUSCI_A2->BRW = 24;		//28800
	//EUSCI_A2->BRW = 13;		//for 57600

	EUSCI_A2->BRW = 6;      //for Baudrate = 115200

	// MCTLW
	//   BRF = 4  (bits 7-4)
	//   BRS = 0  (bits 15-8)
	//   OS16 = 1 (bit0)
	EUSCI_A2->MCTLW = EUSCI_A_MCTLW_OS16 | (4<<EUSCI_A_MCTLW_BRF_OFS);

	// clear reset to enable UART
	EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

	// reset flags (RX=0, TX=1) and enable interrupts
	EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;
	EUSCI_A2->IFG |=  EUSCI_A_IFG_TXIFG;

	//EUSCI_A0->IE |= EUSCI_A_IE_RXIE | EUSCI_A_IE_TXIE;
	EUSCI_A2->IE |= EUSCI_A_IE_RXIE;

	__NVIC_EnableIRQ(EUSCIA2_IRQn);
}

/*
 * Function :  UART2 IRQ handler
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void EUSCIA2_IRQHandler()
{
    if(EUSCI_A_IFG_TXIFG & EUSCI_A2->IFG)
    {
        EUSCI_A2->IFG &= ~(EUSCI_A_IFG_TXIFG);
    }
	
    if(EUSCI_A_IFG_RXIFG & EUSCI_A2->IFG) // rx interrupt
    {
        char read;		
        EUSCI_A2->IFG &= ~(EUSCI_A_IFG_RXIFG);
        read= getch_u2();
		
        if((cli_mode_on==0) && (tx_msg_flag==0))
        {
            if((read !='\n'))
            {
                st[i++] = read;
            }
            else
            {
                st[i++]='\n';
                st[i++]='\r';
                st[i++]='\0';
                if(st[0]=='#')
                {
                    puts_u0("\n\rMsg:");
                    puts_u0(&st[1]);
                    delay();
                    delay();
                }
                else
                {
                    puts_u0("\n\rRX Data:");
                    puts_u0(&st[0]);
                }
                puts_u0("\n\r");
                memset(st,'\0',sizeof(st));
                i= 0;
            }
        }
    }
}

/*
 * Function : read data over uart 2
 *
 * Parameters :
 *
 * Returns  : None
 *
 */ 
char getch_u2(void)
{
    char ch = EUSCI_A2->RXBUF;
    putch_u2(ch);
    return ch;
}
/*
 * Function : to send a char on u2
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void putch_u2(char c)
{
    while(EUSCI_A2->STATW & BIT0);
    EUSCI_A2->TXBUF = c;
}

/*
 * Function : software delay funciton
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
 /*
 * Function :  send string on u2
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void send_stringu2(char*s)
{
    for(;*s;s++)
    {
        putch_u2(*s);
        sdelay(1);
    }
}
/*
 * Function : software delay 
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void sdelay(int n)
{
    int k;
    for(k=0;k<1000;k++)
    {
        __asm(" NOP");
    }
}
