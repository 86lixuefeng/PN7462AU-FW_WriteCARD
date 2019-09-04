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
 * phExCcid_UsbCcid_Process.c : <This File process the CCID Request Commands from Host PC>
 *
 * Project: PN7462AU
 * $Date: 2016-09-06 15:13:20 +0530 (Tue, 06 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 3843 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUser.h"
//#include "phRtos.h"
//#include "portmacro.h"
#include "phExCcid_UsbCcid.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
uint8_t pPPP[7];
extern uint8_t gMifareULC;
extern uint8_t gMifarePlusSL1;
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * @brief Function to handle the Request PC->RDR ICC Power ON
 */
void phExCcid_UsbCcid_Icc_Power_On ()
{
    uint32_t dwLength = 0;

    /** Get the ATR of the Card and its length. */
    dwLength = phExCcid_UsbCcid_Get_ATR();

    /* If the Length is not equal to 0 send the ATR along with the response */
    if (dwLength != 0)
    {
        phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x00, 0x00, 0x00);
    }
    else
    {
        phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);
        /* Send Error if the length is not 0 */
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED, PH_EXCCID_USBCCID_CCID_ERROR_SLOT_XFR_OVERRUN, 0x00);

    }
}

/**
 * @brief Function to handle the Request PC->RDR Slot Status
 */
void phExCcid_UsbCcid_GetSlotStatus ()
{
    uint32_t dwLength = 0;

    phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);

    /** Check if the slot is empty. */
    if (gphExCcid_sUsb_SlotInfo.bSlotType != PH_EXCCID_USBCCID_CHANNEL_NONE)
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x00, 0x00, 0x00);
    else
        /** Send ICC not present information to Host. */
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x02, 0x00, 0x00);

}

/**
 * @brief Function to handle the Request PC->RDR ICC Power OFF
 */
void phExCcid_UsbCcid_Icc_Power_Off ()
{
    /** Check for the Slot type. */
    if (gphExCcid_sUsb_SlotInfo.bSlotType == PH_EXCCID_USBCCID_CT_CHANNEL_NO)
     {
        /** Slot Type CT Post the deactivate card command. */
        phExCcid_UsbCcid_UsbPostEventCTTask(PH_EXCCID_USBCCID_CT_DEACTIVATE_CARD_CMD);
     }
     else if (gphExCcid_sUsb_SlotInfo.bSlotType == PH_EXCCID_USBCCID_CL_CHANNEL_NO)
     {
         /** Slot Type CL Post the Deactivate card command. */
         phExCcid_UsbCcid_UsbPostEventCLTask(PH_EXCCID_USBCCID_CL_DEACTIVATE_CARD_CMD);
     }
     else
     {
         /** Slot type is not CT or CL set output payload length as zero. */
         phExCcid_UsbCcid_Set_Output_Payload_Length(0);
         phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x00, 0x00, 0x00);

         /** Inform the host that card is removed. */
         phExCcid_UsbCcid_CardRemoved();
     }
}


/**
 * @brief Function to handle the Request Get Parameters
 */
void phExCcid_UsbCcid_GetParameters()
{
    uint8_t  bByte1;
    uint8_t  bByte2;
    uint8_t  bByte3;
    uint32_t dwLength;

    switch (gphExCcid_sUsb_SlotInfo.bSlotType)
    {
    /** Check the slots are CL/CT. */
    case PH_EXCCID_USBCCID_CT_CHANNEL_NO:
    case PH_EXCCID_USBCCID_CL_CHANNEL_NO:
         {
             switch (gphExCcid_sUsb_SlotInfo.bProtocolType)
             {
             case PH_EXCCID_USBCCID_PROTOCOL_T1:
             case PH_EXCCID_USBCCID_PROTOCOL_T0:
                 dwLength = (gphExCcid_sUsb_SlotInfo.bProtocolType ? PH_EXCCID_USBCCID_PROTOCOL_T1_LEN : PH_EXCCID_USBCCID_PROTOCOL_T0_LEN);
                 bByte1 = 0x00;
                 bByte2 = 0x00;
                 bByte3 = gphExCcid_sUsb_SlotInfo.bProtocolType;
                 /* Copy the Information to be send to the host */
                 phUser_MemCpy(&gphExCcid_sUsb_BulkInMsg.aTxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH], &pPPP[0], dwLength);
             break;
             default:
                 bByte1 = PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED;
                 bByte2 = PH_EXCCID_USBCCID_CCID_ERROR_SLOT_NOT_EXIST;
                 bByte3 = 0x00;
                 dwLength = 0;
             break;
             }
         }
    break;
    /** No Slot Exist. */
    default:
        bByte1 = PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED;
        bByte2 = PH_EXCCID_USBCCID_CCID_ERROR_SLOT_NOT_EXIST;
        bByte3 = 0x00;
        dwLength = 0;
    break;
    }

    phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);

    /** Send the response along with the protocol information. */
    phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_PARAMETERS, bByte1, bByte2, bByte3);
}

