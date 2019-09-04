/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * phExCcidPoll.c: Contains the core logic for the Clif Reader part.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phacDiscLoop.h"
#include "phExNFCCcid_Poll.h"
#include "phExNFCCcid_Clif.h"
#include "phExNFCCcid_MiFareClassic.h"
#include "phExNFCCcid_MiFareUltraLight.h"
#include "phExNFCCcid_TypeA_L4Exchange.h"
#include "phExNFCCcid_TypeB_L4Exchange.h"
#include "phExNFCCcid_Felica.h"
#include "phExNFCCcid_15693.h"
#include "phExNFCCcid_18000p3m3.h"
#include "ph_Log.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p3b.h"
#include "phpalFelica.h"
#include "phpalSli15693.h"
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
#include "phpalI18000p3m3.h"
#include "phalI18000p3m3.h"
#endif
#include "phUser.h"
#include "phExNFCCcid_LED.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "phpalMifare.h"
#include "phalMful.h"
#include "phalMfc.h"
#include "phExNFCCcid_Usb_If.h"
#include "phExNFCCcid_UsbCcid.h"
#include "phCfg_EE.h"
#include "phhalHif_Usb.h"
#include "phhalPmu.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXNFCCCID_MIFARE_AUTHENTICATE_LC    0x01U
#define PH_EXNFCCCID_MIFARE_AUTHENTICATE_P2    0x00U
#define PH_EXNFCCCID_MIFARE_CLASSIC_APDU_LC    0x10U
#define PH_EXNFCCCID_MIFARE_ULTRALIGHT_APDU_LC 0x04U

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
uint8_t gMifareULC;
uint8_t gMifarePlusSL1;
phpalMifare_Sw_DataParams_t      *ppalMifare;
phpalMifare_Sw_DataParams_t      spalMifare;

extern phalMfc_Sw_DataParams_t   *palMifareC;
extern phalMful_Sw_DataParams_t  *palMifareUl;

