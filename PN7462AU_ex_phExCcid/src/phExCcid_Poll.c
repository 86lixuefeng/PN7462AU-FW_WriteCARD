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
 * $Date: 2016-09-07 10:17:07 +0530 (Wed, 07 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 3849 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phacDiscLoop.h"
#include "phExCcid_Poll.h"
#include "phExCcid_Clif.h"
#include "phExCcid_MiFareClassic.h"
#include "phExCcid_MiFareUltraLight.h"
#include "phExCcid_TypeA_L4Exchange.h"
#include "phExCcid_TypeB_L4Exchange.h"
#include "phExCcid_Felica.h"
#include "phExCcid_15693.h"
#include "phExCcid_18000p3m3.h"
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
#include "phExCcid_LED.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "phpalMifare.h"
#include "phalMful.h"
#include "phalMfc.h"
#include "phExCcid_Usb_If.h"
#include "phExCcid_UsbCcid.h"
#include "phCfg_EE.h"
#include "phhalHif_Usb.h"
#include "phhalPmu.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXCCID_MIFARE_AUTHENTICATE_LC    0x01U
#define PH_EXCCID_MIFARE_AUTHENTICATE_P2    0x00U
#define PH_EXCCID_MIFARE_CLASSIC_APDU_LC    0x10U
#define PH_EXCCID_MIFARE_ULTRALIGHT_APDU_LC 0x04U

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

phpalMifare_Sw_DataParams_t      *ppalMifare;
phpalMifare_Sw_DataParams_t      spalMifare;
uint8_t gMifareULC;
uint8_t gMifarePlusSL1;
extern phalMfc_Sw_DataParams_t   *palMifareC;
extern phalMful_Sw_DataParams_t  *palMifareUl;

extern uint8_t  gphExCcid_bUid[10];
extern uint8_t  gphExCcid_bUidLength;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExCcid_Handle_USB_Process(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExCcid_Check_Presence_L3A_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExCcid_Check_Presence_L4_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExCcid_Mifare_Do_Auth(uint8_t bAuth);
static void phExCcid_Mifare_Write_Operations(uint8_t bApduLc);
static void phExCcid_Mifare_Read_Operations(void);
static phStatus_t phExCcid_Check_Presence_TypeF_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static phStatus_t phExCcid_Check_Presence_TypeV_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
static phStatus_t phExCcid_Check_Presence_Type18000P3M3_Tag_Read(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif
static phStatus_t phExCcid_MifarePlus_SL1_Check(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phExCcid_Poll_Main(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_FAILED;

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    ppalMifare = &spalMifare;

    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        LOG_TXT("Type A Card - ");
        phExCcid_LED_Status(YELLOW_LED, LED_ON);

        if((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_1K_SAK) ||
           (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_4K_SAK) ||
           (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_TNP3xxx)||
           (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_MF1S020))
        {
            LOG_TXT("Mifare - ");
            status = phExCcid_MifarePlus_SL1_Check(psDiscLoopParams);

            if (status != PH_ERR_SUCCESS)
            {
                gMifarePlusSL1 = 0;
                status = PH_ERR_SUCCESS;

                if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_1K_SAK)
                    LOG_TXT("Classic - 1K\n");
                else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_4K_SAK)
                    LOG_TXT("Classic - 4K\n");
                else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_MF1S020)
                    LOG_TXT("Mini\n");
                else
                    LOG_TXT("Gaming\n");
            }
            status = phExCcid_MiFareClassic(psDiscLoopParams);

            /** Assign the CL Slot as Mifare Classic type. */
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_MIFARECLASSIC;

            /** Get the UID Length of the Card. */
            gphExCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            /** Copy the UID Information. */
            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFAREULTRALIGHT_SAK)
        {
            status = phExCcid_MiFareUltraLight(psDiscLoopParams);

            if (status != PH_ERR_SUCCESS)
            {
                status = PH_ERR_SUCCESS;
                gMifareULC = 0;
                LOG_TXT("Mifare UltraLight\n");
            }
            else
            {
                gMifareULC = 1;
                LOG_TXT("Mifare UltraLight C\n");
            }

            /** Assign the CL Slot type as Mifare Ultralight. */
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT;

            /** Get the UID Length and copy UID Information. */
            gphExCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }
        else if ((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak & PH_EXCCID_TYPEA_L4EXCHANGE_SAK))
        {
            LOG_TXT("ISO14443-4A - ");

            status = phExCcid_TypeA_L4Exchange(psDiscLoopParams);

            /** Assign the CL Slot type as Mifare Desfire. */
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_TYPE_A_L4;

            /** Get the UID Length of the Card. */
            gphExCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            /** Copy the UID Information. */
            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }
        else
        {
             /* Do Nothing */
            status = PH_ERR_SUCCESS;
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_TYPE_A_NS;

            if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == 0x11)
                LOG_TXT("Mifare Plus SL2 4K\n");
            else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == 0x10)
                LOG_TXT("Mifare Plus SL2 2K\n");
            else
                LOG_TXT("Type A Undefined\n");

            /** Get the UID Length of the Card. */
            gphExCcid_bUidLength = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;

            /** Copy the UID Information. */
            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
        }

        if (status ==  PH_ERR_SUCCESS)
        {
            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExCcid_Clif_PollTimer);

            if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC) ||
                (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT))
            {
                /** Get the ATR Information. */
                phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
            }
            else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_A_NS)
            {
                /** Get the ATR Information. */
                phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
            }
            else
            {
                phExCcid_UsbCcid_ATR_FromAts(psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts);
            }

            /** Assign the Slot Type as CL Presence. */
            gphExCcid_sUsb_SlotInfo.bSlotType = PH_EXCCID_USBCCID_CL_CHANNEL_NO;

            /** Call the USB Processing Functions of CCID Class. */
            phExCcid_Handle_USB_Process(psDiscLoopParams);
         }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        LOG_TXT("Type B Card\n");

        phExCcid_LED_Status(YELLOW_LED, LED_ON);

        /** Perform the Initialization of the Type B Card. */
        status = phExCcid_TypeB_L4Exchange(psDiscLoopParams);

        if (status == PH_ERR_SUCCESS)
        {
            /** Get the UID Length and Copy the UID Information. */
            gphExCcid_bUidLength = 4;
            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, 4);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as Type B Card. */
            gphExCcid_sUsb_SlotInfo.bSlotType   = PH_EXCCID_USBCCID_CL_CHANNEL_NO;
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_TYPE_B;

            /** Get the ATR Information. */
            phExCcid_UsbCcid_ATR_TypeBL4(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB, psDiscLoopParams->sTypeBTargetInfo.sTypeB_I3P4.bMbli);

            /** Call the USB Processing Functions of CCID Class. */
            phExCcid_Handle_USB_Process(psDiscLoopParams);

        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    if ((psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_F212)) ||
        (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_F424)))
    {
        LOG_TXT("Felica Card\n");
        phExCcid_LED_Status(YELLOW_LED, LED_ON);

        /** Initialize the Felica Card. */
        status = phExCcid_Felica(psDiscLoopParams);

        if (status == PH_ERR_SUCCESS)
        {
            /** Assign the UID Length and Copy the UID Information. */
            gphExCcid_bUidLength = 8;
            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm, 8);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as Felica Card. */
            gphExCcid_sUsb_SlotInfo.bSlotType   = PH_EXCCID_USBCCID_CL_CHANNEL_NO;
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_FELICA;

            /** Get the ATR Information. */
            phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(0);

            /** Call the USB Processing Functions of the CCID Class. */
            phExCcid_Handle_USB_Process(psDiscLoopParams);
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    /** Type ICode card detected. */
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_V))
    {
        LOG_TXT("ICode SLI !!!\n");
        phExCcid_LED_Status(YELLOW_LED, LED_ON);

        status = phExCcid_15693(psDiscLoopParams);
        if (status == PH_ERR_SUCCESS)
        {
            /** Assign the UID Length and Copy the UID Information. */
            gphExCcid_bUidLength = 8;
            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sTypeVTargetInfo.aTypeV[0].aUid, 8);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as ICODE Card. */
            gphExCcid_sUsb_SlotInfo.bSlotType   = PH_EXCCID_USBCCID_CL_CHANNEL_NO;
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_TYPE_V;

            /** Get the ATR Information. */
            phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(0);

            /** Call the USB Processing Functions of the CCID Class. */
            phExCcid_Handle_USB_Process(psDiscLoopParams);

        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_18000P3M3))
    {
        LOG_TXT("ICode ILT !!!\n");
        phExCcid_LED_Status(YELLOW_LED, LED_ON);

        status = phExCcid_18000p3m3(psDiscLoopParams);

        if (status == PH_ERR_SUCCESS)
        {
            /** Assign the UID Length and Copy the UID Information. */
            gphExCcid_bUidLength = 2;

            phUser_MemCpy(gphExCcid_bUid, psDiscLoopParams->sI18000p3m3TargetInfo.aI18000p3m3[0].aUii, gphExCcid_bUidLength);

            LOG_AU8("UID : ", psDiscLoopParams->sI18000p3m3TargetInfo.aI18000p3m3[0].aUii, 2);

            /** Stop the Polling Loop. */
            phhalTimer_Stop(gpphExCcid_Clif_PollTimer);

            /** Assign the General Slot as CL and CL Slot as ICODE Card. */
            gphExCcid_sUsb_SlotInfo.bSlotType   = PH_EXCCID_USBCCID_CL_CHANNEL_NO;
            gphExCcid_sUsb_SlotInfo.bCLSlotType = PH_EXCCID_USBCCID_CL_18000P3M3;

            /** Get the ATR Information. */
            phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(0);

            /** Call the USB Processing Functions of the CCID Class. */
            phExCcid_Handle_USB_Process(psDiscLoopParams);
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
static void phExCcid_Handle_USB_Process(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    uint8_t *pp;
    uint16_t wOutLen;

    static phStatus_t status = PH_ERR_FAILED;

    volatile uint32_t dwBits = 0;
    uint32_t dwEventTimeOut = 0xF;

    uint8_t bErrorAPDU[2] = { 0x6A, 0x81 };
    uint16_t wOptions = 0;
    /** Send the Card Inserted Information to the host. */
    phExCcid_UsbCcid_CardInserted();

    while (true)
    {
        /** Check for the General Slot Type and CL Slot Type
         *  If nothing set exit the loop
         */
        if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == 0x00) &&
            (gphExCcid_sUsb_SlotInfo.bSlotType == PH_EXCCID_USBCCID_CHANNEL_NONE))
        {
            break;
        }
#ifdef PH_EXCCID_USB_IF_COMPLIANCY
        if (gphExCcid_sUsb_Bus_Status.bAddressed == 0)
        {
        	break;
        }
#endif
#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
        /** If Suspend is initiated by the host
         *  Exit the loop and send the card removal information
         */
        if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
        {
            gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
            phExCcid_UsbCcid_CardRemoved();
            break;
        }
#endif

        if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_B) ||
            (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_A_L4))
        {
            /* Check for the Card Presence and Exit the loop if not presence is detected */
            status = phExCcid_Check_Presence_L4_Card(psDiscLoopParams);
            if (status != PH_ERR_SUCCESS)
            {
                gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                phpalI14443p4_ResetProtocol(psDiscLoopParams->pPal14443p4DataParams);
                phExCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC))
        {
            status = phExCcid_Check_Presence_L3A_Card(psDiscLoopParams);
            /* Check for the Card Presence and Exit the loop if not presence is detected */
            if ((status != PH_ERR_SUCCESS) && (status != PH_ERR_INTEGRITY_ERROR))
            {
                gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT) ||
				 (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_A_NS))
        {
        	status = phExCcid_Check_Presence_L3A_Card(psDiscLoopParams);
        	/* Check for the Card Presence and Exit the loop if not presence is detected */
            if ((status != PH_ERR_SUCCESS) && (status != PH_ERR_INTEGRITY_ERROR))
            {
                gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_V)
        {
            if (phExCcid_Check_Presence_TypeV_Card(psDiscLoopParams) != PH_ERR_SUCCESS)
            {
                gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExCcid_UsbCcid_CardRemoved();
                break;
            }
        }
        else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_FELICA)
        {
            if (phExCcid_Check_Presence_TypeF_Card(psDiscLoopParams) != PH_ERR_SUCCESS)
            {
                gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExCcid_UsbCcid_CardRemoved();
                break;
            }
        }
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
        else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_18000P3M3)
        {
            if (phExCcid_Check_Presence_Type18000P3M3_Tag_Read(psDiscLoopParams) != PH_ERR_SUCCESS)
            {
                gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                phExCcid_UsbCcid_CardRemoved();
                break;
            }
        }