/**
 * @brief Function to handle the PC->RDR Set Parameter Request
 */
void phExCcid_UsbCcid_SetParameters ()
{
    uint32_t dwLength = 0;

    /** Check if the slot type is CT. */
    if (gphExCcid_sUsb_SlotInfo.bSlotType == PH_EXCCID_USBCCID_CT_CHANNEL_NO)
    {
        if (gphExCcid_sUsb_SlotInfo.bProtocolType == PH_EXCCID_USBCCID_PROTOCOL_T1)
        {
            dwLength = 7;

            /** Get the Information transmitted by the host for Protocol T1. */
            phUser_MemCpy(&pPPP[0], &gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH], dwLength);
        }
        else if (gphExCcid_sUsb_SlotInfo.bProtocolType == PH_EXCCID_USBCCID_PROTOCOL_T0)
        {
            dwLength = 5;

            /** Get the Information transmitted by the host for Protocol T0. */
            phUser_MemCpy(&pPPP[0], &gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH], dwLength);
        }
        else
        {
            dwLength = 0;

            phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);

            /** Send the response along with the protocol information. */
            phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_PARAMETERS, PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED, PH_EXCCID_USBCCID_CCID_ERROR_SLOT_NOT_EXIST, 0x00);

            return;
        }
    }
    /** Check if the slot type is CL. */
    else if (gphExCcid_sUsb_SlotInfo.bSlotType == PH_EXCCID_USBCCID_CL_CHANNEL_NO)
    {
        /** Check the CCID Header Message Byte 1.  */
        /** Message Byte_1 -> 1 means T1 protocol. */
        /** Message Byte_1 -> 0 means T0 protocol. */
        if (gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_1] == 0x01)
        {
            /** Assign the Protocol type as T1. */
            gphExCcid_sUsb_SlotInfo.bProtocolType = PH_EXCCID_USBCCID_PROTOCOL_T1;
            dwLength = 7;

            /** Get the Information transmitted by the host for Protocol T1. */
            phUser_MemCpy(&pPPP[0], &gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH], dwLength);
        }
        else if(gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_1] == 0x00)
        {
            /** Assign the Protocol type as T0. */
            gphExCcid_sUsb_SlotInfo.bProtocolType = PH_EXCCID_USBCCID_PROTOCOL_T0;
            dwLength = 5;

            /** Get the Information transmitted by the host for Protocol T0. */
            phUser_MemCpy(&pPPP[0], &gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH], dwLength);

        }
        else
        {
            dwLength = 0;

            phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);

            /** Send the response along with the protocol information. */
            phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_PARAMETERS, PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED, PH_EXCCID_USBCCID_CCID_ERROR_SLOT_NOT_EXIST, 0x00);

            return;
        }
    }
    else
    {
        /* For Qmore Purpose */
        dwLength = 0;

        phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);

        /** Send the response along with the protocol information. */
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_PARAMETERS, PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED, PH_EXCCID_USBCCID_CCID_ERROR_SLOT_NOT_EXIST, 0x00);

        return;
    }

    /* Send back the parameter Information got from the host back to the host */
    phExCcid_UsbCcid_GetParameters();
}

/**
 * @brief Function to handle the processing of APDU and Custom APDU commands
 */
void phExCcid_UsbCcid_XfrBlock ()
{
    /*
     * Check for the general APDU commands or user defined custom APDU commands
     * 0xA0 means Custom user defined APDU command
     * Other commands General APDU commands for exchange and control
     */
    if (gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH] != 0xA0)
    {
        /** Call the APDU decoding command. */
        phExCcid_UsbCcid_Decode_APDU();
    }
    else
    {
        /* Call the User Defined APDU command with parameter to distinguish
         *  between user defined APDU call and Escape (Control) command call
         */
        phExCcid_UsbCcid_Escape_Function(PH_EXCCID_USBCCID_ESCAPE_COMMAND_DISABLE);
    }
}

