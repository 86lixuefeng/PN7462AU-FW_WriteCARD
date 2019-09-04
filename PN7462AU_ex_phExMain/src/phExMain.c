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
 * phExMain.c:  Main application file and entry point of phExMain. Contains core logic part of the phExMain application.
 *              To know about phExMain application refer to 5.1 section of PN7462AU_FW_UM user manual .
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-12-01 14:26:44 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18769 $ (v05.19.00)
 */


/** *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"

#ifdef PHFL_HALAPI_WITH_RTOS

#include "phRtos.h"
#include "ph_Log.h"
#include "phacDiscLoop.h"
#include "phCfg_EE.h"
#include "phExMain.h"
#include "phExMain_Cfg.h"
#include "phExMain_Clif.h"
#include "phExMain_Ct.h"
#include "phExMain_Log.h"
#include "phFlashBoot.h"
#include "phFlashBoot_Event.h"
#include "phhalCt.h"
#include "phhalGpio.h"
#include "phhalPmu.h"
#include "phhalRf.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "phUser.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_pmu.h"
#include "string.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define SYS_TASK_FREQUENCY_MS                     ( 20 / portTICK_PERIOD_MS )
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/** RTOS message queue handlers */
static phRtos_QueueHandle_t gphExMain_SysTask_Queue = NULL;
static phRtos_QueueHandle_t gphExMain_Ct_Queue = NULL;
static phRtos_QueueHandle_t gphExMain_Clif_Queue = NULL;
PH_NOINIT static phhalTimer_Timers_t *pLedTimer;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExMain_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                             phExMain_Clif_MsgId_t eClifCmd, void *pDes);
static void phExMain_CtTaskInit(void *pSysMsgQueue);
static void phExMain_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
static void phExMain_CtPostMsg(phExMain_EventCtCmds_t eCtCmd, void *pDes);
static void phExMain_CtCallBack(uint32_t dwIRQMessage);
static void phExMain_ClifTask(void *pvParams);
static void phExMain_CtTask(void *pvParams);

static void phExMain_PmuCallBack(uint32_t dwIRQMessage);
static void phExMain_PcrCallBack(uint32_t dwIRQMessage);

#ifndef PHFL_ENABLE_STANDBY
static void phExMain_RfCallBackFunc(uint32_t dwInterrupts);
#endif /* PHFL_ENABLE_STANDBY */

static void phExMain_TxLdoMonitorEnable(void);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */


#ifndef PHFL_ENABLE_STANDBY
/** Timer timeout function to kick start discovery loop polling during NON STANDBY case. */
void phExMain_TimerCallBackFunc(void)
{
    phExMain_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXMAIN_CLIF_CMD_START, gphExMain_SysTask_Queue);
}

static void phExMain_RfCallBackFunc(uint32_t dwInterrupts)
{
    phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_DISABLE);

    if(dwInterrupts & CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK)
    {
        phExMain_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_LISTEN, E_PHEXMAIN_CLIF_CMD_START, gphExMain_SysTask_Queue);
    }
    else
    {
        /* Do Nothing */
        ;
    }
    phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_ENABLE);
}
#endif

/**
 * Posts boot reason to the System/app Task.
 *
 * @param eBootReason : Boot reason to be posted to the System/app Task.
 * @return Status of the posting operation.
 */
static phStatus_t phExMain_PostBootReason(phhalPcr_BootReason_t eBootReason)
{
    phFlashBoot_Event_SysMsg_t eSysMsg;

    eSysMsg.dwMsg[0] = (uint32_t)eBootReason;
    eSysMsg.eSrcId = E_PH_BOOT;
    eSysMsg.pvDes = gphExMain_SysTask_Queue;

    /** Post boot reason to system task queue. */
    if(phRtos_QueueSend( gphExMain_SysTask_Queue,  &eSysMsg, PHRTOS_MAX_DELAY)!= phRtos_Success ){
        return PH_ERR_OPERATION_TIMEDOUT;
    }
    return PH_ERR_SUCCESS;
}

/**
 * Wake up Boot Reason handler.
 */
static phStatus_t phExMain_BootHandler(void)
{
    phhalPcr_BootReason_t eBootReason;

    /** Get & clear the boot reason. */
    eBootReason = phFlashBoot_GetBtRns();
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /** Post the Boot reason to the system task. Hang here if the posting is failed. */
    PH_USER_ASSERT(phExMain_PostBootReason(eBootReason) == PH_ERR_SUCCESS);

    return PH_ERR_SUCCESS;
}

/**
 * Initializes the System/app Task and its queue.
 *
 * @return Status of the System/app's Task & queue initialization.
 */
