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
 * phExRf_ActInit.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 12:36:10 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16977 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phLED.h"
#include "ph_Status.h"
#include "phhalRf.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phExRf_ActInit.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
#define CHECK_STATUS(X) if ( (X) != PH_ERR_SUCCESS)   \
        break
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

static const uint8_t gkphExRf_ActInit_AtrReq[] = {0x25, 0xD4, 0x00, 0x03, 0x50, 0x71, 0xEF, 0x1F, 0xEE, 0xE9,
        0x58, 0xA9, 0xF9, 0x00, 0x00, 0x00, 0x32, 0x46, 0x66, 0x6D, 0x01, 0x01, 0x11,
        0x02, 0x02, 0x07, 0xFF, 0x03, 0x02, 0x00, 0x13, 0x04, 0x01, 0x64, 0x07, 0x01,
        0x03};

static const uint8_t gkphExRf_ActInit_PslReq[] = { 0x05, 0xD4, 0x04, 0x00, 0x00, 0x00};

static const uint8_t gkphExRf_ActInit_LoopBack[] = { 255, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E};
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExRf_ActInit_106(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
static phStatus_t phExRf_ActInit_212(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
static phStatus_t phExRf_ActInit_424(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phExRf_ActInit(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phStatus_t status;

    do{
        status = phExRf_ActInit_106(pTxBuffer, pRxBuffer);
        PH_BREAK_ON_SUCCESS(status)

        status = phExRf_ActInit_212(pTxBuffer, pRxBuffer);
        PH_BREAK_ON_SUCCESS(status)

        status = phExRf_ActInit_424(pTxBuffer, pRxBuffer);

    }while(0);
    return status;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static phStatus_t phExRf_ActInit_106(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;
    do
    {
        status = phhalRf_PCD_SetExchgFdt(1356000);
        PH_BREAK_ON_FAILURE(status)

        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_NFCIP1, E_PH_HALRF_ENABLE);
        PH_BREAK_ON_FAILURE(status)

        phUser_MemCpy(pTxBuffer, gkphExRf_ActInit_AtrReq, sizeof(gkphExRf_ActInit_AtrReq));

        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_ActInit_AtrReq), &sRxParams);
        PH_BREAK_ON_FAILURE(status)

        phLED_SetPattern(gkphLED_Clif_Pass);
        LOG_TXT("ATR SUCCESS 106 \n");

        phUser_MemCpy(pTxBuffer, gkphExRf_ActInit_LoopBack, sizeof(gkphExRf_ActInit_LoopBack));
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_ActInit_LoopBack), &sRxParams);
        PH_BREAK_ON_FAILURE(status)

        LOG_TXT("Loop Back SUCCESS 106 \n");
        phLED_SetPattern(gkphLED_Clif_Pass);

    }while(0);

    phhalRf_FieldReset(5100, 5100);

    return status;
}

static phStatus_t phExRf_ActInit_212(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{

    phhalRf_RxParams_t sRxParams;
    phStatus_t status;

    do
    {
        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_ACT_212, E_PHHAL_RF_LP_I_RX_ACT_212);
        PH_BREAK_ON_FAILURE(status)

        status = phhalRf_PCD_SetExchgFdt(1356000);
        PH_BREAK_ON_FAILURE(status)

        phUser_MemCpy(pTxBuffer, gkphExRf_ActInit_AtrReq, sizeof(gkphExRf_ActInit_AtrReq));

        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_ActInit_AtrReq), &sRxParams);
        /* Integrity Issue. */
        if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_INTEGRITY_ERROR))
        {
            break;
        }

        phLED_SetPattern(gkphLED_Clif_Detect);
        LOG_TXT("ATR SUCCESS 212 \n");

        phUser_MemCpy(pTxBuffer, gkphExRf_ActInit_LoopBack, sizeof(gkphExRf_ActInit_LoopBack));
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_ActInit_LoopBack), &sRxParams);
        PH_BREAK_ON_FAILURE(status)

        LOG_TXT("Loop Back SUCCESS 212 \n");
        phLED_SetPattern(gkphLED_Clif_Pass);

    }while(0);

    phhalRf_FieldReset(5100, 5100);

    return status;
}

static phStatus_t phExRf_ActInit_424(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phhalRf_RxParams_t sRxParams;
    phStatus_t status;

    do
    {

        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_ACT_424, E_PHHAL_RF_LP_I_RX_ACT_424);
        PH_BREAK_ON_FAILURE(status)

        status = phhalRf_PCD_SetExchgFdt(1356000);
        PH_BREAK_ON_FAILURE(status)

        phUser_MemCpy(pTxBuffer, gkphExRf_ActInit_AtrReq, sizeof(gkphExRf_ActInit_AtrReq));
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_ActInit_AtrReq), &sRxParams);
        /* Integrity Issue. */
        if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_INTEGRITY_ERROR))
        {
            break;
        }

        phLED_SetPattern(gkphLED_Clif_Detect);
        LOG_TXT("ATR SUCCESS 424 \n");

        phUser_MemCpy(pTxBuffer, gkphExRf_ActInit_LoopBack, sizeof(gkphExRf_ActInit_LoopBack));
        status = phhalRf_PCD_Exchg(pTxBuffer, pRxBuffer, sizeof(gkphExRf_ActInit_LoopBack), &sRxParams);
        PH_BREAK_ON_FAILURE(status)

        LOG_TXT("Loop Back SUCCESS 424 \n");
        phLED_SetPattern(gkphLED_Clif_Pass);

    }while(0);

    phhalRf_FieldReset(5100, 5100);

    return status;
}
