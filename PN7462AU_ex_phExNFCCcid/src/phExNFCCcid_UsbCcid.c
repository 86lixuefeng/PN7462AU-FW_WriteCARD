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
 * phExNFCCcid_UsbCcid.c: <This File handles the callback functions of the CCID class >
 *
 * Project: PN7462AU
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phUser.h"

#include "app_usbd_cfg.h"
#include "mw_usbd_hw.h"
#include "phExNFCCcid_Usb_If.h"
#include "phExNFCCcid_UsbCcid.h"

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
phExNFCCcid_Usb_BulkOut_Msg_t    gphExNFCCcid_sUsb_BulkOutMsg;
phExNFCCcid_Usb_BulkIn_Msg_t     gphExNFCCcid_sUsb_BulkInMsg;
phExNFCCcid_Usb_IntIn_Msg_t      gphExNFCCcid_sUsb_IntInMsg = {0};
phExNFCCcid_Usb_Comm_Out_t       gphExNFCCcid_sUsb_Comm_Out = {0};
phExNFCCcid_Usb_Comm_In_t        gphExNFCCcid_sUsb_Comm_In = {0};
phExNFCCcid_Usb_Comm_Status_t    gphExNFCCcid_sUsb_Status = {0};
phExNFCCcid_Slot_Info_t          gphExNFCCcid_sUsb_SlotInfo = {0};
phExNFCCcid_MiFare_Info_t        gphExNFCCcid_sUsb_MifareInfo = {0};
phExNFCCcid_Usb_Bus_Status_t     gphExNFCCcid_sUsb_Bus_Status = {0};
phExNFCCcid_Usb_Operation_Mode_t  gphExNFCCcid_sUsb_Operation_Mode = {0};

uint8_t gphExNFCCcid_bIntMsgValid = 0;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
/*
 * Function to process the incoming CCID and Payload
 */
static void phExNFCCcid_Command_Dispatch (void);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * @brief Callback function of the CCID class to handle the Bulk Out functionality
 */
void phExNFCCcid_UsbCcid_Usb_Bulk_Out(void)
{
    uint8_t  aErrorBuffer[PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH];

    if (gphExNFCCcid_sUsb_Bus_Status.bAddressed == 0)
        return;

    /* First We Check if we are already talked to => error */
    if (gphExNFCCcid_sUsb_Status.bIsBusy)
    {
        /* Read out the Header */
        hwUSB_ReadEP(UsbHandle, PH_EXNFCCCID_USBCCID_USB_BULK_OUT_EP, &aErrorBuffer[0]);

        /* Set the Status byte to error */
        aErrorBuffer[7] = PH_EXNFCCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED;

        /* Set the Error Code */
        aErrorBuffer[8] = PH_EXNFCCCID_USBCCID_CCID_ERROR_SLOT_BUSY;

        hwUSB_WriteEP(UsbHandle, PH_EXNFCCCID_USBCCID_USB_BULK_IN_EP, &aErrorBuffer[0], PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH);

        return;
    }

    /* Read the whole USB Message */
    gphExNFCCcid_sUsb_Comm_Out.dwLength = gphExNFCCcid_sUsb_Comm_Out.dwLength + (uint32_t)hwUSB_ReadEP (UsbHandle, PH_EXNFCCCID_USBCCID_USB_BULK_OUT_EP, (uint8_t *)&gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[gphExNFCCcid_sUsb_Comm_Out.dwIndex]);

    /* Call the Top half dispatch */
    phExNFCCcid_Command_Dispatch();

}

/**
 * @brief Callback function to handle the CCID class response back to the host request
 * @param what - Data buffer pointer to be send
 * @param how_much - Length of the bytes to be written
 * @return Number of bytes written(send) back to the host
 */
uint32_t phExNFCCcid_UsbCcid_Usb_Send(uint8_t bEP, uint8_t * what, uint32_t how_much)
{
    uint32_t l_count = 0;

    if (gphExNFCCcid_sUsb_Bus_Status.bAddressed == 0)
        return l_count;

    /* Check if we have more than MAX USB Packet Size data to send */
    if (how_much >= PN7462AU_USB_MAX_PACKET_SIZE)
    {
        /* At once we can send only Maximum of USB MAX Packet Size data */
        l_count = PN7462AU_USB_MAX_PACKET_SIZE;
    }
    else
    {
        /* Data length is less than maximum packet size */
        l_count = how_much;
    }
    /*
     * The Rest data will be send next time when the BULK IN is invoked in the USB Stack
     */
    return ((uint32_t) hwUSB_WriteEP(UsbHandle, bEP, &what[0], l_count));
}

