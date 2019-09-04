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
 * phExVCom.c:  Main application file and entry point of phExVCom.
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

#include <ph_Datatypes.h>
#include <ph_Platform.h>
#include <ph_Status.h>
#include <phhalPcr.h>

#ifdef PHFL_HALAPI_WITH_RTOS

#include "phRtos.h"
#include "ph_Log.h"
#include "phCfg_EE.h"
#include <phExVCom.h>
#include <phExVCom_Cfg.h>
#include "phFlashBoot.h"
#include "phFlashBoot_Event.h"
#include "phhalGpio.h"
#include "phhalPmu.h"
#include "phhalRf.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "phUser.h"
#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_pmu.h"
#include "string.h"
#include "phExVCom_PollTypeA.h"
#include "phOsal.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXVCOM_USERCMD_T     ((uint8_t)'T')  /* Command for Starting Type A Polling */
#define PH_EXVCOM_USERCMD_O     ((uint8_t)'O')  /* Command for Making the RF Field ON  */
#define PH_EXVCOM_USERCMD_F     ((uint8_t)'F')  /* Command for Making the RF Field OFF */

#define PH_EXVCOM_MAX_CMD_PAYLOAD_SIZE 270
#define PH_EXVCOM_MAX_RSP_PAYLOAD_SIZE 270

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/** RTOS message queue handlers */
static phRtos_QueueHandle_t gphExVCom_SysTask_Queue = NULL;
static phRtos_QueueHandle_t gphExVCom_Clif_Queue = NULL;
static phhalTimer_Timers_t *pLedTimer = {0};

/* Buffer where the content will be received */
PH_NOINIT_ALIGNED uint8_t gExVCom_CmdBuffer[PH_EXVCOM_MAX_CMD_PAYLOAD_SIZE];

const char gExVCom_Hex_Numbers[] = "0123456789ABCDEF";
/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */
extern uint8_t gphExVCom_Rf_UID[PH_EXVCOM_RF_MAX_NUM_TAG][7];
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phExVCom_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
static void phExVCom_ClifTask(void *pvParams);

static void phExVCom_TxLdoMonitorEnable();
static void phExVCom_Display_UID();
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * Posts boot reason to the System/app Task.
 *
 * @param eBootReason : Boot reason to be posted to the System/app Task.
 * @return Status of the posting operation.
 */
static phStatus_t phExVCom_PostBootReason(phhalPcr_BootReason_t eBootReason)
{
    phFlashBoot_Event_SysMsg_t eSysMsg;

    eSysMsg.dwMsg[0] = (uint32_t)eBootReason;
    eSysMsg.eSrcId = E_PH_BOOT;
    eSysMsg.pvDes = gphExVCom_SysTask_Queue;

    /** Post boot reason to system task queue. */
    if(phRtos_QueueSend( gphExVCom_SysTask_Queue,  &eSysMsg, PHRTOS_MAX_DELAY)!= phRtos_Success ){
        return PH_ERR_OPERATION_TIMEDOUT;
    }
    return PH_ERR_SUCCESS;
}

/**
 * Wake up Boot Reason handler.
 */
static phStatus_t phExVCom_BootHandler(void)
{
    phhalPcr_BootReason_t eBootReason;

    /** Get & clear the boot reason. */
    eBootReason = phFlashBoot_GetBtRns();
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /** Post the Boot reason to the system task. Hang here if the posting is failed. */
    PH_USER_ASSERT(phExVCom_PostBootReason(eBootReason) == PH_ERR_SUCCESS);

    return PH_ERR_SUCCESS;
}

/**
 * Initializes the System/app Task and its queue.
 *
 * @return Status of the System/app's Task & queue initialization.
 */
static phStatus_t phExVCom_SystemTaskInit(void)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Create the System/app queue to which PMU/PCR ISR and other tasks post messages */
    gphExVCom_SysTask_Queue = phRtos_QueueCreate(PHFL_SYS_TASK_Q_LEN, sizeof(phFlashBoot_Event_SysMsg_t));

    PH_USER_ASSERT(gphExVCom_SysTask_Queue);

    /** Create system task */
    phRtos_TaskCreate( &phExVCom_SystemTask, "SystemTask", PH_EXVCOM_CFG_RTOS_SYS_TASK_STACK_SIZE, NULL,
        PH_EXVCOM_CFG_RTOS_SYS_TASK_PRIORITY, &pHandle );

    PH_USER_ASSERT(pHandle);

    return PH_ERR_SUCCESS;
}

