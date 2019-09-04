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
 * phExPos.c:  Main application file and entry point of phExPos.
 *
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
#include "ph_Status.h"

#ifdef PHFL_HALAPI_WITH_RTOS

#include "phRtos.h"
#include "phExPos.h"
#include "phExPos_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phFlashBoot_Event.h"
#include "phhalRf.h"
#include "phhalNvic.h"
#include "phCfg_EE.h"
#include "phExPos_Cfg.h"
#include "string.h"
#include "phUser.h"
#include "phhalTimer.h"
#include "phFlashBoot.h"
#include "phExPos_Ct.h"
#include "phhalCt.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phExPos_Log.h"
#include "phhalGpio.h"
#include "ph_Log.h"
#include "phLED.h"
#include "phhalPcr.h"
#include "phhalPmu.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "phExPos_Utils.h"
#include "phExPos_SPIHIF.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define SYS_TASK_FREQUENCY_MS                     ( 20 / portTICK_PERIOD_MS )
#define PH_ENABLE                       0x0001U /**< Enabled. */
#define PH_DISABLE                      0x0000U /**< Disabled. */
#define PH_EXPOS_TXLDO_MONITOR_ENABLE   0X01
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static phRtos_QueueHandle_t gphExPos_SysTask_Queue = NULL;



phhalTimer_Timers_t *pLedTimer;


/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExPos_MonitorEnableTxLdo(void);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */


#ifndef PHFL_ENABLE_STANDBY
/** Timer timeout function to kick start discovery loop polling during NON STANDBY case. */
void phExPos_TimerCallBackFunc(void)
{
    phExPos_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXMAIN_CLIF_CMD_START, gphExPos_SysTask_Queue);
}
#endif

/**
 * Posts boot reason to the System/app Task.
 *
 * @param eBootReason : Boot reason to be posted to the System/app Task.
 * @return Status of the posting operation.
 */
static phStatus_t phExPos_PostBootReason(phhalPcr_BootReason_t eBootReason)
{
    phFlashBoot_Event_SysMsg_t eSysMsg;

    eSysMsg.dwMsg[0] = (uint32_t)eBootReason;
    eSysMsg.eSrcId = E_PH_BOOT;
    eSysMsg.pvDes = gphExPos_SysTask_Queue;

    /** wait indefinitely until post boot reason to system task. */
    if(phRtos_QueueSend( gphExPos_SysTask_Queue,  &eSysMsg, PHRTOS_MAX_DELAY)!= phRtos_Success ){
        /** Failed to post the message, even after 10 ticks. */
        return PH_ERR_OPERATION_TIMEDOUT;
    }
    return PH_ERR_SUCCESS;
}

/**
 * Wake up Boot Reason handler.
 */
static phStatus_t phExPos_BootHandler(void)
{
    phhalPcr_BootReason_t eBootReason;

    /** Get & clear the boot reason. */
    eBootReason = phFlashBoot_GetBtRns();
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /** Post the Boot reason to the system task. Hang here if the posting is failed. */
    PH_USER_ASSERT(phExPos_PostBootReason(eBootReason) == PH_ERR_SUCCESS);

    return PH_ERR_SUCCESS;
}

/**
 * Initializes the System/app Task and its queue.
 *
 * @return Status of the System/app's Task & queue initialization.
 */
