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
 * phExDoorAccess.c:  Main application file and entry point of phExDoorAccessNonExportControl. Contains core logic part of the phExDoorAccessNonExportControl application.
 *              To know about phExDoorAccess application refer to 5.1 section of PN7462AU_FW_UM user manual .
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
#include <phExDoorAccess.h>
#include <phExDoorAccess_Clif.h>
#include "phacDiscLoop.h"
#include <phExDoorAccess_Poll.h>
#include "phFlashBoot_Event.h"
#include "phhalRf.h"
#include "phhalNvic.h"
#include "phCfg_EE.h"
#include <phExDoorAccess_Cfg.h>
#include "string.h"
#include "phUser.h"
#include "phhalTimer.h"
#include "phFlashBoot.h"
#include "phhalCt.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_pmu.h"
#include "phExDoorAccess_Log.h"
#include "phhalGpio.h"
#include "ph_Log.h"
#include "phLED.h"
#include "phhalPmu.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "portmacro.h"

#ifdef PHFL_ENABLE_LLCPSNEP
#include "phpalI18092mPI.h"
#include "phOsal.h"
#include "phlnLlcp.h"
#include "phnpSnep.h"
#include "phExDoorAccessLLCP_Client.h"
#include "PN7462AU/PN7462AU_clif.h"
#endif
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define SYS_TASK_FREQUENCY_MS                     ( 20 / portTICK_PERIOD_MS )
#define PH_ENABLE                       0x0001U /**< Enabled. */
#define PH_DISABLE                      0x0000U /**< Disabled. */
#define PH_EXDOORACCESS_TXLDO_MONITOR_ENABLE   0X01
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/** RTOS message queue handlers */
PH_NOINIT static phRtos_QueueHandle_t gphExDoorAccess_SysTask_Queue = NULL;
PH_NOINIT static phRtos_QueueHandle_t gphExDoorAccess_Clif_Queue = NULL;
phhalTimer_Timers_t *pLedTimer;
phhalTimer_Timers_t *pHSUTxTimer;

extern phlnLlcp_Sw_DataParams_t        slnLlcp;                    /* LLCP component holder */
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExDoorAccess_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                             phExDoorAccess_Clif_MsgId_t eClifCmd, void *pDes);
static phStatus_t phExDoorAccess_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
static void phExDoorAccess_ClifTask(void *pvParams);

static void phExDoorAccess_PmuCallBack(uint32_t dwIRQMessage);
static void phExDoorAccess_PcrCallBack(uint32_t dwIRQMessage);

#ifndef PHFL_ENABLE_STANDBY
static void phExDoorAccess_RfCallBackFunc(uint32_t dwInterrupts);
#endif /* PHFL_ENABLE_STANDBY */

static void phExDoorAccess_MonitorEnableTxLdo(void);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */


#ifndef PHFL_ENABLE_STANDBY
/** Timer timeout function to kick start discovery loop polling during NON STANDBY case. */
void phExDoorAccess_TimerCallBackFunc(void)
{
    phExDoorAccess_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXDOORACCESS_CLIF_CMD_START, gphExDoorAccess_SysTask_Queue);
}

