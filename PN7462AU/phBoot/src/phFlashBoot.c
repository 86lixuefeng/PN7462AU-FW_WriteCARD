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
 * Implementation of boot up routine for Flash
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-05 00:55:37 +0530 (Tue, 05 Jul 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18545 $ (v04.13.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phFlashBoot.h"
#include "phhalPcr.h"
#include "phhalPmu.h"
#include "phUser.h"
#include "ph_Platform.h"
#include "phhalNvic.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phCfg_EE.h"
#include "phhalTimer.h"
#include "phhalGpio.h"
#include "phhalRng.h"
#include "phhalEeprom.h"
#ifdef NXPBUILD__PHHAL_FLASH
#    include "phhalFlash.h"
#endif
#include "phhalClkGen.h"
#include "phhalSysSer.h"
#include "phRtos.h"
#include "ph_MemoryMap.h"
#ifdef NXPBUILD__PHHAL_CT
#  include "phhalCt.h"
#endif
#ifdef NXPBUILD__PHHAL_RF
#  include "phhalRf.h"
#endif



/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* Supported ROM Version(s)
 *
 * Extend this list for supported backward compatible ROM Versions */

/* A2 Candidate ROM Version */
#define PH_FLASHBOOT_SUPPORTED_ROM_VERISON_A2 (  0x10  /* ROM */ \
        | ((0x02) << 8*3 ) /* Major */                           \
        | ((0x04) << 8*2 ) /* Minor */                           \
        | ((0x00) << 8*1 ) /* Dev   */                           \
    )


#define PH_FLASHBOOT_SUPPORTED_ROM_VERISON (  0x10  /* ROM */ \
        | ((0x02) << 8*3 ) /* Major */                        \
        | ((0x05) << 8*2 ) /* Minor */                        \
        | ((0x00) << 8*1 ) /* Dev   */                        \
    )

/* V Gate Candidate ROM Version */
#define PH_FLASHBOOT_SUPPORTED_ROM_VERISON_V (  0x10  /* ROM */ \
        | ((0x02) << 8*3 ) /* Major */                           \
        | ((0x06) << 8*2 ) /* Minor */                           \
        | ((0x00) << 8*1 ) /* Dev   */                           \
    )

#define PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_MAJOR       (PH_CFG_EE_VER_MAJOR)
#define PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_MINOR       (PH_CFG_EE_VER_MINOR)
#define PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_DEV         (PH_CFG_EE_VER_DEVELOPMENT)

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/**
 *  Assert that the FLASH is booting against correct ROM */
static void phFlashBoot_AssertROMVersion(void);

 /**
  * Initializes all the Hw modules.
  * Checks the 27.12 MHz clock availability.
  * If NO CLOCK/HARDWARE INIT failed then DeInitializes all the Hw modules and stops the boot. */
 static void phFlashBoot_HwInit(void);

/**
 *  Initializes all the Hw modules. */
static phStatus_t phFlashBoot_HwSetup(void);

/**
 * Sets the exceptions and interrupts at different priority levels.
 */
static void phFlashBoot_SetIntrptsPrio(void);


/**
 * Performs PreCheck before system startUp.
 * Checks PVDD availability, ROM and EEPROM versions.
 * Mandatory to call before going to any application start.
 */
static phStatus_t phFlashBoot_PreCheck(void);

/**
 * DeINitializes all the Hw modules.
 */
static void phFlashBoot_HwTearDown(void);

/**
 * Handle Boot because of VBUS going low in suspend or standby. */
static void phFlashBoot_VbusLowHandler(void);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5)
#pragma diag_suppress 951
#endif