extern uint8_t  gphExNFCCcid_bUid[10];
extern uint8_t  gphExNFCCcid_bUidLength;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExNFCCcid_Handle_USB_Process(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExNFCCcid_Check_Presence_L3A_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExNFCCcid_Check_Presence_L4_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExNFCCcid_Mifare_Do_Auth(uint8_t bAuth);
static void phExNFCCcid_Mifare_Write_Operations(uint8_t bApduLc);
static void phExNFCCcid_Mifare_Read_Operations(void);
static phStatus_t phExNFCCcid_Check_Presence_TypeF_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

static phStatus_t phExNFCCcid_Check_Presence_TypeV_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
static phStatus_t phExNFCCcid_Check_Presence_Type18000P3M3_Tag_Read(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif
static phStatus_t phExNFCCcid_MifarePlus_SL1_Check(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phExNFCCcid_Poll_Main(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_FAILED;

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    ppalMifare = &spalMifare;

    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        LOG_TXT("Type A Card - ");
        phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);

        if((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFARECLASSIC_1K_SAK) ||
           (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFARECLASSIC_4K_SAK) ||
		   (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFARECLASSIC_TNP3xxx)||
		   (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFARECLASSIC_MF1S020))
        {
            LOG_TXT("MifareClassic - ");

            status = phExNFCCcid_MifarePlus_SL1_Check(psDiscLoopParams);

            if (status != PH_ERR_SUCCESS)
            {
                gMifarePlusSL1 = 0x00;
                status = PH_ERR_SUCCESS;
            }

            status = phExNFCCcid_MiFareClassic(psDiscLoopParams);

            /** Assign the CL Slot as Mifare Classic type. */
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC;

            /** Get the UID Length of the Card. */
            gphExNFCCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            /** Copy the UID Information. */
            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFAREULTRALIGHT_SAK)
        {
            /* need to distinguish between UL and ULC , can u a macro*/
            LOG_TXT("Mifare UltraLight - ");

            status = phExNFCCcid_MiFareUltraLight(psDiscLoopParams);

            if (status != PH_ERR_SUCCESS)
            {
                status = PH_ERR_SUCCESS;
                gMifareULC = 0;
            }
            else
            {
                gMifareULC = 1;
            }

            /** Assign the CL Slot type as Mifare Ultralight. */
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT;

            /** Get the UID Length and copy UID Information. */
            gphExNFCCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak & PH_EXNFCCCID_TYPEA_L4EXCHANGE_SAK)
        {
            LOG_TXT("ISO14443-4A - ");

            status = phExNFCCcid_TypeA_L4Exchange(psDiscLoopParams);

            /** Assign the CL Slot type as Mifare Desfire. */
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_TYPE_A_L4;

            /** Get the UID Length of the Card. */
            gphExNFCCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            /** Copy the UID Information. */
            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }
        else
        {
        	 /* Do Nothing */
			status = PH_ERR_SUCCESS;
			gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_TYPE_A_NS;

			/** Get the UID Length of the Card. */
			gphExNFCCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

			/** Copy the UID Information. */
			phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExNFCCcid_bUidLength);

			LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }

        if (status ==  PH_ERR_SUCCESS)
        {
            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExNFCCcid_Clif_PollTimer);

            if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC) ||
                (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT))
            {
                /** Get the ATR Information. */
                phExNFCCcid_UsbCcid_ATR_Felica_Mifare_ICode(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
            }
            else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_A_NS)
			{
            	phExNFCCcid_UsbCcid_ATR_Felica_Mifare_ICode(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
			}
            else
            {
                phExNFCCcid_UsbCcid_ATR_FromAts(psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts);
            }

            /** Assign the Slot Type as CL Presence. */
            gphExNFCCcid_sUsb_SlotInfo.bSlotType = PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO;

            /** Call the USB Processing Functions of CCID Class. */
            phExNFCCcid_Handle_USB_Process(psDiscLoopParams);
         }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        LOG_TXT("Type B Card - ");

        phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);

        /** Perform the Initialization of the Type B Card. */
        status = phExNFCCcid_TypeB_L4Exchange(psDiscLoopParams);

        if (status == PH_ERR_SUCCESS)
        {
            /** Get the UID Length and Copy the UID Information. */
            gphExNFCCcid_bUidLength = 4;
            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, 4);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExNFCCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as Type B Card. */
            gphExNFCCcid_sUsb_SlotInfo.bSlotType   = PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO;
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_TYPE_B;

            /** Get the ATR Information. */
            phExNFCCcid_UsbCcid_ATR_TypeBL4(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB, psDiscLoopParams->sTypeBTargetInfo.sTypeB_I3P4.bMbli);

            /** Call the USB Processing Functions of CCID Class. */
            phExNFCCcid_Handle_USB_Process(psDiscLoopParams);

        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    if ((psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_F212)) ||
        (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_F424)))
    {
        LOG_TXT("Felica Card - ");
        phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);

        /** Initialize the Felica Card. */
        status = phExNFCCcid_Felica(psDiscLoopParams);

        if (status == PH_ERR_SUCCESS)
        {
            /** Assign the UID Length and Copy the UID Information. */
            gphExNFCCcid_bUidLength = 8;
            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm, 8);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExNFCCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as Felica Card. */
            gphExNFCCcid_sUsb_SlotInfo.bSlotType   = PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO;
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_FELICA;

            /** Get the ATR Information. */
            phExNFCCcid_UsbCcid_ATR_Felica_Mifare_ICode(0x0);

            /** Call the USB Processing Functions of the CCID Class. */
            phExNFCCcid_Handle_USB_Process(psDiscLoopParams);
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    /** Type ICode card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_V))
    {
        LOG_TXT("ICode Sli!!!\n");
        phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);

        status = phExNFCCcid_15693(psDiscLoopParams);
        if (status == PH_ERR_SUCCESS)
        {
            /** Assign the UID Length and Copy the UID Information. */
            gphExNFCCcid_bUidLength = 8;
            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid, 8);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExNFCCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as ICODE Card. */
            gphExNFCCcid_sUsb_SlotInfo.bSlotType   = PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO;
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_TYPE_V;

            /** Get the ATR Information. */
            phExNFCCcid_UsbCcid_ATR_Felica_Mifare_ICode(0x0);

            /** Call the USB Processing Functions of the CCID Class. */
            phExNFCCcid_Handle_USB_Process(psDiscLoopParams);

        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_18000P3M3))
    {
        LOG_TXT("ICode ILT!!!\n");
        phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);

        status = phExNFCCcid_18000p3m3(psDiscLoopParams);

        if (status == PH_ERR_SUCCESS)
        {
            /** Assign the UID Length and Copy the UID Information. */
            gphExNFCCcid_bUidLength = 2;

            phUser_MemCpy(gphExNFCCcid_bUid, psDiscLoopParams->sI18000p3m3TargetInfo.aI18000p3m3[0].aUii, gphExNFCCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sI18000p3m3TargetInfo.aI18000p3m3[0].aUii, 2);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExNFCCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as ICODE Card. */
            gphExNFCCcid_sUsb_SlotInfo.bSlotType   = PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO;
            gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = PH_EXNFCCCID_USBCCID_CL_18000P3M3;

            /** Get the ATR Information. */
            phExNFCCcid_UsbCcid_ATR_Felica_Mifare_ICode(0x0);

            /** Call the USB Processing Functions of the CCID Class. */
            phExNFCCcid_Handle_USB_Process(psDiscLoopParams);
        }

    }
