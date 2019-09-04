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
 * Implements CT HAL which directly interacts with CT IP registers.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-08-08 18:10:10 +0530 (Mon, 08 Aug 2016) $
 * $Author: Stevenson James (NXP66090) $
 * $Revision: 18579 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalCt.h"
#include "phhalCt_Int.h"
#include "phUser.h"
#include "ph_Status.h"
#include "ph_Platform.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "phhalCt_Event.h"
#include "ph_Reg.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/**
 * PPS request and response PPSS byte.
 */
#define PHHAL_CT_PPSS_BYTE                     0xFF

/**
 * Maximum fifo size in the CT IP.
 */
#define PHHAL_CT_MAX_FIFO_SIZE            32

/**
 * Maximum parity repetitions allowed.
 */
#define PHHAL_CT_MAX_RETRY_PARITY       4

/**
 *  Initial Threshold value for Fifo to get interrupt.
 */
#define PHHAL_CT_INITIAL_FIFO_THRESHOLD_VALUE  0x12

/**
 *  Fifo fill count for remaining bytes.
 */
#define PHHAL_CT_FIFO_FILL_COUNT   12

/**
 *  Maximum receive timeout value. This is calculated over default baud rate with maximum number of received bytes can
 *  be 254+4 = 258 bytes. 258 bytes in the interval of 10079 ETU(829 mili second).
 *  Hence the timeout value will be at least 258*1000(Mili Second)
 */
