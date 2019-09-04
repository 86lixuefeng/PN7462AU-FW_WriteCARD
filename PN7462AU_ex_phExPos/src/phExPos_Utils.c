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
 * phExPos_Utils.c:  Handling of control commands and definitions of general functions.
 *
 *
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phRtos.h"
#include "task.h"
#include "queue.h"
#include "phhalHif.h"
#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phhalGpio.h"
#include "phhalTimer.h"
#include "phUser.h"
#include "phExPos_Common.h"
#include "phExPos_SPIHIF.h"
#include "phExPos_Utils.h"
#include "phExPos.h"
#include "phhalTimer.h"

extern phhalTimer_Timers_t *gpphExPos_Clif_PollTimer;

phExPos_Posfw_Data_t gphExPos_UtilsPosParama;

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */


void phExPos_UtilsExecuteControlCmd()
{
    gphExPos_UtilsPosParama.bTxDataValid = 0;
    if((gphExPos_UtilsPosParama.bpRxBuf[PH_EXPOS_COMMON_INS_BYTE]) == 0xF8)
    {
	   switch(gphExPos_UtilsPosParama.bpRxBuf[PH_EXPOS_COMMON_P1_BYTE])
	   {
		  case PH_EXPOS_COMMON_CNTRL_CMD_GET_CONFIG:
		  {
			 phExPos_Utils_Clear_Interrupt();
			 gphExPos_UtilsPosParama.bTxDataValid = 1;
			 gphExPos_UtilsPosParama.bpTxBuf[0] = 4;
			 gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
			 gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
			 gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
			 gphExPos_UtilsPosParama.bpTxBuf[4] = PH_EXPOS_COMMON_CL_SLOT_TAG_TYPE;
			 gphExPos_UtilsPosParama.bpTxBuf[5] = PH_EXPOS_COMMON_CT_SLOT_TAG_TYPE;
			 gphExPos_UtilsPosParama.bpTxBuf[6] = 0x90; /*success code*/
			 gphExPos_UtilsPosParama.bpTxBuf[7] = 00;
			 /*turn off all LED's*/
			 phExPos_UtilsAll_LEDS_OFF();
			 break;
		  }

		  case PH_EXPOS_COMMON_CNTRL_CMD_GET_STATUS:
		  {
			 gphExPos_UtilsPosParama.bTxDataValid = 1;
			 gphExPos_UtilsPosParama.bpTxBuf[0] = 4;
			 gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
			 gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
			 gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
			 gphExPos_UtilsPosParama.bpTxBuf[4] =   gphExPos_UtilsPosParama.eCurrEvent;
			 gphExPos_UtilsPosParama.bpTxBuf[5] =	gphExPos_UtilsPosParama.bActiveSlot;
			 gphExPos_UtilsPosParama.bpTxBuf[6] =	0x90;
			 gphExPos_UtilsPosParama.bpTxBuf[7] =	00;
			 phExPos_Utils_Clear_Interrupt();
			 break;
		  }

		  case PH_EXPOS_COMMON_CNTRL_CMD_DEACTIVATE_CARD:
		  {
			 /*post msg directly to clif / ct task based on current active slot*/
			 if (gphExPos_UtilsPosParama.bActiveSlot == PH_EXPOS_COMMON_CL_CHANNEL_NO)
			 {
				phExPos_SPIHIFSPIPostEventCLTask(PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD);
			 }else if (gphExPos_UtilsPosParama.bActiveSlot == PH_EXPOS_COMMON_CT_CHANNEL_NO)
			 {
				 phExPos_SPIHIFSPIPostEventCTTask(PH_EXPOS_COMMON_CT_DEACTIVATE_CARD_CMD);
			 }
			 else
			 {
				gphExPos_UtilsPosParama.bTxDataValid = 1;
				/* header*/
				gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02;
				gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
				gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00;
				gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
				gphExPos_UtilsPosParama.bpTxBuf[4] = 0x90;
				gphExPos_UtilsPosParama.bpTxBuf[5] = 0x00;
			 }
			 break;
		  }
		  case PH_EXPOS_COMMON_CNTRL_CMD_GET_ATR:
		  {
			 gphExPos_UtilsPosParama.bTxDataValid = 1;
			 if (gphExPos_UtilsPosParama.bAtrValid)
			 {
				gphExPos_UtilsPosParama.bpTxBuf[0] = (gphExPos_UtilsPosParama.bAtrSize + 2) & 0xFF;
				gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
				gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
				gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
				phUser_MemCpy (&gphExPos_UtilsPosParama.bpTxBuf[4], &gphExPos_UtilsPosParama.aAtr[0],
							   gphExPos_UtilsPosParama.bAtrSize);
				gphExPos_UtilsPosParama.bpTxBuf[gphExPos_UtilsPosParama.bAtrSize + 4] = 0x90;
				gphExPos_UtilsPosParama.bpTxBuf[gphExPos_UtilsPosParama.bAtrSize + 5] = 0x00;
			 }
			 else
			 {
				/* header */
				gphExPos_UtilsPosParama.bpTxBuf[0] = 2;
				gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
				gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
				gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
				/* copy failure code */
				gphExPos_UtilsPosParama.bpTxBuf[4] = 0x67;
				gphExPos_UtilsPosParama.bpTxBuf[5] = 0x00;
			 }
			 break;
		  }
		  case PH_EXPOS_COMMON_CNTRL_CMD_EN_POLLING:
		  {
			  gphExPos_UtilsPosParama.Trans_complete = 0;

			 gphExPos_UtilsPosParama.bEnPollStartup = 1;
			 /* prepare command success response in global buffer*/
			 gphExPos_UtilsPosParama.bTxDataValid = 1;
			 gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
			 gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
			 gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
			 gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
			 gphExPos_UtilsPosParama.bpTxBuf[4] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0xFF00) >> 0x08;
			 gphExPos_UtilsPosParama.bpTxBuf[5] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0x00FF);
			 /* start polling timer*/
			 phhalTimer_Start(gpphExPos_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
			 break;
		  }
		  case PH_EXPOS_COMMON_CNTRL_CMD_DIS_POLLING:
		  {
			 phhalTimer_Stop(gpphExPos_Clif_PollTimer);
			 gphExPos_UtilsPosParama.bEnPollStartup = 0;
			 gphExPos_UtilsPosParama.bTxDataValid = 1;
			 gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
			 gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
			 gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
			 gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
			 gphExPos_UtilsPosParama.bpTxBuf[4] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0xFF00) >> 0x08;
			 gphExPos_UtilsPosParama.bpTxBuf[5] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0x00FF);
			 break;
		  }
		  case PH_EXPOS_COMMON_CNTRL_CMD_ABORT_TRANSFER:
		  {
			 if (gphExPos_UtilsPosParama.bActiveSlot == PH_EXPOS_COMMON_CL_CHANNEL_NO)
			 {
				phExPos_SPIHIFSPIPostEventCLTask(PH_EXPOS_COMMON_CL_ABORT_CMD);
			 }else if (gphExPos_UtilsPosParama.bActiveSlot == PH_EXPOS_COMMON_CT_CHANNEL_NO)
			 {
				phExPos_SPIHIFSPIPostEventCTTask(PH_EXPOS_COMMON_CL_ABORT_CMD);
			 }else
			 {
				PH_USER_ASSERT(0);
			 }
			 break;
		  }
		  case PH_EXPOS_COMMON_CNTRL_CMD_GET_UID:
		  {
					gphExPos_UtilsPosParama.bTxDataValid = 1;

					gphExPos_UtilsPosParama.bpTxBuf[0] = (gphExPos_UtilsPosParama.Uid_Length + 2);
					gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
					gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
					gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
					phUser_MemCpy (&gphExPos_UtilsPosParama.bpTxBuf[4], &gphExPos_UtilsPosParama.Uid,gphExPos_UtilsPosParama.Uid_Length);
					gphExPos_UtilsPosParama.bpTxBuf[gphExPos_UtilsPosParama.Uid_Length + 4] = 0x90;
					gphExPos_UtilsPosParama.bpTxBuf[gphExPos_UtilsPosParama.Uid_Length + 5] = 0x00;

			  break;
		  }

		  default:
		  {
			 gphExPos_UtilsPosParama.bTxDataValid = 1;
			 /* header */
			 gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
			 gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
			 gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
			 gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
			 /* Invalid parameter in the APDU  */
			 gphExPos_UtilsPosParama.bpTxBuf[4] = 0x65;
			 gphExPos_UtilsPosParama.bpTxBuf[5] = 0x02;
			 break;
		  }
	    }
    }

    else
    {
       gphExPos_UtilsPosParama.bTxDataValid = 1;
       /* header */
       gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
       gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
       gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
       gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
       /* INS code is invalid */
       gphExPos_UtilsPosParama.bpTxBuf[4] = 0x65;
       gphExPos_UtilsPosParama.bpTxBuf[5] = 0x04;
    }
    if (gphExPos_UtilsPosParama.bTxDataValid)
    {
    	/*Send the response to the host*/
       phExPos_UtilsSendResponseToHost();
    }
}


