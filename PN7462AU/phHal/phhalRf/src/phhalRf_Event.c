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
 * Prototype of CLIF RF HAL API Event Mechanism with RTOS.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-22 17:42:08 +0530 (Tue, 22 Dec 2015) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 17116 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_RF

#include "phhalRf.h"
#include "phhalSysSer.h"
#include "phhalRf_Event.h"
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
/**
 * Global Status of ISR Event Register Status to Receive Events from ISR
 */
PH_NOINIT static volatile phRtos_EventHandle_t gphhalRf_EventGroup;

/**
 * Source of Events
 */
PH_NOINIT static volatile phhalRf_EventSource_t gphhalRf_EvtLastSource;
/**
 * Destination of Events
 */
PH_NOINIT static volatile phhalRf_EventDestination_t gphhalRf_EvtLastDest;
/**
 * Event Detail
 */
PH_NOINIT static volatile phhalRf_EventDetail_t gphhalRf_EvtDetail;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phhalRf_Event_Init(void)
{
    gphhalRf_EventGroup = phRtos_EventGroupCreate();

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    if (gphhalRf_EventGroup == NULL)
    {
        return PH_ERR(RESOURCE_ERROR, RF);
    }
#endif

    gphhalRf_EvtLastSource = E_PH_HALRF_EVT_SRC_NONE;
    gphhalRf_EvtLastDest = E_PH_HALRF_EVT_DEST_NONE;
    gphhalRf_EvtDetail = E_PH_HALRF_EVT_DETAIL_SUCCESS;
    return PH_ERR_SUCCESS;
}

phStatus_t phhalRf_Event_GetEventHandle(void *pHandle)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    /* In case it is not initialized yet */
    if (gphhalRf_EventGroup == NULL)
    {
        return PH_ERR(USE_CONDITION, RF);
    }
#endif

    if ( pHandle != NULL) {
        (* (phRtos_EventHandle_t *)pHandle) = gphhalRf_EventGroup;
    }
    return PH_ERR_SUCCESS;
}

void phhalRf_Event_Post(phhalRf_EventType_t eEvtType, phhalRf_EventSource_t eSrc, phhalRf_EventDestination_t eDest)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    phRtos_Status_t xBaseRet = (phRtos_Status_t)0;

    /* In case it is not initialized yet */
    if (gphhalRf_EventGroup != NULL)
    {
#endif

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
        xBaseRet = /* We are interested in the value only conditionally */
#endif
            phRtos_EventGroupSetBits(gphhalRf_EventGroup, (uint32_t) eEvtType);

        /* Set the Last Source */
        gphhalRf_EvtLastSource = eSrc;

        /* Set the Last Destination */
        gphhalRf_EvtLastDest = eDest;

        /* Set Default Event Detail */
        gphhalRf_EvtDetail = E_PH_HALRF_EVT_DETAIL_SUCCESS;

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
        if ((xBaseRet) == phRtos_Failure )
        {
            gphhalRf_EvtDetail = E_PH_HALRF_EVT_DETAIL_ERROR;
        }
    }
#endif

}

void phhalRf_Event_Consume(phhalRf_EventType_t eEvtType, phhalRf_EventSource_t eSrc)
{
    uint32_t xRet = 0;

    xRet = (uint32_t) phRtos_EventGroupClearBits(gphhalRf_EventGroup, (uint32_t) eEvtType);

    if ((((uint32_t) xRet) & ((uint32_t) eEvtType)) == 0)
    {
        gphhalRf_EvtDetail = E_PH_HALRF_EVT_DETAIL_ERROR;
    }

    /* Set the Last Source */
    gphhalRf_EvtLastSource = eSrc;

}

phStatus_t phhalRf_Event_WaitAny(phhalRf_EventType_t eEvtType, uint32_t dwTimeoutCount,
    phhalRf_EventType_t *pRcvdEvt)
{
    phStatus_t status = PH_ERR_SUCCESS;
    uint32_t xbits = 0;

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    PH_RETURN_IF_TRUE_WITH_ERROR((eEvtType > E_PH_HALRF_EVT_TYPE_ALL), INVALID_PARAMETER, RF)

    if (gphhalRf_EventGroup == NULL) /* In case it is not initialized yet */
    {
        return PH_ERR(RESOURCE_ERROR, RF);
    }
#endif

    /* Only Possible from HAL */

    /* Don't Clear flags after ISR and Don't Wait for All bits to be Set */
    xbits = phRtos_EventGroupWaitBits(gphhalRf_EventGroup, (uint32_t) eEvtType, false, false, dwTimeoutCount);
    if ((xbits & ((uint32_t) eEvtType)) != 0)
    {
        status = PH_ERR_SUCCESS;
    }
    else
    {
        status = PH_ERR(IO_TIMEOUT, RF);
    }

    /* Return the Events That were Set */
    *pRcvdEvt = (phhalRf_EventType_t) xbits;
    return status;
}

phStatus_t phhalRf_Event_WaitAll(phhalRf_EventType_t eEvtType, uint32_t dwTimeoutCount,
    phhalRf_EventType_t *pRcvdEvt)
{
    phStatus_t status = PH_ERR_SUCCESS;
    uint32_t xbits = 0;

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    PH_RETURN_IF_TRUE_WITH_ERROR((eEvtType > E_PH_HALRF_EVT_TYPE_ALL), INVALID_PARAMETER, RF)

    if (gphhalRf_EventGroup == NULL) /* In case it is not initialized yet */
    {
        return PH_ERR_SUCCESS;
    }
#endif

    /* Only Possible from HAL */

    /* Don't Clear flags after ISR and Wait for All bits to be Set */
    xbits = phRtos_EventGroupWaitBits(gphhalRf_EventGroup, (uint32_t) eEvtType, false, true, dwTimeoutCount);
    if ((xbits & ((uint32_t) eEvtType)) == ((uint32_t) eEvtType))
    {
        status = PH_ERR_SUCCESS;
    }
    else
    {
        status = PH_ERR(IO_TIMEOUT, RF);
    }

    /* Return the Events That were Set */
    *pRcvdEvt = (phhalRf_EventType_t) xbits;
    return status;
}

void phhalRf_Event_SetDetail(phhalRf_EventDetail_t eEvtDetail)
{
    gphhalRf_EvtDetail = eEvtDetail;
}

void phhalRf_Event_GetDetail(phhalRf_EventDetail_t * eEvent_Detail)
{
    *eEvent_Detail = gphhalRf_EvtDetail;
}

phStatus_t phhalRf_Event_DeInit(void)
{

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_SW_RESOURCE
    if (gphhalRf_EventGroup == NULL)
    {
        return PH_ERR(RESOURCE_ERROR, RF);
    }
#endif

    /* Delete the Event Group */
    phRtos_EventGroupDelete(gphhalRf_EventGroup);

    gphhalRf_EvtLastSource = E_PH_HALRF_EVT_SRC_NONE;
    gphhalRf_EvtLastDest = E_PH_HALRF_EVT_DEST_NONE;
    gphhalRf_EvtDetail = E_PH_HALRF_EVT_DETAIL_SUCCESS;
    return PH_ERR_SUCCESS;
}

void phhalRf_Event_PostwithDetail(phhalRf_EventType_t eEvtType, phhalRf_EventSource_t eSrc,
    phhalRf_EventDestination_t eDest, phhalRf_EventDetail_t eDetail)
{
    phhalRf_Event_Post(eEvtType, eSrc, eDest);
    phhalRf_Event_SetDetail(eDetail);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_RF */