/**
 * @brief Function to handle the PC->RDR Escape Command Request
 */
void phExCcid_UsbCcid_Escape ()
{
    if (gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH] == 0xFF)
    {
        /* Change the Mode of the Reader */
        phExCcid_UsbCcid_Operation_Mode();
    }
    else
    {
        phExCcid_UsbCcid_Escape_Function(PH_EXCCID_USBCCID_ESCAPE_COMMAND_ENABLE);
    }
}

/**
 * @brief Function to handle the PC->RDR ICC Clock Information
 */
void phExCcid_UsbCcid_Icc_Clock ()
{
    uint32_t dwLength = 0;

    phExCcid_UsbCcid_Set_Output_Payload_Length(dwLength);

    /*
     *  Check CCID Header Message Byte 1
     *  1 - ICC Clock ON(0x03)
     *  0 - ICC Clock OFF(0x00)
     *  Ref: CCID Specification
     */
    if (gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_1] == 0x01)
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x01, 0x00, 0x03);
    else
        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x00, 0x00, 0x00);
}

/**
 * @brief Function to Set the Output Payload Length
 * @param dwPayloadLength - Payload length information
 */
void phExCcid_UsbCcid_Set_Output_Payload_Length(uint32_t dwPayloadLength)
{
    /** Total Length = CCID Header Length + Payload Length. */
    gphExCcid_sUsb_Comm_In.dwLength = PH_EXCCID_USBCCID_CCID_HEADER_LENGTH + dwPayloadLength;
}

/**
 * @brief Function to Get the Input Payload Length
 * @return Payload Length excluding the CCID header length
 */
uint32_t phExCcid_UsbCcid_Get_Input_Payload_Length(void)
{
    /* Return the Payload length */
    return ((uint32_t)(gphExCcid_sUsb_Comm_Out.dwLength - PH_EXCCID_USBCCID_CCID_HEADER_LENGTH));
}

/**
 * @brief Function to get the CCID header Message Byte Information
 * @param bByte
 * @return Address of the Packet containing the message byte information
 */
uint8_t phExCcid_UsbCcid_CCID_InHdr_Get_Byte(uint8_t bByte)
{
    if (bByte > PH_EXCCID_USBCCID_CCID_HEADER_LENGTH)
    {
        return (0xFF);
    }
    return (gphExCcid_sUsb_BulkOutMsg.aRxBuff[bByte]);
}

/**
 * @brief Function to get the output payload buffer address information
 * @return Address of the Output payload buffer
 */
uint8_t * phExCcid_UsbCcid_Get_Output_Payload_Buffer(void)
{
    return (&gphExCcid_sUsb_BulkInMsg.aTxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH]);
}

/**
 * @brief Function to get the Input payload buffer address information
 * @return Address of the Input payload buffer
 */
uint8_t * phExCcid_UsbCcid_Get_Input_Payload_Buffer(void)
{
    return (&gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH]);
}

/**
 * @brief Function to Get the ATR information of the Card when detected
 * @return Length of the ATR got from the card detected
 */
uint32_t phExCcid_UsbCcid_Get_ATR()
{
    uint32_t dwLength = 0;

    /** Check for the ATR valid flag set. */
    if(gphExCcid_sUsb_SlotInfo.bAtrValid)
    {
        /** Copy the ATR Information in the Transmission Buffer. */
        phUser_MemCpy(&gphExCcid_sUsb_BulkInMsg.aTxBuff[PH_EXCCID_USBCCID_CCID_HEADER_LENGTH], &gphExCcid_sUsb_SlotInfo.aAtr[0], gphExCcid_sUsb_SlotInfo.bAtrSize);
        dwLength = gphExCcid_sUsb_SlotInfo.bAtrSize;
    }

    /** Return the ATR length. */
    return (dwLength);

}

/**
 * @brief Function to handle the ATR for TypeB L4 Exchange Cards
 */
