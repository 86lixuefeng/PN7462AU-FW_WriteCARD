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
 * phExMain_NoRTOS.c: no rtos logic for phExMain
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
#include "phExMain_NoRTOS.h"
#include "phhalCt.h"
#include "phExMain_Cfg.h"
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
#include "phpalCt.h"
#include "ph_Log.h"
#include "string.h"
#include "phhalGpio.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "phExMain_Clif.h"
#include "phExMain_Ct.h"
#include "phExMain_Log.h"
#include "phhalPmu.h"
PH_NOINIT static phhalTimer_Timers_t *pLedTimer;

static phacDiscLoop_Sw_DataParams_t *psDiscLoopParams;
static phacDiscLoop_Sw_DataParams_t sDiscLoopParam;

#ifdef PHFL_ENABLE_STANDBY
static void phExMainNoRTOS_WakeUpHandler(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
                                         phhalPcr_BootReason_t eBootReason);
#else
static void phExMain_NoRTOS_Perform_CardPolling(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif /*PHFL_ENABLE_STANDBY*/
static void phExMain_NoRTOS_TxLdoMonitorEnable();

/** phExMain entry point for NON-RTOS based. */
int main(void)
{

    uint16_t wStatus;
#ifdef NXPBUILD__CLIF_RM
    uint8_t bAts[64] ;
#endif /* NXPBUILD__CLIF_RM*/

    phhalCt_InitParam_t  sInitParams;
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;
    phFlashBoot_Main();
#ifdef PHFL_ENABLE_STANDBY
    phhalPcr_BootReason_t eBootReason;
#ifdef PH_EXMAIN_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE
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
#endif /* PH_EXMAIN_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE */
#endif /* PHFL_ENABLE_STANDBY */
    LOG_TXT("BootUp\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n");

    phExMain_Clif_HalInit();
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    phLED_Init();
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
    phLED_SetPattern(gkphLED_BootUp);

    /* Initialize the CT HAL */
    PH_USER_ASSERT(phpalCt_Init(&sInitParams) == PH_ERR_SUCCESS);
    (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);
    phUser_Wait(6000);

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

    phExMain_NoRTOS_TxLdoMonitorEnable();
    while(1)
    {
#ifdef PHFL_ENABLE_STANDBY

        /** Get the boot reason and clear. */
        eBootReason = phFlashBoot_GetBtRns();
        PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

        /** Handle the WakeUp reason from Standby. */
        phExMainNoRTOS_WakeUpHandler(psDiscLoopParams, eBootReason);
#else
        /** Start card polling. */
        phExMain_NoRTOS_Perform_CardPolling(psDiscLoopParams);
        phUser_Wait(1000000);
        phLED_SetPattern(gkphLED_BootUp);
#endif /*end PHFL_ENABLE_STANDBY*/
    }

    return 0;
}

#ifdef PHFL_ENABLE_STANDBY
/**
 * Performs wakeUp handling.
 */
static void phExMainNoRTOS_WakeUpHandler(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
                                         phhalPcr_BootReason_t eBootReason)
{
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason;
    phhalPcr_WakeUpConfig_t phhalPcr_WakeUpConfig;
    phhalCt_InitParam_t  sInitParams;
    phStatus_t eStatus = PH_ERR_FAILED;

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

    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;
    LOG_U32("Wkup",eBootReason);
    phExMain_Log_BootReason(eBootReason);

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
        (void)phExMain_ClifMain(psDiscLoopParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);
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
        (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);
        phUser_Wait(6000);
        eStatus = phExMain_Ct_Example_App();
        if(PH_ERR_SUCCESS != eStatus)
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Pass);
        }
        /** Check the card absence */
        while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
        {
          /** Wait here until there is removal of card */
        }
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
        phLED_SetStatus(LED_B | LED_R | LED_Y | LED_G );
        PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);
        (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);
        phUser_Wait(6000);
        if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
        {
            /** CT card present. perform CT operations. */
            /** create the required CT queues and task. */

            eStatus = phExMain_Ct_Example_App();
            if(PH_ERR_SUCCESS != eStatus)
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Pass);
            }
            /** Check the card absence */
            while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
            {
              /** Wait here until there is removal of card */
            }
        }
        else
        {
            /** perform Clif operations. */
            /** Starts the CLIF task which executes the CLIF main example */
            (void)phExMain_ClifMain(psDiscLoopParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);

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

#else /* Non Standby case */

/**
 * Check card presence and perform operations.
 */
static void phExMain_NoRTOS_Perform_CardPolling(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    if (PH_REG_TEST_BIT(CT_MSR_REG, PRES) == 1)
    {
        LOG_TXT("CT card detected.\n");
        /** start the CT application. */
        phExMain_Ct_Example_App();
        /** Check the card absence */
        while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
        {
          /** Wait here until there is removal of card */
        }
        LOG_TXT("CT card removed.\n");
    }
    else
    {
        psDiscLoopParams->bPollState = PHAC_DISCLOOP_POLL_STATE_DETECTION;
        /** perform Clif operations. */
        /** Starts the CLIF task which executes the CLIF main example */
        (void)phExMain_ClifMain(psDiscLoopParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);
    }

}
#endif /*PHFL_ENABLE_STANDBY*/

static void phExMain_NoRTOS_TxLdoMonitorEnable()
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
