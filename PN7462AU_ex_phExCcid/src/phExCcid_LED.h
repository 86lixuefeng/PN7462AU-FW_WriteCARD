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
 * phExCcid_Led.h:  Contains the LEDs operations related Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */


#ifndef PHEXCCID_LED_H
#define PHEXCCID_LED_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phLED.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define LED_ON     1
#define LED_OFF    0
#define BLUE_LED   9
#define GREEN_LED  10
#define YELLOW_LED 11
#define RED_LED    12

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */
extern const uint8_t gkphLED_TVDD_Fail[PH_LED_BLINK_LEN];
/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
void phExCcid_LED_Status(uint8_t bLedColor, uint8_t bOnOff);
void phExCcid_All_LED_Off();
void phExCcid_All_LED_On();

void phExCcid_LED_TxnPass(uint8_t bSlotType);
void phExCcid_LED_TxnFail(uint8_t bSlotType);

#endif /* PHEXCCID_LED_H_ */
