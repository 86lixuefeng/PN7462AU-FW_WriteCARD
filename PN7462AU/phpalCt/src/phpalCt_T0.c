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
 * This file contains the implementation for T=0 protocol.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-29 10:37:36 +0530 (Fri, 29 Jan 2016) $
 * $Author: Shikha Khandelwal (nxp82684) $
 * $Revision: 17616 $ (v04.13.00)
 *
 */



/* *****************************************************************************************************************
 * Includes
 * ******************************************************************************************************************/

#include <ph_Datatypes.h>
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHPAL_CT
#include <ph_Status.h>
#include "phhalCt.h"
#include "ph_Datatypes.h"
#include "phpalCt.h"
#include "phpalCt_T0.h"
#include "phUser.h"
#include "ph_Reg.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * **************************************************************************************************************** */

/************************ 7816-3 T=0 CASES ******************************************* */
#define PHPAL_CT_CASE_1S                 0
#define PHPAL_CT_CASE_2S                 1
#define PHPAL_CT_CASE_3S                 2
#define PHPAL_CT_CASE_4S                 3
#define PHPAL_CT_CASE_2E                 4
#define PHPAL_CT_CASE_3E                 5
#define PHPAL_CT_CASE_4E                 6

typedef enum {
    PH_ERR_CT_INS_NO_MATCH_ERROR = (PH_ERR_CUSTOM_BEGIN+0x0011)
}phpalCt_ErrorCodesT0_t;

/* *******************************************************************************************************************
 * Global Variables
 * ****************************************************************************************************************** */
/**
 * It is used for the class byte for framing of the APDU command during the reception of procedure byte to save command
 */
static uint8_t gClass;
/**
 * It is used for the Instruction byte for framing of the APDU command
 * during the reception of procedure byte to save command.
 */
static uint8_t gINS;
/**
 * It is used for the P1 byte for framing of the APDU command during the reception of procedure byte to save command
 */
static uint8_t gbP1;
/**
 * It is used for the P2 byte for framing of the APDU command during the reception of procedure byte to save command
 */
static uint8_t gbP2;
/**
 * It is used for the length byte for framing of the APDU command during
 * the reception of procedure byte to save command.
 */
static uint16_t gbLen;
/**
 * It is used for ISO 7816, in case we are getting 0x9000 for case 4s command in response for case 3s.
 * And we have to send get response command apdu for it
 */
static uint8_t gbGetResponseApdu;
/**
 * It is used for the T0 case detection
 */
static uint8_t gCase_APDU;

static uint8_t gbSW1;

static uint8_t gbSW2;
/**
 * It is used in short APDU exchange since in case of case1 command an extra
 * command byte should be added to make it a 5 byte header
 */
static uint8_t *gbBuffReference;

/* *******************************************************************************************************************
 * Private Functions Prototypes
 * ****************************************************************************************************************** */
/**
 * API used to Short APDU exchange with the card
 */
static phStatus_t phpalHwCt_ShortAPDU_Exchange(
         uint8_t* pbTransmitBuff,uint8_t* pbReceiveBuff,uint16_t* pwReceiveSize);

static phStatus_t phpalHwCt_Receive_ProcByte(uint8_t* bProcByte,uint16_t wCount, uint8_t* pbReceiveBuff);
static phStatus_t phpalHwCt_Case34_Send( uint8_t *pbTransmitBuff,uint8_t bProcByte,uint8_t* nb, uint8_t* bDataToSend);
static phStatus_t phpalHwCt_Case24_Receive(uint8_t bProcByte,uint16_t* wCount, uint8_t* pbReceiveBuff);
static phStatus_t phpalHwCt_OtherCases_Receive( uint8_t bProcByte,uint8_t* bDataToSend,
                                                uint8_t* bWarning, uint8_t* pbReceiveBuff, uint16_t bCount);
/**
 * API used to receive the response from the card
 */
static phStatus_t phpalHwCt_RcvCardData(uint8_t *pbAPDU_ExchBuffer,uint16_t nb,uint16_t ptr);
/**
 * API used to send the command to the card
 */
