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
 * phExPos_Clif.c: Implements the Clif task and the logical operations related to contact less interface.
 *
 * Project:  PN7462AU
 *s
 * $Date: 2016-12-01 14:26:44 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18769 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phhalPcr.h"
#include "phhalRf.h"
#include "phhalHw.h"
#include "phFlashBoot_Event.h"
#include "phExPos.h"
#include "phExPos_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phCfg_EE.h"
#include "phExPos_Cfg.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phExPos_Poll.h"
#include "phExPos_Log.h"
#include "phalT1T.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4a.h"
#include "phpalI14443p4.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "ph_Reg.h"
#include "phhalPmu.h"
#include "phExPos_Common.h"
#include "phExPos_Utils.h"
#include "phUser.h"

#include "phKeyStore.h"
/*
 * All the Hardware specific API's will be placed in this file.
 */

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
extern phExPos_Posfw_Data_t gphExPos_UtilsPosParama;
extern phhalTimer_Timers_t *pLedTimer;
PH_NOINIT phRtos_QueueHandle_t gphExPos_Clif_Queue = NULL;
PH_NOINIT static uint8_t gphExPos_Rxbuf[PH_EXPOS_CLIF_RXBUFSIZE];
PH_NOINIT static uint8_t gphExPos_Txbuf[PH_EXPOS_CLIF_TXBUFSIZE];

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */

phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */
PH_NOINIT static phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A = {0};
PH_NOINIT static phpalI14443p4a_Sw_DataParams_t  gphpal_Sw_DataParams4A = {0};
PH_NOINIT static phpalI14443p4_Sw_DataParams_t gphpal_Sw_DataParams4 = {0};
PH_NOINIT static phalT1T_Sw_DataParams_t gphpal_Sw_DataParamsT1T = {0};
PH_NOINIT static phpalI14443p3b_Sw_DataParams_t  gphpal_Sw_DataParamsB = {0};


PH_NOINIT static phSysSv_Clif_CardMode_TypeA_Param_t gphExPos_A;
PH_NOINIT static phhalRf_CardMode_TypeB_Param_t gphExPos_B;
PH_NOINIT static phhalRf_CardMode_TypeF_Param_t gphExPos_F;
static const uint8_t gkphExPos_Atqb[] = {0x50, 0x10, 0xDF, 0x27, 0xAF, 0x1C, 0x2D, 0x94, 0x11, 0xF7, 0x71, 0x85};
static const uint8_t gkphExPos_SensFRes[] = {0x12, 0x01, 0x01, 0x27, 0x00, 0x68, 0x53, 0xDD, 0xA6, 0xCA, 0x00,
    0xF0, 0x00, 0x00, 0x02, 0x06, 0x03, 0x00};

PH_NOINIT uint8_t gphExPos_Clif_PollLoopLOGCount; /* Don't log Card Not Detected too much */

#ifndef PHFL_ENABLE_STANDBY
PH_NOINIT phhalTimer_Timers_t *gpphExPos_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExPosClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
static void phExPosClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib);
static phStatus_t phExPos_Clif_MonitorCheckTxLdo();

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Performs message posting to the Clif task.
 *
 * @param eDiscLoopEntry : Discovery loop entry point.
 * @param eClifCmd : Clif command.
 * @param pDes : Destination message queue handler.
 */
void phExPos_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                            phExPos_Clif_MsgId_t eClifCmd, void *pDes)
{
    phExPos_Clif_Msg_t sClifMsg;

    sClifMsg.sClifConfig.eDiscLoopEntry = eDiscLoopEntry;

    sClifMsg.eSrcId = E_PH_CLIF;
    sClifMsg.pvDes = pDes;
    sClifMsg.eClifMsgId = eClifCmd;
    /** wait indefinitely until post boot to clif task is success. */
    PH_USER_ASSERT(phRtos_QueueSend( gphExPos_Clif_Queue, &sClifMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Initializes Clif Queue & task, posts bootUp message in case of Standby enabled.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
phStatus_t phExPos_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /* LOG_TXT("SchdClifTask"); */
    /* Create the queue to which System Task can post messages */
    gphExPos_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExPos_Clif_Msg_t));
    PH_USER_ASSERT(gphExPos_Clif_Queue);


#ifdef PHFL_ENABLE_STANDBY
    /* Depending on the wakeup reason, post CLIF message to enter poll/listen mode */
    if(eBootReason == E_RFLDT_BOOT)
    {
        phExPos_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_LISTEN, E_PHEXMAIN_CLIF_CMD_START, pDes);
    }
    else if(eBootReason == E_WUC_CNT)
    {
        phExPos_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXMAIN_CLIF_CMD_START, pDes);
    }
    else
    {
        /* If no boot reason, expect Poll Timer(T0) to expire and post message to Clif queue */
        ;
    }
