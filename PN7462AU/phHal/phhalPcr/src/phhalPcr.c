/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2013,2014
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
 * Pcr hal implementation
 *
 * Project:  PN7462AU
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 18485 $ (v04.13.00)
 * $Date: 2016-06-22 11:25:52 +0530 (Wed, 22 Jun 2016) $
 *
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_PCR
#include "phhalNvic.h"
#include "phhalPcr.h"
#include "phhalPmu.h"
#include "phhalGpio.h"
#include "phUser.h"
#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "PN7462AU/PN7462AU_clkgen.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "PN7462AU/PN7462AU_pcr.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalCt.h"
#endif


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define PHHAL_PCR_GET_HIF()                             PH_REG_GET_FIELD(PCR_SYS_REG,HIF_SELECTION)
#define PHHAL_PCR_MAX_I2C_ADDR                          0x7F
#define PHHAL_PCR_USB_SUSPEND_TIMEOUT                   5000   /* 5 msec */
#define PHHAL_PCR_USB_SUSPEND_POLLING_FREQUENCY         100   /* 100 uS */
#define PHHAL_PCR_RFLDT_COMPARATOR_DELAY                250   /* 250 uS */
#define PHHAL_PCR_NUM_GPIO                              12
#define PHHAL_PCR_NUM_ATX_PADS                          4
#define PHHAL_PCR_TRIM_VALUES_MASK                      0x7FF
#define PHHAL_PCR_ADV_RFLD_VALUE                        0x631329

/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

/**
 * Internal Structure used for backing up register values before Suspend operation, which will be restored after
 * exiting Suspend.
 */
typedef struct {
    uint32_t pcr_padgpio1_reg;  /**< Backup for PCR_PADGPIO1_REG */
    uint32_t pcr_padgpio2_reg;  /**< Backup for PCR_PADGPIO2_REG */
    uint32_t pcr_padgpio3_reg;  /**< Backup for PCR_PADGPIO3_REG */
    uint32_t pcr_padgpio4_reg;  /**< Backup for PCR_PADGPIO4_REG */
    uint32_t pcr_padgpio5_reg;  /**< Backup for PCR_PADGPIO5_REG */
    uint32_t pcr_padgpio6_reg;  /**< Backup for PCR_PADGPIO6_REG */
    uint32_t pcr_padgpio7_reg;  /**< Backup for PCR_PADGPIO7_REG */
    uint32_t pcr_padgpio8_reg;  /**< Backup for PCR_PADGPIO8_REG */
    uint32_t pcr_padgpio9_reg;  /**< Backup for PCR_PADGPIO9_REG */
    uint32_t pcr_padgpio10_reg;  /**< Backup for PCR_PADGPIO10_REG */
    uint32_t pcr_padgpio11_reg;  /**< Backup for PCR_PADGPIO11_REG */
    uint32_t pcr_padgpio12_reg;  /**< Backup for PCR_PADGPIO12_REG */
    uint32_t pcr_pad_atx_a_reg;  /**< Backup for PCR_PAD_ATX_A_REG */
    uint32_t pcr_pad_atx_b_reg;  /**< Backup for PCR_PAD_ATX_B_REG */
    uint32_t pcr_pad_atx_c_reg;  /**< Backup for PCR_PAD_ATX_C_REG */
    uint32_t pcr_pad_atx_d_reg;  /**< Backup for PCR_PAD_ATX_D_REG */
    uint32_t pcr_pad_int_aux_reg;  /**< Backup for PCR_PAD_INT_AUX_REG */
    uint32_t pcr_pad_io_aux_reg;  /**< Backup for PCR_PAD_IO_AUX_REG */
    uint32_t pcr_pad_clk_aux_reg;  /**< Backup for PCR_PAD_CLK_AUX_REG */
    uint32_t pcr_spim_reg;  /**< Backup for PCR_SPIM_REG */
    uint32_t pcr_padiicm_reg;  /**< Backup for PCR_PADIICM_REG */
    uint32_t pcr_clk_cfg_reg;  /**< Backup for PCR_CLK_CFG_REG */
    uint32_t clkgen_clif_pll_global_control_reg;  /**< Backup for CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG */
    uint32_t clif_ana_cm_config_reg;  /**< Backup for CLIF_ANA_CM_CONFIG_REG */
    uint32_t clif_ana_pbf_control_reg;  /**< Backup for CLIF_ANA_PBF_CONTROL_REG */
    uint32_t clif_ana_rx_reg;  /**< Backup for CLIF_ANA_RX_REG */
    uint32_t pcr_ana_tx_standby_reg;  /**< Backup for PCR_ANA_TX_STANDBY_REG */
}phhalPcr_RegisterBackUp_t;

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: 132 bytes
 * ***************************************************************************************************************** */
PH_NOINIT static uint8_t gbphHalPcrLowTempTarget0;   /* Define what is the "low" temperature level */
PH_NOINIT static uint8_t gbphHalPcrHighTempTarget0;  /* Define what is the "High" temperature level */

PH_NOINIT static uint8_t gbphHalPcrLowTempTarget1;   /* Define what is the "low" temperature level */
PH_NOINIT static uint8_t gbphHalPcrHighTempTarget1;  /* Define what is the "High" temperature level */

PH_NOINIT static uint32_t gdwphHalPcrAnaTxStandByValue;
PH_NOINIT static uint32_t gdwphHalPcrAnaTxProtStandByValue;

PH_NOINIT static pphhalPcr_CallbackFunc_t gpphhalPcr_CallbackFunc;
PH_NOINIT static uint32_t gdwphHalPcrRegIntrpts;

PH_NOINIT static uint32_t gdwphhalPcr_PwrDownSettingSelect; /* 32 bit Bit-file, to store the settings requested for
                                                               lowering power consumption. This variable will be used
                                                               when restoring the settings done. */


PH_NOINIT static phhalPcr_RegisterBackUp_t gdwphhalPcr_RegisterBackUp; /* variable to backup only the registers selected
                                                                for change*/
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

static phStatus_t phhalPcr_ApplyLowPower(phhalPcr_WakeUpConfig_t* psWakeUpConfig,
                                            phhalPcr_StandbyPrevReason_t* psPreventionReason);

static phStatus_t phhalPcr_ConfigWakeup(phhalPcr_RfLdtParams_t* psRfLdtParams,
                                            phhalPcr_WakeUpConfig_t* psWakeUpConfig);

