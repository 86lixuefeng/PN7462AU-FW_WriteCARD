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
 * $Date: 2016-06-23 13:52:29 +0530 (Thu, 23 Jun 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18502 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phLED.h"
#include "phExRf_Poll.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phExRf_Poll_B.h"
#include "ph_Log.h"
#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_clif.h"

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
static const uint8_t gkphExRf_B_ReqB[] = { 0x05, 0x00, 0x00 };
static const uint8_t gkphExRf_B_PRIME_CMD[] = { 0x01, 0x0B, 0x03F, 0x80 };	/* */

static const uint8_t gkphExRf_B_Attrib[] = { 0x1D, 0x00, 0x00, 0x00, 0x00,  /* Blank Uid */
                                             0x00,                          /* Param1 No SOF, No EOF, Minimum TR0 and Maximum TR1 = 00*/
                                             0x07,                          /* Param2 , b4-b1(Frame size)MAx size 128, 128K b8-b5(Rx/Tx bit rate) */
                                             0x01,                          /* Param3*/
                                             0x05                           /* Param4 CID is 5 */
                                            };

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t sendAttribCommand(uint8_t *pTxBuffer, uint8_t *pRxBuffer);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */

phStatus_t phExRf_B_106_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams = { 0, 0, 0, 0, 0, 0 };
    phStatus_t wStatus;

    wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_B_106, E_PHHAL_RF_LP_I_RX_B_106);
    PH_RETURN_ON_FAILURE(wStatus)

    /* Set Transceive state & set exchange timing*/
    /*TxWait is 5.1ms and RxWait is 120us, FDT is 720us */
    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 120);
    phhalRf_PCD_SetExchgFdt(720);

    while (1)
    {
//      phhalRf_FieldOff();

        /* 300 milli secs */
//      phUser_Wait(300000);

//      phLED_SetPattern(gkphLED_BootUp);

//      wStatus = phhalRf_FieldReset(5100, 0);
//      PH_CONTINUE_ON_FAILURE(wStatus);

        /* RegB */
        pTxBuffer[0] = gkphExRf_B_ReqB[0];
        pTxBuffer[1] = gkphExRf_B_ReqB[1];
        pTxBuffer[2] = gkphExRf_B_ReqB[2];
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        if (wStatus != PH_ERR_SUCCESS)
        {
            wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
            break;
        }

        LOG_STATUS(wStatus, "Found TypeB\n");
        phLED_SetPattern(gkphLED_Clif_Detect);

        sendAttribCommand(pTxBuffer, pRxBuffer);
    }

    return wStatus;
}

