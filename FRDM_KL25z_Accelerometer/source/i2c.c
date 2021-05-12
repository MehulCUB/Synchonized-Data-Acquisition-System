/*--------------------------------------------------------------------------------------------------
 * i2c.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *  File brief :
 *  	Initialization  of I2C module communication over I2C
 *  	functions for different modes of I2C
 *  	Drivers for reading and writing functionalities for over I2C bus  in master mode
 *
 * 	  https://github.com/sunsided/frdm-kl25z-marg-fusion/blob/master/frdm-kl25z-acc-uart/Sources/i2c/i2c.c
 * --------------------------------------------------------------------------------------------------
 */


#include "i2c.h"
#include "systick.h"
#include "assert.h"
#include "LOG.h"


#include "i2c.h"
#include "assert.h"
#include "systick.h"



#define MMA8451_SCL 24
#define MMA8451Q_SDA 25
#define MMA8451Q_I2C_MUX 5



int lock_detect=0;
int i2c_lock=0;

#define I2C_READ_ADDRESS(slaveAddress) 		((uint8_t)((slaveAddress << 1) | 1))		// Encodes the read address from the 7-bit slave address
#define I2C_WRITE_ADDRESS(slaveAddress) 	((uint8_t)((slaveAddress << 1) | 0))		// Encodes the write address from the 7-bit slave address

#define I2C_MOD_NO_OR_MASK	(0x0)		// no-or modify operation
#define I2C_MOD_NO_AND_MASK	(~0x0)		// no-and modify operation


#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    while(I2C0->S & 0x01)
#define TRANS_COMP		while(!(I2C0->S & 0x80))
#define I2C_WAIT 			i2c_wait();

#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK



 //Sends a start condition and enters TX mode.
 