#endif /* PHFL_ENABLE_STANDBY */

    /* Create clif task with stack size of 1800 bytes and highest priority */
    pHandle = NULL;
    phRtos_TaskCreate(&phExPos_ClifTask, "ClifTask", PH_EXPOS_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXPOS_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    return PH_ERR_SUCCESS;
}

void phExPos_ClifTask(void *pvParams)
{
    phExPos_Clif_Msg_t sClifMsg;
#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif

    /* Initialize the Clif Hal. */
    //phExPosClif_HalInit();

    gphExPos_Clif_PollLoopLOGCount = 0;

#ifndef PHFL_ENABLE_STANDBY
    /*Standby is disabled, enable the poll timer, configure and start running in single shot mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,
        &gpphExPos_Clif_PollTimer) == PH_ERR_SUCCESS);

    phhalTimer_Configure(gpphExPos_Clif_PollTimer, (uint32_t)gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal,
        &phExPos_TimerCallBackFunc);
#endif
    while(1)
    {
#ifndef PHFL_ENABLE_STANDBY

    	if (gphExPos_UtilsPosParama.bEnPollStartup == 1)
    	{
        phhalTimer_Start(gpphExPos_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
    	}

#endif
        /* check for the Clif profile and startup sequence. */
        phRtos_QueueReceive( gphExPos_Clif_Queue, (void *)&sClifMsg, portMAX_DELAY );
        /* LOG_TXT_D("=>Clif Task\n"); */

        switch(sClifMsg.eClifMsgId)
        {
        case E_PHEXMAIN_CLIF_CMD_START:
            /* phExPos_Main is a standalone application for Clif. */
#ifdef PHFL_ENABLE_STANDBY
            sSysTaskMsg.dwMsg[1] =
#endif
			phRtos_MutexTake(gphExPos_UtilsPosParama.xCT_CL_Mutex, 0xffffffff);
            (uint32_t)phExPos_ClifMain(sClifMsg.sClifConfig.eDiscLoopEntry);
			phRtos_MutexGive(gphExPos_UtilsPosParama.xCT_CL_Mutex);



#ifdef PHFL_ENABLE_STANDBY
            /* wait indefinitely until post STANDBY to System task. */
            sSysTaskMsg.eSrcId = E_PH_CLIF;
            sSysTaskMsg.pvDes = gphExPos_Clif_Queue;
            sSysTaskMsg.dwMsg[0] = E_PHEXMAIN_CLIF_RSP_END;
            (void)phRtos_QueueSend( sClifMsg.pvDes, (void *)&sSysTaskMsg, portMAX_DELAY);
#endif /* PHFL_ENABLE_STANDBY */
            break;
#ifndef PHFL_ENABLE_STANDBY
        case E_PHEXMAIN_CLIF_CMD_STOP:
            /* system task wants us to stop */
            break;
#endif
        default:
            /* It is not expected to reach here. */
            PH_USER_ASSERT(0);
            break;
        }

    }
}

phStatus_t phExPos_ClifMain(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry)
{
    phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
    phacDiscLoop_Sw_DataParams_t sDiscLoopParam;
    phStatus_t wDiscLoopStatus;
    uint16_t wMeasuredAgcValue;
    uint8_t bSensFResLen;
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    uint8_t bAts[64] ;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/

    uint16_t wStatus;

    psDiscLoopParams = &sDiscLoopParam;
    phLED_SetPattern(gkphLED_BootUp);

    wStatus = phacDiscLoop_Sw_Init(psDiscLoopParams,
                                    sizeof(phacDiscLoop_Sw_DataParams_t),
                                    &sHal
                                    );

    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    /* Link the global HalDataParams to the pDiscLoop params.
     * NOTE: gphphhalHw_Nfc_Ic_DataParams must be initialized by this time. */
    psDiscLoopParams->pHalDataParams = (void *)&sHal;

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, false);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }


    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, 3);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }


#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
        psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts = &bAts[0];
