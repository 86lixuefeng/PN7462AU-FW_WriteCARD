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
 * phExEMVco_NoRTOS.c: no rtos logic for exemvco
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-12-01 14:14:28 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18767 $ (v05.19.00)
 */

#include "ph_Datatypes.h"
#include "ph_Status.h"

#ifdef PHFL_HALAPI_NO_RTOS

#include "phhalTimer.h"
#include "phExEMVCo_NoRTOS.h"
#include "phhalCt.h"
#include "phExEMVCo_Cfg.h"
#include "phFlashBoot.h"
#include "ph_Datatypes.h"
#include "phhalPcr.h"
#include "phhalRf.h"
#include "phpalFelica.h"
#include "phhalHw.h"
#include "phFlashBoot_Event.h"
#include "phhalNvic.h"
#include "phCfg_EE.h"
#include "phUser.h"
#include "phacDiscLoop.h"
#include "phalT1T.h"
#include "phpalCt.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4a.h"
#include "phpalI14443p4.h"
#include "ph_Log.h"
#include "string.h"
#include "phhalGpio.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "phExEMVCo_Clif.h"
#include "phExEMVCo_Ct.h"
#include "phExEMVCo_Log.h"
#include "phExEMVCo_Poll.h"
#include "phhalPmu.h"

PH_NOINIT static phhalTimer_Timers_t *pLedTimer;

static phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
static phacDiscLoop_Sw_DataParams_t sDiscLoopParam;

#ifdef PHFL_ENABLE_STANDBY
static void phExEMVCoNoRTOS_WakeUpHandler(phhalPcr_BootReason_t eBootReason);
#endif /*PHFL_ENABLE_STANDBY*/
static void phExEMVCoNoRTOS_TxLdoMonitorEnable();
/** phExMain entry point for NON-RTOS based. */
int main(void)
{
    phhalCt_InitParam_t  sInitParams;
    uint16_t wStatus;
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    uint8_t bAts[64] ;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/

    phFlashBoot_Main();

#ifdef PHFL_ENABLE_STANDBY
    phhalPcr_BootReason_t eBootReason;
#ifdef PH_EXEMVCO_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE
    /** Configure GPIO 8 for SWD download mode. */
    phhalPcr_ConfigInput(PH_EXEMVCO_CFG_SWD_DNLD_PIN, PH_ENABLE, PH_DISABLE, PH_DISABLE, PH_DISABLE,
            PH_DISABLE, PH_DISABLE);
    phhalPcr_ConfigPuPd(PH_EXEMVCO_CFG_SWD_DNLD_PIN, PH_DISABLE, PH_ENABLE);

    /** Wait infinitely if GPIO #PH_EXEMVCO_CFG_SWD_DNLD_PIN is equal to
     *  #PH_EXEMVCO_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE to support download through
     *  SWD before going to StandBy. See the description of #PH_EXMAIN_CFG_SWD_DNLD_PIN
     *  for more information.
     */
    while( (PH_EXEMVCO_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE << (PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXEMVCO_CFG_SWD_DNLD_PIN))
        == PH_REG_GET_FIELD_NS_SC(PCR_PADIN_REG, 1UL << (PCR_PADIN_REG_PADIN_DWL_REQ_POS + PH_EXEMVCO_CFG_SWD_DNLD_PIN)) )
    {
        ;

    }
#endif /* PH_EXEMVCO_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE */
#endif /* PHFL_ENABLE_STANDBY */
    LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    phExEMVCoClif_HalInit();
    phExEMVCoNoRTOS_TxLdoMonitorEnable();
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
    phLED_SetPattern(gkphLED_BootUp);

    /* Initialize the CT */
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

    PH_USER_ASSERT(phpalCt_Init(&sInitParams) == PH_ERR_SUCCESS);
    phUser_Wait(6000);

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

    while(1)
    {
#ifdef PHFL_ENABLE_STANDBY

        /** Get the boot reason and clear. */
        eBootReason = phFlashBoot_GetBtRns();
        PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

        /** Handle the WakeUp reason from Standby. */
        phExEMVCoNoRTOS_WakeUpHandler(eBootReason);
#else
        /** Start card polling. */
        phExEMVCo_NoRTOS_Perform_CardPolling();
        phUser_Wait(1000000);
        phLED_SetPattern(gkphLED_BootUp);
#endif /*end PHFL_ENABLE_STANDBY*/
    }
}

