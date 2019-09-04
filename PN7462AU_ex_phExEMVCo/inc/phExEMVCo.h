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
 * phExEMVCo.h: phExEMVCo application Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-05-05 10:51:29 +0530 (Thu, 05 May 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18357 $ (v05.19.00)
 */

#ifndef PHEXEMVCO_H
#define PHEXEMVCO_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phRtos.h"
#include "phExEMVCo_Cfg.h"

#ifdef __CODE_RED
#    include <stdio.h>
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXEMVCo_USETXLDO_EXTERNAL               0x0
#define PH_EXEMVCo_TXLDO_MONITOR_ENABLE            0x1
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
extern phRtos_QueueHandle_t gphExEMVCo_Clif_SysTask_Queue;

extern uint8_t gphExEMVCo_Clif_PollLoopLOGCount;
extern void phExEMVCo_TimerCallBackFunc(void);

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
void phExEMVCo_SystemTask(void *pvParams);


#endif /* PHEXEMVCO_H */