void I2C_SendStart()
{

	I2C0->C1 |= ((1 << I2C_C1_MST_SHIFT) & I2C_C1_MST_MASK)
				| ((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);

}
//Enters transmit mode.
void I2C_EnterTransmitMode()
{

	I2C0->C1 |= ((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);

}

//Enters receive mode.
void I2C_EnterReceiveMode()
{

	I2C0->C1 &= ~((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);

}
//Enters receive mode and enables ACK.
void I2C_EnterReceiveModeWithAck()
{

	I2C0->C1 &= ~((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK)
			& ~((1 << I2C_C1_TXAK_SHIFT) & I2C_C1_TXAK_MASK);

}


//Enters receive mode and disables ACK.
void I2C_EnterReceiveModeWithoutAck()
{
	/* Straightforward method of clearing TX mode and
	 * setting NACK bit sending.
	 */

	register uint8_t reg = I2C0->C1;
	reg &= ~((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);
	reg |=  ((1 << I2C_C1_TXAK_SHIFT) & I2C_C1_TXAK_MASK);
	I2C0->C1 = reg;


}

/*
 * Sends a repeated start condition and enters TX mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_SendRepeatedStart()
{

	register uint8_t reg = I2C0->F;
	I2C0->F = reg & ~I2C_F_MULT_MASK; /* NOTE: According to KINETIS_L_2N97F errata (e6070), repeated start condition can not be sent if prescaler is any other than 1 (0x0). A solution is to temporarily disable the multiplier. */



	I2C0->C1 |= ((1 << I2C_C1_RSTA_SHIFT) & I2C_C1_RSTA_MASK)
			  | ((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);

	I2C0->F = reg;

}


// Sends a stop condition (also leaves TX mode)
void I2C_SendStop()
{
	I2C0->C1 &= ~((1 << I2C_C1_MST_SHIFT) & I2C_C1_MST_MASK)
			& ~((1 << I2C_C1_TX_SHIFT) & I2C_C1_TX_MASK);
}



//Enables sending of ACK
void I2C_EnableAck()
{
	I2C0->C1 &= ~((1 << I2C_C1_TXAK_SHIFT) & I2C_C1_TXAK_MASK);

}

//Enables sending of NACK (disabling ACK)
void I2C_DisableAck()
{
	I2C0->C1 |= ((1 << I2C_C1_TXAK_SHIFT) & I2C_C1_TXAK_MASK);
}


//Sends a byte over the I2C bus and waits for the operation to complete
void I2C_SendBlocking(const uint8_t value)
{
	I2C0->D = value;
	I2C_Wait();
}

// Reads a byte over the I2C bus and drives the clock for another byte
uint8_t I2C_ReceiveDriving()
{
	register uint8_t value = I2C0->D;
	I2C_Wait();
	return value;
}



//Reads a byte over the I2C bus and drives the clock for another byte, while sending NACK
uint8_t I2C_ReceiveDrivingWithNack()
{
	I2C_DisableAck();
	return I2C_ReceiveDriving();
}

//Reads the last byte over the I2C bus and sends a stop condition
uint8_t I2C_ReceiveAndStop()
{
	I2C_SendStop();
	return I2C0->D;
}



//Reads a byte over the I2C bus and sends a repeated start condition
uint8_t I2C_ReceiveAndRestart()
{
	I2C_SendRepeatedStart();
	return I2C0->D;
}

// Drives the clock in receiver mode in order to receive the first byte.
void I2C_ReceiverModeDriveClock()
{
	register uint8_t val = I2C0->D;
	if(val) 
	{
		val = 0;
	}
	I2C_Wait();
}


/*
 * Waits for an I2C bus operation to complete
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_Wait()
{
	while((I2C0->S & I2C_S_IICIF_MASK)==0) {}	/* loop until interrupt is detected */

	I2C0->S |= I2C_S_IICIF_MASK; /* clear interrupt flag */
}


/*
 * Waits for an I2C bus operation to complete
 *		
 * Parameters :
 *  		None
 * Parameters :
 *  		None
 */

void I2C_WaitWhileBusy()
{
 	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
 	I2C_TRAN;
 	I2C_M_START;
 	I2C0->C1 |=  I2C_C1_IICEN_MASK;

 	// Write to clear line
 	I2C0->C1 |= I2C_C1_MST_MASK; 	// set MASTER mode
 	I2C0->C1 |= I2C_C1_TX_MASK; 	// Set transmit (TX) mode
 	I2C0->D = 0xFF;
 	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
 	} 								// wait interrupt
 	I2C0->S |= I2C_S_IICIF_MASK; 	// clear interrupt bit

 	I2C0->S |= I2C_S_ARBL_MASK;		// Clear arbitration error flag


 						/* Send start */
 	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
 	I2C0->C1 |= I2C_C1_TX_MASK; 	// Set transmit (TX) mode
 	I2C0->C1 |= I2C_C1_MST_MASK; 	// START signal generated

 	I2C0->C1 |= I2C_C1_IICEN_MASK;
 									// Wait until start is send

 						/* Send stop */
 	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
 	I2C0->C1 |= I2C_C1_MST_MASK;
 	I2C0->C1 &= ~I2C_C1_MST_MASK; 	// set SLAVE mode
 	I2C0->C1 &= ~I2C_C1_TX_MASK; 	// Set Rx
 	I2C0->C1 |= I2C_C1_IICEN_MASK;

 	I2C0->S |= I2C_S_IICIF_MASK; 	// wait
 	I2C0->S |= I2C_S_ARBL_MASK; 	// Clear arbitration error & interrupt flag
 }

/*
 *  Initialises the I2C interface 
 * 	Initializes the I2C communication  by clock gating and configure the I2C clock
 *
 */
void I2C_Init()
{
	/* enable clock gating to I2C0 */

	SIM->SCGC4 |= (1 << SIM_SCGC4_I2C0_SHIFT) & SIM_SCGC4_I2C0_MASK;

	SIM->SCGC5 |= (1 << SIM_SCGC5_PORTE_SHIFT) & SIM_SCGC5_PORTE_MASK;		// enable the clock gate to port E

	/* configure port E pins to I2C operation for MMA8451Q */
	PORTE->PCR[24] = PORT_PCR_MUX(5); 	/* SCL */
	PORTE->PCR[25] = PORT_PCR_MUX(5); 	/* SDA */

	I2C0->F = I2C_F_MULT(0x00) | I2C_F_ICR(0x12); 							// divide by 64 instead, so 375 kHz

	I2C0->C1 = (1 << I2C_C1_IICEN_SHIFT) & I2C_C1_IICEN_MASK;				// enable the I2C module

	I2C0->C2 |= (I2C_C2_HDRS_MASK);											// Select high drive mode
}

 /*
 * Reads an 8-bit register from an I2C slave
 * Parameters :
 *  slaveId The device's I2C slave id
 * 		registerAddress Address of the device register to read
 *
 * Returns :
 *  The value at the register
 */
uint8_t I2C_ReadRegister(register uint8_t slaveId, register uint8_t registerAddress)
{
	I2C_WaitWhileBusy();							// loop while the bus is still busy

	I2C_SendStart();								// send I2C start signal and set write direction, also enables ACK

	I2C_SendBlocking(I2C_WRITE_ADDRESS(slaveId));	// send the slave address and wait for the I2C bus operation to complete

	I2C_SendBlocking(registerAddress);				// send the register address

	I2C_SendRepeatedStart();						// signal a repeated start condition

	I2C_SendBlocking(I2C_READ_ADDRESS(slaveId));	// send the read address

	I2C_EnterReceiveModeWithoutAck();				// switch to receive mode but disable ACK because only one data byte will be read

	I2C_ReceiverModeDriveClock();					// read a dummy byte to drive the clock

	I2C_SendStop();									// stop signal

	register uint8_t result = I2C0->D;				// fetch the last received byte
	return result;
}

/*
 * Reads multiple 8-bit registers from an I2C slave
 * 
 * Parameters :
 *  	slaveId				: The slave device ID
 * 		startRegisterAddress: The first register address
 * 		registerCount		: The number of registers to read; Must be greater than or equal to two.
 * 		buffer				: The buffer to write into
 */
static void I2C_ReadRegistersInternal(register uint8_t slaveId, register uint8_t startRegisterAddress, register uint8_t registerCount, uint8_t *const buffer)
{
	assert(registerCount >= 2);

	/* loop while the bus is still busy */
	I2C_WaitWhileBusy();

	/* send I2C start signal and set write direction, also enables ACK */
	I2C_SendStart();

	/* send the slave address and wait for the I2C bus operation to complete */
	I2C_SendBlocking(I2C_WRITE_ADDRESS(slaveId));

	/* send the register address */
	I2C_SendBlocking(startRegisterAddress);

	/* signal a repeated start condition */
	I2C_SendRepeatedStart();

	/* send the read address */
	I2C_SendBlocking(I2C_READ_ADDRESS(slaveId));

	/* switch to receive mode and assume more than one register */
	I2C_EnterReceiveModeWithAck();

	/* read a dummy byte to drive the clock */
	I2C_ReceiverModeDriveClock();

	/* for all remaining bytes, read */
	--registerCount;
	uint8_t index = 0;
	while (--registerCount > 0)
	{
		/* fetch and store value */
		register uint8_t value = I2C0->D;
		buffer[index++] = value;

		/* wait for completion */
		I2C_Wait();
	}

	/* disable ACK and read second-to-last byte */
	I2C_DisableAck();

	/* fetch and store value */
	buffer[index++] = I2C0->D;

	/* wait for completion */
	I2C_Wait();

	/* stop signal */
	I2C_SendStop();

	/* fetch the last received byte */
	buffer[index++] = I2C0->D;
}

/*
 * Reads multiple 8-bit registers from an I2C slave
 * 
 * Parameters :
 *  		slaveId				: The slave device ID
 * 			startRegisterAddress: The first register address
 * 			registerCount		: The number of registers to read; Must be larger than zero.
 * 			buffer				: The buffer to write into
 */
void I2C_ReadRegisters(register uint8_t slaveId, register uint8_t startRegisterAddress, register uint8_t registerCount, register uint8_t *buffer)
{
	assert(registerCount > 0);

	if (registerCount >= 2)
	{
		I2C_ReadRegistersInternal(slaveId, startRegisterAddress, registerCount, buffer);
	}
	else
	{
		assert(1 == registerCount);
		register uint8_t result = I2C_ReadRegister(slaveId, startRegisterAddress);
		buffer[0] = result;
	}
}

/*
 * Reads an 8-bit register from an I2C slave
 */
void I2C_WriteRegister(register uint8_t slaveId, register uint8_t registerAddress, register uint8_t value)
{
	/* loop while the bus is still busy */
	I2C_WaitWhileBusy();

	/* send I2C start signal and set write direction*/
	I2C_SendStart();

	/* send the slave address and wait for the I2C bus operation to complete */
	I2C_SendBlocking(I2C_WRITE_ADDRESS(slaveId));

	/* send the register address */
	I2C_SendBlocking(registerAddress);

	/* send the register address */
	I2C_SendBlocking(value);

	/* issue stop signal by clearing master mode. */
	I2C_SendStop();
}

/*
 * Reads an 8-bit register from an I2C slave, modifies it by FIRST and-ing with {@see andMask} and THEN or-ing with {@see orMask} and writes it back
 * 
 * Parameters :
 *
 *  slaveId: 		The slave device ID
 *	registerAddress The register to modify
 * 		orMask 		The mask to OR the register with
 * 		andMask 	The mask to AND the register with The register after modification
 *
 */
uint8_t I2C_ModifyRegister(register uint8_t slaveId, register uint8_t registerAddress, register uint8_t andMask, register uint8_t orMask)
{
	/* loop while the bus is still busy */
	I2C_WaitWhileBusy();

	/* send the slave address and register */
	I2C_SendStart();
	I2C_SendBlocking(I2C_WRITE_ADDRESS(slaveId));
	I2C_SendBlocking(registerAddress);

	/* signal a repeated start condition */
	I2C_SendRepeatedStart();
	I2C_SendBlocking(I2C_READ_ADDRESS(slaveId));

	/* switch to receive mode but disable ACK because only one data byte will be read */
	I2C_EnterReceiveModeWithoutAck();
	I2C_ReceiverModeDriveClock();

	/* instead of a stop signal, send repeated start again */
	I2C_SendRepeatedStart();

	/* fetch the last received byte */
	register uint8_t value = I2C0->D;

	/* modify the register */
	value &= andMask;
	value |= orMask;

	/* send the slave address and wait for the I2C bus operation to complete */
	I2C_SendBlocking(I2C_WRITE_ADDRESS(slaveId));

	/* send the register address */
	I2C_SendBlocking(registerAddress);

	/* send the register address */
	I2C_SendBlocking(value);

	/* issue stop signal by clearing master mode. */
	I2C_SendStop();
	return value;
}

/*
 * Processing the I2C bus operation to complete
 *
 * Parameters :
 *  		None
 * Parameters :
 *  		None
 */
void i2c_busy(void)
{
	// Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;
	// Write to clear line
	I2C0->C1 |= I2C_C1_MST_MASK; 		/* set MASTER mode */
	I2C0->C1 |= I2C_C1_TX_MASK; 		/* Set transmit (TX) mode */
	I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U)
	{
	} 									/* wait interrupt */
	I2C0->S |= I2C_S_IICIF_MASK; 		/* clear interrupt bit */

	I2C0->S |= I2C_S_ARBL_MASK;			/* Clear arbitration error flag*/

							/* Send start */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK; 		/* Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_MST_MASK;		/* START signal generated */

	I2C0->C1 |= I2C_C1_IICEN_MASK;
							/*Wait until start is send*/

							/* Send stop */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_MST_MASK; 		/* set SLAVE mode */
	I2C0->C1 &= ~I2C_C1_TX_MASK; 		/* Set Rx */
	I2C0->C1 |= I2C_C1_IICEN_MASK;


							/* wait */
							/* Clear arbitration error & interrupt flag*/
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

/*
 * Waits for an I2C bus operation to complete
 *
 * Parameters :
 *  		None
 * Parameters :
 *  		None
 */
void i2c_wait(void) {
	lock_detect = 0;
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	if (lock_detect >= 200)
		i2c_busy();
	I2C0->S |= I2C_S_IICIF_MASK;
}



/* Start I2C communication
 * 	i.e send start sequence over the i2c
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void i2c_start()	
{
	I2C_TRAN;					/*set to transmit mode */
	I2C_M_START;				/*send start	*/
}

/* setup I2C bus for read operaiton over I2C bus from slave device 
 * Read the slave configuration
 *
 * Parameters :
 *  	1) dev: Slave Device Address
 * 		2) Address: Register Address to Read config from
 * Returns :
 *  None
 */
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev;			  	/*send dev address	*/
	I2C_WAIT					/*wait for completion */

	I2C0->D =  address;			/*send read address	*/
	I2C_WAIT					/*wait for completion */

	I2C_M_RSTART;				/*repeated start */
	I2C0->D = (dev|0x1);	 	/*send dev address (read)	*/
	I2C_WAIT					/*wait for completion */

	I2C_REC;					/*set to receive mode */

}

/* 
 * read byte over I2C bus from slave device
 * using 7bit addressing writes a byte data to dev:address
 *
 * Parameters :
 *  1) dev: Slave Device Address
 * 	2) Address: Register Address to Read data from
 * Returns :
 *  None
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	I2C_TRAN;					/*set to transmit mode */
	I2C_M_START;				/*send start	*/
	I2C0->D = dev;			  	/*send dev address	*/
	I2C_WAIT					/*wait for completion */

	I2C0->D =  address;			/*send read address	*/
	I2C_WAIT					/*wait for completion */

	I2C_M_RSTART;				/*repeated start */
	I2C0->D = (dev|0x1);	 	/*send dev address (read)	*/
	I2C_WAIT					/*wait for completion */

	I2C_REC;					/*set to recieve mode */
	NACK;						/*set NACK after read	*/

	data = I2C0->D;				/*dummy read	*/
	I2C_WAIT					/*wait for completion */

	I2C_M_STOP;					/*send stop	*/
	data = I2C0->D;				/*read data	*/

	return data;
}

 
 /*
 * Read a byte and ack/nack as appropriate
 * repeated read over I2C bus from slave device 
 * Parameters :
 *  	uint8_t value: Read multiple Data Generated from register source
 * Returns :
 *  	None
 */
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;

	lock_detect = 0;

	if(isLastRead)	{
		NACK;					/*set NACK after read	*/
	} else	{
		ACK;					/*ACK after read	*/
	}

	data = I2C0->D;				/*dummy read	*/
	I2C_WAIT					/*wait for completion */

	if(isLastRead)	{
		I2C_M_STOP;				/*send stop	*/
	}
	data = I2C0->D;				/*read data	*/

	return  data;
}
/*
 * using 7bit addressing writes a byte data to dev:address
 *
 * Parameters :
 *  1) dev: Slave Device Address
 * 	2) Address: Register Address to Read data from
 * Returns :
 *  None
 */

void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

	I2C_TRAN;					/*set to transmit mode */
	I2C_M_START;				/*send start	*/
	I2C0->D = dev;			  	/*send dev address	*/
	I2C_WAIT					/*wait for ack */

	I2C0->D =  address;			/*send write address	*/
	I2C_WAIT

	I2C0->D = data;				/*send data	*/
	I2C_WAIT
	I2C_M_STOP;

}



