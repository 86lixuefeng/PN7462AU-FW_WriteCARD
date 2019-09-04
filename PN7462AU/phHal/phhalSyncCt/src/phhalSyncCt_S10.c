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
 * Synchronous CT card S10 protocol.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 11:44:15 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16973 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SYNCCT

#include "ph_Status.h"

#include "phhalSyncCt_S10.h"

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
PH_NOINIT static uint8_t gbDataByte;
PH_NOINIT static uint8_t gbAddrByte;
PH_NOINIT static uint8_t gbCtrlByte;
PH_NOINIT static uint8_t gbOutgoing;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* This static function is use for processing command .*/
static phStatus_t phhalSyncCt_S10_CommandProcessing(uint8_t *pbDataBuffer ,uint32_t dwDataNumber);
/* This function is used to send start condition to card.*/
static phStatus_t phhalSyncCt_Start(void);
/* This function is used to send stop condition to card.*/
static void phhalSyncCt_Stop(void);



/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * This function is used to power up S9 card which consists IP initialization and card activation.
 * @param    uint8_t bPullupSetStatus ->        If logic set to 1 Pull up else  pull down config.
 * @param    uint8_t bCardPresConnectorStatus ->  If Logic 1 Normally open else normally close config.
 * @param    uint8_t bSlewRateConfig ->           To rise from 0 to its selected value (see ct_pcr_reg register), the time
                                                    needed by VCC depends on the capacitance.
 * @param   pbAtrBuffer -> Pointer to the ATR buffer where the ATR header bytes will be stored
 *
 * @retval    #PH_ERR_SUCCESS - Activation for S10 card is successful.
 * @retval    #PH_ERR_FAILED  - Activation for S10 card is not successful.
 * @maskedret #PH_ERR_SYNC_CT_CARD_ABSENT - If card is absent in main slot.
 */
phStatus_t phhalSyncCt_S10Powerup(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,
                                                                        uint8_t bSlewRateConfig ,uint8_t *pbAtrBuffer)
{
    phStatus_t eStatus = PH_ERR_FAILED;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbAtrBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif
    eStatus = phhalSyncCt_Init(bPullupSetStatus, bCardPresConnectorStatus, bSlewRateConfig);

    /* wait for connector type stability.*/
     (void)phUser_Wait(10000);

     eStatus =phhalSyncCt_CardActivate(pbAtrBuffer ,E_TYPE1);
     if(eStatus != PH_ERR_SUCCESS)
     {
         if(eStatus == PH_ADD_COMPCODE(PH_ERR_SYNC_CT_ACTIVATION_FAILED,PH_COMP_HAL_SYNC_CT))
         {
             eStatus =phhalSyncCt_CardActivate(pbAtrBuffer ,E_TYPE2);
         }
         return eStatus;
     }
    return PH_ERR_SUCCESS;
}

/**
 * This function is used to process command send from terminal to card.
 * @param   uint8_t pbDataBuffer  -> It points to  DataBuffer in which commands are stored.
 * @param   uint32_t dwOffset -> Offset should be fixed before calling this api .
 *                              Offset will indicate towards the location having control,address and data byte.
 * @param uin32_t  dwByteCount -> Number of byte count to be write in card other than control data address byte.
 *
 * @retval PH_ERR_SUCCESS - Command processing(read/write operations)are done successfully.
 * @retval PH_ERR_FAILED - Command processing is not done successfully.
 *
 */

phStatus_t phhalSyncCt_S10_ProcessCommand(uint8_t *pbDataBuffer,uint32_t dwOffset, uint32_t dwByteCount)
{

    uint32_t dwDataNumber;
    uint32_t dwEndBlockNumber;
    uint8_t bReceivedbit =0x00;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbDataBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif
    phhalSyncCt_SetTRMode(0);
    (void)phUser_Wait(5000);
    phhalSyncCt_GetSyncBit(&bReceivedbit);
    if(bReceivedbit == 0)
    {
        return PH_ERR_FAILED;
    }
    gbCtrlByte = pbDataBuffer[dwOffset];
    dwOffset++;
    gbAddrByte = pbDataBuffer[dwOffset];
    dwOffset++;
    gbDataByte = pbDataBuffer[dwOffset];

    dwDataNumber = gbCtrlByte & 0x0F;

    if ((dwDataNumber==0x00) || (dwDataNumber== 0x01) || (dwDataNumber== 0x04))    /*Testing Control byte.*/
    {
        gbOutgoing = 0x01;
        dwEndBlockNumber = 0x100;                           /*user memory.*/
        if(dwDataNumber==1)                              /* Outgoing command.*/
        dwEndBlockNumber = 0x04;
        if(dwDataNumber==0x04)
        dwEndBlockNumber = 0x20;
        /*read data from addr_byte to the end of block.*/
        phhalSyncCt_S10_CommandProcessing(pbDataBuffer, (dwEndBlockNumber-gbAddrByte));
        if(gbDataByte==0)
        dwDataNumber = 0x100;
        else
        {
            if(gbDataByte<(dwEndBlockNumber-gbAddrByte))
            dwDataNumber = dwByteCount;
            else
            dwDataNumber = (dwEndBlockNumber-gbAddrByte);
        }
    }
    else                                            /* Processing command.*/
    {
        gbOutgoing = 0x00;
        dwDataNumber = dwByteCount;
        for(; dwDataNumber>0 ; dwDataNumber--, gbAddrByte++, dwOffset++)
        {
            gbDataByte = pbDataBuffer[dwOffset];       /*update next data to write.*/
            phhalSyncCt_S10_CommandProcessing(pbDataBuffer,0);
            phUser_Wait(1000);                     /* wait 4*(400 cycles = 1ms).*/
        }

    }
    return PH_ERR_SUCCESS;
}


