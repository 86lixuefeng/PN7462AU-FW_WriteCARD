/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors 2014
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** @file
 *
 * Gpio Hal implementation
 *
 * Project:  PN7462AU
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 *
 */

#ifndef PHHALHWGPIO_H_
#define PHHALHWGPIO_H_
/**
 * \addtogroup phhalGPIO
 *
 * The General Purpose IO (HAL) APIs
 *
 * @{
 *
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_PCR
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU_ROM3.h"
#include "ph_Status.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * @name GPIO Input/Output Configuration
 *
 * @ref phhalPcr_GpioInit would use these bits as part of parameters
 *
 * @{
 * GPIO bit map configurations.
 *
 * <pre>
 * |Bit 7| | Bit 6 | | Bit 5 |   |Bit 4    |  |Bit 3 | |Bit 2  |  |Bit 1| |Bit 0|
 * | Cfg | | Intrpt| | WakeUp|   | SlewRate|  | PullD| | PullUp|  | Op  | | Ip  |
 * </pre>
 *
 * @return
 */

/*
    <option value="Bit0=0" description="Do not configure as Output" />
    <option value="Bit0=1" description="Configure Gpio as output" />
    <option value="Bit1=1" description="Enable slew-rate" />
    <option value="Bit2=1" description="Drive the output high" />
    <option value="Bit2=0" description="Drive the output low" />

    <option value="Bit5=1" description="Apply Pull UP" />
    <option value="Bit6=1" description="Apply Pull Down" />
 */

#define PH_HAL_GPIO_OP_SET            (1 << 0)
#define PH_HAL_GPIO_OP_SET_SLEWRATE   (1 << 1)
#define PH_HAL_GPIO_OP_SET_VALUE_HIGH (1 << 2)
#define PH_HAL_GPIO_PULL_UP_EN        (1 << 5)
#define PH_HAL_GPIO_PULL_DOWN_EN      (1 << 6)
#define PH_HAL_GPIO_PUPD_EN_MASK      \
    (PH_HAL_GPIO_PULL_UP_EN | PH_HAL_GPIO_PULL_DOWN_EN)

/*
    <!--          -BLV-IWS -->
    <option value="ALL=0" description="Skip Configuration on Boot" />
    <option value="Bit0=0" description="Un-Configure as input" />
    <option value="Bit0=1" description="Configure/SET as Input" />
    <option value="Bit1=1" description="GPIO is a wakeup source" />
    <option value="Bit2=1" description="GPIO is an interrupt source" />
    <option value="Bit4=1" description="Level Sensitive Interrupt" />
    <option value="Bit5=1" description="Interrupt on Active Low or Falling Edge" />
    <option value="Bit6=1" description="Interrupt on Both Edges" />
 */
#define PH_HAL_GPIO_IN_SET              (1 << 0)
#define PH_HAL_GPIO_IN_WAKEUP           (1 << 1)
#define PH_HAL_GPIO_IN_INTRUPT          (1 << 2)
#define PH_HAL_GPIO_IN_LVL              (1 << 4)
#define PH_HAL_GPIO_IN_ACT_LOW_FALL_EDG (1 << 5)
#define PH_HAL_GPIO_IN_BOTH_EDG         (1 << 6)


/** @} */

/**
 * Number of GPIO's
 */
#define PH_HAL_GPIO_NUMBER_OF_GPIOS               12


/**
 * Inline function macro for setting the value of a perticular Gpio pin
 *
 * @param bGpioNumber Gpio number which has to be set or reset ( 1 to 12)
 * @param fValue Value to be set or reset. If true bit will be set else bit will be reset
 *
 */
#define PH_HAL_GPIO_SETGPIOVAL(bGpioNumber,fValue)\
{                                                                                                \
    uint32_t dwRegisterOffset = (uint32_t)((bGpioNumber)-1);                                     \
    if(fValue)                                                                                   \
    {                                                                                            \
        PH_REG_SET_BITN(PCR_PADOUT_REG,(PCR_PADOUT_REG_PADOUT_GPIO1_POS+dwRegisterOffset));      \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        PH_REG_CLEAR_BITN(PCR_PADOUT_REG,(PCR_PADOUT_REG_PADOUT_GPIO1_POS+dwRegisterOffset));    \
    }                                                                                            \
}

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 *  @brief This enumeration provides the status of the gpio of a perticular gpio pi
 */
typedef enum phhalPcr_GpioStatus {
    /** \b 0x00 */ E_PCR_GPIO_HAL_BOTH_DISABLED,  /**< Input and output is disabled  */
    /** \b 0x01 */ E_PCR_GPIO_HAL_INPUT_ENABLED,  /**< Input is enabled */
    /** \b 0x02 */ E_PCR_GPIO_HAL_OUTPUT_ENABLED, /**< Output is enabled */
    /** \b 0x03 */ E_PCR_GPIO_HAL_BOTH_ENABLED    /**< Input and output is enabled */
} phhalPcr_GpioStatus_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * **************************************************************************************************************** */


/**
 * @brief This Api provides the status of the Gpio pad of the provided Gpio number
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[out] eGpioStatus Pointer to the enumeration #phhalPcr_GetGpioStatus_t in which the status of the provided
 *                        Gpio number will be returned
 * @return Return value
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Getting the Gpio status is successful
 */

