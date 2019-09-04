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
 * phExEMVCo_Clif.c: Implements the Clif task and the logical operations related to contact less interface.
 *
 * Project:  PN7462AU
 *s
 * $Date: 2016-12-01 14:14:28 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18767 $ (v05.19.00)
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
#include "phExEMVCo.h"
#include "phExEMVCo_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phCfg_EE.h"
#include "phExEMVCo_Cfg.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phExEMVCo_Poll.h"
#include "phExEMVCo_Log.h"
#include "phalT1T.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4a.h"
#include "phpalI14443p4.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "ph_Reg.h"
#include "phhalPmu.h"
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
PH_NOINIT phRtos_QueueHandle_t gphExEMVCo_Clif_Queue = NULL;
PH_NOINIT_ALIGNED static uint8_t gphExEMVCo_Rxbuf[PH_EXEMVCO_CLIF_RXBUFSIZE];
PH_NOINIT_ALIGNED static uint8_t gphExEMVCo_Txbuf[PH_EXEMVCO_CLIF_TXBUFSIZE];
phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */

PH_NOINIT static phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A;
PH_NOINIT static phpalI14443p4a_Sw_DataParams_t  gphpal_Sw_DataParams4A;
PH_NOINIT static phpalI14443p4_Sw_DataParams_t gphpal_Sw_DataParams4;
PH_NOINIT static phalT1T_Sw_DataParams_t gphpal_Sw_DataParamsT1T;
PH_NOINIT static phpalI14443p3b_Sw_DataParams_t  gphpal_Sw_DataParamsB;

PH_NOINIT uint8_t gphExEMVCo_Clif_PollLoopLOGCount; /* Don't log Card Not Detected too much */

#ifndef PHFL_ENABLE_STANDBY
PH_NOINIT phhalTimer_Timers_t *gpphExEMVCo_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phEXEMVCo_TxLdoMonitorCheck(void);
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
#ifdef PHFL_HALAPI_WITH_RTOS
void phExEMVCo_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry,
                            phExEMVCo_Clif_MsgId_t eClifCmd, void *pDes)
{
    phExEMVCo_Clif_Msg_t sClifMsg;

    sClifMsg.sClifConfig.eDiscLoopEntry = eDiscLoopEntry;

    sClifMsg.eSrcId = E_PH_CLIF;
    sClifMsg.pvDes = pDes;
    sClifMsg.eClifMsgId = eClifCmd;
    /** wait indefinitely until post boot to clif task is success. */
    PH_USER_ASSERT(phRtos_QueueSend( gphExEMVCo_Clif_Queue, &sClifMsg, portMAX_DELAY) == phRtos_Success );
}
#endif
/**
 * Initializes Clif Queue & task, posts bootUp message in case of Standby enabled.
 *
 * @param eBootReason : WakeUp boot reason.
 * @param pDes : Destination message queue handler.
 * @return Status of the operation.
 */
phStatus_t phExEMVCo_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /* LOG_TXT("SchdClifTask"); */
    /* Create the queue to which System Task can post messages */
    gphExEMVCo_Clif_Queue  = phRtos_QueueCreate(PHFL_EVENT_CLIF_Q_LEN, sizeof(phExEMVCo_Clif_Msg_t));
    PH_USER_ASSERT(gphExEMVCo_Clif_Queue);


#ifdef PHFL_ENABLE_STANDBY
    /* Depending on the wakeup reason, post CLIF message to enter poll/listen mode */
    if(eBootReason == E_RFLDT_BOOT)
    {
        phExEMVCo_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_LISTEN, E_PHEXMAIN_CLIF_CMD_START, pDes);
    }
    else if(eBootReason == E_WUC_CNT)
    {
        phExEMVCo_ClifPostMsg(PHAC_DISCLOOP_ENTRY_POINT_POLL, E_PHEXMAIN_CLIF_CMD_START, pDes);
    }
    else
    {
        /* If no boot reason, expect Poll Timer(T0) to expire and post message to Clif queue */
        ;
    }
