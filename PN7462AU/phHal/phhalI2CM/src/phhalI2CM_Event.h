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
 * I2C Master event based mechanism.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-07-15 10:55:40 +0530 (Wed, 15 Jul 2015) $
 * $Author: Sugasi Rajesh (nxp69678) $
 * $Revision: 13812 $ (v04.13.00)
 */
#ifndef PHHALI2CM_EVENT_H
#define PHHALI2CM_EVENT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM


/* *****************************************************************************************************************
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
extern phStatus_t phhalI2CM_EventRegister(uint32_t dwTicksToWait) ;
extern phStatus_t phhalI2CM_EventWait(void);
extern phStatus_t phhalI2CM_EventHandler(uint8_t bId, uint32_t dwStatus);
extern void phhalI2CM_EventUnRegister(void);

#endif /* NXPBUILD__PHHAL_I2CM */

#endif /* PHHALI2CM_EVENT_H */