static void phhalPcr_PreConfigLowPowerEntry(uint32_t dwAnaTxInStandbyMode, uint32_t dwAnaTxProtInStandbyMode,
                                            phhalPcr_WakeUpConfig_t* psWakeUpConfig);

static void phhalPcr_PostConfigLowPowerExit(uint32_t dwAnaTxInNormalMode, uint32_t dwAnaTxProtInNormalMode,
                                            phhalPcr_WakeUpConfig_t* psWakeUpConfig);

static phStatus_t phhalPcr_ApplyStandby(phhalPcr_StandbyPrevReason_t* psStandbyPrevReason);

static phStatus_t phhalPcr_ApplySuspend(phhalPcr_StandbyPrevReason_t* psSuspendPrevReason);

static phStatus_t phhalPcr_ReducePowerConsumption(uint32_t ePwrDownSettingSelect);

static phStatus_t phhalPcr_RestorePowerConsumptionChanges(void);

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
static phStatus_t phhalPcr_CheckWakeUpParams(phhalPcr_WakeUpConfig_t* psWakeUpConfig);
#endif

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalPcr_Init(phhalPcr_PwrTempConfig_t * phhalPcr_PwrTempConfig,
                     phhalPcr_TxAnaStandByConfig_t* psTxAnaStandByConfig,
                     uint32_t dwPwrDownSettingSelect)
{

    uint32_t dwClockConfig = PHHAL_PCR_CLK_CONFIGURATION;
    uint32_t dwTrimValues  = PH_REG_GET(PCR_CLK_CFG_REG);

    /* Retain the trim values, mask the trim values for higher bits  */
    dwTrimValues &= PHHAL_PCR_TRIM_VALUES_MASK;
    dwClockConfig = (dwClockConfig | dwTrimValues);

    /* Apply the value */
    PH_REG_SET(PCR_CLK_CFG_REG,dwClockConfig);

    gpphhalPcr_CallbackFunc = NULL;
    gdwphHalPcrRegIntrpts = 0;

    /* Save the standby values to be applied for the AN TX and ANA TX PROT */
    gdwphHalPcrAnaTxStandByValue = psTxAnaStandByConfig->dwAnaTxStandByValue ;
    gdwphHalPcrAnaTxProtStandByValue = psTxAnaStandByConfig->dwAnaTxProtStandByValue ;

    /* Save the temperature sensor hysterisis values */
    gbphHalPcrLowTempTarget0  = phhalPcr_PwrTempConfig->bLowTempTarget0;
    gbphHalPcrHighTempTarget0 = phhalPcr_PwrTempConfig->bHighTempTarget0;

    gbphHalPcrLowTempTarget1  = phhalPcr_PwrTempConfig->bLowTempTarget1;
    gbphHalPcrHighTempTarget1 = phhalPcr_PwrTempConfig->bHighTempTarget1;

    /* save given setting flags to know which registers must be restored */
    gdwphhalPcr_PwrDownSettingSelect = dwPwrDownSettingSelect;

    /* Disable both the temperature sensor */
    PH_REG_SET_FIELDS_NS(PCR_TEMP_REG,PCR_TEMP_REG_TEMP_ENABLE_0_MASK | PCR_TEMP_REG_TEMP_ENABLE_1_MASK, 0x0);

    /* Disable the all PCR interrupts */
    PH_REG_SET(PCR_INT_CLR_ENABLE_REG,0xFFFFFFFF);

    if (phhalPcr_PwrTempConfig->bUseTempSensor0)
    {
        /* Configure temperature sensor 0 */
        PH_REG_SET_FIELD(PCR_TEMP_REG, TEMP_DELTA_0, phhalPcr_PwrTempConfig->bHighTempTarget0);
        PH_REG_SET_BIT(PCR_TEMP_REG, TEMP_ENABLE_0);
    }

    if (phhalPcr_PwrTempConfig->bUseTempSensor1)
    {
        /* Configure temperature sensor 1 */
        PH_REG_SET_FIELD(PCR_TEMP_REG, TEMP_DELTA_1, phhalPcr_PwrTempConfig->bHighTempTarget1);
        PH_REG_SET_BIT(PCR_TEMP_REG, TEMP_ENABLE_1);
    }

    /* Enable the temperature interrupts */
    PH_REG_SET(PCR_INT_SET_ENABLE_REG,(PCR_INT_SET_ENABLE_REG_TEMPERROR1_INT_SET_ENABLE_MASK |
                                               PCR_INT_SET_ENABLE_REG_TEMPERROR0_INT_SET_ENABLE_MASK));

    /* Reset the wake up configuration value of Gpio's to 0 */
    PH_REG_SET(PCR_GPIO_WAKEUP_CFG_REG, 0x00);

    /* Enables the NVIC for PCR Interface */
    PH_REG_SET(NVIC_ISER_REG, (1 << PHHAL_NVIC_PCR));

    return PH_ERR_SUCCESS;
}


phStatus_t phhalPcr_EnterLowPowerMode(phhalPcr_RfLdtParams_t* psRfLdtParams,
                                        phhalPcr_WakeUpConfig_t* psWakeUpConfig,
                                        phhalPcr_StandbyPrevReason_t* psPreventionReason)
{
    phStatus_t eStatus;
    bool blRestoreSysTickInt;

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    PH_RETURN_IF_TRUE_WITH_ERROR(((NULL == psPreventionReason) || (NULL == psWakeUpConfig)), INVALID_PARAMETER, PCR);
    eStatus = phhalPcr_CheckWakeUpParams(psWakeUpConfig);
    PH_RETURN_ON_FAILURE(eStatus);
#endif


    /* Enter Critical Section.
     * This is to ensure that the sequence to place IC in low power mode is not interrupted, till the point
     * just before the actual entry into low power mode.
     * Note that the Critical section exit API is called just before entry into low power mode */
    __phUser_EnterCriticalSection();

   /* If SysTick exception is enabled, disable it.
    * This is to ensure that the SysTick exception will not occur during the actual entry into low power mode.
    * See Ticket artf251456. */
    blRestoreSysTickInt = false;
    if(PH_REG_TEST_BIT(SYST_CSR_REG,TICKINT))
    {
        blRestoreSysTickInt = true;
        PH_REG_CLEAR_BIT(SYST_CSR_REG,TICKINT);
    }


    /* Configure  wake up sources and related configuration */
    eStatus = phhalPcr_ConfigWakeup(psRfLdtParams,psWakeUpConfig);

    /* Everything is success apply the standby/suspend */
    if (eStatus == PH_ERR_SUCCESS)
    {
        eStatus = phhalPcr_ApplyLowPower(psWakeUpConfig,psPreventionReason);
    }

    /* If SysTick exception was previously enabled, re-enable it */
    if(blRestoreSysTickInt)
    {
        PH_REG_SET_BIT(SYST_CSR_REG,TICKINT);
    }

    /* Irrespective of the status exit the critical section
     * This may be due to entering into standby failed, IC came out of suspend or the config wake up was not success */
    /* Exit Critical Section */
    __phUser_ExitCriticalSection();

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PCR);
}