static void phExDoorAccess_RfCallBackFunc(uint32_t dwInterrupts)
{
    phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_DISABLE);

    if(dwInterrupts & CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK)
    {
        /* Do Nothing */
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
static phStatus_t phExDoorAccess_PostBootReason(phhalPcr_BootReason_t eBootReason)
{
    phFlashBoot_Event_SysMsg_t eSysMsg;

    eSysMsg.dwMsg[0] = (uint32_t)eBootReason;
    eSysMsg.eSrcId = E_PH_BOOT;
    eSysMsg.pvDes = gphExDoorAccess_SysTask_Queue;

    /** Post boot reason to system task queue. */
    if(phRtos_QueueSend( gphExDoorAccess_SysTask_Queue,  &eSysMsg, PHRTOS_MAX_DELAY)!= phRtos_Success ){
        return PH_ERR_OPERATION_TIMEDOUT;
    }
    return PH_ERR_SUCCESS;
}

/**
 * Wake up Boot Reason handler.
 */
static phStatus_t phExDoorAccess_BootHandler(void)
{
    phhalPcr_BootReason_t eBootReason;

    /** Get & clear the boot reason. */
    eBootReason = phFlashBoot_GetBtRns();
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /** Post the Boot reason to the system task. Hang here if the posting is failed. */
    PH_USER_ASSERT(phExDoorAccess_PostBootReason(eBootReason) == PH_ERR_SUCCESS);

    return PH_ERR_SUCCESS;
}

/**
 * Initializes the System/app Task and its queue.
 *
 * @return Status of the System/app's Task & queue initialization.
 */


static phStatus_t phExDoorAccess_SystemTaskInit(void)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Initialize the Clif Hal. */
    phExDoorAccess_Clif_HalInit();

    /** Create the System/app queue to which PMU/PCR ISR and other tasks post messages */
    gphExDoorAccess_SysTask_Queue = phRtos_QueueCreate(PHFL_SYS_TASK_Q_LEN, sizeof(phFlashBoot_Event_SysMsg_t));

    PH_USER_ASSERT(gphExDoorAccess_SysTask_Queue);

    /** Create system task */
    phRtos_TaskCreate( &phExDoorAccess_SystemTask, "SystemTask", PH_EXDOORACCESS_CFG_RTOS_SYS_TASK_STACK_SIZE, NULL,
        PH_EXDOORACCESS_CFG_RTOS_SYS_TASK_PRIORITY, &pHandle );


    PH_USER_ASSERT(pHandle);

    phExDoorAccess_MonitorEnableTxLdo();

    return PH_ERR_SUCCESS;
}
/** phExDoorAccess entry point for RTOS based. */
int main(void)
{
	phFlashBoot_Main();
    phhalTimer_Init();
    phOsal_Init();
	

#if defined(PHFL_ENABLE_STANDBY) && defined (PH_EXMAIN_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE)
    /** Configure GPIO #PH_EXMAIN_CFG_SWD_DNLD_PIN for SWD download mode. */
    phhalPcr_ConfigInput(PH_EXDOORACCESS_CFG_SWD_DNLD_PIN, PH_ENABLE, PH_DISABLE, PH_DISABLE, PH_DISABLE,
            PH_DISABLE, PH_DISABLE);
    phhalPcr_ConfigPuPd(PH_EXDOORACCESS_CFG_SWD_DNLD_PIN, PH_DISABLE, PH_ENABLE);

    /** Wait infinitely if GPIO #PH_EXDOORACCESSNONEXPORT_CFG_SWD_DNLD_PIN is equal to
     *  #PH_EXDOORACCESSNONEXPORT_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE to support download through
     *  SWD before going to StandBy. See the description of #PH_EXDOORACCESS_CFG_SWD_DNLD_PIN
     *  for more information.
     */
    while( (PH_EXDOORACCESS_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE << (PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXDOORACCESS_CFG_SWD_DNLD_PIN))
        == PH_REG_GET_FIELD_NS_SC(PCR_PADIN_REG, 1UL << (PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXDOORACCESS_CFG_SWD_DNLD_PIN)) )
    {
        ;

    }
#endif

    /** Create & initialize system task */
    phExDoorAccess_SystemTaskInit();

    /** Check the Boot reason and post the message to system task OR perform the corresponding boot operation. */
    phExDoorAccess_BootHandler();

    /** Start the tasks and timer running. */
    phRtos_Start();

    return 0;
}

/**
 * System wake up handler.
 *
 * @param eBootReason : Boot reason to be handled.
 */
static void phExDoorAccess_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
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
    phhalPcr_WakeUpConfig.wWakeUpTimerVal = 500;

    /* Wakeup's not required to configure */
    phhalPcr_WakeUpConfig.bSmBusAddrResp = 0x00;
    phhalPcr_WakeUpConfig.bWakeUpTemp = 0x00;
    phhalPcr_WakeUpConfig.blWakeupVbusLow = 0x00;
#endif

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

        /** Initialize the clif Tasks. */
        phExDoorAccess_ClifTaskInit(E_BOOT_NONE, NULL);