#endif

        /** Wait for the Events for the CL Cards. */
        dwBits = phRtos_EventGroupWaitBits(gphExCcid_sUsb_EventInfo.xCL_Events,
                                           PH_EXCCID_USBCCID_CL_TRNSP_EX_CMD |
                                           PH_EXCCID_USBCCID_CL_DEACTIVATE_CARD_CMD |
                                           PH_EXCCID_USBCCID_CL_AUTH_CMD |
                                           PH_EXCCID_USBCCID_CL_READ_CARD_CMD |
                                           PH_EXCCID_USBCCID_CL_WRITE_CARD_CMD,
                                           true, /* status bits should be cleared before returning. */
                                           false, /* wait for any status bit being set. */
                                           dwEventTimeOut); /* wait until the time expires. */

      /** Deactivate Card Command Event - ICC POWER OFF. */
      if ((dwBits & (uint32_t) PH_EXCCID_USBCCID_CL_DEACTIVATE_CARD_CMD))
      {
		  /** Return the Status for the Request when no exchange is performed */
		  phExCcid_UsbCcid_Set_Output_Payload_Length(0);
		  phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x00, 0x00, 0x00);
       } /* - PH_EXCCID_USBCCID_CL_DEACTIVATE_CARD_CMD */
       /* APDU command from HOST, do transparent exchange and send
        * response to host*/
       if (dwBits & (uint32_t) PH_EXCCID_USBCCID_CL_TRNSP_EX_CMD)
       {
           if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_A_L4) ||
               (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_B))
           {
               switch (phExCcid_UsbCcid_CCID_InHdr_Get_Byte(PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_2))
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
                                               phExCcid_UsbCcid_Get_Input_Payload_Buffer(),
                                               (uint16_t)phExCcid_UsbCcid_Get_Input_Payload_Length(),
                                               (uint8_t **)&pp,
                                               &wOutLen);

        	   gphExCcid_sUsb_Status.bProcess_Pend = 0;
        	   if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
               {
                   /*APDU exchange success, send response to host*/
                   phUser_MemCpy(phExCcid_UsbCcid_Get_Output_Payload_Buffer(), pp, (uint32_t)wOutLen);
                   phExCcid_UsbCcid_Set_Output_Payload_Length((uint32_t)wOutLen);
                   phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
                   phExCcid_LED_TxnPass(gphExCcid_sUsb_SlotInfo.bSlotType);
               }
               else if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
               {
            	   /*APDU exchange success, send response to host*/
				   phUser_MemCpy(phExCcid_UsbCcid_Get_Output_Payload_Buffer(), pp, (uint32_t)wOutLen);
				   phExCcid_UsbCcid_Set_Output_Payload_Length((uint32_t)wOutLen);
				   phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x03);
				   phExCcid_LED_TxnPass(gphExCcid_sUsb_SlotInfo.bSlotType);
               }
               else
               {
            	   phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x42, PH_EXCCID_USBCCID_CCID_ERROR_SLOT_ICC_MUTE, 0x00);
            	   /** Send Card Removal Event and Mark the Transaction as Failure. */
                   gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                   phExCcid_UsbCcid_CardRemoved();
                   phExCcid_LED_TxnFail(gphExCcid_sUsb_SlotInfo.bSlotType);
                   break;
               }
           }
           else
           {
        	   gphExCcid_sUsb_Status.bProcess_Pend = 0;
        	   /** Exchange Not Supported for the Cards. */
               phUser_MemCpy(phExCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
               phExCcid_UsbCcid_Set_Output_Payload_Length(0x2);
               phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
           }
       } /* - PH_EXCCID_USBCCID_CL_TRNSP_EX_CMD */

       /** Card Read Command Events. */
       if (dwBits & (uint32_t) PH_EXCCID_USBCCID_CL_READ_CARD_CMD)
       {
            /** Check for the CL type as Mifare Classic. */
            if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC)
            {
            	/* Put the Card in the known state */
            	if (PH_ERR_SUCCESS != phExCcid_Check_Presence_L3A_Card(psDiscLoopParams))
                {
                	/** Authentication Failure. */
                	phExCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
                	return;
                }
            	/** Check for the Authentication. */
                if (PH_ERR_SUCCESS != phExCcid_Mifare_Do_Auth(PH_EXCCID_MIFARE_AUTHENTICATE_P2))
                {
                    /** Authentication Failure. */
                    phExCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
                    return;
                }

                /** Perform the Read Operation on the Mifare Classic Card. */
                phExCcid_Mifare_Read_Operations();
            }
            else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT)
            {
                /** Perform the Read Operation on the Mifare Ultralight Card. */
                phExCcid_Mifare_Read_Operations();
            }
       } /* - PH_EXCCID_USBCCID_CL_READ_CARD_CMD */

       /** Card Write Command Events. */
       if (dwBits & (uint32_t) PH_EXCCID_USBCCID_CL_WRITE_CARD_CMD)
       {
            if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC)
            {
            	/* Put the Card in the known state */
            	if (PH_ERR_SUCCESS != phExCcid_Check_Presence_L3A_Card(psDiscLoopParams))
				{
					/** Authentication Failure. */
					phExCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
					return;
				}
            	/** Check for Authentication for Mifare Classic Cards. */
                if (PH_ERR_SUCCESS != phExCcid_Mifare_Do_Auth(PH_EXCCID_MIFARE_AUTHENTICATE_P2))
                {
                    /** Authentication Failure. */
                    phExCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
                    return;
                }

                /** Perform the Write Operation in the Mifare Classic Card. */
                phExCcid_Mifare_Write_Operations(PH_EXCCID_MIFARE_CLASSIC_APDU_LC);
            }
            else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT)
            {
                /** Perform the Write Operation in the Mifare Ultralight Card. */
                phExCcid_Mifare_Write_Operations(PH_EXCCID_MIFARE_ULTRALIGHT_APDU_LC);
            }
       } /* - PH_EXCCID_USBCCID_CL_WRITE_CARD_CMD */

       /** Authentication Command Events. */
       if (dwBits & (uint32_t) PH_EXCCID_USBCCID_CL_AUTH_CMD)
       {
            if ((gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC))
            {
            	/* Put the Card in the known state */
            	if (PH_ERR_SUCCESS != phExCcid_Check_Presence_L3A_Card(psDiscLoopParams))
				{
					/** Authentication Failure. */
					phExCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
					return;
				}
            	/** Perform the Authentication in the Mifare Classic Card. */
                status = phExCcid_Mifare_Do_Auth(PH_EXCCID_MIFARE_AUTHENTICATE_LC);

                if (status == PH_ERR_SUCCESS)
                {
                    /** Send the Success Status on Successful Authentication. */
                    gphExCcid_sUsb_MifareInfo.bAuth = 1;
                    phExCcid_UsbCcid_PCSC_Send_APDU(0x90, 0x00, 0);
                }
                else
                {
                    /** Send the Failure Status on Authentication failure. */
                    gphExCcid_sUsb_MifareInfo.bAuth = 0;
                    phExCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
                }
            }
       } /* - PH_EXCCID_USBCCID_CL_AUTH_CMD */
    } /* - While */
}