#endif  /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/

    gphExPos_A.bAtqa[0] = 0x44;
    gphExPos_A.bAtqa[1] = 0x33;
    gphExPos_A.bSak = 0x20;
    gphExPos_A.bEntryState = PHHAL_RF_CMA_ENTRY_STATE_IDLE;
    gphExPos_A.bUid[0] = 0x01;
    gphExPos_A.bUid[1] = 0x02;
    gphExPos_A.bUid[2] = 0x03;
    gphExPos_A.bCurrentState = 0x00;

    gphExPos_B.pAtqb = (uint8_t *)gkphExPos_Atqb;
    gphExPos_B.bExtendedAtqb = false;

    bSensFResLen = gkphExPos_SensFRes[0];
    gphExPos_F.pSensfResp = (uint8_t *)gkphExPos_SensFRes;
    gphExPos_F.bSensfRespLen = bSensFResLen;
    /**
     *  hardware specific API to initialize the Hal and Pal.
     *  Initializes the psDiscLoopParams->pHalDataParams & psDiscLoopParams->pPalDataParams variables
     *  based on Poll and Listen bit map configuration.
     */
    if(phExPosClif_PalInit(psDiscLoopParams) != PH_ERR_SUCCESS)
    {
        phExPosClif_DeInit(psDiscLoopParams->pHalDataParams);
        return PH_ERR_INTERNAL_ERROR;
    }

    if(psDiscLoopParams->bPasPollTechCfg || psDiscLoopParams->bActPollTechCfg)
    {
        /* LPCD configuration */
        if(psDiscLoopParams->bLpcdEnabled)
        {
            /* Configure if value needs to be different from default */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_DURATION, 1500);
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD, 60);

            /* Read the Last measured value from GPREG and set the reference value. */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE,
                (uint16_t)PH_REG_GET( PCR_GPREG1_REG));
        }
    }
    /*start the LED timer running in free running mode.*/
	 phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

    if ( 0 == gphExPos_Clif_PollLoopLOGCount ) {
        LOG_TXT("Entering Polling mode..\n");
    }
    phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0
          | PHAC_DISCLOOP_POS_BIT_MASK_A
          | PHAC_DISCLOOP_POS_BIT_MASK_B
          | PHAC_DISCLOOP_POS_BIT_MASK_F212
          | PHAC_DISCLOOP_POS_BIT_MASK_F424
          | PHAC_DISCLOOP_POS_BIT_MASK_V
          );

    phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG,0);
    phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG,0);
    phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG,0);

    wStatus = phExPos_Clif_MonitorCheckTxLdo();
    if (wStatus != PH_ERR_SUCCESS)
	{
    	phhalTimer_Stop(pLedTimer);
    	phExPos_UtilsAll_LEDS_OFF();
   	    phExPos_UtilsGlow_Error_LEDS();
		return PHAC_DISCLOOP_NO_TECH_DETECTED;
	}

    wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);

    wDiscLoopStatus &= PH_ERR_MASK;
    phExPos_Log_DiscLoop(wDiscLoopStatus, gphExPos_Clif_PollLoopLOGCount);
    gphExPos_Clif_PollLoopLOGCount++; /* Let it roll back. */

    /* If LPCD returns object found, but the object is not valid card, then set bacup
     * current AGC value for future reference.
     */
    if(wDiscLoopStatus == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
    {
        /* Store the Agc Value in GPREG1 so that it is persited during standby */
        (void)phhalHw_GetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE,
                &wMeasuredAgcValue);
        PH_REG_SET( PCR_GPREG1_REG, (uint32_t)wMeasuredAgcValue);
    }

    switch(wDiscLoopStatus)
    {
    /*Poll Device/Tech found. */
    case PHAC_DISCLOOP_DEVICE_ACTIVATED:
        gphExPos_Clif_PollLoopLOGCount = 0;
    	phhalTimer_Stop(pLedTimer);
    	phExPos_UtilsAll_LEDS_OFF();
        phExPos_Poll_Main(psDiscLoopParams);
        break;

        /* Listen Device found. */
        /* No Device/Error found. */
    case PHAC_DISCLOOP_FAILURE:
    case PHAC_DISCLOOP_NO_TECH_DETECTED:
    case PHAC_DISCLOOP_NO_DEVICE_RESOLVED:
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED :
    case PHAC_DISCLOOP_MULTI_TECH_DETECTED:
    case PHAC_DISCLOOP_EXTERNAL_RFON:
    case PHAC_DISCLOOP_COLLISION_PENDING:
    case PHAC_DISCLOOP_MERGED_SEL_RES_FOUND:
    case PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED:
    case PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED:
    case PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED:

        break;
    default:
        /* It is not expected to reach here. */
        //PH_USER_ASSERT(0);
    	break;
    }

    (void)phhalHw_FieldOff(psDiscLoopParams->pHalDataParams);

    return (phStatus_t)wDiscLoopStatus;
}