#endif
        break;

    case E_RFLDT_BOOT :
        /** Initialize the clif and CT Tasks with wakeup reason as RFLD. */
        phExDoorAccess_ClifTaskInit(eBootReason, gphExDoorAccess_SysTask_Queue);
        break;

    case E_WUC_CNT :
        /** wakeUp due to wakeUp counter. If CT card present then start CT app else
         * perform Clif operations.
         *  */
        {
        	if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
            {
                /* Do Nothing */
            }
            else
            {
                /** perform Clif operations. */
                phExDoorAccess_ClifTaskInit(eBootReason, gphExDoorAccess_SysTask_Queue);
            }
        }
        break;

    case E_CT_PRESENCE :
        break;

    case E_I2C_ADDR:
    case E_HSU_INT :
    case E_SPI_ADDR:
        /** Host based system. It is not expected to reach here. */
        PH_USER_ASSERT(0);
        break;

    case E_SMBUS_ADDR:
    case E_SWP_DET:
    case E_TVDD_MON:
    case E_GPIO_INT:
    case E_DFT_RESET:
    case E_INT_AUX_BOOT:
        /** As of now nothing to deal with these WakeUp reasons.
         * May be required for future. */
        PH_USER_ASSERT(0);
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
static void phExDoorAccess_PcrCallBack(uint32_t dwIRQMessage)
{
    /* No functionality */
}

/**
 * Performs callback function for the PMU IRQ's.
 *
 * @param dwIRQMessage :  IRQ from the PMU
 */
static void phExDoorAccess_PmuCallBack(uint32_t dwIRQMessage)
{
	/* No functionality */
}


/**
 * Example application main task, handles application sequencing.
 *
 * @param pvParams : Unused.
 */

void phExDoorAccess_SystemTask(void *pvParams)
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
    //phhalPcr_WakeUpConfig.wWakeUpTimerVal = gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal;
    phhalPcr_WakeUpConfig.wWakeUpTimerVal = 500;

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
    phhalPcr_RegCallBack(&phExDoorAccess_PcrCallBack,dwPcrIRQMessage);
    phhalPmu_RegCallBack(&phExDoorAccess_PmuCallBack,dwPmuIRQMessage);

    while(1)
    {
        phRtos_QueueReceive( gphExDoorAccess_SysTask_Queue, &sSysMsg, portMAX_DELAY );
        switch(sSysMsg.eSrcId)
        {
        /** Message from the flash boot. */
        case E_PH_BOOT:
            phExDoorAccess_WakeUpHandler((phhalPcr_BootReason_t)sSysMsg.dwMsg[0]);
            break;

            /** Message from the CLIF task */
        case E_PH_CLIF:
            switch(sSysMsg.dwMsg[0])
            {
            /** CLIF application completed its task either go to standby OR start poll timer */
            case (uint32_t)E_PHEXDOORACCESS_CLIF_RSP_END:
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
                phhalTimer_Start(gpphExDoorAccess_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
#endif
            break;

            default:
                PH_USER_ASSERT(0); /** It is not expected to reach here. */
            }

            break;
            case E_PH_TIMER:
                /** Post the polling message to clif task. */
                phExDoorAccess_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXDOORACCESS_CLIF_CMD_START,
                        gphExDoorAccess_SysTask_Queue);
                break;

                /** Handle the message posts from the CT. */
            case E_PH_CT:
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
static void phExDoorAccess_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                             phExDoorAccess_Clif_MsgId_t eClifCmd, void *pDes)
{
    phExDoorAccess_Clif_Msg_t sClifMsg;

    sClifMsg.sClifConfig.eDiscLoopEntry = eDiscLoopEntry;

    sClifMsg.eSrcId = E_PH_CLIF;
    sClifMsg.pvDes = pDes;
    sClifMsg.eClifMsgId = eClifCmd;
    /** wait indefinitely until post boot clif task is success. */
    PH_USER_ASSERT(phRtos_QueueSend( gphExDoorAccess_Clif_Queue, &sClifMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Initializes Clif Queue & task, posts bootUp message in case of Standby enabled.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
static phStatus_t phExDoorAccess_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Create the queue to which System Task can post messages */
    gphExDoorAccess_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExDoorAccess_Clif_Msg_t));
    PH_USER_ASSERT(gphExDoorAccess_Clif_Queue);

#ifdef PHFL_ENABLE_STANDBY
    /** Depending on the wakeup reason, post CLIF message to enter poll/listen mode */
    if(eBootReason == E_RFLDT_BOOT)
    {
        phExDoorAccess_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_LISTEN, E_PHEXDOORACCESS_CLIF_CMD_START, pDes);
    }
    else if(eBootReason == E_WUC_CNT)
    {
        phExDoorAccess_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXDOORACCESS_CLIF_CMD_START, pDes);
    }
    else
    {
        /** If no boot reason, expect Poll Timer(T0) to expire and post message to Clif queue */
        ;
    }