/**
 * @brief Function to Perform the Authentication process on the Mifare Classic Cards
 * @param bAuth - Flag to check whether authentication is needed or not
 * @return PH_ERR_SUCCESS on Success.
 */
static phStatus_t phExCcid_Mifare_Do_Auth(uint8_t bAuth)
{
    static phStatus_t status = PH_ERR_FAILED;
    uint8_t * pbInputBuffer = NULL;
    uint8_t bUidOffset = 0;

    /** Get the Input Payload Buffer from the host. */
    pbInputBuffer = phExCcid_UsbCcid_Get_Input_Payload_Buffer();

    /** Check the UID Length and Set the Offset of UID. */
    switch (gphExCcid_bUidLength)
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
                                          (bAuth ? pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC + 3] : pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_P2]),
                                          gphExCcid_sUsb_MifareInfo.bKeyType,
                                          &gphExCcid_sUsb_MifareInfo.aMFCKey[0],
                                          &gphExCcid_bUid[bUidOffset]
                                          );

    return status;
}

static void phExCcid_Mifare_Read_Operations()
{
    uint8_t bSW1;
    uint8_t bSW2;
    uint8_t bReplayPayloadLength;
    phStatus_t status = PH_ERR_FAILED;
    uint8_t * pbInputBuffer = NULL;
    uint8_t * pbOutputBuffer = NULL;

    /** Get the Input and Output Payload Buffer Information. */
    pbInputBuffer = phExCcid_UsbCcid_Get_Input_Payload_Buffer();
    pbOutputBuffer = phExCcid_UsbCcid_Get_Output_Payload_Buffer();

    if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC)
    {
        /** Perform the Read Operation. */
        status = phalMfc_Read(palMifareC, pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_P2], &pbOutputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_PAYLOAD]);
    }
    else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT)
    {
        /** Perform the Read Operation for the Mifare Ultralight Card Detected. */
        status = phalMful_Read(palMifareUl, pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_P2], &pbOutputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_PAYLOAD]);
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
        if (pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC] > 0x10)
        {
            bSW1 = 0x62;
            bSW2 = 0x82;
            bReplayPayloadLength = 0x10;
        }
        /** Wrong Le Field. */
        else if ((pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC] < 0x10) && (pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC] != 0x00))
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
    phExCcid_UsbCcid_PCSC_Send_APDU(bSW1, bSW2, bReplayPayloadLength);
    return;

}

