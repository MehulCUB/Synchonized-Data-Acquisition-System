/*--------------------------------------------------------------------------------------------------
 * cli.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 * File Brief: function for comamnd line input handling and processing
 *--------------------------------------------------------------------------------------------------
 */
#include <appmode.h>
#include "cli.h"
#include "string.h"
#include "stdint.h"
#include "systick.h"
#include "UART0.h"
#include "UART1.h"

extern cbfifo_t Tx_buff0, Rx_buff0;
extern volatile int tx_msg_flag;
extern int volatile cli_mode_on;

extern int log_frq_div;

typedef void (*command_handler_t)(const char* cmd);
typedef struct
{
	const char* head;
	command_handler_t func_name;
} command_lookup_t;


/*
 * Calibrate the accelerometer
 *
 * Parameters
 * 		: command
 * Returns
 * 		: None
 */
static void calibrate(const char* cmd)
{
	delay_ms(1000);
	printf("Calibrating accelerometer");
	calibrate_accelerometer();
	delay_ms(1000);
	return;
}




/*
 * Calibrate the accelerometer
 *
 * Parameters
 * 		: command
 * Returns
 * 		: None
 */
static void himsg(const char* cmd)
{
	delay_ms(500);


	char str[30] = "Hi,From frdm board\n\r";
	UART1_Tx_string(str, strlen(str)); 	//Transmits String over to UART2


	printf("Sending msg");
	delay_ms(1000);
	return;
}
 /*
 * If unknown prints unknown command
 *
 * Parameters
 * 		: command
 *
 * Returns
 * 	: None
 */
static void unknown_cmd(const char* cmd)
{
	printf("Unknown Command: %s\r\n", cmd);
	printf("? ");
}

/*
 * Help menu for all the commands available
 *
 * Parameters
 *		 : command
 * Returns
 *
 *	 	: None
 */

static void h_calibrate(void)
{
	printf("\n\rcalibrate:\n\r");
	printf("To Calibrate accelerometer at specific angle\n\r");
	printf("Hold accelerometer at specific angle and Enter 'calibrate'\r\n");
}


static void h_himsg(void)
{
	printf("\n\rtxmsg:\n\rSend Hi msg from frdm to MSP \n\r");
}


static void h_logfreq(void)
{	
	printf("\n\rlogfreq:\n\rTo change no of log per second angle gauge\n\r");
}

static void h_tcmd(void)
{
	printf("\n\rwhen out of the commadline mode press 'T'or 't' and enter tx msg to send the message\n\r");
}

static void help(const char* cmd)
{
	printf("Help menu\n\r");
	h_calibrate();
	h_himsg();
	h_logfreq();
	//h_tcmd();
	printf("? ");
}


/*
 * function to change/set logging  frequency of the angle gauge in number of log msg per second 
 * 
 */
static void logfreq(const char* cmd)
{
	uint32_t freq;
	if(sscanf(cmd,"logfreq %i", &freq) == 1)
	{
		if(freq<0)
		{
			printf("Please Enter freq properly\r\n");
			return;
		}
		log_frq_div = (int) (16/freq);
	}
	else
	{
		printf("Please Enter command properly\n\r");
		printf("Command Usage: logfreq <per sec>\r\n");
		
		
	}
}


static const command_lookup_t cmds[] =
{
	{"calibrate", calibrate},
	{"himsg",himsg},
	{"logfreq",logfreq},
	{"help", help},	
	{"", unknown_cmd}
	//to add new command - add entry in lookup table and define function
};


/*
 * Parse rx string over UART and send process command
 *
 * Parameters :
 *  	str : String to Parse
 *
 * Returns
 * 		: None
 */

char s_buff[640];
void process_cmd(char * input)
{
    char* p = input;
    char* end;
    int j=0;
    char s[256]={0};
	char* argv[10];
	int argc = 0; //no of arg
	char cmd_head[25] = {0};


	for (end = input;* end != '\0'; end++)
		;

    memset(argv, 0, sizeof(argv)); // set initalize all pointer to 0
    memset(&s[0], 0, 256);

	int space_taken_flag = 1;

	for (p = input; p < end; p++)
	{
		if(*p == '\r' ||*p == '\n' )
		{
			argv[argc] = &s_buff[0];
			strcpy(argv[argc],s);
			memset(&s[0], 0, sizeof(s));
			argc++;
			j=0;
		}

		if(*p != ' ')
		{
			s[j] = *p;
			space_taken_flag = 0;
			j++;
		}
		else
		{
			if (space_taken_flag == 1)	//if input string got space
			{
			  p++;
			}
			else
			{
			  s[j]=' ';
			  space_taken_flag = 0;
			  j++;
			}
		}
	}

	argv[argc] = NULL;
	if (argc == 0)   // no command
		return;


	sscanf(argv[0],"%s",&cmd_head[0]);
	int num = sizeof(cmds) / sizeof(command_lookup_t);

	for(int i=0; i<num-1; i++)
	{
		if(strcmp(cmd_head, cmds[i].head) == 0)
	    if(strcmp(cmd_head, cmds[i].head) == 0)
		{
			cmds[i].func_name(argv[0]); //call the function using function lookup table
			return; 					// function command is successfully executed return to take new command
		}
	}
	unknown_cmd(argv[0]);
	memset(&s_buff[0], '\0', sizeof(s_buff));
	return;
}
/*
 * Command Line Interface  handling function mcu UART and cmd-terminal
 *		
 * Parameters:
 * 		  void
 * Returns:
 *  	 void
 */
#define MAX_IP_STRING_SIZE 256
int manage_cli(void)
{
	char si[MAX_IP_STRING_SIZE] = {0};

	memset(si,'\0',MAX_IP_STRING_SIZE);

	char *s = &si[0];

	char ch = 0;

	while(ch != '\r')				 //take input till receive carriage return
	{
		if(ch == '\r')
		{
			break;
		}
		while(check_cbfifo_empty(&Rx_buff0))
			;

		cbfifo_dequeue(&Rx_buff0,&ch,1);	//dequeue one by one character
		putchar(ch); 						//echo the character

		// start transmitter if it isn't already running
		if (!(UART0->C2 & UART0_C2_TIE_MASK))
		{
			UART0->C2 |= UART0_C2_TIE(1);
		}

		if((ch!= '\r') || (ch!='\0'))
		{
			if(ch == '\b') 				// Back space 0x08
			{
										//here printf \b will remove last displayed char delete the one character
				printf(" \b");			//so after one white space give \b will
										//for next input take one character back
				s--;
			}
			else
			{
				*s++ = (char)ch; 		// take input
			}
		}
		if(ch == '\r' ||ch == '\n')
		{
			ch = '\n';
			printf("\n\r");
			*s = '\0'; 					//put null at the end and send accumulated line for command processing
			break;
		}
	}
	if(tx_msg_flag==1)
	{


		char sc[200];

		sprintf(sc,"#\n\rMsg from frdm board : %s\n\r\n\r",si);


		UART1_Tx_string(sc,strlen(sc));
		printf("Sent Msg : %s\n\r",si);
	}
	else
	{
		process_cmd(si);
	}

	memset(&si[0], '\0', sizeof(si));
	s = &si[0]; 			//reset to start from string

	cli_mode_on =0;
	printf("\r\n");

	putchar('\0');
	putchar('\r');
	putchar('\n');
	return 0;
}
/*
 * function to enter into command line interface mode
 *
 * Parameters:
 *   		void
 * Returns:
 *  		 void
 */
void Cli()
{
	int flag_not_done  = 1;
	while(flag_not_done)
	{
		flag_not_done = manage_cli();
	}
}
