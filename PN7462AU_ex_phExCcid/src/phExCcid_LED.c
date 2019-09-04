/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * phExCcid_LED.c: Contains the LEDs related operations and the GPIO configurations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phhalGpio.h"
#include "ph_Reg.h"
#include "phExCcid_LED.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define GPIO_BLUE    (9)
#define GPIO_GREEN   (10)
#define GPIO_YELLOW  (11)
#define GPIO_RED     (12)

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
const uint8_t gkphLED_TVDD_Fail[PH_LED_BLINK_LEN] = {L____R,L_____,L____R,L_____,L____R,L_____};
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phExCcid_LED_Status(uint8_t bLedColor, uint8_t bOnOff)
{
    PH_HAL_GPIO_SETGPIOVAL(bLedColor, bOnOff);
}

void phExCcid_All_LED_Off()
{
    PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,false);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_GREEN,false);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,false);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_RED,false);
}

void phExCcid_All_LED_On()
{
    PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,true);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_GREEN,true);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,true);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_RED,true);
}

void phExCcid_LED_TxnPass(uint8_t bSlotType)
{
    PH_HAL_GPIO_SETGPIOVAL(GPIO_RED,false);

    if (bSlotType == 0x01)
    {
        /* CL Slot glow Yellow Led */
        PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,true);
    }
    else if (bSlotType == 0x02)
    {
        /* CT Slot glow Blue Led */
        PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,true);
    }
    else
    {
        PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,false);
        PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,false);
    }

    PH_HAL_GPIO_SETGPIOVAL(GPIO_GREEN,true);
}

void phExCcid_LED_TxnFail(uint8_t bSlotType)
{
    PH_HAL_GPIO_SETGPIOVAL(GPIO_GREEN,false);

    if (bSlotType == 0x01)
    {
        /* CL Slot glow Yellow Led */
        PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,true);
    }
    else if (bSlotType == 0x02)
    {
        /* CT Slot glow Blue Led */
        PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,true);
    }
    else
    {
        PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,false);
        PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,false);
    }

    PH_HAL_GPIO_SETGPIOVAL(GPIO_RED,true);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