#endif /* PHFL_ENABLE_STANDBY */

    /* Create clif task with stack size of 1800 bytes and highest priority */
    pHandle = NULL;
    phRtos_TaskCreate(&phExEMVCo_ClifTask, "ClifTask", PH_EXEMVCO_CFG_RTOS_CLIF_TASK_STACK_SIZE, NULL,
        PH_EXEMVCO_CFG_RTOS_CLIF_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    return PH_ERR_SUCCESS;
}
#ifdef PHFL_HALAPI_WITH_RTOS
void phExEMVCo_ClifTask(void *pvParams)
{
    phExEMVCo_Clif_Msg_t sClifMsg;
    phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
    phacDiscLoop_Sw_DataParams_t sDiscLoopParam;
    uint16_t wStatus;

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    uint8_t bAts[64] ;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/

#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif

    psDiscLoopParams = &sDiscLoopParam;
    phLED_SetPattern(gkphLED_BootUp);

    wStatus = phExEMVCoClif_DiscLoopConfig(psDiscLoopParams,bAts);
    if(wStatus != PH_ERR_SUCCESS)
    {
        PH_USER_ASSERT(0);
    }
    /**
     *  hardware specific API to initialize the Hal and Pal.
     *  Initializes the psDiscLoopParams->pHalDataParams & psDiscLoopParams->pPalDataParams variables
     *  based on Poll and Listen bit map configuration.
     */
    if(phExEMVCoClif_PalInit(psDiscLoopParams) != PH_ERR_SUCCESS)
    {
        phExEMVCoClif_DeInit(psDiscLoopParams->pHalDataParams);
        PH_USER_ASSERT(0);
    }

    gphExEMVCo_Clif_PollLoopLOGCount = 0;

#ifndef PHFL_ENABLE_STANDBY
    /*Standby is disabled, enable the poll timer, configure and start running in single shot mode. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,
        &gpphExEMVCo_Clif_PollTimer) == PH_ERR_SUCCESS);

    phhalTimer_Configure(gpphExEMVCo_Clif_PollTimer, (uint32_t)gpkphCfg_EE_HW_WakeUpConfig->wWakeUpTimerVal,
        &phExEMVCo_TimerCallBackFunc);
#endif
    while(1)
    {
#ifndef PHFL_ENABLE_STANDBY
        phhalTimer_Start(gpphExEMVCo_Clif_PollTimer, E_TIMER_SINGLE_SHOT);
#endif
        /* check for the Clif profile and startup sequence. */
        phRtos_QueueReceive( gphExEMVCo_Clif_Queue, (void *)&sClifMsg, portMAX_DELAY );
        /* LOG_TXT_D("=>Clif Task\n"); */

        switch(sClifMsg.eClifMsgId)
        {
        case E_PHEXMAIN_CLIF_CMD_START:

            psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
            /* phExEMVCo_Main is a standalone application for Clif. */
#ifdef PHFL_ENABLE_STANDBY
            sSysTaskMsg.dwMsg[1] =
#endif
            (uint32_t)phExEMVCo_ClifMain(psDiscLoopParams,sClifMsg.sClifConfig.eDiscLoopEntry);


#ifdef PHFL_ENABLE_STANDBY
            /* wait indefinitely until post STANDBY to System task. */
            sSysTaskMsg.eSrcId = E_PH_CLIF;
            sSysTaskMsg.pvDes = gphExEMVCo_Clif_Queue;
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
#endif
phStatus_t phExEMVCo_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
                              phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry)
{
    phStatus_t wDiscLoopStatus;
    uint16_t wMeasuredAgcValue;

    wDiscLoopStatus = phEXEMVCo_TxLdoMonitorCheck();
	if (wDiscLoopStatus != PH_ERR_SUCCESS)
	{
		phLED_SetStatus(LED_R);
		LOG_TXT("phExEMVCo: No External TVDD Present \n");
		return PHAC_DISCLOOP_NO_TECH_DETECTED;
	}

    if ( 0 == gphExEMVCo_Clif_PollLoopLOGCount )
    {
        LOG_TXT("Entering Polling mode..\n");
    }

    wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
    wDiscLoopStatus &= PH_ERR_MASK;
    phExEMVCo_Log_DiscLoop(wDiscLoopStatus, gphExEMVCo_Clif_PollLoopLOGCount);
    gphExEMVCo_Clif_PollLoopLOGCount++; /* Let it roll back. */

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
        gphExEMVCo_Clif_PollLoopLOGCount = 0;
        phLED_SetPattern(gkphLED_Clif_Detect);
        phExEMVCo_Poll_Main(psDiscLoopParams);

        /* EMVCo card removal procedure */
        phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,PHAC_DISCLOOP_POLL_STATE_REMOVAL);
        wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
        LOG_TXT("CL card removed.\n");
        phLED_SetPattern(gkphLED_BootUp);

        break;

        /* No Device/Error found. */
    case PHAC_DISCLOOP_NO_TECH_DETECTED:
        break;

        /* Listen Device found /Error found. */
    case PHAC_DISCLOOP_FAILURE:
    case PHAC_DISCLOOP_NO_DEVICE_RESOLVED:
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED :
    case PHAC_DISCLOOP_MULTI_TECH_DETECTED:
    case PHAC_DISCLOOP_EXTERNAL_RFON:
    case PHAC_DISCLOOP_COLLISION_PENDING:
    case PHAC_DISCLOOP_MERGED_SEL_RES_FOUND:
    case PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED:
        phExEMVCoEmvcoRfReset(psDiscLoopParams);
        break;
    default:
        /* It is not expected to reach here. */
        PH_USER_ASSERT(0);
    }

    /** Perform field off */
    (void)phhalHw_FieldOff(psDiscLoopParams->pHalDataParams);

    return (phStatus_t)wDiscLoopStatus;
}