phStatus_t phhalPcr_RegCallBack(pphhalPcr_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    if ((pCallBackFunc == NULL) || (dwInterrupts == 0)){
        return PH_ERR(INVALID_PARAMETER,PCR);
    }
#endif

    gpphhalPcr_CallbackFunc = pCallBackFunc;
    gdwphHalPcrRegIntrpts = dwInterrupts;

    return PH_ERR_SUCCESS;
}

/*
 *Function Name     : phhalPcr_Isr
 *Description       : Interrupt service routine for the Pcr module
 *
 *Input Parameters  : None
 *
 *Output Parameters : None
 *
 */
void PCR_IRQHandler(void)
{
    phhalPcr_WakeUpConfig_t sWakeUpConfig = {0x00};
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason;
    phhalPcr_RfLdtParams_t phhalPcr_RfLdtParams = {0x00};
    uint32_t dwPcrIsrStatus;

    /* Get the ISR Status */
    dwPcrIsrStatus = (PH_REG_GET(PCR_INT_STATUS_REG));

    /* Clear the Pending Interrupt Status */
    PH_REG_SET(PCR_INT_CLR_STATUS_REG, dwPcrIsrStatus);

    /*Mask the non-enabled interrupts*/
    dwPcrIsrStatus &= (PH_REG_GET(PCR_INT_ENABLE_REG));

    /* Check the interrupt is from Temperature error 0 and 1 */
    if ((dwPcrIsrStatus & PCR_INT_STATUS_REG_TEMPERROR0_INT_STATUS_MASK) ||
        (dwPcrIsrStatus & PCR_INT_STATUS_REG_TEMPERROR1_INT_STATUS_MASK))
    {
#ifdef NXPBUILD__PHHAL_CT
        phhalCt_AsyncShutDown();
        /* Shut down the CT in case it is activated or auto deactivation is called  */
        phhalCt_CardDeactivate();
#endif
        /* Go to standby mode until the temperature does come to a lower value */
        sWakeUpConfig.bWakeUpTemp = 0x01;
        (void)phhalPcr_EnterLowPowerMode(&phhalPcr_RfLdtParams,&sWakeUpConfig, &sStandbyPrevReason);
        return;
    }
    /* Check the interrupt is from VBUS monitor going low */
    if (dwPcrIsrStatus & PCR_INT_STATUS_REG_VBUS_MON_LOW_INT_STATUS_MASK)
    {

#ifdef NXPBUILD__PHHAL_CT
        phhalCt_AsyncShutDown();
        /* Shut down the CT in case it is activated or auto deactivation is called  */
        phhalCt_CardDeactivate();
#endif
        /* Clear the boot register */
        PH_REG_SET_BIT_WO(PCR_CTRL_REG,CLR_BOOT_REGS);
        /* Set the automatic HPD */
        PH_REG_SET_BIT(PCR_SYS_REG,AUTOMATIC_HPD);
        return;
    }
    /* Check the interrupt is from VBUSP monitor going low */
    if (dwPcrIsrStatus & PCR_INT_STATUS_REG_VBUSP_MON_LOW_INT_STATUS_MASK)
    {
#ifdef NXPBUILD__PHHAL_CT
        phhalCt_AsyncShutDown();
        /* Shut down the CT in case it is activated or auto deactivation is called  */
        phhalCt_CardDeactivate();
#endif
    }
    /* Check the interrupt is from PVDD current limiter active */
    if (dwPcrIsrStatus & PCR_INT_STATUS_REG_PVDD_CURLIM_ACT_INT_STATUS_MASK)
    {
        (void)phhalPmu_PvddLdoStop();
    }

    if(gpphhalPcr_CallbackFunc && (gdwphHalPcrRegIntrpts & dwPcrIsrStatus))
    {
        gpphhalPcr_CallbackFunc(dwPcrIsrStatus);
    }

    return;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/*
 *Function Name     : phhalPcr_ReducePowerConsumption
 *Description       : Reduces power consumption by pulling down selected Pads, disabling selected clocks and modules,etc
 *
 *Input Parameters  : ePwrDownSettingSelect -> 32 bit bit-file created by ORing enums of type phhalPcr_PwrDown_Setting_t
                      Used to select which settings must be applied to reduce power consumption.
 *
 *Output Parameters : None
 *
 */
static phStatus_t phhalPcr_ReducePowerConsumption(uint32_t ePwrDownSettingSelect)
{
    uint32_t dwIndex;
    uint32_t *pdwBackUpPtr;
    uint32_t dwRegAddress;

    PH_REG_SET_BIT(CLIF_ANA_TX_AMPLITUDE_REG, TX_PD); /* See PT_SC3803 */

    /* If no changes are to be done to reduce power consumption, just return */
    if(ePwrDownSettingSelect != 0)
    {
        /* If any of the 12 GPIO pads must be pulled down, save the current register value before pulling down the pad*/
        pdwBackUpPtr = &(gdwphhalPcr_RegisterBackUp.pcr_padgpio1_reg);
        for(dwIndex=0; dwIndex < PHHAL_PCR_NUM_GPIO; ++dwIndex )
        {
            if (ePwrDownSettingSelect & (uint32_t)(E_PD_GPIO1 << dwIndex))
            {
                dwRegAddress = PCR_PADGPIO1_REG + (dwIndex*sizeof(uint32_t));
                *(pdwBackUpPtr + dwIndex) = PH_REG_GET_SC(dwRegAddress);
                PH_REG_SET_SC(dwRegAddress, PCR_PADGPIO1_REG_GPIO1_PUPD_MASK);
            }
        }

        /* If any of the 4 ATX pads must be pulled down, save the current register value before pulling down the pad*/
        pdwBackUpPtr = &(gdwphhalPcr_RegisterBackUp.pcr_pad_atx_a_reg);
        for(dwIndex=0; dwIndex < PHHAL_PCR_NUM_ATX_PADS; ++dwIndex )
        {
            if (ePwrDownSettingSelect & (uint32_t)(E_PD_ATXA << dwIndex))
            {
                dwRegAddress = PCR_PAD_ATX_A_REG + (dwIndex*sizeof(uint32_t));
                *(pdwBackUpPtr + dwIndex) = PH_REG_GET_SC(dwRegAddress);
                PH_REG_SET_SC(dwRegAddress, PCR_PAD_ATX_A_REG_ATX_A_PUPD_MASK);
            }
        }

        /* If INT_AUX pad must be pulled down, save the current register value before pulling down the pad */
        if(ePwrDownSettingSelect & E_PD_INT_AUX)
        {
            gdwphhalPcr_RegisterBackUp.pcr_pad_int_aux_reg = PH_REG_GET(PCR_PAD_INT_AUX_REG);
            PH_REG_SET(PCR_PAD_INT_AUX_REG,
                    (PCR_PAD_INT_AUX_REG_INT_AUX_PUPD_MASK      |
                    PCR_PAD_INT_AUX_REG_INT_AUX_SW_ENABLE_MASK  |
                    PCR_PAD_INT_AUX_REG_INT_AUX_GPIOMODE_EN_MASK));
        }

        /* If IO_AUX pad must be pulled down, save the current register value before pulling down the pad */
        if(ePwrDownSettingSelect & E_PD_IO_AUX)
        {
            gdwphhalPcr_RegisterBackUp.pcr_pad_io_aux_reg = PH_REG_GET(PCR_PAD_IO_AUX_REG);
            PH_REG_SET(PCR_PAD_IO_AUX_REG,
                       (PCR_PAD_IO_AUX_REG_IO_AUX_PUPD_MASK       |
                        PCR_PAD_IO_AUX_REG_IO_AUX_SW_ENABLE_MASK  |
                        PCR_PAD_IO_AUX_REG_IO_AUX_GPIOMODE_EN_MASK));
        }

        /* If CLK_AUX pad must be pulled down, save the current register value before pulling down the pad */
        if(ePwrDownSettingSelect & E_PD_CLK_AUX)
        {
            gdwphhalPcr_RegisterBackUp.pcr_pad_clk_aux_reg = PH_REG_GET(PCR_PAD_CLK_AUX_REG);
            PH_REG_SET(PCR_PAD_CLK_AUX_REG,
                       (PCR_PAD_CLK_AUX_REG_CLK_AUX_PUPD_MASK       |
                          PCR_PAD_CLK_AUX_REG_CLK_AUX_SW_ENABLE_MASK  |
                        PCR_PAD_CLK_AUX_REG_CLK_AUX_GPIOMODE_EN_MASK));
        }

        /* If SPIM related pads must be pulled down, save the current register value before pulling down the pads */
        if(ePwrDownSettingSelect & E_PD_SPIM)
        {
            gdwphhalPcr_RegisterBackUp.pcr_spim_reg = PH_REG_GET(PCR_SPIM_REG);
            PH_REG_SET(PCR_SPIM_REG,
                       (PCR_SPIM_REG_SPIM_SW_ENABLE_MASK |
                        PCR_SPIM_REG_SPIM_SCK_EPUD_MASK  |
                        PCR_SPIM_REG_SPIM_NSS_EPUD_MASK  |
                        PCR_SPIM_REG_SPIM_MOSI_EPUD_MASK |
                        PCR_SPIM_REG_SPIM_MISO_EPUD_MASK));
        }

        /* If IICM related pads must be pulled down, save the current register value before pulling down the pads */
        if(ePwrDownSettingSelect & E_PD_IICM)
        {
            gdwphhalPcr_RegisterBackUp.pcr_padiicm_reg = PH_REG_GET(PCR_PADIICM_REG);
            PH_REG_SET(PCR_PADIICM_REG,
                       (PCR_PADIICM_REG_SCL_PUPD_MASK  |
                        PCR_PADIICM_REG_SDA_PUPD_MASK  |
                        PCR_PADIICM_REG_IICM_SW_ENABLE_MASK));
        }

        /* If CT Clocks must be disabled, save the current register values before disabling clocks */
        if(ePwrDownSettingSelect & E_DISABLE_CT_CLK)
        {
            gdwphhalPcr_RegisterBackUp.pcr_clk_cfg_reg = PH_REG_GET(PCR_CLK_CFG_REG);
            PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG,CLOCK_CTIF_ENABLE);
            PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG,IPCLOCK_CTIF_ENABLE);
        }

        /* If CLIF modules must be disabled, save the current register values before disabling modules */
        if(ePwrDownSettingSelect & E_DISABLE_CLIF)
        {
            gdwphhalPcr_RegisterBackUp.clkgen_clif_pll_global_control_reg =
                                        PH_REG_GET(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG);
            PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,CLIF_PLL_ENABLE);

            /* CLIF ANA CM */
            gdwphhalPcr_RegisterBackUp.clif_ana_cm_config_reg = PH_REG_GET(CLIF_ANA_CM_CONFIG_REG);
            PH_REG_SET(CLIF_ANA_CM_CONFIG_REG,
                       (CLIF_ANA_CM_CONFIG_REG_CM_PD_DLL_CP_MASK      |
                        CLIF_ANA_CM_CONFIG_REG_CM_PD_DLL_MASK         |
                        CLIF_ANA_CM_CONFIG_REG_CM_PD_CLKREC_MASK      |
                        CLIF_ANA_CM_CONFIG_REG_CM_PD_MIXER_Q_MASK     |
                        CLIF_ANA_CM_CONFIG_REG_CM_PD_MIXER_I_MASK     |
                        CLIF_ANA_CM_CONFIG_REG_CM_MILLER_EN_LOCK_MASK |
                        CLIF_ANA_CM_CONFIG_REG_CM_PD_MILLER_MASK      |
						CLIF_ANA_CM_CONFIG_REG_CM_PD_RSSI_MASK));

            PH_REG_SET_BIT(CLIF_ANA_NFCLD_REG, CM_PD_NFC_DET);

        	PH_REG_CLEAR_BIT(CLKGEN_CLOCK_PRESENCE_BYPASS_REG, CLOCK_PRESENCE_BYPASS_ENABLE);


            /* CLIF ANA PBF */
            gdwphhalPcr_RegisterBackUp.clif_ana_pbf_control_reg = PH_REG_GET(CLIF_ANA_PBF_CONTROL_REG);
            PH_REG_SET(CLIF_ANA_PBF_CONTROL_REG,
                       (CLIF_ANA_PBF_CONTROL_REG_PBF_EN_LIMVDHF_MASK      |
                        CLIF_ANA_PBF_CONTROL_REG_PBF_PD_REG_MASK          |
                        CLIF_ANA_PBF_CONTROL_REG_PBF_SHORT_SELECT_RM_MASK));

            /* CLIF ANA RX */
            gdwphhalPcr_RegisterBackUp.clif_ana_rx_reg = PH_REG_GET(CLIF_ANA_RX_REG);
            PH_REG_SET(CLIF_ANA_RX_REG,
                       (CLIF_ANA_RX_REG_RX_PD_VMID_MASK       |
                        CLIF_ANA_RX_REG_RX_PD_BBA_I_MASK      |
                        CLIF_ANA_RX_REG_RX_PD_BBA_Q_MASK      |
                        CLIF_ANA_RX_REG_RX_PD_BBA_BIAS_MASK   |
                        CLIF_ANA_RX_REG_RX_PD_ADC_I_MASK      |
                        CLIF_ANA_RX_REG_RX_PD_ADC_Q_MASK      |
                        CLIF_ANA_RX_REG_RX_PD_MIXER_MASK      |
                        CLIF_ANA_RX_REG_RX_PD_TEST_CLK_MASK));
        }

        /* If GSN value in Standby mode must be changed, save the current register value */
        if(ePwrDownSettingSelect & E_PCR_ANA_TX_STANDBY_GSN)
        {
            gdwphhalPcr_RegisterBackUp.pcr_ana_tx_standby_reg = PH_REG_GET(PCR_ANA_TX_STANDBY_REG);
            PH_REG_SET(PCR_ANA_TX_STANDBY_REG, PCR_ANA_TX_STANDBY_REG_TX_GSN_CW_SB_MASK);
        }

    }

    return PH_ERR_SUCCESS;
}

