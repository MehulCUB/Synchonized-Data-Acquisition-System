/* ----------------------------------------------------------------------------------------------------
 * Author  :  Mehul Patel
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * MMA8451Q.c
 *
 * File brief :
 * 				function related to MMA8451Q sensor interfacing, and communication
 * 				Accelerometer raw reading
 * 				based on Acceleration value convention to Roll,pitch, yaw angle
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *    References :
 *     DataStructures MMA8451Q sensor coomunication
 * 	   	https://www.nxp.com/docs/en/data-sheet/MMA8451Q.pdf
 * 	   	https://github.com/adafruit/Adafruit_MMA8451_Library
 *-------------------------------------------------------------------------------------------------
 */

#include <MMA8451Q.h>
#include "stdint.h"
#include "assert.h"
#include "MKL25Z4.h"
#include "math.h"

#define MMA8451_SCL 24
#define MMA8451Q_SDA 25
#define MMA8451Q_I2C_MUX 5


/*
 * I2C slave address of the MMA8451Q accelerometer
 */
#define MMA8451Q_I2CADDR	(0b0011101) /* on the FRDM-KL25Z, the SA0 pin of the MMA8451Q is pulled high */

#define MMA8451Q_REG_WHOAMI				(0x0D)	/*< WHO_AM_I register for device identification */


#define MMA8451Q_REG_CTRL_REG1			(0x2A)	/*< CTRL_REG1 System Control 1 Register */
#define MMA8451Q_REG_CTRL_REG2			(0x2B)	/*< CTRL_REG2 System Control 2 Register */
#define MMA8451Q_REG_CTRL_REG3			(0x2C)	/*< CTRL_REG2 System Control 3 Register */
#define MMA8451Q_REG_CTRL_REG4			(0x2D)	/*< CTRL_REG2 System Control 4 Register */
#define MMA8451Q_REG_CTRL_REG5			(0x2E)	/*< CTRL_REG2 System Control 5 Register */

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)

#define REG_XHI 0x01

#define CTRL_REG1_ACTIVE_SHIFT 	(0x00U)
#define CTRL_REG1_ACTIVE_MASK 	(0x01U)
#define CTRL_REG1_DR_MASK 		(0x38u)
#define CTRL_REG1_DR_SHIFT 		(0x3u)
#define CTRL_REG1_LNOISE_MASK 	(0x4u)
#define CTRL_REG1_LNOISE_SHIFT 	(0x2u)

#define MMA_ADDR 0x3A
#define MMA8451Q_INT_PORT	PORTA				/*! Port at which the MMA8451Q INT1 and INT2 pins are attached */
#define MMA8451Q_INT_GPIO	GPIOA				/*! Port at which the MMA8451Q INT1 and INT2 pins are attached */
#define MMA8451Q_INT1_PIN	14					/*! Pin at which the MMA8451Q INT1 is attached */
#define MMA8451Q_INT2_PIN	15					/*! Pin at which the MMA8451Q INT2 is attached */

#define I2C_MOD_NO_OR_MASK	(0x0)		// no-or modify operation
#define I2C_MOD_NO_AND_MASK	(~0x0)		// no-and modify operation



/*
 * Macro for usage in configuration commands that executes configuration changes directly
 */
#define MMA8451Q_CONFIGURE_DIRECT ((mma8451q_confreg_t*)0x0)

/*
 * Function : 
 * 			Sets the data rate and the active mode
 * Parameters : 
 *		   1) mma8451q_confreg_t *const configuration: Existing Configuration to be set to MMA8451Q
 * 		   2) datarate: Update Hz for the sensor
 * 		   3) lownoise: Bool - Set if Low noise mode
 */

void MMA8451Q_SetDataRate(mma8451q_confreg_t *const configuration, mma8451q_datarate_t datarate, mma8451q_lownoise_t lownoise)
{
	if (MMA8451Q_CONFIGURE_DIRECT == configuration)
	{
		const register uint8_t value = ((datarate << CTRL_REG1_DR_SHIFT) & CTRL_REG1_DR_MASK) | ((lownoise << CTRL_REG1_LNOISE_SHIFT) & CTRL_REG1_LNOISE_MASK);
		const register uint8_t mask = (uint8_t)~(CTRL_REG1_DR_MASK | CTRL_REG1_LNOISE_MASK);
		I2C_ModifyRegister(MMA8451Q_I2CADDR, MMA8451Q_REG_CTRL_REG1, mask, value);
	}
	else
	{
		configuration->CTRL_REG1 &= ~(CTRL_REG1_DR_MASK | CTRL_REG1_LNOISE_MASK);
		configuration->CTRL_REG1 |= ((datarate << CTRL_REG1_DR_SHIFT) & CTRL_REG1_DR_MASK) | ((lownoise << CTRL_REG1_LNOISE_SHIFT) & CTRL_REG1_LNOISE_MASK);
	}
}

void MMA8451Q_InitializeData(mma8451q_acc_t *const data)
{
	data->status = 0;
	data->x = 0;
	data->y = 0;
	data->z = 0;
}


/*
 * Function :
 * 			Brings the MMA8451Q into passive mode
 */
void MMA8451Q_EnterPassiveMode()
{
	I2C_ModifyRegister(MMA8451Q_I2CADDR, MMA8451Q_REG_CTRL_REG1, ~0b00000001, I2C_MOD_NO_OR_MASK);
}

/*
 * Function :
 * 		Resets the MMA8451Q
 */
 void MMA8451Q_Reset()
{
	I2C_WriteRegister(MMA8451Q_I2CADDR, MMA8451Q_REG_CTRL_REG2, 0b01000000);
}

/*
 * Function :
 *		 Brings the MMA8451Q into active mode
 * Parameters
 *			:  configuration The configuration structure or {@see MMA8451Q_CONFIGURE_DIRECT} if changes should be sent directly over the wire.
 */

void MMA8451Q_EnterActiveMode()
{
	I2C_ModifyRegister(MMA8451Q_I2CADDR, MMA8451Q_REG_CTRL_REG1, I2C_MOD_NO_AND_MASK, 0x01);
}

/*
 * Function :
 *  Parameters :
 *  		 Reads the WHO_AM_I register from the MMA8451Q.
 *	Return:
 *   		Device identification code; Should be 0b00011010 (0x1A).
 */
uint8_t MMA8451Q_WhoAmI()
{
	return I2C_ReadRegister(MMA8451Q_I2CADDR, MMA8451Q_REG_WHOAMI);
}


/*
 * Function : 
 *		 Convert the acceleration data read to roll and pitch
 *
 * Parameters : 
 *		1) configuration: Inertial Sensor Configuration to read from
 * 		2) roll: float angle tilt calculated in degrees
 * 		3) pitch: float angle tilt calculated in degrees
 */
void convert_xyz_to_roll_pitch(mma8451q_acc_t *acc, double *roll, double *pitch)
{
	float ax = acc->xyz[0]/COUNTS_PER_G,
		  ay = acc->xyz[1]/COUNTS_PER_G,
		  az = acc->xyz[2]/COUNTS_PER_G;

	*roll = atan2(ay, az)*180/M_PI;
	*pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;
}

/*
 * Function : 
 * 		Read acceleration data from inerital sensor and updat
 *
 * Parameters : 
 * 		configuration: Inertial Sensor Config storage for acceleration data.
 */
void read_full_xyz(mma8451q_acc_t *acc)
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	
	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) 
	{
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc->xyz[0] = temp[0]/4;
	acc->xyz[1] = temp[1]/4;
	acc->xyz[2] = temp[2]/4;
}
