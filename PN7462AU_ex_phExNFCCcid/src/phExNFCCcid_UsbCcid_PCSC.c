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
 * phExNFCCcid_UsbCcid_PCSC.c: <This File process the CCID PCSC APDU commands >
 *
 * Project: PN640
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phExNFCCcid_LED.h"
#include "phUser.h"

#include "phhalTimer.h"
#include "phExNFCCcid_UsbCcid.h"

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
extern phhalTimer_Timers_t *pLedTimer;
extern phhalTimer_Timers_t *gpphExNFCCcid_Clif_PollTimer;

uint8_t gphExNFCCcid_bUid[10];
uint8_t gphExNFCCcid_bUidLength;

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXNFCCCID_PCSC_STD_EXT_READ_BIN    0x1U
#define PH_EXNFCCCID_PCSC_STD_EXT_WRITE_BIN   0x0U


/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExNFCCcid_PCSC_Std_Ext_Cmd_Get_Data (void);
static void phExNFCCcid_Get_FW_Version(void);
static void phExNFCCcid_PCSC_Std_Ext_Cmd_Load_Key(void);
static void phExNFCCcid_PCSC_Std_Ext_Cmd_Auth(void);
static void phExNFCCcid_PCSC_Std_Ext_Cmd(uint8_t bReadOrWriteBin);

static void phExNFCCcid_PCSC_Start_Polling(uint8_t bEscape);
static void phExNFCCcid_PCSC_Stop_Polling(uint8_t bEscape);
static void phExNFCCcid_PCSC_LED_Control(uint8_t bApduIns, uint8_t bEscape);

static void phExNFCCcid_PCSC_Escape_Operation_Mode(void);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * @brief Handle the Custom Command According to the CCID command from PC
 * @param bEscape - 1 for 0x6B Escape Command Support, 0 for 0x6F Command
 */
void phExNFCCcid_UsbCcid_Escape_Function(uint8_t bEscape)
{
    uint8_t bMessageType;                             /* Message Type for Return packet      */
    uint8_t * pbInputBuffer = NULL;                   /* Input Payload Buffer for parsing    */
    uint8_t bErrorAPDU[2] = { 0x6A, 0x81 };           /* Error Code - Function not Supported */
    uint8_t bSuccessAPDU[2] = {0x90, 0x00};

    /** Get the Buffer transmitted by host. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /* Assign the Response Message Type based on Input Message */
    bMessageType = bEscape ? PH_EXNFCCCID_USBCCID_RDR_TO_PC_ESCAPE : PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK;

    /** Check for CLA as 0xA0 and Length equals 2. */
    if ((gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH] == 0xA0) &&
        (phExNFCCcid_UsbCcid_Get_Input_Payload_Length() == 2))
    {
        /** Check the INS. */
        switch (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_INS])
        {
        case PH_EXNFCCCID_USBCCID_ESCAPE_BLUE_LED_ON:
        case PH_EXNFCCCID_USBCCID_ESCAPE_GREEN_LED_ON:
        case PH_EXNFCCCID_USBCCID_ESCAPE_YELLOW_LED_ON:
        case PH_EXNFCCCID_USBCCID_ESCAPE_RED_LED_ON:
        case PH_EXNFCCCID_USBCCID_ESCAPE_BLUE_LED_OFF:
        case PH_EXNFCCCID_USBCCID_ESCAPE_GREEN_LED_OFF:
        case PH_EXNFCCCID_USBCCID_ESCAPE_YELLOW_LED_OFF:
        case PH_EXNFCCCID_USBCCID_ESCAPE_RED_LED_OFF:
        case PH_EXNFCCCID_USBCCID_ESCAPE_ALL_LED_OFF:
        case PH_EXNFCCCID_USBCCID_ESCAPE_ALL_LED_ON:
            phExNFCCcid_PCSC_LED_Control(pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_INS], bEscape);
            break;
        case PH_EXNFCCCID_USBCCID_ESCAPE_START_POLLING:
            /* Start the Polling Loop */
            phExNFCCcid_PCSC_Start_Polling(bEscape);
            break;
        case PH_EXNFCCCID_USBCCID_ESCAPE_STOP_POLLING:
            /* Stop the Polling Loop. */
            phExNFCCcid_PCSC_Stop_Polling(bEscape);
            break;
        default:
            /* Assign the Error Code in the Output Buffer */
            phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
            /* Set the Output Payload Length */
            phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);
            /* Send the Response for the Request */
            phExNFCCcid_UsbCcid_Send_Frame(bMessageType, 0x00, 0x00, 0x00);
            return;
        }
        /* Assign the Success Code in the Output Buffer */
        phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bSuccessAPDU[0], 2);
        /* Set the Payload Length */
        phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);
        /* Send the Response for the request */
        phExNFCCcid_UsbCcid_Send_Frame(bMessageType, 0x00, 0x00, 0x00);

    }
    /* Send the Error Code if the CLA is different */
    else
    {
        phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
        phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);
        phExNFCCcid_UsbCcid_Send_Frame(bMessageType, 0x00, 0x00, 0x00);
    }
}

