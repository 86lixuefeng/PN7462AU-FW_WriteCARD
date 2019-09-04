/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2016
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
 * phExVCom_PollTypeA.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-10-19 16:08:50 +0530 (Wed, 19 Oct 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18733 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phExVCom_PollTypeA.h"

#include "phhalHw.h"
#include "phhalRf.h"
#include "phUser.h"
#include "ph_Log.h"
#include "ph_Reg.h"
#include "phLED.h"
#include "phhalGpio.h"
#include "phKeyStore.h"

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
phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */
PH_NOINIT_ALIGNED static uint8_t gphExVCom_Rxbuf[PH_EXVCOM_CLIF_RXBUFSIZE];
PH_NOINIT_ALIGNED static uint8_t gphExVCom_Txbuf[PH_EXVCOM_CLIF_TXBUFSIZE];

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */

const uint8_t gkphExVCom_ReqA = 0x26;
const uint8_t gkphExVCom_AC_CL[] = {0x93, 0x95};
const uint8_t gkphExVCom_BitMask[] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};

uint8_t gphExVCom_Rf_UID[PH_EXVCOM_RF_MAX_NUM_TAG][7];
uint8_t gphExVCom_PollLoopLOGCount;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExVCom_AntiCollLoop(uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t* pwNumOfTags);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Performs the Clif HAL initialization.
 */
void phExVCom_HalInit(void)
{
	phStatus_t status;

    /** Clear the RX buf */
    phUser_MemSet((void *)&gphExVCom_Rxbuf[0], 0x00, PH_EXVCOM_CLIF_RXBUFSIZE);
    /** Clear the TX buf */
    phUser_MemSet((void *)&gphExVCom_Txbuf[0], 0x00, PH_EXVCOM_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
			gphExVCom_Txbuf,
            sizeof(gphExVCom_Txbuf),
			gphExVCom_Rxbuf,
            sizeof(gphExVCom_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}

phStatus_t phExVCom_PalInit(void)
{
    phStatus_t status = PH_ERR_INVALID_PARAMETER;

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_OPE_MODE, PHHAL_RF_OPE_MODE_ISO);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_A_106_P);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_FieldReset(200000, 200000);
    PH_RETURN_ON_FAILURE(status);

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TIMEOUT_VALUE_US, 92);
    PH_RETURN_ON_FAILURE(status);

    return status;
}


