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
 * $Date: 2016-09-19 17:33:22 +0530 (Mon, 19 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18665 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExRf_Poll.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phExRf_Poll_A.h"
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
static const uint8_t gkphExRf_A_ReqA = 0x26;
static const uint8_t gkphExRf_A_AC1[] = { 0x93, 0x20 };
static const uint8_t gkphExRf_A_AC2[] = { 0x95, 0x20 };
static const uint8_t gkphExRf_A_SEL_CL1[] = { 0x93, 0x70 };
static const uint8_t gkphExRf_A_SAK[] = { 0x95, 0x70 };

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t printOutUID(uint8_t *pTxBuffer, uint8_t *pRxBuffer);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_A_106_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;

    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_A_106_P);
    PH_RETURN_ON_FAILURE(status);

    /* Set Transceive state*/
    /*TxWait is 5.1ms and RxWait is 85us, FDT 92 us*/
    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 85);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 92);
    PH_RETURN_ON_FAILURE(status);

    while (1)
    {

//      phhalRf_FieldOff();
      /* 300 milli secs */
      phUser_Wait(300000);

      phLED_SetPattern(gkphLED_BootUp);

//      status = phhalRf_FieldReset(5100, 0);
//      PH_CONTINUE_ON_FAILURE(status);

        status = phhalRf_FieldOn();
        PH_CONTINUE_ON_FAILURE(status);

        /* only 1 card is allowed in a field */
        pTxBuffer[0] = gkphExRf_A_ReqA;
        status = phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &sRxParams);
        PH_CONTINUE_ON_FAILURE(status);

        printOutUID(pTxBuffer, pRxBuffer);
    }

    return status;
}

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_A_212_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;

    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_212, E_PHHAL_RF_LP_I_RX_A_212);
    PH_RETURN_ON_FAILURE(status);

    /* Set Transceive state*/
    /*TxWait is 5.1ms and RxWait is 85us, FDT 92 us*/
    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 85);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 92);
    PH_RETURN_ON_FAILURE(status);

    while (1)
    {
        phhalRf_FieldOff();
        /* 300 milli secs */
        phUser_Wait(300000);

        phLED_SetPattern(gkphLED_BootUp);

        status = phhalRf_FieldReset(5100, 0);
        PH_CONTINUE_ON_FAILURE(status);

        /* only 1 card is allowed in a field */
        pTxBuffer[0] = gkphExRf_A_ReqA;
        status = phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &sRxParams);
        PH_CONTINUE_ON_FAILURE(status);

        printOutUID(pTxBuffer, pRxBuffer);
    }

    return status;
}

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_A_424_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;

    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_424, E_PHHAL_RF_LP_I_RX_A_424);
    PH_RETURN_ON_FAILURE(status);

    /* Set Transceive state*/
    /*TxWait is 5.1ms and RxWait is 85us, FDT 92 us*/
    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 85);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 92);
    PH_RETURN_ON_FAILURE(status);

    while (1)
    {
        phhalRf_FieldOff();
        /* 300 milli secs */
        phUser_Wait(300000);

        phLED_SetPattern(gkphLED_BootUp);

        status = phhalRf_FieldReset(5100, 0);
        PH_CONTINUE_ON_FAILURE(status);

        /* only 1 card is allowed in a field */
        pTxBuffer[0] = gkphExRf_A_ReqA;
        status = phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &sRxParams);
        PH_CONTINUE_ON_FAILURE(status);

        printOutUID(pTxBuffer, pRxBuffer);
    }

    return status;
}

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_A_848_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;

    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_848, E_PHHAL_RF_LP_I_RX_A_848);
    PH_RETURN_ON_FAILURE(status);

    /* Set Transceive state*/
    /*TxWait is 5.1ms and RxWait is 85us, FDT 92 us*/
    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 85);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 92);
    PH_RETURN_ON_FAILURE(status);

    while (1)
    {
        phhalRf_FieldOff();
        /* 300 milli secs */
        phUser_Wait(300000);

        phLED_SetPattern(gkphLED_BootUp);

        status = phhalRf_FieldReset(5100, 0);
        PH_CONTINUE_ON_FAILURE(status);

        /* only 1 card is allowed in a field */
        pTxBuffer[0] = gkphExRf_A_ReqA;
        status = phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &sRxParams);
        PH_CONTINUE_ON_FAILURE(status);

        printOutUID(pTxBuffer, pRxBuffer);
    }

    return status;
}