/*
 *Function Name     : phhalPcr_RestorePowerConsumptionChanges
 *Description       : Restores all the changes that were done by the last call to phhalPcr_ReducePowerConsumption() API
 *
 *Input Parameters  : None
 *
 *Output Parameters : None
 *
 */
static phStatus_t phhalPcr_RestorePowerConsumptionChanges(void)
{
    uint32_t dwIndex;
    uint32_t *pdwBackUpPtr;
    uint32_t dwRegAddress;

    /* If no changes were done to reduce power consumption, just return */
    if(gdwphhalPcr_PwrDownSettingSelect != 0)
    {
        /* If any of the 12 GPIO pads were pulled down, restore their previous state */
        pdwBackUpPtr = &(gdwphhalPcr_RegisterBackUp.pcr_padgpio1_reg);
        for(dwIndex=0; dwIndex < PHHAL_PCR_NUM_GPIO; ++dwIndex )
        {
            if (gdwphhalPcr_PwrDownSettingSelect & (uint32_t)(E_PD_GPIO1 << dwIndex))
            {
                dwRegAddress = PCR_PADGPIO1_REG + (dwIndex*sizeof(uint32_t));
                PH_REG_SET_SC(dwRegAddress, *(pdwBackUpPtr + dwIndex));
            }
        }

        /* If any of the 4 ATX pads were pulled down, restore their previous state */
        pdwBackUpPtr = &(gdwphhalPcr_RegisterBackUp.pcr_pad_atx_a_reg);
        for(dwIndex=0; dwIndex < PHHAL_PCR_NUM_ATX_PADS; ++dwIndex )
        {
            if (gdwphhalPcr_PwrDownSettingSelect & (uint32_t)(E_PD_ATXA << dwIndex))
            {
                dwRegAddress = PCR_PAD_ATX_A_REG + (dwIndex*sizeof(uint32_t));
                PH_REG_SET_SC(dwRegAddress, *(pdwBackUpPtr + dwIndex));
            }
        }

        /* If INT_AUX pad was pulled down, restore it's previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_PD_INT_AUX)
        {
            PH_REG_SET(PCR_PAD_INT_AUX_REG, gdwphhalPcr_RegisterBackUp.pcr_pad_int_aux_reg);
        }

        /* If IO_AUX pad was pulled down, restore it's previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_PD_IO_AUX)
        {
            PH_REG_SET(PCR_PAD_IO_AUX_REG, gdwphhalPcr_RegisterBackUp.pcr_pad_io_aux_reg);
        }

        /* If CLK_AUX pad was pulled down, restore it's previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_PD_CLK_AUX)
        {
            PH_REG_SET(PCR_PAD_CLK_AUX_REG, gdwphhalPcr_RegisterBackUp.pcr_pad_clk_aux_reg);
        }

        /* If SPIM related pads were pulled down, restore their previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_PD_SPIM)
        {
            PH_REG_SET(PCR_SPIM_REG, gdwphhalPcr_RegisterBackUp.pcr_spim_reg);
        }

        /* If IICM related pads were pulled down, restore their previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_PD_IICM)
        {
            PH_REG_SET(PCR_PADIICM_REG, gdwphhalPcr_RegisterBackUp.pcr_padiicm_reg);
        }

        /* If CT Clocks were disabled, restore their previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_DISABLE_CT_CLK)
        {
            PH_REG_SET(PCR_CLK_CFG_REG, gdwphhalPcr_RegisterBackUp.pcr_clk_cfg_reg);
        }

        /* If CLIF modules were disabled, restore their previous state */
        if(gdwphhalPcr_PwrDownSettingSelect & E_DISABLE_CLIF)
        {
            /* CLIF PLL enable/disable */
            PH_REG_SET(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,
                       gdwphhalPcr_RegisterBackUp.clkgen_clif_pll_global_control_reg);

            /* Disable the PLL since it will be enabled depending on the usecase*/
            PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,CLIF_PLL_ENABLE);


            /* CLIF ANA CM */
            PH_REG_SET(CLIF_ANA_CM_CONFIG_REG, gdwphhalPcr_RegisterBackUp.clif_ana_cm_config_reg);

            /* CLIF ANA PBF */
            PH_REG_SET(CLIF_ANA_PBF_CONTROL_REG, gdwphhalPcr_RegisterBackUp.clif_ana_pbf_control_reg);

            /* CLIF ANA RX */
            PH_REG_SET(CLIF_ANA_RX_REG, gdwphhalPcr_RegisterBackUp.clif_ana_rx_reg);


            PH_REG_CLEAR_BIT(CLIF_ANA_NFCLD_REG, CM_PD_NFC_DET);

        	PH_REG_SET_BIT(CLKGEN_CLOCK_PRESENCE_BYPASS_REG, CLOCK_PRESENCE_BYPASS_ENABLE);
        }

        /* If GSN value in Standby mode was changed, restore it */
        if(gdwphhalPcr_PwrDownSettingSelect & E_PCR_ANA_TX_STANDBY_GSN)
        {
            PH_REG_SET(PCR_ANA_TX_STANDBY_REG, gdwphhalPcr_RegisterBackUp.pcr_ana_tx_standby_reg);
        }
    }

    return PH_ERR_SUCCESS;
}

