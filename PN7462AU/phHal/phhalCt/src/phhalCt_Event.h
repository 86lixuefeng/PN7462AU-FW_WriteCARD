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
 * This file contains all the CT Events and functions for handling CT Events.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-08-08 18:10:10 +0530 (Mon, 08 Aug 2016) $
 * $Author: Stevenson James (NXP66090) $
 * $Revision: 18579 $ (v04.13.00)
 */


#ifndef PHHALCT_EVENT_H
#define PHHALCT_EVENT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT

#include "ph_Status.h"
#include "phFlashBoot_Event.h"
#include "PN7462AU/PN7462AU_regcore.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */



/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** CT Event Type.*/
typedef enum phhalCt_EventType {

    E_PH_HALCT_EVENT_WAITING                = 0, /**< Default Waiting State.*/
    E_PH_HALCT_EVENT_RX                     = (1 << 0), /**< Successfully Data Reception Event in Fifo.*/
    E_PH_HALCT_EVENT_TX                     = (1 << 1), /**< Successfully Data Transmission Event from Fifo.*/
    E_PH_HALCT_EVENT_TO1                    = (1 << 2),/**< Timer 1 Elapsed.*/
    E_PH_HALCT_EVENT_TO2                    = (1 << 3), /**< Timer 2 Elapsed.*/
    E_PH_HALCT_EVENT_TO3                    = (1 << 4), /**< Timer 3 Elapsed.*/
    E_PH_HALCT_EVENT_MUTE                   = (1 << 5), /**< Card Mute is detected during Activation.*/
    E_PH_HALCT_EVENT_PARITY                 = (1 << 6), /**< Parity Error Detected.*/
    E_PH_HALCT_EVENT_CARD_REMOVED           = (1 << 7), /**< Card removal detected.*/
    E_PH_HALCT_EVENT_EARLY                  = (1 << 8), /**< Early ATR is detected during activation.*/
    E_PH_HALCT_EVENT_FRM_ERR                = (1 << 9), /**< Framing Error detected during Reception.*/
    E_PH_HALCT_EVENT_OVR_ERR                = (1 << 10), /**<  Fifo Overflow Detected.*/
    E_PH_HALCT_EVENT_PTL_ERR                = (1 << 11), /**< Temperature Latch Detected.*/
    E_PH_HALCT_EVENT_PROTL_ERR              = (1 << 12), /**< Protection Latch Detected.*/
    E_PH_HALCT_EVENT_ASYNC                  = (1 << 13), /**< PMU DCDC Error Detected.*/
    E_PH_HALCT_EVENT_CWT_TIMEOUT            = (1 << 14),  /**< CWT Timer Elapsed.*/
    E_PH_HALCT_EVENT_ALL                    = (
            E_PH_HALCT_EVENT_RX |
            E_PH_HALCT_EVENT_TX |
            E_PH_HALCT_EVENT_TO1 |
            E_PH_HALCT_EVENT_TO2 |
            E_PH_HALCT_EVENT_TO3 |
            E_PH_HALCT_EVENT_MUTE |
            E_PH_HALCT_EVENT_PARITY |
            E_PH_HALCT_EVENT_FRM_ERR |
            E_PH_HALCT_EVENT_OVR_ERR |
            E_PH_HALCT_EVENT_PTL_ERR |
            E_PH_HALCT_EVENT_PROTL_ERR |
            E_PH_HALCT_EVENT_ASYNC |
            E_PH_HALCT_EVENT_CWT_TIMEOUT
          ),            /**< All Events Enabled */
          E_PHHAL_CT_ERROR_EVENTS = (
                  E_PH_HALCT_EVENT_PARITY |
                  E_PH_HALCT_EVENT_FRM_ERR |
                  E_PH_HALCT_EVENT_OVR_ERR |
                  E_PH_HALCT_EVENT_PTL_ERR |
                  E_PH_HALCT_EVENT_PROTL_ERR |
                  E_PH_HALCT_EVENT_CARD_REMOVED |
                  E_PH_HALCT_EVENT_ASYNC) /**< All Error Events for CT.*/
}phhalCt_EventType_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * @brief   This function initializes the Event for the CT.
 * @return Status of operation
 * @maskedret #PH_ERR_FAILED Unable to create the event group.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalCt_Event_Init(void);

/**
 * @brief   This function will wait for any given event.
 * @param[in] eEventType       Event type.
 * @param[in] dwTimeout Timeout value(ms).
 * @param[out] pRcvdEvt      Received events.
 * @param[in] fConsume       Flag to check events to be consumed before wating or not
 * @return Status of operation
 * @maskedret #PH_ERR_IO_TIMEOUT    No event occurred.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalCt_Event_WaitAny(phhalCt_EventType_t eEventType, uint32_t dwTimeout,
        phhalCt_EventType_t * pRcvdEvt, uint32_t fConsume);


/**
 * @brief   This function post the event.
 * @param[in] eEventType   Event type.
 * @maskedret #PH_ERR_INVALID_PARAMETER Event group is not initialized.
 * @maskedret #PH_ERR_FAILED Event could not be set.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t  phhalCt_Event_Post(phhalCt_EventType_t eEventType);

/**
 * @brief   This function consumes the event.
 * @param[in] eEventType   Event type.
 * @maskedret #PH_ERR_INVALID_PARAMETER Event group is not initialized.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t  phhalCt_Event_Consume(phhalCt_EventType_t eEventType);


/**
 * @brief   This function de-initialize the events.
 * @return Status of the API
 * @maskedret #PH_ERR_INVALID_PARAMETER Event group is not initialized.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalCt_Event_Deinit(void);

#endif /* NXPBUILD__PHHAL_CT */

#endif
