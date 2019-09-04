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
 * $Date: 2016-12-01 14:26:44 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18769 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phExNFCCcid.h"
#include "phExNFCCcid_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phFlashBoot_Event.h"
#include "phhalRf.h"
#include "phhalNvic.h"
#include "phCfg_EE.h"
#include "phExNFCCcid_Cfg.h"
#include "string.h"
#include "phUser.h"
#include "phhalTimer.h"
#include "phFlashBoot.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phExNFCCcid_Log.h"
#include "phhalGpio.h"
#include "ph_Log.h"
#include "phLED.h"

#include "phExNFCCcid_LED.h"
#include "PN7462AU/PN7462AU_ctip.h"

#include "PN7462AU/PN7462AU_pmu.h"
#include "phExNFCCcid_Usb_If.h"
#include "phhalPmu.h"
#include "phExNFCCcid_UsbCcid.h"
#include "phExNFCCcid_LLCP_Client.h"
#include "phlnLlcp.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXNFCCCID_TXLDO_MONITOR_ENABLE            0x1

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static phRtos_QueueHandle_t gphExNFCCcid_SysTask_Queue = NULL;
static phRtos_QueueHandle_t gphExNFCCcid_Clif_Queue = NULL;
phhalTimer_Timers_t *pLedTimer;
#ifdef PHFL_LLCP_SNEP_ENABLE
extern phlnLlcp_Sw_DataParams_t        slnLlcp;                    /* LLCP component holder */
#endif
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExNFCCcid_PcrCallBack(uint32_t dwIRQMessage);
static void phExNFCCcid_MonitorEnableTxLdo();

static void phExNFCCcid_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
		                            phExNFCCcid_Clif_MsgId_t eClifCmd, void *pDes);
static void phExNFCCcid_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
static void phExNFCCcid_ClifTask(void *pvParams);
//static void phExNFCCcid_Clif_Osal_Event_Init();
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phExNFCCcid_TimerCallBackFunc(void)
{
	/* Post the polling message to clif task. */
	phExNFCCcid_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXNFCCCID_CLIF_CMD_START, gphExNFCCcid_SysTask_Queue);
}

static phStatus_t phExNFCCcid_PostBootReason(phhalPcr_BootReason_t eBootReason)
{
    phFlashBoot_Event_SysMsg_t eSysMsg;

    eSysMsg.dwMsg[0] = (uint32_t)eBootReason;
    eSysMsg.eSrcId = E_PH_BOOT;
    eSysMsg.pvDes = gphExNFCCcid_SysTask_Queue;

    /* wait indefinitely until post boot reason to system task. */
    if(phRtos_QueueSend( gphExNFCCcid_SysTask_Queue,  &eSysMsg, PHRTOS_MAX_DELAY)!= phRtos_Success ){
        /* Failed to post the message, even after 10 ticks. */
        return PH_ERR_OPERATION_TIMEDOUT;
    }
    return PH_ERR_SUCCESS;
}
/*
 * Wake up Boot Reason handler.
 */
static phStatus_t phExNFCCcid_BootHandler(void)
{
    phhalPcr_BootReason_t eBootReason;

    /* Get the boot reason. */
    eBootReason = phFlashBoot_GetBtRns();

    /* Clear the existing boot reasons to avoid each module to clear specifically. */
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /* Post the Boot reason to the system task.
     * Go to infinite loop if the posting is failed. */
    PH_USER_ASSERT(phExNFCCcid_PostBootReason(eBootReason) == PH_ERR_SUCCESS);

    return PH_ERR_SUCCESS;
}

static phStatus_t phExNFCCcid_SystemTaskInit(void)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /* Create the queue to which PMU/PCR ISR and other tasks post messages */
    gphExNFCCcid_SysTask_Queue = phRtos_QueueCreate(PHFL_SYS_TASK_Q_LEN, sizeof(phFlashBoot_Event_SysMsg_t));

    PH_USER_ASSERT(gphExNFCCcid_SysTask_Queue);

    /* Create system task with stack size of 384 bytes and highest priority */
    phRtos_TaskCreate( &phExNFCCcid_SystemTask, "SystemTask", PH_EXNFCCCID_CFG_RTOS_SYS_TASK_STACK_SIZE, NULL,
        PH_EXNFCCCID_CFG_RTOS_SYS_TASK_PRIORITY, &pHandle );

    PH_USER_ASSERT(pHandle);

    /* Enable the TxLdo Monitor if External TVDD is selected */
    phExNFCCcid_MonitorEnableTxLdo();

    return PH_ERR_SUCCESS;
}