/*
 *Function Name     : phHalPcrEnterLowPower
 *Description       : Restores the registers stored before entering into the standby or suspend
 *
 *Input Parameters  : dwAnaTxInNormalMode ->
 *                    dwAnaTxProtInNormalMode ->
 *Output Parameters : None
 *
 */
static void phhalPcr_PreConfigLowPowerEntry(uint32_t dwAnaTxInStandbyMode, uint32_t dwAnaTxProtInStandbyMode, phhalPcr_WakeUpConfig_t* psWakeUpConfig)
{
    /* Set Standby ANA settings */
    PH_REG_SET(PCR_ANA_TX_STANDBY_REG,dwAnaTxInStandbyMode);
    PH_REG_SET(PCR_ANA_TXPROT_REG,dwAnaTxProtInStandbyMode);

    /* Clear the Standby Prevention and Boot up register values in the PCR_BOOT_REG register */
    PH_REG_SET_BIT_WO(PCR_CTRL_REG,CLR_BOOT_REGS);

    if (psWakeUpConfig->bEnableHIFWakeup && PH_REG_TEST_BIT(PCR_SYS_REG,PVDD_INT))
    {
        phhalPmu_PvddLdoLowPower();
    }

    phhalPmu_TxLdoStandby(0x01);


}

/*
 *Function Name     : phHalPcrExitLowPower
 *Description       : Restores the registers stored before entering into the standby or suspend
 *
 *Input Parameters  : dwAnaTxInNormalMode ->
 *                    dwAnaTxProtInNormalMode ->
 *Output Parameters : None
 *
 */
