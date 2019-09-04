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
 * CT RTOS event mechanism.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 11:44:15 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16973 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalNvic.h"
#include "phhalCt.h"

/* Event Based Functionality. */
#include "phhalCt_Event.h"

/* Kernel includes. */
#include "phRtos.h"
#include "phFlashBoot_Event.h"


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

static volatile phRtos_EventHandle_t gphhalCt_EventGroup = NULL;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalCt_Event_Init(void)
{
    gphhalCt_EventGroup = phRtos_EventGroupCreate();
    if (gphhalCt_EventGroup == NULL)
        return PH_ERR(FAILED,HAL_CT);

    return PH_ERR_SUCCESS;
}

phStatus_t phhalCt_Event_WaitAny(phhalCt_EventType_t eEventType,
        uint32_t dwTimeout, phhalCt_EventType_t * pRcvdEvt, uint32_t fConsume)
{
    phStatus_t eStatus = PH_ERR_FAILED;
    uint32_t xbits = 0;

    if (gphhalCt_EventGroup == NULL) /* In case it is not initialized yet */
        return PH_ERR(FAILED,HAL_CT);

    /* Clear the Events */
    if (fConsume == TRUE)
    {
        eStatus = phhalCt_Event_Consume(eEventType);
    }

    /* Don't Clear flags after ISR and Wait for any bits to be Set */
    xbits = phRtos_EventGroupWaitBits(gphhalCt_EventGroup,(uint32_t)eEventType,false,false,dwTimeout);
    if( (xbits & ((uint32_t)eEventType)) != 0 )
    {
        /* Return the Events That were Set */
        *pRcvdEvt = (phhalCt_EventType_t) xbits;
        eStatus = PH_ERR_SUCCESS;
    }
    else
    {
        eStatus = PH_ERR_OPERATION_TIMEDOUT;
    }

    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}
/**
 *
 * @param eEventType
 * @return
 */
phStatus_t  phhalCt_Event_Post(phhalCt_EventType_t eEventType)
{
    if( gphhalCt_EventGroup == NULL )    /* In case it is not initialized yet */
        return PH_ERR(INVALID_PARAMETER,HAL_CT);

    if(phRtos_EventGroupSetBits(gphhalCt_EventGroup, (uint32_t)eEventType)!= phRtos_Success){
        return PH_ERR(FAILED, HAL_CT);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t  phhalCt_Event_Consume(phhalCt_EventType_t eEventType)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    if( gphhalCt_EventGroup == NULL )    /* In case it is not initialized yet */
        return PH_ERR(INVALID_PARAMETER,HAL_CT);
#endif

    (void)phRtos_EventGroupClearBits(gphhalCt_EventGroup, (uint32_t)eEventType);

    return PH_ERR_SUCCESS;
}

phStatus_t phhalCt_Event_Deinit(void)
{

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    if (gphhalCt_EventGroup == NULL)
    {
        return PH_ERR(INVALID_PARAMETER,HAL_CT);
    }
#endif

    /* Delete the Event Group */
    phRtos_EventGroupDelete(gphhalCt_EventGroup);
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHHAL_CT */