static phStatus_t phpalHwCt_SendCardData( uint8_t *pbAPDU_ExchBuffer,uint16_t nb,uint16_t ptr);

/* *******************************************************************************************************************
 * Public Functions
 * ****************************************************************************************************************** */
/**
 *@brief   phpalHwCt_TranscieveT0,This Function is used transmitting command/data to/from the card in T0 protocol
 *          and receive response.
 *@param   pbTransmitBuff : Pointer to the pbTransmitbuff passed by the upper layer
 *         dwTransmitSize  : Size of the pbTransmitBuff passed by the upper layer
 *         *pbReceiveBuff  : Pointer to the pbReceiveBuff passed by the upper layer for data reception
 *         *pwReceiveSize  : Pointer to the pwReceiveSize to copy the received data size
 *@return  PH_ERR_SUCCESS - PPS Exchange done successfully
 *         PH_ERR_CT_MAIN_CARD_ABSENT - If the card is absent in the main slot
 *         PH_ERR_INVALID_PARAMETER - If invalid parameter is passed
 *
 */

 phStatus_t phpalCt_T0_Transcieve( uint8_t* pbTransmitBuff, uint32_t dwTransmitSize,
                                  uint8_t* pbReceiveBuff,uint16_t* pwReceiveSize)
{
    uint16_t wCount=0;
    phStatus_t eStatus = PH_ERR_FAILED;
    do
    {
        eStatus = phhalCt_GetBufferPointer(&gbBuffReference);
        /* Short APDU Handling */
        /* CASE 1 : 4 bytes header */
        if(dwTransmitSize == 0x04)
        {
            gCase_APDU = PHPAL_CT_CASE_1S;
        }
        /* CASE 2 short: 5 bytes header */
        else if(dwTransmitSize == 0x05)
        {
            gCase_APDU = PHPAL_CT_CASE_2S;
        }
        /* CASE 3 and CASE 4 : */
        else if((dwTransmitSize > 0x05) && (pbTransmitBuff[4] != 0x00))
        {
            wCount = pbTransmitBuff[PHPAL_CT_P3];
            if(dwTransmitSize == (wCount + 0x05))                              /* pass the length*/
            {
                gCase_APDU = PHPAL_CT_CASE_3S;                           /*Case 3 short*/
            }
            else if(dwTransmitSize == (wCount + 0x06))                         /* 00 at the end of APDU */
            {
                gCase_APDU = PHPAL_CT_CASE_4S;                           /* then CASE 4 short */
            }
            else
            {
                eStatus = PH_ERR_INVALID_PARAMETER;
                break;
            }
        }
        /* Extended APDU Handling */
        else if((dwTransmitSize >= 0x07) && ( pbTransmitBuff[PHPAL_CT_P3] == 0))
        {
            /* Extended APDU support is not applicable*/
            eStatus = PH_ERR_CT_EXT_APDU_NOT_SUPPORTED;
            break;
        }
        else
        {
            eStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }

        eStatus = phpalHwCt_ShortAPDU_Exchange(pbTransmitBuff,pbReceiveBuff,pwReceiveSize);

    }while(0);

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PAL_CT);
}
 /* *****************************************************************************************************************
  * Private Functions Prototypes
  * ***************************************************************************************************************** */
/**
 *@brief   phpalHwCt_ShortAPDU_Exchange, This is Api used perform a the APDU exchange with card in Short APDU
 *         where max length is 255 bytes.
 *@param   uint8_t* pbTransmitBuff - Pointer to the transmit buffer
 *@param   uint32_t dwTransmitSize - Size of the transmit buffer
 *@param   uint8_t* pbReceiveBuff - Pointer to the receive buffer
 *@param   uint16_t* pwReceiveSize - Pointer to the address where receive size shall be populated
 *@return  PH_ERR_SUCCESS - Exchange done successfully
 *@return  PH_ERR_CT_RX_LENGTH_ERROR - Error in the received length of the response from the card
 *@return  PH_ERR_CT_TX_LENGTH_ERROR - Error in the Transmit length of the response to the card
 *@retval  PH_ERR_CT_INS_COMMAND_ERROR - Incorrect command passed from the upper layer
 */

