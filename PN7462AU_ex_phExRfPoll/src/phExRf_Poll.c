/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * phExRf.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-06-23 13:52:29 +0530 (Thu, 23 Jun 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18502 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExRf_Poll.h"
#include "phhalRf.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "phUser.h"
#include "phhalTimer.h"
#include "phExRf_Poll_A.h"
#include "phExRf_Poll_B.h"
#include "phExRf_Poll_F.h"
#include "ph_Log.h"
#include "phhalGpio.h"
#include "phhalPcr.h"
#include "ph_Reg.h"
#include "phExRf_Poll_CM.h"
#include "phExRf_Poll_15693.h"
#include "phExRf_Poll_18000p3m3.h"
#include "phFlashBoot.h"
#include "phExRf_Poll_ActInit.h"
#include "phLED.h"
#include <phRtos.h>
#include "phhalSysSer.h"
#include "phCfg_EE.h"
#include "phhalPmu.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define PH_EXRFPOLL_USETXLDO_EXTERNAL               0x0
#define PH_EXRFPOLL_TXLDO_MONITOR_ENABLE            0x1
#define PH_EXRF_EVENT        1

#define NXPBUID_RF_POLL_TYPE_A_106
//#define NXPBUID_RF_POLL_TYPE_A_212
//#define NXPBUID_RF_POLL_TYPE_A_424
//#define NXPBUID_RF_POLL_TYPE_A_848

//#define NXPBUID_RF_POLL_TYPE_B_106
//#define NXPBUID_RF_POLL_TYPE_B_212
//#define NXPBUID_RF_POLL_TYPE_B_424
//#define NXPBUID_RF_POLL_TYPE_B_848
//#define NXPBUID_RF_POLL_TYPE_B_PRIME

//#define NXPBUID_RF_POLL_TYPE_F_212
//#define NXPBUID_RF_POLL_TYPE_F_424

//#define NXPBUID_RF_POLL_15693
//#define NXPBUID_RF_POLL_18000p3m3

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
#define PH_EXRF_TESTBUS1_SELECT                                   0x01
#define PH_EXRF_TESTBUS2_SELECT                                   0x02
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static uint8_t gbaphExRf_TxBuf[PH_EXRF_TX_BUFSIZE];
static uint8_t gbaphExRf_RxBuf[PH_EXRF_RX_BUFSIZE];
static void phExRFPOLL_TxLdoMonitorEnable();
static phStatus_t phExRFPOLL_TxLdoMonitorCheck();

PH_NOINIT static phRtos_EventHandle_t gphExRf_Event;

/* Not being used currently */
#if 0
const uint8_t gkphExRf_ATR_REQ[] =
{   0x25, 0xD4, 0x00, 0x03, 0x50, 0x71, 0xEF, 0x1F, 0xEE, 0xE9,
    0x58, 0xA9, 0xF9, 0x00, 0x00, 0x00, 0x32, 0x46, 0x66, 0x6D, 0x01, 0x01, 0x11,
    0x02, 0x02, 0x07, 0xFF, 0x03, 0x02, 0x00, 0x13, 0x04, 0x01, 0x64, 0x07, 0x01,
    0x03, 0x5C, 0xE7};
#endif

static phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/** Main Polling function */
static phStatus_t phExRf_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
static void phExRf_phhalRfCallBackFunc(uint32_t dwInterrupts);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

int main(void)
{
    phStatus_t status;
    /* GPIO4 = tx_active.
     * GPIO5 = rx_active. */
    uint8_t aTestBus1Config[] = { 0x02,
            PH_EXRF_TESTBUS1_SELECT };
    uint8_t aTestBus2Config[] = { 0x06,
            PH_EXRF_TESTBUS2_SELECT };
    uint32_t dwClifAnalogTestBus[] = { 0xDD, 0x00301};

#if defined NXPBUILD__CLIF_CM || defined NXPBUILD__CLIF_ACTIVE_TARGET
    uint32_t dwEventBits = 0;
#endif

#ifdef PHFL_ENABLE_STANDBY
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason;
    phhalPcr_RfLdtParams_t sRfLdParams =
    {   0, 2, 3, 0};
    phhalPcr_WakeUpConfig_t sWakeUpConfig =
    {   300, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1};
    phhalPcr_BootReason_t eBootReason;
#endif

    phFlashBoot_Main();

#ifdef PHFL_ENABLE_STANDBY
    /* Configure GPIO 8 for SWD download mode. */
    phhalPcr_ConfigInput(PH_EXRF_CFG_SWD_DNLD_PIN, PH_ENABLE, PH_DISABLE, PH_DISABLE);

    /* enable pull up for the SWD Download pin */
    phhalPcr_ConfigPuPd(PH_EXRF_CFG_SWD_DNLD_PIN, PH_ENABLE, PH_DISABLE);

    /* Wait infinitely to support download through SWD before going to app. */
    while(!PH_REG_TEST_BITN(PCR_PADIN_REG, PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXRF_CFG_SWD_DNLD_PIN))
    {
        ;
    }
#endif
    /* Enable the LED timer.
     * configure it for 100 milli secs to timeout and start running in free running mode. */

    LOG_TXT("BootUp:phExRf\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n\n");

    phExRFPOLL_TxLdoMonitorEnable();

    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS, &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100, &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

    gphExRf_Event = phRtos_EventGroupCreate();
    /* Check the External TVDD supply is OK if TxLdo is external */
    status = phExRFPOLL_TxLdoMonitorCheck();
    if (status != PH_ERR_SUCCESS)
    {
    	phLED_SetStatus(LED_R);
    	LOG_TXT("phRfPoll: No External TVDD Present \n");
    	return 1;
    }

    phhalRf_RegCallBack(&phExRf_phhalRfCallBackFunc,
    CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK);

    /* Enable the Digital Test Bus. */
    //aTestBus1Config[0] = gpkphCfg_EE_HW_RfTestBus->bTbDigi1;
    status = phhalSysSer_CLIF_Config_DigitalTestBus((void *) aTestBus1Config);
    if (status != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
    }

    status = phhalSysSer_CLIF_Config_DigitalTestBus((void *) aTestBus2Config);
    if (status != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
    }

    status = phhalSysSer_CLIF_Config_AnalogTestBus(&dwClifAnalogTestBus[0]);
    if (status != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
    }


    while (1)
    {

#ifdef PHFL_ENABLE_STANDBY
        /* Get the boot reason. */
        eBootReason = phFlashBoot_GetBtRns();
        /* Clear the existing boot reasons to avoid each module to clear specifically. */
        PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

        if(!(eBootReason == E_WUC_CNT))
        {
#endif

#if 0
        /** Chk for ext field presence */
        if(phhalRf_GetExtFieldStatus() == PH_ERR_SUCCESS)
        {
            LOG_TXT("Ext Field ON\n");
            /** If we have external field, enable card mode \see phExRf_CM_Activate  */
            status = phExRf_CM_Activate(gbaphExRf_TxBuf, gbaphExRf_RxBuf, (uint16_t)PH_EXRF_RX_BUFSIZE);
            if (((status & PH_ERR_MASK) == PH_ERR_RFOFF_EXT)
                    || ((status & PH_ERR_MASK) == PH_ERR_RF_ACTIVE))
            {
                LOG_TXT("Ext Field OFF\n");
            }
        }
#endif

#ifdef PHFL_ENABLE_STANDBY
    }
    else
    {
#endif
        if (phhalRf_GetExtFieldStatus() != PH_ERR_SUCCESS)
        {
            phExRf_Poll(gbaphExRf_TxBuf, gbaphExRf_RxBuf);
            phhalRf_FieldOff();
            phUser_Wait(5100);
            LOG_TXT_D("RF OFF\n");
        }
        else
        {
            LOG_TXT("Ext Field ON\n");
#if defined NXPBUILD__CLIF_CM || defined NXPBUILD__CLIF_ACTIVE_TARGET
            /** If we have external field, enable card mode \see phExRf_CM_Activate  */
            status = phExRf_CM_Activate(gbaphExRf_TxBuf, gbaphExRf_RxBuf, (uint16_t) PH_EXRF_RX_BUFSIZE);
                if ((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
            {
                LOG_TXT("Ext Field OFF\n");
            }
#else
            LOG_TXT("Target Mode not supported. Hence waiting for external field to go off.\n");
            while(phhalRf_GetExtFieldStatus() == PH_ERR_SUCCESS)
            {
                __WFE();
            }
#endif
        }

#if defined NXPBUILD__CLIF_CM || defined NXPBUILD__CLIF_ACTIVE_TARGET
        phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_ENABLE);
        dwEventBits = phRtos_EventGroupWaitBits(gphExRf_Event, PH_EXRF_EVENT, true, false, 20);
        if (dwEventBits & (~0x80000000))
        {
            LOG_TXT("Ext Field ON\n");
            /** If we have external field, enable card mode \see phExRf_CM_Activate  */
            status = phExRf_CM_Activate(gbaphExRf_TxBuf, gbaphExRf_RxBuf, (uint16_t) PH_EXRF_RX_BUFSIZE);
                if ((status & PH_ERR_MASK) == PH_ERR_EXT_RF_ERROR)
            {
                LOG_TXT("Ext Field OFF\n");
            }
        }
        phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_DISABLE);
#endif

#ifdef PHFL_ENABLE_STANDBY
    }

    while(1)
    {
        phLED_SetPattern(gkphLED_StandBy);
        /* Set the wakeup configurations */
        phhalPcr_EnterLowPowerMode(&sRfLdParams, &sWakeUpConfig, &sStandbyPrevReason);
        /* It is not expected to reach here, if so then Standby prevention has occured,
         * wait for a while, perform operation and try Standby. */
        phUser_Wait(300000);
        break;
    }

#else
        /* 300 milli secs */
        /*        phUser_Wait(300000);*/

#endif
    }

    return 0;
    return 0;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static phStatus_t phExRf_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phStatus_t status = PH_ERR_INVALID_PARAMETER;

#if(PH_EXRF_ENABLE_LPCD == 1)
    uint32_t dwLPCDThreshold = 0xFFFF;
    static uint32_t dwLPCDRefValue = 0xFFFF;
    uint32_t dwLPCDAgcValue;
#endif

#if(PH_EXRF_ENABLE_LPCD == 1)
    /*Check for card presence */
    status = phhalRf_LPCD(dwLPCDThreshold, dwLPCDRefValue, &dwLPCDAgcValue);
    if( status!= PH_ERR_SUCCESS){
        return status;
    }
#endif

    do
    {

#ifdef NXPBUILD__CLIF_ACTIVE_INITIATOR
        LOG_TXT("\nActive Polling Start\n");

        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_ACT_106, E_PHHAL_RF_LP_I_RX_ACT_106);
        PH_BREAK_ON_FAILURE(status)

        status = phhalRf_FieldOn();
        PH_BREAK_ON_FAILURE(status)

        status = phExRf_ActInit(pTxBuffer, pRxBuffer);
        PH_BREAK_ON_SUCCESS(status)

        status = phhalRf_FieldReset(5000, 0);
        if (status != PH_ERR_SUCCESS)
        {
            LOG_TXT_E("Field ON Failed\n");
            return status;
        }
#endif

#ifdef NXPBUILD__CLIF_RM
        LOG_TXT("\nPolling Start\n");

        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_A_106_P);
        PH_BREAK_ON_FAILURE(status)

#ifndef NXPBUILD__CLIF_ACTIVE_INITIATOR
        status = phhalRf_FieldOn();
        PH_BREAK_ON_FAILURE(status)
#endif /* NXPBUILD__CLIF_ACTIVE_INITIATOR */

#ifdef NXPBUID_RF_POLL_TYPE_A_106
        status = phExRf_A_106_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_A_212
        status = phExRf_A_212_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_A_424
        status = phExRf_A_424_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_A_848
        status = phExRf_A_848_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_B_106
        status = phExRf_B_106_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_B_212
        status = phExRf_B_212_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_B_424
        status = phExRf_B_424_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_B_848
        status = phExRf_B_848_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_B_PRIME
        status = phExRf_B_Prime_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_F_212
        status = phExRf_F_212_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_TYPE_F_424
        status = phExRf_F_424_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_15693
        status = phExRf_15693_Poll(pTxBuffer, pRxBuffer);
#endif
#ifdef NXPBUID_RF_POLL_18000p3m3
        status = phExRf_18000p3m3_Poll(pTxBuffer, pRxBuffer);
#endif

#endif /* NXPBUILD__CLIF_RM */
#if (PH_EXRF_ENABLE_LPCD == 1)
        /* LPCD is pass but no card found, update the Ref value */
        if(status == PH_ERR_EXRF_CARD_NOT_FOUND){
            LOG_TXT("LPCD ERROR\n");
            dwLPCDRefValue = dwLPCDAgcValue;
        }
#endif

    } while (0);

    return status;
}

static void phExRf_phhalRfCallBackFunc(uint32_t dwInterrupts)
{
    if (dwInterrupts & CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK)
    {
        phRtos_EventGroupSetBits(gphExRf_Event, PH_EXRF_EVENT);
    }
    else
    {
        /* Do Nothing */
        ;
    }
}

static void phExRFPOLL_TxLdoMonitorEnable()
{
    /* Check the configured TxLdo is External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXRFPOLL_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_EXTERNAL_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXRFPOLL_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXRFPOLL_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

static phStatus_t phExRFPOLL_TxLdoMonitorCheck()
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXRFPOLL_USETXLDO_EXTERNAL)
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
