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
 * SPI Master event based mechanism implementation.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 11:16:59 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16970 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SPIM
#include "phhalSPIM.h"
#include "phhalSPIM_Event.h"
#include "PN7462AU/PN7462AU_spim.h"
#include "phRtos.h"

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
/* The Event signals used by SPIM tasks. */
PH_NOINIT static phRtos_EventHandle_t SPIM_TransactionEvent = NULL;
PH_NOINIT static uint32_t dwEventTimeOut = 0;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalSPIM_EventRegister(uint32_t dwSpimTimeOut PH_UNUSED)
{

    dwEventTimeOut  = dwSpimTimeOut;

    SPIM_TransactionEvent =  phRtos_EventGroupCreate();
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    /* Was the event group created successfully? */
    PH_RETURN_IF_TRUE_WITH_ERROR((SPIM_TransactionEvent == NULL),RESOURCE_ERROR, SPIM )
#endif
    return PH_ERR_SUCCESS;
}

/*
 * phhalSPIM_EventHandler Event handler is called from Interrupt/Handler context.
 *
 * */
phStatus_t phhalSPIM_EventHandler(phStatus_t eStatus PH_UNUSED)
{

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    phRtos_Status_t e_phRtosStatus;
#endif

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    e_phRtosStatus = /* We are interested in the value only conditionally */
#endif
        phRtos_EventGroupSetBits(SPIM_TransactionEvent, (uint32_t) eStatus);

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    PH_RETURN_IF_TRUE_WITH_ERROR((e_phRtosStatus != phRtos_Success),RESOURCE_ERROR,SPIM)
#endif
    return PH_ERR_SUCCESS;
}

phStatus_t phhalSPIM_EventWait(void)
{
    uint32_t dwBits;

    dwBits = phRtos_EventGroupWaitBits(
            SPIM_TransactionEvent,
            SPIM_INT_STATUS_REG_AHB_ADDR_ERROR_STATUS_MASK
            | SPIM_INT_STATUS_REG_AHB_ERROR_STATUS_MASK
            | SPIM_INT_STATUS_REG_EOT_STATUS_MASK
            | SPIM_INT_STATUS_REG_EOR_STATUS_MASK,
            true,          /* status bits should be cleared before returning. */
            false,         /* wait for any status bit being set. */
            dwEventTimeOut ); /* wait till timeout. */

    if ( dwBits & (SPIM_INT_STATUS_REG_AHB_ADDR_ERROR_STATUS_MASK | SPIM_INT_STATUS_REG_AHB_ERROR_STATUS_MASK) )
    {
        dwBits >>= 8;
        return (uint16_t)(dwBits | PH_ERR_CUSTOM_BEGIN | PH_COMP_SPIM);
    }else if ( dwBits & (SPIM_INT_STATUS_REG_EOT_STATUS_MASK | SPIM_INT_STATUS_REG_EOR_STATUS_MASK) ){
        return PH_ERR_SUCCESS;
    }
    else{
        return PH_ERR(OPERATION_TIMEDOUT, SPIM);
    }
}

void phhalSPIM_EventUnRegister(void)
{
    /* Delete an event group that was previously created by phhalSPIM_EventRegister. */
    phRtos_EventGroupDelete(SPIM_TransactionEvent);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_SPIM */
