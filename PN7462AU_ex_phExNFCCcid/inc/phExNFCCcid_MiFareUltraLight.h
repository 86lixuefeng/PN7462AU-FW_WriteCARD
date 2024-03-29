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
 * phExNFCCcid_MiFareUltraLight.h : MiFare Ultra Light cards Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

#ifndef PHEXNFCCCID_MIFAREULTRALIGHT_H
#define PHEXNFCCCID_MIFAREULTRALIGHT_H

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
#define PH_EXNFCCCID_MIFAREULTRALIGHT_BLOCK_SIZE_UL                  4
#define PH_EXNFCCCID_MIFAREULTRALIGHT_WRITETIMEOUT_DEFAULT_US (0xF46 + 1000)
#define PH_EXNFCCCID_MIFAREULTRALIGHT_READTIMEOUT_DEFAULT_US (0x50 + 0x3C)



/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
#define PH_EXNFCCCID_MIFAREULTRALIGHT_SAK      0x00
/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExNFCCcid_MiFareUltraLight(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXNFCCCID_MIFAREULTRALIGHT_H */
