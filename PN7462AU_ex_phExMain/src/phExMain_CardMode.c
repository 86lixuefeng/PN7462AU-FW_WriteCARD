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
 * phExMain_CardMode.c : Implements the core logic that handles the Felica cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-11 19:20:13 +0530 (Fri, 11 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 16957 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phacDiscLoop.h"
#include "phExMain_Clif.h"
#include "phExMain_CardMode.h"
#include "phpalI14443p4mC.h"
#include "ph_Log.h"

#ifdef NXPBUILD__CLIF_CM
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
/** Performs card mode operations. */
phStatus_t phExMain_CardMode(uint32_t wDiscLoopStatus, phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    uint16_t wStatus;
    uint8_t pAts[6] = {0x06, 0x72, 0x00, 0x20, 0x00, 0x80};
    phpalI14443p4mC_Sw_DataParams_t   spalI14443p4mC = {0};

    wStatus = phpalI14443p4mC_Sw_Init((phpalI14443p4mC_Sw_DataParams_t *)&spalI14443p4mC,
                                      sizeof(spalI14443p4mC),
                                      psDiscLoopParams->pHalDataParams,
                                      NULL
                                      );
    if(wStatus != PH_ERR_SUCCESS)
    {
        LOG_X32("Card Mode Init failure!",wStatus);
    }

    if(wStatus == PH_ERR_SUCCESS)
    {
        wStatus = phpalI14443p4mC_Activate((phpalI14443p4mC_Sw_DataParams_t *)&spalI14443p4mC,
                                           psDiscLoopParams->sTargetParams.pRxBuffer,
                                           (uint8_t)psDiscLoopParams->sTargetParams.wRxBufferLen,
                                           pAts,
                                           0x06
                                           );
    }
    if(wStatus == PH_ERR_SUCCESS)
    {
        LOG_TXT_D("Card Mode exchange successful!!!\n");
    }
    else
    {
        LOG_X32("Card Mode exchange failure!",wStatus);
    }

    return wStatus;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__CLIF_CM*/