#define PHHAL_CT_MAX_RECEIVE_TIMEOUT   (258*1000)

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phhalCt_PPSExchangeHandling(uint8_t *pbPPSRequestBuffer,uint8_t *pbPPSResponseBuffer,
                                              phhalCt_PPSParamType_t  *psPPSParams);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalCt_Transmit(uint8_t *pbTransmitData, uint16_t wTransmitSize)
{

    uint8_t bLoopCount = 0x00;
    uint16_t  bCount = 0;
    uint16_t wRemainingBytes = 0x00;
    uint32_t dwExitFlag = 0x00;
    phStatus_t eStatus = PH_ERR_FAILED;

    gphhalCt_BTransmitComplete = 0x00;
    gphhalCt_WPendingBytes = 0x00;
    /* Clear the global data counter updated in ISR for reception of bytes.*/
    gphhalCt_WDataCount = 0x00;
    /* Clear the offset as well.*/
    gphhalCt_WReceiveOffset = 0x00;
    gphhalCt_BCWTFlag = 0x00;
    gphhalCt_BParityErr = 0x00;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the null pointer.*/
        if ((pbTransmitData == NULL) || (wTransmitSize == 0x00))
        {
            return PH_ERR(INVALID_PARAMETER,HAL_CT);
        }

#endif

        while(!(PH_REG_TEST_BIT(CT_MSR_REG, BGT)))
        {
            /* To ensure transmission should start, only after BGT timer elapsed.*/
        }

        /* Check if the card removal event is pending */
        eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_CARD_REMOVED),
                                          1,&gphhalCt_InEvent,FALSE);

        if(PH_ERR_SUCCESS == eStatus)
        {
            if((gphhalCt_InEvent & E_PH_HALCT_EVENT_CARD_REMOVED) == E_PH_HALCT_EVENT_CARD_REMOVED)
            {
                phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_CARD_REMOVED);
                eStatus = PH_ERR_CT_CARD_REMOVED;
                break;
            }
        }

        /* Save the last byte which will be filled in Fifo in ISR and LCT bit will be set */
        gphhalCt_BLastByteTransmit = pbTransmitData[wTransmitSize-1];

        /* Clear Events */
       (void) phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL));

        gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

        /* Flush the FIFO */
        PH_REG_SET_BIT(CT_UCR2x_REG,FIFO_FLUSH);

        /* Set the Mode to transmission.*/
        PH_REG_SET_BIT(CT_UCR1x_REG, T_R);

        /* Set the mode to byte access since we are transmitting one byte at a time.*/
        PH_REG_SET_FIELD(CT_UCR2x_REG, WRDACC, 0x00);

        /* Transmission algorithm starts here.*/
        wRemainingBytes = wTransmitSize;

        if(wRemainingBytes > PHHAL_CT_MAX_FIFO_SIZE)
        {
            /* Load first 32 bytes in Fifo.*/
            for(bLoopCount = 0x00; bLoopCount<PHHAL_CT_MAX_FIFO_SIZE; bLoopCount++)
            {
                PH_REG_SET(CT_UTR_URR_REG_adr1, pbTransmitData[bCount++]);
            }

            /* Set the fifo threshold to 20 in the transmission mode(default is 1)
             * so that whenever the Fifo will have 20 more bytes, we will get an interrupt.
             * Initial threshold is set to 20 because we will have enough time to load more bytes in fifo,
             * which will prevent more time gap in bytes during transmission.
             */
            PH_REG_SET_FIELD(CT_FCR_REG, FTC4_FTC0,PHHAL_CT_INITIAL_FIFO_THRESHOLD_VALUE);

            /* Wait for the transmission completed interrupt.*/
            eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_TX | E_PHHAL_CT_ERROR_EVENTS),
                                              1000,&gphhalCt_InEvent,FALSE);
            if(PH_ERR_SUCCESS != eStatus)
            {
                dwExitFlag = 0x01;
                break;
            }
            eStatus = phhalCt_HandleCommonEvent();
            if(PH_ERR_SUCCESS != eStatus)
            {
                dwExitFlag = 0x01;
                break;
            }
            /* Consume the event transmit */
            phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_TX);
            gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;
            /* Reduce the remaining bytes by 32 since already 32 bytes have been loaded to Fifo.*/
            wRemainingBytes -= PHHAL_CT_MAX_FIFO_SIZE;

            /* Load 12 bytes in Fifo one by one , because after 20 bytes are already in Fifo so we can fill only 12 bytes at once .*/
            while(wRemainingBytes > PHHAL_CT_FIFO_FILL_COUNT)
            {
                /* Disable the Fifo threshold interrupt */
                PH_REG_SET_BIT(CT_UCR2x_REG,DISFT);

                /* Load all 12 bytes in Fifo.*/
                for(bLoopCount = 0x00; bLoopCount<PHHAL_CT_FIFO_FILL_COUNT; bLoopCount++)
                {
                    PH_REG_SET(CT_UTR_URR_REG_adr1, pbTransmitData[bCount++]);
                }

                /* Enable the Fifo threshold interrupt again.*/
                PH_REG_CLEAR_BIT(CT_UCR2x_REG,DISFT);

                /* Wait for the transmission completed interrupt.*/
                eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_TX | E_PHHAL_CT_ERROR_EVENTS),
                                                  1000,&gphhalCt_InEvent,FALSE);
                if(PH_ERR_SUCCESS != eStatus)
                {
                    dwExitFlag = 0x01;
                    break;
                }
                eStatus = phhalCt_HandleCommonEvent();
                if(PH_ERR_SUCCESS != eStatus)
                {
                    dwExitFlag = 0x01;
                    break;
                }
                /* Consume the event transmit */
                phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_TX);
                gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;
                /* Reduce the remaining bytes by 12 since already 12 bytes have been loaded to Fifo.*/
                wRemainingBytes -= PHHAL_CT_FIFO_FILL_COUNT;

            }

        }
        if(0x01 == dwExitFlag)
        {
            break;
        }

        /* Set the fifo threshold to 0 in the transmission mode(default is 1)
         * so that whenever the Fifo is empty we will get an interrupt
         */
        PH_REG_SET_FIELD(CT_FCR_REG, FTC4_FTC0,0x00);

        /* Still bytes to be transmitted ? Load the Fifo with the remaining bytes.*/
        if(wRemainingBytes > 1)
        {
            /* Disable the Fifo threshold interrupt */
            PH_REG_SET_BIT(CT_UCR2x_REG,DISFT);

            while(wRemainingBytes-1)
            {
                PH_REG_SET(CT_UTR_URR_REG_adr1, pbTransmitData[bCount++]);
                wRemainingBytes--;
            }

            /* Enable the Fifo threshold interrupt again.*/
            PH_REG_CLEAR_BIT(CT_UCR2x_REG,DISFT);

            eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_TX | E_PHHAL_CT_ERROR_EVENTS),
                                                        1000,&gphhalCt_InEvent,FALSE);
            PH_BREAK_ON_FAILURE(eStatus);

            eStatus = phhalCt_HandleCommonEvent();
            PH_BREAK_ON_FAILURE(eStatus);

            /* Consume the event transmit */
            phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_TX);
            gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;
        }
        else
        {
            if(wRemainingBytes == 0x01)
            {
                /* Disable the Fifo threshold so that we will not get interrupt for this last byte */
                PH_REG_SET_BIT(CT_UCR2x_REG,DISFT);
                PH_REG_SET_BIT(CT_UCR1x_REG,LCT);
                PH_REG_SET(CT_UTR_URR_REG_adr1, gphhalCt_BLastByteTransmit);
            }
        }

        /* During the T=0 transmit if the last byte in the Fifo is naked by the card for more than
         * PEC retry counter this error is captured here and updated. */
        /* Looping for transmit to receive state.*/
        while(((PH_REG_TEST_BIT(CT_UCR1x_REG,T_R)) && (!gphhalCt_BParityErr)))
        {
            /* This is required to know the last byte is sent and fifo is empty.*/
        }
        /* Set the transmission completion flag.*/
        gphhalCt_BTransmitComplete = 0x01;

        /* Enable the Fifo threshold interrupt again.*/
        PH_REG_CLEAR_BIT(CT_UCR2x_REG,DISFT);

        if(gphhalCt_BParityErr == 0x01)
        {
            gphhalCt_BParityErr = 0x00;
            /* Consume the event transmit */
            phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_PARITY);
            eStatus = PH_ERR_CT_PARITY_ERROR;
            break;
        }

        /* Clear Events, to consume all events before Receive api.
         * It will help in RTOS where previous Receive api's last character CWT out event will occur in next receive api.
         */
       (void) phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL));
        gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

        eStatus = PH_ERR_SUCCESS;
    }while(0);

    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}