static phStatus_t printOutUID(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phStatus_t status;
    phhalRf_RxParams_t sRxParams;
    uint8_t baUid[10];
    uint8_t bUidLen = 0;
    uint8_t bExpUidLen;

    LOG_TXT("Found TypeA\n");
    phLED_SetPattern(gkphLED_Clif_Detect);
    bExpUidLen = (((pRxBuffer[0] & (PHEXRF_A_UID_FRAME_BIT0_MASK | PHEXRF_A_UID_FRAME_BIT1_MASK))
            >> PHEXRF_A_UID_FRAME_BIT0) * 3) + 4;

    /* anticollision */
    pTxBuffer[0] = gkphExRf_A_AC1[0];
    pTxBuffer[1] = gkphExRf_A_AC1[1];
    status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 2, 0, 0, &sRxParams);
    PH_RETURN_ON_FAILURE(status)

    phLED_SetPattern(gkphLED_Clif_Ongoing);
    switch (bExpUidLen)
    {
    case 4:
        phUser_MemCpy(baUid, pRxBuffer, sRxParams.wRxBytes - 1);
        bUidLen = (uint8_t) (sRxParams.wRxBytes - 1);

        /* Select card : 93 70 */
        pTxBuffer[0] = gkphExRf_A_SEL_CL1[0];
        pTxBuffer[1] = gkphExRf_A_SEL_CL1[1];
        break;

    case 7:
        phUser_MemCpy(baUid, &pRxBuffer[1], sRxParams.wRxBytes - 2);
        bUidLen = (uint8_t) (sRxParams.wRxBytes - 2);

        /* 93 70 */
        pTxBuffer[0] = gkphExRf_A_SEL_CL1[0];
        pTxBuffer[1] = gkphExRf_A_SEL_CL1[1];
        phUser_MemCpy(&pTxBuffer[2], pRxBuffer, sRxParams.wRxBytes);
        status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 7, 0, 0, &sRxParams);
        PH_RETURN_ON_FAILURE(status)

        /* cascade level 2 */
        pTxBuffer[0] = gkphExRf_A_AC2[0];
        pTxBuffer[1] = gkphExRf_A_AC2[1];
        status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, 2, 0, 0, &sRxParams);
        PH_RETURN_ON_FAILURE(status)

        phUser_MemCpy(&baUid[bUidLen], pRxBuffer, sRxParams.wRxBytes - 1);
        bUidLen += ((uint8_t) (sRxParams.wRxBytes - 1));

        /* Just send SAK */
        pTxBuffer[0] = gkphExRf_A_SAK[0];
        pTxBuffer[1] = gkphExRf_A_SAK[1];
        break;

    case 10:
        break;
    default:
        break;
    }

    LOG_AU8("UID", &baUid[0], bUidLen);

    return PH_ERR_SUCCESS;
}

static const uint8_t gkphExRf_A_DesRes[] = {0xC2};
static const phhalRf_LP_T_Tx_t gkphExRf_A_TxBR[4] = {E_PHHAL_RF_LP_T_TX_A_106_P, E_PHHAL_RF_LP_T_TX_A_212,
        E_PHHAL_RF_LP_T_TX_A_424,    E_PHHAL_RF_LP_T_TX_A_848};

static const phhalRf_LP_T_Rx_t gkphExRf_A_RxBR[4] = {E_PHHAL_RF_LP_T_RX_A_106, E_PHHAL_RF_LP_T_RX_A_212,
        E_PHHAL_RF_LP_T_RX_A_424,    E_PHHAL_RF_LP_T_RX_A_848};
static const uint8_t gkphExRf_A_Ats[] = {0x06, 0x72, 0xF7, 0x20, 0x00, 0x80};
static const uint8_t gkphExRf_A_Pps_Res[] = {0xD0};


phStatus_t phExRf_A_CM(phhalRf_TargetActivatedParam_t * peTargetParam, uint8_t *pTxBuffer, uint8_t *pRxBuffer,
    uint16_t wMaxLen)
{
    phStatus_t gstatus;
    uint8_t bDRI;
    uint8_t bDSI;
    uint8_t bPPS_Cmd = 0;
    phhalRf_RxParams_t pRxParams;
    do{
        if (pRxBuffer[0] == 0xE0)
        {
            /* Handling Rats*/
            phUser_MemCpy(pTxBuffer, gkphExRf_A_Ats, sizeof(gkphExRf_A_Ats));
            gstatus = phhalRf_Transmit(pTxBuffer, sizeof(gkphExRf_A_Ats));
            PH_BREAK_ON_FAILURE(gstatus)
        }
        phLED_SetPattern(gkphLED_Clif_Ongoing);
        do
        {
            gstatus = phhalRf_Receive(&pRxBuffer, &pRxParams);
        }while((gstatus != PH_ERR_SUCCESS) && ((gstatus & PH_ERR_MASK) != PH_ERR_EXT_RF_ERROR));
        PH_BREAK_ON_FAILURE(gstatus)

        if ((pRxBuffer[0] & 0xF0) == 0xD0)
        {
            /* Handling the PPS. */

                phUser_MemCpy(pTxBuffer, gkphExRf_A_Pps_Res, sizeof(gkphExRf_A_Pps_Res));
                gstatus = phhalRf_Transmit(pTxBuffer, sizeof(gkphExRf_A_Pps_Res));
                PH_BREAK_ON_FAILURE(gstatus)

                /* Handling PPS1 */
                bDRI = pRxBuffer[2] & 0x03;
                bDSI = (pRxBuffer[2] & 0x0C) >> 0x02;

                if ((bDRI != 0x00) && (bDSI != 0x00))
                {
                    gstatus = phhalRf_LoadProtocol_Target(gkphExRf_A_TxBR[bDSI], gkphExRf_A_RxBR[bDRI]);
                }
                bPPS_Cmd = 1;
        }

        while(1)
        {
            /* I-Block exchange. */
            if (bPPS_Cmd)
            {
                do
                {
                    gstatus = phhalRf_Receive(&pRxBuffer, &pRxParams);
                }while((gstatus != PH_ERR_SUCCESS) && ((gstatus & PH_ERR_MASK) != PH_ERR_EXT_RF_ERROR));
                PH_BREAK_ON_FAILURE(gstatus)
            }
            if ((pRxBuffer[0] & 0xC0) == 0xC0)
            {
                break;
            }

            phUser_MemCpy(pTxBuffer, pRxBuffer, pRxParams.wRxBytes);
            gstatus = phhalRf_Transmit(pTxBuffer, pRxParams.wRxBytes);

            if (gstatus != PH_ERR_SUCCESS)
            {
                break;
            }
            else
            {
                bPPS_Cmd = 1;
            }
        }

        if ((gstatus == PH_ERR_SUCCESS))
        {
            phUser_MemCpy(pTxBuffer, gkphExRf_A_DesRes, sizeof(gkphExRf_A_DesRes));
            gstatus = phhalRf_Transmit(pTxBuffer, sizeof(gkphExRf_A_DesRes));
        }
    }while(0);

    return gstatus;
}