static phStatus_t phpalHwCt_ShortAPDU_Exchange(uint8_t* pbTransmitBuff,
                                               uint8_t* pbReceiveBuff,  uint16_t* pwReceiveSize)
{
    uint16_t wCount = 0;
    uint8_t bProcByte = 0,nb = 0;
    uint8_t bCase_3_4 = 0;
    uint8_t bCase_2_4 = 0;
    uint8_t bWarning = 0;
    uint8_t bDataToSend = 0x01;
    uint8_t bTransmitBuffer[5];
    phStatus_t eStatus = PH_ERR_FAILED;

    (void) phUser_MemCpy(bTransmitBuffer,pbTransmitBuff,5);

    /* sending first command */
    gClass  = bTransmitBuffer[PHPAL_CT_CLASS];                                  /* command save */
    gINS    = bTransmitBuffer[PHPAL_CT_INS];
    gbP1    = bTransmitBuffer[PHPAL_CT_P1];
    gbP2    = bTransmitBuffer[PHPAL_CT_P2];
    gbSW1   = 0x00;
    gbSW2   = 0x00;

    if(gCase_APDU == PHPAL_CT_CASE_1S)
    {
        bTransmitBuffer[PHPAL_CT_P3] = 0x00;
        gbLen = 0x00;
    }
    else
    {
        gbLen = bTransmitBuffer[PHPAL_CT_P3];
    }

    /* Start the timer here just before sending the first Apdu */
    /* Set the timer in WWT mode */
    (void)phhalCt_SetTimer(PHHAL_CT_APDUMODE_WWT,0x00);

    eStatus = phpalHwCt_SendCardData(bTransmitBuffer,0x05,0x00);/* send command CLA INS P1 P2 L */
    PH_RETURN_ON_FAILURE(eStatus);
    /* *******************  reception loop  ********************** */
    while(1)                                                 /* main loop */
    {
        /* reception of the first byte*/
        while(1)                                             /* null byte case loop */
        {
            eStatus = phpalHwCt_Receive_ProcByte(&bProcByte, wCount, pbReceiveBuff);
            if((eStatus != PH_ERR_SUCCESS) && (eStatus != PH_ERR_CT_INS_NO_MATCH_ERROR))
            {
                return eStatus;
            }
            else
            {

               /* The correct procedure byte/ status byte has come exit the loop */
                PH_BREAK_ON_SUCCESS(eStatus);
                /* It is a NULL byte 60 then remain in same loop to receive procedure byte*/

            }
        }

        /**
         * ISO7816-3 Case4S.2 : For ISO 7816, In case 4s apdu if we get 0x90 proc byte for case 3s command.
         * We should continue with card by sending get response command.
         */
        if((!gphpalCt_BEmvEn)&&(bProcByte == 0x90)&&(gCase_APDU == PHPAL_CT_CASE_4S) && bCase_3_4)
        {
        	gbGetResponseApdu = 0x01;
        }
        bCase_3_4 = (gCase_APDU == PHPAL_CT_CASE_3S) || ((gCase_APDU == PHPAL_CT_CASE_4S) && bDataToSend);
        bCase_2_4 = (gCase_APDU == PHPAL_CT_CASE_2S) || ((gCase_APDU == PHPAL_CT_CASE_4S) && !bDataToSend);

        if(bCase_3_4)
        {
            eStatus = phpalHwCt_Case34_Send(pbTransmitBuff ,bProcByte,&nb,&bDataToSend);
        }
        else
        {
            if(bCase_2_4)
            {
                eStatus = phpalHwCt_Case24_Receive(bProcByte,&wCount,pbReceiveBuff);
            }
        }
        if((eStatus != PH_ERR_SUCCESS) && (eStatus != PH_ERR_CT_INS_NO_MATCH_ERROR))
        {
            return eStatus;
        }
        else
        {
            if(eStatus == PH_ERR_CT_INS_NO_MATCH_ERROR)
            {
                eStatus = phpalHwCt_OtherCases_Receive(bProcByte,&bDataToSend, &bWarning, pbReceiveBuff, wCount);
            }
        }
        if((eStatus != PH_ERR_SUCCESS) && (eStatus != PH_ERR_CT_INS_NO_MATCH_ERROR))
        {
            return eStatus;
        }
        if((eStatus == PH_ERR_CT_INS_NO_MATCH_ERROR)||(gCase_APDU == PHPAL_CT_CASE_1S))
        {
            if( (((bProcByte & 0xF0) == 0x60) || ((bProcByte & 0xF0) == 0x90)) )
            {
                eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,1+wCount); /* receive SW2 */
                PH_RETURN_ON_FAILURE(eStatus);
                if(bWarning)
                {
                    /* Case 4 with warnings */
                    pbReceiveBuff[wCount] = gbSW1;
                    pbReceiveBuff[wCount+1] = gbSW2;
                }
                *pwReceiveSize = wCount+2;
                PHHAL_CT_STOP_TIMER;
                return  PH_ERR_SUCCESS;
            }
            else
            {
                /* Error procedure byte */
                return PH_ERR_CT_INS_COMMAND_ERROR;
            }
        }
    }
}