#endif

}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/**
 * @brief Function to handle CCID command events based on the Card detected
 * @param psDiscLoopParams - Discovery Loop Parameters used for processing
 */
static void phExNFCCcid_Handle_USB_Process(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    uint8_t *pp;
    uint16_t wOutLen;

    static phStatus_t status = PH_ERR_FAILED;
    volatile uint32_t dwBits = 0;
    uint32_t dwEventTimeOut = 0xF;
    uint16_t wOptions = 0;
    uint8_t bErrorAPDU[2] = { 0x6A, 0x81 };

    /** Send the Card Inserted Information to the host. */
    phExNFCCcid_UsbCcid_CardInserted();

    while (true)
    {
        /** Check for the General Slot Type and CL Slot Type
         *  If nothing set exit the loop
         */
        if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == 0x00) &&
            (gphExNFCCcid_sUsb_SlotInfo.bSlotType == PH_EXNFCCCID_USBCCID_CHANNEL_NONE))
        {
            break;
        }
#ifdef  PH_EXNFCCCID_USB_IF_COMPLIANCY
        if (gphExNFCCcid_sUsb_Bus_Status.bAddressed == 0)
        {
          	break;
        }
#endif
#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
        /** If Suspend is initiated by the host
         *  Exit the loop and send the card removal information
         */
        if (gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable == 1)
        {
            gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
            phExNFCCcid_UsbCcid_CardRemoved();
            break;
        }
