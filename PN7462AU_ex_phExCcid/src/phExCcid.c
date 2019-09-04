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
 * phExCcid.c:  Main application file and entry point of phExCcid. Contains core logic part of the phExCcid application.
 *
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-08-31 11:47:41 +0530 (Wed, 31 Aug 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 3840 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phExCcid.h"
#include "phExCcid_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phFlashBoot_Event.h"
#include "phhalRf.h"
#include "phhalNvic.h"
#include "phCfg_EE.h"
#include "phExCcid_Cfg.h"
#include "string.h"
#include "phUser.h"
#include "phhalTimer.h"
#include "phFlashBoot.h"
#include "phExCcid_Ct.h"
#include "phhalCt.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phExCcid_Log.h"
#include "phhalGpio.h"
#include "ph_Log.h"
#include "phLED.h"

#include "phExCcid_LED.h"
#include "PN7462AU/PN7462AU_ctip.h"

#include "PN7462AU/PN7462AU_pmu.h"
#include "phExCcid_Usb_If.h"
#include "phhalPmu.h"
#include "phExCcid_UsbCcid.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXCCID_TXLDO_MONITOR_ENABLE            0x1

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static phRtos_QueueHandle_t gphExCcid_SysTask_Queue = NULL;
static phRtos_QueueHandle_t gphExCcid_Ct_Queue = NULL;
static phRtos_QueueHandle_t gphExCcid_Clif_Queue = NULL;
phhalTimer_Timers_t *pLedTimer;
static uint8_t bCTActive_Event = 1;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExCcid_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
		phExCcid_Clif_MsgId_t eClifCmd, void *pDes);
static void phExCcid_CtTaskInit(void *pSysMsgQueue);
static void phExCcid_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
static void phExCcid_CtPostMsg(phExCcid_Ct_EventCmds_t eCtCmd, void *pDes);
static void phExCcid_CtCallBack(uint32_t dwIRQMessage);
static void phExCcid_ClifTask(void *pvParams);
static void phExCcid_CtTask(void *pvParams);

static void phExCcid_PcrCallBack(uint32_t dwIRQMessage);
static void phExCcid_MonitorEnableTxLdo();

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phExCcid_TimerCallBackFunc(void)
{
	if (PH_EXCCID_USBCCID_CHANNEL_NONE == gphExCcid_sUsb_SlotInfo.bSlotType)
	    phExCcid_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXCCID_CLIF_CMD_START, gphExCcid_SysTask_Queue);
}

static phStatus_t phExCcid_PostBootReason(phhalPcr_BootReason_t eBootReason)
{
    phFlashBoot_Event_SysMsg_t eSysMsg;

    eSysMsg.dwMsg[0] = (uint32_t)eBootReason;
    eSysMsg.eSrcId = E_PH_BOOT;
    eSysMsg.pvDes = gphExCcid_SysTask_Queue;

    /* wait indefinitely until post boot reason to system task. */
    if(phRtos_QueueSend( gphExCcid_SysTask_Queue,  &eSysMsg, PHRTOS_MAX_DELAY)!= phRtos_Success ){
        /* Failed to post the message, even after 10 ticks. */
        return PH_ERR_OPERATION_TIMEDOUT;
    }
    return PH_ERR_SUCCESS;
}
/*
 * Wake up Boot Reason handler.
 */
static phStatus_t phExCcid_BootHandler(void)
{
    phhalPcr_BootReason_t eBootReason;

    /* Get the boot reason. */
    eBootReason = phFlashBoot_GetBtRns();

    /* Clear the existing boot reasons to avoid each module to clear specifically. */
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /* Post the Boot reason to the system task.
     * Go to infinite loop if the posting is failed. */
    PH_USER_ASSERT(phExCcid_PostBootReason(eBootReason) == PH_ERR_SUCCESS);

    return PH_ERR_SUCCESS;
}

