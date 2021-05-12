/*--------------------------------------------------------------------------------------------------
 * i2c.h
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *  File brief :
 *  	Initialization  of I2C module communication over I2C
 *  	functions for different modes I2C communciation
 *  	Drivers for reading and writing functionalities for over I2C bus  in master mode
 *
 * 	  https://github.com/sunsided/frdm-kl25z-marg-fusion/blob/master/frdm-kl25z-acc-uart/Sources/i2c/i2c.h
 * --------------------------------------------------------------------------------------------------
 */

#ifndef I2C_H_
#define I2C_H_



#include "MKL25Z4.h"
#include "systick.h"
#include "stdint.h"



/*
 * Sends a start condition and enters TX mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_SendStart();

/*
 * Enters transmit mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterTransmitMode();

/*
 * Enters receive mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterReceiveMode();

/*
 * Enters receive mode and enables ACK.
 *
 * Enabling ACK may be required when more than one data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterReceiveModeWithAck();


/*
 * Enters receive mode and disables ACK.
 *
 * Disabling ACK may be required when only one data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterReceiveModeWithoutAck();


/*
 * Sends a repeated start condition and enters TX mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_SendRepeatedStart();

/*
 * Sends a stop condition (also leaves TX mode)
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
 void I2C_SendStop();



/*
 * Enables sending of ACK
 *
 * Enabling ACK may be required when more than one data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
 void I2C_EnableAck();

/*
 * Enables sending of NACK (disabling ACK)
 *
 * Enabling NACK may be required when no more data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_DisableAck();


/*
 * Sends a byte over the I2C bus and waits for the operation to complete
 *
 * Parameters : value The byte to send
 *
 * Returns :
 *  None
 */
 void I2C_SendBlocking(const uint8_t value);

/*
 * Reads a byte over the I2C bus and drives the clock for another byte
 *
 * Returns :
 *  vale: The received byte
 */
 uint8_t I2C_ReceiveDriving();



/*
 * Reads a byte over the I2C bus and drives the clock for another byte, while sending NACK
 * Returns :
 *  The received byte
 */
uint8_t I2C_ReceiveDrivingWithNack();


/*
 * Reads the last byte over the I2C bus and sends a stop condition
 * Returns :
 *  The received byte
 */
 uint8_t I2C_ReceiveAndStop();


/*
 * Reads a byte over the I2C bus and sends a repeated start condition.
 * Returns :
 *  The received byte
 *
 * The I2C module is in transmit mode afterwards.
 */
 uint8_t I2C_ReceiveAndRestart();


/*
 * Drives the clock in receiver mode in order to receive the first byte.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
 void I2C_ReceiverModeDriveClock();


/*
 * Waits for an I2C bus operation to complete
 *
 * Parameters :
 *  None
 * Parameters :
 *  None
 */

void I2C_WaitWhileBusy();


/*
 * Initializes the I2C communication, configure the I2C clock
 *
 */
void I2C_Init();
/*
 * Resets the bus by toggling master mode if the bus is busy. This will interrupt ongoing traffic.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_ResetBus();

/*
 * Reads an 8-bit register from an I2C slave
 * Parameters :
 *  slaveId The device's I2C slave id
 * 		registerAddress Address of the device register to read
 *
 * Returns :
 *  The value at the register
 */
uint8_t I2C_ReadRegister(register uint8_t slaveId, register uint8_t registerAddress);

/*
 * Reads multiple 8-bit registers from an I2C slave
 * Parameters :
 *  slaveId: The slave device ID
 * 		startRegisterAddress: The first register address
 * 		registerCount: The number of registers to read; Must be larger than zero.
 * 		buffer: The buffer to write into
 *
 * Returns :
 *  None
 */
void I2C_ReadRegisters(register uint8_t slaveId, register uint8_t startRegisterAddress, register uint8_t registerCount, uint8_t *buffer);

/*
 * Writes an 8-bit value to an 8-bit register on an I2C slave
 * Parameters :
 *  slaveId: The device's I2C slave id
 * 		registerAddress: Address of the device register to read
 * 		value: The value to write
 *
 * Returns :
 *  None
 */
void I2C_WriteRegister(register uint8_t slaveId, register uint8_t registerAddress, register uint8_t value);

/*
 * Reads an 8-bit register from an I2C slave, modifies it by FIRST and-ing with {@see andMask} and THEN or-ing with {@see orMask} and writes it back
 * Parameters :
 *  slaveId: The device's I2C slave id
 * 		registerAddress: Address of the device register to read
 * 		andMask: The mask to AND the register with
 * Returns :
 *  The register after modification
 */
uint8_t I2C_ModifyRegister(register uint8_t slaveId, uint8_t register registerAddress, register uint8_t andMask, register uint8_t orMask);


/*
 * Waits for an I2C bus operation to complete
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_Wait();



/*
 * Waits for an I2C bus operation to complete
 *
 * Parameters :
 *  None
 * Parameters :
 *  None
 */
void I2C_WaitWhileBusy();


/*
 * Sends a start condition and enters TX mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_SendStart();

/*
 * Enters transmit mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterTransmitMode();

/*
 * Enters receive mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterReceiveMode();


/*
 * Enters receive mode and enables ACK.
 *
 * Enabling ACK may be required when more than one data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterReceiveModeWithAck();

/*
 * Enters receive mode and disables ACK.
 *
 * Disabling ACK may be required when only one data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_EnterReceiveModeWithoutAck();

/*
 * Sends a repeated start condition and enters TX mode.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_SendRepeatedStart();


/*
 * Sends a stop condition (also leaves TX mode)
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void I2C_SendStop();


/*
 * Enables sending of ACK
 *
 * Enabling ACK may be required when more than one data byte will be read.
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
 void I2C_EnableAck();



/*
 * Initiates a register read after the module was brought into TX mode.
 * Parameters :
 *  slaveId The slave id
 *  registerAddress the register to read from
 *
 * Returns :
 *  None
 */
void I2C_InitiateRegisterReadAt(const register uint8_t slaveId, const register uint8_t registerAddress);


/**
 * send start sequence over the i2c
 *
 * Parameters :
 *  None
 * Returns :
 *  None
 */
void i2c_start(void);

/*
 * Read the slave configuration
 *
 * Parameters :
 *  	1) dev: Slave Device Address
 * 		2) Address: Register Address to Read config from
 * Returns :
 *  None
 */
void i2c_read_setup(uint8_t dev, uint8_t address);

/*
 * Read a byte and ack/nack as appropriate
 *
 * Parameters :
 *  	uint8_t value: Read multiple Data Generated from register source
 * Returns :
 *  	None
 */
uint8_t i2c_repeated_read(uint8_t);

/*
 * For reading and writing a single byte
		  using 7bit addressing reads a byte from dev:address
 *
 * Parameters :
 * 		 1) dev: Slave Device Address
 * 		 2) Address: Register Address to Read data from
 * Returns :
 *  None
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/*
 * using 7bit addressing writes a byte data to dev:address
 *
 * Parameters :
 *  1) dev: Slave Device Address
 * 	2) Address: Register Address to Read data from
 * Returns :
 *  None
 */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);
#endif
