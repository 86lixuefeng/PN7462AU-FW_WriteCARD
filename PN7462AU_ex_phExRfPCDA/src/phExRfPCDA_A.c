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
 * phExRf_A.c:  <The purpose and scope of this file>
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

#include "phExRfPCDA.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phExRfPCDA_A.h"
#include "ph_Log.h"
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
static const uint8_t gkphExRfPCDA_A_ReqA = 0x26;
static const uint8_t gkphExRfPCDA_A_AC1[] = {0x93, 0x20};
static const uint8_t gkphExRfPCDA_A_AC2[] = {0x95, 0x20};
static const uint8_t gkphExRfPCDA_A_SEL_CL1[] = {0x93, 0x70};
static const uint8_t gkphExRfPCDA_A_SAK[] = {0x95, 0x70};
static const uint8_t gkphExRfPCDA_A_RID[] = {0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t gkphExRfPCDA_A_ReadAll[] = {0x00, /* Read all command */
                    0x00,/* Address */
                    0x00, /* Data */
                    0x00, 0x00, 0x00, 0x00}; /* Uid Yet to Update */

static const uint8_t gkphExRfPCDA_A_MiFrBlk47Key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//const uint8_t gkphExRfPCDA_A_MiFrDsFrSelFile[] = {0x0A, 0x00, 0x5A};
static const uint8_t gkphExRfPCDA_A_MiFrDsFrSelFile[] = {0x02, 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76, 0x00,
        0x00, 0x85, 0x01, 0x01, 0x00};
static const uint8_t gkphExRfPCDA_A_TopazRseg[] = {0x10, /* RSEG cmd*/
    0x00,  /* address*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* just mandate zeros */
    0x00, 0x00, 0x00, 0x00}; /* Uid Yet to be update */



static const uint8_t gkphExRfPCDA_A_AtrReq[] = {0x25, 0xD4, 0x00, 0x03, 0x50, 0x71, 0xEF, 0x1F, 0xEE, 0xE9,
    0x58, 0xA9, 0xF9, 0x00, 0x00, 0x00, 0x32, 0x46, 0x66, 0x6D, 0x01, 0x01, 0x11,
    0x02, 0x02, 0x07, 0xFF, 0x03, 0x02, 0x00, 0x13, 0x04, 0x01, 0x64, 0x07, 0x01,
    0x03, 0x5C, 0xE7};

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExRfPCDA_A_MifareClassic(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams,
    phhalRf_MFCParams_t *psMFCparams);
static phStatus_t phExRfPCDA_A_MifareUltraLight(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams);
static phStatus_t phExRfPCDA_A_Tag1(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams);
static phStatus_t phExRfPCDA_A_MifareDesFire(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams);


/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRfPCDA_A_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;
    uint8_t baUid[10];
    uint8_t bUidLen;
    uint8_t bExpUidLen;
    phhalRf_MFCParams_t sMFCparams;

    do{
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 92);
        PH_BREAK_ON_FAILURE(status)

        /* only 1 card is allowed in a field */
        pTxBuffer[0] = gkphExRfPCDA_A_ReqA;
        status = phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &sRxParams);
        PH_BREAK_ON_FAILURE(status)

        LOG_TXT("Found TypeA\n");
        phLED_SetPattern(gkphLED_Clif_Detect);
        bExpUidLen = (((pRxBuffer[0] & (PH_EXRFPCDA_A_UID_FRAME_BIT0_MASK | PH_EXRFPCDA_A_UID_FRAME_BIT1_MASK))
                >> PH_EXRFPCDA_A_UID_FRAME_BIT0) *3) + 4;

        /* Check for Type 1 TOPAZ cards */
        if(pRxBuffer[0] == 0x00)
        {
            status = phExRfPCDA_A_Tag1(pTxBuffer, pRxBuffer, &sRxParams);
            break;
        }

        /* anticollision */
        pTxBuffer[0] = gkphExRfPCDA_A_AC1[0];
        pTxBuffer[1] = gkphExRfPCDA_A_AC1[1];
        status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 2, 0, 0, &sRxParams);
        PH_BREAK_ON_FAILURE(status)

        phLED_SetPattern(gkphLED_Clif_Ongoing);
        switch(bExpUidLen)
        {
        case 4:
            phUser_MemCpy(baUid, pRxBuffer, sRxParams.wRxBytes-1);
            bUidLen = (uint8_t) (sRxParams.wRxBytes-1);

            /* Select card : 93 70 */
            pTxBuffer[0] = gkphExRfPCDA_A_SEL_CL1[0];
            pTxBuffer[1] = gkphExRfPCDA_A_SEL_CL1[1];
            break;

        case 7:
            phUser_MemCpy(baUid, &pRxBuffer[1], sRxParams.wRxBytes-2);
            bUidLen = (uint8_t) (sRxParams.wRxBytes-2);

            /* 93 70 */
            pTxBuffer[0] = gkphExRfPCDA_A_SEL_CL1[0];
            pTxBuffer[1] = gkphExRfPCDA_A_SEL_CL1[1];
            phUser_MemCpy(&pTxBuffer[2], pRxBuffer, sRxParams.wRxBytes);
            status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 7, 0, 0, &sRxParams);
            PH_RETURN_ON_FAILURE(status)

            /* cascade level 2 */
            pTxBuffer[0] = gkphExRfPCDA_A_AC2[0];
            pTxBuffer[1] = gkphExRfPCDA_A_AC2[1];
            status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 2, 0, 0, &sRxParams);
            PH_RETURN_ON_FAILURE(status)

            phUser_MemCpy(&baUid[bUidLen], pRxBuffer, sRxParams.wRxBytes - 1);
            bUidLen += ((uint8_t) (sRxParams.wRxBytes - 1));

            /* Just send SAK */
            pTxBuffer[0] = gkphExRfPCDA_A_SAK[0];
            pTxBuffer[1] = gkphExRfPCDA_A_SAK[1];
            break;

        case 10:
            break;
        default:
            break;
        }

        /* Card selection */
        phUser_MemCpy(&pTxBuffer[2], pRxBuffer, sRxParams.wRxBytes);
        status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 7, 0, 0, &sRxParams);
        if(*pRxBuffer & 0x04){
            status = PH_ERR_INTERFACE_ERROR;
        }

        /* FDT :- 500us. */
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 500);

        /* check for NFCDEP support*/
        if(*pRxBuffer & PH_EXRFPCDA_A_NFCDEP_SUPPORT)
        {
            phhalRf_SetConfig(PHHAL_RF_CONFIG_NFCIP1, (uint32_t)PH_ON);

            phUser_MemCpy(pTxBuffer, gkphExRfPCDA_A_AtrReq, sizeof(gkphExRfPCDA_A_AtrReq));
            status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRfPCDA_A_AtrReq), &sRxParams);

            PH_BREAK_ON_FAILURE(status)

            LOG_TXT("NFCDEP ATR SUCCESS\n");

            break;
        }

        if((*pRxBuffer == PH_EXRFPCDA_A_MIFARECLASSIC_1K_SAK) || (*pRxBuffer == PH_EXRFPCDA_A_MIFARECLASSIC_4K_SAK))
        {
            sMFCparams.bsizeUID = bExpUidLen;
            sMFCparams.pUID = baUid;
            /* For R/W max it takes 10ms */
            phhalRf_PCD_SetExchgFdt(10000);
            phExRfPCDA_A_MifareClassic(pTxBuffer, pRxBuffer, &sRxParams, &sMFCparams);
        }else if((pRxBuffer[0] & PH_EXRFPCDA_A_TAGTYPE) == PH_EXRFPCDA_A_TYPE2TAG)
        {
            /* Type 2 Tags*/
            /* For R/W max it takes 5ms */
            phhalRf_PCD_SetExchgFdt(5000);
            phExRfPCDA_A_MifareUltraLight(pTxBuffer, pRxBuffer, &sRxParams);

        }else if((pRxBuffer[0] & PH_EXRFPCDA_A_TAGTYPE) == PH_EXRFPCDA_A_TYPE4TAG)
        {
            /* Type 4 Tags*/
            phExRfPCDA_A_MifareDesFire(pTxBuffer, pRxBuffer, &sRxParams);
        }
        else
        {
            /* To remove QL Warnings */
        }
    }while(0);

    return status;
}