void phExNFCCcid_UsbCcid_Operation_Mode(void)
{
    uint8_t * pbInputBuffer = NULL;
    uint8_t bErrorAPDU[2] = { 0x6A, 0x81 };                 /* Error Code - Function Not Supported */

    /** Get the Buffer transmitted by the host. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    if ((gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH] == 0xFF) &&
        (phExNFCCcid_UsbCcid_Get_Input_Payload_Length() > 4))
    {
        switch (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_INS])
        {
        case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_OPERATION_MODE_INS:
            phExNFCCcid_PCSC_Escape_Operation_Mode();
            break;
        default:
          {
              /* Send the Error Code - Function not Supported */
              phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);

              /* Set the output Payload length */
              phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);

              /* Send the Response for the Request */
              phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_ESCAPE, 0x00, 0x00, 0x00);
              return;;
          }
        }
    }
    /* Failure - CLA is not 0xFF or the Input Payload Length is not greater than 4 */
    else
    {
       /* Set the Error Code */
       phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
       phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);
       /* Send the Response */
       phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_ESCAPE, 0x00, 0x00, 0x00);
       phExNFCCcid_LED_TxnFail(gphExNFCCcid_sUsb_SlotInfo.bSlotType);
    }
}

/**
 * @brief Function to Decode the APDU and post to the CT/CL Task
 */
void phExNFCCcid_UsbCcid_Decode_APDU(void)
{
    uint8_t * pbInputBuffer = NULL;
    uint8_t bErrorAPDU[2] = { 0x6A, 0x81 };                 /* Error Code - Function Not Supported */

    /** Get the Buffer transmitted by the host. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    if (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH] == 0xFF)
    {
        if ((phExNFCCcid_UsbCcid_Get_Input_Payload_Length() <= 4))
        {
            /* Set the Error Code */
            bErrorAPDU[0] = 0x67;
            bErrorAPDU[1] = 0x00;
            phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
            phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);
            /* Send the Response */
            phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
            return;
        }
        else
        {
            switch (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_INS])
            {
            case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_GET_DATA_INS:
                 /* Get the Data Information like UID etc from the card */
                 phExNFCCcid_PCSC_Std_Ext_Cmd_Get_Data();
                 return;
            case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_GET_FW_VERS_INS:
                 /* User Defined APDU command to get the Version Information */
                 phExNFCCcid_Get_FW_Version();
                 return;
            case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LOAD_KEY_INS:
                 /* External Authenticate */
                 phExNFCCcid_PCSC_Std_Ext_Cmd_Load_Key();
                 return;
            case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_G_AUTH_CMD_INS:
                 if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC) ||
                     (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT))
                 {
                     /* Perform the General Authentication */
                     phExNFCCcid_PCSC_Std_Ext_Cmd_Auth();
                     return;
                 }
                 break;
            case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_READ_BIN:
                 if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC) ||
                     (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT))
                 {
                     /* Read the Binary Data from Card */
                     phExNFCCcid_PCSC_Std_Ext_Cmd(PH_EXNFCCCID_PCSC_STD_EXT_READ_BIN);
                     return;
                 }
                 break;
            case PH_EXNFCCCID_USBCCID_PN7462AU_APDU_UPDATE_BIN:
                 if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC) ||
                     (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT))
                 {
                     /* Update the Binary Information */
                     phExNFCCcid_PCSC_Std_Ext_Cmd(PH_EXNFCCCID_PCSC_STD_EXT_WRITE_BIN);
                     return;
                 }
                 break;
            }
        }
    }
    if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_A_L4) ||
        (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_B))
    {
        phExNFCCcid_UsbCcid_UsbPostEventCLTask(PH_EXNFCCCID_USBCCID_CL_TRNSP_EX_CMD);
    }
    else
    {
        phUser_MemCpy (phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer(), &bErrorAPDU[0], 2);
        phExNFCCcid_UsbCcid_Set_Output_Payload_Length(2);
        /* Send the Response */
        phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
        phExNFCCcid_LED_TxnFail(gphExNFCCcid_sUsb_SlotInfo.bSlotType);
    }
}