static phStatus_t phExCcid_SystemTaskInit(void)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /* Create the queue to which PMU/PCR ISR and other tasks post messages */
    gphExCcid_SysTask_Queue = phRtos_QueueCreate(PHFL_SYS_TASK_Q_LEN, sizeof(phFlashBoot_Event_SysMsg_t));

    PH_USER_ASSERT(gphExCcid_SysTask_Queue);

    /* Create system task with stack size of 384 bytes and highest priority */
    phRtos_TaskCreate( &phExCcid_SystemTask, "SystemTask", PH_EXCCID_CFG_RTOS_SYS_TASK_STACK_SIZE, NULL,
        PH_EXCCID_CFG_RTOS_SYS_TASK_PRIORITY, &pHandle );

    PH_USER_ASSERT(pHandle);

    /* Enable the TxLdo Monitor if External TVDD is selected */
    phExCcid_MonitorEnableTxLdo();

    return PH_ERR_SUCCESS;
}

int main (void)
{
	LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    phFlashBoot_Main();
    phhalTimer_Init();
    phExCcid_Clif_HalInit();
    phOsal_Init();

    /* Initialize the HAL of USB and Core and the CCID Class */
    PH_USER_ASSERT(phExCcid_Usb_If_TotalInit() == PH_ERR_SUCCESS);

    /* Create & initialize system task */
    phExCcid_SystemTaskInit();

    /* Check the Boot reason and post the message to system task OR perform the corresponding boot operation. */
    phExCcid_BootHandler();

    /* Start the tasks and timer running. */
    phRtos_Start();

    return 0;

}

static void phExCcid_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
{
    LOG_U32("Wkup",eBootReason);
    phExCcid_Log_BootReason(eBootReason);

    switch(eBootReason)
    {
    /* Normal boot scenario, set wakeups and goto Power down mode. */
    case E_STARTUP_POR  :
    case E_SOFT_RESET   :
    case E_ACTIVE_HPD   :
    case E_TEMP_SENSOR0 :     /* Temp became normal. */
    case E_TEMP_SENSOR1 :     /* Temp became normal. */
    case E_HIF_RESET    :
    case E_WATCH_DOG    :
    case E_NO_PVDD      :
    case E_SWP_DET      :
        /* Initialize the Tasks based on its Module configurations. */
        phExCcid_CtTaskInit(gphExCcid_SysTask_Queue);
        phExCcid_ClifTaskInit(E_BOOT_NONE, NULL);

        break;

        /* CLIF related wakeup reasons */
    case E_RFLDT_BOOT :
        /* Starts the CLIF task which executes the CLIF main example */
        phExCcid_ClifTaskInit(eBootReason, gphExCcid_SysTask_Queue);
        phExCcid_CtTaskInit(gphExCcid_SysTask_Queue);

        break;

    case E_WUC_CNT :
        /* Check the CT card presence. */
        {
            phhalCt_InitParam_t  sInitParams;
            sInitParams.bAutoCTDeactivationEnable= 0;
            sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
            sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
            sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

            PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);
            (void)phpalCt_SetConfig( E_EMV_ENABLE, TRUE);

            if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
            {
                /* CT card present. perform CT operations. */
                /* create the required CT queues and task. */
                phExCcid_CtTaskInit(gphExCcid_SysTask_Queue);
                /* start the CT application. */
                phExCcid_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExCcid_SysTask_Queue);
            }
            else
            {
                /* perform Clif operations. */
                /* Starts the CLIF task which executes the CLIF main example */
                phExCcid_ClifTaskInit(eBootReason, gphExCcid_SysTask_Queue);
            }
        }
        break;

        /* contact card presence. */
    case E_CT_PRESENCE :
        /* create the required CT queues and task. */
        phExCcid_CtTaskInit(gphExCcid_SysTask_Queue);
        phExCcid_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExCcid_SysTask_Queue);

        break;

    default:
        PH_USER_ASSERT(0); /* It is not expected to reach here. */
    }
}
void phExCcid_SystemTask(void *pvParams)
{
    phFlashBoot_Event_SysMsg_t sSysMsg;

    uint32_t dwPcrIRQMessage = PCR_INT_STATUS_REG_SUSPEND_INT_STATUS_MASK | PCR_INT_ENABLE_REG_SUSPEND_DEFAULTED_INT_ENABLE_MASK;
    /* Enable the LED timer.
     * configure it for 100 milli secs to timeout and start running in free running mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100 , phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
    phhalPcr_RegCallBack(&phExCcid_PcrCallBack,dwPcrIRQMessage);

    while(1)
    {
        phRtos_QueueReceive( gphExCcid_SysTask_Queue, &sSysMsg, portMAX_DELAY );
        switch(sSysMsg.eSrcId)
        {
        /* Message from the flash boot. */
        case E_PH_BOOT:
            phExCcid_WakeUpHandler((phhalPcr_BootReason_t)sSysMsg.dwMsg[0]);
            break;

            /* Message from the CLIF task */
        case E_PH_CLIF:
            switch(sSysMsg.dwMsg[0])
            {
            /* Since CLIF application completed the Sys task either go to standby, start poll timer */
            case (uint32_t)E_PHEXCCID_CLIF_RSP_END:
                phhalTimer_Start(gpphExCcid_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
                phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
            break;

            default:
                PH_USER_ASSERT(0); /* It is not expected to reach here. */
            }

            break;

			/* Handle the message posts from the CT. */
		case E_PH_CT:
			switch((phhalCt_ErrorCodes_t)sSysMsg.dwMsg[0])
			{
			case PH_ERR_CT_MAIN_CARD_ABSENT:
			break;
			default:
				/* Work around for the CT hw bug. */
				PH_USER_ASSERT(1); /* It is not expected to reach here. */
			}
		break;
		default:
			PH_USER_ASSERT(0); /* It is not expected to reach here. */
        }

    }
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static void phExCcid_PcrCallBack(uint32_t dwIRQMessage)
{
    if(dwIRQMessage & (PCR_INT_STATUS_REG_SUSPEND_INT_STATUS_MASK|PCR_INT_ENABLE_REG_SUSPEND_DEFAULTED_INT_ENABLE_MASK))
    {

    }
    return;
}

