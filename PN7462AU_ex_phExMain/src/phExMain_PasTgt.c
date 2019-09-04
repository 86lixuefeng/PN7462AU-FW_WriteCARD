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
 * phExMain_PasTgt.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-08 19:25:31 +0530 (Tue, 08 Sep 2015) $
 * $Author: ASHISH PAL (nxp79566) $
 * $Revision: 15174 $ (v05.19.00)
 */

#ifndef PHEXMAIN_PASTGT_C_
#define PHEXMAIN_PASTGT_C_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phacDiscLoop.h"
#include "phRtos.h"
#include "phExMain_Clif.h"
#include "ph_Log.h"
#include "phUser.h"
#include "phpalI18092mT.h"
#include "phExMain_PasTgt.h"

#ifdef NXPBUILD__CLIF_CM
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
/**
 * Example application's Passive Target operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_PasTgt(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phpalI18092mT_Sw_DataParams_t  spalI18092mT;
    uint16_t wStatus;
    uint16_t wBytes = 0x00;
    uint8_t *pDataBytes;
    uint8_t pAtrRES[15] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00};

    do
    {
        /** Peer Target Software Init */
        wStatus = phpalI18092mT_Sw_Init(&spalI18092mT,
                                        sizeof(spalI18092mT),
                                        psDiscLoopParams->pHalDataParams,
                                        NULL
                                        );
        if(wStatus != PH_ERR_SUCCESS)
        {
            LOG_X32("Peer Target Software Init failure!!!\n",wStatus);
            break;
        }

        /** Perform Target activation. */
        wStatus = phpalI18092mT_Activate(&spalI18092mT,
                                         psDiscLoopParams->sTargetParams.pRxBuffer,
                                         psDiscLoopParams->sTargetParams.wRxBufferLen,
                                         pAtrRES,
                                         0x0E,
                                         NULL,
                                         &wBytes
                                         );
        if(wStatus != PH_ERR_SUCCESS)
        {
            LOG_X32("Target Activation failure!!!\n",wStatus);
            break;
        }

        /** Passive Target exchange. */
        wStatus = phpalI18092mT_Receive(&spalI18092mT,
                                         PH_RECEIVE_DEFAULT,
                                         &pDataBytes,
                                         &wBytes
                                         );

        if(wStatus != PH_ERR_SUCCESS)
        {
            LOG_X32("Passive Target exchange failure!",wStatus);
            break;
        }
        LOG_TXT_D("Passive Target Data Receive successful!!!\n");
    }while(0);

    return wStatus;
}
#endif /* NXPBUILD__CLIF_P2P*/
#endif /* PHEXMAIN_PASTGT_C_ */