static phStatus_t phExRfPCDA_A_MifareClassic(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams,
    phhalRf_MFCParams_t *psMFCparams)
{
    phStatus_t status;
    uint8_t i;

    psMFCparams->bKeyCmd = 0x60; /* type A key cmd */;
    psMFCparams->bBlock = 9;
    psMFCparams->pKey = (uint8_t *)gkphExRfPCDA_A_MiFrBlk47Key;

    do{
        status = phhalRf_PCD_ExchgMFC_Auth(pTxBuffer, pRxBuffer, psMFCparams, psRxParams);
        PH_BREAK_ON_FAILURE(status)

        pTxBuffer[0] = 0x30; /* Read cmd */
        pTxBuffer[1] = 9; /* Read address 9 */
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 2, psRxParams);
        PH_BREAK_ON_FAILURE(status)

        phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRC, PH_OFF);
        /* Timeout 10ms */
        phhalRf_PCD_SetExchgFdt(10000);

        pTxBuffer[0] = 0xA0;    /* write operation 0xA0 */
        pTxBuffer[1] = 9;       /* Block 9 */
        pRxBuffer[0] = 0;
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 2, psRxParams);
        if( pRxBuffer[0] != 0x0A){ /* chk for Ack */
            break;
        }

        for(i=0; i < 16 ; i++){
            pTxBuffer[i] = i;
        }
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 16, psRxParams);

        pTxBuffer[0] = 0x30;    /* Read cmd */
        pTxBuffer[1] = 9;       /* Block 9 */
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 2, psRxParams);

    }while(0);

    phhalRf_MFC_Disable();

    if(status == PH_ERR_SUCCESS){
        LOG_TXT("Classic R/W : PASS\n");
        phLED_SetPattern(gkphLED_Clif_Pass);
        phUser_Wait(30000);

    }else{
        LOG_TXT("Classic R/W : FAILURE\n");
        phLED_SetPattern(gkphLED_Clif_Fail);
        phUser_Wait(30000);
    }

    return status ;
}