phStatus_t phExRf_B_Prime_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
	phhalRf_RxParams_t sRxParams = { 0, 0, 0, 0, 0, 0 };
	phStatus_t wStatus;

	wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_B_106,
			E_PHHAL_RF_LP_I_RX_B_106);
	PH_RETURN_ON_FAILURE(wStatus)

	/* Set Transceive state & set exchange timing*/
	/*TxWait is 5.1ms and RxWait is 120us, FDT is 720us */
	phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
	phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 120);
	phhalRf_PCD_SetExchgFdt(720);

	/* SIGPRO_RM_CONFIG_RFU - Change Sync byte */
	PH_REG_SET(CLIF_SIGPRO_RM_CONFIG0_REG, 0x00000049);
	/* force CRC write */
	PH_REG_SET(CLIF_RX_CONFIG_REG, 0x00080000U | PH_REG_GET(CLIF_RX_CONFIG_REG));

	/* Disable the RX CRC */
	phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRC, E_PH_HALRF_DISABLE);

	while (1)
	{
		phUser_MemCpy(pTxBuffer, gkphExRf_B_PRIME_CMD, sizeof(gkphExRf_B_PRIME_CMD));
		wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_B_PRIME_CMD), &sRxParams);
		if (wStatus == PH_ERR(PROTOCOL_ERROR, RF) && sRxParams.wRxBytes == 30)
		{
			/* Ok: Found B Prime Card */
			LOG_TXT("Found TypeB Prime\n");
			phLED_SetPattern(gkphLED_Clif_Detect);
		} else {
			wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
			break;
		}
	}

    return wStatus;
}

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_B_212_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams = { 0, 0, 0, 0, 0, 0 };
    phStatus_t wStatus;

    wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_B_212, E_PHHAL_RF_LP_I_RX_B_212);
    PH_RETURN_ON_FAILURE(wStatus)

    /* Set Transceive state & set exchange timing*/
    /*TxWait is 5.1ms and RxWait is 120us, FDT is 720us */
    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 120);
    phhalRf_PCD_SetExchgFdt(720);

    while (1)
    {
        phhalRf_FieldOff();
        /* 300 milli secs */
        phUser_Wait(300000);

        phLED_SetPattern(gkphLED_BootUp);

        wStatus = phhalRf_FieldReset(5100, 0);
        PH_CONTINUE_ON_FAILURE(wStatus);

        /* RegB */
        pTxBuffer[0] = gkphExRf_B_ReqB[0];
        pTxBuffer[1] = gkphExRf_B_ReqB[1];
        pTxBuffer[2] = gkphExRf_B_ReqB[2];
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        if (wStatus != PH_ERR_SUCCESS)
        {
            wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
            break;
        }

        LOG_STATUS(wStatus, "Found TypeB\n");
        phLED_SetPattern(gkphLED_Clif_Detect);

        sendAttribCommand(pTxBuffer, pRxBuffer);
    }

    return wStatus;
}

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_B_424_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams = { 0, 0, 0, 0, 0, 0 };
    phStatus_t wStatus;

    wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_B_424, E_PHHAL_RF_LP_I_RX_B_424);
    PH_RETURN_ON_FAILURE(wStatus)

    /* Set Transceive state & set exchange timing*/
    /*TxWait is 5.1ms and RxWait is 120us, FDT is 720us */
    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 120);
    phhalRf_PCD_SetExchgFdt(720);

    while (1)
    {
        phhalRf_FieldOff();
        /* 300 milli secs */
        phUser_Wait(300000);

        phLED_SetPattern(gkphLED_BootUp);

        wStatus = phhalRf_FieldReset(5100, 0);
        PH_CONTINUE_ON_FAILURE(wStatus);

        /* RegB */
        pTxBuffer[0] = gkphExRf_B_ReqB[0];
        pTxBuffer[1] = gkphExRf_B_ReqB[1];
        pTxBuffer[2] = gkphExRf_B_ReqB[2];
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        if (wStatus != PH_ERR_SUCCESS)
        {
            wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
            break;
        }

        LOG_STATUS(wStatus, "Found TypeB\n");
        phLED_SetPattern(gkphLED_Clif_Detect);

        sendAttribCommand(pTxBuffer, pRxBuffer);
    }

    return wStatus;
}

/**
 *
 * @param pTxBuffer
 * @param pRxBuffer
 * @return
 */
phStatus_t phExRf_B_848_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams = { 0, 0, 0, 0, 0, 0 };
    phStatus_t wStatus;

    wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_B_848, E_PHHAL_RF_LP_I_RX_B_848);
    PH_RETURN_ON_FAILURE(wStatus)

    /* Set Transceive state & set exchange timing*/
    /*TxWait is 5.1ms and RxWait is 120us, FDT is 720us */
    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 5100);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 120);
    phhalRf_PCD_SetExchgFdt(720);

    while (1)
    {
        phhalRf_FieldOff();
        /* 300 milli secs */
        phUser_Wait(300000);

        phLED_SetPattern(gkphLED_BootUp);

        wStatus = phhalRf_FieldReset(5100, 0);
        PH_CONTINUE_ON_FAILURE(wStatus);

        /* RegB */
        pTxBuffer[0] = gkphExRf_B_ReqB[0];
        pTxBuffer[1] = gkphExRf_B_ReqB[1];
        pTxBuffer[2] = gkphExRf_B_ReqB[2];
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
        if (wStatus != PH_ERR_SUCCESS)
        {
            wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
            break;
        }

        LOG_STATUS(wStatus, "Found TypeB\n");
        phLED_SetPattern(gkphLED_Clif_Detect);

        sendAttribCommand(pTxBuffer, pRxBuffer);
    }

    return wStatus;
}

static phStatus_t sendAttribCommand(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams = { 0, 0, 0, 0, 0, 0 };
    phStatus_t wStatus;
    uint8_t baUid[4];

    phUser_MemCpy(baUid, &pRxBuffer[1], 4);

    /* Send ATTRIB command at 128Kbps */
    phUser_MemCpy(pTxBuffer, gkphExRf_B_Attrib, 9);
    phUser_MemCpy(&pTxBuffer[1], baUid, 4);
    wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 9, &sRxParams);

    if (wStatus == PH_ERR_SUCCESS)
    {
        LOG_TXT("ATTRIB : PASS\n");
        phLED_SetPattern(gkphLED_Clif_Pass);
        phUser_Wait(30000);

    }
    else
    {
        LOG_TXT("ATTRIB : FAILURE\n");
        phLED_SetPattern(gkphLED_Clif_Fail);
        phUser_Wait(30000);

    }

    return PH_ERR_SUCCESS;
}
