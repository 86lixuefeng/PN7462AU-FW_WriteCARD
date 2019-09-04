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
 * phExCcid_Ct.c:  Implements the CT task and the logical operations related to contact interface.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-08-12 14:11:13 +0530 (Fri, 12 Aug 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 3810 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "portmacro.h"
#include "phExCcid_Ct.h"
#include "phFlashBoot_Event.h"
#include "phUser.h"
#include "phExCcid_Cfg.h"
#include "ph_Log.h"
#include "phCfg_EE.h"
#include "phExCcid_LED.h"
#include "phExCcid.h"
#include "phhalTimer.h"
#include "phhalPmu.h"
#include "phExCcid_Clif.h"
#include "phExCcid_Usb_If.h"
#include "phExCcid_UsbCcid.h"
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
extern phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phpalCt_TransceiveOption_t phExCcid_Ct_GetTransceiveParam (void);

static void phExCcid_Ct_GetCtFrameParam(phpalCt_TransceiveOption_t eTransceiveOption,
                                        phStatus_t *pwTransceiveStatus,
                                        uint8_t *pbRdrToPcChainingParam,
                                        uint8_t *pbRdrToPcStatusParam);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * This is an example application running on PN7462AU on JCOP card with PPSE Applet installed
 * @return
 */
phStatus_t phExCcid_Ct_Example_App(void)
{
    uint16_t wOutLen;
    phStatus_t eStatus = PH_ERR_FAILED;
    /* Global instance for  CT pal parameter's structure */
    phpalCt_DataParams_t tDataParams ;
    uint8_t gbAtrBuffer[33]={0};
    volatile uint32_t dwBits = 0;
    uint32_t dwEventTimeOut = 0x20;
    uint8_t bApduRespBuffer[256];
    uint8_t bRetryCount = 0;
    /* Local Variables to store Chaining parameter and Status parameter,
     * that will be sent in response RDR_to_PC_DataBlock Message
     */
    uint8_t bRdrToPcChainingParam;
    uint8_t bRdrToPcStatusParam;
    /* PC_to_RDR_XfrBlock Message mode of operation for CT PAL Transceive */
    phpalCt_TransceiveOption_t eOption;

    do
    {
    	/* Stop the polling timer */
		phhalTimer_Stop(gpphExCcid_Clif_PollTimer);
		phhalTimer_Stop(pLedTimer);

        tDataParams.pbAtrBuffer = (uint8_t *)&gbAtrBuffer;
        tDataParams.bSizeOfATRbuffer = 33;

        gphExCcid_sUsb_SlotInfo.bAtrSize  = 0;
        gphExCcid_sUsb_SlotInfo.bAtrValid = false;

        if (PH_EXCCID_USBCCID_CHANNEL_NONE != gphExCcid_sUsb_SlotInfo.bSlotType)
           break;

        eStatus = phpalCt_ActivateCard(&tDataParams);
        if((PH_ERR_SUCCESS == eStatus) || (PH_ERR_CT_EARLY_ERROR | PH_COMP_HAL_CT) == eStatus)
        {
        	/*
             * Based on Protocols supported by Card, Configure PAL Layer
             */
            if(E_PHPAL_CT_T1 == tDataParams.bCardSupportedProt)
            {
                phpalCt_SetProtocol(E_PHPAL_CT_T1);
            }
            else if(E_PHPAL_CT_T0 == tDataParams.bCardSupportedProt)
            {
                phpalCt_SetProtocol(E_PHPAL_CT_T0);
            }
            else
            {
                /* Only for QA warnings */
            }

            /* Get the ATR Size and Whether Valid ATR */
            if (tDataParams.bSizeOfATRbuffer <= PH_EXCCID_USBCCID_MAX_ATR_LEN)
            {
                /*Copy ATR bytes to global ATR buffer*/
                phUser_MemCpy(&gphExCcid_sUsb_SlotInfo.aAtr ,&gbAtrBuffer,tDataParams.bAtrReceivedLength);
            }
            else
            {
                PH_USER_ASSERT(0);
            }

            /* Assign the Slot type as CT */
            gphExCcid_sUsb_SlotInfo.bSlotType     = PH_EXCCID_USBCCID_CT_CHANNEL_NO;

            /* Assign the ATR Received Length */
            gphExCcid_sUsb_SlotInfo.bAtrSize      = tDataParams.bAtrReceivedLength;
            gphExCcid_sUsb_SlotInfo.bAtrValid     = true;

            /* Assign the Card Supported Protocol */
            gphExCcid_sUsb_SlotInfo.bProtocolType = tDataParams.bCardSupportedProt;

            /* Send the Information to host about Card Presence */
            phExCcid_UsbCcid_CardInserted();

            while (true)
            {
            	eStatus = phhalCt_CheckCardPres();

            	if (eStatus == (PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT))
            	{
                    gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                    /* Deactivate the CT */
                    phhalCt_CardDeactivate();
                    eStatus = PH_ERR_SUCCESS;
                    break;
            	}

#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
                /** If Suspend is initiated by the host
                 *  Exit the loop and send the card removal information
                 */
                if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
                {
                    gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                    phhalCt_CardDeactivate();
                    break;
                }
#endif

                /* If the Slot Type is none break from the processing loop */
                if ((gphExCcid_sUsb_SlotInfo.bSlotType == PH_EXCCID_USBCCID_CHANNEL_NONE))
                {
                   break;
                }

                dwBits = phRtos_EventGroupWaitBits(gphExCcid_sUsb_EventInfo.xCT_Events,
                                                   PH_EXCCID_USBCCID_CT_TRNSP_EX_CMD |
                                                   PH_EXCCID_USBCCID_CT_DEACTIVATE_CARD_CMD |
                                                   PH_EXCCID_USBCCID_CT_ABORT_CMD,
                                                   true,          /* status bits should be cleared before returning. */
                                                   false,         /* wait for any status bit being set. */
                                                   dwEventTimeOut);

                /* Check for the Deactivate Card Command */
                if ((dwBits & (uint32_t) PH_EXCCID_USBCCID_CT_DEACTIVATE_CARD_CMD))
                {
                        /* Just send the success response if no exchange is performed
                         * No Deactivation of the Card
                         */
                        phExCcid_UsbCcid_Set_Output_Payload_Length(0);
                        phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS, 0x00, 0x00, 0x00);
                }

                /* Check for the Transparent Exchange Command */
                if (dwBits & (uint32_t)PH_EXCCID_USBCCID_CT_TRNSP_EX_CMD )
                {
                	eOption = phExCcid_Ct_GetTransceiveParam();
                    /* Perform the Transparent Exchange */
                    eStatus = phpalCt_Transceive_SplitChaining(phExCcid_UsbCcid_Get_Input_Payload_Buffer(),
                                                 phExCcid_UsbCcid_Get_Input_Payload_Length(),
                                                 bApduRespBuffer,
                                                 &wOutLen,
                                                 eOption);

                    gphExCcid_sUsb_Status.bProcess_Pend = 0;

                    /*
                     * Based on CT Transceive Mode option and status decide on the chaining and status parameters to
                     * be send back to the host PC
                     */
                    phExCcid_Ct_GetCtFrameParam(eOption, &eStatus, &bRdrToPcChainingParam, &bRdrToPcStatusParam);

                    if (eStatus == PH_ERR_SUCCESS)
                    {
                    	/*APDU exchange success, send response to host*/
                    	phUser_MemCpy(phExCcid_UsbCcid_Get_Output_Payload_Buffer(), bApduRespBuffer, (uint32_t)wOutLen);
                    	phExCcid_UsbCcid_Set_Output_Payload_Length((uint32_t)wOutLen);
                    	phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, bRdrToPcStatusParam, 0x00, bRdrToPcChainingParam);
                    	phExCcid_LED_TxnPass(gphExCcid_sUsb_SlotInfo.bSlotType);
                    }
                    else
                    {
                    	wOutLen = 0;
                    	phExCcid_UsbCcid_Set_Output_Payload_Length((uint32_t)wOutLen);
                    	phExCcid_UsbCcid_Send_Frame(PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK, 0x42, PH_EXCCID_USBCCID_CCID_ERROR_SLOT_ICC_MUTE, 0x00);
                    	gphExCcid_sUsb_SlotInfo.bAtrValid = 0;
                    }
                }
            }
            break;
        }
        /* Retry to activate the Card 3 times after failure */
		else
		{
			phhalCt_CardDeactivate();
			(void)phUser_Wait(10000);
			bRetryCount++;
			if (bRetryCount > 3)
			{
				break;
			}
		}
    }while((PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) == phhalCt_CheckCardPres());
    phExCcid_UsbCcid_CardRemoved();
    return eStatus;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static phpalCt_TransceiveOption_t phExCcid_Ct_GetTransceiveParam (void)
{
    phpalCt_TransceiveOption_t eOption = E_PHPAL_CT_TXRX_DEFAULT;

    switch(gphExCcid_sUsb_BulkOutMsg.aRxBuff[PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_2])
    {
        case PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_00:
        {
            /* Command APDU begins and ends with this command */
            eOption = E_PHPAL_CT_TXRX_DEFAULT;
        }
        break;

        case PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_01:
        {
            /* Command APDU begins with this command, and continue in the next PC_to_RDR_XfrBlock */
            eOption = E_PHPAL_CT_TX_CHAINING;
        }
        break;

        case PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_02:
        {
            /* abData field continues a command APDU and ends the APDU command */
            eOption = E_PHPAL_CT_TXRX_DEFAULT;
        }
        break;

        case PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_03:
        {
            /* abData field continues a command APDU and another block is to follow */
            eOption = E_PHPAL_CT_TX_CHAINING;
        }
        break;

        case PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_10:
        {
            /* empty abData field, continuation of response APDU is expected in the next RDR_to_PC_DataBlock */
            eOption = E_PHPAL_CT_RX_CHAINING;
        }
        break;

        default:
        break;
    }

    return eOption;
}