void phExPos_UtilsSendResponseToHost(void)
{
    uint16_t wTxLen;
    if(gphExPos_UtilsPosParama.bTxDataValid == 1)
    {
       wTxLen = (uint16_t)gphExPos_UtilsPosParama.aTxBuff[0];
       wTxLen += PH_EXPOS_COMMON_HEADER_SIZE;
       phhalHif_Transmit((uint32_t *)&gphExPos_UtilsPosParama.aTxBuff, wTxLen,
                                        (pphhalHif_Callback_t)&phExPos_SPIHIFSPITxCallback);
       gphExPos_UtilsPosParama.bTxDataValid = 0;
       /*Make the state of the Synchronizing GPIO Pin as Low*/
       /*When this pin becomes low, LPC can start reading the response*/
       phExPos_SPIHIFGPIOLineIdle();
    }
}

void phExPos_UtilsInitGlobals(void)
{
    gphExPos_UtilsPosParama.bAtrValid = 0;
    gphExPos_UtilsPosParama.bAtrSize = 0;
    gphExPos_UtilsPosParama.bEnPollStartup = 0;
    gphExPos_UtilsPosParama.bTxDataValid = FALSE;
    gphExPos_UtilsPosParama.bRxDataValid = FALSE;
    gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CHANNEL_NONE;
    gphExPos_UtilsPosParama.eCurrEvent = E_STS_NO_EVENT;
    gphExPos_UtilsPosParama.bpRxBuf = (uint8_t*)gphExPos_UtilsPosParama.aRxBuff;
    gphExPos_UtilsPosParama.bpTxBuf = (uint8_t*)gphExPos_UtilsPosParama.aTxBuff;
}

