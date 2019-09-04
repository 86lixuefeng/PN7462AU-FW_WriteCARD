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
 * WDT HAL Implementation for WDT Module
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALWDT_H
#define PHHALWDT_H

/** \addtogroup phhalWdt
 *
 * The Watch Dog Timer (HAL) APIs.
 *
 * This module implements phhalWdt interface Flash code HAL, specific to phhalWdt module.
 *
 * @warning
 *
 *  If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *  checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *  and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *  failures.
 *
 *  @{
 *
 * \dot
 *
 *      digraph "Sample Usage"{
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalWdt_DeInit [URL ="\ref phhalWdt_DeInit"];
 *          phhalWdt_GetStatus [URL ="\ref phhalWdt_GetStatus"];
 *          phhalWdt_Refresh [URL ="\ref phhalWdt_Refresh"];
 *          phhalWdt_Start [URL ="\ref phhalWdt_Start"];
 *
 *      //Logical Flow
 *          Start -> phhalWdt_Start -> phhalWdt_Refresh;
 *          phhalWdt_Refresh -> phhalWdt_DeInit;
 *          phhalWdt_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *          phhalWdt_Start -> phhalWdt_GetStatus;
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 *
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_WDT
#include "ph_Status.h"
#include "PN7462AU/PN7462AU_timers.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/**
* This macro refresh/restarts the watch dog timer.
*/
#define phhalWdt_Refresh() \
	PH_REG_SET(TIMERS_WDOG_CONTROL_REG, \
			TIMERS_WDOG_CONTROL_REG_WDOG_KICK_MASK)

/**
* This macro reads the watch dog timer current count value in step size of 21.5ms.
*/
#define phhalWdt_GetStatus() \
	PH_REG_GET(\
			TIMERS_WDOG_COUNT_REG)

/**
* This macro disables the watch dog timer.
*/
#define phhalWdt_DeInit() PH_REG_SET(TIMERS_WDOG_TIMEOUT_REG, 0x00000000UL)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Function prototype of the watch-dog timer handler which is called by the watch-dog ISR.
 */
typedef void (*pphhalWdt_Handler_t)(void);

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * This function initializes the watch dog timer, time out value and threshold value.
 * @param[in] pWdtTimeoutVal Watch dog time out value in step size of 21.5ms.
 * @param[in] pWdtIntThreshVal Watch dog timer threshold value.
 * @param[in] pHandler Pointer to user callback in WDT ISR.
 * @param[in] bResetOnTimeOut bool 1 --> PN7462AU reset on WDT Timeout, 0 --> No reset.
 * @return Status of the Operation
 *
 * @retval #PH_ERR_SUCCESS - WDT initialized successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER - Passing parameter error.
 */
phStatus_t phhalWdt_Start(uint16_t pWdtTimeoutVal, uint16_t pWdtIntThreshVal, pphhalWdt_Handler_t pHandler,
        uint8_t bResetOnTimeOut);

/**
 * WDT ISR handler.
 *
 * This API is part the Vector Table and directly called in the event of WDT Expire Event.
 */
void NMI_Handler(void);

#endif /* NXPBUILD__PHHAL_WDT */
/** @} */
#endif /* PHHALWDT_H */
