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
 * Gpio Hal implementation
 *
 * Project:  PN7462AU
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 18569 $ (v04.13.00)
 * $Date: 2016-07-27 17:31:35 +0530 (Wed, 27 Jul 2016) $
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_PCR
#include "phhalGpio.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU_ROM3.h"
#include "PN7462AU/PN7462AU_regcore.h"
#include "phhalNvic.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_GPIO_1                    0x01
#define PHHAL_GPIO_PCR_INT_GPIO_POS     15

/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

/**
 * @brief This Api  configures the internal pull-up and pull-down resistors.
 *
 * The internal pull up and pull down can be enabled for each Gpio.
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[in] blPullUpEn If this boolean variable is true then pull up will be enabled for that perticular Gpio.
 *                        Else it will be disabled.
 * @param[in] blPullDownEn If this boolean variable is true then pull down will be enabled for that perticular Gpio.
 *                        Else it will be disabled.
 *
 * @return Return value
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Configuring the pull-up and pull-down is successfull
 */

typedef phStatus_t (phRomhalPcr_ConfigPuPd_t)(uint32_t dwGpioNumber, uint32_t blPullUpEn,uint32_t blPullDownEn);
typedef phStatus_t (phRomhalPcr_ConfigInput_t) (uint32_t dwGpioNumber, uint32_t bEnable,uint32_t bWakeup,
                    uint32_t bEnableInterrupt,uint8_t bActiveLow, uint8_t bLevelSensitive, uint8_t bBothEdge);

#define phRomhalPcr_ConfigPuPd(dwGpioNumber, blPullUpEn, blPullDownEn)          \
    ((phRomhalPcr_ConfigPuPd_t *) PH_HAL_GPIO_CONFIGPUPD_ADDR)(dwGpioNumber, blPullUpEn, blPullDownEn)

#define phRomhalPcr_ConfigInput(dwGpioNumber, bEnable, bWakeup, bEnableInterrupt, bActiveLowOrFallingEdge,           \
    bLevelSensitive, bBothEdge)                                                                                      \
    ((phRomhalPcr_ConfigInput_t *)PH_HAL_GPIO_CONFIGINPUT_ADDR)(dwGpioNumber, bEnable, bWakeup, bEnableInterrupt,     \
        bActiveLowOrFallingEdge, bLevelSensitive, bBothEdge)


/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalPcr_ConfigInput(uint32_t dwGpioNumber, uint32_t bEnable,uint32_t bWakeup,uint32_t bEnableInterrupt,
                                   uint8_t bActiveLow, uint8_t bLevelSensitive, uint8_t bBothEdge)
{
    phStatus_t wStatus;

    wStatus = phRomhalPcr_ConfigInput(dwGpioNumber, bEnable, bWakeup, bEnableInterrupt, bActiveLow, bLevelSensitive,
        bBothEdge);

    PH_RETURN_ON_FAILURE(wStatus);

    /* Clear any previous spurious interrupts before enabling at NVIC level. */
    PH_REG_SET_BITN_WO(PCR_INT_CLR_STATUS_REG, PHHAL_GPIO_PCR_INT_GPIO_POS + dwGpioNumber);
    phHal_Nvic_ClearPendingInterrupt( 1U << (PHHAL_NVIC_GPIO_Common_Isr + dwGpioNumber));

    /* Enable/Disable NVIC for GPIO1 to 12 ISR's. */
    if(bEnable && bEnableInterrupt)
    {
        phHal_Nvic_EnableInterrupt(1U << (PHHAL_NVIC_GPIO_Common_Isr + dwGpioNumber));
    }
    else
    {
        phHal_Nvic_DisableInterrupt(1U << (PHHAL_NVIC_GPIO_Common_Isr + dwGpioNumber));
    }

    return wStatus;
}


phStatus_t phhalPcr_ConfigPuPd(uint32_t dwGpioNumber, uint32_t blPullUpEn, uint32_t blPullDownEn)
{

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    PH_RETURN_IF_TRUE_WITH_ERROR((dwGpioNumber > 12 || dwGpioNumber < 1), INVALID_PARAMETER, PCR);
#endif

    /* First GPIO PUPD is swapped in hardware. */
    if((dwGpioNumber == PHHAL_GPIO_1) && (blPullUpEn))
    {
        PH_REG_SET_FIELDS_NS_SC((PCR_PADGPIO1_REG),PCR_PADGPIO1_REG_GPIO1_PUPD_MASK,
                                (uint32_t)(0x01) << (PCR_PADGPIO1_REG_GPIO1_PUPD_POS));
    }
    else
    {   /* For all other GPIO's */
        phRomhalPcr_ConfigPuPd(dwGpioNumber,blPullUpEn,blPullDownEn);
    }

    return PH_ERR_SUCCESS;
}



phStatus_t phhalPcr_GpioInit(const uint8_t * pbInputISR,const uint8_t * pbOutputPUPD)
{
    uint32_t i, dwGpioNum;
    uint8_t bGpioOutputPUPDCfg;
    uint8_t bGpioInputCfg;

    for(i=0; i < PH_HAL_GPIO_NUMBER_OF_GPIOS; i++)
    {
        bGpioOutputPUPDCfg = pbOutputPUPD[i];
        bGpioInputCfg = pbInputISR[i];
        dwGpioNum = i + 1;
        if (PH_HAL_GPIO_OP_SET == (PH_HAL_GPIO_OP_SET & bGpioOutputPUPDCfg))
        {
            phhalPcr_ConfigOutput(dwGpioNum, true, bGpioOutputPUPDCfg & PH_HAL_GPIO_OP_SET_SLEWRATE);
            PH_HAL_GPIO_SETGPIOVAL(dwGpioNum, bGpioOutputPUPDCfg & PH_HAL_GPIO_OP_SET_VALUE_HIGH);
            break;
        }
        if (0 != (PH_HAL_GPIO_PUPD_EN_MASK & bGpioOutputPUPDCfg)) {
            phhalPcr_ConfigPuPd(dwGpioNum,
                PH_HAL_GPIO_PULL_UP_EN & bGpioOutputPUPDCfg,
                PH_HAL_GPIO_PULL_DOWN_EN & bGpioOutputPUPDCfg );
        }
        if ( 0 != bGpioInputCfg) {
            phhalPcr_ConfigInput(
                dwGpioNum,
                0 != (bGpioInputCfg & PH_HAL_GPIO_IN_SET),
                0 != (bGpioInputCfg & PH_HAL_GPIO_IN_WAKEUP),
                0 != (bGpioInputCfg & PH_HAL_GPIO_IN_INTRUPT),
                0 != (bGpioInputCfg & PH_HAL_GPIO_IN_ACT_LOW_FALL_EDG),
                0 != (bGpioInputCfg & PH_HAL_GPIO_IN_LVL),
                0 != (bGpioInputCfg & PH_HAL_GPIO_IN_BOTH_EDG));
        }
    }
    return PH_ERR_SUCCESS;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_PCR */