static phStatus_t phExMain_SystemTaskInit(void)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Create the System/app queue to which PMU/PCR ISR and other tasks post messages */
    gphExMain_SysTask_Queue = phRtos_QueueCreate(PHFL_SYS_TASK_Q_LEN, sizeof(phFlashBoot_Event_SysMsg_t));

    PH_USER_ASSERT(gphExMain_SysTask_Queue);

    /** Create system task */
    phRtos_TaskCreate( &phExMain_SystemTask, "SystemTask", PH_EXMAIN_CFG_RTOS_SYS_TASK_STACK_SIZE, NULL,
        PH_EXMAIN_CFG_RTOS_SYS_TASK_PRIORITY, &pHandle );

    PH_USER_ASSERT(pHandle);

    return PH_ERR_SUCCESS;
}

/** phExMain entry point for RTOS based. */
int main(void)
{
	phFlashBoot_Main();
    phhalTimer_Init();
    phOsal_Init();
	phExMain_Clif_HalInit();


#if defined(PHFL_ENABLE_STANDBY) && defined (PH_EXMAIN_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE)
    /** Configure GPIO #PH_EXMAIN_CFG_SWD_DNLD_PIN for SWD download mode. */
    phhalPcr_ConfigInput(PH_EXMAIN_CFG_SWD_DNLD_PIN, PH_ENABLE, PH_DISABLE, PH_DISABLE, PH_DISABLE,
            PH_DISABLE, PH_DISABLE);
    phhalPcr_ConfigPuPd(PH_EXMAIN_CFG_SWD_DNLD_PIN, PH_DISABLE, PH_ENABLE);

    /** Wait infinitely if GPIO #PH_EXMAIN_CFG_SWD_DNLD_PIN is equal to
     *  #PH_EXMAIN_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE to support download through
     *  SWD before going to StandBy. See the description of #PH_EXMAIN_CFG_SWD_DNLD_PIN
     *  for more information.
     */
    while( (PH_EXMAIN_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE << (PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXMAIN_CFG_SWD_DNLD_PIN))
        == PH_REG_GET_FIELD_NS_SC(PCR_PADIN_REG, 1UL << (PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXMAIN_CFG_SWD_DNLD_PIN)) )
    {
        ;

    }
#endif

    LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    /** Create & initialize system task */
    phExMain_SystemTaskInit();

    /** Check the Boot reason and post the message to system task OR perform the corresponding boot operation. */
    phExMain_BootHandler();

    /** Start the tasks and timer running. */
    phRtos_Start();

    return 0;
}

/**
 * System wake up handler.
 *
 * @param eBootReason : Boot reason to be handled.
 */
static void phExMain_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
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
    phExMain_Log_BootReason(eBootReason);
    PH_USER_ASSERT(sizeof(phhalPcr_RfLdtParams_t) == sizeof(phCfg_EE_HW_RfLdtParams_t));

    switch(eBootReason)
    {
    /** Normal boot scenario, set wakeups and goto Power down mode. */
    case E_STARTUP_POR  :
    case E_SOFT_RESET   :
    case E_ACTIVE_HPD   :
    case E_TEMP_SENSOR0 :     /* Temp became normal. */
    case E_TEMP_SENSOR1 :     /* Temp became normal. */
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

        /** Initialize the clif and CT Tasks. */
        phExMain_CtTaskInit(gphExMain_SysTask_Queue);
        phExMain_ClifTaskInit(E_BOOT_NONE, NULL);
#endif
        break;

    case E_RFLDT_BOOT :
        /** Initialize the clif and CT Tasks with wakeup reason as RFLD. */
        phExMain_ClifTaskInit(eBootReason, gphExMain_SysTask_Queue);
        phExMain_CtTaskInit(gphExMain_SysTask_Queue);
        break;

    case E_WUC_CNT :
        /** wakeUp due to wakeUp counter. If CT card present then start CT app else
         * perform Clif operations.
         *  */
        {
            phhalCt_InitParam_t  sInitParams;
            sInitParams.bAutoCTDeactivationEnable= 0;
            sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
            sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
            sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;
            phLED_SetStatus(LED_B | LED_R | LED_Y | LED_G );

            PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);
            (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);

            if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
            {
                phExMain_CtTaskInit(gphExMain_SysTask_Queue);
                /** perform CT operations. */
                phExMain_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExMain_SysTask_Queue);
            }
            else
            {
                /** perform Clif operations. */
                phExMain_ClifTaskInit(eBootReason, gphExMain_SysTask_Queue);
            }
        }
        break;

    case E_CT_PRESENCE :
        /** WakeUp due to CT card presence, perform CT operations. */
        phExMain_CtTaskInit(gphExMain_SysTask_Queue);
        phExMain_CtPostMsg(E_PH_PERFORM_CARD_ACT, gphExMain_SysTask_Queue);
        break;

    default:
    /** Invalid wakeUp, it is not expected to reach here. */
        PH_USER_ASSERT(0);
    }
}