void phFlashBoot_Main(void) {

    /* Initialize the interrupts at different priority levels */
    phFlashBoot_SetIntrptsPrio();

#ifdef __CODE_RED
    /* With CODE RED/LPCXpresso,
     * ResetISR of LPCXpresso would have already done ZI BSS */
#elif PH_CONFIG_TARGET_PLATFORM_IAR == PHFL_CONFIG_TARGET_PLATFORM
    /* Not needed */
#elif PH_CONFIG_TARGET_PLATFORM_DS5 == PHFL_CONFIG_TARGET_PLATFORM
    /* Not needed */
#else
    phFlashBoot_ZIBSS();
#endif /* __CODE_RED */


    gCriticalSectionSemaphore = 0;

    /* Perform exceptions check. */
    if(phFlashBoot_PreCheck() != PH_ERR_SUCCESS) {
        /* Something gone wrong, its an error. STOP BOOT. */
        while(1) {
            __WFI();
        }
    }

    /* Initialize RTOS before HwInit as few Hals are tied with RTOS. */
    phRtos_Initialize();

    /* Initialize hardwares */
    phFlashBoot_HwInit();
}


/*
 * Get the Boot reasons.
 * Returns the 1st set boot reason position from 1 to 22.
 */
phhalPcr_BootReason_t phFlashBoot_GetBtRns(void)
{
    uint32_t dwBootReason;
    uint8_t bBootPos;

    /* Get the boot reason. */
    phhalPcr_GetBootReason(&dwBootReason);

    /* Handle 2.7 volt auto HPD. */
    if(!dwBootReason){
        return E_STARTUP_POR;
    }

    /* calculate the boot reason Bit position. */
    for(bBootPos = 0; !(dwBootReason & ( ((uint32_t)1) << bBootPos)); bBootPos++);

    return (phhalPcr_BootReason_t)++bBootPos;
}