static void phhalPcr_PostConfigLowPowerExit(uint32_t dwAnaTxInNormalMode, uint32_t dwAnaTxProtInNormalMode, phhalPcr_WakeUpConfig_t* psWakeUpConfig)
{
    /* Restore ANA settings */
    PH_REG_SET(PCR_ANA_TX_STANDBY_REG,dwAnaTxInNormalMode);
    PH_REG_SET(PCR_ANA_TXPROT_REG,dwAnaTxProtInNormalMode);


    /* Configure temperature sensor 0 for higher hystresis*/
    PH_REG_SET_FIELD(PCR_TEMP_REG, TEMP_DELTA_0, gbphHalPcrHighTempTarget0);
    PH_REG_SET_FIELD(PCR_TEMP_REG, TEMP_DELTA_1, gbphHalPcrHighTempTarget1);

    /* Clear the wake up config registers */
    PH_REG_SET(PCR_WAKEUP_CFG_REG,0x00);
    PH_REG_SET(PCR_HOSTIF_WAKEUP_CFG_REG,0x00);

    /* Clear prevention reason */
    PH_REG_SET_BIT_WO(PCR_CTRL_REG,CLR_BOOT_REGS);

    /* If Internal PVDD is used, Start PVDD LDO */
    if(PH_REG_TEST_BIT(PCR_SYS_REG,PVDD_INT))
    {
        phhalPmu_PvddLdoStart();
    }

    phhalPmu_TxLdoStandby(0x00);


}
/**
 * @brief Configures the different wake up and collects in a wake up configuration variable
 * @param psRfLdtParams  : RfLdt parameters for the standby configuration
 * @param pdwWakeUpConfig  : Wake up configuration collector
 * @param psWakeUpConfig  : Different wake ups
 * @return
 */