/**
 * @brief Function to Send the Response for the APDU requests
 * @param bSW1 - Processing State of the Card - Warning/Error
 * @param bSW2 - Processing State of the Card - Warning/Error
 * @param wPayloadLength - Payload Length of the Response data
 * @return PH_ERR_SUCCESS
 */
phStatus_t phExNFCCcid_UsbCcid_PCSC_Send_APDU(uint8_t bSW1, uint8_t bSW2, uint16_t wPayloadLength)
{
    uint8_t * pbOutputBuffer = NULL;

    /* Get the Output Buffer */
    pbOutputBuffer = phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer();

    /* Set the SW1 and SW2 */
    pbOutputBuffer[wPayloadLength + 0] = bSW1;
    pbOutputBuffer[wPayloadLength + 1] = bSW2;

    /* Set the Output Length + 2SW bytes */
    phExNFCCcid_UsbCcid_Set_Output_Payload_Length(wPayloadLength + 2);

    /* Send the frame */
    phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);

    if (bSW1 == 0x90)
    {
        /* For Success Blink the Green Led */
        phExNFCCcid_LED_TxnPass(gphExNFCCcid_sUsb_SlotInfo.bSlotType);

    }
    else
    {
        /* Failure blink the Red Led */
        phExNFCCcid_LED_TxnFail(gphExNFCCcid_sUsb_SlotInfo.bSlotType);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phExNFCCcid_UsbCcid_PCSC_SendEscape_APDU(uint8_t bSW1, uint8_t bSW2, uint16_t wPayloadLength)
{
    uint8_t * pbOutputBuffer = NULL;

    /* Get the Output Buffer */
    pbOutputBuffer = phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer();

    /* Set the SW1 and SW2 */
    pbOutputBuffer[wPayloadLength + 0] = bSW1;
    pbOutputBuffer[wPayloadLength + 1] = bSW2;

    /* Set the Output Length + 2SW bytes */
    phExNFCCcid_UsbCcid_Set_Output_Payload_Length(wPayloadLength + 2);

    /* Send the frame */
    phExNFCCcid_UsbCcid_Send_Frame(PH_EXNFCCCID_USBCCID_RDR_TO_PC_ESCAPE, 0x00, 0x00, 0x00);

    if (bSW1 == 0x90)
    {
        /* For Success Blink the Green Led */
        phExNFCCcid_LED_TxnPass(0x01);
    }
    else
    {
        /* Failure blink the Red Led */
        phExNFCCcid_LED_TxnFail(0x01);
    }
    return PH_ERR_SUCCESS;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static void phExNFCCcid_PCSC_Escape_Operation_Mode()
{
    uint8_t * pbInputBuffer = NULL;

    /* Get the Input Buffer */
    pbInputBuffer  = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /** Check the Payload Length and FW version Length Input. */
    if ((phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < 5) || (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] != PH_EXNFCCCID_USBCCID_PN7462AU_OPERATION_MODE_LEN))
    {
        /* return error: Wrong length */
        phExNFCCcid_UsbCcid_PCSC_SendEscape_APDU(0x67, 0x00, 0x00);
        return;
    }

    /** Check the P1-P2 parameters. */
    if ((0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1]) || (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2]))
    {
        /* return error: Wrong parameter P1-P2 */
        phExNFCCcid_UsbCcid_PCSC_SendEscape_APDU (0x6B, 0x00, 0);
        return;
    }

    gphExNFCCcid_sUsb_Operation_Mode.bOperationMode = pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 1];

    /** Send the APDU with Success Code. */
    phExNFCCcid_UsbCcid_PCSC_SendEscape_APDU (0x90, 0x00, 0x00);

    return;
}

/**
 * @brief Function to Return the FW version - User Defined APDU
 */
