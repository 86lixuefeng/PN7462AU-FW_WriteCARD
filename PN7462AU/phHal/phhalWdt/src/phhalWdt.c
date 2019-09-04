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
 * WDT HAL implementation for its configuration and access.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-07-27 09:47:10 +0530 (Mon, 27 Jul 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14132 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_WDT
#include "ph_Reg.h"
#include "phhalWdt.h"
#include "PN7462AU/PN7462AU_timers.h"
#include "ph_Status.h"

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
PH_NOINIT static pphhalWdt_Handler_t gpWdtHandler;
PH_NOINIT static uint8_t gbResetPN7462AU;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */


phStatus_t phhalWdt_Start( uint16_t pWdtTimeoutVal, uint16_t pWdtIntThreshVal, pphhalWdt_Handler_t pHandler,
    uint8_t bResetOnTimeOut )
{
    gbResetPN7462AU = bResetOnTimeOut;

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
   if( (pHandler != NULL) && (pWdtTimeoutVal != 0) && ((pWdtIntThreshVal != 0) && (pWdtIntThreshVal<pWdtTimeoutVal)) )
#endif
   {

       gpWdtHandler = pHandler;
       /** Configure watch-dog threshold  */
       PH_REG_SET(TIMERS_WDOG_TRIGGER_INT_REG,
               (pWdtIntThreshVal&TIMERS_WDOG_TRIGGER_INT_REG_WDOG_INT_THRESHOLD_MASK));
       /** Configure watch-dog reset timeout  */
       PH_REG_SET(TIMERS_WDOG_TIMEOUT_REG, (pWdtTimeoutVal&TIMERS_WDOG_TIMEOUT_REG_WDOG_TIMEOUT_MASK));

       phhalWdt_Refresh();
   }
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
   else
   {
       return PH_ERR(INVALID_PARAMETER, TIMER);
   }
#endif
   return PH_ERR_SUCCESS;
}

/*
 * Description   : Watch dog timer ISR.
 */
void NMI_Handler(void)
{
    /** Perform WDT re kick if PN7462AU reset is disabled on WDT Timeout. */
    if(!gbResetPN7462AU){
        phhalWdt_Refresh();
    }
    /** call upper layer function to save context */
    gpWdtHandler();
    PH_REG_SET_BIT_WO(TIMERS_WDOG_INT_CLR_STATUS_REG,WDOG_TIMEOUT_CLR_STATUS);
    PH_REG_SET(TIMERS_WDOG_INT_CLR_STATUS_REG, (~TIMERS_WDOG_INT_CLR_STATUS_REG_WDOG_TIMEOUT_CLR_STATUS_MASK));
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_WDT */
