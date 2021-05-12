/**----------------------------------------------------------------------------------------------------------------
 *
 *
 * File brief  : i2c_driver.h
 *
 *      Author: mehul
 *----------------------------------------------------------------------------------------------------------------
 */
#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

//data tyes
typedef enum {
	eUSCI_IDLE = 0,
	eUSCI_SUCCESS = 0,
	eUSCI_BUSY = 1,
	eUSCI_NACK = 2,
	eUSCI_STOP,
	eUSCI_START
} eUSCI_status;
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */


extern void Init_I2C(void);
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
extern bool writeI2C(uint8_t ui8Addr, uint8_t ui8Reg, uint8_t *Data, uint8_t ui8ByteCount);
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
extern bool readI2C(uint8_t ui8Addr, uint8_t ui8Reg, uint8_t *Data, uint8_t ui8ByteCount);
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */
extern bool readBurstI2C(uint8_t ui8Addr, uint8_t ui8Reg, uint8_t *Data, uint32_t ui32ByteCount);
#endif /* _I2C_DRIVER_H_ */