#endif

        if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_B) ||
            (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_A_L4))
        {
            /* Check for the Card Presence and Exit the loop if not presence is detected */
            status = phExNFCCcid_Check_Presence_L4_Card(psDiscLoopParams);
            if (status != PH_ERR_SUCCESS)
            {
                gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                phpalI14443p4_ResetProtocol(psDiscLoopParams->pPal14443p4DataParams);
                phExNFCCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC))
        {
            /* Check for the Card Presence and Exit the loop if not presence is detected */
            status = phExNFCCcid_Check_Presence_L3A_Card(psDiscLoopParams);
            if ((status!= PH_ERR_SUCCESS) && (status != PH_ERR_INTEGRITY_ERROR))
            {
                gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExNFCCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT) ||
                 (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_A_NS))
        {
            /* Check for the Card Presence and Exit the loop if not presence is detected */
        	status = phExNFCCcid_Check_Presence_L3A_Card(psDiscLoopParams);
            if ((status!= PH_ERR_SUCCESS) && (status != PH_ERR_INTEGRITY_ERROR))
            {
                gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExNFCCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_V)
        {
            if (phExNFCCcid_Check_Presence_TypeV_Card(psDiscLoopParams) != PH_ERR_SUCCESS)
            {
                gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExNFCCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_FELICA)
        {
            if (phExNFCCcid_Check_Presence_TypeF_Card(psDiscLoopParams) != PH_ERR_SUCCESS)
            {
                gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExNFCCcid_UsbCcid_CardRemoved();
                break;
            }
        }
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
        else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_18000P3M3)
        {
            if (phExNFCCcid_Check_Presence_Type18000P3M3_Tag_Read(psDiscLoopParams) != PH_ERR_SUCCESS)
            {
                gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExNFCCcid_UsbCcid_CardRemoved();
                break;
            }
        }
#endif

        /** Wait for the Events for the CL Cards. */
        dwBits = phRtos_EventGroupWaitBits(gphExNFCCcid_sUsb_SlotInfo.xCL_Events,
                                           PH_EXNFCCCID_USBCCID_CL_TRNSP_EX_CMD |
                                           PH_EXNFCCCID_USBCCID_CL_DEACTIVATE_CARD_CMD |
                                           PH_EXNFCCCID_USBCCID_CL_AUTH_CMD |
                                           PH_EXNFCCCID_USBCCID_CL_READ_CARD_CMD |
                                           PH_EXNFCCCID_USBCCID_CL_WRITE_CARD_CMD,
                                           true, /* status bits should be cleared before returning. */
                                           false, /* wait for any status bit being set. */
                                           dwEventTimeOut); /* wait until the time expires. */

      /** Deactivate Card Command Event - ICC POWER OFF. */
      if ((dwBits & (uint32_t) PH_EXNFCCCID_USBCCID_CL_DEACTIVATE_CARD_CMD))
      {
		  /** Return the Status for the Request when no exchange is performed */
		  phExNFCCcid_UsbCcid_Set_Output_Payload_Length(0);
		  phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x00, 0x00, 0x00);

       } /* - PH_EXNFCCCID_USBCCID_CL_DEACTIVATE_CARD_CMD */
       /* APDU command from HOST, do transparent exchange and send
        * response to host*/
       if (dwBits & (uint32_t) PH_EXNFCCCID_USBCCID_CL_TRNSP_EX_CMD)
       {
           if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_A_L4) ||
               (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_B))
           {
        	   switch (phExNFCCcid_UsbCcid_CCID_InHdr_Get_Byte(PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_2))
			   {
			   case 0x01:
			   case 0x03:
			        wOptions = PH_EXCHANGE_TXCHAINING;
			        break;
			   case 0x10:
			        wOptions = PH_EXCHANGE_RXCHAINING;
			        break;
			   case 0x00:
			   case 0x02:
			   default:
			        wOptions = 0;
			        break;
			   }
        	   status = phpalI14443p4_Exchange(psDiscLoopParams->pPal14443p4DataParams,
        			                           wOptions,
                                               phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer(),
                                               (uint16_t)phExNFCCcid_UsbCcid_Get_Input_Payload_Length(),
                                               (uint8_t **)&pp,
                                               &wOutLen);

               if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
               {
                   /*APDU exchange success, send response to host*/
                   phUser_MemCpy(phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), pp, (uint32_t)wOutLen);
                   phExNFCCcid_UsbCcid_Set_Output_Payload_Length((uint32_t)wOutLen);
                   phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
                   phExNFCCcid_LED_TxnPass(gphExNFCCcid_sUsb_SlotInfo.bSlotType);
               }
               else if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
               {
                   /*APDU exchange success, send response to host*/
                   phUser_MemCpy(phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), pp, (uint32_t)wOutLen);
                   phExNFCCcid_UsbCcid_Set_Output_Payload_Length((uint32_t)wOutLen);
                   phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x03);
                   phExNFCCcid_LED_TxnPass(gphExNFCCcid_sUsb_SlotInfo.bSlotType);
               }
               else
               {
                   /** Timeout Error Occurred. */
                   /** Send Card Removal Event and Mark the Transaction as Failure. */
                   gphExNFCCcid_sUsb_SlotInfo.bAtrValid = 0;
                   phExNFCCcid_UsbCcid_PCSC_Send_APDU((status & 0x00ff), ((0xff00 & status) >> 8), 0);
                   gphExNFCCcid_sUsb_SlotInfo.bCLSlotType = 0x00;
                   phExNFCCcid_UsbCcid_CardRemoved();
                   phExNFCCcid_LED_TxnFail(gphExNFCCcid_sUsb_SlotInfo.bSlotType);
                   break;
               }
           }
           else
           {
               /** Exchange Not Supported for the Cards. */
               phUser_MemCpy(phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
               phExNFCCcid_UsbCcid_Set_Output_Payload_Length(0x2);
               phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
           }
       } /* - PH_EXNFCCCID_USBCCID_CL_TRNSP_EX_CMD */

       /** Card Read Command Events. */
       if (dwBits & (uint32_t) PH_EXNFCCCID_USBCCID_CL_READ_CARD_CMD)
       {
            /** Check for the CL type as Mifare Classic. */
            if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC)
            {
            	/* Put the Card in the known state */
				if (PH_ERR_SUCCESS != phExNFCCcid_Check_Presence_L3A_Card(psDiscLoopParams))
				{
					/** Authentication Failure. */
					phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
					return;
				}
            	/** Check for the Authentication. */
                if (PH_ERR_SUCCESS != phExNFCCcid_Mifare_Do_Auth(PH_EXNFCCCID_MIFARE_AUTHENTICATE_P2))
                {
                    /** Authentication Failure. */
                    phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
                    return;
                }

                /** Perform the Read Operation on the Mifare Classic Card. */
                phExNFCCcid_Mifare_Read_Operations();
            }
            else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT)
            {
                /** Perform the Read Operation on the Mifare Ultralight Card. */
                phExNFCCcid_Mifare_Read_Operations();
            }
       } /* - PH_EXNFCCCID_USBCCID_CL_READ_CARD_CMD */

       /** Card Write Command Events. */
       if (dwBits & (uint32_t) PH_EXNFCCCID_USBCCID_CL_WRITE_CARD_CMD)
       {
            if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC)
            {
            	/* Put the Card in the known state */
				if (PH_ERR_SUCCESS != phExNFCCcid_Check_Presence_L3A_Card(psDiscLoopParams))
				{
					/** Authentication Failure. */
					phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
					return;
				}
            	/** Check for Authentication for Mifare Classic Cards. */
                if (PH_ERR_SUCCESS != phExNFCCcid_Mifare_Do_Auth(PH_EXNFCCCID_MIFARE_AUTHENTICATE_P2))
                {
                    /** Authentication Failure. */
                    phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
                    return;
                }

                /** Perform the Write Operation in the Mifare Classic Card. */
                phExNFCCcid_Mifare_Write_Operations(PH_EXNFCCCID_MIFARE_CLASSIC_APDU_LC);
            }
            else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT)
            {
                /** Perform the Write Operation in the Mifare Ultralight Card. */
                phExNFCCcid_Mifare_Write_Operations(PH_EXNFCCCID_MIFARE_ULTRALIGHT_APDU_LC);
            }
       } /* - PH_EXNFCCCID_USBCCID_CL_WRITE_CARD_CMD */

       /** Authentication Command Events. */
       if (dwBits & (uint32_t) PH_EXNFCCCID_USBCCID_CL_AUTH_CMD)
       {
            if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC))
            {
            	/* Put the Card in the known state */
				if (PH_ERR_SUCCESS != phExNFCCcid_Check_Presence_L3A_Card(psDiscLoopParams))
				{
					/** Authentication Failure. */
					phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
					return;
				}
            	/** Perform the Authentication in the Mifare Classic Card. */
                status = phExNFCCcid_Mifare_Do_Auth(PH_EXNFCCCID_MIFARE_AUTHENTICATE_LC);

                if (status == PH_ERR_SUCCESS)
                {
                    /** Send the Success Status on Successful Authentication. */
                    gphExNFCCcid_sUsb_MifareInfo.bAuth = 1;
                    phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x90, 0x00, 0);
                }
                else
                {
                    /** Send the Failure Status on Authentication failure. */
                    gphExNFCCcid_sUsb_MifareInfo.bAuth = 0;
                    phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
                }
            }
       } /* - PH_EXNFCCCID_USBCCID_CL_AUTH_CMD */
    } /* - While */
}