static phStatus_t phExRfPCDA_A_Tag1(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams)
{
    uint8_t baUid[10];
    uint8_t bUidLen = 4;
    phStatus_t status;

    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 7);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_JEWEL_MODE, PH_ON);
    phhalRf_PCD_SetExchgFdt(5000);
    phUser_MemCpy(pTxBuffer, gkphExRfPCDA_A_RID, 7);

    do{
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 7, psRxParams);
        PH_BREAK_ON_FAILURE(status)

        phLED_SetPattern(gkphLED_Clif_Ongoing);
        phUser_MemCpy(baUid, &pRxBuffer[2], bUidLen);

        if(pRxBuffer[0] != 0x11){ /* HR0 */
            /* Its dynamic card */
            phUser_MemCpy(pTxBuffer, gkphExRfPCDA_A_TopazRseg, 10);
            phUser_MemCpy(&pTxBuffer[10], baUid, bUidLen);
            status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 14, psRxParams);
        }else{

            bUidLen = (uint8_t) (psRxParams->wRxBytes - 2);
            phUser_MemCpy(baUid, &pRxBuffer[2], bUidLen);

            /* Perform Read All command */
            pTxBuffer[0] = gkphExRfPCDA_A_ReadAll[0];
            pTxBuffer[1] = gkphExRfPCDA_A_ReadAll[1];
            pTxBuffer[2] = gkphExRfPCDA_A_ReadAll[2];
            phUser_MemCpy(&pTxBuffer[3], baUid, bUidLen);
            status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 7, psRxParams);
        }

    }while(0);

    if(status == PH_ERR_SUCCESS){
        LOG_TXT("Topaz ReadAll/RSEG : PASS\n");
        phLED_SetPattern(gkphLED_Clif_Pass);
        phUser_Wait(30000);

    }else{
        LOG_TXT("Topaz ReadAll/RSEG : FAILURE\n");
        phLED_SetPattern(gkphLED_Clif_Fail);
        phUser_Wait(30000);

    }
    return status;
}

