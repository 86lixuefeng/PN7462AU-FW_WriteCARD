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
 * I2C Master event based mechanism implementation.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 11:15:22 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16968 $ (v04.13.00)
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM
#include "phhalI2CM.h"
#include "PN7462AU/PN7462AU_i2cm.h"
#include "phhalI2CM_Event.h"
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
PH_NOINIT static phRtos_EventHandle_t I2CM_TransactionEvent;
PH_NOINIT static uint32_t dwI2CM_TicksToWait;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalI2CM_EventRegister(uint32_t dwTicksToWait PH_UNUSED)
{
    dwI2CM_TicksToWait = dwTicksToWait;
    I2CM_TransactionEvent =  phRtos_EventGroupCreate();

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    /* Was the event group created successfully */
    PH_RETURN_IF_TRUE_WITH_ERROR(( I2CM_TransactionEvent == NULL ),RESOURCE_ERROR, I2CM)
#endif

    return PH_ERR_SUCCESS;
}

/**
 * phhalI2CM_EventHandler Event handler is called from Interrupt/Handler context.
 *
 */
phStatus_t phhalI2CM_EventHandler(uint8_t bId PH_UNUSED, uint32_t dwStatus PH_UNUSED)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    phRtos_Status_t e_phRtosStatus;
#endif

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    e_phRtosStatus = phRtos_EventGroupSetBits( I2CM_TransactionEvent, dwStatus);
    PH_RETURN_IF_TRUE_WITH_ERROR((e_phRtosStatus != phRtos_Success),RESOURCE_ERROR,I2CM)
#else
    phRtos_EventGroupSetBits( I2CM_TransactionEvent, dwStatus);
#endif

    return PH_ERR_SUCCESS;
}


phStatus_t phhalI2CM_EventWait(void)
{
    uint32_t dwBits;

    dwBits = phRtos_EventGroupWaitBits(
            I2CM_TransactionEvent,
            I2CMASTER_INT_STATUS_REG_I2C_BUS_ERROR_MASK
            | I2CMASTER_INT_STATUS_REG_NACK_MASK
            | I2CMASTER_INT_STATUS_REG_ARB_FAILURE_MASK
            | I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_MASK
            | PH_ERR_ADDR_NAK_ERROR,
            true,          /* status bits should be cleared before returning. */
            false,         /* wait for any status bit being set. */
            dwI2CM_TicksToWait ); /* wait until the time expires. */

    if ( dwBits & (I2CMASTER_INT_STATUS_REG_I2C_BUS_ERROR_MASK
            | I2CMASTER_INT_STATUS_REG_NACK_MASK
            | I2CMASTER_INT_STATUS_REG_ARB_FAILURE_MASK
            | I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_MASK
            | PH_ERR_ADDR_NAK_ERROR) )
    {
        return ((phStatus_t)dwBits);
    }
    else{
         return PH_ERR(OPERATION_TIMEDOUT,I2CM);
    }
}

void phhalI2CM_EventUnRegister(void)
{
    /* Delete an event group that was previously created by phhalI2CM_EventRegister. */
    phRtos_EventGroupDelete(I2CM_TransactionEvent);
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_I2CM */