static phStatus_t phhalPcr_ConfigWakeup(phhalPcr_RfLdtParams_t* psRfLdtParams,
                                        phhalPcr_WakeUpConfig_t* psWakeUpConfig)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    uint16_t wGpioWakeup = 0x00;
    uint32_t dwWakeUpConfig = 0x00;
    uint32_t dwHostWakeUpConfig = 0x00;
    uint32_t dwWakeupTimerCount = 0x00;
    uint8_t bHifSelection;

    /* Wake up enable for the host interface only if any of the Hif module is selected */
    bHifSelection = (uint8_t)PHHAL_PCR_GET_HIF();

    /* Clear the wake up configuration registers */
    PH_REG_RESET(PCR_WAKEUP_CFG_REG);
    PH_REG_RESET(PCR_HOSTIF_WAKEUP_CFG_REG);
    PH_REG_RESET(PCR_RFLD_REG);

    do
    {
        /* If the temperature as a wake up source then immediately go to standby without checking any
         * other standby configurations */
        if ((psWakeUpConfig->bWakeUpTemp) != 0x0)
        {
            PH_REG_SET_FIELD(PCR_TEMP_REG, TEMP_DELTA_0, gbphHalPcrLowTempTarget0);
            PH_REG_SET_FIELD(PCR_TEMP_REG, TEMP_DELTA_1, gbphHalPcrLowTempTarget1);
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_TEMP0_MASK;
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_TEMP1_MASK;
            break;
        }

        if (psWakeUpConfig->blWakeupVbusLow)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_VBUS_LOW_MASK;
        }
        if (psWakeUpConfig->bWakeUpTvddMon)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_TVDD_MON_MASK;
        }
        if (psWakeUpConfig->bWakeUpIntAux)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_INT_AUX_MASK;
        }
        if (psWakeUpConfig->bWakeUpCtPr)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_CT_PR_MASK;
        }
        if (psWakeUpConfig->bWakeUpPvddLim)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_PVDD_LIMITER_MASK;
        }
        if (psWakeUpConfig->bWakeUpRfLdt)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_RFLDT_MASK;

            if ( psRfLdtParams->bEnableAdvancedRFLD && (bHifSelection != PHHAL_PCR_HIF_USB))
            {
                dwWakeUpConfig |=  PCR_WAKEUP_CFG_REG_EN_ADV_RFLD_MASK;
                /* 0x631329 is HW Characterized value.
                 * This value shall not be changed, otherwise it leads to undefined behaviour */
                PH_REG_SET(PCR_ADV_RFLD_REG, PHHAL_PCR_ADV_RFLD_VALUE);
            }
            else
            {
                PH_REG_SET_BIT(PCR_ADV_RFLD_REG, ADV_RFLD_BYPASS);
            }

            PH_REG_SET(PCR_RFLD_REG, (0x2F << PCR_RFLD_REG_RFLD_VREF_ENABLE_POS) /* 0x2F is HW Characterized value. Not to be changed. */
                    | PH_FIELD(PCR_RFLD_REG,RFLD_REF_LO, psRfLdtParams->bRfldRefLo)
                    | PH_FIELD(PCR_RFLD_REG,RFLD_REF_HI, psRfLdtParams->bRfldRefHi) );

            /* Delay required for RFLDT comparator. */
            phUser_Wait(PHHAL_PCR_RFLDT_COMPARATOR_DELAY);

        }

        if (psWakeUpConfig->bWakeUpTimer)
        {
            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_WUC_MASK;

            /* Calculate the count */
            dwWakeupTimerCount = (uint32_t)((uint32_t)(((psWakeUpConfig->wWakeUpTimerVal)*(10)) / (uint32_t)(27)));
        }

        /* Gpio as a wake up source */
        if (psWakeUpConfig->bWakeUpGpio && PHHAL_PCR_CHECKPVDDOK())
        {
            /* Check any of the Gpio is configured as wake up signal */
            wGpioWakeup = (uint16_t)(PH_REG_GET_FIELD_NS(PCR_GPIO_WAKEUP_CFG_REG, GPIO_WAKEUP_ENABLE));
            PH_BREAK_IF_TRUE_WITH_ERROR((0 == wGpioWakeup),eStatus,PCR_NOWAKEUP_ENABLED);

            dwWakeUpConfig |= PCR_WAKEUP_CFG_REG_EN_GPIO_INT_MASK;

        }

        /* Host interface wake up enabled  Check the PVDD OK first */
        if (psWakeUpConfig->bEnableHIFWakeup && PHHAL_PCR_CHECKPVDDOK())
        {
            dwHostWakeUpConfig = PCR_HOSTIF_WAKEUP_CFG_REG_EN_INTERFACE_MASK;

            if (PHHAL_PCR_HIF_I2C == bHifSelection) /* I2C */
            {
                dwHostWakeUpConfig |= ((psWakeUpConfig->bI2CAddr << PCR_HOSTIF_WAKEUP_CFG_REG_I2C_ADDR_POS) & PCR_HOSTIF_WAKEUP_CFG_REG_I2C_ADDR_MASK);
            }

            PH_REG_SET(PCR_HOSTIF_WAKEUP_CFG_REG, dwHostWakeUpConfig);
        }

        PH_BREAK_IF_TRUE_WITH_ERROR((dwWakeUpConfig == 0x0 && dwHostWakeUpConfig == 0x0),eStatus,PCR_NOWAKEUP_ENABLED);

        /* Apply the wakeup config to the register */
        PH_REG_SET(PCR_WAKEUP_CFG_REG,dwWakeUpConfig);

        /* dwWakeupTimerCount is set to valid value if enabled else is zero */
        PH_REG_SET_FIELD(PCR_WAKEUP_CFG_REG, WUC_VALUE, dwWakeupTimerCount);

    } while (0);

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PCR);
}


/**
 * @brief Apply the standby
 * @param dwWakeUpConfig - Final wake up configuration value
 * @param psWakeUpConfig - Wake up configuration structure to know the status of wake up timer value
 * @param psPreventionReason - Standby prevention reason
 * @return
 */
static phStatus_t phhalPcr_ApplyLowPower(phhalPcr_WakeUpConfig_t* psWakeUpConfig,
                                          phhalPcr_StandbyPrevReason_t* psPreventionReason)
{

    uint32_t dwAnaTxInNormalMode = 0x00;     /* To restore properly if going in standby failed. */
    uint32_t dwAnaTxProtInNormalMode = 0x00; /* To restore properly if going in standby failed. */
    uint8_t bHifSelection;

    /* ----------------------- */
    /* Manage ANA TX standby   */
    /* ----------------------- */

    /* Save values */
    dwAnaTxInNormalMode =     PH_REG_GET(PCR_ANA_TX_STANDBY_REG);
    dwAnaTxProtInNormalMode = PH_REG_GET(PCR_ANA_TXPROT_REG);

    /*Configure required stuff before IC is taken to low power*/
    phhalPcr_PreConfigLowPowerEntry(gdwphHalPcrAnaTxStandByValue, gdwphHalPcrAnaTxProtStandByValue, psWakeUpConfig);

    bHifSelection = (uint8_t)PHHAL_PCR_GET_HIF();

    /* If Temp Error occurs with USB as HIF, then go to standby */
    if (bHifSelection == PHHAL_PCR_HIF_USB && psWakeUpConfig->bWakeUpTemp == 0)
    {
        (void)phhalPcr_ApplySuspend(psPreventionReason);
    }
    else
    {
        if(bHifSelection == PHHAL_PCR_HIF_USB  && psWakeUpConfig->bWakeUpTemp == 1)
        {
            /* Set the HIF selection to NONE */
            PH_REG_SET_FIELD(PCR_SYS_REG,HIF_SELECTION,0x00);
        }

       (void)phhalPcr_ApplyStandby(psPreventionReason);
    }

    /*Configure required stuff before IC is returned back to normal power*/
    phhalPcr_PostConfigLowPowerExit(dwAnaTxInNormalMode, dwAnaTxProtInNormalMode, psWakeUpConfig);

    return PH_ERR_SUCCESS;


}

