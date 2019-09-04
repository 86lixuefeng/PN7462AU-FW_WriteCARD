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
 * phExNFCCcid_LLCP_Client.h: Snep client Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date:
 * $Author:
 * $Revision:
 */
#ifndef PHEXNFCCCID_LLCP_CLIENT_H
#define PHEXNFCCCID_LLCP_CLIENT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phExNFCCcid_Cfg.h"
#ifdef PHFL_LLCP_SNEP_ENABLE
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
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXNFCCCID_LLCP_CLIENT_TASK_PRIO                   3
#define PH_EXNFCCCID_LLCP_CLIENT_TASK_STACK                  180

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
void phExNFCCcid_LLCP_Client(void * pLlcp);
#endif
#endif /* PHEXNFCCCID_LLCP_CLIENT_H_ */