phStatus_t phhalCt_Receive(uint8_t *pbReceiveData, uint16_t wReceiveSize)
{
    volatile phStatus_t eStatus = PH_ERR_FAILED; /*artf555048 : Prevent Optimization. False BWT event is randomly raised
                                                   when using O2 Optimization */
    uint8_t  bBlockParity = 0x00;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if (pbReceiveData == NULL)
        {
            eStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }
#endif

        /* Enter Critical section */
        __phUser_EnterCriticalSection();
        while(PH_REG_GET(CT_FSR_REG))
        {
            gphhalCt_DriverBuff[gphhalCt_WDataCount++] = (uint8_t) PH_REG_GET(CT_UTR_URR_REG_adr1);
            gphhalCt_WPendingBytes++;

        }
        /* Exit Critical section */
        __phUser_ExitCriticalSection();

        /* Check if there are already requested bytes available in the buffer */
        if(gphhalCt_WPendingBytes >= wReceiveSize)
        {
            /* Decrease the pending bytes to be copied to the protocol layer */
            gphhalCt_WPendingBytes-=wReceiveSize;
            /* Check if there are any errors pending */
            eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PHHAL_CT_ERROR_EVENTS),
                                             1,&gphhalCt_InEvent,FALSE);
        }
        else
        {
            /* Critical section */
            __phUser_EnterCriticalSection();
            /* Assign the receive size to the global receive size which is checked in the ISR */
            gphhalCt_WReceiveSize = wReceiveSize;
            __phUser_ExitCriticalSection();

            /* artf187980: Workaround to avoid waiting for events if the Card is absent.
               Check if the card is present */
            eStatus = phhalCt_CheckCardPres();

            if(eStatus == PH_ERR(CT_MAIN_CARD_PRESENT,HAL_CT))
            {
                if (!PH_REG_TEST_BIT(CT_PCR_REG, START))
                {
                    /* Card is present but not activated. Break with Error Code */
                    eStatus = PH_ERR(CT_HAL_INVALID,HAL_CT);
                    break;
                }
            }
            else
            {
                /* Card is not present. Break with error code. */
                /* Consume pending Card Removal events, if any */
                (void)phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_CARD_REMOVED);
                break;
            }

            /* Wait for response bytes */
            /* Blocking until there is timeout or RX complete event is coming */
            /* 2 seconds timeout has been kept for the RTOS timer */
            if ((PH_REG_TEST_BIT(CT_UCR1x_REG, PROT)))
            {
                /* For T=1 the parity event should not be waited since the parity error should be handled in the
                 * protocol layer */
                eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)( E_PH_HALCT_EVENT_RX | E_PH_HALCT_EVENT_TO3 |
                                                 E_PH_HALCT_EVENT_FRM_ERR | E_PH_HALCT_EVENT_OVR_ERR |
                                                 E_PH_HALCT_EVENT_PTL_ERR | E_PH_HALCT_EVENT_PROTL_ERR |
                                                 E_PH_HALCT_EVENT_ASYNC | E_PH_HALCT_EVENT_CARD_REMOVED),
                                                 PHHAL_CT_MAX_RECEIVE_TIMEOUT,&gphhalCt_InEvent,FALSE);
            }
            else
            {
                /* For T=0 the parity event should be and deactivation to be performed */
                eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_RX |
                                                                      E_PH_HALCT_EVENT_TO3 |E_PHHAL_CT_ERROR_EVENTS),
                                                                      PHHAL_CT_MAX_RECEIVE_TIMEOUT,&gphhalCt_InEvent,FALSE);
            }
            PH_BREAK_ON_FAILURE(eStatus);
        }

        if(((gphhalCt_InEvent & E_PH_HALCT_EVENT_RX) == E_PH_HALCT_EVENT_RX))
        {
            phhalCt_Event_Consume(E_PH_HALCT_EVENT_RX);
            /* If parity event has occurred with Receive event.
             * Useful for T=1 protocol EMVCo Compliance test cases.*/
            if((gphhalCt_InEvent & E_PH_HALCT_EVENT_PARITY) == E_PH_HALCT_EVENT_PARITY)
            {
                if(PH_REG_TEST_BIT(CT_UCR1x_REG, PROT))
                {
                    /* If T=1 protocol,then first receive full block then send parity error status.*/
                    bBlockParity = 0x01;
                }
                else
                {
                    /* In T=0, As soon as parity error detected send parity error status.*/
                    eStatus = PH_ERR_CT_PARITY_ERROR;
                    break;
                }
            }
            gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

            /* Critical section */
            __phUser_EnterCriticalSection();
            /* Decrease the pending bytes to be copied to the protocol layer */
            gphhalCt_WPendingBytes -= wReceiveSize;
            __phUser_ExitCriticalSection();

        }
        /*
         * If WT/BWT timer expired in case of T=0 protocol return a timeout error,
         * so that a de-activation(T=0) or an R block(T=1) is called is called from the pal layer.
         */
        if(((gphhalCt_InEvent & E_PH_HALCT_EVENT_TO3) == E_PH_HALCT_EVENT_TO3))
        {

            phhalCt_Event_Consume(E_PH_HALCT_EVENT_TO3);
            /* Either the WWT timer or BWT timer expired or CWT timer expired */
            eStatus = PH_ERR_CT_TIME_OUT_WWT_OR_BWT;
            if(gphhalCt_BCWTFlag)
            {
                /* If this flag is set that means T=1 character waiting time out.*/
                eStatus = PH_ERR_CT_TIME_OUT_CWT;
            }

            gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;
            break;
        }
        eStatus = phhalCt_HandleCommonEvent();
        PH_BREAK_ON_FAILURE(eStatus);

        /* Copy the requested bytes for the user */
        phUser_MemCpy((uint8_t*) pbReceiveData, &gphhalCt_DriverBuff[gphhalCt_WReceiveOffset], wReceiveSize);

        /* Increment the offset for the pending bytes if any to be returned */
        gphhalCt_WReceiveOffset += wReceiveSize;
        eStatus = PH_ERR_SUCCESS;
        if(bBlockParity)
        {
            eStatus = PH_ERR_CT_PARITY_ERROR;
        }
    }while(0);
   return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}


