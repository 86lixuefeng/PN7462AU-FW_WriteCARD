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
 * phExMain_PasIni.c:  <The purpose and scope of this file>
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
#include "phRtos.h"
#include "phExMain_Clif.h"
#include "phpalI18092mPI.h"
#include "phExMain_PasIni.h"
#include "ph_Log.h"

#ifdef NXPBUILD__CLIF_RM

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
/**
 * Example application's Passive initiator operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_PasIni(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    uint16_t wStatus;
    uint16_t wTxLength = 0x36;

    uint8_t pData[54] = {0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5, 0xA5, 0x14, 0x54, 0xA5, 0x14,
            0xA5, 0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5,
        0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5, 0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5, 0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5,
        0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5, 0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5, 0xA5, 0x14, 0x54, 0xA5, 0x14, 0xA5};


    wStatus = phpalI18092mPI_Transmit(psDiscLoopParams->pPal18092mPIDataParams,
                                      PH_TRANSMIT_DEFAULT,
                                      pData,
                                      wTxLength
                                      );
    if(wStatus == PH_ERR_SUCCESS)
    {
        LOG_TXT_D("Passive Initiator exchange successful!!!\n");
    }
    else
    {
        LOG_X32("Passive Initiator exchange failure!",wStatus);
    }
    return wStatus;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__CLIF_P2P*/
