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
 * phExDoorAccess_Log.h : Facilitates the LOG feature while running Debug sessions.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */

#ifndef PHEXDOORACCESS_LOG_H
#define PHEXDOORACCESS_LOG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <phExDoorAccess_Cfg.h>
#include "ph_Datatypes.h"
#include "phhalPcr.h"
#include "phacDiscLoop.h"
#include "phFlashBoot_Event.h"
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
    void phExDoorAccess_Log_BootReason(phhalPcr_BootReason_t br);
    void phExDoorAccess_Log_DiscLoop(phStatus_t st, uint8_t bPollingLoopEntryCount);
    void phExDoorAccess_Log_FlashBootEventIds(phFlashBoot_Event_Ids st);
    /* Log array of Uint8 */
#else /* PHFL_LOG_ENABLE */
#   define phExDoorAccess_Log_BootReason(X)
#   define phExDoorAccess_Log_DiscLoop(X, Y)
#   define phExDoorAccess_Log_FlashBootEventIds(X)
#endif /* PHFL_LOG_ENABLE */


#endif /* PHEXDOORACCESS_LOG_H */