void phExCcid_UsbCcid_ATR_TypeBL4(uint8_t *pAtqb, uint8_t bMbli)
{
    uint16_t wIndex;

    gphExCcid_sUsb_SlotInfo.aAtr[0] = 0x3B;
    gphExCcid_sUsb_SlotInfo.aAtr[1] = 0x88;
    gphExCcid_sUsb_SlotInfo.aAtr[2] = 0x80;
    gphExCcid_sUsb_SlotInfo.aAtr[3] = 0x01;

    /* Application Data from ATQB */
    gphExCcid_sUsb_SlotInfo.aAtr[4] = pAtqb[5];
    gphExCcid_sUsb_SlotInfo.aAtr[5] = pAtqb[6];
    gphExCcid_sUsb_SlotInfo.aAtr[6] = pAtqb[7];
    gphExCcid_sUsb_SlotInfo.aAtr[7] = pAtqb[8];

    /* Protocol Info from ATQB */
    gphExCcid_sUsb_SlotInfo.aAtr[8] = pAtqb[9];
    gphExCcid_sUsb_SlotInfo.aAtr[9] = pAtqb[10];
    gphExCcid_sUsb_SlotInfo.aAtr[10] = pAtqb[11];

    /* MBLI */
    gphExCcid_sUsb_SlotInfo.aAtr[11] = (0x00 | (bMbli << 4)) ;
    gphExCcid_sUsb_SlotInfo.aAtr[12] = 0x00;

    for (wIndex = 1; wIndex < 12; wIndex++)
    {
        gphExCcid_sUsb_SlotInfo.aAtr[12] = (uint8_t)(gphExCcid_sUsb_SlotInfo.aAtr[12] ^ gphExCcid_sUsb_SlotInfo.aAtr[wIndex]);
    }

    /*update actual ATR length*/
    gphExCcid_sUsb_SlotInfo.bAtrSize = 13;
    gphExCcid_sUsb_SlotInfo.bAtrValid = 1;
}

/**
 * @brief Function to handle the ATR for Felica and Mifare Cards
 */
void phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(uint8_t bSak)
{
    uint16_t wIndex;

    /* Initial Header */
    gphExCcid_sUsb_SlotInfo.aAtr[0] = 0x3B;

    /* T0 */
    gphExCcid_sUsb_SlotInfo.aAtr[1] = 0x8F;

    /* TD1 */
    gphExCcid_sUsb_SlotInfo.aAtr[2] = 0x80;

    /* TD2 */
    gphExCcid_sUsb_SlotInfo.aAtr[3] = 0x01;

    /* T1 */
    gphExCcid_sUsb_SlotInfo.aAtr[4] = 0x80;

    /* Application Identifier Presence Indicator */
    gphExCcid_sUsb_SlotInfo.aAtr[5] = 0x4F;

    /* Length */
    gphExCcid_sUsb_SlotInfo.aAtr[6] = 0x0C;

    /* RID */
    gphExCcid_sUsb_SlotInfo.aAtr[7] = 0xA0;
    gphExCcid_sUsb_SlotInfo.aAtr[8] = 0x00;
    gphExCcid_sUsb_SlotInfo.aAtr[9] = 0x00;
    gphExCcid_sUsb_SlotInfo.aAtr[10] = 0x03;
    gphExCcid_sUsb_SlotInfo.aAtr[11] = 0x06;

    /* Card Name -C0 */
    gphExCcid_sUsb_SlotInfo.aAtr[13] = 0x00;

    if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_FELICA)
    {
        /* Standard - SS */
        gphExCcid_sUsb_SlotInfo.aAtr[12] = 0x11;

        /* Card Name - C1 */
        gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x3B;
    }
    else if(gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFARECLASSIC)
    {
        /* Standard - SS */
        gphExCcid_sUsb_SlotInfo.aAtr[12] = 0x03;

        if (bSak == 0x18)
        {
            if (gMifarePlusSL1 == 0x02)
               /* Card Name - C1 */
               gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x37;
            else
               gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x02;
        }
        else if (bSak == 0x08)
        {
            if (gMifarePlusSL1 == 0x01)
                gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x36;
            else
                gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x01;
        }
        else if (bSak == 0x09)
        {
        	gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x26;
        }
        else
        {
            /* Card Name - C1 */
            gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x00;
        }
    }
    else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT)
    {
        /* Standard - SS */
        gphExCcid_sUsb_SlotInfo.aAtr[12] = 0x03;

        if (gMifareULC == 0x1)
        {
            /* Card Name - C1 */
            gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x3A;
        }
        else
        {
            /* Card Name - C1 */
            gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x03;
        }
    }
    else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_TYPE_V)
    {
    	 /* Standard - SS */
    	gphExCcid_sUsb_SlotInfo.aAtr[12] = 0x0B;

    	/* Card Name - C1 */
    	gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x14;
    }
    else if (gphExCcid_sUsb_SlotInfo.bCLSlotType == PH_EXCCID_USBCCID_CL_18000P3M3)
    {
        /* Standard - SS */
        gphExCcid_sUsb_SlotInfo.aAtr[12] = 0xFF;

        /* Card Name - C1 */
        gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x23;
    }
    else
    {
        /* Standard - SS */
        gphExCcid_sUsb_SlotInfo.aAtr[12] = 0x03;

        if (bSak == 0x11)
        {
            /* Card Name -C0 */
            gphExCcid_sUsb_SlotInfo.aAtr[13] = 0x00;
            /* Card Name - C1 */
            gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x39;
        }
        else if (bSak == 0x10)
        {
            /* Card Name -C0 */
            gphExCcid_sUsb_SlotInfo.aAtr[13] = 0x00;
            /* Card Name - C1 */
            gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x38;
        }
        else
        {
            /* Card Name -C0 */
            gphExCcid_sUsb_SlotInfo.aAtr[13] = 0x00;
            /* Card Name - C1 */
            gphExCcid_sUsb_SlotInfo.aAtr[14] = 0x00;
        }
    }

    /* RFU */
    gphExCcid_sUsb_SlotInfo.aAtr[15] = 0x00;
    gphExCcid_sUsb_SlotInfo.aAtr[16] = 0x00;
    gphExCcid_sUsb_SlotInfo.aAtr[17] = 0x00;
    gphExCcid_sUsb_SlotInfo.aAtr[18] = 0x00;

    gphExCcid_sUsb_SlotInfo.aAtr[19]  = 0x00;

    for (wIndex = 1; wIndex < 19; wIndex++)
    {
        gphExCcid_sUsb_SlotInfo.aAtr[19] = (uint8_t)(gphExCcid_sUsb_SlotInfo.aAtr[19] ^ gphExCcid_sUsb_SlotInfo.aAtr[wIndex]);
    }

    /*update actual ATR length*/
    gphExCcid_sUsb_SlotInfo.bAtrSize  = 20;
    gphExCcid_sUsb_SlotInfo.bAtrValid = 1;
}

