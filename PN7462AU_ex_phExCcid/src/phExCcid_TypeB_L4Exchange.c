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
 * phExCcid_TypeB_L4Exchange.c : Implements the core logic for the Type B level 4 exchange operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-05 15:53:53 +0530 (Tue, 05 Jul 2016) $
 * $Author: nxp86397 $
 * $Revision: 3651 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExCcid_TypeB_L4Exchange.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p4.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
phStatus_t phExCcid_TypeB_L4Exchange(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_SUCCESS;

    phpalI14443p4_SetProtocol(psDiscLoopParams->pPal14443p4DataParams,
                              ((phpalI14443p3b_Sw_DataParams_t *)psDiscLoopParams->pPal1443p3bDataParams)->bCidSupported,
                              ((phpalI14443p3b_Sw_DataParams_t *)psDiscLoopParams->pPal1443p3bDataParams)->bCid,
                              ((phpalI14443p3b_Sw_DataParams_t *)psDiscLoopParams->pPal1443p3bDataParams)->bNadSupported,
                              0,
                              ((phpalI14443p3b_Sw_DataParams_t *)psDiscLoopParams->pPal1443p3bDataParams)->bFwi,
                              ((phpalI14443p3b_Sw_DataParams_t *)psDiscLoopParams->pPal1443p3bDataParams)->bFsdi,
                              ((phpalI14443p3b_Sw_DataParams_t *)psDiscLoopParams->pPal1443p3bDataParams)->bFsci);


    return status;
}

#endif
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
