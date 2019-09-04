/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * phExMain_Type1Tag.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-11-23 17:22:51 +0530 (Mon, 23 Nov 2015) $
 * $Author: Ashish Pal (nxp79566) $
 * $Revision: 16617 $ (v05.19.00)
 */

#ifndef PHEXMAIN_TYPE1TAG_H
#define PHEXMAIN_TYPE1TAG_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phalTop.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXMAIN_TYPE1TAG_READTIMEOUT_DEFAULT_US (0x220)
#define PH_EXMAIN_TYPE1TAG_WRITEETIMEOUT_DEFAULT_US (0x2328)
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExMain_Type1TagOp(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,phalTop_Sw_DataParams_t *palTop);

#endif /* PHEXMAIN_TYPE1TAG_H_ */