void phExEMVCoClif_HalInit(void)
{
    uint16_t status;
	/* Clear the RX buf */
    phUser_MemSet((void *)&gphExEMVCo_Rxbuf[0], 0x00, PH_EXEMVCO_CLIF_RXBUFSIZE);
    /* Clear the TX buf */
    phUser_MemSet((void *)&gphExEMVCo_Txbuf[0], 0x00, PH_EXEMVCO_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
			gphExEMVCo_Txbuf,
            sizeof(gphExEMVCo_Txbuf),
			gphExEMVCo_Rxbuf,
            sizeof(gphExEMVCo_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}


phStatus_t phExEMVCoClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
    if(psDiscLoopParams->bPasPollTechCfg & ((1 << PHAC_DISCLOOP_TECH_TYPE_A) || (1 << PHAC_DISCLOOP_TECH_TYPE_B)))
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


void phExEMVCoClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib)
{

    /*switching off the rf field*/
    (void)phhalHw_FieldOff(phhalHwClifRdLib);

    /* Deinitialisng the bare metal HAL */
    (void)phhalRf_DeInit ();

}

uint16_t phExEMVCoClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts)
{

    uint16_t wStatus = PH_ERR_INTERNAL_ERROR;

    do
    {
        wStatus = phacDiscLoop_Sw_Init(psDiscLoopParams,
                                        sizeof(phacDiscLoop_Sw_DataParams_t),
                                        &sHal
                                        );
        PH_BREAK_ON_FAILURE(wStatus);

        /* Link the global HalDataParams to the pDiscLoop params.
         * NOTE: gphphhalHw_Nfc_Ic_DataParams must be initialized by this time. */
        psDiscLoopParams->pHalDataParams = (void *)&sHal;

        wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, false);
        PH_BREAK_ON_FAILURE(wStatus);

        wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, 3);
        PH_BREAK_ON_FAILURE(wStatus);

        wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG,0);
        PH_BREAK_ON_FAILURE(wStatus);

        wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG,0);
        PH_BREAK_ON_FAILURE(wStatus);

        wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG,0);
        PH_BREAK_ON_FAILURE(wStatus);

        psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts = pbAts;

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

        phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0
              | PHAC_DISCLOOP_POS_BIT_MASK_A
              | PHAC_DISCLOOP_POS_BIT_MASK_B
              | PHAC_DISCLOOP_POS_BIT_MASK_F212
              | PHAC_DISCLOOP_POS_BIT_MASK_F424
              | PHAC_DISCLOOP_POS_BIT_MASK_V
              );
    }while(0);

    return wStatus;
}

/**
* \brief Perform RF Reset as per Emvco Specification
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
void phExEMVCoEmvcoRfReset(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    /*RF Field OFF*/
    (void)phhalHw_FieldOff(psDiscLoopParams->pHalDataParams);

    /* waiting for Rf reset delay period */
    (void)phhalHw_Wait(psDiscLoopParams->pHalDataParams,PHHAL_HW_TIME_MICROSECONDS, 5100);
    /*RF Field ON*/
    (void)phhalHw_FieldOn(psDiscLoopParams->pHalDataParams);
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static phStatus_t phEXEMVCo_TxLdoMonitorCheck(void)
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXEMVCo_USETXLDO_EXTERNAL)
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