PH_PLACE_FUNCTION_TO_SECTION(".after_vectors")
void phFlashBoot_HardFault_HandlerC(
    unsigned long *args)
{
    volatile unsigned long cm0_r0;
    volatile unsigned long cm0_r1;
    volatile unsigned long cm0_r2;
    volatile unsigned long cm0_r3;
    volatile unsigned long cm0_r12;
    volatile unsigned long cm0_lr;
    volatile unsigned long cm0_pc;
    volatile unsigned long cm0_psr;

    if (((uint32_t) args < PH_RAM_MEM_END)&& ( (uint32_t)args > PH_RAM_MEM_START)){
        cm0_r0 = ((unsigned long)args[0]);
        cm0_r1 = ((unsigned long)args[1]);
        cm0_r2 = ((unsigned long)args[2]);
        cm0_r3 = ((unsigned long)args[3]);
        cm0_r12 = ((unsigned long)args[4]);
        cm0_lr = ((unsigned long)args[5]);
        cm0_pc = ((unsigned long)args[6]);
        cm0_psr = ((unsigned long)args[7]);

        UNUSED(cm0_r0);
        UNUSED(cm0_r1);
        UNUSED(cm0_r2);
        UNUSED(cm0_r3);
        UNUSED(cm0_r12);
        UNUSED(cm0_lr);
        UNUSED(cm0_pc);
        UNUSED(cm0_psr);
    }

    /* Break into the debugger */
    __asm("BKPT #0\n");

    while (1)
    {
        __WFI();
    }
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static void phFlashBoot_AssertROMVersion(void) {
    uint32_t ROMVersion = 0;
    phhalSysSer_GetROM_Version(
        PH_SYSRV_GET_DATA, &ROMVersion);
    /* This Flash Image, depends on specific ROM implementation
     * Whithout it, the behaviour is undefined */
    /* Extend this list for backward compatible ROM Versions */
    if (PH_FLASHBOOT_SUPPORTED_ROM_VERISON == ROMVersion)
    {
        /* OK */
    }
    else if (PH_FLASHBOOT_SUPPORTED_ROM_VERISON_A2 == ROMVersion)
    {
        /* OK */
    }
    else if (PH_FLASHBOOT_SUPPORTED_ROM_VERISON_V == ROMVersion)
    {
        /* OK */
    }
    else
    {
        /* If the execution has stopped here,
         * ROM/PATCHES have to be upgraded. */
        PH_USER_ASSERT(0);
    }
}

static phStatus_t phFlashBoot_HwSetup(void)
{
    phStatus_t wStatus;

    /* Initialize the HAL's */

    do{
        /* Initialize delay loop variable based on CPU speed of 20Mhz*/
        /* Internally, this will initialize timer HAL */
        phUser_WaitInit(E_COMMON_CPUSPEED_20MHZ);

        /* PMU TXLDO initialization */
        wStatus = phhalPmu_TxLdoInit(gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo,
            (phhalPmu_TvddSel_t)gpkphCfg_EE_Boot_TxLdoParams->eFullPowerTvddSel,
            (phhalPmu_LowPower_TvddSrc_t)gpkphCfg_EE_Boot_TxLdoParams->eLowPowerTvddSrc,
            gpkphCfg_EE_Boot_TxLdoParams->wWaitTime,
            gpkphCfg_EE_Boot_TxLdoParams->bOverCurrentEnable);
        PH_BREAK_ON_FAILURE(wStatus);

        /* PCR initialization */
        wStatus = phhalPcr_Init( (phhalPcr_PwrTempConfig_t *)gpkphCfg_EE_Boot_PcrPwrTempConfig,
            (phhalPcr_TxAnaStandByConfig_t *)gpkphCfg_EE_Boot_TxAnaStandByConfig,
            (uint32_t)(gpkphCfg_EE_Boot_PcrPwrDown->dwPwrDownSettings));
        PH_BREAK_ON_FAILURE(wStatus);

        /*Clock Gen Init */
        wStatus =  phhalClkGen_Init((phhalClkGen_Source_t)gpkphCfg_EE_Boot_ClkGen->eSource,
            (uint32_t)(gpkphCfg_EE_Boot_ClkGen->wXtalActivationTimeOut),
            gpkphCfg_EE_Boot_ClkGen->bKickOnError);
        PH_BREAK_ON_FAILURE(wStatus);

#ifdef NXPBUILD__PHHAL_RNG
        /* RNG HAL Initialization */
        wStatus =  phhalRng_Init(gpkphCfg_EE_Boot_RNG->bTrngFeedTimeout);
        PH_BREAK_ON_FAILURE(wStatus);
#endif /* NXPBUILD__PHHAL_RNG */

        /* Initialize the Gpio's based on EEPROM configs. */
        phhalPcr_GpioInit(gpkphCfg_EE_Boot_GPIO->InputISR, gpkphCfg_EE_Boot_GPIO->OutputPUPD);

#ifdef NXPBUILD__PHHAL_EEPROM
        /* EEPROM HAL Initialization */
        wStatus =  phhalEeprom_Init((uint8_t)(gpkphCfg_EE_Boot_EEPROM->bEnableFastMode));
        PH_BREAK_ON_FAILURE(wStatus);
#endif /* NXPBUILD__PHHAL_EEPROM */

        wStatus = phhalClkGen_Clk27m12Detect();
        PH_BREAK_ON_FAILURE(wStatus);

#ifdef NXPBUILD__PHHAL_FLASH
        /* FLASH HAL Initialization */
        wStatus =  phhalFlash_Init( (uint32_t)(gpkphCfg_EE_Boot_FLASH->bEnableFastMode),
            (uint32_t)(gpkphCfg_EE_Boot_FLASH->bEnableSkipProgramOnEraseFail));
        PH_BREAK_ON_FAILURE(wStatus);
#endif /* NXPBUILD__PHHAL_FLASH */

#ifdef NXPBUILD__PHHAL_CT
        wStatus = phhalCt_Init((phhalCt_InitParam_t *)gpkphCfg_EE_Boot_CT);
        PH_BREAK_ON_FAILURE(wStatus);
#endif

#ifdef NXPBUILD__PHHAL_RF
        wStatus = phhalRf_Init();
#endif

        /* Wait for Tx Ldo to start. */
        phUser_Wait((gpkphCfg_EE_Boot_TxLdoParams->wWaitTime & PHHAL_PMU_TXLDO_WAITTIME_VALUE_MASK)
        		- PH_FLASHBOOT_TXLDO_START_TIME_US);

    }while(0);

    return wStatus;
}

static void phFlashBoot_SetIntrptsPrio(void)
{
    /* interrupts at lowest levels */
    phHal_Nvic_SetPendSVPrio(PH_HAL_INTRPT_PRIO_LOW);
    phHal_Nvic_SetSysTick(PH_HAL_INTRPT_PRIO_LOW);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_BMA, PH_HAL_INTRPT_PRIO_LOW);

    /* interrupts at medium prio */
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_TIMER, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_EECTRL, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_SPI, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_I2C, PH_HAL_INTRPT_PRIO_MED);

    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_Common_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_01_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_02_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_03_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_04_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_05_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_06_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_07_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_08_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_09_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_10_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_11_Isr, PH_HAL_INTRPT_PRIO_MED);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_GPIO_12_Isr, PH_HAL_INTRPT_PRIO_MED);

    /* interrupts at high prio */
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_CLIF, PH_HAL_INTRPT_PRIO_HIGH);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_HIF, PH_HAL_INTRPT_PRIO_HIGH);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_CTIF, PH_HAL_INTRPT_PRIO_HIGH);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_PMU, PH_HAL_INTRPT_PRIO_HIGH);
    phHal_Nvic_SetInterruptPrio(PHHAL_NVIC_PCR, PH_HAL_INTRPT_PRIO_HIGH);

    /* interrupts at real time prio */
    phHal_Nvic_SetSvCPrio(PH_HAL_INTRPT_PRIO_REALTIME);
}

