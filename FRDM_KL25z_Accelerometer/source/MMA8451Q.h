/*-------------------------------------------------------------------------------------------------
 * MMA8451Q.h
 *
 * Created on: May 1, 2021
 *      Author: mehul
 *
 * File brief : Header file for function related to MMA8451Q sensor interfacing,
 * 				Accelerometer raw reading,
 * 				and based on Acceleration value conversion to Roll,pitch, yaw angle
 *-------------------------------------------------------------------------------------------------
 */

#ifndef MMA8451Q_H_
#define MMA8451Q_H_

#include "MKL25Z4.h"
#include "i2c.h"


/*
 * Acquisition data rate
 */
typedef enum {
	MMA8451Q_DATARATE_800Hz	= (0b000),		/*< 800Hz acquisition data rate */
	MMA8451Q_DATARATE_400Hz	= (0b001),		/*< 400Hz acquisition data rate */
	MMA8451Q_DATARATE_200Hz	= (0b010),		/*< 200Hz acquisition data rate */
	MMA8451Q_DATARATE_100Hz	= (0b011),		/*< 100Hz acquisition data rate */
	MMA8451Q_DATARATE_50Hz	= (0b100),		/*< 50Hz acquisition data rate */
	MMA8451Q_DATARATE_12p5Hz= (0b101),		/*< 12.5Hz acquisition data rate */
	MMA8451Q_DATARATE_6p2Hz	= (0b110),		/*< 6.25Hz acquisition data rate */
	MMA8451Q_DATARATE_1p5Hz	= (0b111)		/*< 1.56Hz acquisition data rate */
} mma8451q_datarate_t;



/*
 * Reduced noise mode
 */
typedef enum {
	MMA8451Q_LOWNOISE_DISABLED	= (0b0),	/*< Reduced noise/reduced maximum range disabled */
	MMA8451Q_LOWNOISE_ENABLED	= (0b1)		/*< Reduced noise/reduced maximum range enabled */
} mma8451q_lownoise_t;


/*
 * 	Accelerometer data
 */
#pragma pack(1)
typedef struct __attribute__ ((__packed__))
{
	uint8_t :8; 		/*< padding byte */
	uint8_t status;		/*< the status register contents */
	union {
		struct {
			int16_t x;			/*< the x acceleration */
			int16_t y;			/*< the y acceleration */
			int16_t z;			/*< the z acceleration */
		};
		int16_t xyz[3];
	};
} mma8451q_acc_t;

/*
 * 	The MMA8451Q configuration registers
 */
typedef struct {

	const uint8_t WHO_AM_I;		/*< Device ID (0x1A), register address 0x0D */


	uint8_t CTRL_REG1;			/*< ODR, Active/Standby mode, register address 0x2A */
	uint8_t CTRL_REG2;			/*< Sleep Enable, OS Modes, RST, ST, register address 0x2B */
	uint8_t CTRL_REG3;			/*< Wake from Sleep, IPOL, PP_OD, register address 0x2C */
	uint8_t CTRL_REG4;			/*< Interrupt enable register, register address 0x2D */
	uint8_t CTRL_REG5;			/*< Interrupt pin (INT1/INT2) map, register address 0x2E */

} mma8451q_confreg_t;

/*
 * Function :
 * Macro for usage in configuration commands that executes configuration changes directly
 */
#define MMA8451Q_CONFIGURE_DIRECT ((mma8451q_confreg_t*)0x0)

/*
 * Function :
 * 			Brings the MMA8451Q into passive mode
 */
void MMA8451Q_EnterPassiveMode();
/*
 * Function :
 * Resets the MMA8451Q
 */
 void MMA8451Q_Reset();
 /*
 * Function :
 *		 Brings the MMA8451Q into active mode
 * Parameters
 * 			:  configuration The configuration structure or {@see MMA8451Q_CONFIGURE_DIRECT} if changes should be sent directly over the wire.
 */

void MMA8451Q_EnterActiveMode();

/*
 * Function :
 *  Parameters :
 *   			 Reads the WHO_AM_I register from the MMA8451Q.
 *	Return:
 *   			Device identification code; Should be 0b00011010 (0x1A).
 */
uint8_t MMA8451Q_WhoAmI();

void MMA8451Q_InitializeData(mma8451q_acc_t *const data);
/*
 * Function : 
 * 		Sets the data rate and the active mode
 * Parameters : configuration The configuration structure or {@see MMA8451Q_CONFIGURE_DIRECT} if changes should be sent directly over the wire.
 * 				datarate The data rate
 *			   lownoise The low noise state
 */
void MMA8451Q_SetDataRate(mma8451q_confreg_t *const configuration, register mma8451q_datarate_t datarate, mma8451q_lownoise_t lownoise);




/*
 * Function : 
 * 		Convert the acceleration data read to roll and pitch
 *
 * Parameters : 1) configuration: Inertial Sensor Configuration to read from
 * 		   		2) roll: float angle tilt calculated in degrees
 * 		   		3) pitch: float angle tilt calculated in degrees
 */
void convert_xyz_to_roll_pitch(mma8451q_acc_t *acc, double *roll, double  *pitch);

/*
 * Function : 
 * 		Read acceleration data from inerital sensor and update
 *
 * Parameters : 
 *  	configuration: Inertial Sensor Config storage for acceleration data.
 */
void read_full_xyz(mma8451q_acc_t *acc);


/*
 * Function : calibration of accelerometer 	 ​
 *
 * ​ Parameters: ​ none
​ *
​ * ​ ​Returns: ​ none
 */

void calibrate_accelerometer(void);

#endif /* MMA8451Q_H_ */