phStatus_t phhalCt_GetBufferPointer(uint8_t ** bBufferPointer)
{
    *bBufferPointer = (uint8_t *)gphhalCt_DriverBuff;
    return PH_ERR_SUCCESS;
}

phStatus_t phhalCt_PPSRequestHandling(phhalCt_PPSParamType_t  *psPPSParams)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    uint8_t bPPSRequestBuffer[4];
    uint8_t bPPSResponseBuffer[4] = {0x00,0x00,0x00,0x00};
    uint8_t bFiDiLSB;
    uint8_t bFiDiMSB;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        uint8_t bCount = 0;

        /*Check for the null pointers*/
        if(psPPSParams == NULL)
        {
            break;
        }

        if(psPPSParams->bNegotiableProtocol > 0x01)
        {
            break;
        }

        /*Check for RFU value */
        for(bCount=0;bCount <PHHAL_CT_FIDI_TAB_LEN;bCount++)
        {
            if(psPPSParams->bFiDi == gkphhalCt_BPreScalar[bCount].bFiDi)
            {
                break;
            }
        }
        if(bCount == PHHAL_CT_FIDI_TAB_LEN) /* End of table reached no FiDi found */
        {
            break;
        }
#endif
        bFiDiLSB = (psPPSParams->bFiDi & PHHAL_CT_LSB_NIBBLE_MASK);
        bFiDiMSB = ((psPPSParams->bFiDi)>> 4);

        if(bFiDiMSB > (gphhalCt_BFiDi >>4)||(psPPSParams->bFiDi < (PHHAL_CT_DEFAULT_FIDI)))
        {
            eStatus = PH_ERR_PPS_EXCHANGE_NOT_REQUIRED;
            break;
        }

        if((bFiDiLSB > (gphhalCt_BFiDi & PHHAL_CT_LSB_NIBBLE_MASK)))
        {
            eStatus = PH_ERR_PPS_EXCHANGE_NOT_REQUIRED;
            break;
        }

        if((psPPSParams->bFiDi == PHHAL_CT_DEFAULT_FIDI )&&(psPPSParams->bNegotiableProtocol == gphhalCt_BFirstOfferedProt))
        {
           eStatus = PH_ERR_PPS_EXCHANGE_NOT_REQUIRED;
           break;
        }

        /* Clear the protocol bit.*/
        PH_REG_CLEAR_BIT(CT_UCR1x_REG,PROT);
        /* Setting Parity retransmission count as maximum for PPS Response.*/
        PH_REG_SET_FIELD(CT_FCR_REG ,PEC2_PEC0,PHHAL_CT_MAXPARITY_ERROR_COUNT);

        /* PPS Request and Response exchange handling.*/
        eStatus = phhalCt_PPSExchangeHandling(bPPSRequestBuffer,bPPSResponseBuffer, psPPSParams);
        PH_BREAK_ON_FAILURE(eStatus);

        /* Setting baud rate for negotiated FiDi.*/
        eStatus = phhalCt_SetBaudRate(bPPSRequestBuffer[2],psPPSParams->bWI ,psPPSParams->bBWI ,psPPSParams->bCWI);
        PH_BREAK_ON_FAILURE(eStatus);

        /* Setting transmission protocol using negotiated protocol value.*/
        phhalCt_SetTransmissionProtocol(psPPSParams->bNegotiableProtocol);
        if(psPPSParams->bNegotiableProtocol)
        {
            psPPSParams->bProtSelT0 = 0x00;
            psPPSParams->bProtSelT1 = 0x01;
        }
        else
        {
            psPPSParams->bProtSelT0 = 0x01;
            psPPSParams->bProtSelT1 = 0x00;
        }
    }while(0);
    if((PH_ERR_SUCCESS != eStatus) && (PH_ERR_PPS_EXCHANGE_NOT_REQUIRED != eStatus))
    {
        /* ReSetting Parity retransmission count as zero for errorneous PPS Response.*/
        PH_REG_SET_FIELD(CT_FCR_REG ,PEC2_PEC0,PHHAL_CT_RESET_PARITY_ERR_COUNT);
        /* Make return structure all the parameter zero on failure.*/
        if(psPPSParams != NULL)
            phUser_MemSet(psPPSParams ,0x00 ,sizeof(phhalCt_PPSParamType_t));
    }
    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t phhalCt_PPSExchangeHandling(uint8_t *pbPPSRequestBuffer ,
                                              uint8_t *pbPPSResponseBuffer,phhalCt_PPSParamType_t  *psPPSParams)
{
    phStatus_t eStatus = PH_ERR_FAILED;
    uint8_t bPpsSecondByte = 0x00;
    uint8_t bValue = 0xFF; /* Default value.*/
    do
    {
        /* Construct PPS request Buffer.*/
        pbPPSRequestBuffer[0] = PHHAL_CT_PPSS_BYTE;
        pbPPSRequestBuffer[1] = (PHHAL_CT_BIT5_MASK | psPPSParams->bNegotiableProtocol);
        bValue ^= pbPPSRequestBuffer[1];
        pbPPSRequestBuffer[2] = psPPSParams->bFiDi;
        bValue ^= pbPPSRequestBuffer[2];
        /* Assign Final PCK value to PPS request buffer.*/
        pbPPSRequestBuffer[3] = bValue;

        eStatus = phhalCt_Transmit(pbPPSRequestBuffer,4);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }

           /* Receive Starting 2 byte of pps response so that further byte presence can come to know.*/
           eStatus = phhalCt_Receive(pbPPSResponseBuffer,2);
           PH_BREAK_ON_FAILURE(eStatus);

           if((pbPPSRequestBuffer[0] != pbPPSResponseBuffer[0])
               ||((pbPPSRequestBuffer[1] & PHHAL_CT_LSB_NIBBLE_MASK) != (pbPPSRequestBuffer[1] & PHHAL_CT_LSB_NIBBLE_MASK)))
           {
                eStatus = PH_ERR_CT_PPS_EXCHANGE_ERROR;
                break;
           }
            /* Testing b5 of PPS0 response for PPS1 presence.*/
           if( (pbPPSResponseBuffer[1] & PHHAL_CT_BIT5_MASK) == 0)
           {
                /* If bit b5 of PPS0 indicates PPS1 absence so card is only ready for default value of FiDi.*/
               gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;  /*default value.*/
               psPPSParams->bFiDi = PHHAL_CT_DEFAULT_FIDI;
           }
           else
           {
               eStatus = phhalCt_Receive(&(pbPPSResponseBuffer[2]),1);
               PH_BREAK_ON_FAILURE(eStatus);

               /*Setting PPS1 byte presence flag.*/
               bPpsSecondByte = 0x01;
               if(pbPPSRequestBuffer[2] != pbPPSResponseBuffer[2] )
               {
                   eStatus = PH_ERR_CT_PPS_EXCHANGE_ERROR;
                   break;
               }
           }
           eStatus = phhalCt_Receive(&(pbPPSResponseBuffer[3]),1);
           PH_BREAK_ON_FAILURE(eStatus);

           /* If second byte is missing in PPS Response, remove PPS request's second byte from PCK calculation*/
           if(!(bPpsSecondByte))
           {
               if((pbPPSRequestBuffer[1] & PHHAL_CT_BIT5_MASK)==0x10)   /* b5 present in PPS0 request.*/
               {
                   pbPPSRequestBuffer[3]^=  PHHAL_CT_BIT5_MASK;          /* so remove bit b5 for PCK calculation.*/
               }
               pbPPSRequestBuffer[3]^= pbPPSRequestBuffer[2];
               /* Negotiate FiDi value value using default value .*/
               pbPPSRequestBuffer[2] = gphhalCt_BCurrentFiDi;
           }
           if(pbPPSRequestBuffer[3] != pbPPSResponseBuffer[3] )
           {
               eStatus = PH_ERR_CT_PPS_EXCHANGE_ERROR;
           }
    }while(0);
    return eStatus;
}

#endif /* NXPBUILD__PHHAL_CT */
