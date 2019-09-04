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
 * phExPos.h: phExPos application Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
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
#include "phExPos_Cfg.h"

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
extern phRtos_QueueHandle_t gphExPos_Clif_SysTask_Queue;

extern uint8_t gphExPos_Clif_PollLoopLOGCount;
extern void phExPos_TimerCallBackFunc(void);

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
void phExPos_SystemTask(void *pvParams);

#endif /* PHEXEMVCO_H */
