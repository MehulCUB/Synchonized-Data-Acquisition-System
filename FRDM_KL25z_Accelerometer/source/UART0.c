/*---------------------------------------------------------------------------------------------------
 * UART0.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 * 
 * File brief: UART 0 Initialization & function for communication over UART0
 * Usign sysread and write function to tie the printf - stdin/out functions to UART0
 * ---------------------------------------------------------------------------------------------------
 */


#include <appmode.h>
#include "UART0.h"
#include "UART1.h"


#define UART0_BAUDRATE			(115200)
//#define UART0_BAUDRATE		(9600)

#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 				(24000000)
#define SYS_CLOCK				(48000000)

// critical section macro functions
#define START_CRITICAL()	__disable_irq()
#define END_CRITICAL(x)	__set_PRIMASK(x)


extern volatile int cli_mode_on;
extern volatile int tx_msg_flag;
cbfifo_t Tx_buff0, Rx_buff0;

char tx_msgb[512];
volatile int j = 0;
//Initializing the UART0
void Init_UART0(void)
{
	uint16_t sbr;
	uint32_t baud_rate =UART0_BAUDRATE;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx


    /*
	* Baud rate caluclation :  (SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE)
	* SYS_CLOCK 		   : 48000000
	* UART_OVERSAMPLE_RATE : 16
	*/ 
	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0) | UART0_C1_PT(0);

	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// Enable interrupts. Listing 8.11 on p. 234


	Init_cbfifo(&Tx_buff0);
	Init_cbfifo(&Rx_buff0);

	NVIC_SetPriority(UART0_IRQn, 3);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);
}


//Transmits String over to UART
void Send_String(const void* str, size_t count)
{
	cbfifo_enqueue(&Tx_buff0, str, count);

	// start transmitting if it isint already
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
}


//Receive the Data from UART to Receive Buffer to store
size_t Receive_String(void* str, size_t count)
{
	return cbfifo_dequeue(&Rx_buff0, str, count);
}


/*
 * 
 * UART IRQ_Handler
 *
 */
 
 /*
 * Function :
 * 		UART 0 IRQ handler 
 * receive interrupt is raised then
 * 		if command line mode was off
 * 		then enter into command line mode only if 'c' is pressed - ignore  otherwise 
 * 		if command line mode is on enque the data - for UART 0 - terminal ineterface 
 * 		tx char to UART 0 
 * 		
 * for tx tx the data u using tx interrupt 
 * 
 * Parameters:
 *		None 
 * Returns :
 *  	None 
 * */

void UART0_IRQHandler(void)
{

	uint8_t ch;
	// clear the error flags
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
		ch = UART0->D;
	}
	
	//receive interrupt is raised then
	if (UART0 -> S1 & UART0_S1_RDRF_MASK)
	{
	  if(cli_mode_on == 0)			//if command line mode was off 
	  {								//then enter into command line mode only if 'c' is pressed 
		ch = UART0 -> D;			//ignoer otherwise 
		if(ch == 'c'||ch == 'C')
		{
			cli_mode_on = 1;
		}
		else if(ch == 'm' ||ch == 'M')
		{
			tx_msg_flag = 1;
		}
	  }
	  else if((tx_msg_flag ==1) || (cli_mode_on==1))
	  {
		// received a character
		ch = UART0 -> D;
		cbfifo_enqueue( &Rx_buff0, & ch, 1);
	  }					//if command line mode is on enque the data - for UART 0 - terminal ineterface
	}
   
	if( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) )// transmitter interrupt enabled
	{
		if(cbfifo_dequeue(&Tx_buff0, &ch, 1))  //tx char to UART 0 
		{
			UART0->D = ch;
		}
		else
		{
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

/*
 *
 * Function :
 * 		Writes the specified bytes to either stdout (handle = 1) or stderr (handle = 2).
 * Returns 
 * 		:  -1 on error or 0 on success.
 *
 * */
int __sys_write(int handle, char* buffer, int count)
{
	if(buffer == NULL)
	{
		return -1;
	}
	while(check_cbfifo_full(&Tx_buff0))
	{
		; // Wait for the space to openup
	}

	if(cbfifo_enqueue(&Tx_buff0, buffer, count) != count)
	{
		return -1;
	}

	if(!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0;
}
/*
 * Function :
 * 		Reads one character from the serial connection and returns it.
 *
 * Returns: 
 *		-1 if no data is available to be read.
 * */
int __sys_readc(void)
{
	char ch;
	if(cbfifo_dequeue(&Rx_buff0, &ch, 1) != 1)
	{
		return -1;
	}
	return ch;
}


