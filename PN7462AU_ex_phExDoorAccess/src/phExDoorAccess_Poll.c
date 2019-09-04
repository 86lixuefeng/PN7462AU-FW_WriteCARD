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
 * phExDoorAccessPoll.c: Contains the core logic for the Clif Reader part.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <phExDoorAccess_Clif.h>
#include <phExDoorAccess_MiFareClassic.h>
#include <phExDoorAccess_Poll.h>
#include <phExDoorAccess_Utils.h>
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phacDiscLoop.h"
#include "ph_Log.h"
#include "phpalFelica.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p4.h"
#include "phpalI18000p3m3.h"
#include "phpalSli15693.h"
#include "phalTop.h"
#include "phalT1T.h"
#include "phalFelica.h"
#include "phUser.h"
#include "phLED.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
#define PH_EXDOORACCESS_MIFARECLASSIC_1K_SAK      0x08
#define PH_EXDOORACCESS_MIFARECLASSIC_4K_SAK      0x18
#define PH_EXDOORACCESS_TYPEA_L4EXCHANGE_MIFAREDESFIRE_SAK      0x20
#define PH_EXDOORACCESS_MIFAREULTRALIGHT_SAK      0x00
#define PH_EXDOORACCESS_PASINI_NFCDEP_SAK   0x40
#define PHPAL_I18000P3M3_TARGET_INVENTORIED_S0    0x00U /**< INVENTORIED_S0 Target. */
#define PHPAL_I18000P3M3_MEMBANK_UII        0x01U   /** < UII Memory Bank. */
#define PHPAL_I18000P3M3_SEL_ALL_00     0x00U   /**< All. */
#define PHPAL_I18000P3M3_SESSION_S0 0x00U   /**< Session S0. */
#define PHPAL_I18000P3M3_TARGET_INVENTORIED_S0    0x00U /**< INVENTORIED_S0 Target. */
#define PHPAL_SLI15693_FLAG_NBSLOTS         0x20U   /**< Use 1 slot instead of 16. */
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
PH_NOINIT static  phpalMifare_Sw_DataParams_t spalMifare;
PH_NOINIT static  phalMfdf_Sw_DataParams_t salMifareDf;
#endif