int main(void)
{
    LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    phFlashBoot_Main();
    phhalTimer_Init();
    phOsal_Init();
    phExNFCCcid_Clif_HalInit();

    /* Initialize the HAL of USB and Core and the CCID Class */
    PH_USER_ASSERT(phExNFCCcid_Usb_If_TotalInit() == PH_ERR_SUCCESS);

    /* Create & initialize system task */
    phExNFCCcid_SystemTaskInit();

    /* Check the Boot reason and post the message to system task OR perform the corresponding boot operation. */
    phExNFCCcid_BootHandler();

    /* Start the tasks and timer running. */
    phRtos_Start();

    return 0;

}


static void phExNFCCcid_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
{
    LOG_U32("Wkup",eBootReason);
    phExNFCCcid_Log_BootReason(eBootReason);

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
        phExNFCCcid_ClifTaskInit(E_BOOT_NONE, NULL);

        break;

        /* CLIF related wakeup reasons */
    case E_RFLDT_BOOT :
        /* Starts the CLIF task which executes the CLIF main example */
        phExNFCCcid_ClifTaskInit(eBootReason, gphExNFCCcid_SysTask_Queue);
        break;

    case E_WUC_CNT :
        /* Starts the CLIF task which executes the CLIF main example */
		phExNFCCcid_ClifTaskInit(eBootReason, gphExNFCCcid_SysTask_Queue);

        break;

    default:
        PH_USER_ASSERT(0); /* It is not expected to reach here. */
    }
}
void phExNFCCcid_SystemTask(void *pvParams)
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
    phhalPcr_RegCallBack(&phExNFCCcid_PcrCallBack,dwPcrIRQMessage);

    while(1)
    {
        phRtos_QueueReceive( gphExNFCCcid_SysTask_Queue, &sSysMsg, portMAX_DELAY );
        switch(sSysMsg.eSrcId)
        {
        /* Message from the flash boot. */
        case E_PH_BOOT:
            phExNFCCcid_WakeUpHandler((phhalPcr_BootReason_t)sSysMsg.dwMsg[0]);
            break;

            /* Message from the CLIF task */
        case E_PH_CLIF:
            switch(sSysMsg.dwMsg[0])
            {
            /* Since CLIF application completed the Sys task either go to standby, start poll timer */
            case (uint32_t)E_PHEXNFCCCID_CLIF_RSP_END:
                phhalTimer_Start(gpphExNFCCcid_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
                phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
            break;

            default:
                PH_USER_ASSERT(0); /* It is not expected to reach here. */
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
static void phExNFCCcid_PcrCallBack(uint32_t dwIRQMessage)
{
    if(dwIRQMessage & (PCR_INT_STATUS_REG_SUSPEND_INT_STATUS_MASK|PCR_INT_ENABLE_REG_SUSPEND_DEFAULTED_INT_ENABLE_MASK))
    {

    }
    return;
}

static void phExNFCCcid_MonitorEnableTxLdo()
{
	/* Check the configured TxLdo is External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXNFCCCID_USETXLDO_EXTERNAL)
	{
#if defined (PH_EXNFCCCID_EXTERNAL_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXNFCCCID_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXNFCCCID_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
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
static void phExNFCCcid_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                             phExNFCCcid_Clif_MsgId_t eClifCmd, void *pDes)
{
    phExNFCCcid_Clif_Msg_t sClifMsg;

    sClifMsg.sClifConfig.eDiscLoopEntry = eDiscLoopEntry;

    sClifMsg.eSrcId = E_PH_CLIF;
    sClifMsg.pvDes = pDes;
    sClifMsg.eClifMsgId = eClifCmd;
    /* wait indefinitely until post boot clif task is success. */
    PH_USER_ASSERT(phRtos_QueueSend( gphExNFCCcid_Clif_Queue, &sClifMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Initializes Clif Queue & task, posts bootUp message in case of Standby enabled.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
static void phExNFCCcid_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /* Create the queue to which System Task can post messages */
    gphExNFCCcid_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExNFCCcid_Clif_Msg_t));
    PH_USER_ASSERT(gphExNFCCcid_Clif_Queue);

    /* Create clif task with stack size of 1800 bytes and highest priority */
    pHandle = NULL;
    phRtos_TaskCreate(&phExNFCCcid_ClifTask, "ClifTask", PH_EXNFCCCID_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXNFCCCID_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    /* create CLIF and CT event groups*/
    gphExNFCCcid_sUsb_SlotInfo.xCL_Events =  phRtos_EventGroupCreate();
    phRtos_EventGroupClearBits (gphExNFCCcid_sUsb_SlotInfo.xCL_Events,
                                (PH_EXNFCCCID_USBCCID_CL_TRNSP_EX_CMD        |
                                 PH_EXNFCCCID_USBCCID_CL_DEACTIVATE_CARD_CMD |
                                 PH_EXNFCCCID_USBCCID_CL_AUTH_CMD            |
                                 PH_EXNFCCCID_USBCCID_CL_READ_CARD_CMD       |
                                 PH_EXNFCCCID_USBCCID_CL_WRITE_CARD_CMD)
                                );
    /* assert if fail to create event group */
    if( gphExNFCCcid_sUsb_SlotInfo.xCL_Events == NULL )
    {
        PH_USER_ASSERT(0); /* It is not expected to reach here. */
    }
#ifdef PHFL_LLCP_SNEP_ENABLE
    pHandle = NULL;
    phRtos_TaskCreate(&phExNFCCcid_LLCP_Client, "LLCP_Client", PH_EXNFCCCID_LLCP_CLIENT_TASK_STACK, (void *)&slnLlcp,
           PH_EXNFCCCID_LLCP_CLIENT_TASK_PRIO, &pHandle );
    PH_USER_ASSERT(pHandle);
#endif
}

/**
 * Clif Task which performs all the Contact less interface operations.
 *
 * @param pvParams : Unused.
 */
static void phExNFCCcid_ClifTask(void *pvParams)
{
	  phExNFCCcid_Clif_Msg_t sClifMsg;
    uint16_t wStatus;

#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
    phStatus_t eStatus;
#endif
    phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
    phacDiscLoop_Sw_DataParams_t sDiscLoopParam;

    /* Initialize the Clif Hal. */
    //phExNFCCcid_Clif_HalInit();
    //phExNFCCcid_Clif_Osal_Event_Init();

    /* Configure the discovery loop parameters, for polling/listen/active */
	psDiscLoopParams = &sDiscLoopParam;
	wStatus = phExNFCCcidClif_DiscLoopConfig(psDiscLoopParams);
	if(wStatus != PH_ERR_SUCCESS)
	{
		/** It is not expected to reach here. */
		PH_USER_ASSERT(0);
	}

	/**
	 *  hardware specific API to initialize the Hal and Pal.
	 *  Initializes the psDiscLoopParams->pHalDataParams & psDiscLoopParams->pPalDataParams variables
	 *  based on Poll and Listen bit map configuration.
	 */
	if(phExNFCCcidClif_PalInit(psDiscLoopParams) != PH_ERR_SUCCESS)
	{
		phExNFCCcidClif_DeInit(psDiscLoopParams->pHalDataParams);
		/** It is not expected to reach here. */
		PH_USER_ASSERT(0);
	}

	/*Standby is disabled, enable the poll timer, configure and start running in single shot mode. */
	PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,
		&gpphExNFCCcid_Clif_PollTimer) == PH_ERR_SUCCESS);

	phhalTimer_Configure(gpphExNFCCcid_Clif_PollTimer, (uint32_t)gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal,
		&phExNFCCcid_TimerCallBackFunc);

    while(1)
    {
    	phhalTimer_Start(gpphExNFCCcid_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
    	/* check for the Clif profile and startup sequence. */
        phRtos_QueueReceive( gphExNFCCcid_Clif_Queue, (void *)&sClifMsg, portMAX_DELAY );
        /* LOG_TXT_D("=>Clif Task\n"); */

        switch(sClifMsg.eClifMsgId)
        {
        case E_PHEXNFCCCID_CLIF_CMD_START:
        	psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
            /* phExNFCCcid_Main is a standalone application for Clif. */
#ifdef PH_EXNFCCCID_USB_IF_COMPLIANCY
            if (gphExNFCCcid_sUsb_Bus_Status.bAddressed)
#endif
                (uint32_t)phExNFCCcid_ClifMain(psDiscLoopParams, sClifMsg.sClifConfig.eDiscLoopEntry);

#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
			if (gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable == 1)
			{
				eStatus = phExNFCCcid_Usb_If_Suspend();

				if (eStatus == PH_ERR_SUCCESS)
				{
					phExNFCCcid_Usb_If_Resume();
				}
				else
				{
					phExNFCCcid_LED_Status(RED_LED, LED_ON);
					phExNFCCcid_Usb_If_Resume();
				}
			}
#endif
			phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
            break;

        case E_PHEXNFCCCID_CLIF_CMD_STOP:
            /* system task wants us to stop */
            break;

        default:
            /* It is not expected to reach here. */
            PH_USER_ASSERT(0);
            break;
        }
    }
}

//static void phExNFCCcid_Clif_Osal_Event_Init()
//{
//    phStatus_t  status;
//
//    status = phOsal_Event_Init();
//    PH_USER_ASSERT(status == PH_ERR_SUCCESS);
//
//}
