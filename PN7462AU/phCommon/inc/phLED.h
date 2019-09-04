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
 * \addtogroup led
 *
 * LED Handling
 *
 * @{
 *
 * phLED.h:  Generic helper utility to generated specific pattern on LEDs connected to bard.
 *
 * @warning Specific GPIOs which are connected to specific LEDs are switched on and off using this utility.
 * Hence the behaviour is tightly coupled with the board configuration.
 *
 *
 * This module expects that
 *
 * Project:  PN7462AU
 *
 * To use this module,
 * 1. Ensure that the module is already initialized with \ref phLED_Init
 * 2. Identify a pattern of #PH_LED_BLINK_LEN bytes
 *      e.g.
 *
 *      \code{.c}
 *          / * keep Green On and blink, yellow and red LED * /
 *          static const uint8_t gkphLED_SoFarSoGood[PH_LED_BLINK_LEN] = {L__GY_,L__G_R,L__GY_,L__G_R,L__GY_,L__G_R};
 *      \endcode
 *
 * 3. Initialize the pattern with \ref phLED_SetPattern before the operation.
 *
 *      \code{.c}
 *          phLED_SetPattern(gkphLED_SoFarSoGood);
 *      \endcode
 *
 *
 * 4. Invoke \ref phLED_TimerCallback either from a running timer or at every step of the
 *    ongoing operation.
 *
 *      \code{.c}
 *          phLED_SetPattern()
 *      \endcode
 *
 *
 * $Date: 2015-05-04 16:39:24 +0530 (Mon, 04 May 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12183 $ (v04.13.00)
 *
 */


#ifndef phLED_H
#define phLED_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_LED

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * \addtogroup
 *
 * The number of distinct LED sequnces triggered. See \ref phLED_SetPattern for details. */
#define PH_LED_BLINK_LEN 6

/**
 * \addtogroup ledpatterns Patterns for LED
 *
 * These patterns can be used as described in \ref phLED_SetPattern
 *
 * \ingroup ledpatterns
 *
 * @{
 *
 * \addtogroup bitsforled Bits for LED
 *
 * Bits to identify which LED has to be swithed on.
 *
 * @{
 */
#define LED_B (1 << 1)  /**< Bit to set Blue LED */
#define LED_R (1 << 2)  /**< Bit to set Red LED */
#define LED_Y (1 << 3)  /**< Bit to set Yello LED */
#define LED_G (1 << 4)  /**< Bit to set Green LED */
/** @} */


#define L_BGYR ( LED_B | LED_G | LED_Y | LED_R | 1 ) /**< All 4 LEDs ON */
#define L_BG_R ( LED_B | LED_G |   0   | LED_R | 1 )
#define L_B_YR ( LED_B |   0   | LED_Y | LED_R | 1 )
#define L_B__R ( LED_B |   0   |   0   | LED_R | 1 )
#define L_BGY_ ( LED_B | LED_G | LED_Y |   0   | 1 )
#define L_BG__ ( LED_B | LED_G |   0   |   0   | 1 )
#define L_B_Y_ ( LED_B |   0   | LED_Y |   0   | 1 )
#define L_B___ ( LED_B |   0   |   0   |   0   | 1 )  /**< Only Blue LED ON */

#define L__GYR ( 0 | LED_G | LED_Y | LED_R | 1 )
#define L__G_R ( 0 | LED_G |   0   | LED_R | 1 ) /**< Green and RED LED ON */
#define L___YR ( 0 |   0   | LED_Y | LED_R | 1 ) /**< Yellow and Red LED ON */
#define L____R ( 0 |   0   |   0   | LED_R | 1 )
#define L__GY_ ( 0 | LED_G | LED_Y |   0   | 1 ) /**< Green and Yellow LED ON */
#define L__G__ ( 0 | LED_G |   0   |   0   | 1 ) /**< Only GREEN LED ON */
#define L___Y_ ( 0 |   0   | LED_Y |   0   | 1 ) /**< Only Yellow LED ON */
#define L_____ ( 0 |   0   |   0   |   0   | 1 ) /**< All 4 LEDs OFF */

/** @} */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

extern const uint8_t gkphLED_BootUp[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Clif_Detect[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Clif_Pass[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Clif_Fail[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Clif_Ongoing[PH_LED_BLINK_LEN];

extern const uint8_t gkphLED_Ct_Detect[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Ct_Ongoing[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Ct_Pass[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Ct_Fail[PH_LED_BLINK_LEN];
extern const uint8_t gkphLED_Ct_Removed[PH_LED_BLINK_LEN];

extern const uint8_t gkphLED_StandBy[PH_LED_BLINK_LEN];

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/**
 * \addtogroup ledapi APIs for LED
 * @{
 */

/** Initialize the module and set specific GPIOs to OFF and output mode */
void phLED_Init(void);

/**
 * Set the sequence of the LED Patterns to be shown.
 *
 * @param newPattern A array of #PH_LED_BLINK_LEN bytes.  Each byte in the pattern shows which LED has to be on.
 *
 * Some of the refernce patterns are
 *
 *  - Yellow LED Blinking :              \code {L___Y_,L_____,L___Y_,L_____,L___Y_, L_____ }; \endcode
 *  - Yellow LED Always ON :             \code {L___Y_,L___Y_,L___Y_,L___Y_,L___Y_, L___Y_ }; \endcode
 *  - Yellow LED ON, with RED Blinking : \code {L___YR,L___Y_,L___YR,L___Y_,L___YR, L___Y_ }; \endcode
 *
 */

void phLED_SetPattern(const uint8_t newPattern[PH_LED_BLINK_LEN]);

/** This API triggers/shows the *next* LED pattern in a selected pattern. See \ref phLED_SetPattern */

void phLED_TimerCallback(void);

/**
 * LEDs to be set. To set individual bits, see LED_B,LED_G,LED_Y,LED_R
 *
 * Note: This API takes 32bit parameter to avoid un-necessary optimization.
 */
void phLED_SetStatus(uint32_t u8LED);

/** @} */

#endif /* NXPBUILD__PHHAL_LED */
#endif /* phLED_H_ */

/** @} */