/**
 * @brief Function to Perform the Authentication process on the Mifare Classic Cards
 * @param bAuth - Flag to check whether authentication is needed or not
 * @return PH_ERR_SUCCESS on Success.
 */
static phStatus_t phExNFCCcid_Mifare_Do_Auth(uint8_t bAuth)
{
    static phStatus_t status = PH_ERR_FAILED;
    uint8_t * pbInputBuffer = NULL;
    uint8_t bUidOffset = 0;

    /** Get the Input Payload Buffer from the host. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /** Check the UID Length and Set the Offset of UID. */
    switch (gphExNFCCcid_bUidLength)
    {
    case 0x04:
        bUidOffset = 0;
        break;
    case 0x07:
        bUidOffset = 3;
        break;
    case 0x00:
    default:
        bUidOffset = 0;
        break;
    }

    /** Call the Authentication function of the Mifare Classic. */
    status = phpalMifare_MfcAuthenticate (ppalMifare,
                                          (bAuth ? pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 3] : pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2]),
                                          gphExNFCCcid_sUsb_MifareInfo.bKeyType,
                                          &gphExNFCCcid_sUsb_MifareInfo.aMFCKey[0],
                                          &gphExNFCCcid_bUid[bUidOffset]
                                          );

    return status;
}

static void phExNFCCcid_Mifare_Read_Operations()
{
    uint8_t bSW1;
    uint8_t bSW2;
    uint8_t bReplayPayloadLength;
    phStatus_t status = PH_ERR_FAILED;
    uint8_t * pbInputBuffer = NULL;
    uint8_t * pbOutputBuffer = NULL;

    /** Get the Input and Output Payload Buffer Information. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();
    pbOutputBuffer = phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer();

    if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC)
    {
        /** Perform the Read Operation. */
        status = phalMfc_Read(palMifareC, pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2], &pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD]);
    }
    else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT)
    {
        /** Perform the Read Operation for the Mifare Ultralight Card Detected. */
        status = phalMful_Read(palMifareUl, pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2], &pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD]);
    }
    else
    {
        /* Do nothing */
    }

    /** Failure return security status not satisfied. */
    if (status != PH_ERR_SUCCESS)
    {
        bSW1 = 0x69;
        bSW2 = 0x82;
        bReplayPayloadLength = 0;
    }
    else
    {
        /** Check the Length of Lc & greater than 0x10 Send Error
         *  End of Record Reached
         */
        if (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] > 0x10)
        {
            bSW1 = 0x62;
            bSW2 = 0x82;
            bReplayPayloadLength = 0x10;
        }
        /** Wrong Le Field. */
        else if ((pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] < 0x10) && (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] != 0x00))
        {
            bSW1 = 0x6C;
            bSW2 = 0x10;
            bReplayPayloadLength = 0;
        }
        else
        {
            /** Success Status. */
            bSW1 = 0x90;
            bSW2 = 0x00;
            bReplayPayloadLength = 0x10;
        }
    }

    /** Send the APDU Status Information. */
    phExNFCCcid_UsbCcid_PCSC_Send_APDU(bSW1, bSW2, bReplayPayloadLength);
    return;

}