typedef phStatus_t (phhalPcr_GetGpioStatus_t)(uint32_t dwGpioNumber,  phhalPcr_GpioStatus_t * eGpioStatus);

/** @brief This Api configures the Gpio as output.
 *
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[in] blEnable If this boolean variable is true then the provided Gpio pin(Gpio number) will be configured
 *                        as output.
 * @param[in] blSlewRate If this boolean variable is true then the provided Gpio pin(Gpio number)slew rate
 *                        (Driver strength) will be enabled.
 * @return Return value
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Configuring the Gpio as output enable or disable successful
 */

typedef phStatus_t (phhalPcr_ConfigOutput_t)(uint32_t dwGpioNumber, uint32_t dwEnableAsOuput,uint32_t dwSlewRate);

/**
 * @brief This Api provides the pad value of the provided Gpio number
 * @param[in] bGpioNumber Gpio number which ranges form 1 to 12.
 * @param[out] pbValue Pointer in which the value of the Gpio pad will be returned for the provided Gpio number
 * @return Return value
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Getting the Gpio value is successful
 */

typedef phStatus_t (phhalPcr_GetGpioVal_t)(uint32_t dwGpioNumber, uint8_t* pbValue);


/**
 * Initialzes the GPIOs based on EEPROM configuration.
 */
phStatus_t phhalPcr_GpioInit(const uint8_t * pbInputISR,const uint8_t * pbOutputPUPD);

/**
 * @brief This Api configures the GPIO as input.
 *
 * Along with enabling/disabling the Gpio as input it can also be configured as a wake up source and can be enabled
 * as interrupt.
 *
 * @note By default the Gpio number from 01 to 08 is configured as input and also the wake up is enabled.
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[in] bEnable If this boolean variable is true then the provided Gpio pin(Gpio number) will be configured
 *                        as input.
 * @param[in] bWakeup If this boolean variable is true then the provided Gpio pin(Gpio number) will be configured
 *                        as wake up source.
 * @param[in] bEnableInterrupt If this boolean variable is true then the provided Gpio pin(Gpio number) will be
 *            configured as interrupt source.
 * @param[in] bActiveLow If this variable is true then the interrupt will be generated on Active_low/falling edge else
 *                       Active_high/Rising edge.
 * @param[in] bLevelSensitive If this variable is true then the interrupt will be generated on Level sensitive else
 *                       Edge sensitive.
 * @param[in] bBothEdge  If this variable is true then the interrupt will be generated on both positive edge and
 *                      negative else Not both edge triggered.
 * @return Return value
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Configuring the Gpio as input enable or disable successful
 */
phStatus_t phhalPcr_ConfigInput(uint32_t dwGpioNumber, uint32_t bEnable,uint32_t bWakeup,uint32_t bEnableInterrupt,
                                   uint8_t bActiveLow, uint8_t bLevelSensitive, uint8_t bBothEdge);

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
 * @return Status of operation
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Configuring the pull-up and pull-down is successfull
 */

phStatus_t phhalPcr_ConfigPuPd(uint32_t dwGpioNumber, uint32_t blPullUpEn,uint32_t blPullDownEn);

/** @brief This Api configures the Gpio as output.
 *
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[in] dwEnableAsOuput If this boolean variable is true then the provided Gpio pin(Gpio number) will be
 *  configured as output.
 * @param[in] dwSlewRate If this boolean variable is true then the provided Gpio pin(Gpio number)slew rate
 *                        (Driver strength) will be enabled.
 * @return Status of operation
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Configuring the Gpio as output enable or disable successful
 */
#define phhalPcr_ConfigOutput(dwGpioNumber, dwEnableAsOuput, dwSlewRate)       \
    ((phhalPcr_ConfigOutput_t *) \
    		PH_HAL_GPIO_CONFIGOUTPUT_ADDR)\
			(dwGpioNumber, dwEnableAsOuput, dwSlewRate)

/**
 * @brief This Api provides the pad value of the provided Gpio number
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[out] pbValue Pointer in which the value of the Gpio pad will be returned for the provided Gpio number
 * @return Status of operation
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Getting the Gpio value is successful
 */
#define phhalPcr_GetGpioVal(dwGpioNumber, pbValue)              \
    ((phhalPcr_GetGpioVal_t *) \
    		PH_HAL_GPIO_GETGPIOVAL_ADDR)\
			(dwGpioNumber, pbValue)

/**
 * @brief This Api provides the status of the Gpio pad of the provided Gpio number
 * @param[in] dwGpioNumber Gpio number which ranges form 1 to 12.
 * @param[out] eGpioStatus Pointer to the enumeration #phhalPcr_GpioStatus_t in which the status of the provided
 *                        Gpio number will be returned
 * @return Status of operation
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @retval    #PH_ERR_SUCCESS Getting the Gpio status is successful
 */
#define phhalPcr_GetGpioStatus(dwGpioNumber, eGpioStatus)           \
    ((phhalPcr_GetGpioStatus_t *) \
    		PH_HAL_GPIO_GETGPIOSTATUS_ADDR)\
			(dwGpioNumber, eGpioStatus)

#endif /* NXPBUILD__PHHAL_PCR */
/** @} */
#endif /* PHHALHWGPIO_H_ */
