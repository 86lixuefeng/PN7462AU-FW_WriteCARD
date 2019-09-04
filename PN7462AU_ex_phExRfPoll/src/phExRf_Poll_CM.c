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
 * phExRf_CM.c:  <The purpose and scope of this file>
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
#include "phhalRf.h"
#include "ph_Status.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phExRf_Poll_A.h"
#include "phExRf_Poll_CM.h"
#include "phLED.h"
#include "phhalRng.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "ph_Reg.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHEXRF_CM_SAK_T2T_SUPPORTED             0x00
#define PHEXRF_CM_SAK_T4ATAG_SUPPORTED          0x20
#define PHEXRF_CM_SAK_NFC_DEP_SUPPORTED         0x40

#define PHEXRF_CM_SAK                           (0x00 | PHEXRF_CM_SAK_T4ATAG_SUPPORTED)
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static phSysSv_Clif_CardMode_TypeA_Param_t gphExRf_A;

#ifdef NXPBUILD__PHHAL_HW_PICC_B
static phhalRf_CardMode_TypeB_Param_t gphExRf_B;

static const uint8_t gkphExRf_Atqb[] = {0x50, 0x10, 0xDF, 0x27, 0xAF, 0x1C, 0x2D, 0x94, 0x11, 0xF7, 0x71, 0x85};
static uint8_t gphExRf_Atqb[sizeof(gkphExRf_Atqb)];
#endif /* NXPBUILD__PHHAL_HW_PICC_B */

static const uint8_t pATR_REP[] = {0x11, 0xD5, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0xAA,
    0xBB, 0xCC, 0xDD};

static const uint8_t gkpAtqa[] = { 0x04, 0x04};

static const uint8_t gkpProtSup[2] = {0x01, 0xFE};

static const uint8_t gkbNfcId2[] = {0x00, 0x68, 0x53, 0xDD, 0xA6, 0xCA};

static const uint8_t gkbPad[] = {0x00, 0x68, 0x53, 0xDD, 0xA6, 0xCA, 0x01, 0x02};