static phStatus_t phpalHwCt_Receive_ProcByte(uint8_t* bProcByte,uint16_t wCount, uint8_t* pbReceiveBuff)
{
    phStatus_t eStatus = PH_ERR_CT_INS_NO_MATCH_ERROR;
    eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,wCount);
    PH_RETURN_ON_FAILURE(eStatus);

    *bProcByte = pbReceiveBuff[wCount];
    if(( (*bProcByte) != 0x60 ) &&
         !((gCase_APDU == PHPAL_CT_CASE_1S)
         && (((*bProcByte) == (0xFF-gINS)) || ((*bProcByte) == gINS)
         || ((*bProcByte) == (gINS+1)) || ((*bProcByte) == (gINS^0xFE)))))
    {
        return PH_ERR_SUCCESS;       /* SW1!=0x60 (NUL BYTE) */
    }
    return PH_ERR_CT_INS_NO_MATCH_ERROR;
}

static phStatus_t phpalHwCt_Case34_Send(uint8_t *pbTransmitBuff,uint8_t bProcByte,uint8_t* nb, uint8_t* bDataToSend)
{
    phStatus_t eStatus = PH_ERR_CT_INS_NO_MATCH_ERROR;
    do
    {
        /* ************************************************** */
        /* *         case 3 and case 4(data send)           * */
        /* ************************************************** */
        if((bProcByte == gINS) || (bProcByte == (gINS+1)))/* non asserted mode */
        {
            /* transmission of all remaining bytes  with programming state */
            eStatus = phpalHwCt_SendCardData(pbTransmitBuff,gbLen,5+(*nb));
            PH_BREAK_ON_FAILURE(eStatus);
            gbLen = 0x00;
            *bDataToSend = 0x00;
            eStatus = PH_ERR_SUCCESS;
        }
        else if((bProcByte == (0xFF-gINS)) || (bProcByte == (gINS^0xFE)))
        {
            /* transmission byte per byte */
            eStatus = phpalHwCt_SendCardData(pbTransmitBuff,1,5+(*nb));
            PH_BREAK_ON_FAILURE(eStatus);
            gbLen--;                                             /* data byte available number */
            (*nb)++;                                                /* number of data byte sent to the card */
            if (gbLen == 0x00)                                      /* end of transmission */
            {
                *bDataToSend = 0x00;
            }
            eStatus = PH_ERR_SUCCESS;
        }
        else
        {
            /* Only for quality QA */
        }
    }while(0);
    return eStatus;
}