static phStatus_t phExRfPCDA_A_MifareUltraLight(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams)
{
    phStatus_t status;

    do{
        pTxBuffer[0] = 0x30;    /* Read cmd */
        pTxBuffer[1] = 6;      /* address */
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 2, psRxParams);
        PH_BREAK_ON_FAILURE(status)

        /* disable rx crc */
        phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRC, PH_ON);
        phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRC, PH_OFF);

        pTxBuffer[0] = 0xA2;    /* Write cmd */
        pTxBuffer[1] = 6;      /* address */
        pTxBuffer[2] = 1;    /* Data */
        pTxBuffer[3] = 2;
        pTxBuffer[4] = 3;
        pTxBuffer[5] = 4;

        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 6, psRxParams);
        if( (status != PH_ERR_SUCCESS) && ( (status & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE)){
            break;
        }

        if(pRxBuffer[0] != 0x0A){
            status = PH_ERR_READ_WRITE_ERROR;
        }else{
            status = PH_ERR_SUCCESS;
        }

    }while(0);

    /* Enable Tx & Rx crc */
    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRC, PH_ON);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRC, PH_ON);

    if(status == PH_ERR_SUCCESS){
        LOG_TXT("UltraLight R/W : PASS\n");
        phLED_SetPattern(gkphLED_Clif_Pass);
        phUser_Wait(30000);

    }else{
        LOG_TXT("UltraLight R/W : FAILURE\n");
        phLED_SetPattern(gkphLED_Clif_Fail);
        phUser_Wait(30000);

    }
    return status;
}