#endif /** PHFL_ENABLE_STANDBY */

    /** Create clif task with \ref PH_EXDOORACCESS_CFG_RTOS_CLIF_TASK_PRIORITY priority. */
    pHandle = NULL;
    phRtos_TaskCreate(&phExDoorAccess_ClifTask, "ClifTask", PH_EXDOORACCESS_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXDOORACCESS_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

#ifdef PHFL_ENABLE_LLCPSNEP
    /* Create clif task with stack size of 1800 bytes and highest priority */
    pHandle = NULL;
    phRtos_TaskCreate(&phExLLCP_Client, "LLCP_Client", PH_EXLLCP_CLIENT_TASK_STACK, (void *)&slnLlcp,
        PH_EXLLCP_CLIENT_TASK_PRIO, &pHandle );
    PH_USER_ASSERT(pHandle);
#endif

    return PH_ERR_SUCCESS;
}

/**
 * Clif Task which performs all the Contact less interface operations.
 *
 * @param pvParams : Unused.
 */
static void phExDoorAccess_ClifTask(void *pvParams)
{
    phExDoorAccess_Clif_Msg_t sClifMsg;
    uint16_t wStatus;

    phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
    phacDiscLoop_Sw_DataParams_t sDiscLoopParam;

#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif

    /* Configure the discovery loop parameters, for polling/listen/active */
    psDiscLoopParams = &sDiscLoopParam;
    wStatus = phExDoorAccess_Clif_DiscLoopConfig(psDiscLoopParams);
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
    if(phExDoorAccess_Clif_PalInit(psDiscLoopParams) != PH_ERR_SUCCESS)
    {
    	phExDoorAccess_Clif_DeInit(psDiscLoopParams->pHalDataParams);
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

#ifndef PHFL_ENABLE_STANDBY
    /*Standby is disabled, enable the poll timer, configure and start running in single shot mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,
        &gpphExDoorAccess_Clif_PollTimer) == PH_ERR_SUCCESS);

    phhalTimer_Configure(gpphExDoorAccess_Clif_PollTimer, (uint32_t)gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal,
        &phExDoorAccess_TimerCallBackFunc);

    phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_ENABLE);
    phhalRf_RegCallBack(&phExDoorAccess_RfCallBackFunc, CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK);
#endif
    while(1)
    {
#ifndef PHFL_ENABLE_STANDBY
        phhalTimer_Start(gpphExDoorAccess_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
#endif
        /** check for the Clif profile and startup sequence. */
        phRtos_QueueReceive( gphExDoorAccess_Clif_Queue, (void *)&sClifMsg, portMAX_DELAY );

        switch(sClifMsg.eClifMsgId)
        {
        case E_PHEXDOORACCESS_CLIF_CMD_START:
            psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
#ifdef PHFL_ENABLE_STANDBY
            sSysTaskMsg.dwMsg[1] =
#endif
            (uint32_t)phExDoorAccess_ClifMain(psDiscLoopParams, sClifMsg.sClifConfig.eDiscLoopEntry);

#ifdef PHFL_ENABLE_STANDBY
            /** Intimate the Contact less operations are done. */
            sSysTaskMsg.eSrcId = E_PH_CLIF;
            sSysTaskMsg.pvDes = gphExDoorAccess_Clif_Queue;
            sSysTaskMsg.dwMsg[0] = E_PHEXDOORACCESS_CLIF_RSP_END;
            (void)phRtos_QueueSend( sClifMsg.pvDes, (void *)&sSysTaskMsg, portMAX_DELAY);
#endif /** PHFL_ENABLE_STANDBY */
            break;
#ifndef PHFL_ENABLE_STANDBY
        case E_PHEXDOORACCESS_CLIF_CMD_STOP:
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

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static void phExDoorAccess_MonitorEnableTxLdo(void)
{
    if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXDOORACCESS_CFG_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXDOORACCESS_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXDOORACCESS_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

#endif
