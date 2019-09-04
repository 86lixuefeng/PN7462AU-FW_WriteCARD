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
 * phExMain_TypeA_L4Exchange.h : Contains the Api signatures and other declarations for Type A level 4 exchange
 *                               operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-04 16:39:24 +0530 (Mon, 04 May 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12183 $ (v05.19.00)
 */

#ifndef PHEXMAIN_TYPEA_L4EXCHANGE_H
#define PHEXMAIN_TYPEA_L4EXCHANGE_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phacDiscLoop.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#define PH_EXMAIN_TYPEA_L4EXCHANGE_MIFAREDESFIRE_SAK      0x20
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExMain_TypeA_L4Exchange(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXMAIN_TYPEA_L4EXCHANGE_H */