static phStatus_t phExRfPCDA_A_MifareDesFire(uint8_t *pTxBuffer, uint8_t *pRxBuffer, phhalRf_RxParams_t *psRxParams)
{

    uint32_t dwDri = 0;
    uint32_t dwDsi = 0;
    phhalRf_LP_I_Tx_t eTxBitRate[4] = {E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_TX_A_212, E_PHHAL_RF_LP_I_TX_A_424, E_PHHAL_RF_LP_I_TX_A_848};
    phhalRf_LP_I_Rx_t eRxBitRate[4] = {E_PHHAL_RF_LP_I_RX_A_106_P, E_PHHAL_RF_LP_I_RX_A_212, E_PHHAL_RF_LP_I_RX_A_424, E_PHHAL_RF_LP_I_RX_A_848};
    phhalRf_RxParams_t tsRxParam;
    uint32_t dwtmp = 0;
    phStatus_t status;
#if PHFL_LOG_ENABLE
    uint32_t bRate[] = {106, 212, 424, 848};
#endif /* PHFL_LOG_ENABLE*/

    pTxBuffer[0] = 0x50; /* halt command*/
    pTxBuffer[1] = 0x00;
    /*sending halt command*/
    status = (phStatus_t)(phhalRf_PCD_ExchgISO14443A_StandardFrame(
        pTxBuffer,
        pRxBuffer,
        2,
        &tsRxParam));
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        return status;
    }

    for( dwDri = 0; dwDri < 4; dwDri++, dwDsi++)
    {
        do
        {
            /******************************************************************************************/

            /*Load Protocol*/
            phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_A_106_P);

            /******************************************************************************************/
            /*Set exchange timing*/
            phhalRf_PCD_SetExchgFdt(500000);
            phhalRf_MFC_Disable();

            /******************************************************************************************/
            /*Set up WUPA*/
            pTxBuffer[0] = 0x52;
            /*Send Short command for WUPA*/
            if(phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &tsRxParam)!= PH_ERR_SUCCESS)
            {
                break;
            }

            /******************************************************************************************/

            /*Set exchange timing*/
            phhalRf_PCD_SetExchgFdt(500000);

            /******************************************************************************************/
            /*Set up AC_CL1*/
            pTxBuffer[0] = 0x93;
            pTxBuffer[1] = 0x20;

            /*Send Short command for AC-CL1*/
            if(phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 2, 0, 0, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }

            /******************************************************************************************/
            /*Set up SELECT_CL1*/
            pTxBuffer[0] = 0x93;
            pTxBuffer[1] = 0x70;
            pTxBuffer[2] = pRxBuffer[0];
            pTxBuffer[3] = pRxBuffer[1];
            pTxBuffer[4] = pRxBuffer[2];
            pTxBuffer[5] = pRxBuffer[3];
            pTxBuffer[6] = pRxBuffer[4];

            /*Send Short command for SEL-CL1*/
            if(phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 7, 0, 0, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }

            /******************************************************************************************/

            /*Get SAK CL1 */
            dwtmp = (uint32_t)pRxBuffer[0];
            if( (dwtmp & 0x04UL) != 0x00)
            {
                /* for 7 - byte UID carsd */
                /*Set up AC_CL2*/
                pTxBuffer[0] = 0x95;
                pTxBuffer[1] = 0x20;
                /*Send Short command for AC-CL2*/
                if(phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 2, 0, 0, &tsRxParam) != PH_ERR_SUCCESS)
                {
                    break;
                }

                /*Set up SELECT_CL2*/
                pTxBuffer[0] = 0x95;
                pTxBuffer[1] = 0x70;
                pTxBuffer[2] = pRxBuffer[0];
                pTxBuffer[3] = pRxBuffer[1];
                pTxBuffer[4] = pRxBuffer[2];
                pTxBuffer[5] = pRxBuffer[3];
                pTxBuffer[6] = pRxBuffer[4];

                /*Send Short command for SEL-CL2*/
                if(phhalRf_PCD_ExchgISO14443A_ACFrame( pTxBuffer, pRxBuffer, 7, 0, 0, &tsRxParam) != PH_ERR_SUCCESS)
                {
                    break;
                }
            }


            /* RATS */
            pTxBuffer[0] = 0xE0;
            pTxBuffer[1] = 0x80;
            if(phhalRf_PCD_ExchgISO14443A_StandardFrame( pTxBuffer, pRxBuffer, 2, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }
            /*PPS*/
            pTxBuffer[0] = 0xD0;/*Select CID = 1*/
            pTxBuffer[1] = 0x11;/*PPS1 is transmitted*/
            pTxBuffer[2] = (uint8_t)(((dwDsi << 2) & 0x0C) | (dwDri & 0x03));
            /*gpmt_halRf_TxBuffer[2] = 0x0F; 0x00 = 106, 0x05 = 212, 0x0A = 424, 0x0F = 848*/

            if(phhalRf_PCD_ExchgISO14443A_StandardFrame( pTxBuffer, pRxBuffer, 3, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }

            phhalRf_PCD_SetExchgFdt(500000);

            /*Load Protocol*/
            phhalRf_LoadProtocol_Initiator(eTxBitRate[dwDri], eRxBitRate[dwDri]);

            /*Send I-block for Get Version application*/
            pTxBuffer[0] = 0x0A; /*CID following, Block Number = 0*/
            pTxBuffer[1] = 0x00; /*CID = 0*/
            pTxBuffer[2] = 0x60; /*Select Application*/

            /* Call standard frame exchange */
            if(phhalRf_PCD_ExchgISO14443A_StandardFrame( pTxBuffer, pRxBuffer, 3, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }

            /*Send I-block for Get Version application*/
            pTxBuffer[0] = 0x0B; /*CID following, Block Number = 1*/
            pTxBuffer[1] = 0x00; /*CID = 1*/
            pTxBuffer[2] = 0xAF;

            /* Call standard frame exchange */
            if(phhalRf_PCD_ExchgISO14443A_StandardFrame( pTxBuffer, pRxBuffer, 3, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }

            phUser_MemCpy(pTxBuffer, gkphExRfPCDA_A_MiFrDsFrSelFile, 14);
            status = phhalRf_PCD_ExchgISO14443A_StandardFrame( pTxBuffer, pRxBuffer, 14, psRxParams);
            PH_BREAK_ON_FAILURE(status)

            /*S(DESELECT)*/
            pTxBuffer[0] = 0xCA; /*CID following*/
            pTxBuffer[1] = 0x00; /*CID = 1*/

            if(phhalRf_PCD_ExchgISO14443A_StandardFrame( pTxBuffer, pRxBuffer, 2, &tsRxParam) != PH_ERR_SUCCESS)
            {
                break;
            }

            LOG_U32("DesFire R/W PASS : ", bRate[dwDri]);
        }while(0);

        phUser_Wait(6000);
    }

    if(status != PH_ERR_SUCCESS)
    {
        LOG_TXT("Desfire R/W : FAILURE\n");
    }

    /* return status */
    return status;
}