/**
 * @brief Callback function of the CCID class Bulk IN Functionality
 */
void phExNFCCcid_UsbCcid_Usb_Bulk_In()
{
    /* Check for Busy State */
    if (gphExNFCCcid_sUsb_Status.bIsBusy)
    {
        /* We are busy, so let see if there is still some data to be send*/
        if (gphExNFCCcid_sUsb_Comm_In.dwLength > gphExNFCCcid_sUsb_Comm_In.dwWritenLength)
        {
            /* send MAX packet or the rest */
            gphExNFCCcid_sUsb_Comm_In.dwWritenLength = gphExNFCCcid_sUsb_Comm_In.dwWritenLength + phExNFCCcid_UsbCcid_Usb_Send(PH_EXNFCCCID_USBCCID_USB_BULK_IN_EP,
                                                                                      &gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[gphExNFCCcid_sUsb_Comm_In.dwWritenLength],
                                                                                      (gphExNFCCcid_sUsb_Comm_In.dwLength - gphExNFCCcid_sUsb_Comm_In.dwWritenLength));

            /* if this was the last package and its size was NOT PN7462AU_USB_MAX_PACKET_SIZE, *
             * then lets set send state machine to idle and clear out com stuff            */
            if ((gphExNFCCcid_sUsb_Comm_In.dwWritenLength == gphExNFCCcid_sUsb_Comm_In.dwLength) &&
                (0 != (gphExNFCCcid_sUsb_Comm_In.dwWritenLength % PN7462AU_USB_MAX_PACKET_SIZE)))
            {
                /* mark state machine idle */
                gphExNFCCcid_sUsb_Status.bIsBusy = 0;

                /* and clear out com stuff */
                gphExNFCCcid_sUsb_Comm_In.dwLength = 0;
                gphExNFCCcid_sUsb_Comm_In.dwWritenLength = 0;

                /* and then in com stuff */
                gphExNFCCcid_sUsb_Comm_Out.dwIndex = 0;
                gphExNFCCcid_sUsb_Comm_Out.dwLength = 0;
            }
        }
        else
        {
            /* no data to send but check if the (last) packet was of      *
             * PN7462AU_USB_MAX_PACKET_SIZE then we have to send empty packet */
            if (0 == (gphExNFCCcid_sUsb_Comm_In.dwWritenLength % PN7462AU_USB_MAX_PACKET_SIZE))
            {
                phExNFCCcid_UsbCcid_Usb_Send(PH_EXNFCCCID_USBCCID_USB_BULK_IN_EP, &gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[gphExNFCCcid_sUsb_Comm_In.dwWritenLength], 0);
            }

            /* mark state machine idle */
            gphExNFCCcid_sUsb_Status.bIsBusy = 0;

            /* and clear out com stuff */
            gphExNFCCcid_sUsb_Comm_In.dwLength = 0;
            gphExNFCCcid_sUsb_Comm_In.dwWritenLength = 0;

            /* and then in com stuff */
            gphExNFCCcid_sUsb_Comm_Out.dwIndex = 0;
            gphExNFCCcid_sUsb_Comm_Out.dwLength = 0;
        }

    }
}

/**
 * @brief Callback function to handle the Interrupt IN functionality of the CCID class
 */
void phExNFCCcid_UsbCcid_Usb_IntIn (void)
{
    /** Check if the Message for Interrupt endpoint is valid. */
    if (gphExNFCCcid_bIntMsgValid)
    {
        /** Send the message through Interrupt IN Endpoint. */
        phExNFCCcid_UsbCcid_Usb_Send(PH_EXNFCCCID_USBCCID_USB_INT_IN_EP, (uint8_t *)&gphExNFCCcid_sUsb_IntInMsg, sizeof(gphExNFCCcid_sUsb_IntInMsg));

        /** Reset the Message Valid flag. */
        gphExNFCCcid_bIntMsgValid = 0;
    }
}

/**
 * @brief Function to provide the information of card presence
 */
