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
 * phExRf_F.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-19 15:43:50 +0530 (Mon, 19 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18663 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phLED.h"
#include "phExRf.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phExRf_F.h"
#include "ph_Log.h"

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
static const uint8_t gkphExRf_F_ReqC[] = {0x06, 0x00, 0xFF, 0xFF, 0x01, 0x00};//{0x06, 0x00, 0x12, 0xFC, 0x01, 0x03};
static const uint8_t gkphExRf_F_Read[] = {0x10, /* length*/
        0x06, /* Read command */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* UID yet to update */
        0x01,/*Nr of services */
        0x0B, /*Service code list lower byte*/
        0x00, /*Service code list higher byte*/
        0x01, /*Nr of blocks*/
        0x80, /*Block list*/
        0x00 /*Block list*/};

static const uint8_t gkphExRf_F_Write[] = {0x20, /* length*/
        0x08, /* Read command */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* UID yet to update */
        0x01,/*Nr of services */
        0x09, /*Service code list lower byte*/
        0x00, /*Service code list higher byte*/
        0x01, /*Nr of blocks*/
        0x80, /*Block list*/
        0x01, /*Block list*/
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF};

static const phhalRf_LP_I_Tx_t gkphExRf_F_TxRate[] = {(phhalRf_LP_I_Tx_t)E_PHHAL_RF_LP_I_TX_F_212,
        (phhalRf_LP_I_Tx_t)E_PHHAL_RF_LP_I_TX_F_424};
static const phhalRf_LP_I_Rx_t gkphExRf_F_RxRate[] = {(phhalRf_LP_I_Rx_t)E_PHHAL_RF_LP_I_RX_F_212_P,
        (phhalRf_LP_I_Rx_t)E_PHHAL_RF_LP_I_RX_F_424_P};
static const uint32_t gkphExRf_F_Rate[] = {212, 424};

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExRf_F_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phStatus_t wStatus;
    phhalRf_RxParams_t sRxParams;
    uint8_t baUid[8];
    uint8_t bRate = 0;


    do
    {
        /*Load Protocol*/
        wStatus = phhalRf_LoadProtocol_Initiator(gkphExRf_F_TxRate[bRate], gkphExRf_F_RxRate[bRate]);
        PH_RETURN_ON_FAILURE(wStatus)
        phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, 20000);
        phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, 100);
        phhalRf_PCD_SetExchgFdt(3000);

        phUser_MemCpy(pTxBuffer, gkphExRf_F_ReqC, 6);
        /*Send Short command for Req C*/
        wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 6, &sRxParams);

        if( wStatus == PH_ERR_SUCCESS)
        {
            LOG_TXT("Found TypeF\n");
            phLED_SetPattern(gkphLED_Clif_Detect);

            phUser_MemCpy(baUid, &pRxBuffer[2], 8);
            /* Perform Read command */
            phUser_MemCpy(pTxBuffer, gkphExRf_F_Read, 16);
            phUser_MemCpy(&pTxBuffer[2], baUid, 8);
            wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 16, &sRxParams);
            PH_BREAK_ON_FAILURE(wStatus)

            phLED_SetPattern(gkphLED_Clif_Ongoing);
            /* Check for status */
            if((pRxBuffer[10] != 0) || (pRxBuffer[11] != 0)){
                wStatus = PH_ERR_UNSUPPORTED_COMMAND;
                break;
            }

            /* Perform Write command */
            phUser_MemCpy(pTxBuffer, gkphExRf_F_Write, 0x20);
            phUser_MemCpy(&pTxBuffer[2], baUid, 8);
            wStatus = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, 0x20, &sRxParams);
            if((pRxBuffer[10] != 0) || (pRxBuffer[11] != 0)){
                wStatus = PH_ERR_UNSUPPORTED_COMMAND;
                break;
            }

            if(wStatus == PH_ERR_SUCCESS){
                LOG_U32("Felica R/W : PASS", gkphExRf_F_Rate[bRate]);
                phLED_SetPattern(gkphLED_Clif_Pass);
                phUser_Wait(30000);

            }else{
                LOG_U32("Felica R/W : FAILURE", gkphExRf_F_Rate[bRate]);
                phLED_SetPattern(gkphLED_Clif_Fail);
                phUser_Wait(30000);

            }
        }

        bRate++;
    }while(bRate < 2);

    return wStatus;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