static phStatus_t phExPos_SystemTaskInit(void)
{
    phRtos_TaskHandle_t pHandle = NULL;

    phExPosClif_HalInit();
    /** Create the queue to which PMU/PCR ISR and other tasks post messages */
    gphExPos_SysTask_Queue = phRtos_QueueCreate(PHFL_SYS_TASK_Q_LEN, sizeof(phFlashBoot_Event_SysMsg_t));

    PH_USER_ASSERT(gphExPos_SysTask_Queue);

    /** Create system task */
    phRtos_TaskCreate( &phExPos_SystemTask, "SystemTask", PH_EXPOS_CFG_RTOS_SYS_TASK_STACK_SIZE, NULL,
        PH_EXPOS_CFG_RTOS_SYS_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    phExPos_MonitorEnableTxLdo();

    return PH_ERR_SUCCESS;
}

/** phExPos entry point for RTOS based. */
int main(void)
{
	phFlashBoot_Main();
    phhalTimer_Init();
    phOsal_Init();

#ifdef PHFL_ENABLE_STANDBY
    /** Configure GPIO 8 for SWD download mode. */
    phhalPcr_ConfigInput(PH_EXPOS_CFG_SWD_DNLD_PIN, PH_ENABLE, PH_DISABLE, PH_DISABLE);
    phhalPcr_ConfigPuPd(PH_EXPOS_CFG_SWD_DNLD_PIN, PH_DISABLE, PH_ENABLE);

    /** Wait infinitely if Download pin is signaled to support download through SWD before going to app. */
    while(PH_REG_TEST_BITN_SC(PCR_PADIN_REG, PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXPOS_CFG_SWD_DNLD_PIN))
    {
        ;
    }
#endif

    LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    /** Create & initialize system task */
    phExPos_SystemTaskInit();

    /** Check the Boot reason and post the message to system task OR perform the corresponding boot operation. */
    phExPos_BootHandler();

    /** Start the tasks and timer running. */
    phRtos_Start();
    return 0;
}

/**
 * System wake up handler.
 *
 * @param eBootReason : Boot reason to be handled.
 */
static void phExPos_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
{
#ifdef PHFL_ENABLE_STANDBY
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason;
    phhalPcr_WakeUpConfig_t phhalPcr_WakeUpConfig;
    /* Wakeup's configured from EEPROM */
    phhalPcr_WakeUpConfig.bEnableHIFWakeup = gpkphCfg_EE_HW_WakeUpConfig->bEnableHIFWakeup;
    phhalPcr_WakeUpConfig.bI2CAddr = gpkphCfg_EE_HW_WakeUpConfig->bI2CAddr;
    phhalPcr_WakeUpConfig.bWakeUpCtPr = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpCtPr;
    phhalPcr_WakeUpConfig.bWakeUpGpio = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpGpio;
    phhalPcr_WakeUpConfig.bWakeUpIntAux = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpIntAux;
    phhalPcr_WakeUpConfig.bWakeUpPvddLim = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpPvddLim;
    phhalPcr_WakeUpConfig.bWakeUpRfLdt = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpRfLdt;
    phhalPcr_WakeUpConfig.bWakeUpTimer = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpTimer;
    phhalPcr_WakeUpConfig.bWakeUpTvddMon = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpTvddMon;
    phhalPcr_WakeUpConfig.wWakeUpTimerVal = gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal;

    /* Wakeup's not required to configure */
    phhalPcr_WakeUpConfig.bSmBusAddrResp = 0x00;
    phhalPcr_WakeUpConfig.bWakeUpTemp = 0x00;
    phhalPcr_WakeUpConfig.blWakeupVbusLow = 0x00;
#endif
    LOG_U32("Wkup",eBootReason);
    phExPos_Log_BootReason(eBootReason);

    switch(eBootReason)
    {
    /** Normal boot scenario, set wakeups and goto Power down mode. */
    case E_STARTUP_POR  :
    case E_SOFT_RESET   :
    case E_ACTIVE_HPD   :
    case E_TEMP_SENSOR0 :     /** Temp became normal. */
    case E_TEMP_SENSOR1 :     /** Temp became normal. */
    case E_HIF_RESET    :
    case E_WATCH_DOG    :
    case E_NO_PVDD      :

#ifdef PHFL_ENABLE_STANDBY

        while(1)
        {
            phLED_SetPattern(gkphLED_StandBy);
            /** All the wakeup configurations are set by default in the EEPROM. */
            phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
            /** It is not expected to reach here, if so then wait for a while and try Standby. */
            phRtos_TaskDelay(SYS_TASK_FREQUENCY_MS);
        }

#else

        /** Initialize the Tasks based on its Module configurations. */
        phExPos_CtTaskInit(gphExPos_SysTask_Queue);
        phExPos_ClifTaskInit(E_BOOT_NONE, NULL);
        phExPos_UtilsPOS_Init();
        phExPos_SPIHIFGPIOLineIdle();
#endif
        break;

        /** CLIF related wakeup reasons */
    case E_RFLDT_BOOT :
        /** Starts the CLIF task which executes the CLIF main example */
        phExPos_ClifTaskInit(eBootReason, gphExPos_SysTask_Queue);
        phExPos_CtTaskInit(gphExPos_SysTask_Queue);
        break;

    case E_WUC_CNT :
        /** Check the CT card presence. */
        {
            phhalCt_InitParam_t  sInitParams;
            sInitParams.bAutoCTDeactivationEnable= 0;
            sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
            sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
            sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

            PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);

            if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
            {
                /** CT card present. perform CT operations. */
                /** create the required CT queues and task. */
                phExPos_CtTaskInit(gphExPos_SysTask_Queue);
                /** start the CT application. */
                phExPos_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExPos_SysTask_Queue);
            }
            else
            {
                /** perform Clif operations. */
                /** Starts the CLIF task which executes the CLIF main example */
                phExPos_ClifTaskInit(eBootReason, gphExPos_SysTask_Queue);
            }
        }
        break;

        /** contact card presence. */
    case E_CT_PRESENCE :
        /** create the required CT queues and task. */
        phExPos_CtTaskInit(gphExPos_SysTask_Queue);
        phExPos_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExPos_SysTask_Queue);
        break;

        /** Host based system. */
    case E_I2C_ADDR:
    case E_HSU_INT :
    case E_SPI_ADDR:
        PH_USER_ASSERT(0);
        break;

        /** As of now nothing to deal with the below WakeUp reasons.
         * May be required for future. */
    case E_SMBUS_ADDR:
    case E_SWP_DET:
    case E_TVDD_MON:
    case E_GPIO_INT:
    case E_DFT_RESET:
    case E_INT_AUX_BOOT:
        PH_USER_ASSERT(0);
        break;

    default:
        PH_USER_ASSERT(0); /** It is not expected to reach here. */
    }
}