void phExNFCCcid_UsbCcid_CardInserted(void)
{
    /** Card Present in the Slot flag is set. */
    gphExNFCCcid_sUsb_SlotInfo.bCardPresent = PH_EXNFCCCID_USBCCID_CCID_CARD_PRESENT_IN_SLOT;

    /** Assign the Message Type as Slot Change Command(CCID). */
    gphExNFCCcid_sUsb_IntInMsg.bMsgType     = PH_EXNFCCCID_USBCCID_CCID_INT_IN_SLOT_CHANGE_CMD;

    /** Set the status as Card present. */
    gphExNFCCcid_sUsb_IntInMsg.bStatus      = 0x03;

    /* Set the Interrupt Message Valid flag */
    gphExNFCCcid_bIntMsgValid               = 1;

    /** Call the Interrupt Endpoint notification Function. */
    phExNFCCcid_UsbCcid_Usb_IntIn();
}

/**
 * @brief Function to provide the information of card removal
 */
void phExNFCCcid_UsbCcid_CardRemoved (void)
{
    /* Clear the CL Slot type Information */
    gphExNFCCcid_sUsb_SlotInfo.bCLSlotType  = 0x00;

    /* Clear the Slot type Information */
    gphExNFCCcid_sUsb_SlotInfo.bSlotType    = PH_EXNFCCCID_USBCCID_CHANNEL_NONE;

    /* Clear the Card presence flag */
    gphExNFCCcid_sUsb_SlotInfo.bCardPresent = PH_EXNFCCCID_USBCCID_CCID_CARD_NOT_PRESENT_IN_SLOT;

    /* Clear the Authentication flag for Mifare Card */
    gphExNFCCcid_sUsb_MifareInfo.bAuth      = 0;

    /* Assign the Message Type as Slot Change Command(CCID) */
    gphExNFCCcid_sUsb_IntInMsg.bMsgType     = PH_EXNFCCCID_USBCCID_CCID_INT_IN_SLOT_CHANGE_CMD;

    /* Set the Status as card Removed */
    gphExNFCCcid_sUsb_IntInMsg.bStatus      = 0x02;
    gphExNFCCcid_bIntMsgValid               = 1;

    /* Call the Interrupt Endpoint Notification Function */
    phExNFCCcid_UsbCcid_Usb_IntIn();

}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/**
 * @brief CCID Class Command Dispatcher Function
 */
static void phExNFCCcid_Command_Dispatch(void)
{
    uint32_t dwCCIDLength = 0;

    /* Check for the Valid Supported PC to RDR Requests of CCID Class */
    if ((gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_ICCPOWERON    ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_ICCPOWEROFF   ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_GETSLOTSTATUS ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_XFRBLOCK      ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_GETPARAMETERS ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_SETPARAMETERS ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_ESCAPE        ) &&
        (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE] != PH_EXNFCCCID_USBCCID_PC_TO_RDR_ICCCLOCK      )
       )
    {
        /* Send the Response as Command not Supported */
        phExNFCCcid_UsbCcid_Send_Frame (PH_EXNFCCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, PH_EXNFCCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED, PH_EXNFCCCID_USBCCID_CCID_ERROR_SLOT_CMD_NOT_SUPPORTED, 0x00);
        return;
    }

    /* Check if we have full header */
    if (gphExNFCCcid_sUsb_Comm_Out.dwLength < PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH)
    {
        /* we do not have the whole header - we must
         * wait for more data -> just set the index
         */
        gphExNFCCcid_sUsb_Comm_Out.dwIndex = gphExNFCCcid_sUsb_Comm_Out.dwLength;
        return;
    }

    /* It looks like we have a valid CCID header - lets extract length of the payload */
    dwCCIDLength = (uint32_t)                (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_4] << 24);
    dwCCIDLength = (uint32_t)(dwCCIDLength | (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_3] << 16));
    dwCCIDLength = (uint32_t)(dwCCIDLength | (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_2] << 8));
    dwCCIDLength = (uint32_t)(dwCCIDLength | (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_1]));

    if (gphExNFCCcid_sUsb_Comm_Out.dwLength != (dwCCIDLength + PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH))
    {
        /* we do not have the whole payload - we must
         * wait for more data -> just set the index
         */
        gphExNFCCcid_sUsb_Comm_Out.dwIndex = gphExNFCCcid_sUsb_Comm_Out.dwLength; //dwCCIDLength ;//
        return;
    }

    /*
     * Indicating that the USB is in process state
     * Cannot Receive further messages till processed
     */
    gphExNFCCcid_sUsb_Status.bIsBusy = 1;

    /* Check the Message Type from the CCID Header Information */
    switch (gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE])
    {
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_ICCPOWERON:
        phExNFCCcid_UsbCcid_Icc_Power_On ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_GETSLOTSTATUS:
        phExNFCCcid_UsbCcid_GetSlotStatus ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_ICCPOWEROFF:
        phExNFCCcid_UsbCcid_Icc_Power_Off ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_XFRBLOCK:
        phExNFCCcid_UsbCcid_XfrBlock ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_GETPARAMETERS:
        phExNFCCcid_UsbCcid_GetParameters ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_SETPARAMETERS:
        phExNFCCcid_UsbCcid_SetParameters ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_ESCAPE:
        phExNFCCcid_UsbCcid_Escape ();
        break;
    case PH_EXNFCCCID_USBCCID_PC_TO_RDR_ICCCLOCK:
        phExNFCCcid_UsbCcid_Icc_Clock ();
        break;
    default:
        phExNFCCcid_UsbCcid_Send_Frame (PH_EXNFCCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, PH_EXNFCCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED, PH_EXNFCCCID_USBCCID_CCID_ERROR_SLOT_CMD_NOT_SUPPORTED, 0x00);
        return;
    }
}