/**
 * @brief Function to get the ATR from the ATS information for the L4 Exchange cards
 */
void phExCcid_UsbCcid_ATR_FromAts(uint8_t *pAts)
{
    uint16_t wIndex;
    uint8_t bLength = 0;
    uint8_t bOffset = 0;
    uint8_t bAtsCopyLen = 0;

    /* Len + Format are always present */
    bOffset = 2;

    /* check for TA(1) */
    if (pAts[1] & 0x10)
    {
       bOffset++;
    }

    /* check for TB(1) */
    if (pAts[1] & 0x20)
    {
       bOffset++;
    }

    /* check for TC(1) */
    if (pAts[1] & 0x40)
    {
       bOffset++;
    }

    bAtsCopyLen = (pAts[0] - bOffset);
    bLength = (5 + bAtsCopyLen);
    gphExCcid_sUsb_SlotInfo.aAtr[0] = 0x3B;
    gphExCcid_sUsb_SlotInfo.aAtr[1] = (uint8_t)(0x80 | bAtsCopyLen);
    gphExCcid_sUsb_SlotInfo.aAtr[2] = 0x80;
    gphExCcid_sUsb_SlotInfo.aAtr[3] = 0x01;
    /*Copy ATS bytes to global ATR array*/
    phUser_MemCpy (&gphExCcid_sUsb_SlotInfo.aAtr[4], &pAts[bOffset], bAtsCopyLen);

    gphExCcid_sUsb_SlotInfo.aAtr[(0x04 + bAtsCopyLen)] = 0x00;
    /*copy ATR validation code*/
    for (wIndex = 1; wIndex < (0x04 + bAtsCopyLen); wIndex++)
    {
        gphExCcid_sUsb_SlotInfo.aAtr[(0x04 + bAtsCopyLen)] =
       (uint8_t)(gphExCcid_sUsb_SlotInfo.aAtr[(0x04 + bAtsCopyLen)] ^ gphExCcid_sUsb_SlotInfo.aAtr[wIndex]);
    }

    /*update actual ATR length*/
    gphExCcid_sUsb_SlotInfo.bAtrSize = bLength;
    gphExCcid_sUsb_SlotInfo.bAtrValid = 1;
}
