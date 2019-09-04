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
 * phExPos_SPIHIF.c:  This file takes care of SPI Host Interface and event posting based on the command received.
 *
 *
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-21 14:39:49 +0530 (Wed, 21 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18674 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */


#include "phExPos_SPIHIF.h"
#include "phhalHif.h"
#include <ph_Status.h>
#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phhalGpio.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "phExPos_Common.h"
#include "phExPos_Utils.h"
#include "phUser.h"
#include "PN7462AU/PN7462AU_hif.h"

extern phExPos_Posfw_Data_t gphExPos_UtilsPosParama;

void phExPos_SPIHIFPosConfigSPIHIF(void)
{
	phStatus_t bstatus = PH_EXPOS_COMMON_STATUS_SUCCESS;
    uint16_t wPacket_size;

    wPacket_size = PH_EXPOS_COMMON_HOST_RX_BUF_LEN;

    phhalHif_ReleaseRxBuffer(E_RX_BUFFER_ID0);
    phhalHif_DeInitRxBuffer(E_RX_BUFFER_ID0);
    PH_REG_SET(HOSTIF_INT_CLR_STATUS_REG,0xFFFFFFFF);
    bstatus = phExPos_SPIHIFSPI_Init();
    if(!bstatus)
    {
	   bstatus = phhalHif_InitRxBuffer(E_RX_BUFFER_ID0,wPacket_size,(uint32_t *)gphExPos_UtilsPosParama.aRxBuff,(pphhalHif_Callback_t)&phExPos_SPIHIFSPIRxCallBack);
    }

}

phStatus_t phExPos_SPIHIFSPI_Init(void)
{

    phhalHif_Config_t SpiConfig;
    phStatus_t eStatus;

    SpiConfig.bHeaderSize = 0;
    SpiConfig.bShortFrameLen = 0;
    SpiConfig.bStoreErrData = 0;
    SpiConfig.bTimeout = 0x00;
    SpiConfig.eBufferType = E_BUFFER_FORMAT_FREE;
    SpiConfig.eInterface = E_HIF_SPI;
    SpiConfig.sConfig.sSpiConfig = 0x00;

    /*Initialise the Host interface*/
    eStatus = phhalHif_Init(&SpiConfig,(pphhalHif_Callback_t)&phExPos_SPIHIFSPISlaveErrorCallBack);

    __ENABLE_IRQ();

    return eStatus;
}

void phExPos_SPIHIFGPIOLineBusy(void)
{
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GPIO_LINE,true);
}

void phExPos_SPIHIFGPIOLineIdle(void)
{
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GPIO_LINE,false);
}

void phExPos_SPIHIFSPIPostEventCLTask(uint32_t dwEvent)
{
	phRtos_Status_t xResult;

    xResult =  phRtos_EventGroupSetBits( gphExPos_UtilsPosParama.xCL_Events, dwEvent );

    /*Assert if event posting failure*/
    if( xResult != phRtos_Success )
    {
       /* Switch context if necessary. */
    	PH_USER_ASSERT(0);
    }

}

void phExPos_SPIHIFSPIPostEventCTTask(uint32_t dwEvent)
{
	phRtos_Status_t xResult;

    xResult =  phRtos_EventGroupSetBits( gphExPos_UtilsPosParama.xCT_Events, dwEvent );

    /*Assert if event posting failure*/
    if( xResult != phRtos_Success )
    {
       /* Switch context if necessary. */
    	PH_USER_ASSERT(0);
    }

}

void phExPos_SPIHIFSPIRxCallBack(uint32_t dwIntStatus, void *fptr)
{

    phExPos_SPIHIFGPIOLineBusy();
    phhalHif_ReleaseRxBuffer(E_RX_BUFFER_ID0);

    if(PH_EXPOS_COMMON_TRANS_EXCHANGE_CMD == gphExPos_UtilsPosParama.bpRxBuf[PH_EXPOS_COMMON_CLA_BYTE])
    	/*Send the control commands to the interpreter*/
    {
       phExPos_UtilsExecuteControlCmd();
    }
    else
    {
    	/*Send the Received commands to the appropriate tasks based on the actice slot*/
       switch(gphExPos_UtilsPosParama.bActiveSlot)
       {
          case PH_EXPOS_COMMON_CL_CHANNEL_NO:
             phExPos_SPIHIFSPIPostEventCLTask(PH_EXPOS_COMMON_CL_TRNSP_EX_CMD);
             break;
          case PH_EXPOS_COMMON_CT_CHANNEL_NO:
             phExPos_SPIHIFSPIPostEventCTTask(PH_EXPOS_COMMON_CT_TRNSP_EX_CMD);
             break;
          default:
             gphExPos_UtilsPosParama.bTxDataValid = 1;
             gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
             gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
             gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
             gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
             // copy failure code
             gphExPos_UtilsPosParama.bpTxBuf[4] = 0x67;
             gphExPos_UtilsPosParama.bpTxBuf[5] = 0x00;

             if (gphExPos_UtilsPosParama.bTxDataValid)
             {
            	 /*Send the response to the host*/
                phExPos_UtilsSendResponseToHost();
             }
             break;
      }
    }
}

void phExPos_SPIHIFSPITxCallback(uint32_t dwIntStatus, void *fptr)
{
    phExPos_SPIHIFGPIOLineIdle();
}

void phExPos_SPIHIFSPISlaveErrorCallBack(uint32_t dwIntStatus, void *fptr)
{
    /* Add the code which is to be processed during HIF error interrupt.*/
}

/*************************************************************************************************************
 * 		EOF - End Of File
 ************************************************************************************************************/



