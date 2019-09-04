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
 * r02.06.02_Internal $Revision: 18502 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExRf_Poll.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phExRf_Poll_15693.h"
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

static const uint8_t gkphExRf_15693Inv[] = {0x26, 0x01, 0x00};
static const uint8_t gkphExRf_15693InvRead[] = {0x26, 0xA0, 0x04, 0x00, 0x06, 0x01};
static const uint8_t gkphExRf_15693FastInvRead[] = {0x26, 0xA1, 0x04, 0x00, 0x06, 0x01};
static const uint8_t gkphExRf_15693WriteCard[] = {0x22, 0x21,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Uid to be updated */
    0x06, 0x06, 0x06, 0x06, 0x06};

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
phStatus_t phExRf_15693_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t wStatus;
    uint8_t baUid[8];

    wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_100, E_PHHAL_RF_LP_I_RX_15693_26);

    PH_RETURN_ON_FAILURE(wStatus)

    /* Set Transceive state*/
    /*TxWait is 5.1ms(1/fc) = 69156 and RxWait is 85us(1/fc) = 1152 */
    phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 1200);
    phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 10);
    phhalRf_PCD_SetExchgFdt(3840);

    /* only 1 card is allowed in a field */
    /* Performing inventory request */
    pTxBuffer[0] = gkphExRf_15693Inv[0];
    pTxBuffer[1] = gkphExRf_15693Inv[1];
    pTxBuffer[2] = gkphExRf_15693Inv[2];
    wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 3, &sRxParams);
    if( wStatus!= PH_ERR_SUCCESS){
        wStatus = PH_ERR_EXRF_CARD_NOT_FOUND;
        return wStatus;
    }

    LOG_TXT("Found ISO 15693 Card\n");
    phLED_SetPattern(gkphLED_Clif_Detect);

    do{
        phUser_MemCpy(baUid, &pRxBuffer[2], 8);
        /* selecting the card */
        pTxBuffer[0] = 0x22;
        pTxBuffer[1] = 0x25;
        phUser_MemCpy(&pTxBuffer[2], &pRxBuffer[2], 8);
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 10, &sRxParams);
        PH_BREAK_ON_FAILURE(pRxBuffer[0])

        phLED_SetPattern(gkphLED_Clif_Ongoing);
        /* performing inventory read */
        phUser_MemCpy(pTxBuffer, gkphExRf_15693InvRead, 6);
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 6, &sRxParams);
        if( wStatus!= PH_ERR_SUCCESS){
            LOG_TXT("Detected ISO 15693 Card does not support inventory read\n");
            LOG_TXT("Use NXP manufactured ICODE SLIX\n");
            break;
        }

        wStatus = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_10, E_PHHAL_RF_LP_I_RX_15693_53);

        /* performing fast inventory read */
        wStatus = phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_RX_BAUDRATE, 0x01);
        phUser_MemCpy(pTxBuffer, gkphExRf_15693FastInvRead, 6);
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 6, &sRxParams);
        if( wStatus!= PH_ERR_SUCCESS){
            LOG_TXT("Detected ISO 15693 Card does not support fast inventory read\n");
            LOG_TXT("Use NXP manufactured ICODE SLIX\n");
            break;
        }

        wStatus = phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_RX_BAUDRATE, 0x00);
        /* reading data from the card */
        pTxBuffer[0] = 0x22;
        pTxBuffer[1] = 0x20;
        phUser_MemCpy(&pTxBuffer[2], baUid, 0x08);
        pTxBuffer[10] = 0x06;

        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 11, &sRxParams);
        PH_BREAK_ON_FAILURE(pRxBuffer[0])

        /* 20ms for write */
        phhalRf_PCD_SetExchgFdt(20000);
        /* writing data to the card */
        phUser_MemCpy(pTxBuffer, gkphExRf_15693WriteCard, 15);
        phUser_MemCpy(&pTxBuffer[2], baUid, 0x08);
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 15, &sRxParams);
        PH_BREAK_ON_FAILURE(pRxBuffer[0])

        phhalRf_PCD_SetExchgFdt(3840);
        /* again reading the card to verify the write */
        pTxBuffer[0] = 0x22;
        pTxBuffer[1] = 0x20;
        phUser_MemCpy(&pTxBuffer[2], baUid, 0x08);
        pTxBuffer[10] = 0x06;
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 11, &sRxParams);
        PH_BREAK_ON_FAILURE(pRxBuffer[0])

    }while(0);

    if(wStatus == PH_ERR_SUCCESS)
    {
        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT("Card R/W Successful : PASS\n");
        phUser_Wait(30000);

    }
    else
    {
        phLED_SetPattern(gkphLED_Clif_Fail);
        LOG_TXT("Card R/W Successful : FAILURE\n");
    }
    return wStatus;
}