/**
 * @brief Function to process the Write Operation of the Mifare Classic and Ultralight Cards
 * @param bApduLc - Information of the Lc Length for the Mifare Classic/Ultralight Cards
 */
static void phExNFCCcid_Mifare_Write_Operations(uint8_t bApduLc)
{
    uint8_t bSW1;
    uint8_t bSW2;
    uint8_t bReplayPayloadLength;
    phStatus_t status = PH_ERR_FAILED;
    uint8_t * pbInputBuffer = NULL;

    /** Get the Input Payload Buffer Information. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /** Return Wrong Length if Lc Length is not matching. */
    if (bApduLc > pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC])
    {
        bSW1 = 0x67;
        bSW2 = 0x00;
        bReplayPayloadLength = 0;
    }
    else
    {
        if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC)
        {
            /** Perform the Write Operation for the Mifare Classic Card Detected. */
            status = phalMfc_Write(palMifareC, pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2], &pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 1]);
        }
        else if (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT)
        {
            /** Perform the Write Operation for the Mifare Ultralight Card Detected. */
            status = phalMful_Write(palMifareUl, pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2], &pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 1]);
        }
        else
        {
            /* Do nothing */
        }

        /** Memory Failure Error. */
        if (status != PH_ERR_SUCCESS)
        {
            bSW1 = 0x65;
            bSW2 = 0x81;
            bReplayPayloadLength = 0;
        }
        else
        {
            if (bApduLc < pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC])
            {
                bSW1 = 0x62;
                bSW2 = 0x82;
                bReplayPayloadLength = 0;
            }
            else
            {
                /** Success. */
                bSW1 = 0x90;
                bSW2 = 0x00;
                bReplayPayloadLength = 0;
            }
        }
    }

    /** Send the APDU Response back to the host. */
    phExNFCCcid_UsbCcid_PCSC_Send_APDU(bSW1, bSW2, bReplayPayloadLength);
    return;
}