/**
 * PCR/PMU event handler.
 *
 * @param ePcrEvent : PCR/PMU system events.
 */
static void phExPos_PcrHandler(phhalPcr_SystemEventType_t ePcrEvent)
{
    switch(ePcrEvent)
    {
    case E_SYSTEM_EVENT_WAITING          :
        break;
    case E_SYSTEM_EVENT_RFLD_ACT         :
        break;
    case E_SYSTEM_EVENT_VBUSP_MON_LOW    :
        break;
    case E_SYSTEM_EVENT_VBUSP_MON_HIGH   :
        break;
    case E_SYSTEM_EVENT_PVDD_CURLIM_ACT  :
        break;
    case E_SYSTEM_EVENT_DCDC_OVERLOAD:
        break;
    case E_SYSTEM_EVENT_PVDD_THRESHOLD_LOW:
        break;
    case E_SYSTEM_EVENT_PVDD_THRESHOLD_HIGH:
        break;
    case E_SYSTEM_EVENT_TXLDO_OVERCURRENT:
        break;
    case E_SYSTEM_EVENT_VCC_ILIM_ACT:
        break;
    case E_SYSTEM_EVENT_USB_RESUME:
        break;
        /**
    case E_PH_HALPCR_EVENT_SUSPEND_INT      :
        break;
        **/
    default:
        PH_USER_ASSERT(0); /** It is not expected to reach here. */
    }
}

/**
 * Example application main task, handles application sequencing.
 *
 * @param pvParams : Unused.
 */
