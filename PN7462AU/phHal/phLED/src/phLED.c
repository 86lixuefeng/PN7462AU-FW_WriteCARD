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
 * phExMain_Gpio.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-07-27 09:47:10 +0530 (Mon, 27 Jul 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14132 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_LED
#include "phhalGpio.h"
#include "phLED.h"
#include "phUser.h"
#include "ph_Reg.h"
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

PH_NOINIT static uint32_t gdwBlinkPosition;

PH_NOINIT static const uint8_t * gBlinkPattern;

const uint8_t gkphLED_BootUp[PH_LED_BLINK_LEN] = {L_B___,L__G__,L___Y_,L____R,0,0};
const uint8_t gkphLED_StandBy[PH_LED_BLINK_LEN] = {L_____,L_____,L_____,L_____,L_____,L_____};

const uint8_t gkphLED_Clif_Detect[PH_LED_BLINK_LEN] = {L___Y_,L___Y_,L___Y_,L___Y_,L___Y_,L___Y_};
const uint8_t gkphLED_Clif_Pass[PH_LED_BLINK_LEN] = {L__GY_,L__GY_,L__GY_,L__GY_,L__GY_,L__GY_};
const uint8_t gkphLED_Clif_Fail[PH_LED_BLINK_LEN] = {L___YR,L___YR,L___YR,L___YR,L___YR,L___YR};
const uint8_t gkphLED_Clif_Ongoing[PH_LED_BLINK_LEN] = {L__GY_,L___Y_,L__GY_,L___Y_,L__GY_,L___Y_};

const uint8_t gkphLED_Ct_Detect[PH_LED_BLINK_LEN]   = {L_B___,L_B___,L_B___,L_B___,L_B___,L_B___};
const uint8_t gkphLED_Ct_Ongoing[PH_LED_BLINK_LEN]  = {L_BG__,L_B___,L_BG__,L_B___,L_BG__,L_B___};
const uint8_t gkphLED_Ct_Pass[PH_LED_BLINK_LEN]     = {L_BG__,L_BG__,L_BG__,L_BG__,L_BG__,L_BG__};
const uint8_t gkphLED_Ct_Fail[PH_LED_BLINK_LEN]     = {L_B__R,L_B__R,L_B__R,L_B__R,L_B__R,L_B__R};
const uint8_t gkphLED_Ct_Removed[PH_LED_BLINK_LEN] = {L_B__R,L_B__R,L__GY_,L__GY_,0,0};

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phLED_TimerCallback(void) {
    uint32_t value_now;
    if ( NULL == gBlinkPattern) {
        return;
    }
    gdwBlinkPosition++;
    if ( gdwBlinkPosition >= PH_LED_BLINK_LEN)
        gdwBlinkPosition = 0;
    value_now = (uint32_t )gBlinkPattern[gdwBlinkPosition];
    if ( value_now == 0 ) {
        /* End of pattern */
        gdwBlinkPosition = 0;
        value_now = (uint32_t )gBlinkPattern[0];
    }
    PH_USER_ASSERT(0 != value_now);
    PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,(LED_B == (value_now & LED_B)));
    PH_HAL_GPIO_SETGPIOVAL(GPIO_RED,(LED_R == (value_now & LED_R)));
    PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW,(LED_Y == (value_now & LED_Y)));
    PH_HAL_GPIO_SETGPIOVAL(GPIO_GREEN,(LED_G == (value_now & LED_G)));
}

void phLED_SetPattern(const uint8_t * newPattern) {
    if ( newPattern != gBlinkPattern ) {
        gdwBlinkPosition = 0;
        gBlinkPattern = newPattern;
        phLED_TimerCallback();
    }
}

void phLED_Init(void) {
    gdwBlinkPosition = 0;
    gBlinkPattern = gkphLED_BootUp;
    phhalPcr_ConfigOutput(GPIO_BLUE, true, false);
    phhalPcr_ConfigOutput(GPIO_RED, true, false);
    phhalPcr_ConfigOutput(GPIO_GREEN, true, false);
    phhalPcr_ConfigOutput(GPIO_YELLOW, true, false);
}

void phLED_SetStatus(uint32_t dwLeds)
{
    gBlinkPattern = NULL;
    PH_HAL_GPIO_SETGPIOVAL(GPIO_BLUE,   (LED_B == (dwLeds & LED_B)));
    PH_HAL_GPIO_SETGPIOVAL(GPIO_RED,    (LED_R == (dwLeds & LED_R)));
    PH_HAL_GPIO_SETGPIOVAL(GPIO_YELLOW, (LED_Y == (dwLeds & LED_Y)));
    PH_HAL_GPIO_SETGPIOVAL(GPIO_GREEN,  (LED_G == (dwLeds & LED_G)));
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_LED */