static phStatus_t phpalHwCt_Case24_Receive(uint8_t bProcByte, uint16_t* wCount, uint8_t* pbReceiveBuff)
{
    phStatus_t eStatus = PH_ERR_CT_INS_NO_MATCH_ERROR;
    uint16_t wLocalCount = *wCount;
    uint8_t bGetResponseCmd[5] = {0x00,0xC0,0x00,0x00,0x00};
    uint8_t bTransmitBuffer[5];

    do
    {
        /* ************************************************** */
        /* *        case 2 and case 4(data to received)     * */
        /* ************************************************** */
        /** NOTE : '61XX' and '6CXX' procedure bytes are only used when processing case 2 and 4 commands */
        if((bProcByte == 0x61) && (gCase_APDU != PHPAL_CT_CASE_4S))     /* SW2= number of response bytes available */
        {
            /*  data byte available   */
            eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,wLocalCount+1);            /* receive SW2 */
            PH_BREAK_ON_FAILURE(eStatus);
            /* Form the GET RESPONSE instruction */
            bGetResponseCmd[PHPAL_CT_P3] = pbReceiveBuff[1+wLocalCount];          /* Licc = SW2 */

            if(!(gphpalCt_BEmvEn))
            {
                bGetResponseCmd[PHPAL_CT_CLASS] = gClass;
            }
            gClass = bGetResponseCmd[PHPAL_CT_CLASS];                            /* command save */
            gINS = bGetResponseCmd[PHPAL_CT_INS];
            gbP1 = bGetResponseCmd[PHPAL_CT_P1];
            gbP2 = bGetResponseCmd[PHPAL_CT_P2];
            gbLen = bGetResponseCmd[PHPAL_CT_P3];

            eStatus = phpalHwCt_SendCardData(bGetResponseCmd,5,0);   /* send get response command */
            PH_BREAK_ON_FAILURE(eStatus);

        }
        else if(bProcByte == 0x6C)                       /* SW2 = exact length Le */
        {
            /* data length not accepted  */
            eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,1+wLocalCount);            /* receive SW2 */
            PH_BREAK_ON_FAILURE(eStatus);

            gbLen = pbReceiveBuff[1];                              /* length equal to SW2 */
            bTransmitBuffer[0] = gClass;                            /* command to send */
            bTransmitBuffer[1] = gINS;
            bTransmitBuffer[2] = gbP1;
            bTransmitBuffer[3] = gbP2;
            bTransmitBuffer[4] = (uint8_t)gbLen;
            eStatus = phpalHwCt_SendCardData(bTransmitBuffer,5,0); /* send command CLA INS P1 P2 L */
            PH_BREAK_ON_FAILURE(eStatus);

        }
        else if(bProcByte == gINS)
        {
            /* reads all the data sent by the card */
            if (gbLen == 0x00)
            {
                gbLen = PHPAL_CT_MAX_LENGTH;
            }
            eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,gbLen,wLocalCount); /* read data send by the card */
            PH_BREAK_ON_FAILURE(eStatus);


            *wCount += gbLen;                                          /* length of the response (gINS + data ) */
            eStatus = PH_ERR_SUCCESS;
        }
        else if(bProcByte == (0xFF-gINS))
        {
            /*  reads byte by byte all the data sent by the card */
            eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,wLocalCount++);/*read data send by the card */
            PH_BREAK_ON_FAILURE(eStatus);

            gbLen -= 1;                                           /* decrement the length of the block */
            (*wCount)++;
            eStatus = PH_ERR_SUCCESS;
        }
        else
        {
            /* Only for quality QA */
        }
    }while(0);
    return eStatus;
}