/** phExVCom entry point for RTOS based. */
int main(void)
{
    LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    phFlashBoot_Main();
    phOsal_Init();
    /* Perform the Initialization of the Platform */
    phExVCom_HalInit();

    /** Perform the USB COM Initialization */
    PH_USER_ASSERT(VCom_Init() == PH_ERR_SUCCESS);

    /** Create & initialize system task */
    phExVCom_SystemTaskInit();

    /** Check the Boot reason and post the message to system task OR perform the corresponding boot operation. */
    phExVCom_BootHandler();

    /** Start the tasks and timer running. */
    phRtos_Start();

    return 0;
}

/**
 * System wake up handler.
 *
 * @param eBootReason : Boot reason to be handled.
 */
static void phExVCom_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
{
    LOG_U32("Wkup",eBootReason);
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

        /** Initialize the clif and CT Tasks. */
        phExVCom_ClifTaskInit(E_BOOT_NONE, NULL);

        break;

    case E_RFLDT_BOOT :
        /** Initialize the clif and CT Tasks with wakeup reason as RFLD. */
        phExVCom_ClifTaskInit(eBootReason, gphExVCom_SysTask_Queue);
        break;

    case E_WUC_CNT :
        /** wakeUp due to wakeUp counter. If CT card present then start CT app else perform Clif operations.  */
        {
            /** perform Clif operations. */
            phExVCom_ClifTaskInit(eBootReason, gphExVCom_SysTask_Queue);
        }
        break;

    case E_CT_PRESENCE :
        /** WakeUp due to CT card presence, perform CT operations. */
        break;

    default:
    /** Invalid wakeUp, it is not expected to reach here. */
        PH_USER_ASSERT(0);
    }
}

/**
 * Example application main task, handles application sequencing.
 *
 * @param pvParams : Unused.
 */
void phExVCom_SystemTask(void *pvParams)
{
    phFlashBoot_Event_SysMsg_t sSysMsg;

    /** Enable the LED timer for 300 milli secs in free running mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100, &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

    phExVCom_TxLdoMonitorEnable();

    while(1)
    {
        phRtos_QueueReceive( gphExVCom_SysTask_Queue, &sSysMsg, portMAX_DELAY );
        switch(sSysMsg.eSrcId)
        {
        /** Message from the flash boot. */
        case E_PH_BOOT:
            phExVCom_WakeUpHandler((phhalPcr_BootReason_t)sSysMsg.dwMsg[0]);
            break;

            /** Message from the CLIF task */
        case E_PH_CLIF:
            break;
        /* In both the cases go to standby if enabled or WFI in case of non standby case */
        case E_PH_PMU:
        case E_PH_PCR:
            /* Simply WFI here since any error from the PMU/PCR should be debugged */
            PH_USER_ASSERT(0);
            break;
            default:
                PH_USER_ASSERT(0); /** It is not expected to reach here. */
        }
    }
}

/**
 * Initializes Clif Queue & task.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
static void phExVCom_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** Create the queue to which System Task can post messages */
    gphExVCom_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExVCom_Clif_Msg_t));
    PH_USER_ASSERT(gphExVCom_Clif_Queue);

    /** Create clif task with \ref PH_EXVCOM_CFG_RTOS_CLIF_TASK_PRIORITY priority. */
    pHandle = NULL;
    phRtos_TaskCreate(&phExVCom_ClifTask, "ClifTask", PH_EXVCOM_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXVCOM_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);
}

/**
 * Clif Task which performs all the Contact less interface operations.
 *
 * @param pvParams : Unused.
 */