void phExPos_UtilsPOS_Init(void)
{
	phExPos_UtilsConfig_GPIO();
	phExPos_UtilsAll_LEDS_OFF();

    phExPos_SPIHIFGPIOLineBusy();

    phExPos_UtilsInitGlobals();
    phExPos_SPIHIFPosConfigSPIHIF();

    phExPos_Utils_Clear_Interrupt();


    /* create CLIF and CT event groups*/
    gphExPos_UtilsPosParama.xCL_Events =  phRtos_EventGroupCreate();

    phRtos_EventGroupClearBits(gphExPos_UtilsPosParama.xCL_Events,
                               (PH_EXPOS_COMMON_CL_TRNSP_EX_CMD |
                                PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD |
                                PH_EXPOS_COMMON_CL_ABORT_CMD));

    /* assert if fail to create event group */
    if( gphExPos_UtilsPosParama.xCL_Events == NULL )
    {
       PH_USER_ASSERT(0); /* It is not expected to reach here. */
    }

    gphExPos_UtilsPosParama.xCT_Events =  phRtos_EventGroupCreate();
    phRtos_EventGroupClearBits(gphExPos_UtilsPosParama.xCT_Events,
                               (PH_EXPOS_COMMON_CL_TRNSP_EX_CMD |
                                PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD |
                                PH_EXPOS_COMMON_CL_ABORT_CMD));

    /* assert if fail to create event group */
    if( gphExPos_UtilsPosParama.xCT_Events == NULL )
    {
       PH_USER_ASSERT(0); /* It is not expected to reach here. */
    }

    gphExPos_UtilsPosParama.xCT_CL_Mutex = phRtos_MutexCreate();

}

