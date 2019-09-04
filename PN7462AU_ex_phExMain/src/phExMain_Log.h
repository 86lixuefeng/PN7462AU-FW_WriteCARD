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
 * phExMain_Log.h : Facilitates the LOG feature while running Debug sessions.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-04 16:39:24 +0530 (Mon, 04 May 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12183 $ (v05.19.00)
 */

#ifndef PHEXMAIN_LOG_H
#define PHEXMAIN_LOG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phhalPcr.h"
#include "phacDiscLoop.h"
#include "phFlashBoot_Event.h"
#include "phExMain_Cfg.h"
#include "ph_Log.h"
/****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

#if PHFL_LOG_ENABLE
    void phExMain_Log_BootReason(phhalPcr_BootReason_t br);
    void phExMain_Log_DiscLoop(phStatus_t st, uint8_t bPollingLoopEntryCount);
    void phExMain_Log_FlashBootEventIds(phFlashBoot_Event_Ids st);
    /* Log array of Uint8 */
#else /* PHFL_LOG_ENABLE */
#   define phExMain_Log_BootReason(X)
#   define phExMain_Log_DiscLoop(X, Y)
#   define phExMain_Log_FlashBootEventIds(X)
#endif /* PHFL_LOG_ENABLE */


#endif /* PHEXMAIN_LOG_H */
