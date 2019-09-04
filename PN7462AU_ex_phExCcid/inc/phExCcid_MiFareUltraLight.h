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
 * phExCcid_MiFareUltraLight.h : MiFare Ultra Light cards Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */

#ifndef PHEXCCID_MIFAREULTRALIGHT_H
#define PHEXCCID_MIFAREULTRALIGHT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phacDiscLoop.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/* define UltraLight Block Size */
#define PH_EXCCID_MIFAREULTRALIGHT_BLOCK_SIZE_UL                  4
#define PH_EXCCID_MIFAREULTRALIGHT_WRITETIMEOUT_DEFAULT_US (0xF46 + 1000)
#define PH_EXCCID_MIFAREULTRALIGHT_READTIMEOUT_DEFAULT_US (0x50 + 0x3C)



/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
#define PH_EXCCID_MIFAREULTRALIGHT_SAK      0x00
/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExCcid_MiFareUltraLight(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXCCID_MIFAREULTRALIGHT_H */
