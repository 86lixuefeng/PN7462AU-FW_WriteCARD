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
 * phExEMVCo_Poll.c: Contains the core logic for the Clif Reader part.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-11 11:57:15 +0530 (Fri, 11 Sep 2015) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 15255 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phacDiscLoop.h"
#include "phExEMVCo_Poll.h"
#include "phExEMVCo_Clif.h"
#include "phExEMVCo_TypeAEMV.h"
#include "ph_Log.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p4.h"
#include "phUser.h"
#include "phLED.h"

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
 * Example application's Reader mode operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 */
void phExEMVCo_Poll_Main(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_FAILED;

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        /** Type A card detected. */
        LOG_TXT("Type A Card - ");
        phLED_SetPattern(gkphLED_Clif_Ongoing);

        if(psDiscLoopParams->sTypeATargetInfo.bT1TFlag )
        {
            LOG_TXT("Card not supported\n");
        }
        else
        {
            if ((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak & 0x20) &&
                (!(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak & 0x04)))
            {
                LOG_TXT("ISO14443-4A - ");
                LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
                status = phExEMVCo_TypeAEMV(psDiscLoopParams);
            }

            else
            {
                LOG_TXT("Card not supported - ");
                phLED_SetPattern(gkphLED_Clif_Fail);
                LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
                if(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak & 0x20)
                {
                    (void)phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
                }
                else
                {
                    (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
                }
            }
        }

        if (status != PH_ERR_SUCCESS)
        {
            phLED_SetPattern(gkphLED_Clif_Fail);
            LOG_TXT("Operation not successful\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Clif_Pass);
             LOG_TXT("Operation successful\n");
        }
    }
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        /** Type B card detected. */
        LOG_TXT("Type B Card - ");
        LOG_AU8("UID : ", psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, 4);
        phLED_SetPattern(gkphLED_Clif_Fail);

        LOG_TXT("NO EMVCO EXAMPLE.\n");
        status = phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
    }
#endif
    LOG_TXT("Remove CL card.\n");
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