void phExPos_UtilsGenerateATRFromAts(uint8_t * pAts)
{

	uint16_t i;
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
    gphExPos_UtilsPosParama.aAtr[0] = 0x3B;
    gphExPos_UtilsPosParama.aAtr[1] = (uint8_t)(0x80 | bAtsCopyLen);
    gphExPos_UtilsPosParama.aAtr[2] = 0x80;
    gphExPos_UtilsPosParama.aAtr[3] = 0x01;
    /*Copy ATS bytes to global ATR array*/
    phUser_MemCpy (&gphExPos_UtilsPosParama.aAtr[4], &pAts[bOffset], bAtsCopyLen); /* PRQA S 3200 */

    gphExPos_UtilsPosParama.aAtr[(0x04 + bAtsCopyLen)] = 0x00;
    /*copy ATR valication code*/
    for (i = 1; i < (0x04 + bAtsCopyLen); i++)
    {
       gphExPos_UtilsPosParama.aAtr[(0x04 + bAtsCopyLen)] =
       (uint8_t)(gphExPos_UtilsPosParama.aAtr[(0x04 + bAtsCopyLen)] ^ gphExPos_UtilsPosParama.aAtr[i]);
    }

    /*update actual ATR length*/
    gphExPos_UtilsPosParama.bAtrSize = bLength;
    gphExPos_UtilsPosParama.bAtrValid = 1;

}

void phExPos_UtilsConfig_GPIO(void)
{
    /*configure GPIO pin for communication coordination between LPC and PN7462AU*/
    phhalPcr_ConfigOutput(PH_EXPOS_COMMON_POS_GPIO_LINE,true,false);
    /*configure GPIO pins to glow status LEDs*/
    phhalPcr_ConfigOutput(PH_EXPOS_COMMON_POS_BLUE_LED,true,false); /*Blue LED*/
    phhalPcr_ConfigOutput(PH_EXPOS_COMMON_POS_GREEN_LED,true,false); /*Green LED*/
    phhalPcr_ConfigOutput(PH_EXPOS_COMMON_POS_YELLOW_LED,true,false); /*Yellow LED*/
    phhalPcr_ConfigOutput(PH_EXPOS_COMMON_POS_RED_LED,true,false); /*Red LED*/
}
void phExPos_UtilsAll_LEDS_OFF(void)
{
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_BLUE_LED,false);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GREEN_LED,false);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_YELLOW_LED,false);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_RED_LED,false);
}
void phExPos_UtilsGlow_Error_LEDS(void)
{
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_RED_LED,true);
	phUser_Wait(50000);
	PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_RED_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_RED_LED,true);
	phUser_Wait(50000);
	PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_RED_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_RED_LED,true);
}
void phExPos_UtilsGlow_Success_LEDS(void)
{
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GREEN_LED,true);
	phUser_Wait(50000);
	PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GREEN_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GREEN_LED,true);
	phUser_Wait(50000);
	PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GREEN_LED,false);
    phUser_Wait(50000);
    PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_GREEN_LED,true);
}

void phExPos_UtilsGlow_CLIF_Card_Detection_LED(void)
{
	PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_YELLOW_LED,true);

}

void phExPos_UtilsGlow_CT_Card_Detection_LED(void)
{
	PH_HAL_GPIO_SETGPIOVAL(PH_EXPOS_COMMON_POS_BLUE_LED,true);
}

void phExPos_Utils_Raise_Interrupt(void)
{
    PH_REG_SET_BIT(PCR_PADOUT_REG,PADOUT_IRQ);
}
void phExPos_Utils_Clear_Interrupt(void)
{
    PH_REG_CLEAR_BIT(PCR_PADOUT_REG, PADOUT_IRQ);
}

