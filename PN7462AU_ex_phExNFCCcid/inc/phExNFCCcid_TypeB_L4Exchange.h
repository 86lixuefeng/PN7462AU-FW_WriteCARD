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
 * phExNFCCcid_TypeB_L4Exchange.h : Contains the Api signatures and other declarations for Type B level 4 exchange
 *                               operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

#ifndef PHEXNFCCCID_TYPEB_L4EXCHANGE_H
#define PHEXNFCCCID_TYPEB_L4EXCHANGE_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phacDiscLoop.h"
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
phStatus_t phExNFCCcid_TypeB_L4Exchange(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXNFCCCID_TYPEB_L4EXCHANGE_H */