/**
 * Performs callback function for the PCR IRQ's.
 *
 * @param dwIRQMessage :  IRQ from the PCR
 */
static void phExMain_PcrCallBack(uint32_t dwIRQMessage)
{
    /* No functionality */
}

/**
 * Performs callback function for the PMU IRQ's.
 *
 * @param dwIRQMessage :  IRQ from the PMU
 */
static void phExMain_PmuCallBack(uint32_t dwIRQMessage)
{
    /* No functionality */
}


/**
 * Example application main task, handles application sequencing.
 *
 * @param pvParams : Unused.
 */
void phExMain_SystemTask(void *pvParams)
{
    phFlashBoot_Event_SysMsg_t sSysMsg;
    uint32_t dwPcrIRQMessage = (PCR_INT_STATUS_REG_VBUSP_MON_LOW_INT_STATUS_MASK |
                                PCR_INT_STATUS_REG_VBUSP_MON_HIGH_INT_STATUS_MASK |
                                PCR_INT_STATUS_REG_PVDD_CURLIM_ACT_INT_STATUS_MASK);
    uint32_t dwPmuIRQMessage = (PMU_INTERRUPT_STATUS_REG_VCC_ILIM_ACT_IRQ_STATUS_MASK |
                                PMU_INTERRUPT_STATUS_REG_DCDC_OVERLOAD_IRQ_STATUS_MASK |
                                PMU_INTERRUPT_STATUS_REG_PVDD_IRQ_STATUS_MASK |
                                PMU_INTERRUPT_STATUS_REG_TXLDO_OVERCURRENT_IRQ_STATUS_MASK);

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
#endif /** PHFL_ENABLE_STANDBY */

    /** Enable the LED timer for 300 milli secs in free running mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

    /* Register the call backs for PCR and PMU modules */
    phhalPcr_RegCallBack(&phExMain_PcrCallBack,dwPcrIRQMessage);
    phhalPmu_RegCallBack(&phExMain_PmuCallBack,dwPmuIRQMessage);
    phExMain_TxLdoMonitorEnable();

    while(1)
    {
        phRtos_QueueReceive( gphExMain_SysTask_Queue, &sSysMsg, portMAX_DELAY );
        switch(sSysMsg.eSrcId)
        {
        /** Message from the flash boot. */
        case E_PH_BOOT:
            phExMain_WakeUpHandler((phhalPcr_BootReason_t)sSysMsg.dwMsg[0]);
            break;

            /** Message from the CLIF task */
        case E_PH_CLIF:
            if(sSysMsg.dwMsg[0] == E_PHEXMAIN_CLIF_RSP_END)
            {
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
                phhalTimer_Start(gpphExMain_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
#endif
            }
            break;

        /** Handle the message posts from the CT. */
        case E_PH_CT:
                if((phhalCt_ErrorCodes_t)sSysMsg.dwMsg[0] == PH_ERR_CT_MAIN_CARD_ABSENT)
                {
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
#endif
                }
                break;
        /* In both the cases go to standby if enabled or WFI in case of non standby case */
        case E_PH_PMU:
        case E_PH_PCR:

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
                /* Simply WFI here since any error from the PMU/PCR should be debugged */
                PH_USER_ASSERT(0);
#endif
                break;

                default:
                    PH_USER_ASSERT(0); /** It is not expected to reach here. */
        }

    }
}

/**
 * Performs message posting to the Clif task.
 *
 * @param eDiscLoopEntry : Discovery loop entry point.
 * @param eClifCmd : Clif command.
 * @param pDes : Destination message queue handler.
 */
static void phExMain_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                             phExMain_Clif_MsgId_t eClifCmd, void *pDes)
{
    phExMain_Clif_Msg_t sClifMsg;

    sClifMsg.sClifConfig.eDiscLoopEntry = eDiscLoopEntry;

    sClifMsg.eSrcId = E_PH_CLIF;
    sClifMsg.pvDes = pDes;
    sClifMsg.eClifMsgId = eClifCmd;
    /** wait indefinitely until post boot clif task is success. */
    PH_USER_ASSERT(phRtos_QueueSend( gphExMain_Clif_Queue, &sClifMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Initializes Clif Queue & task, posts bootUp message in case of Standby enabled.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
static void phExMain_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Create the queue to which System Task can post messages */
    gphExMain_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExMain_Clif_Msg_t));
    PH_USER_ASSERT(gphExMain_Clif_Queue);

#ifdef PHFL_ENABLE_STANDBY
    /** Depending on the wakeup reason, post CLIF message to enter poll/listen mode */
    if(eBootReason == E_RFLDT_BOOT)
    {
        phExMain_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_LISTEN, E_PHEXMAIN_CLIF_CMD_START, pDes);
    }
    else if(eBootReason == E_WUC_CNT)
    {
        phExMain_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXMAIN_CLIF_CMD_START, pDes);
    }
    else
    {
        /** If no boot reason, expect Poll Timer(T0) to expire and post message to Clif queue */
        ;
    }