/**
 * This function is used to process command and send address data and control byte to card.
 * And also stores all the received byte in buffer for outgoing mode and
 * send all the byte from buffer for non outgoing mode.
 * @param   uint8_t pbDataBuffer  -> It points to  DataBuffer in which commands are stored.
 * @param   uint32_t dwDataNumber -> Byte count number to write or read.
 *
 */
static phStatus_t phhalSyncCt_S10_CommandProcessing(uint8_t *pbDataBuffer ,uint32_t dwDataNumber)
{

    uint32_t dwLoopCount;
    uint8_t bIO_line;

    if(PH_ERR_SUCCESS != phhalSyncCt_Start())
    {
        return PH_ERR_FAILED;
    }
    phhalSyncCt_TransmitByte(gbCtrlByte);
    phhalSyncCt_TransmitByte(gbAddrByte);
    phhalSyncCt_TransmitByte(gbDataByte);

    phhalSyncCt_Stop();
    if (gbOutgoing)
    {
        /*if outgoing mode then read datas.*/
        for(dwLoopCount=0; dwDataNumber>0; dwLoopCount++, dwDataNumber--)
        {
            phhalSyncCt_ReceiveByte(&(pbDataBuffer[dwLoopCount]));
        }
    }
    else
    {
        do
        {
            phhalSyncCt_Send_Nbclocks(0x01);                        /*send clock until IO line remain high.*/
            phhalSyncCt_GetSyncBit(&(bIO_line));
        } while (!bIO_line);
    }
    return PH_ERR_SUCCESS;
}
/**
 * This function is use to send start condition to S10 card.
 */
static phStatus_t phhalSyncCt_Start(void)
{
    uint8_t byte = 0x00;
    uint8_t  bSdahigh = 0x00;
    uint8_t bReceivedbit = 0x00;
    phStatus_t eStatus = PH_ERR_SUCCESS;

    bSdahigh = 0x01;

    phhalSyncCt_SetTRMode(0);  /*set SDA in high state.*/
    (void)phUser_Wait(5000);
    phhalSyncCt_SetClock(1);/* set CLK in high state.*/
    (void)phUser_Wait(40);
    phhalSyncCt_GetSyncBit(&(bReceivedbit));
    if (!(bReceivedbit))        /* if SDA is not in high state.*/
    {
        phhalSyncCt_ReceiveByte(&(byte));       /*try to unlock sda line by sending 8 clock pulses.*/

        if (!(byte & (0x80)))
        {
            phhalSyncCt_Stop();
            bSdahigh = 0;
            eStatus = PH_ERR_FAILED;
        }
    }
    if (bSdahigh)
    {
        phhalSyncCt_SetSyncBit(0);         /* set SDA in low state.*/
        phhalSyncCt_SetTRMode(1);             /* ISO-UART in output mode*/
        phUser_Wait(20);
        phhalSyncCt_SetClock(0);            /*set CLK in low state.*/
    }
    return eStatus;
}

/**
 *  DESCRIPTION : This function is use to send stop condition to S10 card.
 */
static void phhalSyncCt_Stop(void)
{
    phhalSyncCt_SetSyncBit(0);         /* Be sure SDA is Low.*/
    phhalSyncCt_SetTRMode(1);              /* ISO-UART in transmission.*/
    phhalSyncCt_SetClock(1);           /* clock line high.*/
    phhalSyncCt_SetSyncBit(1);         /*stop condition.*/
    phUser_Wait(20);
    phhalSyncCt_SetClock(0);
    phhalSyncCt_SetTRMode(0);              /*ISO-UART in reception mode.*/
}

#endif /* NXPBUILD__PHHAL_SYNCCT */
