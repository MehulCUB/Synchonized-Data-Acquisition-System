/**----------------------------------------------------------------------------------------------------------------
 * File brief  : cbfifo.c
 *
 *      Author: mehul
 *
 * circular buffer with
 *  - support for multiple FIFO instances
 *  - and for protecting critical sections as we will be using the circular buffers from within interrupt handlers
 *----------------------------------------------------------------------------------------------------------------
 */



#include <ti/devices/msp432p4xx/inc/msp432.h>

#include "cbfifo.h"


#define min(x,y) ((x)<(y)?(x):(y))

//see header file detailed description of functions


//Function : Initializing the cbfifo
void Init_cbfifo(cbfifo_t * cb)
{
    cb->write = 0;              //write pointer
    cb->read = 0;               //read pointer
    cb->Full_Status = 0;
    cb->curr_length = 0;
    int i;
    for ( i=0; i<MAX_CB_SIZE; i++)
        cb->data[i] = '_';  // to simplify our lives when debugging
}


//Function :
// return maximum capacity of cb FIFO
size_t cbfifo_capacity(cbfifo_t * cb)
{
    return MAX_CB_SIZE;
}

//Function :
//to check if cbfifo is empty or not
bool check_cbfifo_empty(cbfifo_t * cb)
{
    assert(cb);
    if(cb->write == cb->read)
        return 1;
    return 0;
}

//Function :
//return current length of given cbfifo i.e no of total enqueued byte
int cbfifo_current_length(cbfifo_t * cb)
{
    assert(cb);
    return cb->curr_length;
}


//Function :
//function to check if circular buffer is completely full
bool check_cbfifo_full(cbfifo_t * cb)
{
    assert(cb);
    if(cbfifo_length(cb) == MAX_CB_SIZE)
        return 1;
    return 0;
}

//Function :
//returns no. of bytes currently enqued on to the circular buffer/ FIFO
size_t cbfifo_length(cbfifo_t * cb){
    //uint32_t masking_state;
    size_t val=0;
    if(cb->Full_Status)
    {
        val = MAX_CB_SIZE;
    }
    else if(cb->write >= cb->read)
    {
        val = cb->write - cb->read;
    }
    else
    {
        val = MAX_CB_SIZE - (cb->read - cb->write);
    }
    return val;
}

//Function :
//function to Enque data into circular buffer FIFO it will enqueued data till max capacity available in fifo
size_t cbfifo_enqueue(cbfifo_t * cb, char * buf, size_t nbytes)
{
  int enqueued_bytes = 0;        //to count no. of enqueued bytes
  char* enq_rptr = (char*) buf;  //pointer for reading from the received block of buffer
  if(cb->Full_Status)
  {
        return 0;
  }
  //if cbfifo is already full  -  can't enqueue any byte
  if (cbfifo_length(cb) == MAX_CB_SIZE)
  {
    return 0;
  }

  __disable_irq();  //disable interrupt for protecting critical section

  int i;
  for ( i= 0; i < nbytes; i++)
  {
    if((cbfifo_length(cb) == MAX_CB_SIZE)) // if cbuffer is full - stop enqueing
    {
        cb -> Full_Status = 1;
        break;
    }
    // if next CB byte is empty then
    if (!check_cbfifo_full(cb))
    {
      // write to current CB byte from buffer
      cb -> data[cb -> write] = *enq_rptr++;  //enqueued byte form buffer onto cbfifo
      enqueued_bytes++;
      cb -> write++;                          //inc. pointer to write to next byte
      if(cb -> write == MAX_CB_SIZE)
      {
         cb->write = 0;                      //roll over to initial point
      }
      if(cb->read == cb->write)              //keep checking if cbfifo is full  --- if full stop enqueing
      {
        cb -> Full_Status = 1;
        break;
      }
    }
    else
    {
      //if cbfifo is full cant enqueue now
      break;
    }
  }
  cb -> curr_length +=enqueued_bytes;
  __enable_irq();
  return enqueued_bytes;
}


//Function :
//cbfifo_dequeue function will dequeue bytes and write to passed buffer
size_t cbfifo_dequeue(cbfifo_t * cb, char *buf , size_t nbyte)
{

    size_t dequeued_bytes=0;    //to track no of actually  dequeued bytes
    size_t dq_bytes=0;          //Calculate no. of bytes to be dequeued
    size_t dq_bytes2=0;         //to calculate no. of bytes remaining to do after rolling over read pointer

    char *dq_ptr = (char *)buf; //pointer for writing to buffer the read data from cbfifo onto given buffer


    //fifo is empty
    if ((cbfifo_length(cb) == 0) && (!cb->Full_Status)) //i.e. empty
    {
        return 0;
    }
    cb->Full_Status = 0;

    if(check_cbfifo_full(cb)==1)
    {
        cb->Full_Status = 1;
    }

       __disable_irq();   //disable UART and other interrupts for protecting critical section i.e. buffer



    dq_bytes = min(nbyte, MAX_CB_SIZE - cb->read);  //calculate no of byte can be dequeued from

    if((cb->write > cb->read) && (dq_bytes > (cb->write - cb->read)))
    {
        dq_bytes = cb->write - cb->read;
    }

    int i;
    //dequeue bytes from cb and update the pointer and counter
    for(i=0;i<dq_bytes;i++)
    {
        *dq_ptr++ = cb->data[cb->read]; //pop the data from the location and write to buffer
        cb->read++;                     //update the read pointer
        dequeued_bytes++;               //track no of dequeued_bytes
    }


    if(cb->read >= MAX_CB_SIZE)
    {
        cb->read = 0; //roll over read pointer from starting
        dq_bytes2=min(nbyte - dequeued_bytes,cb->write);
        int i;

        for(i=0; i<dq_bytes2; i++)
        {
            *dq_ptr++ = cb->data[cb->read]; //pop the data from the location and write to buffer
            cb->data[cb->read] = '\0';      //put null after popped data
            cb->read++;                     //update the read pointer
            dequeued_bytes++;               //track no of dequeued_bytes
        }
    }

    cb->curr_length-= (dequeued_bytes); //update the current length i.e. currently enqueued bytes on given cbfifo

    __enable_irq(); //END of critical section
    return dequeued_bytes;
}

