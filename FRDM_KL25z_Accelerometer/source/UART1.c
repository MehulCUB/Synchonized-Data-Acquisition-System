/*--------------------------------------------------------------------------------------------------
 * UART1.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * 	File Brief: UART 1 Initialization & function for communication over UART1
 *
 *  Reference/Source: https://github.com/alexander-g-dean/ESF/tree/master/NXP/
 *
 *  --------------------------------------------------------------------------------------------------
 */

#include "MKL25Z4.h"
#include "UART0.h"
#include "UART1.h"
#include "sysclock.h"
#include "cbfifo.h"
#include "cli.h"
#include "stdio.h"


/*-----------------------------------------------------------------------------------------
 * Port Pins for UART2 -- connected to RX, TX pins of MSP432 board
 * PORTE.0 E.1
 * PTE0	1	PTE0 UART1_TX
 * PTE1	2	PTE1 UART1_RX
 * Ensure to connect common ground pins first
 * -----------------------------------------------------------------------------------------
 */

#define UART1_TX_PIN (0)		//PORTE.0
#define UART1_RX_PIN (1)		//PORTE.1


volatile char tx_ch= 0;
volatile char rx_ch = 0;

extern volatile tx_msg_flag;
/*
 * use this macros to change the Baud rate, Parity,Stop Bits
 */
#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 				(24e6)
#define SYS_CLOCK				(48e6)

//#define BAUD_RATE_B 			(9600)

#define BAUD_RATE_B 			(115200)
#define PARITY_VALUE_B 			PARITY_NON
#define UART1_OVERSAMPLE_RATE 	(16)

int uflag;


extern int cli_mode_on;


 //Initializing the UART 1
char rs[150];
int i = 0;
void Init_UART1()
{
	uint16_t sbr;
	uint32_t baud_rate = BAUD_RATE_B;

	SIM->SOPT5 = SIM_SOPT5_UART1RXSRC(0);
	SIM->SOPT5 = SIM_SOPT5_UART1TXSRC(0);

	// Enable clock gating for UART1 and Port E
	SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;


	// Make sure transmitter and receiver are disabled before init
	UART1->C2 &= ~UART_C2_TE_MASK & ~UART_C2_RE_MASK;

	// Set UART clock to 24 MHz clock
	SIM->SOPT2 |=  SIM_SCGC4_UART1(1);

	PORTE->PCR[UART1_TX_PIN] &= ~(PORT_PCR_MUX_MASK);  // clear that mask
	PORTE->PCR[UART1_RX_PIN] &= ~(PORT_PCR_MUX_MASK);



	PORTE->PCR[0] = PORT_PCR_MUX(3); // tx
	PORTE->PCR[1] = PORT_PCR_MUX(3); // rx

	// Set baud rate and over sampling ratio

	//over sample rate for UART1  = 16
	sbr = (uint16_t)((SYS_CLOCK)/((baud_rate*2) * UART1_OVERSAMPLE_RATE));
	UART1->BDH &= ~UART_BDH_SBR_MASK;
	UART1->BDH |= UART_BDH_SBR(sbr>>8);
	UART1->BDL = UART_BDL_SBR(sbr);
	UART1->C4 = 0;

	// Disable interrupts for RX active edge and LIN break detect, select stop bit =1
    UART1->BDH |= UART_BDH_RXEDGIE(0) | UART_BDH_SBNS(1) | UART_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
    UART1->C1 = UART_C1_LOOPS(0) | UART_C1_M(0) | UART_C1_PE(0) | UART_C1_PT(0);

	// Don't invert transmit data, don't enable interrupts for errors
	UART1->C3 = UART_C3_TXINV(0) | UART_C3_ORIE(0)| UART_C3_NEIE(0)
			| UART_C3_FEIE(0) | UART_C3_PEIE(0);


	//NVIC_SetPriority(UART1_IRQn, 3); // 0, 1, 2, or 3  // origna;

	NVIC_SetPriority(UART1_IRQn, 2);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART1->C2 |= UART_C2_RIE(1)|UART_C2_TIE(1);

	// Enable UART receiver and transmitter
	UART1->C2 |= UART_C2_RE(1) | UART_C2_TE(1);
}



/*
 * Function :
 * 		UART 1 IRQ handler 
 * for receive 
 * 		Read the char uisng receive interrut 
 * 		handle it for string processing - get/ fill into buffer till receive \n - or \r 
 * 		and print the received data 
 * for tx tx the data u using tx interrupt 
 * 
 * Parameters:
 *				None
 * Returns :
 *  			None
 * */

void UART1_IRQHandler(void)
{
	uint8_t ch;

	// clear the error flags
	if(UART1->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		ch = UART1->D;	// read the data register to clear RDRF
	}

	//receive interrupt is raised then
	if((UART1->S1 & UART_S1_RDRF_MASK))
	{
		rx_ch = UART1->D;	//read rx character from UART1->D
		//printf("%c",rx_ch);
		//UART1_Tx_char(rx_ch); // echo back char  // if u want to echo the charrx

		if((cli_mode_on == 0) && (tx_msg_flag==0))
		{
			if((rx_ch == '\n') ||(rx_ch == '\r'))
			//if((rx_ch == '\r'))
			{
				rs[i++] ='\n';
				rs[i++] ='\r';
				rs[i++] ='\0';
				if(rs[0]=='$')
				{
					printf("\n\r\n\rrx data : %s",&rs[1]);
				}

				memset(rs,'\0',strlen(rs));
				i = 0;
			}
			/*
			else if(rx_ch=='$')
			{
				memset(rs,'\0',strlen(rs));
				i = 0;
			}
			*/
			else
			{
				rs[i++] = rx_ch;
			}
		}
	}

	//tx interrupt
	if((UART1->C2 & UART_C2_TIE_MASK) && // transmitter interrupt enabled
					(UART1->S1 & UART_S1_TDRE_MASK))// tx buffer empty
	{
		if(uflag == 1)
		{
			UART1->D = tx_ch;
			uflag = 0;
			UART1->C2 &= ~UART_C2_TIE_MASK;
		}
		else
		{
			UART1->C2 &= ~UART_C2_TIE_MASK;
		}
	}
}
/*
 * Function : receive a char over UART to get in global char 
 *
 * Parameters:
 * 		none 
 * Returns:
 *   read char 
 **/
char UART1_rx_char(void)
{
	char ch;
	ch = rx_ch;

	return ch;
}


/*
 * Function : transmit one character on UART1
 *
 * Parameters:
 * 		enqueue one character to be Transmit onto Tx circular buffer
 * Returns:
 *   void
 **/ 
int UART1_Tx_char(char ch)
{
	tx_ch = ch;
	uflag = 1;
	//start transmitting if it isn't already
	if(!(UART1->C2 & UART_C2_TIE_MASK))
	{
		UART1->C2 |= UART_C2_TIE(1);
	}
	return 1;
}

/*
 * Function : software delay
 *
 * Parameters:
 * 		none
 * Returns:
 *   none 
 **/
void delay1()
{
	for(int k=0;k<1000;k++)
	{
		__asm("nop");
	}
}


/*
 * Function : Transmits String over to UART2
 *
 * Parameters:
 * 		const char* str: string to transmit
 * 		size_t nbytes :	no of bytes to tx
 * Returns:
 *   	none
 **/
void UART1_Tx_string(const char* str, size_t nbytes)
{
	for(int i=0;i<nbytes;i++)
	{
		UART1_Tx_char(str[i]);
		delay1();
	}
}
