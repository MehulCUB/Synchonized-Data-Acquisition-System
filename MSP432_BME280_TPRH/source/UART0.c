/*--------------------------------------------------------------------------------------------------
 * UART0.c
 *
 * Author: mehul
 *
 * Created on: Apr 25, 2021
 *
 *  File brief  :
 *  brief : MSP432 UART initialization driver using interrupts  and use of UART RX/TX circular  buffers.
 *  The EUSCIA0 device is the UART used /channeled over terminal on PC
 *  --------------------------------------------------------------------------------------------------
 */

#include <stdint.h>
#include <stddef.h>
#include "UART0.h"
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "cbfifo.h"
#include "cbfifo.h"
#include "cli.h"
#include "stdio.h"

#define UART_TX_READY       (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)
#define UART_IRQ_DISABLE    (EUSCI_A0->IE &= ~(EUSCI_A_IE_TXIE|EUSCI_A_IE_RXIE))
#define UART_IRQ_ENABLE     (EUSCI_A0->IE |= EUSCI_A_IE_TXIE|EUSCI_A_IE_RXIE)


cbfifo_t Rx_cbuff0;
cbfifo_t Tx_cbuff0;


extern int cli_mode_on;
extern int tx_msg_flag;
extern int time_print_on;
extern int farenheit_on;
/*
 * Function : EUSCI A0 Initialization function 
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
void Init_UART0(void)
{
	// Configure Port 1 pins to select UART module
	// RX : P1.2
	// TX : P1.3
	P1->SEL0 |= BIT2 | BIT3;
	P1->SEL1 &= ~(BIT2 | BIT3);

	// Reset device before configuration
	EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;

	// UART Clock = SMCLK (low-speed subsystem master clock) = 3MHz
	EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SSEL__SMCLK | EUSCI_A_CTLW0_SWRST;

	//EUSCI_A0->BRW = 78;
	//EUSCI_A0->BRW = 50;     //14400
	//EUSCI_A0->BRW = 24;
	//EUSCI_A0->BRW = 12;   //for57600
	
	EUSCI_A0->BRW = 6;      //115200  

	// MCTLW
	//   BRF = 4  (bits 7-4)
	//   BRS = 0  (bits 15-8)
	//   OS16 = 1 (bit0)
	EUSCI_A0->MCTLW = EUSCI_A_MCTLW_OS16 | (4<<EUSCI_A_MCTLW_BRF_OFS);

	// clear reset to enable UART
	EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

	// reset flags (RX=0, TX=1) and enable interrupts
	EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;
	EUSCI_A0->IFG |=  EUSCI_A_IFG_TXIFG;

	//EUSCI_A0->IE |= EUSCI_A_IE_RXIE | EUSCI_A_IE_TXIE;
	EUSCI_A0->IE |= EUSCI_A_IE_RXIE;

	__NVIC_EnableIRQ(EUSCIA0_IRQn);

	Init_cbfifo(&Tx_cbuff0);
	Init_cbfifo(&Rx_cbuff0);
}

void EUSCIA0_IRQHandler(void)
{
  char data;

  // Received a byte
  if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
  {
      if(cli_mode_on==1 ||tx_msg_flag ==1)
      {
         char c[1];
         c[0] = EUSCI_A0->RXBUF;
         cbfifo_enqueue(&Rx_cbuff0,&c[0],1);
      }
      else if(cli_mode_on==0)
      {
          char cc[1];
          cc[0] = EUSCI_A0->RXBUF;
          if(cc[0]=='c'||cc[0]=='C')
          {
              cli_mode_on =1;
          }
          else if(cc[0]=='m'||cc[0]=='M')
          {
              tx_msg_flag = 1;
          }
          else if(cc[0]=='T'||cc[0]=='t')
          {
              time_print_on = ~time_print_on;
          }
          else if(cc[0]=='F'||cc[0]=='f')
          {
             // farenheit_on = 1; //~farenheit_on;
              farenheit_on = !(farenheit_on);
          }
      }
      else
      {

      }
  }

  // Clear tx interrrupt to send
  if (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)
  {
    if(Tx_cbuff0.curr_length)
    {
      // Send next character, keep interrupt active
      cbfifo_dequeue(&Tx_cbuff0, &data,1);
      EUSCI_A0->TXBUF = data;
    }
    else
    {
      // fifo empty, disable tx interrupt
      EUSCI_A0->IE &= ~EUSCI_A_IE_TXIE;
    }
  }
}
//Function : Send a character directly out the UART
void uart0_putc(char c)
{
  while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
  EUSCI_A0->TXBUF = c;
}

//Function :  get ready for trasnmision UART 0 check if ready and the fifo isn't empty
void ready_u0(void)
{
  char c;
  UART_IRQ_DISABLE;
  if (Tx_cbuff0.curr_length)
  {
    if (UART_TX_READY)
    {
      cbfifo_dequeue(&Tx_cbuff0, &c,1);
      EUSCI_A0->TXBUF = c;
    }
  }
  UART_IRQ_ENABLE;;
}


//Function: uart_queue Queues a character for UART transmission
void putch_u0(char c)
{
  char ch[1]= {c};
  cbfifo_enqueue(&Tx_cbuff0, ch,1);
  ready_u0();
}

//Function : Queues a null-terminated string for UART transmission
void puts_u0(char *str)
{
  while(*str != 0)
  {
    cbfifo_enqueue(&Tx_cbuff0, str,1);
    str++;
  }
  ready_u0();
}