static phStatus_t phFlashBoot_PreCheck(void)
{
    /* Enter critical section. */
    __phUser_EnterCriticalSection();

    /* clear all the the spurious pending interrupts. */
    phHal_Nvic_ClearAllPendingInterrupt();

    /* Disable all NVIC interrupts */
    PH_HAL_NVIC_DISABLE_ALL_INTERRUPTS();

    /* Clear the CT Enable. */
    PH_REG_CLEAR_BITN(PCR_SYS_REG, PCR_SYS_REG_ENABLE_CT_POS);

    /* Configure Auto HPD if IC wokeup from standby due to VBUS low */
    if(phFlashBoot_GetBtRns() == E_VBUS_LOW)
    {
        phFlashBoot_VbusLowHandler();

        /* It is not expected to reach here. */
        return PH_ERR_INTERNAL_ERROR;
    }

    /* The Main example application is not  expected to work without PVDD */
    if(!PHHAL_PCR_CHECKPVDDOK())
    {
#ifdef NXPBUILD__PHHAL_EEPROM
        /* Updated the cause code into EEPROM. */
        (void) phhalEeprom_Init((uint8_t)(gpkphCfg_EE_Boot_EEPROM->bEnableFastMode));
        phhalEeprom_WriteBuffer((uint8_t *)PCR_GPREG0_REG, (uint8_t *)&gpkphCfg_EE_ErrorMem_Err->dwCauseCode, 4);
        (void) phhalEeprom_DeInit();
#endif
        /* If it is internal then start the Pvdd Ldo internally */
        if(PH_REG_TEST_BIT(PCR_SYS_REG,PVDD_INT))
        {
            /* Try to start the Pvdd again  and enable the IRQ */
            phhalPmu_PvddLdoStart();
        }
        PH_REG_SET(NVIC_ISER_REG, (uint32_t)PH_HAL_NVIC_PMU_MASK);
        __phUser_ExitCriticalSection();
        while(1)
        {
            /* Check if 1.8V PVDD is OK */
            if(PHHAL_PCR_CHECKPVDDOK())
            {
                break;
            }

            /* Should not ideally come here */
            __WFI();
        }
    }

    PH_REG_SET(NVIC_ISER_REG, (uint32_t)PH_HAL_NVIC_PMU_MASK);
    __phUser_ExitCriticalSection();

    /* Ensure the Major version of EEPROM is correct
     *
     * Warning: Firmware might be tightly linked with the EEPROM contents and layout.
     * If there is a mismatch please do upgrade.
     * */
    PH_USER_ASSERT(gpkphCfg_EE_Ctrl_VerInfo->major == PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_MAJOR);
#ifdef PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_MINOR
    PH_USER_ASSERT(gpkphCfg_EE_Ctrl_VerInfo->minor ==  PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_MINOR);
#endif

#if defined(PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_DEV) && PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_DEV >= 100
    PH_USER_ASSERT(gpkphCfg_EE_Ctrl_VerInfo->development ==  PH_FLASHBOOT_SUPPORTED_EEPROM_VERISON_DEV);
#endif

#ifdef NXPBUILD__CFG_ENABLE_EE_DOWNLOAD
    /* Enforce forward looking to phCfg_EE_Build_Values so that phCfg_EE_Build_Values does not
     * plainly go out of the build post linker optimization.
     * Also, expect that thiese values remain as expected.
     */
    PH_USER_ASSERT(phCfg_EE_Build_Values[PH_CFG_EE_VERINFO_START_ADDRESS - PH_CFG_EE_ERR_START_ADDRESS] \
      ==  (*(uint8_t * )PH_CFG_EE_VERINFO_START_ADDRESS));
#endif /* NXPBUILD__CFG_ENABLE_EE_DOWNLOAD */

    /* Check for ROM versions. */
    phFlashBoot_AssertROMVersion();

    return PH_ERR_SUCCESS;
}