/**
 * @brief Function to handle the CCID response of the request back to the host
 * @param bMsgType - Response Message Type for the Request
 * @param bByte1   - CCID Response Message Header Byte 1 Information
 * @param bByte2   - CCID Response Message Header Byte 2 Information
 * @param bByte3   - CCID Response Message Header Byte 3 Information
 */
void phExNFCCcid_UsbCcid_Send_Frame (uint8_t bMsgType, uint8_t bByte1, uint8_t bByte2, uint8_t bByte3)
{
    uint32_t dwCCIDLength = 0;

    /* Assign the Response Message Type */
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE]  = bMsgType;

    /* Set CCID length */
    if (0 == gphExNFCCcid_sUsb_Comm_In.dwLength)
    {
        /* If the Length is 0 assign the CCID header(0xa) length */
        gphExNFCCcid_sUsb_Comm_In.dwLength = PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH;
    }
    else
    {
        dwCCIDLength = (uint32_t)(gphExNFCCcid_sUsb_Comm_In.dwLength - PH_EXNFCCCID_USBCCID_CCID_HEADER_LENGTH);
    }

    /* Assign the Length in the CCID Header Information */
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_1] = (uint8_t) (dwCCIDLength & 0x000000FF);
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_2] = (uint8_t)((dwCCIDLength & 0x0000FF00) >> 8);
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_3] = (uint8_t)((dwCCIDLength & 0x00FF0000) >> 16);
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_4] = (uint8_t)((dwCCIDLength & 0xFF000000) >> 24);

    /* Assign the Slot Information same as the Request received */
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_SLOT]    = gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_SLOT];

    /* Assign the Sequence Number same as the Request received */
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_SEQ]     = gphExNFCCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_SEQ];

    /* Assign the Byte Informations */
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_1] = bByte1;
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_2] = bByte2;
    gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[PH_EXNFCCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_3] = bByte3;

    /* Start sending data */
    gphExNFCCcid_sUsb_Comm_In.dwWritenLength = phExNFCCcid_UsbCcid_Usb_Send (PH_EXNFCCCID_USBCCID_USB_BULK_IN_EP, &gphExNFCCcid_sUsb_BulkInMsg.aTxBuff[0], gphExNFCCcid_sUsb_Comm_In.dwLength);
}

/**
 * @brief Function to Post the events for the CL task for processing
 * @param dwEvent - Event to be send
 */
void phExNFCCcid_UsbCcid_UsbPostEventCLTask(uint32_t dwEvent)
{
    phRtos_Status_t phRtos_Result;

    phRtos_Result = phRtos_EventGroupSetBits(gphExNFCCcid_sUsb_SlotInfo.xCL_Events,
                                             dwEvent);

    /*Assert if event posting fails*/
   if( phRtos_Result != phRtos_Success )
   {
      PH_USER_ASSERT(0);
   }
}

/**
 * @brief Function to Post the events for CT task for processing
 * @param dwEvent - Event to be send
 */
#if 0
void phExNFCCcid_UsbCcid_UsbPostEventCTTask(uint32_t dwEvent)
{
    phRtos_Status_t phRtos_Result;

    phRtos_Result = phRtos_EventGroupSetBits(gphExNFCCcid_sUsb_SlotInfo.xCT_Events,
                                             dwEvent);

    /*Assert if event posting failed*/
    if( phRtos_Result != phRtos_Success )
    {
       PH_USER_ASSERT(0);
    }
}
#endif