void phExPos_SystemTask(void *pvParams)
{
    phFlashBoot_Event_SysMsg_t sSysMsg;

#ifdef PHFL_ENABLE_STANDBY
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason;
    phhalPcr_WakeUpConfig_t phhalPcr_WakeUpConfig;
    /* Wakeup's configured from EEPROM */
    phhalPcr_WakeUpConfig.bEnableHIFWakeup = gpkphCfg_EE_HW_WakeUpConfig->bEnableHIFWakeup;
    phhalPcr_WakeUpConfig.bI2CAddr = gpkphCfg_EE_HW_WakeUpConfig->bI2CAddr;
    phhalPcr_WakeUpConfig.bWakeUpCtPr = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpCtPr;
    phhalPcr_WakeUpConfig.bWakeUpGpio = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpGpio;
    phhalPcr_WakeUpConfig.bWakeUpIntAux = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpIntAux;
    phhalPcr_WakeUpConfig.bWakeUpPvddLim = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpPvddLim;
    phhalPcr_WakeUpConfig.bWakeUpRfLdt = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpRfLdt;
    phhalPcr_WakeUpConfig.bWakeUpTimer = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpTimer;
    phhalPcr_WakeUpConfig.bWakeUpTvddMon = gpkphCfg_EE_HW_WakeUpConfig->bWakeUpTvddMon;
    phhalPcr_WakeUpConfig.wWakeUpTimerVal = gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal;

    /* Wakeup's not required to configure */
    phhalPcr_WakeUpConfig.bSmBusAddrResp = 0x00;
    phhalPcr_WakeUpConfig.bWakeUpTemp = 0x00;
    phhalPcr_WakeUpConfig.blWakeupVbusLow = 0x00;
#else
    bool bCtCardPresence = false;
#endif /* PHFL_ENABLE_STANDBY */

    /** Enable the LED timer for 100 milli secs in free running mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);


    while(1)
    {
        phRtos_QueueReceive( gphExPos_SysTask_Queue, &sSysMsg, portMAX_DELAY );
        LOG_TXT_D("=>System Task\n");
        phExPos_Log_FlashBootEventIds(sSysMsg.eSrcId);

        switch(sSysMsg.eSrcId)
        {
        /** Message from the flash boot. */
        case E_PH_BOOT:
            phExPos_WakeUpHandler((phhalPcr_BootReason_t)sSysMsg.dwMsg[0]);
            break;

            /** Message from the CLIF task */
        case E_PH_CLIF:
            switch(sSysMsg.dwMsg[0])
            {
            /** Since CLIF application completed the Sys task either go to standby, start poll timer */
            case (uint32_t)E_PHEXMAIN_CLIF_RSP_END:
#ifdef PHFL_ENABLE_STANDBY
                while(1)
                {
                    phLED_SetPattern(gkphLED_StandBy);
                    /** All the wakeup configurations are set by default in the EEPROM. */
                    phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                        &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
                    /** It is not expected to reach here, if so then wait for a while and try Standby. */
                    phRtos_TaskDelay(SYS_TASK_FREQUENCY_MS);
                }

#else
            /** Make sure that CT card is not present and then start the timer. */
            if(bCtCardPresence == false){
                phhalTimer_Start(gpphExPos_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
            }
#endif
            break;

            default:
                PH_USER_ASSERT(0); /** It is not expected to reach here. */
            }

            break;

            case E_PH_PMU:
                /** Handle the message posts from the PMU module. */
                /** phExPos_PmuHandler((phhalPmu_EventType_t) sSysMsg.dwMsg[0]); */
                break;

            case E_PH_PCR:
                /** Handle the message posts from the PCR module. */
                phExPos_PcrHandler((phhalPcr_SystemEventType_t) sSysMsg.dwMsg[0]);
                break;

            case E_PH_TIMER:
                /** Post the polling message to clif task. */
                phExPos_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXMAIN_CLIF_CMD_START,
                        gphExPos_SysTask_Queue);
                break;

                /** Handle the message posts from the CT. */
            case E_PH_CT:
                switch((phhalCt_ErrorCodes_t)sSysMsg.dwMsg[0])
                {
                /** card inserted in the main slot. */
                case PH_ERR_CT_MAIN_CARD_PRESENT:

#ifndef PHFL_ENABLE_STANDBY
                    bCtCardPresence = true;
                    /** STOP the polling timer */
                    phhalTimer_Stop(gpphExPos_Clif_PollTimer);
#endif
                    /** start the CT application. */
                    phExPos_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExPos_SysTask_Queue);
                    break;
                    /** card removed from the main slot. */
                case PH_ERR_CT_MAIN_CARD_ABSENT:

#ifdef PHFL_ENABLE_STANDBY
                    while(1)
                    {
                        phLED_SetPattern(gkphLED_StandBy);
                        /** All the wakeup configurations are set by default in the EEPROM. */
                        phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                            &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
                        /** It is not expected to reach here, if so then wait for a while and try Standby. */
                        phRtos_TaskDelay(SYS_TASK_FREQUENCY_MS);
                    }
#else
                    LOG_TXT("Contact card removed\n");
                    bCtCardPresence = false;
                    /** Start the timer again in single shot */
                    if ( NULL != gpphExPos_Clif_PollTimer) {
                        phhalTimer_Start(gpphExPos_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
                    }
                    gphExPos_Clif_PollLoopLOGCount = 0;
#endif
                    break;
                default:
                    PH_USER_ASSERT(1); /** It is not expected to reach here. */
                }
                break;
                /** Handle the message posts from the CT Task. */
                case E_PH_CT_TASK:
                    break;

                    /** As of now nothing to deal with the below modules.
                     * May be required for future. */
                case E_PH_HIF:
                    PH_USER_ASSERT(0);
                    break;

                default:
                    PH_USER_ASSERT(0); /** It is not expected to reach here. */
        }

    }
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static void phExPos_MonitorEnableTxLdo(void)
{
    if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXPOS_CFG_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXPOS_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXPOS_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

#endif /* PHFL_HALAPI_WITH_RTOS */
