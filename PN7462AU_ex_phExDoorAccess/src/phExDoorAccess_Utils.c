/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/** @file
 *
 * phExDoorAccessPoll.c: Contains the core logic for the Clif Reader part.
 *
 * Project:  PN7462AU
 *
 * $Date$
 * $Author$
 * $Revision$ (v05.19.00)
 */

#include <phExDoorAccess_Utils.h>

#define PH_EXDOORACCESS_BLUE_LED                            0x09U
#define PH_EXDOORACCESS_GREEN_LED                           0x0AU
#define PH_EXDOORACCESS_YELLOW_LED                          0x0BU
#define PH_EXDOORACCESS_RED_LED                             0x0CU

volatile uint32_t gphExDoorAccess_Utils_CounterValue;
volatile float gphExDoorAccess_Utils_Timeinms;
phhalHif_Config_t gphExDoorAccess_Utils_HsuHifConfig;
uint8_t gphExDoorAccess_Utils_SysHsuTxBuffer[PH_EXDOORACCESS_UTILS_HSU_BUFFER_SIZE];
uint16_t str_index;


void phExDoorAccess_Utils_HsuHifConfig(void)
{
    /*Configure HSU interface*/
    gphExDoorAccess_Utils_HsuHifConfig.eInterface = E_HIF_HSU;
    gphExDoorAccess_Utils_HsuHifConfig.sConfig.sHsuConfig.bBaudRate = E_HSU_BAUDRATE_9_6K;
    gphExDoorAccess_Utils_HsuHifConfig.sConfig.sHsuConfig.bDummyBytes = 0;

    gphExDoorAccess_Utils_HsuHifConfig.sConfig.sHsuConfig.bStopBits = 1;
    gphExDoorAccess_Utils_HsuHifConfig.sConfig.sHsuConfig.bEOF = 0x8;
    gphExDoorAccess_Utils_HsuHifConfig.bTimeout = 0x00;

    gphExDoorAccess_Utils_HsuHifConfig.eBufferType = E_BUFFER_FORMAT_FREE;
    gphExDoorAccess_Utils_HsuHifConfig.bShortFrameLen = 0;
    gphExDoorAccess_Utils_HsuHifConfig.bStoreErrData = 0;

    phhalHif_Init(&gphExDoorAccess_Utils_HsuHifConfig,(pphhalHif_Callback_t)&phExDoorAccess_Utils_HsuHifErrorCallBack);
}

void phExDoorAccess_Utils_HsuHifReceiveCallBack(uint32_t dwIntStatus)
{
	phhalHif_ReleaseRxBuffer(E_RX_BUFFER_ID0);// Release the receive buffer so that PN640 can receive the next packet.
}


void phExDoorAccess_Utils_HsuHifTransmitCallBack(uint32_t dwIntStatus)
{
    memset(gphExDoorAccess_Utils_SysHsuTxBuffer,0,PH_EXDOORACCESS_UTILS_HSU_BUFFER_SIZE);
}

void phExDoorAccess_Utils_HsuHifErrorCallBack(uint32_t dwIntStatus)
{
// Add the code which is to be processed during HIF error interrupt.
}

void phExDoorAccess_Utils_Hsu_Print(uint8_t *inBuf)
{
    /*Print on the PC console using HSU interface through USB to serial converter*/
#ifdef PHFL_ENABLE_HSU
    phhalHif_Transmit((uint32_t *)inBuf,(uint16_t)strlen((const char *)inBuf),(pphhalHif_Callback_t)&phExDoorAccess_Utils_HsuHifTransmitCallBack);
#endif
}

void phExDoorAccess_Utils_Fill_Tx_Buffer(char inString[])
{
	uint8_t i;
	uint8_t j = 0;
	for (i = str_index; i <= (str_index + strlen(inString)); i++)
	{
		gphExDoorAccess_Utils_SysHsuTxBuffer[i] = inString[j];
		j++;
	}
	str_index += strlen(inString);
}
void phExDoorAccess_Utils_FillUID_Tx_Buffer(uint8_t *inUIDString,uint8_t UID_Length)
{
    uint8_t count;
    /*Convert Hex numbers to Ascii charaters to print on PC Console*/
	for (count = 0; count < UID_Length; count++)
	{
		gphExDoorAccess_Utils_SysHsuTxBuffer[str_index + (count*2)] = hex_numbers[(inUIDString[count] >> 4) & 0x0F];
		gphExDoorAccess_Utils_SysHsuTxBuffer[str_index + (count*2) + 1] = hex_numbers[(inUIDString[count]) & 0x0F];
	}
	str_index +=  2 * UID_Length;
}

void phExDoorAccess_Utils_Config_GPIO(void)
{
    /*configure GPIO pins to glow status LEDs*/
    phhalPcr_ConfigOutput(PH_EXDOORACCESS_BLUE_LED,true,false); /*Blue LED*/
    phhalPcr_ConfigOutput(PH_EXDOORACCESS_GREEN_LED,true,false); /*Green LED*/
    phhalPcr_ConfigOutput(PH_EXDOORACCESS_YELLOW_LED,true,false); /*Yellow LED*/
    phhalPcr_ConfigOutput(PH_EXDOORACCESS_RED_LED,true,false); /*Red LED*/
}

void phExDoorAccess_Utils_Glow_Error_LEDS(void)
{
    /*Blink Red LED 3 times to indicate the Error*/
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_RED_LED,true);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_RED_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_RED_LED,true);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_RED_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_RED_LED,true);
}

void phExDoorAccess_Utils_Glow_Success_LEDS(void)
{
    /*Blink Green LED 3 times to indicate the success*/
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_GREEN_LED,true);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_GREEN_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_GREEN_LED,true);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_GREEN_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_GREEN_LED,true);
}

void phExDoorAccess_Utils_Glow_Authetication_Success_LED(void)
{
    /*Glow Green LED to indicate the successful card authentication*/
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_GREEN_LED,true);
}

void phExDoorAccess_Utils_Glow_Authetication_Failure_LED(void)
{
    /*Glow Red LED to indicate the failed card authentication*/
    PH_HAL_GPIO_SETGPIOVAL(PH_EXDOORACCESS_RED_LED,true);
}

void phExDoorAccess_Utils_Timer1_Enable(void)
{
#ifdef PHFL_API_TIMING_MEASUREMENT
    PH_HALREG_SETREG(TIMERS_TIMER2_CONTROL_REG, 1);
#endif
}

void phExDoorAccess_Utils_Timer1_Start(void)
{
    /*This function is used for timing performance measurement*/
#ifdef PHFL_API_TIMING_MEASUREMENT
    PH_HALREG_SETREG(TIMERS_TIMER2_TIMEOUT_REG, 0xffffffff);
#endif
}

void phExDoorAccess_Utils_Get_Time(char *str)
{
    /*This function is used for timing performance measurement*/
#ifdef PHFL_API_TIMING_MEASUREMENT
    gphExDoorAccess_Utils_CounterValue = ((0xffffffff - PH_HALREG_GETREG(TIMERS_TIMER2_COUNT_REG) &0xffffffff));
    gphExDoorAccess_Utils_Timeinms = gphExDoorAccess_Utils_CounterValue * 0.00005;
    printf("%s Time = %f ms\n", str, gphExDoorAccess_Utils_Timeinms);
#endif
 }
/*------------------------------------------------END OF HIF MODULE FUNCTIONS --------------------------------------------*/