static void phExCcid_MonitorEnableTxLdo()
{
    /* Check the configured TxLdo is External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXCCID_USETXLDO_EXTERNAL)
    {
#if defined (PH_EXCCID_EXTERNAL_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXCCID_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXCCID_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

/**
 * Performs message posting to the Clif task.
 *
 * @param eDiscLoopEntry : Discovery loop entry point.
 * @param eClifCmd : Clif command.
 * @param pDes : Destination message queue handler.
 */
static void phExCcid_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
		                         phExCcid_Clif_MsgId_t eClifCmd, void *pDes)
{
	phExCcid_Clif_Msg_t sClifMsg;

    sClifMsg.sClifConfig.eDiscLoopEntry = eDiscLoopEntry;

    sClifMsg.eSrcId = E_PH_CLIF;
    sClifMsg.pvDes = pDes;
    sClifMsg.eClifMsgId = eClifCmd;
    /** wait indefinitely until post boot clif task is success. */
    PH_USER_ASSERT(phRtos_QueueSend( gphExCcid_Clif_Queue, &sClifMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Initializes Clif Queue & task, posts bootUp message in case of Standby enabled.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
static void phExCcid_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Create the queue to which System Task can post messages */
    gphExCcid_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExCcid_Clif_Msg_t));
    PH_USER_ASSERT(gphExCcid_Clif_Queue);

    /** Create clif task with \ref PH_EXCCID_CFG_RTOS_CLIF_TASK_PRIORITY priority. */
    pHandle = NULL;
    phRtos_TaskCreate(&phExCcid_ClifTask, "ClifTask", PH_EXCCID_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXCCID_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    /* create CLIF and CT event groups*/
    gphExCcid_sUsb_EventInfo.xCL_Events =  phRtos_EventGroupCreate();
	phRtos_EventGroupClearBits (gphExCcid_sUsb_EventInfo.xCL_Events,
								(PH_EXCCID_USBCCID_CL_TRNSP_EX_CMD        |
								 PH_EXCCID_USBCCID_CL_DEACTIVATE_CARD_CMD |
								 PH_EXCCID_USBCCID_CL_AUTH_CMD            |
								 PH_EXCCID_USBCCID_CL_READ_CARD_CMD       |
								 PH_EXCCID_USBCCID_CL_WRITE_CARD_CMD)
								);
	/* assert if fail to create event group */
	if( gphExCcid_sUsb_EventInfo.xCL_Events == NULL )
	{
		PH_USER_ASSERT(0); /* It is not expected to reach here. */
	}

}

/**
 * Clif Task which performs all the Contact less interface operations.
 *
 * @param pvParams : Unused.
 */
static void phExCcid_ClifTask(void *pvParams)
{
	phExCcid_Clif_Msg_t sClifMsg;
    uint16_t wStatus;
    uint8_t bAts[64] ;
#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
    phStatus_t eStatus;
#endif
    phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
    phacDiscLoop_Sw_DataParams_t sDiscLoopParam;

    /* Configure the discovery loop parameters, for polling/listen/active */
    psDiscLoopParams = &sDiscLoopParam;
    wStatus = phExCcidClif_DiscLoopConfig(psDiscLoopParams);
    if(wStatus != PH_ERR_SUCCESS)
    {
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

    /* Initialize the parameters for polling/listen/active */
    phExCcidClif_DiscLoopParamInit(psDiscLoopParams,bAts);

    /**
     *  hardware specific API to initialize the Hal and Pal.
     *  Initializes the psDiscLoopParams->pHalDataParams & psDiscLoopParams->pPalDataParams variables
     *  based on Poll and Listen bit map configuration.
     */
    if(phExCcidClif_PalInit(psDiscLoopParams) != PH_ERR_SUCCESS)
    {
    	phExCcidClif_DeInit(psDiscLoopParams->pHalDataParams);
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

    /*Standby is disabled, enable the poll timer, configure and start running in single shot mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,
        &gpphExCcid_Clif_PollTimer) == PH_ERR_SUCCESS);

    phhalTimer_Configure(gpphExCcid_Clif_PollTimer, (uint32_t)gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal,
        &phExCcid_TimerCallBackFunc);

    while(1)
    {
        phhalTimer_Start(gpphExCcid_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
        /** check for the Clif profile and startup sequence. */
        phRtos_QueueReceive( gphExCcid_Clif_Queue, (void *)&sClifMsg, portMAX_DELAY );
        switch(sClifMsg.eClifMsgId)
        {
        case E_PHEXCCID_CLIF_CMD_START:

            psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
#ifdef PH_EXCCID_USB_IF_COMPLIANCY
            if (gphExCcid_sUsb_Bus_Status.bAddressed)
#endif
               (uint32_t)phExCcid_ClifMain(psDiscLoopParams, sClifMsg.sClifConfig.eDiscLoopEntry);
#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
			if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
			{
				eStatus = phExCcid_Usb_If_Suspend();

				if (eStatus == PH_ERR_SUCCESS)
				{
					phExCcid_Usb_If_Resume();
				}
				else
				{
					phExCcid_LED_Status(RED_LED, LED_ON);
					phExCcid_Usb_If_Resume();
				}
			}
#endif
			phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
            break;

        case E_PHEXCCID_CLIF_CMD_STOP:
            /** Nothing to do */
            break;

        default:
            /** It is not expected to reach here. */
            PH_USER_ASSERT(0);
            break;
        }
    }
}

/**
 * Performs message posting to the CT task.
 *
 * @param eCtCmd : CT commands \ref phExCcid_EventCtCmds_t
 * @param pDes : Destination message queue handler.
 */
static void phExCcid_CtPostMsg(phExCcid_Ct_EventCmds_t eCtCmd, void *pDes)
{
	phExCcid_Ct_EventMsg_t sCtMsg;
    /** wait indefinitely until post to ct task is success. */
    sCtMsg.eSrcId = E_PH_SYS;
    sCtMsg.pvDes = pDes;
    sCtMsg.eCommands = eCtCmd;
    sCtMsg.dwMsg = PH_ERR_CT_MAIN_CARD_PRESENT;
    PH_USER_ASSERT(phRtos_QueueSend( gphExCcid_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Performs callback function for the CT IRQ's.
 *
 * @param dwIRQMessage : Presence IRQ from the CT
 */
static void phExCcid_CtCallBack(uint32_t dwIRQMessage)
{
	phExCcid_Ct_EventMsg_t sCtMsg;
    phStatus_t status = 0x00;

    if ((bCTActive_Event == 1) && (!gphExCcid_sUsb_SlotInfo.bCardPresent)) {
        bCTActive_Event = 0;
    if(dwIRQMessage & CT_USR2_REG_PRESL_MASK)
    {
        status = phhalCt_CheckCardPres();
    }

    if(status == (PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) )
    {
        sCtMsg.dwMsg = PH_ERR_CT_MAIN_CARD_PRESENT;
    }
    else
    {
        if(status == (PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT))
        {
            sCtMsg.dwMsg = PH_ERR_CT_MAIN_CARD_ABSENT;
        }
    }
    /** wait indefinitely until post to ct task is success. */
    sCtMsg.eSrcId = E_PH_CT;

    PH_USER_ASSERT(phRtos_QueueSend( gphExCcid_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );
    }
}

/**
 * Initializes CT Queue & task.
 *
 * @param pSysMsgQueue : Destination system message queue handler.
 * @return Status of the operation.
 */
static void phExCcid_CtTaskInit(void *pSysMsgQueue)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /* create the Clif Task and Clif queue and post the msg. */
    gphExCcid_Ct_Queue  = phRtos_QueueCreate(PHFL_EVENT_CT_Q_LEN, sizeof(phExCcid_Ct_EventMsg_t));
    PH_USER_ASSERT(gphExCcid_Ct_Queue);

    /* Create the ct task. */
    pHandle = NULL;
    phRtos_TaskCreate( &phExCcid_CtTask, "CtTask", PH_EXCCID_CFG_RTOS_CT_TASK_STACK_SIZE, gphExCcid_Ct_Queue,
        PH_EXCCID_CFG_RTOS_CT_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

     /* Create the Events for the CT */
    gphExCcid_sUsb_EventInfo.xCT_Events =  phRtos_EventGroupCreate();
    phRtos_EventGroupClearBits(gphExCcid_sUsb_EventInfo.xCT_Events,
                               (PH_EXCCID_USBCCID_CT_TRNSP_EX_CMD |
                                PH_EXCCID_USBCCID_CT_DEACTIVATE_CARD_CMD |
                                PH_EXCCID_USBCCID_CT_ABORT_CMD));

    /* assert if fail to create event group */
    if( gphExCcid_sUsb_EventInfo.xCT_Events == NULL )
    {
        PH_USER_ASSERT(0); /* It is not expected to reach here. */
    }
}

static void phExCcid_CtTask(void *pvParams)
{
    phStatus_t eStatus;
    phExCcid_Ct_EventMsg_t sCtQueue;

    uint32_t dwIRQMessage = CT_USR2_REG_PRESL_MASK;

    phhalCt_InitParam_t  sInitParams;
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

    /* Initialize the CT PAL layer
     * With the performance board configuration as pull down enabled and normally open connector type */
    /* The init parameters should come from the EEPROM */
    PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);
    (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);

    /* Register the call back */
    phhalCt_RegCallBack(&phExCcid_CtCallBack,dwIRQMessage);

    while(1)
    {
        /* Check for the message from the Ct hal . */
        phRtos_QueueReceive( gphExCcid_Ct_Queue, (void *)&sCtQueue, portMAX_DELAY );

        bCTActive_Event = 1;

        switch((phhalCt_ErrorCodes_t)sCtQueue.dwMsg)
        {
            case PH_ERR_CT_MAIN_CARD_PRESENT:
                LOG_TXT("Contact Card detected\n");
                phLED_SetPattern(gkphLED_Ct_Pass);

                eStatus = phExCcid_Ct_Example_App();
                if(PH_ERR_SUCCESS != eStatus)
                {
                    LOG_TXT("phExCcid_Ct_Example_App FAILED\n");
                    LOG_X32("F:", eStatus);
                }
                else
                {
                    LOG_TXT("phExCcid_Ct_Example_App PASSED\n");
                }
#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
				if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
				{
					eStatus = phExCcid_Usb_If_Suspend();

					if (eStatus == PH_ERR_SUCCESS)
					{
						phExCcid_Usb_If_Resume();
					}
					else
					{
						phExCcid_LED_Status(RED_LED, LED_ON);
						phExCcid_Usb_If_Resume();
					}
				}
#endif
                phhalTimer_Start(gpphExCcid_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
                phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

            break;
            case PH_ERR_CT_MAIN_CARD_ABSENT:
                 LOG_TXT("Contact card removed\n");

#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
				if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
				{
					eStatus = phExCcid_Usb_If_Suspend();

					if (eStatus == PH_ERR_SUCCESS)
					{
						phExCcid_Usb_If_Resume();
					}
					else
					{
						phExCcid_LED_Status(RED_LED, LED_ON);
						phExCcid_Usb_If_Resume();
					}
				}
#endif
            break;
            default:

                /* It is not expected to reach here. */
                PH_USER_ASSERT(0);
        }

    }

}
