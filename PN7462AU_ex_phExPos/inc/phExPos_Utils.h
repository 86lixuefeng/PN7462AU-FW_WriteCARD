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
 * phExPos_Utils.h:  SPI Api signatures and other declarations.
 *
 *
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */

#ifndef PHEXPOS_UTILS_H
#define PHEXPOS_UTILS_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include "phExPos.h"

void phExPos_UtilsPOS_Init(void);
void phExPos_UtilsGenerateATRFromAts(uint8_t *);
void phExPos_UtilsExecuteControlCmd(void);
void phExPos_UtilsSendResponseToHost(void);
void phExPos_UtilsConfig_GPIO(void);
void phExPos_UtilsInitGlobals(void);
void phExPos_UtilsAll_LEDS_OFF(void);
void phExPos_UtilsGlow_Success_LEDS(void);
void phExPos_UtilsGlow_Error_LEDS(void);
void phExPos_UtilsGlow_CLIF_Card_Detection_LED(void);
void phExPos_UtilsGlow_CT_Card_Detection_LED(void);
void phExPos_Utils_Raise_Interrupt(void);
void phExPos_Utils_Clear_Interrupt(void);
#endif  /* PHEXPOS_UTILS_H */