/**
 * @brief Function to Check the Presence of L3A Card
 * @param psDiscLoopParams - Parameters for the Type3A card
 * @return PH_ERR_SUCCESS on Success.
 */
static phStatus_t phExNFCCcid_Check_Presence_L3A_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    static phStatus_t status = PH_ERR_FAILED;

    uint8_t bSak;
    uint8_t bMoreCards;

    uint8_t bUidOutLen;
    uint8_t bUidOut[10];

    /* first we have to set correct HAL settings */
    status = phhalHw_ApplyProtocolSettings(psDiscLoopParams->pHalDataParams,
                                           PHHAL_HW_CARDTYPE_CURRENT);

    if (status != PH_ERR_SUCCESS)
    {
    	status = status & PH_ERR_MASK;
    	return status;
    }


    /* halt the card */
    status = phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);

    if (status != PH_ERR_SUCCESS)
    {
    	status = status & PH_ERR_MASK;
        return status;
    }

    /* activate the card again */
    status = phpalI14443p3a_ActivateCard(psDiscLoopParams->pPal1443p3aDataParams,
                                         psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                                         psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize,
                                         &bUidOut[0],
                                         &bUidOutLen,
                                         &bSak,
                                         &bMoreCards);

    status = status & PH_ERR_MASK;
    return status;

}

/**
 * @brief Function to Check the Presence of L4 Card
 * @param psDiscLoopParams - Parameters for the Type4 card
 * @return PH_ERR_SUCCESS on Success.
 */

