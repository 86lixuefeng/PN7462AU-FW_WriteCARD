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
 * $Date: 2016-09-29 11:31:37 +0530 (Thu, 29 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18683 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExRf.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phExRf_18000p3m3.h"
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



/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */



/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_18000p3m3_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t wStatus;
    uint8_t baUid[8];
    uint8_t bDr = 0;
    uint8_t bM = 0x2;
    uint8_t bTRext = 0;
    uint8_t bSel = 0;
    uint8_t bSession = 0;
    uint8_t bRfu = 0;
    uint8_t bQ = 0;
    uint8_t baStoredCrc[2];
    uint8_t baHandle[2];
    uint8_t bData[2] = {0xFF, 0xFF};
    uint8_t bWordCount = 0x03;
    uint8_t bWordPtrLength = 0x03;
    uint8_t bMemBank = 0x03;
    uint8_t bWordPtr = 0x00;
    uint16_t wCmdLength = 0x00;
    uint8_t bIndex = 0x00;
    uint8_t* pWordptr = &bWordPtr;

    wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_EPCV2_TARI_9_44, E_PHHAL_RF_LP_I_RX_EPCV2_SC424_2MP);
    if( wStatus!= PH_ERR_SUCCESS){
        return wStatus;
    }

    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 10000);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_TX_SYMBOL, 1);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 10);
    phhalRf_PCD_SetExchgFdt(1356000);

    /* Build Begin Round command frame */
    pTxBuffer[0] = (0x08 << 4) | (bDr << 3) | (bM << 1) | (bTRext);  /* 0 bits left */
    pTxBuffer[1] = (bSel << 6) | (bSession << 4) | (bRfu << 3) | (bQ >> 1);                      /* 0 bits left */
    pTxBuffer[2] = (bQ << 7);                                                                    /* 7 bits left */
    /* Setup Bits Info */
    (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x01);


    wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
    if( wStatus!= PH_ERR_SUCCESS){
        wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
        return wStatus;
    }

    phLED_SetPattern(gkphLED_Clif_Detect);
    do{
        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);

        /* 875 us */
        phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 875);
        /* Set EPCV2 TX SYMBOL  */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_TX_SYMBOL, 0x02);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRC, E_PH_HALRF_DISABLE);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRCTYPE, 0X00);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_CRCINVERTED, 0X01);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_CRCPRESET, 0X03);

        phUser_MemCpy(baUid, &pRxBuffer[0], 2);
        phUser_MemCpy(baStoredCrc, &pRxBuffer[0], 2);
        /* only 1 card is allowed in a field */
        /* Build Ack Command frame */
        pTxBuffer[0] = (0x01 << 6) | (baUid[0] >> 2);    /* 0 bits left */
        pTxBuffer[1] = (baUid[0] << 6) | (baUid[1] >> 2);                    /* 0 bits left */
        pTxBuffer[2] = (baUid[1] << 6);                                      /* 6 bits left */

        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x02);

        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        if( (wStatus!= PH_ERR_SUCCESS) && ((wStatus & PH_ERR_MASK)!= PH_ERR_INTEGRITY_ERROR) ){
            break;
        }
        phLED_SetPattern(gkphLED_Clif_Ongoing);
        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRCTYPE, 0X00);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRC, E_PH_HALRF_ENABLE);
        /* Build ReqRn command frame */
        pTxBuffer[0] = 0xC1;
        pTxBuffer[1] = baStoredCrc[0];
        pTxBuffer[2] = baStoredCrc[1];
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        PH_BREAK_ON_FAILURE(wStatus)

        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);
        /* read the card */
        phUser_MemCpy(baStoredCrc, &pRxBuffer[0], 2);
        phUser_MemCpy(baHandle, baStoredCrc, 2);
        pTxBuffer[wCmdLength++] = 0xC2;
        pTxBuffer[wCmdLength]  = (bMemBank << 6) | (bWordPtrLength << 4);  /* 4 bits left */

        /* append pointer */
        for (bIndex = 0; bIndex < (bWordPtrLength + 1); ++bIndex)
        {
            pTxBuffer[wCmdLength++] |= (pWordptr[bIndex]) >> 4;
            pTxBuffer[wCmdLength]    = (pWordptr[bIndex]) << 4;
        }
        /* 4 bits left */

        /* append word-count */
        pTxBuffer[wCmdLength++] |= bWordCount >> 4;  /* 0 bits left */
        pTxBuffer[wCmdLength]    = bWordCount << 4;  /* 4 bits left */

        /* append handle */
        pTxBuffer[wCmdLength++] |= baStoredCrc[0] >> 4;    /* 0 bits left */
        pTxBuffer[wCmdLength]    = baStoredCrc[0] << 4;    /* 4 bits left */
        pTxBuffer[wCmdLength++] |= baStoredCrc[1] >> 4;    /* 0 bits left */
        pTxBuffer[wCmdLength++]  = baStoredCrc[1] << 4;    /* 4 bits left */

        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x04);
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, wCmdLength, &sRxParams);
        if( (wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE){
            break;
        }

        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);

        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRCTYPE, 0X00);
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRC, E_PH_HALRF_ENABLE);

        /* send reqrn again to get back the handle */
        pTxBuffer[0] = 0xC1;
        pTxBuffer[1] = baHandle[0];
        pTxBuffer[2] = baHandle[1];
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        PH_BREAK_ON_FAILURE(wStatus)

        phUser_MemCpy(baStoredCrc, &pRxBuffer[0], 2);
        /* write the card with cover*/
        bWordPtrLength = 0x02;
        wCmdLength = 0;
        pTxBuffer[wCmdLength++] = 0xC3;
        pTxBuffer[wCmdLength]   = (bMemBank << 6) | (bWordPtrLength << 4);  /* 4 bits left */
        /* append pointer */
        for (bIndex = 0; bIndex < (bWordPtrLength + 1); ++bIndex)
        {
            pTxBuffer[wCmdLength++] |= (pWordptr[bIndex]) >> 4;
            pTxBuffer[wCmdLength]    = (pWordptr[bIndex]) << 4;
        }
        /* 4 bits left */
        /* append data */
        pTxBuffer[wCmdLength++] |= bData[0] >> 4;                        /* 0 bits left */
        pTxBuffer[wCmdLength++]  = (bData[0] << 4) | (bData[1] >> 4);    /* 0 bits left */
        pTxBuffer[wCmdLength]    = bData[1] << 4;                        /* 4 bits left */

        /* perform XOR operation */
        pTxBuffer[wCmdLength - 2] ^= baStoredCrc[0] >> 4;
        pTxBuffer[wCmdLength - 1] ^= (baStoredCrc[0] << 4) |  (baStoredCrc[1] >> 4);
        pTxBuffer[wCmdLength]     ^= baStoredCrc[1] << 4;

        /* append handle */
        pTxBuffer[wCmdLength++] |= baHandle[0] >> 4;    /* 0 bits left */
        pTxBuffer[wCmdLength]    = baHandle[0] << 4;    /* 4 bits left */
        pTxBuffer[wCmdLength++] |= baHandle[1] >> 4;    /* 0 bits left */
        pTxBuffer[wCmdLength++]  = baHandle[1] << 4;    /* 4 bits left */

        phhalRf_PCD_SetExchgFdt(600000);
        /* Setup Bits Info */
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x04);
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, wCmdLength, &sRxParams);
        if( (wStatus & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE){
            break;
        }

        wStatus = PH_ERR_SUCCESS;
    }while(0);

    if(wStatus == PH_ERR_SUCCESS)
    {
        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT("EPCV2 R/W : PASS\n");
        phUser_Wait(30000);

    }
    else
    {
        phLED_SetPattern(gkphLED_Clif_Fail);
        LOG_TXT("EPCV2 R/W : FAILURE\n");
        phUser_Wait(30000);

    }

    return wStatus;
}
