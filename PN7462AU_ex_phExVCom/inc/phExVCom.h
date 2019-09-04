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
 * phExVCom.h: phExVCom application Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-10-20 10:18:10 +0530 (Thu, 20 Oct 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18736 $ (v05.19.00)
 */

#ifndef PHEXVCOM_H
#define PHEXVCOM_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <phExVCom_Cfg.h>
#include <VComInterface.h>
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phRtos.h"
#ifdef PHFL_HALAPI_WITH_RTOS
#ifdef __CODE_RED
#    include <stdio.h>
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define LOG_TXT_VCOM(TXT) {VCom_Write((uint8_t*)TXT, (uint32_t)(sizeof(TXT)));}

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
extern phRtos_QueueHandle_t gphExVCom_Clif_SysTask_Queue;

extern uint8_t gphExVCom_Clif_PollLoopLOGCount;
extern void phExVCom_TimerCallBackFunc(void);

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
void phExVCom_SystemTask(void *pvParams);
#define PH_ENABLE                       0x0001U /**< Enabled. */
#define PH_DISABLE                      0x0000U /**< Disabled. */

#endif /*PHFL_HALAPI_WITH_RTOS*/
#endif /* PHEXVCOM_H */
