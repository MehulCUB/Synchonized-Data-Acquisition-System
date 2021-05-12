/*---------------------------------------------------------------------------------------------------------------
 * cli.c
 *
 * Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief  : command line interface
 *-----------------------------------------------------------------------------------------------------------------
*/


#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp432p4xx/inc/msp432.h>
#include <string.h>
#include <stdlib.h>

#include "UART0.h"
#include "UART2.h"
#include "cbfifo.h"



extern cbfifo_t Rx_cbuff0;
extern cbfifo_t Tx_cbuff0;

extern int cli_mode_on;
extern int tx_msg_flag;


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
 *      : command
 * Returns
 *      : None
 */
static void units(const char* cmd)
{
    //delay_ms(1000);


    puts_u0("Print Units");
    puts_u0("Temperature : °celcius,  Humidity :%, Pressure :hg\n\r");
    //delay_ms(1000);
    return;
}


 /*
 * If unknown prints unknown command
 *
 * Parameters
 *      : command
 *
 * Returns
 *  : None
 */
static void unknown_cmd(const char* cmd)
{
    char str[100];
    sprintf(str,"Unknown Command: %s\n\r", cmd);
    puts_u0(str);
    puts_u0("? ");
}

/*
 * Help menu for all the commands available
 *
 * Parameters
 *       : command
 * Returns
 *
 *      : None
 */

static void help(const char* cmd)
{
    puts_u0(" To send msg \n\r");
    puts_u0("help to print this msg");
    puts_u0("? ");
}


static void msg(const char* cmd)
{

    //Function : send string on u2
    send_stringu2("Hi Mehul\n\r");
}


static const command_lookup_t cmds[] =
{
    {"msg", msg},
    {"units", units},
    {"help", help},
    {"", unknown_cmd}
    // to add new command - add entry in lookup table and define function
};
/*
 * Parse rx string over UART and send process command
 *
 * Parameters :
 *      str : String to Parse
 *
 * Returns
 *      : None
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

    memset(argv, 0, sizeof(argv)); // set initialize all pointer to 0
    memset(&s[0], 0, 256);

    int space_taken_flag = 1;

    for (p = input; p < end; p++)
    {
        if(*p == '\r' ||*p == '\n' )		//take input till '\r' or '\n'
        {
            argv[argc] = &s_buff[0];
            strcpy(argv[argc],s);
            argc++;
            j=0;
        }

        if(*p != ' ')						//take one white space and ignore extra white space 
        {
            s[j] = *p;
            space_taken_flag = 0;
            j++;
        }
        else
        {
            if (space_taken_flag == 1)  //if input string got space
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
    if (argc == 0)   				// no command found 
        return;


    sscanf(argv[0],"%s",&cmd_head[0]);
    int num = sizeof(cmds) / sizeof(command_lookup_t);	
    int i;
    for(i=0; i<num-1; i++)
    {
        if(strcmp(cmd_head, cmds[i].head) == 0)
        {
            cmds[i].func_name(argv[0]); //call the function using function lookup table
            return;                     // function command is successfully executed return to take new command
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
 *        void
 * Returns:
 *       void
 */
#define MAX_IP_STRING_SIZE 640
int manage_cli(void)
{
    char si[MAX_IP_STRING_SIZE] = {0};
    memset(si,'\0',MAX_IP_STRING_SIZE);

    char *s = &si[0];

    char ch = 0;


    while(ch != '\r')    						//take input till receive carriage return
    {
        if(ch == '\r')
        {
            break;
        }
        while(check_cbfifo_empty(&Rx_cbuff0))
            ;

        char chr[1];


        cbfifo_dequeue(&Rx_cbuff0, &chr[0],1);  //dequeue one by one character
        ch = chr[0];

        if(ch != '\b')
        {
            putch_u0(ch);                     //echo back the character
        }



        if((ch!= '\r') || (ch!='\0'))
        {
            //back space handling
            if(ch == '\b')                  // Back space 0x08
            {
                putch_u0(ch);
                puts_u0(" \b");      //so after one white space give \b will
                s--;
            }
            else
            {
                *s++ = (char)ch; // take input
            }
        }
        if(ch == '\r' ||ch == '\n')
        {
            ch = '\n';
            puts_u0("\n\r");


            *s++ = '\n';
            *s++ = '\r';
            *s++ = '\0';        //put null at the end and send accumulated line for command processing
            break;
        }
    }

    if(tx_msg_flag==1)
    {
        send_stringu2(&si[0]);
        puts_u0("Sent message: ");
        puts_u0(&si[1]);
        puts_u0("\n\r");
    }
    else
    {
         process_cmd(si);
    }

    memset(&si[0], '\0', sizeof(si));
    s = &si[0];             //reset to start from string


    puts_u0("?");
    return 0;
}


/*
 * function to enter into command line interface mode
 *
 * Parameters:
 *          void
 * Returns:
 *           0 if completed
 */
int Cli()
{
    int flag_not_done  = 1;

    while(flag_not_done)
    {
        flag_not_done = manage_cli();
    }
    return 0;
}
