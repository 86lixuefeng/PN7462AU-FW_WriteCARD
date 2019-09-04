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
 * phExMain_Jewel.h : MiFare Classic cards Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-14 15:17:16 +0530 (Mon, 14 Sep 2015) $
 * $Author: ASHISH PAL (nxp79566) $
 * $Revision: 15315 $ (v05.19.00)
 */

#ifndef PHEXMAIN_JEWEL_H
#define PHEXMAIN_JEWEL_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phacDiscLoop.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXMAIN_JEWEL_READTIMEOUT_DEFAULT_US (0x200)
#define PH_EXMAIN_JEWEL_WRITEETIMEOUT_DEFAULT_US (0x1774)
#define PH_EXMAIN_JEWEL_WRITENETIMEOUT_DEFAULT_US (0xA63)
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExMain_Jewel(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXMAIN_JEWEL_H_ */