static phStatus_t phExNFCCcid_Check_Presence_L4_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_FAILED;
    uint8_t bCount;

    for (bCount = 0; bCount < 2; bCount++)
    {
        status = phpalI14443p4_PresCheck(psDiscLoopParams->pPal14443p4DataParams);

        if (status == PH_ERR_SUCCESS)
            break;
        phRtos_TaskDelay(10);
	}
	return status;
}

/**
 * @brief Function to Check the Presence of Type V Card
 * @param psDiscLoopParams - Parameters for the TypeV Card
 * @return PH_ERR_SUCCESS on Success
 */
static phStatus_t phExNFCCcid_Check_Presence_TypeV_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	static phStatus_t status = PH_ERR_FAILED;

	status = phpalSli15693_Inventory(
                 psDiscLoopParams->pPalSli15693DataParams,
                 psDiscLoopParams->sTypeVTargetInfo.bFlag | PHPAL_SLI15693_FLAG_NBSLOTS | PHPAL_SLI15693_FLAG_INVENTORY,
                 0,
                 NULL,
                 0,
                 &psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].bDsfid,
                 psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid
                 );
    status = status & PH_ERR_MASK;

    return status;
}

/**
 * @brief Function to Check the Presence of Type F Card
 * @param psDiscLoopParams - Parameters for the TypeF Card
 * @return PH_ERR_SUCCESS on Success
 */
static phStatus_t phExNFCCcid_Check_Presence_TypeF_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	static phStatus_t status = PH_ERR_FAILED;
	uint8_t bNumSlots = PHPAL_FELICA_NUMSLOTS_1;
    uint8_t baSystemCode[2] = { 0xFF, 0xFF };
    uint8_t* pSensFResp;
    uint16_t wSensfLen;

    status = phpalFelica_ReqC(psDiscLoopParams->pPalFelicaDataParams,
                              baSystemCode, bNumSlots, &pSensFResp, &wSensfLen);

    status = status & PH_ERR_MASK;

    return status;
}

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
static phStatus_t phExNFCCcid_Check_Presence_Type18000P3M3_Tag_Read(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    uint16_t wRxLength = 0;
    uint8_t *pRxbuffer;
    uint8_t bWordCount = 0x01;
    uint8_t bWordPtrLength = 0x00;
    uint8_t bMemBank = 0x03;
    uint8_t bWordPtr = 0x00;


    /** performing read operation */
    (void )phhalHw_SetConfig(
                psDiscLoopParams->pPal18000p3m3DataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
                0x58);
    status = phalI18000p3m3_Read(psDiscLoopParams->pAl18000p3m3DataParams,
                bMemBank,
                &bWordPtr,
                bWordPtrLength,
                bWordCount,
                &pRxbuffer,
                &wRxLength
            );

    status = status & PH_ERR_MASK;
    return status;
}
#endif

static phStatus_t phExNFCCcid_MifarePlus_SL1_Check(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;
    uint8_t bSak;
    uint8_t bMoreCards;

    uint8_t bUidOutLen;
    uint8_t bUidOut[10];
    status = phpalI14443p4a_Rats (psDiscLoopParams->pPal1443p4aDataParams, 0x8, 0x00, psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts);

    if (status == PH_ERR_SUCCESS)
    {
        if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFARECLASSIC_1K_SAK)
        {
            /* Mifare Plus SL1 2K */
            gMifarePlusSL1 = 0x01;
        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXNFCCCID_MIFARECLASSIC_4K_SAK)
        {
            /* Mifare Plus SL1 - 4K */
            gMifarePlusSL1 = 0x02;
        }
        else
        {
            gMifarePlusSL1 = 0x00;
        }

        phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);

        phpalI14443p3a_ActivateCard(psDiscLoopParams->pPal1443p3aDataParams,
                                    psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                                    psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize,
                                    &bUidOut[0],
                                    &bUidOutLen,
                                    &bSak,
                                    &bMoreCards);
    }
    status = status & PH_ERR_MASK;

    return status;
}