static phStatus_t phpalHwCt_OtherCases_Receive(uint8_t bProcByte,uint8_t* bDataToSend,
                                               uint8_t* bWarning, uint8_t* pbReceiveBuff, uint16_t bCount)
{
    phStatus_t eStatus = PH_ERR_CT_INS_NO_MATCH_ERROR;
    uint8_t bGetResponseCmd[5] = {0x00,0xC0,0x00,0x00,0x00};
    do
    {
        /* On receipt of the warning status in step 3 */
    	/**
    	 * ISO7816-3 Case4S.4 : on reception for 0x9xxx other than 0x9000 in step 3, process should be aborted for case 4s apdu.
    	 * EMV 4.3 : on reception for 0x9xxx other than 0x9000 ,process will be continue considering warning status bytes.
    	 */
        if ((gCase_APDU==PHPAL_CT_CASE_4S) && (gINS != 0xC0 ) && (!(*bDataToSend)) &&
           (( bProcByte == 0x62 || bProcByte ==0x63 ||
           (((gphpalCt_BEmvEn)&&((bProcByte & 0xF0) == 0x90)) && (bProcByte!=0x90)))))
        {
			/* Case 4 with warnings so performs a get response */
			*bWarning = 1;
            eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,1);
            PH_BREAK_ON_FAILURE(eStatus);

            gbSW1 = bProcByte;
            gbSW2 = pbReceiveBuff[1];
            if(!(gphpalCt_BEmvEn))
            {
                bGetResponseCmd[PHPAL_CT_CLASS] = gClass;
            }

			eStatus = phpalHwCt_SendCardData(bGetResponseCmd,5,0);
			PH_BREAK_ON_FAILURE(eStatus);

        }
        /* ************************************************** */
        /* *         Specific cases                         * */
        /* *         -  Case 1 with 1 procedure byte before * */
        /* *            the status bytes                    * */
        /* *         -  Case 4 with ICC discarding the TTL  * */
        /* *            datas                               * */
        /* ************************************************** */
        else if( gbGetResponseApdu || ((gCase_APDU == PHPAL_CT_CASE_4S) && ((bProcByte == 0x61))) )
        {
        	gbGetResponseApdu = 0x00;

            /* perform the get response command and discard data to send */
            eStatus = phpalHwCt_RcvCardData(pbReceiveBuff,1,bCount+1);                         /* receive SW2 */
            PH_BREAK_ON_FAILURE(eStatus);

            *bDataToSend=0;                                                    /* discard bytes to transmit */

            bGetResponseCmd[PHPAL_CT_P3] = pbReceiveBuff[1+bCount];        /* Licc = SW2 */

            if(!(gphpalCt_BEmvEn))
            {
                bGetResponseCmd[PHPAL_CT_CLASS] = gClass;
            }
            eStatus = phpalHwCt_SendCardData(bGetResponseCmd,5,0); /*send get response command*/
            PH_BREAK_ON_FAILURE(eStatus);
        }
        else
        {
            /* Only for quality QA */
        }
    }while(0);
    /* command save */
    gClass = bGetResponseCmd[PHPAL_CT_CLASS];
    gINS   = bGetResponseCmd[PHPAL_CT_INS];
    gbP1   = bGetResponseCmd[PHPAL_CT_P1];
    gbP2   = bGetResponseCmd[PHPAL_CT_P2];
    gbLen  = bGetResponseCmd[PHPAL_CT_P3];

    return eStatus;
}
/**
 *@brief   phpalHwCt_RcvCardData, This Function used perform a reception operation from the card
 *@param  *pbAPDU_ExchBuffer : Pointer to the buffer passed for reception of data from card
 *         wDataLength : Count of bytes to be received from the card
 *         ptr : index to the buffer that is passed for the reception
 *@return  PH_ERR_SUCCESS - Receive done successfully
 *         - If the RX received is erroneous
 */
static phStatus_t phpalHwCt_RcvCardData(uint8_t *pbAPDU_ExchBuffer,uint16_t wDataLength,uint16_t ptr)
{
    phStatus_t eStatus = phhalCt_Receive(pbAPDU_ExchBuffer+ptr,wDataLength);

    if(((PH_COMP_HAL_CT|PH_ERR_CT_PARITY_ERROR)== eStatus)||((PH_COMP_HAL_CT|PH_ERR_CT_TIME_OUT_WWT_OR_BWT)== eStatus))
    {
        (void)phhalCt_CardDeactivate();
    }

    return eStatus;
}
/**
 *@brief   phpalHwCt_SendCardData, This is Function used perform a transmission operation to the card
 *
 *@param  *pbAPDU_ExchBuffer : Pointer to the buffer to be passed to the card
 *         wDataLength : Size of the data to be transmitted to the card
 *         ptr : index to the buffer that is passed for the transmission
 *@return  PH_ERR_SUCCESS - Transmit done successfully
 *          - If the RX received is erroneous
 *          - If the TX data/command is erroneous
 */
static phStatus_t phpalHwCt_SendCardData( uint8_t *pbAPDU_ExchBuffer,uint16_t wDataLength,uint16_t ptr)
{
    phStatus_t eStatus = phhalCt_Transmit(pbAPDU_ExchBuffer+ptr,wDataLength);
    return eStatus;
}
#endif /* NXPBUILD__PHPAL_CT */
