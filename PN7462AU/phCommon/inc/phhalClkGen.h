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
 * Clock generation HAL implementation for the CLKGEN module
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-11 13:30:09 +0530 (Mon, 11 Jul 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18565 $ (v04.13.00)
 */

#ifndef PHHALCLKGEN_H
#define PHHALCLKGEN_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CLKGEN

#include "PN7462AU/PN7462AU_clkgen.h"
/** \addtogroup phhalClkGen
 *
 * The Clock Generation APIs
 *
 * This module implements clock generation Api's for Clif and USB modules.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *                  checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *                  and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *                  failures.
 *  @{
 */

#include "ph_Status.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/**
 * Default delay in Micro Seconds to wait for PLL Stability
 */
#define PHHAL_CLKGEN_CLIF_PLL_DELAY_USECS    10

#define PHHAL_CLKGEN_CHANGE_USB_PLL_REF() PH_REG_SET_FIELD(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_REF_CLK_SELECT, 3)
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Enumeration for the clock source
 */
typedef enum{
    /** \b 0x00 */ E_CLKSRC_XTAL,     /**< Clock source is from XTAL */
    /** \b 0x01 */ E_CLKSRC_EXTERNAL, /**< Clock source is from external */
    /** \b 0x02 */ E_CLKSRC_MAX       /**< Clock source is invalid */
}phhalClkGen_Source_t;

/**
 * Enumeration for the CLIF PLL types
 */
typedef enum{
    /** \b 0x00 */ E_CLIFPLL_NORMAL, /**< CLIF PLL is normal mode */
    /** \b 0x01 */ E_CLIFPLL_DPLL,   /**< CLIF PLL is in DPLL mode */
    /** \b 0x02 */ E_CLIFPLL_MAX     /**< CLIF PLL is invalid */
}phhalClkGen_ClifMode_t;

/** \addtogroup grpErr_HalClkGen
 *
 * Error codes for CLKGen HAL
 *
 * @{
 */
 /** Enumeration for the clock generation error codes */
typedef enum{
    /** \b 0x81 */ PH_ERR_XTAL_ACT_TO_ERROR = (PH_ERR_CUSTOM_BEGIN+0x01),    /**< XTAL Osc Timeout error */
    /** \b 0x82 */ PH_ERR_CLIF_CLK_IN_ERROR,    /**< CLIF PLL Clock Input Not 27M12 */
    /** \b 0x83 */ PH_ERR_USB_CLK_IN_ERROR,     /**< USB PLL Clock Input Not 27M12 */
    /** \b 0x84 */ PH_ERR_USB_PLL_LOCK_ERROR,   /**< USB PLL Lock Error */
    /** \b 0x85 */ PH_ERR_CLKIN_DETECTION_ERROR /**< USB PLL Clock-In Detection procedure Timed out */
}phhalClkGen_ErrorCodes_t;
/** @} */
/**
 * Enumeration for the clock detection
 */
typedef enum{
    /** \b 0x00 */ E_CLIFPLL_CLKDET_SINGLE, /**< Clock detection is single */
    /** \b 0x01 */ E_CLIFPLL_CLKDET_CONTINOUS   /**< Clock detection is continuous */
}phhalClkGen_ClifPllClkDet_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * @brief CLOCKGEN HAL Initialization function
 * Initializes the clock generation module, enables the clock to the clock generation module.
 * Configures the activation timeout if XTAL is used
 * @param[in] eSource - Source of the clock
 * @param[in] dwXtalActivationTimeOut - Xtal activation timeout value in uSeconds(Typical 2000 uSeconds)
 * @param[in] bKickOnError - Flag used to start the XTAL SW kick
 * @retval    #PH_ERR_SUCCESS Clock generation initialization is success
 */
phStatus_t phhalClkGen_Init(phhalClkGen_Source_t eSource, uint32_t dwXtalActivationTimeOut, uint8_t bKickOnError);

/**
 * @brief CLOCKGEN HAL De initialization function
 * De initializes the clock generation module, disables the clock to the clock generation module.
 * @retval    #PH_ERR_SUCCESS Clock generation initialization is success
 */
phStatus_t phhalClkGen_DeInit(void);

/**
 * @brief CLOCKGEN HAL CLIF clock start function
 * This function starts the CLIF clock with the requested mode
 *
 * @param[in] eClifMode - Mode in which the clock should be started
 * @param[in] bClifPllDetFlag - Clock detection flag
 * @retval    #PH_ERR_SUCCESS CLIF clock is successfully started
 * @maskedret #PH_ERR_CLIF_CLK_IN_ERROR-supply clock doesn't exist/less than 27.12MHz
 */
phStatus_t phhalClkGen_ClifClockStart(phhalClkGen_ClifMode_t eClifMode,phhalClkGen_ClifPllClkDet_t bClifPllDetFlag);

/**
 * @brief CLOCKGEN HAL CLIF clock stop function
 * This function stops the CLIF clock
 * @retval    #PH_ERR_SUCCESS CLIF clock is successfully stopped
 */
phStatus_t phhalClkGen_ClifClockStop(void);

/**
 * @brief CLOCKGEN HAL USB clock start function
 * This function starts the USB PLL clock
 * @param[in] bClkEdge - Number of clock edges
 * @param[in] bClkWindowLen - Window length
 * @param[in] wTimeoutMicrosec - Time out duration in micro seconds.
 * @retval    #PH_ERR_SUCCESS USB PLL clock is started successfully
 * @maskedret #PH_ERR_USB_CLK_IN_ERROR Supply clock doesn't exist/less than 27.12MHz
 * @maskedret #PH_ERR_CLKIN_DETECTION_ERROR PLL Clock Input Detection procedure Timed out
 * @maskedret #PH_ERR_USB_PLL_LOCK_ERROR PLL Lock Timed out
 */
phStatus_t phhalClkGen_UsbClockStart(uint8_t bClkEdge, uint8_t bClkWindowLen, uint16_t wTimeoutMicrosec);

/**
 * @brief CLOCKGEN HAL USB clock stop function
 * This function stops the USB PLL clock
 *
 * @retval    #PH_ERR_SUCCESS USB PLL clock is successfully stopped
 */
phStatus_t phhalClkGen_UsbClockStop(void);

/**
 * Ensure we have a 27.12 MHz Clock available.
 */
phStatus_t phhalClkGen_Clk27m12Detect(void);

#endif /* NXPBUILD__PHHAL_CLKGEN */
/** @} */
#endif /* PHHALCLKGEN_H */
