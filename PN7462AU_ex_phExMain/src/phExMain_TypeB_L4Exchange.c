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
 * phExMain_TypeB_L4Exchange.c : Implements the core logic for the Type B level 4 exchange operations.
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

#include "phExMain_TypeB_L4Exchange.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p4.h"
#include "ph_Log.h"

#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
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
static const char gkphExMain_LogInfoB[PH_EXMAIN_MAX_NO_DSI][8]=
{
   {"106\n"},
   {"212\n"},
   {"424\n"},
   {"848\n"},
};
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * Performs TypeB L4 exchange.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_TypeB_L4Exchange(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;
    uint8_t bFsdi = 8;
    uint8_t bDri = 0;
    uint8_t bDsi = 0;
    uint8_t bCid = 0;
    uint8_t bMbli = 0;
    uint8_t bTxBuffer[10];
    uint16_t wOption = 0x00;
    uint16_t wTxLength = 0;
    uint8_t * ppRxBuffer;
    uint16_t pRxLength;
    uint8_t bNumSlots = 0;
    uint8_t bAfi = 0;
    uint8_t bAtqbLen;

    /** Perform data transfer at different data rates. */
    for (; bDsi < 4; bDsi++)
    {
        if (bDsi != 0)
        {
            status = phpalI14443p3b_WakeUpB(psDiscLoopParams->pPal1443p3bDataParams,
                bNumSlots,
                bAfi,
                psDiscLoopParams->sTypeBTargetInfo.bExtendedAtqBbit,
                (uint8_t *) &(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB),
                &bAtqbLen);
            if (status != PH_ERR_SUCCESS)
            {
                LOG_TXT("WakeUpB Failed\n");
                break;
            }
            LOG_TXT_D("Card Activated\n");
            status = phpalI14443p3b_Attrib(psDiscLoopParams->pPal1443p3bDataParams,
                (uint8_t *)&psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB,
                psDiscLoopParams->sTypeBTargetInfo.bExtendedAtqBbit ?
                    13 : 12,
                bFsdi,
                bCid,
                bDri,
                bDsi,
                &bMbli);
            if (status != PH_ERR_SUCCESS)
            {
            LOG_TXT("Attrib Failed\n");
                break;
            }

            LOG_TXT("Baud rate changed\n");

        }
        wTxLength = 0;

        bTxBuffer[wTxLength++] = 0x00; /* INF1 */
        bTxBuffer[wTxLength++] = 0x84; /* INF2 */
        bTxBuffer[wTxLength++] = 0x00; /* INF3 */
        bTxBuffer[wTxLength++] = 0x00; /* INF4 */
        bTxBuffer[wTxLength++] = 0x08; /* INF4 */

        LOG_AU8_D("Command",bTxBuffer,wTxLength);

        status = phpalI14443p4_Exchange(psDiscLoopParams->pPal14443p4DataParams,
            wOption,
            bTxBuffer,
            wTxLength,
            &ppRxBuffer,
            &pRxLength);
        if (status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Exchange failed at ");
            LOG_TXT(gkphExMain_LogInfoB[bDsi]);
            break;
        }
        LOG_AU8_D("Response",ppRxBuffer,pRxLength);

        LOG_TXT("Exchange successful at ");
        LOG_TXT(gkphExMain_LogInfoB[bDsi]);

        /** De-select the selected card */
        status = phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
        if (status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Deselect failed\n");
            break;
        }
        LOG_TXT_D("Card de-selected\n");

        bDri++;
    }
    return status;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS */
#endif /* NXPBUILD__CLIF_RM*/