#endif /** PHFL_ENABLE_STANDBY */

    /** Create clif task with \ref PH_EXMAIN_CFG_RTOS_CLIF_TASK_PRIORITY priority. */
    pHandle = NULL;
    phRtos_TaskCreate(&phExMain_ClifTask, "ClifTask", PH_EXMAIN_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXMAIN_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);
}

/**
 * Clif Task which performs all the Contact less interface operations.
 *
 * @param pvParams : Unused.
 */
static void phExMain_ClifTask(void *pvParams)
{
    phExMain_Clif_Msg_t sClifMsg;
    uint16_t wStatus;
    uint8_t bAts[64] ;

    phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
    phacDiscLoop_Sw_DataParams_t sDiscLoopParam;

#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif

    /* Configure the Test Bus */
    wStatus = phExMainClif_TestBus();
    if(wStatus != PH_ERR_SUCCESS)
    {
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

    /* Configure the discovery loop parameters, for polling/listen/active */
    psDiscLoopParams = &sDiscLoopParam;
    wStatus = phExMainClif_DiscLoopConfig(psDiscLoopParams);
    if(wStatus != PH_ERR_SUCCESS)
    {
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

    /* Initialize the parameters for polling/listen/active */
    phExMainClif_DiscLoopParamInit(psDiscLoopParams,bAts);

    /**
     *  hardware specific API to initialize the Hal and Pal.
     *  Initializes the psDiscLoopParams->pHalDataParams & psDiscLoopParams->pPalDataParams variables
     *  based on Poll and Listen bit map configuration.
     */
    if(phExMainClif_PalInit(psDiscLoopParams) != PH_ERR_SUCCESS)
    {
        phExMainClif_DeInit(psDiscLoopParams->pHalDataParams);
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

    gphExMain_Clif_PollLoopLOGCount = 0;

#ifndef PHFL_ENABLE_STANDBY
    /*Standby is disabled, enable the poll timer, configure and start running in single shot mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,
        &gpphExMain_Clif_PollTimer) == PH_ERR_SUCCESS);

    phhalTimer_Configure(gpphExMain_Clif_PollTimer, (uint32_t)gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal,
        &phExMain_TimerCallBackFunc);

    phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_ENABLE);
    phhalRf_RegCallBack(&phExMain_RfCallBackFunc, CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK);
#endif
    while(1)
    {
#ifndef PHFL_ENABLE_STANDBY
        phhalTimer_Start(gpphExMain_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
#endif
        /** check for the Clif profile and startup sequence. */
        phRtos_QueueReceive( gphExMain_Clif_Queue, (void *)&sClifMsg, portMAX_DELAY );
        switch(sClifMsg.eClifMsgId)
        {
        case E_PHEXMAIN_CLIF_CMD_START:

            psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
#ifdef PHFL_ENABLE_STANDBY
            sSysTaskMsg.dwMsg[1] =
#endif
            (uint32_t)phExMain_ClifMain(psDiscLoopParams, sClifMsg.sClifConfig.eDiscLoopEntry);

#ifdef PHFL_ENABLE_STANDBY
            /** Intimate the Contact less operations are done. */
            sSysTaskMsg.eSrcId = E_PH_CLIF;
            sSysTaskMsg.pvDes = gphExMain_Clif_Queue;
            sSysTaskMsg.dwMsg[0] = E_PHEXMAIN_CLIF_RSP_END;
            (void)phRtos_QueueSend( sClifMsg.pvDes, (void *)&sSysTaskMsg, portMAX_DELAY);
#endif /** PHFL_ENABLE_STANDBY */
            break;
#ifndef PHFL_ENABLE_STANDBY
        case E_PHEXMAIN_CLIF_CMD_STOP:
            /** Nothing to do */
            break;
#endif
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
 * @param eCtCmd : CT commands \ref phExMain_EventCtCmds_t
 * @param pDes : Destination message queue handler.
 */
static void phExMain_CtPostMsg(phExMain_EventCtCmds_t eCtCmd, void *pDes)
{
    phExMain_EventCtMsg_t sCtMsg;
    /** wait indefinitely until post to ct task is success. */
    sCtMsg.eSrcId = E_PH_SYS;
    sCtMsg.pvDes = pDes;
    sCtMsg.eCommands = eCtCmd;
    sCtMsg.dwMsg = PH_ERR_CT_MAIN_CARD_PRESENT;
    PH_USER_ASSERT(phRtos_QueueSend( gphExMain_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Performs callback function for the CT IRQ's.
 *
 * @param dwIRQMessage : Presence IRQ from the CT
 */
static void phExMain_CtCallBack(uint32_t dwIRQMessage)
{
    phExMain_EventCtMsg_t sCtMsg;
    phStatus_t status = 0x00;

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

    PH_USER_ASSERT(phRtos_QueueSend( gphExMain_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Initializes CT Queue & task.
 *
 * @param pSysMsgQueue : Destination system message queue handler.
 * @return Status of the operation.
 */
static void phExMain_CtTaskInit(void *pSysMsgQueue)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** create the Clif Task and Clif queue and post the msg. */
    gphExMain_Ct_Queue  = phRtos_QueueCreate(PHFL_EVENT_CT_Q_LEN, sizeof(phExMain_EventCtMsg_t));
    PH_USER_ASSERT(gphExMain_Ct_Queue);

    /** Create the ct task. */
    pHandle = NULL;
    phRtos_TaskCreate( &phExMain_CtTask, "CtTask", PH_EXMAIN_CFG_RTOS_CT_TASK_STACK_SIZE, gphExMain_Ct_Queue,
        PH_EXMAIN_CFG_RTOS_CT_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);
}

/**
 * CT Task which performs all the Contact interface operations.
 *
 * @param pvParams : Handler to the System message queue.
 */
static void phExMain_CtTask(void *pvParams)
{
    phStatus_t eStatus;
    phExMain_EventCtMsg_t sCtQueue;
    uint32_t dwIRQMessage = CT_USR2_REG_PRESL_MASK;

#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif
    phhalCt_InitParam_t  sInitParams;
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    /*7816 profile*/
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

    /** Initialize the CT PAL layer with configuration as pull down enabled and normally open connector type */
    PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);
    (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);

    /* Register the call back */
    phhalCt_RegCallBack(&phExMain_CtCallBack,dwIRQMessage);

    while(1)
    {
        phRtos_QueueReceive( gphExMain_Ct_Queue, (void *)&sCtQueue, portMAX_DELAY );

        switch((phhalCt_ErrorCodes_t)sCtQueue.dwMsg)
        {
        /** Card present, start CT example app. */
            case PH_ERR_CT_MAIN_CARD_PRESENT:
                LOG_TXT("CT card detected\n");
                phLED_SetPattern(gkphLED_Ct_Detect);
                /** LOG_TXT("Starting the contact card example application"); */
                eStatus = phExMain_Ct_Example_App();
                if(PH_ERR_SUCCESS != eStatus)
                {
                    phLED_SetPattern(gkphLED_Ct_Fail);
                }
                else
                {
                    phLED_SetPattern(gkphLED_Ct_Pass);
                }

                gphExMain_Clif_PollLoopLOGCount = PH_EXMAIN_CLIF_POLLING_ROLLOVER_COUNT;
            break;

            /** Card removed, post message to system task. */
            case PH_ERR_CT_MAIN_CARD_ABSENT:
                LOG_TXT("CT card removed.\n");

#ifdef PHFL_ENABLE_STANDBY
                /** Wait indefinitely until post to System task is success. */
                sSysTaskMsg.eSrcId = E_PH_CT;
                sSysTaskMsg.pvDes = gphExMain_SysTask_Queue;
                sSysTaskMsg.dwMsg[0] = PH_ERR_CT_MAIN_CARD_ABSENT;
                (void)phRtos_QueueSend( gphExMain_SysTask_Queue, (void *)&sSysTaskMsg, portMAX_DELAY);
#endif
                gphExMain_Clif_PollLoopLOGCount = PH_EXMAIN_CLIF_POLLING_ROLLOVER_COUNT;
            break;
            default:
                gphExMain_Clif_PollLoopLOGCount = 0;
                /** It is not expected to reach here. */
                PH_USER_ASSERT(0);
        }
    }
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static void phExMain_TxLdoMonitorEnable(void)
{
    /* Check the configured TxLdo is External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXMAIN_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_EXTERNAL_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXMAIN_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXMAIN_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}
#endif