static void phFlashBoot_HwInit(void)
{
    phStatus_t wStatus;

    wStatus = phFlashBoot_HwSetup();

    if(wStatus != PH_ERR_SUCCESS)
    {
        phFlashBoot_HwTearDown();
    }
}


static void phFlashBoot_HwTearDown(void)
{

    /* Return status of deInit's are discarded as we have nothing to do.
     * Anyway we are here to stop the boot and hang. */

    /* PMU TXLDO. */
    (void)phhalPmu_TxLdoStart(E_LOW_POWER);

#ifdef NXPBUILD__PHHAL_RNG
    /* RNG HAL DeInitialization */
    (void)phhalRng_DeInit();
#endif /* NXPBUILD__PHHAL_RNG */

    /* Timer HAL DeInitialization */
    (void)phhalTimer_DeInit();

    /* Note: State of GPIOs is retained as it is. */

#ifdef NXPBUILD__PHHAL_EEPROM
    /* EEPROM HAL DeInitialization */
    (void)phhalEeprom_DeInit();
#endif /* NXPBUILD__PHHAL_EEPROM */

#ifdef NXPBUILD__PHHAL_CT
    (void)phhalCt_DeInit();
#endif

#ifdef NXPBUILD__PHHAL_RF
    (void)phhalRf_DeInit();
#endif

    /* Clear all the GPIOs. Set RED only as to indicate Boot stop. */
    PH_REG_SET(PCR_PADOUT_REG, PCR_PADOUT_REG__RESET_VALUE);

    /* Enable GPIO12 (Red LED) as Output and set high. */
    PH_REG_SET(PCR_PADGPIO12_REG, PCR_PADGPIO12_REG_GPIO12_EN_OUT_MASK | PCR_PADGPIO12_REG_GPIO12_SLEW_RATE_MASK);
    PH_REG_SET(PCR_PADOUT_REG, PCR_PADOUT_REG_PADOUT_GPIO12_MASK);

    /*Clock Gen DeInit */
    (void)phhalClkGen_DeInit();

    /* Stop Boot. */
    while(1)
    {
        __WFI();
    }
}

static void phFlashBoot_VbusLowHandler(void)
{
    /* clear the boot reason. */
    PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON();

    /* Set pcr_sys_reg.automatic_hpd.
     * With this bit set, PCR will take PN7462AU into low power state. */
    PH_REG_SET_BIT(PCR_SYS_REG,AUTOMATIC_HPD);

    return;
}
