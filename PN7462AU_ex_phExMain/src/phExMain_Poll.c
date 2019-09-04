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
 * phExMainPoll.c: Contains the core logic for the Clif Reader part.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-12-01 14:26:44 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18769 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phacDiscLoop.h"
#include "phExMain_Poll.h"
#include "phExMain_Clif.h"
#include "phExMain_MiFareClassic.h"
#include "phExMain_MiFareUltraLight.h"
#include "phExMain_TypeA_L4Exchange.h"
#include "phExMain_TypeB_L4Exchange.h"
#include "phExMain_Felica.h"
#include "phExMain_Jewel.h"
#include "ph_Log.h"
#include "phExMain_18000p3m3.h"
#include "phExMain_15693.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p4.h"
#include "phpalFelica.h"
#include "phpalSli15693.h"
#include "phpalI18000p3m3.h"
#include "phExMain_PasIni.h"
#include "phExMain_Type1Tag.h"
#include "phExMain_Type2Tag.h"
#include "phExMain_Type3Tag.h"
#include "phExMain_Type4Tag.h"
#include "phalTop.h"
#include "phalT1T.h"
#include "phalFelica.h"
#include "phalICode.h"
#include "phUser.h"
#include "phLED.h"

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
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS)
PH_NOINIT static  phalTop_Sw_DataParams_t  salTop;
PH_NOINIT static  phalT1T_Sw_DataParams_t            salT1T;                      /* AL T1T component */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS || NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
PH_NOINIT static  phpalMifare_Sw_DataParams_t spalMifare;
PH_NOINIT static  phalMfdf_Sw_DataParams_t salMifareDf;
PH_NOINIT static  phalMful_Sw_DataParams_t salMifareUl;
PH_NOINIT static  phalICode_Sw_DataParams_t   salI15693;          /* AL ISO 15693 component */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
PH_NOINIT static  phalFelica_Sw_DataParams_t salFelica;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */
extern phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
static void phExMain_PollA(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phalTop_Sw_DataParams_t  *palTop);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
static void phExMain_PollB(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
static void phExMain_PollF(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phalTop_Sw_DataParams_t  *palTop);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
static void phExMain_PollV(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
static void phExMain_Poll18000p3m3(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Reader mode operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 */
void phExMain_Poll_Main(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    /* This initialization is common for Type A and Type F since TAG type 1,2,4 will have A technology
     * and TAG type 3 will have F technology */
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS)
    phalTop_Sw_DataParams_t  *palTop;
    palTop = &salTop;
    /* Initialize the Top component */
    phalTop_Sw_Init(&salTop, sizeof(phalTop_Sw_DataParams_t), &salT1T, &salMifareUl, &salFelica, &salMifareDf, &salI15693, NULL);
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS || NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    /** Type A card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        phExMain_PollA(psDiscLoopParams,palTop);
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    /** Type B card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        phExMain_PollB(psDiscLoopParams);
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    /** Type F card detected. */
    if ((psDiscLoopParams->bDetectedTechs &  (1 << PHAC_DISCLOOP_TECH_TYPE_F212))
            || (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_F424)))
    {
        phExMain_PollF(psDiscLoopParams,palTop);
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    /** Type ICode card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_V))
    {
        phExMain_PollV(psDiscLoopParams);
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    /** Type 18000 card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_18000P3M3))
    {
        phExMain_Poll18000p3m3(psDiscLoopParams);
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
static void phExMain_PollA(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phalTop_Sw_DataParams_t  *palTop)
{
    phStatus_t status = PH_ERR_FAILED;
    uint8_t bNdefPresence = 0;

    status = phpalMifare_Sw_Init(&spalMifare,
                                sizeof(spalMifare),
                                psDiscLoopParams->pHalDataParams,
                                psDiscLoopParams->pPal14443p4DataParams
                                );

    status = phalMfdf_Sw_Init(&salMifareDf,
                            sizeof(salMifareDf),
                            &spalMifare,
                            NULL,
                            NULL,
                            NULL,
                            psDiscLoopParams->pHalDataParams
                            );

    status =  phalMful_Sw_Init(&salMifareUl,
                                sizeof(salMifareUl),
                                &spalMifare,
                                NULL,
                                NULL,
                                NULL
                                );

    /* Initialize the T1T AL component */
    phalT1T_Sw_Init(&salT1T, sizeof(phalT1T_Sw_DataParams_t), &gphpal_Sw_DataParams3A);

    LOG_TXT("Type A Card - ");
    phLED_SetPattern(gkphLED_Clif_Ongoing);
    if(psDiscLoopParams->sTypeATargetInfo.bT1TFlag )
    {
        salTop.ualTop.salTop_T1T.pAlT1TDataParams = psDiscLoopParams->pAlT1TDataParams;
        /** setting the card to be type 3 tag. */
        status = phalTop_SetConfig(palTop,
            PHAL_TOP_CONFIG_TAG_TYPE,
            PHAL_TOP_TAG_TYPE_T1T_TAG
        );
        /** checking whether card is ndef compliant*/
        status =  phalTop_CheckNdef(palTop, &bNdefPresence);
        if((status == PH_ERR_SUCCESS) && bNdefPresence)
        {
            LOG_TXT("Type 1 Tag\n");
            status = phExMain_Type1TagOp(psDiscLoopParams, palTop);

        }
        else
        {
            LOG_TXT("JEWEL CARD\n");
            status = phExMain_Jewel(psDiscLoopParams);
        }
                /* Disable Jewel Mode */
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_JEWEL_MODE, PH_OFF);
        (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);

    }
    else
    {
        if((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXMAIN_MIFARECLASSIC_1K_SAK)
                || (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXMAIN_MIFARECLASSIC_4K_SAK))
        {
            LOG_TXT("MifareClassic - ");
            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
            status = phExMain_MiFareClassic(psDiscLoopParams);
        }
        else if ((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXMAIN_TYPEA_L4EXCHANGE_MIFAREDESFIRE_SAK))
        {
            salTop.ualTop.salTop_T4T.pAlT4TDataParams = &salMifareDf;

            /** setting the card to be type 4 tag */
            status = phalTop_SetConfig(palTop,
                                        PHAL_TOP_CONFIG_TAG_TYPE,
                                        PHAL_TOP_TAG_TYPE_T4T_TAG
                                        );

            /** checking whether card is ndef compliant*/
            status =  phalTop_CheckNdef(palTop, &bNdefPresence);
            if((status == PH_ERR_SUCCESS) && bNdefPresence)
            {
                LOG_TXT("Type 4 Tag\n");
                status = phExMain_Type4TagOp(psDiscLoopParams, palTop);
            }
            else
            {
                LOG_TXT("ISO14443-4A - ");
                LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
                status = phExMain_TypeA_L4Exchange(psDiscLoopParams);
            }
            (void)phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);

        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXMAIN_MIFAREULTRALIGHT_SAK)
        {
            /** setting the card to be type 2 tag */
            status = phalTop_SetConfig(palTop,
                                        PHAL_TOP_CONFIG_TAG_TYPE,
                                        PHAL_TOP_TAG_TYPE_T2T_TAG
                                        );

            salTop.ualTop.salTop_T2T.pAlT2TDataParams = &salMifareUl;

            /** checking whether card is ndef compliant*/
            status =  phalTop_CheckNdef(palTop, &bNdefPresence);
            if((status == PH_ERR_SUCCESS) && bNdefPresence)
            {
                LOG_TXT("Type 2 Tag\n");
                if (bNdefPresence != 1)
                    status = phExMain_Type2TagOp(psDiscLoopParams, palTop);
                else
                {
                    LOG_TXT("Create NDEF Message in the Tag used\n");
                }
            }
            else
            {
                /* need to distinguish between UL and ULC , using only mful */
                LOG_TXT("Mifare UltraLight - ");
                LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
                status = phExMain_MiFareUltraLight(psDiscLoopParams);
            }
            (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);

        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXMAIN_PASINI_NFCDEP_SAK)
        {
            /* need to distinguish between UL and ULC , can u a macro*/
            LOG_TXT("Passive Peer - ");
            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
#ifdef NXPBUILD__CLIF_P2P
            status = phExMain_PasIni(psDiscLoopParams);
#endif /* NXPBUILD__CLIF_P2P*/
        }
        else
        {
            LOG_TXT("Card not supported - ");
            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
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

    LOG_TXT("Remove the CL card.\n");
    /* Card Removal process. */
    status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
        psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);

    status = status & PH_ERR_MASK;
    while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR)))
    {
        (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
        phRtos_TaskDelay(500);
        status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
            psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);
        status = status & PH_ERR_MASK;
    }
    LOG_TXT("CL card removed.\n");

}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
static void phExMain_PollB(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    uint8_t bAfi = 0x00;
    uint8_t bAtqbLen = 0;
    phStatus_t status = PH_ERR_FAILED;
    uint8_t bNumSlots = 0;

    LOG_TXT("Type B Card - ");
    LOG_AU8("UID : ", psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, 4);
    phLED_SetPattern(gkphLED_Clif_Ongoing);
    status = phExMain_TypeB_L4Exchange(psDiscLoopParams);

    if (status != PH_ERR_SUCCESS)
    {
        phLED_SetPattern(gkphLED_Clif_Fail);
        LOG_X32("TypeB_L4Exchange not successful status:", status);
    }
    else
    {
        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT_D("TypeB_L4Exchange successful!!!\n");
    }

    LOG_TXT("Remove the CL card.\n");
    /* Card Removal process. */
    status = phpalI14443p3b_WakeUpB(psDiscLoopParams->pPal1443p3bDataParams, bNumSlots, bAfi,
            psDiscLoopParams->sTypeBTargetInfo.bExtendedAtqBbit, (uint8_t *)&(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB), &bAtqbLen);
    status = status & PH_ERR_MASK;
    while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR)))
    {
        (void)phpalI14443p3b_HaltB(psDiscLoopParams->pPal1443p3bDataParams);
        phRtos_TaskDelay(500);
        status = phpalI14443p3b_WakeUpB(psDiscLoopParams->pPal1443p3bDataParams, bNumSlots, bAfi,
                psDiscLoopParams->sTypeBTargetInfo.bExtendedAtqBbit, (uint8_t *)&(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB), &bAtqbLen);
        status = status & PH_ERR_MASK;
    }
    LOG_TXT("CL card removed.\n");

}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
static void phExMain_PollF(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phalTop_Sw_DataParams_t  *palTop)
{
    uint8_t baSystemCode[2] = { 0xFF, 0xFF };
    uint8_t* pSensFResp;
    uint16_t wSensfLen = 0x00;
    uint8_t bNdefPresence = 0;
    phStatus_t status = PH_ERR_FAILED;

    status = phalFelica_Sw_Init(&salFelica,
                                sizeof(salFelica),
                                psDiscLoopParams->pPalFelicaDataParams
                                );



    salTop.ualTop.salTop_T3T.pAlT3TDataParams = &salFelica;

    /** setting the card to be type 3 tag. */
    status = phalTop_SetConfig(palTop,
                                PHAL_TOP_CONFIG_TAG_TYPE,
                                PHAL_TOP_TAG_TYPE_T3T_TAG
                                );

    /** checking whether card is ndef compliant*/
    status =  phalTop_CheckNdef(palTop,
                                &bNdefPresence
                                );
    if((status == PH_ERR_SUCCESS) && bNdefPresence)
    {
        LOG_TXT("Type 3 Tag\n");
        status = phExMain_Type3TagOp(psDiscLoopParams, palTop);

    }
    else
    {
        LOG_TXT("Felica Card - ");
        LOG_AU8("UID : ", psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm, 8);
        phLED_SetPattern(gkphLED_Clif_Ongoing);
        status = phExMain_Felica(psDiscLoopParams);
    }

    if (status != PH_ERR_SUCCESS)
    {
        phLED_SetPattern(gkphLED_Clif_Fail);
        LOG_X32("Felica:Operation not successful status:", status);
    }
    else
    {
        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT_D("Operation successful!!!\n");
    }

    LOG_TXT("Remove the CL card.\n");

    do
    {
        status = phpalFelica_ReqC(psDiscLoopParams->pPalFelicaDataParams,
                                    baSystemCode, PHPAL_FELICA_NUMSLOTS_4, &pSensFResp, &wSensfLen);
        phRtos_TaskDelay(500);
        status = status & PH_ERR_MASK;
    } while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR) || (status == PH_ERR_INTEGRITY_ERROR)));
    LOG_TXT("CL card removed.\n");

}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
static void phExMain_PollV(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_FAILED;

    LOG_TXT("ICode Sli!!!\n");
    LOG_AU8("UID : ", psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid, 8);
    phLED_SetPattern(gkphLED_Clif_Ongoing);
    status = phExMain_15693(psDiscLoopParams);
    if (status != PH_ERR_SUCCESS)
    {
        phLED_SetPattern(gkphLED_Clif_Fail);
        LOG_X32("Operation not successful status:", status);
    }
    else
    {
        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT("Operation successful!!!\n");
    }

    LOG_TXT("Remove the CL card.\n");

    do
    {

        status = phpalSli15693_Inventory(
                psDiscLoopParams->pPalSli15693DataParams,
                psDiscLoopParams->sTypeVTargetInfo.bFlag | PHPAL_SLI15693_FLAG_NBSLOTS,
                0,
                NULL,
                0,
                &psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].bDsfid,
                psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid
                );
        phRtos_TaskDelay(500);
        status = status & PH_ERR_MASK;
    }while((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR));
    LOG_TXT("CL card removed.\n");

}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
static void phExMain_Poll18000p3m3(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    phStatus_t status = PH_ERR_FAILED;

    uint8_t     aSelectCmd[39] = {0};
    uint8_t    wSelectCmdLen;
    uint8_t     bSelectCmdValidBits;
    uint8_t     aBeginRoundCmd[3] = {0};
    uint8_t     *pRxBuffer;
    uint16_t    wRxBufferLen;

    LOG_TXT("ICode ILT!!!\n");
    phLED_SetPattern(gkphLED_Clif_Ongoing);
    status = phExMain_18000p3m3(psDiscLoopParams);
    if (status != PH_ERR_SUCCESS)
    {
        phLED_SetPattern(gkphLED_Clif_Fail);
        LOG_X32("Operation not successful status:", status);
    }
    else
    {
        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT("Operation successful!!!\n");
    }

    LOG_TXT("Remove the CL card.\n");

    do
    {
        status = phhalHw_FieldReset(psDiscLoopParams->pHalDataParams);

        /* Set the Guard times */
        status =  phhalHw_Wait(
                psDiscLoopParams->pHalDataParams,
            PHHAL_HW_TIME_MICROSECONDS,
            psDiscLoopParams->waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_18000P3M3]);

        /* Create select command frame */
        status =  phpalI18000p3m3_CreateSelectCmd(
            psDiscLoopParams->pPal18000p3m3DataParams,
            PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
            0,
            PHPAL_I18000P3M3_MEMBANK_UII,
            NULL,
            0,
            NULL,
            0,
            0,
            aSelectCmd,
            &wSelectCmdLen,
            &bSelectCmdValidBits);

        /* Create BeginRound command frame */
            status = phpalI18000p3m3_CreateBeginRoundCmd(
                    psDiscLoopParams->pPal18000p3m3DataParams,
                    psDiscLoopParams->sI18000p3m3TargetInfo.bDr,
                    psDiscLoopParams->sI18000p3m3TargetInfo.bM,
            PH_OFF,
            PHPAL_I18000P3M3_SEL_ALL_00,
            PHPAL_I18000P3M3_SESSION_S0,
            0,
            0,
            aBeginRoundCmd);

        /* Inventory with 1 slot */
        status = phhalHw_I18000p3m3Inventory(
                psDiscLoopParams->pHalDataParams,
            aSelectCmd,
            (uint8_t)wSelectCmdLen,
            bSelectCmdValidBits,
            aBeginRoundCmd,
            PHHAL_HW_I18000P3M3_ONE_TS_ONLY,
            &pRxBuffer,
            &wRxBufferLen);

        phRtos_TaskDelay(500);

        status = status & PH_ERR_MASK;
    }while((status == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR));

    LOG_TXT("CL card removed.\n");
}
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */

#endif /* NXPBUILD__CLIF_RM*/
