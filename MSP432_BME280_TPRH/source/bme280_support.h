/*----------------------------------------------------------------------------------------------------------------
 * leveraged and modifed code of Bosch Sensortec GmbH
 *
 * File brief  : bme280_support.h
 *
 *      Author: mehul
 *----------------------------------------------------------------------------------------------------------------
 */
/*
*
****************************************************************************
* Copyright (C) 2014 Bosch Sensortec GmbH
*
* File : bme280_support.h
*
* Date :
*
* Revision : $
*
* Usage: Sensor Driver support file for BME280 sensor
**************************************************************************/
/*! \file bm280_support.h
    \brief BME280 Sensor Driver Support Header File */
/* user defined code to be added here ... */
/*---------------------------------------------------------------------------*/
/* Includes*/
/*---------------------------------------------------------------------------*/
/* This function is an example for reading sensor data
 *	\param: None
 *	\return: communication result
 */
#ifndef __BME280_SUPPORT_H__
#define __BME280_SUPPORT_H__

#include "bme280.h"

/*******************************************/
/**\name	CONSTANTS        */
/******************************************/
#define         BME280_ZERO_U8X				((u8)0)
#define         BME280_ONE_U8X				((u8)1)
#define         BME280_TWO_U8X				((u8)2)

/*!
 *	@brief This function used for initialize the sensor
 *
 *
 *	@return results of bus communication function
 *
 *
 */
/*
 * Function :
 *
 * Parameters :
 *
 * Returns  : None
 *
 */

extern s32 bme280_data_readout_template(void);
#endif
