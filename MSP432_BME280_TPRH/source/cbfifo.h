/**----------------------------------------------------------------------------------------------------------------
 * cbfifo.h
 *   Author: mehul
 *
 *  File brief:
 *      circular buffer with
 *       - support for multiple FIFO instances, and for protecting critical sections
 *         as we will be using the circular buffers from within interrupt handlers
 *
 *----------------------------------------------------------------------------------------------------------------
 */

#ifndef SOURCE_CBFIFO_H_
#define SOURCE_CBFIFO_H_



#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "cbfifo.h"
#include "UART0.h"



#define MAX_CB_SIZE 256  //as per assigent write up set circular buffer size
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
typedef struct {
    int write;
    int read;
    size_t curr_length;
    bool Full_Status;
    uint8_t data[MAX_CB_SIZE];
} cbfifo_t;



/*
 *  function  : Initializing the cbfifo
 *
 * Parameters:
 * cbfifo_t : cbfifo instance
 * Returns:   none - void
 */

void Init_cbfifo(cbfifo_t * cb);


/*
 * Function : return maximum capacity of cb FIFO
 *
 * Parameters : arguments : cbfifo - circular bffer instance
 *
 * Returns  : return value : type size_t : returns capacity in no bytes
 *
 */
size_t cbfifo_capacity(cbfifo_t * cb);


/*
 * Function :  to check if cbfifo is empty or not
 *
 * Parameters : cbfifo_t : cbfifo isntance
 *
 * Returns  : return : true/false  - 1 / 0
 *
 */
bool check_cbfifo_empty(cbfifo_t * cb);


/*
 * Function : return current length of given cbfifo i.e no of total enqued byte
 *
 * Parameters : cbfifo instance
 *
 * Returns  :  int -- length
 *
 */
int cbfifo_current_length(cbfifo_t * cb);


/*
 * Function :  function to check if circular buffer is completely full
 *
 * Parameters :  cbfifo isntance
 *
 * Returns  : bool True/False
 *
 */
bool check_cbfifo_full(cbfifo_t * cb);

/*
 * Function :  returns no. of bytes currently enqued on to the circular buffer/ FIFO
 *
 * Parameters :  cbfifo instance
 *
 * Returns  :  return type
 *
 */
size_t cbfifo_length(cbfifo_t * cb);


/*
 * Function : function to Enque data into circular buffer fifo
 *  it will enque data till max capacity avalible in fifo
 *
 * Parameters :
 * cbfifo_t * cb    : pointer of type cbfifo_t* to cbfifo instance --i.e. to which you want ot enque the data
 * const void * buf : Buffer from which you want to enque data onto cbfifo
 * size_t nbytes    : number of u want to enque
 *
 *  Return Value :
 *      return type size_t (i.e unsigned int)
 *
 *  number of bytes successfully enqueued
 *
 *  if can not enque any byte = 0 ;
 *  if error return -1 ;
 *
 */
size_t cbfifo_enqueue(cbfifo_t * cb, char* buf, size_t nbytes);





/*
 * Function : cbfifo_dequeue function will dequeue bytes and write to passed buffer
 * it will dequeue number of requested no bytes or no of availbale byte to be dequeued whichever is lower
 * from circular buffer fifo to passed buffer *buf
 *
 * Parameters :
 * cbfifo_t * cb    : pointer of type cbfifo_t* to cbfifo instance --i.e. frpm which you want to deque the data
 * void *buf        : pointer to Buffer onto which you want to read the data from cbfifo
 * size_t nbyte     : number of u want to deque
 *
 * Returns  :
 *  (return type)  :  size_t
 *  if can not dequeue any byte = 0 ;
 *  if error return -1 ;
 *
 */
size_t cbfifo_dequeue(cbfifo_t * cb, char *buf , size_t nbyte);





#endif /* SOURCE_CBFIFO_H_ */
