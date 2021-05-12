/*-------------------------------------------------------------------------------------------------
 *  Init_MMA.c
 *
 *  Created on: May 1, 2021
 *      Author: mehul
 *
 *       File brief : Initialization  of MA8451Q accelerometer sensor communication
 *-------------------------------------------------------------------------------------------------
 */
#include <Init_MMA.h>
#include <MMA8451Q.h>
#include "i2c.h"
#include "assert.h"
#include "MKL25Z4.h"
#include "led.h"
#include "LOG.h"

#define MASK(x)		(1UL << (x))


#define MMA8451Q_INT_PORT	PORTA				/*! Port at which the MMA8451Q INT1 and INT2 pins are attached */
#define MMA8451Q_INT_GPIO	GPIOA				/*! Port at which the MMA8451Q INT1 and INT2 pins are attached */
#define MMA8451Q_INT1_PIN	14					/*! Pin at which the MMA8451Q INT1 is attached */
#define MMA8451Q_INT2_PIN	15					/*! Pin at which the MMA8451Q INT2 is attached */

static union
{
    mma8451q_confreg_t mma8451q_configuration;
} config_buffer;
void Init_MMA8451Q()
{

	mma8451q_confreg_t *configuration = &config_buffer.mma8451q_configuration;

	LOG("\n\rInitializing MMA8451Q Sensor\n\r");

	/* configure interrupts for accelerometer */
    /* INT1_ACCEL is on PTA14, INT2_ACCEL is on PTA15 */
    SIM->SCGC5 |= (1 << SIM_SCGC5_PORTA_SHIFT) & SIM_SCGC5_PORTA_MASK; /* power to the masses */
    MMA8451Q_INT_PORT->PCR[MMA8451Q_INT1_PIN] = PORT_PCR_MUX(0x1) | PORT_PCR_IRQC(0b1010) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* interrupt on falling edge, pull-up for open drain/active low line */
    MMA8451Q_INT_PORT->PCR[MMA8451Q_INT2_PIN] = PORT_PCR_MUX(0x1) | PORT_PCR_IRQC(0b1010) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* interrupt on falling edge, pull-up for open drain/active low line */
    MMA8451Q_INT_GPIO->PDDR &= ~(GPIO_PDDR_PDD(1 << MMA8451Q_INT1_PIN) | GPIO_PDDR_PDD(1 << MMA8451Q_INT2_PIN));

    /* prepare interrupts for pin change / PORTA */
	NVIC_SetPriority(PORTA_IRQn, 2); 		// 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);

    uint8_t id = MMA8451Q_WhoAmI();			//To check slave id

    if(id == 0x1A)
	{
    	assert(1);
    }
    else
	{
    	assert(0);
    }
    LOG("\n\rMMA8451Q: Recognized");

    MMA8451Q_Reset();
    delay_ms(20);

    MMA8451Q_SetDataRate(configuration, MMA8451Q_DATARATE_800Hz, MMA8451Q_LOWNOISE_ENABLED);	// Update Rate and Low Noise Setup Read

    // Enter Active Mode
    MMA8451Q_EnterActiveMode();

    LOG("\n\rMMA8451Q: configuration done.");
}
