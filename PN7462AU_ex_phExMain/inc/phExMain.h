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
 * phExMain.h: phExMain application Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-02-08 14:51:19 +0530 (Mon, 08 Feb 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17825 $ (v05.19.00)
 */

#ifndef PHEXMAIN_H
#define PHEXMAIN_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phRtos.h"
#include "phExMain_Cfg.h"
#ifdef PHFL_HALAPI_WITH_RTOS
#ifdef __CODE_RED
#    include <stdio.h>
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
extern phRtos_QueueHandle_t gphExMain_Clif_SysTask_Queue;

/* Allow roll over to 255, but wait for some time when switching from CT to CLIF
 * before showing the logging message of Polling Loop Started
 */
#define PH_EXMAIN_CLIF_POLLING_ROLLOVER_COUNT 253
extern uint8_t gphExMain_Clif_PollLoopLOGCount;
extern void phExMain_TimerCallBackFunc(void);

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
void phExMain_SystemTask(void *pvParams);
#define PH_ENABLE                       0x0001U /**< Enabled. */
#define PH_DISABLE                      0x0000U /**< Disabled. */

#endif /*PHFL_HALAPI_WITH_RTOS*/
#endif /* PHEXMAIN_H */