static const uint8_t gkbSystemCode[] = {0xFF, 0xFF};
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExRf_P2P(uint8_t *pTxBuffer, phhalRf_TargetActivatedParam_t * eTargetParam);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/** Card Mode */
phStatus_t phExRf_CM_Activate(uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint16_t wMaxLength)
{
    phhalRf_AutoCollParam_t sAutoCollParams;
    phStatus_t wStatus;
    uint32_t dwListenAll;
    uint32_t dwListen;
    phhalRf_Listen_Tech_t wMode;
    uint8_t * ppRxBuffer;
    uint16_t wRxLength;
    phhalRf_TargetActivatedParam_t sProtParams;

    /* Update the Type B parameter. */
#ifdef NXPBUILD__PHHAL_HW_PICC_B
    phUser_MemCpy(gphExRf_Atqb, gkphExRf_Atqb, sizeof(gkphExRf_Atqb));
    gphExRf_B.pAtqb = (uint8_t *)gphExRf_Atqb;
    gphExRf_B.bExtendedAtqb = false;
    sAutoCollParams.pNFC_B = &gphExRf_B;
#endif /* NXPBUILD__PHHAL_HW_PICC_B */

    /* Update the Type A parameter.
     *
     * |---+---+---+---+---+---+---+---|
     * | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 |
     * |---+---+---+---+---+---+---+---|
     * |  UID  |RFU|    Bit Frame AC   |
     * |---+---+---+---+---+---+---+---|
     *
     * UID = 00 = 4 Byte UID
     * Bit Frame AC, only one bit shall be set
     * RFU shall be 0
     *
     * |----+----+----+----+----+----+----+---|
     * | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 9 |
     * |----+----+----+----+----+----+----+---|
     * |         RFU       | Properitary Cdng |
     * |----+----+----+----+----+----+----+---|
     *
     */
    phUser_MemCpy(&sAutoCollParams.pSenseRes[0x00], gkpAtqa, sizeof(gkpAtqa));
    phhalRng_GenerateRng(&sAutoCollParams.pNfcId1[0],sizeof(sAutoCollParams.pNfcId1));
    sAutoCollParams.bSelRes = PHEXRF_CM_SAK;

    phUser_MemCpy(&sAutoCollParams.bFProtSupp[0x00], gkpProtSup, sizeof(gkpProtSup));
    phUser_MemCpy(&sAutoCollParams.pNfcId2[0], gkbNfcId2, sizeof(gkbNfcId2));
    phUser_MemCpy(&sAutoCollParams.pPad[0], gkbPad, sizeof(gkbPad));
    phUser_MemCpy(&sAutoCollParams.pSystemCode[0], gkbSystemCode, sizeof(gkbSystemCode));

    /* Update the Listen mode. */
    dwListenAll = (uint32_t)E_LISTEN_ALL;

#ifndef NXPBUILD__PHHAL_HW_PICC_B
    dwListen = (uint32_t) E_LISTEN_B;
    dwListen = (~(dwListen));
    dwListenAll &= dwListen;
#endif

#ifndef NXPBUILD__CLIF_ACTIVE_TARGET
    dwListen = (uint32_t) E_LISTEN_ACT_A_ACT_F;
    dwListen = (~(dwListen));
    dwListenAll &= dwListen;
#endif /* NXPBUILD__CLIF_ACTIVE_TARGET */

    wMode = (phhalRf_Listen_Tech_t) dwListenAll;


    do{
        wStatus = phhalRf_AutoColl(&sAutoCollParams, PHHAL_RF_CMA_ENTRY_STATE_IDLE, wMode, &ppRxBuffer, &wRxLength,
            &sProtParams);

        if(wStatus == PH_ERR_SUCCESS)
        {
            phUser_MemCpy(pRxBuffer, ppRxBuffer, wRxLength);
            switch(sProtParams.eProtocolDetected)
            {
            case E_ISO14443_4A:
                phLED_SetPattern(gkphLED_Clif_Detect);
                wStatus = phExRf_A_CM(&sProtParams, pTxBuffer, pRxBuffer, wMaxLength);
                if(wStatus == PH_ERR_SUCCESS)
                {
                    gphExRf_A.bEntryState = PHHAL_RF_CMA_ENTRY_STATE_HALT;
                    dwListenAll = (uint32_t)wMode;
                    dwListen = (uint32_t) E_LISTEN_ACT_A_ACT_F;
                    dwListen = (~(dwListen));
                    dwListenAll &= dwListen;
                    wMode = (phhalRf_Listen_Tech_t) dwListenAll;
                    phLED_SetPattern(gkphLED_Clif_Pass);
                    /* phUser_Wait(30000); */

                }else
                {
                    phLED_SetPattern(gkphLED_Clif_Fail);
                    /* phUser_Wait(30000); */

                }
                break;

            case E_ISO18092:
                phLED_SetPattern(gkphLED_Clif_Detect);
                phExRf_P2P(pTxBuffer, &sProtParams);
                break;

            case E_TYPE_F:
                phLED_SetPattern(gkphLED_Clif_Detect);
                break;

            case E_ISO14443B:
                phLED_SetPattern(gkphLED_Clif_Detect);
                break;

            default:
                PH_USER_ASSERT(0);
            }
        }else
        {
            phLED_SetPattern(gkphLED_Clif_Fail);
            /* phUser_Wait(5000);*/

        }

        if(phhalRf_GetExtFieldStatus() != PH_ERR_SUCCESS)
        {
            /* External field is off */
            break;
        }
    }while(1);

    return wStatus;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static void phExRf_P2P(uint8_t *pTxBuffer, phhalRf_TargetActivatedParam_t * eTargetParam)
{
    phhalRf_RxParams_t pRxParams;
    uint8_t *pRxBuffer;
    phStatus_t status;

    do{

        if (eTargetParam->eBaudRateDetected == E_PH_HALRF_BR_106K)
        {
            phhalRf_SetConfig(PHHAL_RF_CONFIG_NFCIP1, PH_ON);
        }

        phUser_MemCpy(pTxBuffer, pATR_REP, sizeof(pATR_REP));
        status = phhalRf_Transmit(pTxBuffer, sizeof(pATR_REP));
        if (status != PH_ERR_SUCCESS)
        {
            phLED_SetPattern(gkphLED_Clif_Fail);
            break;
        }
        phLED_SetPattern(gkphLED_Clif_Ongoing);

        while(1)
        {
            /* Exchange. */
            do
            {
                status = phhalRf_Receive(&pRxBuffer, &pRxParams);
            }while((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_EXT_RF_ERROR) && ((status & PH_ERR_MASK) != PH_ERR_INTEGRITY_ERROR));
            if ((status  != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_INTEGRITY_ERROR))
            {
                break;
            }

            phUser_MemCpy(pTxBuffer, pRxBuffer, pRxParams.wRxBytes);
            status = phhalRf_Transmit(pTxBuffer, pRxParams.wRxBytes);
            if (status != PH_ERR_SUCCESS)
            {
                break;
            }
        }

        phLED_SetPattern(gkphLED_Clif_Pass);
    }while(0);
}
