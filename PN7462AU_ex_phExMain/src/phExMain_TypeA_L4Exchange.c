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
 * phExMain_TypeA_L4Exchange.c : Implements the core logic for the Type A level 4 exchange operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-26 11:34:37 +0530 (Wed, 26 Aug 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14749 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExMain_TypeA_L4Exchange.h"
#include "phpalMifare.h"
#include "phpalI14443p4.h"
#include "phUser.h"
#include "phpalI14443p4a.h"
#include "ph_Log.h"
#include "phpalI14443p3a.h"
#include "phalMfdf.h"

#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXMAIN_MAX_NO_DSI 4
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static const char gkphExMain_LogInfo[PH_EXMAIN_MAX_NO_DSI][8]=
{
   {"106\n"},
   {"212\n"},
   {"424\n"},
   {"848\n"},
};

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Performs TypeA L4 exchange.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_TypeA_L4Exchange(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;
    uint16_t wOption = 0;
    uint8_t pTxBuffer[1];
    uint8_t * ppRxBuffer;
    uint16_t pRxLength;
    uint8_t bFsdi = 8;
    uint8_t bCid = 0;
    uint8_t bDri = 0;
    uint8_t bDsi = 0;
    uint8_t pUidOut[10];
    uint8_t bLenUidOut;
    uint8_t bSak;
    uint8_t bMoreCardsAvailable;


    do
    {
        /** Perform data transfer at different data rates. */
        for (bDsi = 0; bDsi < 4; bDsi++)
        {
            if (bDsi != 0)
            {
                /** Layer 3 activation. */
                status = phpalI14443p3a_ActivateCard(psDiscLoopParams->pPal1443p3aDataParams,
                    psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                    psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize,
                    pUidOut,
                    &bLenUidOut,
                    &bSak,
                    &bMoreCardsAvailable);
                if(status != PH_ERR_SUCCESS)
                {
                    LOG_TXT("ActivateCard:3A Failed\n");
                    break;
                }
                /* LOG_TXT("Card Activated\n"); */

                psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize = bLenUidOut;

                phUser_MemCpy(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, pUidOut,
                    psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak = bSak;

                /** Layer 4 activation. */
                status = phpalI14443p4a_ActivateCard(psDiscLoopParams->pPal1443p4aDataParams,
                    bFsdi,
                    bCid,
                    bDri,
                    bDsi,
                    psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts);
                if(status != PH_ERR_SUCCESS)
                {
                    LOG_TXT("ActivateCard:4A Failed\n");
                    break;
                }

                LOG_TXT("Baud Rate changed\n");

            }

            status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_FWI, ((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bFwi);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_TXT("SetConfig:FWI Failed\n");
                break;
            }

            status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_CID, ((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bCid);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_TXT("SetConfig:CID Failed\n");
                break;
            }

            /** Perform data exchange at different data rates. */
            pTxBuffer[0] = 0x60;
            LOG_AU8_D("Command",pTxBuffer,1);

            status = phpalI14443p4_Exchange(psDiscLoopParams->pPal14443p4DataParams,
                wOption,
                pTxBuffer,
                1,
                &ppRxBuffer,
                &pRxLength);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_TXT("Exchange failed at ");
                LOG_TXT(gkphExMain_LogInfo[bDsi]);
                break;
            }
            LOG_AU8_D("Response",ppRxBuffer,pRxLength);

            LOG_TXT("Exchange successful at ");
            LOG_TXT(gkphExMain_LogInfo[bDsi]);

            /** De-Select the card. */
            status = phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_TXT("Card Deselect failed\n");
                break;
            }
            LOG_TXT_D("Card de-selected\n");

            bDri++;
        } /* for:bDsi */

    }while(0);

    return status;
}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