static phStatus_t phhalPcr_ApplyStandby(phhalPcr_StandbyPrevReason_t* psStandbyPrevReason)
{
    uint32_t dwStbyPrevReason = 0x00;

    /* Exit Critical Section just before entering into the standby */
    __phUser_ExitCriticalSection();

    PH_REG_SET_BIT_WO(PCR_CTRL_REG,STANDBY);

    /* This portion of the code will be reached only when entering into the standby fails */
    dwStbyPrevReason =  PH_REG_GET(PCR_BOOT_REG);
    *psStandbyPrevReason = (phhalPcr_StandbyPrevReason_t)
                        ((dwStbyPrevReason & PCR_BOOT_REG_STBY_PREV_REASON_MASK) >> PCR_BOOT_REG_STBY_PREV_REASON_POS);


    return PH_ERR_SUCCESS;
}

static phStatus_t phhalPcr_ApplySuspend(phhalPcr_StandbyPrevReason_t* psSuspendPrevReason)
{
    uint32_t dwTimerCount = 0x00;
    phStatus_t status = PH_ERR_SUCCESS;

    do
    {
        /* Reduce the power consumption before entering into the suspend */
        (void)phhalPcr_ReducePowerConsumption(gdwphhalPcr_PwrDownSettingSelect);

        /* Exit Critical Section just before entering into the suspend */
        __phUser_ExitCriticalSection();

        PH_REG_SET(PCR_INT_SET_ENABLE_REG,PCR_INT_ENABLE_REG_SUSPEND_DEFAULTED_INT_ENABLE_MASK);
        PH_REG_SET_BIT_WO(PCR_INT_SET_ENABLE_REG,SUSPEND_INT_SET_ENABLE);

        PH_REG_SET_BIT_WO(PCR_CTRL_REG,SUSPEND);

        /* Wait here for 5 millisecond till the suspend default interrupt goes off */
        while (PH_REG_TEST_BIT(PCR_INT_STATUS_REG, SUSPEND_DEFAULTED_INT_STATUS) &&
              (dwTimerCount < PHHAL_PCR_USB_SUSPEND_TIMEOUT) )
        {
            dwTimerCount += PHHAL_PCR_USB_SUSPEND_POLLING_FREQUENCY;
            phUser_Wait(PHHAL_PCR_USB_SUSPEND_POLLING_FREQUENCY); /* 100 us delay */
        }

        *psSuspendPrevReason = (phhalPcr_StandbyPrevReason_t)PH_REG_GET_FIELD(PCR_BOOT_REG, STBY_PREV_REASON);

        if ((PH_REG_TEST_BIT(PCR_INT_STATUS_REG, SUSPEND_DEFAULTED_INT_STATUS)) || (*psSuspendPrevReason != 0))
        {
            /* Entering into the suspend was prevented */
            status = PH_ERR(USB_SUSPEND_PREVENTED, PCR);
            break;
        }

        PH_REG_SET_BIT_WO(PCR_CTRL_REG,SUSPEND);
        __WFI();

    }while(0);

    /* Restore the power consumption register changes after exiting from the suspend */
    (void)phhalPcr_RestorePowerConsumptionChanges();

    PH_REG_SET_BIT_WO(PCR_INT_CLR_ENABLE_REG,SUSPEND_INT_CLR_ENABLE);
    PH_REG_SET_BIT_WO(PCR_INT_CLR_ENABLE_REG,SUSPEND_DEFAULTED_INT_CLR_ENABLE);

    return status;
}


#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
/**
 * @brief Check the wake up source parameters
 * @param psWakeUpConfig : Different wake ups
 * @return
 */
static phStatus_t phhalPcr_CheckWakeUpParams(phhalPcr_WakeUpConfig_t* psWakeUpConfig)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    uint8_t bHifSelection = 0x00;
    do
    {
        /* Wake up enable for the host interface only if any of the Hif module is selected */
        bHifSelection = (uint8_t)(PH_REG_GET_FIELD_NS(PCR_SYS_REG, HIF_SELECTION));
        PH_BREAK_IF_TRUE_WITH_ERROR(((0 == bHifSelection) && (psWakeUpConfig->bEnableHIFWakeup)),eStatus,PCR_NO_HIF_SELECTED);

        if ((psWakeUpConfig->bEnableHIFWakeup) || (psWakeUpConfig->bWakeUpGpio))
        {
            /* Check if the PVDD is OK */
            if (!(PHHAL_PCR_CHECKPVDDOK()))
            {
                eStatus = PH_ERR_PCR_PVDD_ERROR;
                break;
            }
        }
        /* Wake up for timer but timer value is not correct */
        PH_BREAK_IF_TRUE_WITH_ERROR(((psWakeUpConfig->bWakeUpTimer) && ((psWakeUpConfig->wWakeUpTimerVal > PHHAL_PCR_MAX_WAKEUP_TIMER_VALUE)
            || (psWakeUpConfig->wWakeUpTimerVal < PHHAL_PCR_MIN_WAKEUP_TIMER_VALUE))),eStatus,PCR_WUC_PARAM_ERROR);

        /* If wake up source is Tvdd monitoring but the monitoring itself is not enabled!!! */
        PH_BREAK_IF_TRUE_WITH_ERROR(((psWakeUpConfig->bWakeUpTvddMon) &&
            (PH_REG_TEST_BIT(PCR_TXLDO_MON_REG,EN_5V_MONITOR) == 0)),eStatus,PCR_NO_TVDD_MON_ENABLED);

        /* If wake up is Vbus low and if monitoring itself is not enabled then return error */
        PH_BREAK_IF_TRUE_WITH_ERROR(((psWakeUpConfig->blWakeupVbusLow) &&
            (PH_REG_TEST_BIT(PCR_PMU_REG,VBATMON_OVERRIDE_EN) == 0)),eStatus,PCR_NO_VBUS_MON_ENABLED);

    } while (0);

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PCR);
}
#endif

#endif /* NXPBUILD__PHHAL_PCR */
