/*
 *         Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** \file
 * Application specific selection of Reader Library Components
 *
 * $Author: Jaganmohan Reddy (nxp95127) $
 * $Revision: 1 $ (v05.21.00)
 * $Date: 2017-11-08 11:30:00 +0530 (Wed, 08 Nov 2017) $
 *
 */

#ifndef INTFS_NFCRDLIB_SIMPLIFIEDAPI_EMVCO_INTEROP_STATUS_H_
#define INTFS_NFCRDLIB_SIMPLIFIEDAPI_EMVCO_INTEROP_STATUS_H_
#include <ph_Status.h>

#define EMVCO_INTEROP_ERR_SUCCESS    0U
#define EMVCO_INTEROP_ERR_FAIL       1U

#if !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD)

#if defined(PHDRIVER_LPC1769PN5180_BOARD) || defined(PHDRIVER_LPC1769RC663_BOARD)
#define EMVCO_INTEROP_LED_ON    PH_DRIVER_SET_HIGH
#define EMVCO_INTEROP_LED_OFF   PH_DRIVER_SET_LOW
#endif

#if defined(PHDRIVER_FRDM_K82FPN5180_BOARD) || defined(PHDRIVER_FRDM_K82FRC663_BOARD)
#define EMVCO_INTEROP_LED_ON    PH_DRIVER_SET_LOW
#define EMVCO_INTEROP_LED_OFF   PH_DRIVER_SET_HIGH
#endif

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
#define GREEN_LED_PN7462AU   (10U)  /* GREEN LED connected to GPIO 10 */
#define RED_LED_PN7462AU     (12U)  /* RED LED connected to GPIO 12 */
#endif

#ifndef NXPBUILD__PHHAL_HW_PN7462AU
#define EMVCO_INTEROP_STAT_GLED_ON()    phDriver_PinWrite(PHDRIVER_PIN_GLED, EMVCO_INTEROP_LED_ON)
#define EMVCO_INTEROP_STAT_RLED_ON()    phDriver_PinWrite(PHDRIVER_PIN_RLED, EMVCO_INTEROP_LED_ON)
#define EMVCO_INTEROP_STAT_GLED_OFF()   phDriver_PinWrite(PHDRIVER_PIN_GLED, EMVCO_INTEROP_LED_OFF)
#define EMVCO_INTEROP_STAT_RLED_OFF()   phDriver_PinWrite(PHDRIVER_PIN_RLED, EMVCO_INTEROP_LED_OFF)
#else
#define EMVCO_INTEROP_STAT_GLED_ON()    PH_HAL_GPIO_SETGPIOVAL(GREEN_LED_PN7462AU, true)
#define EMVCO_INTEROP_STAT_RLED_ON()    PH_HAL_GPIO_SETGPIOVAL(RED_LED_PN7462AU, true)
#define EMVCO_INTEROP_STAT_GLED_OFF()   PH_HAL_GPIO_SETGPIOVAL(GREEN_LED_PN7462AU, false)
#define EMVCO_INTEROP_STAT_RLED_OFF()   PH_HAL_GPIO_SETGPIOVAL(RED_LED_PN7462AU, false)
#endif

#endif /* !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD) */

extern phStatus_t Emvco_Interop_Init(void);
extern void Emvco_Interop_Status(uint8_t Error_Status);

#endif /* INTFS_NFCRDLIB_SIMPLIFIEDAPI_EMVCO_INTEROP_STATUS_H_ */