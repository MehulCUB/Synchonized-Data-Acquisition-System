/*----------------------------------------------------------------------------------------------------------------
 * system_clock.c
 *
 * File brief  : system clock initialization and  Crystal Oscillator initialization functions
 *
 *  Author: mehul
 *----------------------------------------------------------------------------------------------------------------
 */


#include "system_clock.h"


/*
 * Function :  Initialize CrystalOscillator 
 *
 * Parameters : None
 *
 * Returns  : None
 *
 */
void startCrystalOscillator(void)
{
    /* Configuring pins for peripheral/crystal HFXT*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN3 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring pins for peripheral/crystal LFXT*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
}

/*
 * Function :   setting up the system clock at a specified frequency.
 *
 * Parameters : None
 *
 * Returns  : None
 *
 */
void setSystemClock(uint32_t CPU_Frequency)
{
    /* Setting the external clock frequency. This API is optional, but will
    * come in handy if the user ever wants to use the getMCLK/getACLK/etc
    * functions
    */
    MAP_CS_setExternalClockSourceFrequency(32768, CPU_Frequency);
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ);

    /* Before we start we have to change VCORE to 1 to support the 24MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_AM_LDO_VCORE0);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

    /* Starting HFXT and LFXT in non-bypass mode without a timeout. */
    MAP_CS_startHFXT(false);
    MAP_CS_startLFXT(false);

    /* Initializing the clock source as follows:
    *      MCLK = HFXT/2 = 24MHz
    *      ACLK = LFXT = 32KHz
    *      HSMCLK = HFXT/4 = 6MHz
    *      SMCLK = HFXT/2 = 12MHz
    *      BCLK  = REFO = 32kHz
    */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_8);
    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}