static void phExVCom_ClifTask(void *pvParams)
{
    uint32_t wNumOfTags = 0;

    uint8_t wUserCmd = 0x0;
    uint8_t bTxBufferPass[9] = "Card UID=";
    uint8_t bTxBufferNoPass[15] = "Type A Polling\n";

    /* Perform the Protocol and Field Initialization */
    phExVCom_PalInit();

    gphExVCom_PollLoopLOGCount = 0;

    while(1)
    {
        /* Read the Serial Port for the Command Send from Terminal */
    	if(VCom_Read(gExVCom_CmdBuffer,PH_EXVCOM_MAX_CMD_PAYLOAD_SIZE))
        {
            switch(gExVCom_CmdBuffer[0])
            {
                /* Command for performing Type A Polling */
                case PH_EXVCOM_USERCMD_T:
                {
                    LOG_TXT("Poll command received\n");
                    LOG_TXT_VCOM("Poll command received\n");
                    gphExVCom_PollLoopLOGCount = 0;
                    wUserCmd = PH_EXVCOM_USERCMD_T;
                    break;
                }
                /* Command for doing Field ON */
                case PH_EXVCOM_USERCMD_O:
                {
                    LOG_TXT("RF ON command received\n");
                    LOG_TXT_VCOM("RF ON command received\n");
                    wUserCmd = PH_EXVCOM_USERCMD_O;
                    break;
                }
                /* Command for doing Field OFF */
                case PH_EXVCOM_USERCMD_F:
                {
                    LOG_TXT("RF OFF command received\n");
                    LOG_TXT_VCOM("RF OFF command received\n");
                    wUserCmd = PH_EXVCOM_USERCMD_F;
                    break;
                }
            }
        }

        switch(wUserCmd)
        {
            case PH_EXVCOM_USERCMD_T:
            {
                /* Perform the Type A Polling for Tag Identification */
            	phExVCom_PollTypeA(&wNumOfTags);
                /* Card Detected */
            	if(wNumOfTags > 0)
                {
                    phLED_SetPattern(gkphLED_Clif_Pass);
                    LOG_TXT("Cards Detected\n");
                    VCom_Write((uint8_t *) bTxBufferPass, (uint16_t)(sizeof(bTxBufferPass)));
                    phUser_Wait(5000);
                    /* Display the Card UID in the Terminal */
                    phExVCom_Display_UID();
                }
                else
                {
                    /* Display that Polling is on going */
                	VCom_Write((uint8_t *) bTxBufferNoPass, (uint16_t)(sizeof(bTxBufferNoPass)));
                    phLED_SetPattern(gkphLED_BootUp);
                }
                break;
            }
            case PH_EXVCOM_USERCMD_O:
            {
                /* Perform the Field ON on receiving the User Command O */
            	phhalRf_FieldOn();
                phLED_SetStatus(LED_G | LED_B);
                break;
            }
            case PH_EXVCOM_USERCMD_F:
            {
                /* Perform the Field OFF on receiving the User Command F */
            	phhalRf_FieldOff();
                phLED_SetStatus(LED_Y | LED_R);
                break;
            }
        }
        phUser_Wait(200000);
    }
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static void phExVCom_TxLdoMonitorEnable()
{
    /* Check the configured TxLdo is External */
    if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXVCOM_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_EXTERNAL_TVDD_3V_MON)
        phhalPmu_TxLdoMonitorEnable(PH_EXVCOM_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
        phhalPmu_TxLdoMonitorEnable(PH_EXVCOM_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

/* Function to Display the UID of the Card Detected */
static void phExVCom_Display_UID()
{
    uint8_t bUid[15];
    uint8_t bCnt;

    for (bCnt = 0; bCnt < 7; bCnt++)
    {
        bUid[(bCnt * 2)] = gExVCom_Hex_Numbers[(gphExVCom_Rf_UID[0][bCnt] >> 4) & 0x0F];
        bUid[(bCnt * 2) + 1] = gExVCom_Hex_Numbers[(gphExVCom_Rf_UID[0][bCnt]) & 0x0F];
    }
    bUid[(bCnt * 2)] = '\n';

    VCom_Write((uint8_t *) bUid, (uint16_t)(sizeof(bUid)));
}
#endif
