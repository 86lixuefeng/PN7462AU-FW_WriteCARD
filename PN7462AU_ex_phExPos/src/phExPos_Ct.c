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
 * phExPos_Ct.c:  Implements the CT task and the logical operations related to contact interface.
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
#include "phRtos.h"
#include "phExPos_Ct.h"
#include "phFlashBoot_Event.h"
#include "phhalNvic.h"
#include "phUser.h"
#include "phExPos_Cfg.h"
#include "ph_Log.h"
#include "phCfg_EE.h"
#include "phLED.h"
#include "phExPos.h"
#include "phhalTimer.h"
#include "phExPos_Clif.h"
#include "phExPos_Common.h"
#include "phExPos_Utils.h"

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
PH_NOINIT phRtos_QueueHandle_t gphExPos_Ct_Queue = NULL;
extern phhalTimer_Timers_t *pLedTimer;
extern phExPos_Posfw_Data_t gphExPos_UtilsPosParama;
extern phhalTimer_Timers_t *gpphExPos_Clif_PollTimer;
static uint8_t bCtQueuestatus = 1;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExPos_CtCallBack(uint32_t dwIRQMessage);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Performs callback function for the CT IRQ's.
 *
 * @param dwIRQMessage : Presence IRQ from the CT
 */
static void phExPos_CtCallBack(uint32_t dwIRQMessage)
{

    if (bCtQueuestatus == 1)
    {
        bCtQueuestatus = 0;
		phExPos_EventCtMsg_t sCtMsg;
    	phStatus_t status = 0x00;

    	if(dwIRQMessage & CT_USR2_REG_PRESL_MASK)
    	{
        	status = phhalCt_CheckCardPres();
    	}

    	if(status == (PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) )
    	{
    	    sCtMsg.dwMsg[0] = PH_ERR_CT_MAIN_CARD_PRESENT;
    	}
    	else
    	{
        	if(status == (PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT))
        	{
         	   sCtMsg.dwMsg[0] = PH_ERR_CT_MAIN_CARD_ABSENT;
        	}
    	}
    	/** wait indefinitely until post to ct task is success. */
    	sCtMsg.eSrcId = E_PH_CT;

    	PH_USER_ASSERT(phRtos_QueueSend( gphExPos_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );
	}
}

/**
 * Performs message posting to the CT task.
 *
 * @param eCtCmd : CT commands \ref phExPos_EventCtCmds_t
 * @param pDes : Destination message queue handler.
 */
void phExPos_CtPostMsg(phExPos_EventCtCmds_t eCtCmd, void *pDes)
{
	phExPos_EventCtMsg_t sCtMsg;
    /** wait indefinitely until post to ct task is success. */
    sCtMsg.eSrcId = E_PH_SYS;
    sCtMsg.pvDes = pDes;
    sCtMsg.eCommands = eCtCmd;
    PH_USER_ASSERT(phRtos_QueueSend( gphExPos_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );

}

/**
 * Initializes CT Queue & task.
 *
 * @param pSysMsgQueue : Destination system message queue handler.
 * @return Status of the operation.
 */
phStatus_t phExPos_CtTaskInit(void *pSysMsgQueue)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** create the Clif Task and Clif queue and post the msg. */
    gphExPos_Ct_Queue  = phRtos_QueueCreate(PHFL_EVENT_CT_Q_LEN, sizeof(phExPos_EventCtMsg_t));
    PH_USER_ASSERT(gphExPos_Ct_Queue);

    /** Create the ct task. */
    pHandle = NULL;
    phRtos_TaskCreate( &phExPos_CtTask, "CtTask", PH_EXPOS_CFG_RTOS_CT_TASK_STACK_SIZE, gphExPos_Ct_Queue,
        PH_EXPOS_CFG_RTOS_CT_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    return PH_ERR_SUCCESS;
}

/**
 * CT Task which performs all the Contact interface operations.
 *
 * @param pvParams : Handler to the System message queue.
 */
void phExPos_CtTask(void *pvParams)
{
    phExPos_EventCtMsg_t sCtQueue;
    uint32_t dwIRQMessage = CT_USR2_REG_PRESL_MASK;

#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif
    phhalCt_InitParam_t  sInitParams;
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

    /** Initialize the CT PAL layer */
     /* With the performance board configuration as pull down enabled and normally open connector type */
    /* The init parameters should come from the EEPROM */
    PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);

    /* Register the call back */
    phhalCt_RegCallBack(&phExPos_CtCallBack,dwIRQMessage);

    while(1)
    {
        /** Check for the message from the Ct hal . */
        phRtos_QueueReceive( gphExPos_Ct_Queue, (void *)&sCtQueue, portMAX_DELAY );
        bCtQueuestatus = 1;
        if(sCtQueue.eSrcId == E_PH_SYS)
        {
            LOG_TXT("System message to shut down\n");
            break;
        }
        switch((phhalCt_ErrorCodes_t)sCtQueue.dwMsg[0])
        {
            case PH_ERR_CT_MAIN_CARD_PRESENT:
                LOG_TXT("CT card detected.\n");
                phRtos_MutexTake(gphExPos_UtilsPosParama.xCT_CL_Mutex, 0xffffffff);
                phExPos_Ct_Example_App();
                phRtos_MutexGive(gphExPos_UtilsPosParama.xCT_CL_Mutex);
            break;
            case PH_ERR_CT_MAIN_CARD_ABSENT:
                LOG_TXT("CT card removed.\n");

#ifdef PHFL_ENABLE_STANDBY
                /* Wait indefinitely until post to System task is success. */
                sSysTaskMsg.eSrcId = E_PH_CT;
                sSysTaskMsg.pvDes = gphExPos_Ct_Queue;
                sSysTaskMsg.dwMsg[0] = PH_ERR_CT_MAIN_CARD_ABSENT;
                (void)phRtos_QueueSend( sCtQueue.pvDes, (void *)&sSysTaskMsg, portMAX_DELAY);
#endif
                gphExPos_UtilsPosParama.bActiveSlot = false;
                gphExPos_UtilsPosParama.eCurrEvent = E_CARD_REMOVAL_EVENT;
                gphExPos_UtilsPosParama.bAtrSize = false;
                gphExPos_UtilsPosParama.bAtrValid = false;
                phExPos_Utils_Raise_Interrupt();
            break;
            default:
                gphExPos_Clif_PollLoopLOGCount = 0;
                /** It is not expected to reach here. */
                PH_USER_ASSERT(0);
        }

    }

}


/**
 * This is an example application running on PN7462AU on JCOP card with PPSE Applet installed
 * @return
 */
phStatus_t phExPos_Ct_Example_App(void)
{
    phStatus_t phStatus = PH_ERR_FAILED;
    phpalCt_DataParams_t pDataParams;
    uint8_t bAtrBuffer[32];
    volatile EventBits_t uxBits = 0;

    /*Initialize requires parameters*/
    pDataParams.pbAtrBuffer = (uint8_t *)&bAtrBuffer;
    pDataParams.bSizeOfATRbuffer = 32;

	gphExPos_UtilsPosParama.eCurrEvent = E_STS_NO_EVENT;
	gphExPos_UtilsPosParama.bAtrSize = 0x00;
	gphExPos_UtilsPosParama.bAtrValid = false;

        /*If CT interrupt set, go ahead with card activation*/
        if (PH_REG_TEST_BITN(CT_MSR_REG,CT_MSR_REG_PRES_POS) &&  gphExPos_UtilsPosParama.bEnPollStartup)
        {
        	 phhalTimer_Stop(gpphExPos_Clif_PollTimer);
        	 phhalTimer_Stop(pLedTimer);

        	if (phpalCt_ActivateCard(&pDataParams) == PH_ERR_SUCCESS)
           {
              /*Based on protocols supported by card, configure PAL layer
               * accordingly*/
              if(E_PHPAL_CT_T1 == pDataParams.bCardSupportedProt)
              {
                 phpalCt_SetProtocol(E_PHPAL_CT_T1);
              }
              else if(E_PHPAL_CT_T0 == pDataParams.bCardSupportedProt)
              {
                 phpalCt_SetProtocol(E_PHPAL_CT_T0);
              }
              else
              {
                 /* Only for QA warnings */
              }
              if (pDataParams.bAtrReceivedLength <= PH_EXPOS_COMMON_MAX_ATR_LEN)
                 {
                    /*Copy ATR bytes to global ATR buffer*/
                    memcpy(&gphExPos_UtilsPosParama.aAtr,&bAtrBuffer,pDataParams.bAtrReceivedLength);
                 }
              else
              {
            	  PH_USER_ASSERT(0);
              }
              /* If control reaches this point then it means that the CT is active
              * Update event type in POS global structure and raise intrupt to host(LPC1769)*/
              gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CT_CHANNEL_NO;
              gphExPos_UtilsPosParama.eCurrEvent = E_BANKING_CARD_INSERTION_EVENT_CT;
              gphExPos_UtilsPosParama.bAtrSize = pDataParams.bAtrReceivedLength;
              gphExPos_UtilsPosParama.bAtrValid = true;

              phExPos_UtilsAll_LEDS_OFF();
              /*Glow Blue LED to indicate the CT card detection */
              phExPos_UtilsGlow_CT_Card_Detection_LED();
              /* Raise the interrupt to Host */
              phExPos_Utils_Raise_Interrupt();

           }
        	else
        	{
        		return 0;
        	}

           /*Infinitely wait for APDU commands from host*/
           while (1)
           {
              uxBits = phRtos_EventGroupWaitBits(gphExPos_UtilsPosParama.xCT_Events,
                                           PH_EXPOS_COMMON_CT_TRNSP_EX_CMD |
                                           PH_EXPOS_COMMON_CT_DEACTIVATE_CARD_CMD |
                                           PH_EXPOS_COMMON_CT_ABORT_CMD,
                                           pdTRUE,          /* status bits should be cleared before returning. */
                                           pdFALSE,         /* wait for any status bit being set. */
                                           portMAX_DELAY); /* wait indefinitely */

              /*Card deactivate event from host / card removal interrupt from CT HAL
               * notify event to host, update POS structure and notify to host*/

              if (uxBits & (uint32_t)PH_EXPOS_COMMON_CT_DEACTIVATE_CARD_CMD)
              {
            	 phExPos_Utils_Clear_Interrupt();
                 phhalCt_CardDeactivate();
                 gphExPos_UtilsPosParama.bAtrValid = false;
                 gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CHANNEL_NONE;
                 gphExPos_UtilsPosParama.eCurrEvent = E_STS_NO_EVENT;
                 gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
                 gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
                 gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
                 gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
                 gphExPos_UtilsPosParama.bpTxBuf[4] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0xFF00) >> 0x08;
                 gphExPos_UtilsPosParama.bpTxBuf[5] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0x00FF);
                 gphExPos_UtilsPosParama.bTxDataValid = 1;
                 /*Send the response to the host*/
                 phExPos_UtilsSendResponseToHost();

                 if(gphExPos_UtilsPosParama.Trans_complete)
                 {
                	 /*If all commands are through including Generate AC command then glow LEDs to indicate successful transaction*/
                	 phExPos_UtilsGlow_Success_LEDS();
                 }
                 else
                 {
                	 phExPos_UtilsGlow_Error_LEDS();
                 }
                 /*Wait Untill the card is removed from the CT slot*/
                 while(PH_REG_TEST_BITN(CT_MSR_REG,CT_MSR_REG_PRES_POS));
                 break;
              }
              /*APDU exchange command from host, exchange APDU and send response to host*/
              else if (uxBits & (uint32_t)PH_EXPOS_COMMON_CT_TRNSP_EX_CMD )
              {
                 phStatus = phpalCt_Transceive((uint8_t*)&gphExPos_UtilsPosParama.aRxBuff[1],
                                               gphExPos_UtilsPosParama.aRxBuff[0],
											   (uint8_t*)&gphExPos_UtilsPosParama.aTxBuff[1],
											   (uint16_t*)&gphExPos_UtilsPosParama.aTxBuff[0]);
                if (phStatus != PH_ERR_SUCCESS)
                {
                   phExPos_UtilsGlow_Error_LEDS();
                   gphExPos_UtilsPosParama.bpTxBuf[0] = 2;
                   gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
                   gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
                   gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
                   gphExPos_UtilsPosParama.bpTxBuf[4] =  phStatus&0xff; // error code
                   gphExPos_UtilsPosParama.bpTxBuf[5] =  0xff& (phStatus >> 8);
                   gphExPos_UtilsPosParama.bTxDataValid = true;
                   /*Send the response to the host*/
                   phExPos_UtilsSendResponseToHost();
                }
                /*Successful Generate AC command means the transaction is successful*/
                if(((gphExPos_UtilsPosParama.bpRxBuf[5] == 0xAE)) &&
                    (gphExPos_UtilsPosParama.bpTxBuf[gphExPos_UtilsPosParama.aTxBuff[0] + 2] == 0x90))
                {
                	gphExPos_UtilsPosParama.Trans_complete = 1;
                }
                gphExPos_UtilsPosParama.bTxDataValid = true;
                /*Send the response to the host*/
                phExPos_UtilsSendResponseToHost();
              }
              /*ABORT command from host*/
              else if ((uxBits & (uint32_t) PH_EXPOS_COMMON_CT_ABORT_CMD))
            	  /*Abort command is not being used*/
              {
            	  /*This is not being used*/
                  phExPos_UtilsSendResponseToHost();
                 break;
              }
              else
              {
                 gphExPos_UtilsPosParama.eCurrEvent = E_STS_NO_EVENT;
              }
           }

        }
        return 0;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