/**
 * Check card presence and perform operations.
 */
void phExEMVCo_NoRTOS_Perform_CardPolling()
{

    if (PH_REG_TEST_BIT(CT_MSR_REG, PRES) == 1)
    {
        LOG_TXT("CT card detected.\n");
        /** start the CT application. */
        phExEMVCo_Ct_Example_App();

        /** Check the card absence */
        while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
        {
          /** Wait here until there is removal of card */
        }

        LOG_TXT("CT card removed.\n");
        phLED_SetPattern(gkphLED_BootUp);
    }
    else
    {
        /** Perform Clif operations */
        psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
        /** Starts the CLIF task which executes the CLIF main example */
        (void)phExEMVCo_ClifMain(psDiscLoopParams,PHAC_DISCLOOP_ENTRY_POINT_POLL);
    }

}

#ifdef PHFL_ENABLE_STANDBY
/**
 * Performs wakeUp handling.
 */
static void phExEMVCoNoRTOS_WakeUpHandler(phhalPcr_BootReason_t eBootReason)
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
    phhalCt_InitParam_t  sInitParams;
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;
    LOG_U32("Wkup",eBootReason);

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

        while(1)
        {
            phLED_SetPattern(gkphLED_StandBy);
            /** All the wakeup configurations are set by default in the EEPROM. */
            phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
            /** It is not expected to reach here, if so then wait for a while and try Standby. */

        }

        break;

        /** CLIF related wakeup reasons */
    case E_RFLDT_BOOT :
        /** Starts the CLIF task which executes the CLIF main example */
        (void)phExEMVCo_ClifMain(PHAC_DISCLOOP_ENTRY_POINT_POLL);
        while(1)
               {
                   phLED_SetPattern(gkphLED_StandBy);
                   /** All the wakeup configurations are set by default in the EEPROM. */
                   phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                           &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
                   /** It is not expected to reach here, if so then wait for a while and try Standby. */

               }
        break;

          /** contact card presence. */
    case E_CT_PRESENCE :
        /** create the required CT queues and task. */
        PH_USER_ASSERT(phpalCt_Init(&sInitParams) == PH_ERR_SUCCESS);
        phUser_Wait(6000);
        phExEMVCo_Ct_Example_App();
        while(1)
               {
                   phLED_SetPattern(gkphLED_StandBy);
                   /** All the wakeup configurations are set by default in the EEPROM. */
                   phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                           &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
                   /** It is not expected to reach here, if so then wait for a while and try Standby. */

               }
        break;

        /** Host based system. */
    case E_I2C_ADDR:
    case E_HSU_INT :
    case E_SPI_ADDR:

    case E_WUC_CNT:
    {
        phhalCt_InitParam_t  sInitParams;
        sInitParams.bAutoCTDeactivationEnable= 0;
        sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
        sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
        sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

        PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);
        phUser_Wait(6000);
        if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
        {
            /** CT card present. perform CT operations. */
            /** create the required CT queues and task. */

            phExEMVCo_Ct_Example_App();
        }
        else
        {
            /** perform Clif operations. */
            /** Starts the CLIF task which executes the CLIF main example */
            (void)phExEMVCo_ClifMain(PHAC_DISCLOOP_ENTRY_POINT_POLL);

        }
        while(1)
               {
                   phLED_SetPattern(gkphLED_StandBy);
                   /** All the wakeup configurations are set by default in the EEPROM. */
                   phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                           &phhalPcr_WakeUpConfig, &sStandbyPrevReason);
                   /** It is not expected to reach here, if so then wait for a while and try Standby. */

               }
    }
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
#endif /*PHFL_ENABLE_STANDBY*/

static void phExEMVCoNoRTOS_TxLdoMonitorEnable()
{
    /* Check the configured TxLdo is External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXEMVCo_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_EXTERNAL_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXEMVCo_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXEMVCo_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

#endif