static void phExCcid_Ct_GetCtFrameParam(phpalCt_TransceiveOption_t eTransceiveOption,
                                        phStatus_t *pwTransceiveStatus,
                                        uint8_t *pbRdrToPcChainingParam,
                                        uint8_t *pbRdrToPcStatusParam)
{
    uint16_t wTransceiveStatus = *pwTransceiveStatus;
    do
    {
        /* By default set status as PASS */
        *pbRdrToPcStatusParam = 0x0;

        if ((eTransceiveOption != E_PHPAL_CT_RX_CHAINING) && (wTransceiveStatus == PH_ERR_SUCCESS))
        {
            /* the response APDU begins and ends in this command */
            *pbRdrToPcChainingParam = 0x00;
            *pwTransceiveStatus = PH_ERR_SUCCESS;
            break;
        }

        if ((eTransceiveOption == E_PHPAL_CT_RX_CHAINING) && (wTransceiveStatus == PH_ERR_SUCCESS))
        {
            /* abData field continues the response APDU and ends the response APDU */
            *pbRdrToPcChainingParam = 0x02;
            *pwTransceiveStatus = PH_ERR_SUCCESS;
            break;
        }

        if ((eTransceiveOption != E_PHPAL_CT_RX_CHAINING) && (wTransceiveStatus == PH_ADD_COMPCODE(PH_ERR_CT_PAL_SUCCESS_RX_CHAINING,PH_COMP_PAL_CT)))
        {
            /* the response APDU begins with this command and is to continue */
            *pbRdrToPcChainingParam = 0x01;
            *pwTransceiveStatus = PH_ERR_SUCCESS;
            break;
        }

        if ((eTransceiveOption == E_PHPAL_CT_RX_CHAINING) && (wTransceiveStatus == PH_ADD_COMPCODE(PH_ERR_CT_PAL_SUCCESS_RX_CHAINING,PH_COMP_PAL_CT)))
        {
            /* abData field continues the response APDU and another block is to follow */
            *pbRdrToPcChainingParam = 0x03;
            *pwTransceiveStatus = PH_ERR_SUCCESS;
            break;
        }

        if (wTransceiveStatus == PH_ADD_COMPCODE(PH_ERR_CT_PAL_SUCCESS_TX_CHAINING,PH_COMP_PAL_CT))
        {
            /* empty abData field, continuation of the command APDU is expected in next PC_to_RDR_XfrBlock command */
            *pbRdrToPcChainingParam = 0x10;
            *pwTransceiveStatus = PH_ERR_SUCCESS;
            break;
        }

        /* Some Error has occured */ /* TODO: What should be done here?*/
        *pbRdrToPcChainingParam = 0x00;
        *pbRdrToPcStatusParam = 0x00;

    }while(0);

    return;
}