static void phExNFCCcid_Get_FW_Version()
{
    uint8_t * pbInputBuffer = NULL;
    uint8_t * pbOutputBuffer = NULL;

    uint8_t bVersionAPDU[PH_EXNFCCCID_USBCCID_PN7462AU_FW_UC_VERSION_LEN] = {
    '7','4','6','2','A','U',' ','0','2',' ','0','2',' ','0','2',' ','U','C',' ','0','2',' ','0','0',' ','0','0'
    };

    /* Get the Input Buffer */
    pbInputBuffer  = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();
    /* Get the Output Buffer pointer */
    pbOutputBuffer = phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer();

    /** Check the Payload Length and FW version Length Input. */
    if ((phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < 5) || (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] != PH_EXNFCCCID_USBCCID_PN7462AU_FW_UC_VERSION_LEN))
    {
        /* return error: Wrong length */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x67, 0x00, 0x00);
        return;
    }

    /** Check the P1-P2 parameters. */
    if ((0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1]) || (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2]))
    {
        /* return error: Wrong parameter P1-P2 */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x6B, 0x00, 0);
        return;
    }

    /** Copy the FW version Information to output buffer. */
    phUser_MemCpy (&pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD], &bVersionAPDU[0], PH_EXNFCCCID_USBCCID_PN7462AU_FW_UC_VERSION_LEN);

    /** Send the APDU with Success Code. */
    phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x90, 0x00, PH_EXNFCCCID_USBCCID_PN7462AU_FW_UC_VERSION_LEN);
    return;
}

/**
 * @brief Function to Get the Card Data Like UID information
 */
static void phExNFCCcid_PCSC_Std_Ext_Cmd_Get_Data ()
{
    uint8_t * pbInputBuffer = NULL;
    uint8_t * pbOutputBuffer = NULL;

    /* Get the Input Buffer Pointer Information */
    pbInputBuffer  = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /* Get the Output Buffer Pointer Information */
    pbOutputBuffer = phExNFCCcid_UsbCcid_Get_Output_Payload_Buffer();

    /** Check the Input Payload Length. */
    if (phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < 5)
    {
        /* return error: Wrong length */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x67, 0x00, 0x00);
        return;
    }

    /** Check the P2 Param. */
    if (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2])
    {
        /* return error: Wrong parameter P1-P2 */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6B, 0x00, 0);
        return;
    }

    /** Check the P1 Parameter Value. */
    switch (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1])
    {
        /* return UID or PUPI */
        case 0x00:
            switch (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LE])
            {
                case 0x04:
                case 0x07:
                case 0x0A:
                case 0x08:
                     if (gphExNFCCcid_bUidLength == pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LE])
                     {
                         /* Send the UID Information */
                         phUser_MemCpy (&pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD], &gphExNFCCcid_bUid[0], gphExNFCCcid_bUidLength);
                         phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x90, 0x00, gphExNFCCcid_bUidLength);
                         return;
                     }
                case 0x00:
                     if (0x00 == pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LE])
                     {
                         /* Send the UID Information */
                         phUser_MemCpy (&pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD], &gphExNFCCcid_bUid[0], gphExNFCCcid_bUidLength);
                         phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x90, 0x00, gphExNFCCcid_bUidLength);
                         return;
                     }
                default:
                     if (gphExNFCCcid_bUidLength < pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LE])
                     {
                         phUser_MemSet(&pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD], 0x00, pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LE]);
                         /* Send the UID Information */
                         phUser_MemCpy (&pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD], &gphExNFCCcid_bUid[0], gphExNFCCcid_bUidLength);
                         phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x62, 0x82, pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LE]);
                         return;
                      }
                     else
                     {
                         /* return length error - Wrong Le field */
                         phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x6C, gphExNFCCcid_bUidLength, 0);
                         return;
                     }
            }

        case 0x01:
            /* we return ATS only for L4 cards*/
            if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_A_L4) ||
                (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_TYPE_B))
            {
                phUser_MemCpy (&pbOutputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_PAYLOAD], &gphExNFCCcid_bUid[0], gphExNFCCcid_bUidLength);
                phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x90, 0x00, gphExNFCCcid_bUidLength);
                return;
            }
            else
            {
                phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x6A, 0x81, 0);
                return;
            }

        default:
            phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x6B, 0x00, 0);
            return;
    }
}

/**
 * @brief Perform the External authentication
 */
