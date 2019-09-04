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
 * SPI Master event based mechanism.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-07-15 18:47:31 +0530 (Wed, 15 Jul 2015) $
 * $Author: Thirumalesh Vemishetti (nxp79087) $
 * $Revision: 13839 $ (v04.13.00)
 */

#ifndef PHHALSPIM_EVENT_H
#define PHHALSPIM_EVENT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SPIM

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
phStatus_t phhalSPIM_EventRegister(uint32_t dwSpimTimeOut);
phStatus_t phhalSPIM_EventHandler(phStatus_t eStatus);
phStatus_t phhalSPIM_EventWait(void);
void phhalSPIM_EventUnRegister(void);

#endif /* NXPBUILD__PHHAL_SPIM */

#endif /* PHHALSPIM_EVENT_H */