void phExPosClif_HalInit(void)
{
	uint16_t status;
    /* Clear the RX buf */
    phUser_MemSet((void *)&gphExPos_Rxbuf[0], 0x00, PH_EXPOS_CLIF_RXBUFSIZE);
    /* Clear the TX buf */
    phUser_MemSet((void *)&gphExPos_Txbuf[0], 0x00, PH_EXPOS_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
			gphExPos_Txbuf,
            sizeof(gphExPos_Txbuf),
			gphExPos_Rxbuf,
            sizeof(gphExPos_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}


static phStatus_t phExPosClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    /* ISO14443-3a PAL, T1T AL initialization if polling for Type A is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        phUser_MemSet((void *)&gphpal_Sw_DataParams3A, 0x00, sizeof(phpalI14443p3a_Sw_DataParams_t));
        phUser_MemSet((void *)&gphpal_Sw_DataParamsT1T, 0x00, sizeof(phalT1T_Sw_DataParams_t));
        phUser_MemSet((void *)&gphpal_Sw_DataParams4A, 0x00, sizeof(phpalI14443p4a_Sw_DataParams_t));

        if(phpalI14443p3a_Sw_Init(&gphpal_Sw_DataParams3A, sizeof( gphpal_Sw_DataParams3A),
                &sHal)!= PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        if(phpalI14443p4a_Sw_Init(&gphpal_Sw_DataParams4A, sizeof( gphpal_Sw_DataParams4A),
                &sHal)!= PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        if( phalT1T_Sw_Init(&gphpal_Sw_DataParamsT1T,   sizeof( gphpal_Sw_DataParamsT1T),
                &gphpal_Sw_DataParams3A)  != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal1443p3aDataParams = (void *)&gphpal_Sw_DataParams3A;
        psDiscLoopParams->pPal1443p4aDataParams = (void *)&gphpal_Sw_DataParams4A;

        psDiscLoopParams->pAlT1TDataParams = (void *)&gphpal_Sw_DataParamsT1T;

    }

    /* ISO14443-3b PAL initialization if polling for Type B is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParamsB, 0x00, sizeof(phpalI14443p3b_Sw_DataParams_t));

        if(phpalI14443p3b_Sw_Init(&gphpal_Sw_DataParamsB, sizeof(gphpal_Sw_DataParamsB),
                &sHal) != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal1443p3bDataParams = (void *)&gphpal_Sw_DataParamsB;
    }

    /* ISO14443-4 PAL initialization if polling for Type A or Type B is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & ((1 << PHAC_DISCLOOP_TECH_TYPE_A) | (1 << PHAC_DISCLOOP_TECH_TYPE_B)))
    {
        phUser_MemSet((void *)&gphpal_Sw_DataParams4, 0x00, sizeof(phpalI14443p4_Sw_DataParams_t));
        if(phpalI14443p4_Sw_Init(&gphpal_Sw_DataParams4, sizeof( gphpal_Sw_DataParams4),
                &sHal)            != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal14443p4DataParams = (void *)&gphpal_Sw_DataParams4;
    }
    (void)phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_OPE_MODE, RD_LIB_MODE_EMVCO);



    return PH_ERR_SUCCESS;
}


static void phExPosClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib)
{
    phStatus_t wStatus = PH_ERR_INTERNAL_ERROR;

    do
    {
        /*switching off the rf field*/
        wStatus = phhalHw_FieldOff(phhalHwClifRdLib);

        if (wStatus != PH_ERR_SUCCESS)
        {
            break;
        }

        /* Deinitialisng the bare metal HAL */
        wStatus = (phStatus_t) phhalRf_DeInit ();

        if (wStatus != PH_ERR_SUCCESS)
        {
            break;
        }

    }while(0);

}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static phStatus_t phExPos_Clif_MonitorCheckTxLdo()
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXPOS_CFG_USETXLDO_EXTERNAL)
	{
		/* Perform the Monitor Check of TxLdo */
		eStatus = phhalPmu_TxLdoMonitorCheck();
	}
	else
	{
		return PH_ERR_SUCCESS;
	}
	return eStatus;
}