/**
 * @brief Function to process the Write Operation of the Mifare Classic and Ultralight Cards
 * @param bApduLc - Information of the Lc Length for the Mifare Classic/Ultralight Cards
 */
static void phExCcid_Mifare_Write_Operations(uint8_t bApduLc)
{
    uint8_t bSW1;
    uint8_t bSW2;
    uint8_t bReplayPayloadLength;
    phStatus_t status = PH_ERR_FAILED;
    uint8_t * pbInputBuffer = NULL;

    /** Get the Input Payload Buffer Information. */
    pbInputBuffer = phExCcid_UsbCcid_Get_Input_Payload_Buffer();

    /** Return Wrong Length if Lc Length is not matching. */
    if (bApduLc > pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC])
    {
        bSW1 = 0x67;
        bSW2 = 0x00;
        bReplayPayloadLength = 0;
    }
    else
    {
        if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC)
        {
            /** Perform the Write Operation for the Mifare Classic Card Detected. */
            status = phalMfc_Write(palMifareC, pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_P2], &pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC + 1]);
        }
        else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT)
        {
            /** Perform the Write Operation for the Mifare Ultralight Card Detected. */
            status = phalMful_Write(palMifareUl, pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_P2], &pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC + 1]);
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
            if (bApduLc < pbInputBuffer[PH_EXCCID_USBCCID_PN7462AU_APDU_LC])
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
    phExCcid_UsbCcid_PCSC_Send_APDU(bSW1, bSW2, bReplayPayloadLength);
    return;
}