static void phExNFCCcid_PCSC_Std_Ext_Cmd_Load_Key(void)
{
    uint8_t * pbInputBuffer = NULL;

    /** Get the Input Payload Buffer. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /** Check the Input Payload Length. */
    if (phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < 5)
    {
        /* return error: Wrong length */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x67, 0x00, 0x00);
        return;
    }

    /* Check the P1 Param */
    if (0x80 & pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1])
    {
        /* Reader Key not supported */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x83, 0x00);
        return;
    }

    if (0x40 & pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1])
    {
        /* Reader Key not supported */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x85, 0x00);
        return;
    }

    if (0x20 & pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1])
    {
        /* Reader Key not supported */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x87, 0x00);
        return;
    }

    /* Check the P2 Param */
    if (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2])
    {
        /* we have room for only one key */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x88, 0x00);
        return;
    }

    /* Check if key is correct length */
    if ((phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < (5 + pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC])) ||
        (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] > PH_EXNFCCCID_USBCCID_PN7462AU_MAX_KEY_LEN)               ||
        (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC] == 0))
    {
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x89, 0x00);
        return;
    }

    /* Copy and Initialize the key structure */
    gphExNFCCcid_sUsb_MifareInfo.bMFCKeyLength = pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC];
    phUser_MemCpy(&gphExNFCCcid_sUsb_MifareInfo.aMFCKey[0], &pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 1], pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC]);

    gphExNFCCcid_sUsb_MifareInfo.bAuth = 0;

    /** Send the Success APDU response. */
    phExNFCCcid_UsbCcid_PCSC_Send_APDU (0x90, 0x00, 0x00);
    return;
}

/**
 * @brief Function to perform the General Authentication
 */
static void phExNFCCcid_PCSC_Std_Ext_Cmd_Auth(void)
{
    uint8_t * pbInputBuffer = NULL;

    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /* check length */
    if ((phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < 10) ||
        (5 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC]))
    {
        /* return error: Wrong length */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x67, 0x00, 0x00);
        return;
    }

    /* we only support MFC 1k (0x08) and MFC 4K (0x18) */
    if ((gphExNFCCcid_sUsb_SlotInfo.bSlotType == PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO) &&
        (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC))
    {
        /* version byte should be 0x01*/
        if (0x01 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 1])
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x83, 0);
            return;
        }

        /* key number should be 0x00*/
        if (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 5])
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x88, 0);
            return;
        }

        /* only 0x60 (auth key a) and 0x61 (auth key b)*/
        if (0x60 == pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 4])
        {
            gphExNFCCcid_sUsb_MifareInfo.bKeyType = 0x0A; //PHAL_MFC_KEYA;
        }
        else if (0x61 == pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 4])
        {
            gphExNFCCcid_sUsb_MifareInfo.bKeyType = 0x0B; //PHAL_MFC_KEYB;
        }
        else
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x86, 0);
            return;
        }

        /* address MSB should be 0x00*/
        if (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 2])
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x65, 0x81, 0);
            return;
        }

        /* classic 1k has 64 blocks */
        if ((pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_LC + 3] > 64))
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x65, 0x81, 0);
            return;
        }

        phExNFCCcid_UsbCcid_UsbPostEventCLTask(PH_EXNFCCCID_USBCCID_CL_AUTH_CMD);
        return;

    }

    /* not supported on this type of card */
    gphExNFCCcid_sUsb_MifareInfo.bAuth = 0;

    phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x83, 0);
    return;
}

/**
 * @brief Function to Read the Data from the Card
 */