phStatus_t phExVCom_PollTypeA(uint32_t* pwNumOfTags)
{
    phStatus_t status = PH_ERR_SUCCESS;

    do{
        status = phhalRf_FieldReset(200000, 200000);
        PH_BREAK_ON_FAILURE(status);

        phLED_SetPattern(gkphLED_BootUp);

        if ( 0 == gphExVCom_PollLoopLOGCount )
        {
            LOG_TXT("Entering TypeA Polling mode..\n");
            LOG_TXT_VCOM("Entering TypeA Polling mode..\n");
        }
        gphExVCom_PollLoopLOGCount++;

        status = phExVCom_AntiCollLoop(gphExVCom_Txbuf, gphExVCom_Rxbuf, pwNumOfTags);

        LOG_X32("Num Tags Detected:", *pwNumOfTags);

    }while(0);

    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t phExVCom_AntiCollLoop(uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t* pwNumOfTags)
{
    phStatus_t status = PH_ERR_SUCCESS;
    phhalRf_RxParams_t sRxParams;
    uint16_t wTxLength;
    uint8_t bActivated = false;
    uint8_t bLastCollPos;
    uint8_t bUidLenRec;
    uint8_t bRxSavedBuffer[10] = {0};
    uint8_t bRxSavedLength = 0;
    uint8_t bCascadeLevel;
    uint32_t dwNumTags;
    bCascadeLevel = 0;
    bLastCollPos = 0;
    bUidLenRec=0;
    dwNumTags = 0;

    while (dwNumTags < PH_EXVCOM_RF_MAX_NUM_TAG) /* condition to resolve all cards. */
    {
        /* only 1 card is allowed in a field */
        pTxBuffer[0] = gkphExVCom_ReqA;
        status = phhalRf_PCD_ExchgISO14443A_ShortFrame(pTxBuffer, pRxBuffer, &sRxParams);
        if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_COLLISION_ERROR))
        {
            break;
        }

        while ((bCascadeLevel < 3) && (bActivated != true))
        {
            while(1)
            {
                wTxLength = 0;
                /* CL  */
                pTxBuffer[wTxLength++] = gkphExVCom_AC_CL[bCascadeLevel];
                pTxBuffer[wTxLength] = (2 + bRxSavedLength) << 4;
                pTxBuffer[wTxLength++] |= bLastCollPos;
                if (bLastCollPos)
                {
                    memcpy(&pTxBuffer[2], bRxSavedBuffer, bRxSavedLength + 1);
                    wTxLength += bRxSavedLength + 1;
                }
                else
                {
                    memcpy(&pTxBuffer[2], bRxSavedBuffer, bRxSavedLength);
                    wTxLength += bRxSavedLength;
                }

                status = phhalRf_PCD_ExchgISO14443A_ACFrame(pTxBuffer, pRxBuffer, wTxLength, bLastCollPos,
                    bLastCollPos, &sRxParams);
                if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_COLLISION_ERROR))
                {
                    break;
                }
                if ((pTxBuffer[1] != 0x70))
                {
                    if (bLastCollPos)
                    {
                        bRxSavedBuffer[bRxSavedLength] |= pRxBuffer[0] & gkphExVCom_BitMask[bLastCollPos];
                        phUser_MemCpy(&bRxSavedBuffer[bRxSavedLength + 1], &pRxBuffer[1], sRxParams.wRxBytes - 1);
                    }
                    else
                    {
                        phUser_MemCpy(&bRxSavedBuffer[bRxSavedLength], pRxBuffer, sRxParams.wRxBytes);
                    }
                }
                if (status == PH_ERR_SUCCESS)
                {
                    if ((pTxBuffer[1] == 0x70) && (((pRxBuffer[0] & 0x04) == 0x04) || (((pRxBuffer[0] & 0x04) == 0x00)))&& (sRxParams.wRxBytes == 1))
                    {
                        /* SAK received*/
                        if ((pRxBuffer[0] & 0x04) == 0x04)
                        {
                            memcpy(&gphExVCom_Rf_UID[dwNumTags][bUidLenRec], &bRxSavedBuffer[1], 3);
                            bUidLenRec +=3;
                        }
                        else if ((pRxBuffer[0] & 0x04) == 0x00)
                        {
                            memcpy(&gphExVCom_Rf_UID[dwNumTags][bUidLenRec], &bRxSavedBuffer[0], 4);
                            bUidLenRec +=4;
                            dwNumTags++;
                        }
                        break;
                    }
                    else
                    {
                        bRxSavedLength += sRxParams.wRxBytes;
                    }
                }
                else
                {
                    /**Collision. */
                    bRxSavedLength += sRxParams.wRxBytes - 1;
                }
                bLastCollPos = sRxParams.bRxCollPos ? ((sRxParams.bRxCollPos) % 8) + 1 : 0;
            }
            PH_BREAK_ON_FAILURE(status)
            if ((pTxBuffer[1] == 0x70) && ((pRxBuffer[0] & 0x04) == 0x00) && (sRxParams.wRxBytes == 1))
            {
                /* UID completed. */
                bActivated = true;
            }
            else
            {
                bRxSavedLength = 0;
                bCascadeLevel++;
            }
        }
        PH_BREAK_ON_FAILURE(status)
        pTxBuffer[0] = 0x50;
        pTxBuffer[1] = 0x00;
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 2, &sRxParams);
        bCascadeLevel = 0;
        bActivated = false;
        bRxSavedLength = 0;
        bUidLenRec = 0;
    }

    *pwNumOfTags = dwNumTags;
    return status;
}
