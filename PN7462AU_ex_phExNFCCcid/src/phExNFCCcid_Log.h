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

/** @file
 *
 * phExNFCCcid_Log.h : Facilitates the LOG feature while running Debug sessions.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

#ifndef PHEXNFCCCID_LOG_H
#define PHEXNFCCCID_LOG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phhalPcr.h"
#include "phacDiscLoop.h"
#include "phFlashBoot_Event.h"
#include "phExNFCCcid_Cfg.h"
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
    void phExNFCCcid_Log_BootReason(phhalPcr_BootReason_t br);
    void phExNFCCcid_Log_DiscLoop(phStatus_t st, uint8_t bPollingLoopEntryCount);
    void phExNFCCcid_Log_FlashBootEventIds(phFlashBoot_Event_Ids st);
    /* Log array of Uint8 */
#else /* PHFL_LOG_ENABLE */
#   define phExNFCCcid_Log_BootReason(X)
#   define phExNFCCcid_Log_DiscLoop(X, Y)
#   define phExNFCCcid_Log_FlashBootEventIds(X)
#endif /* PHFL_LOG_ENABLE */


#endif /* PHEXNFCCCID_LOG_H */