static void phExNFCCcid_PCSC_Std_Ext_Cmd(uint8_t bReadOrWriteBin)
{
    uint8_t * pbInputBuffer = NULL;

    /** Get the Input Payload Buffer. */
    pbInputBuffer = phExNFCCcid_UsbCcid_Get_Input_Payload_Buffer();

    /* check length */
    if (phExNFCCcid_UsbCcid_Get_Input_Payload_Length() < 5)
    {
        /* return error: Wrong length */
        phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x67, 0x00, 0x00);
        return;
    }

    /** We only support Ultralight (0x00), MFC 1k (0x08)and MFC 4K (0x18). */
    if ((gphExNFCCcid_sUsb_SlotInfo.bSlotType == PH_EXNFCCCID_USBCCID_CL_CHANNEL_NO) &&
        ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC) ||
        (gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT)))
    {

        /* Check for the Mifare Classic Card */
        if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType != PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT) &&
            (0 == gphExNFCCcid_sUsb_MifareInfo.bAuth))
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x69, 0x82, 0);
            return;
        }

        /* P1 (length MSB) should be 0*/
        if (0x00 != pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P1])
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
            return;
        }

        /* Ultralight has 48 pages */
        if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFAREULTRALIGHT) &&
            ( pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2] > 0xFE /*48*/))
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
            return;
        }

        if ((gphExNFCCcid_sUsb_SlotInfo.bCLSlotType == PH_EXNFCCCID_USBCCID_CL_MIFARECLASSIC) &&
            (pbInputBuffer[PH_EXNFCCCID_USBCCID_PN7462AU_APDU_P2] > 64))
        {
            phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x82, 0);
            return;
        }

        if (PH_EXNFCCCID_PCSC_STD_EXT_READ_BIN == bReadOrWriteBin)
        {
            /** Post the Read Event to the CL Task. */
            phExNFCCcid_UsbCcid_UsbPostEventCLTask(PH_EXNFCCCID_USBCCID_CL_READ_CARD_CMD);
            return;
        }
        else if (PH_EXNFCCCID_PCSC_STD_EXT_WRITE_BIN == bReadOrWriteBin)
        {
            /** Post the Write Event to the CL Task. */
            phExNFCCcid_UsbCcid_UsbPostEventCLTask(PH_EXNFCCCID_USBCCID_CL_WRITE_CARD_CMD);
            return;
        }
        else
        {
            /* Do Nothing and Return Not Supported */
        }
    }

    /* card not supported */
    phExNFCCcid_UsbCcid_PCSC_Send_APDU(0x6A, 0x81, 0);
    return;
}

/**
 * @brief User Defined Control(Escape) Command function to start the polling loop
 * @param bEscape
 */
static void phExNFCCcid_PCSC_Start_Polling(uint8_t bEscape)
{
    /* If Command from Escape Request
     *  Start the Polling
     */
    if (bEscape)
    {
        phhalTimer_Start(gpphExNFCCcid_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
    }
}

/**
 * @brief User Defined Control(Escape) Command Function to stop the polling loop
 * @param bEscape
 */
static void phExNFCCcid_PCSC_Stop_Polling(uint8_t bEscape)
{
    if (bEscape)
    {
        /** Stop the Polling loop. */
        phhalTimer_Stop(gpphExNFCCcid_Clif_PollTimer);

        /** Stop the Polling Led Timer. */
        phhalTimer_Stop(pLedTimer);
    }
}

static void phExNFCCcid_PCSC_LED_Control(uint8_t bApduIns, uint8_t bEscape)
{
    phExNFCCcid_PCSC_Stop_Polling(bEscape);

    switch(bApduIns)
    {
    case PH_EXNFCCCID_USBCCID_ESCAPE_BLUE_LED_ON:
        /* Set the Blue Led */
        phExNFCCcid_LED_Status(BLUE_LED, LED_ON);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_GREEN_LED_ON:
        /* Set the Green Led */
        phExNFCCcid_LED_Status(GREEN_LED, LED_ON);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_YELLOW_LED_ON:
        /* Set the Yellow Led */
        phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_RED_LED_ON:
        /* Set the Red Led */
        phExNFCCcid_LED_Status(RED_LED, LED_ON);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_BLUE_LED_OFF:
        /* OFF the Blue Led */
        phExNFCCcid_LED_Status(BLUE_LED, LED_OFF);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_GREEN_LED_OFF:
        /* OFF the Green Led */
        phExNFCCcid_LED_Status(GREEN_LED, LED_OFF);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_YELLOW_LED_OFF:
        /* OFF the Yellow Led */
        phExNFCCcid_LED_Status(YELLOW_LED, LED_OFF);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_RED_LED_OFF:
        /* OFF the Red Led */
        phExNFCCcid_LED_Status(RED_LED, LED_OFF);
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_ALL_LED_OFF:
        /* Switch OFF all the LED's */
        phExNFCCcid_All_LED_Off();
        break;
    case PH_EXNFCCCID_USBCCID_ESCAPE_ALL_LED_ON:
        /* Switch ON all the LED's */
        phExNFCCcid_All_LED_On();
        break;
    }
}
