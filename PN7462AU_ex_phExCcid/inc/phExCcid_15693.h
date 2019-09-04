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
 * phExCcid_15693.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-04-21 11:46:50 +0530 (Tue, 21 Apr 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 11652 $ (v05.19.00)
 */

#ifndef PHEXCCID_15693_H
#define PHEXCCID_15693_H

/* *****************************************************************************************************************
 *   Includes
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
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExCcid_15693(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);


#endif /* PHEXCCID_15693_H_ */