/**
 * @brief Function to Check the Presence of L3A Card
 * @param psDiscLoopParams - Parameters for the Type3A card
 * @return PH_ERR_SUCCESS on Success.
 */
static phStatus_t phExCcid_Check_Presence_L3A_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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

static phStatus_t phExCcid_Check_Presence_L4_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
static phStatus_t phExCcid_Check_Presence_TypeV_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
static phStatus_t phExCcid_Check_Presence_TypeF_Card(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
static phStatus_t phExCcid_Check_Presence_Type18000P3M3_Tag_Read(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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

static phStatus_t phExCcid_MifarePlus_SL1_Check(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;
    uint8_t bSak;
    uint8_t bMoreCards;

    uint8_t bUidOutLen;
    uint8_t bUidOut[10];
    status = phpalI14443p4a_Rats (psDiscLoopParams->pPal1443p4aDataParams, 0x8, 0x00, psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts);
    if (status == PH_ERR_SUCCESS)
    {
        if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_1K_SAK)
        {
            /* Mifare Plus SL1 2K */
            gMifarePlusSL1 = 0x01;
            LOG_TXT("Plus SL1 2K\n");
        }
        else if (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_EXCCID_MIFARECLASSIC_4K_SAK)
        {
            /* Mifare Plus SL1 - 4K */
            gMifarePlusSL1 = 0x02;
            LOG_TXT("Plus SL1 4K\n");
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