extern uint8_t gphExDoorAccess_Utils_SysHsuTxBuffer[200];
extern uint16_t str_index;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Reader mode operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 */
void phExDoorAccess_Poll_Main(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_FAILED;
    uint8_t bNumSlots = 0;

    str_index = 0;
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    uint8_t bAfi = 0x00;
    uint8_t bAtqbLen = 0;

#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    uint8_t baSystemCode[2] = { 0xFF, 0xFF };
    uint8_t* pSensFResp;
    uint16_t wSensfLen = 0x00;
#endif

    /**Configure the HSU Hif which will be used for printing application messages on the PC Console*/
    phExDoorAccess_Utils_HsuHifConfig();

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
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

#endif

//static uint32_t gphExDoorAccess_Rxbuf[PH_EXDOORACCESS_CLIF_RXBUFSIZE];
//phhalHif_InitRxBuffer(E_RX_BUFFER_ID0, (PH_EXDOORACCESS_CLIF_RXBUFSIZE), gphExDoorAccess_Rxbuf,(pphhalHif_Callback_t)&phExDoorAccess_Utils_HsuHifReceiveCallBack);
//phExDoorAccess_Utils_Hsu_Print((uint8_t *)gphExDoorAccess_Rxbuf);

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    /** Type A card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        phExDoorAccess_Utils_Fill_Tx_Buffer("\n\nType A Card - ");

        if(psDiscLoopParams->sTypeATargetInfo.bT1TFlag )
        {
			phExDoorAccess_Utils_Fill_Tx_Buffer("Type 1 Tag");

            phExDoorAccess_Utils_Glow_Error_LEDS();
            phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

            (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);

        }
        else
        {
            if((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXDOORACCESS_MIFARECLASSIC_1K_SAK)
                    || (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXDOORACCESS_MIFARECLASSIC_4K_SAK))
            {
                phExDoorAccess_Utils_Fill_Tx_Buffer("MifareClassic - ");

                phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

                phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                /*Mifare classic application to authenticate and read a block of data*/
                status = phExDoorAccess_MiFareClassic(psDiscLoopParams);

                if (status == PH_ERR_SUCCESS)
                {
                    phExDoorAccess_Utils_Glow_Success_LEDS();
                }
                else
                {
                    phExDoorAccess_Utils_Glow_Error_LEDS();
                }

                phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

            }
            else if ((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXDOORACCESS_TYPEA_L4EXCHANGE_MIFAREDESFIRE_SAK))
            {

				phExDoorAccess_Utils_Fill_Tx_Buffer("ISO14443-4A - ");

				phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

				phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                phExDoorAccess_Utils_Glow_Error_LEDS();
                phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

                (void)phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);

            }
            else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXDOORACCESS_MIFAREULTRALIGHT_SAK)
            {

				phExDoorAccess_Utils_Fill_Tx_Buffer("\nType 2 Tag");

				phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

				phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                phExDoorAccess_Utils_Glow_Error_LEDS();
                phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

                (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);

            }
            else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXDOORACCESS_PASINI_NFCDEP_SAK)
            {
                phExDoorAccess_Utils_Fill_Tx_Buffer("Passive Peer - ");

                phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

                phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                phExDoorAccess_Utils_Glow_Error_LEDS();
                phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

            }
            else
            {
                phExDoorAccess_Utils_Fill_Tx_Buffer("Card not supported - ");

                phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

                phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                phExDoorAccess_Utils_Glow_Error_LEDS();
                phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

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
    }
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    /** Type B card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        phExDoorAccess_Utils_Fill_Tx_Buffer("\n\nType B Card - ");

        phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

        phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi,4);

        phExDoorAccess_Utils_Glow_Error_LEDS();
        phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);
        phUser_Wait(10000);

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
    }
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    /** Type F card detected. */
    if ((psDiscLoopParams->bDetectedTechs &  (1 << PHAC_DISCLOOP_TECH_TYPE_F212))
            || (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_F424)))
    {

		phExDoorAccess_Utils_Fill_Tx_Buffer("\n\nType 3 Tag- Felica");

		phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

		phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm,8);

        phExDoorAccess_Utils_Glow_Error_LEDS();
        phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);
        phUser_Wait(10000);

        while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR) || (status == PH_ERR_INTEGRITY_ERROR)))
        {
            status = phpalFelica_ReqC(psDiscLoopParams->pPalFelicaDataParams,
                                        baSystemCode, PHPAL_FELICA_NUMSLOTS_4, &pSensFResp, &wSensfLen);
            phRtos_TaskDelay(500);
            status = status & PH_ERR_MASK;
        }
    }
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    /** Type ICode card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_V))
    {
        phExDoorAccess_Utils_Fill_Tx_Buffer("\n\nICode Sli!!! - ");

        phExDoorAccess_Utils_Fill_Tx_Buffer("UID: ");

        phExDoorAccess_Utils_FillUID_Tx_Buffer(psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid,8);

        phExDoorAccess_Utils_Glow_Error_LEDS();
        phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

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
    }
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    /** Type 18000 card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_18000P3M3))
    {
        phExDoorAccess_Utils_Fill_Tx_Buffer("\n\nICode ILT!!!");

        phExDoorAccess_Utils_Glow_Error_LEDS();
        phExDoorAccess_Utils_Hsu_Print(gphExDoorAccess_Utils_SysHsuTxBuffer);

phStatus_t status = PH_ERR_FAILED;

		uint8_t     aSelectCmd[39] = {0};
		uint8_t    wSelectCmdLen;
		uint8_t     bSelectCmdValidBits;
		uint8_t     aBeginRoundCmd[3] = {0};
		uint8_t     *pRxBuffer;
		uint16_t    wRxBufferLen;

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

            if(status == PH_ERR_SUCCESS)
            {
                phRtos_TaskDelay(500);
            }
            status = status & PH_ERR_MASK;
        }while((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR));
    }
#endif
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
