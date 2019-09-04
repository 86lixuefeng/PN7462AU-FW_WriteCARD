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
 * phExMain_ActIni.c : Implements the core logic to act as active initiator
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-20 09:25:05 +0530 (Thu, 20 Aug 2015) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 14611 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phacDiscLoop.h"
#include "phExMain_Clif.h"
#include "phpalI18092mPI.h"
#include "phExMain_ActIni.h"
#include "ph_Log.h"

#ifdef NXPBUILD__CLIF_ACTIVE_INITIATOR
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
/** Performs Active Initiator operations. */
phStatus_t phExMain_ActIni(uint32_t wDiscLoopStatus, phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    uint16_t wStatus;
    uint16_t wTxLength = 0x06;
    uint8_t pData[6] = {0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5};
    do
    {
        wStatus = phpalI18092mPI_Psl((psDiscLoopParams->pPal18092mPIDataParams), 0x01, 0x01, 0x00);

        if(wStatus != PH_ERR_SUCCESS)
        {
            LOG_X32("Active Initiator PSL exchange failure!. wStatus",wStatus);
            break;
        }

        /** Perform Active Initiator DEP Transmit. */
        wStatus = phpalI18092mPI_Transmit(psDiscLoopParams->pPal18092mPIDataParams,
                                          PH_TRANSMIT_DEFAULT,
                                          pData,
                                          wTxLength
                                          );
        if(wStatus != PH_ERR_SUCCESS)
        {
            LOG_X32("Active Initiator Transmit failure!. wStatus",wStatus);
            break;
        }
        LOG_TXT_D("Active Initiator Transmit successful!!!\n");

    }while(0);
    return wStatus;
}

#endif /* NXPBUILD__CLIF_ACTIVE*/
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
