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
 * phExMain_Jewel.c : Implements the core logic that handles the Felica cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-18 14:00:14 +0530 (Tue, 18 Aug 2015) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 14564 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phExMain_Jewel.h"
#include "phalT1T.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phhalHw.h"

#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_JEWEL_TAGS
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
static const uint8_t pBlockData[8] = {0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7};
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Jewel card operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_Jewel(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    /* common variables */
    phStatus_t status;

    /* variables used by the AL component */
    uint8_t bAddress = 5;
    uint16_t wBlockLength;
    uint8_t pRxData[8 + 1];  /* to avoid the overlap of rxbuffer address on txbuffer address */
    do
    {
        /** Initialize the application layer. */
        /*refer table 12 of tag1 doc */
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, PH_EXMAIN_JEWEL_READTIMEOUT_DEFAULT_US);
        /** Read block. */
        status = phalT1T_ReadBlock(psDiscLoopParams->pAlT1TDataParams, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, bAddress, pRxData, &wBlockLength);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Reading Failed\n");
            break;
        }

        LOG_TXT("Reading successful\n");
        LOG_AU8("BlockDataRead",pRxData,sizeof(pRxData));
        /** Write one block */
        /*refer table 12 of tag1 doc */
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_JEWEL_WRITEETIMEOUT_DEFAULT_US);
        status = phalT1T_WriteEraseBlock(psDiscLoopParams->pAlT1TDataParams, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, bAddress, (uint8_t *)(pBlockData), pRxData, &wBlockLength);

        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Writing Failed\n");
            break;
        }
        LOG_TXT("Writing successful\n");
        LOG_AU8("Blockwritten",pBlockData,sizeof(pBlockData));
    }while(0);
    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_JEWEL_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
