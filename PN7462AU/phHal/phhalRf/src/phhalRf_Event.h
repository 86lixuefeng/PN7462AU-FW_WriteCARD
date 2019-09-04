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
 * Prototype of CLIF RF HAL API.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-11 13:30:09 +0530 (Mon, 11 Jul 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18565 $ (v04.13.00)
 */

#ifndef PHHALRF_EVENT_H
#define PHHALRF_EVENT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_RF

#include "phhalRf.h"

#ifndef PH_HALRF_EVENT_SYSSV_SYSTICK_DISABLE
#include "phhalSysSer.h"
#endif

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PHHAL_RF_EVENT_WAIT_MIN      (0x01)           /**< Minimum wait time for any event.*/
#define PHHAL_RF_EVENT_WAIT_CONST    (6000)           /**< Minimum wait time for any event.*/
#define PHHAL_RF_EVENT_WAIT_MAX      (0xFFFFFFFF)     /**< Maximum wait time for any event.*/
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 * RF Event Type
 */
typedef enum phhalRf_EventType{
    E_PH_HALRF_EVT_TYPE_NONE = (0U),                             /**< Default Waiting State.*/
    E_PH_HALRF_EVT_TYPE_IDLESTATE = (1U << 0),                   /**< Idle State Invoked.*/
    E_PH_HALRF_EVT_TYPE_FLDON_IN = (1U << 1),                    /**< Internal Field Turned On.*/
    E_PH_HALRF_EVT_TYPE_FLDOFF_IN = (1U << 2),                   /**< Internal Field Turned Off.*/
    E_PH_HALRF_EVT_TYPE_FLDON_EX = (1U << 3),                    /**< External Field Turned On Detected.*/
    E_PH_HALRF_EVT_TYPE_FLDOFF_EX = (1U << 4),                   /**< External Field Turned Off Detected.*/
    E_PH_HALRF_EVT_TYPE_TIMER0 = (1U << 5),                      /**< Timer 0 Expired.*/
    E_PH_HALRF_EVT_TYPE_TIMER1 = (1U << 6),                      /**< Timer 1 Expired.*/
    E_PH_HALRF_EVT_TYPE_TIMER2 = (1U << 7),                      /**< Timer 2 Expired.*/
    E_PH_HALRF_EVT_TYPE_TIMER3 = (1U << 8),                      /**< Timer 3 Expired.*/
    E_PH_HALRF_EVT_TYPE_TRANSMIT_ENDED = (1U << 9),             /**< Transmission Completed.*/
    E_PH_HALRF_EVT_TYPE_RECEIVE_ENDED = (1U << 10),              /**< Reception Completed.*/
    E_PH_HALRF_EVT_TYPE_EXCHG_FAIL = (1U << 11),                 /**< Exchange Failure Status.*/
    E_PH_HALRF_EVT_TYPE_RECEIVE_BUFFER_OVERFLOW = (1U << 12),    /**< Exchange Failure due to Receive Buffer Overflow.*/
    E_PH_HALRF_EVT_TYPE_CMA_ACTIVATED = (1U << 13),              /**< CMA Activated.*/
    E_PH_HALRF_EVT_TYPE_RX_SOF_DET = (1U << 14),                 /**< Start Of Frame.*/
    E_PH_HALRF_EVT_TYPE_RX_SC_DET = (1U << 15),                 /**< Sub Carrier detected.*/
    E_PH_HALRF_EVT_TYPE_RF_ACT_ERR_04 = (1U << 16),             /**< RF Active Error.*/
    E_PH_HALRF_EVT_TYPE_INTERNAL_ERROR = (1U << 17),            /**< Internal Error. */
    E_PH_HALRF_EVT_TYPE_ALL= (
        E_PH_HALRF_EVT_TYPE_IDLESTATE |
        E_PH_HALRF_EVT_TYPE_FLDON_IN |
        E_PH_HALRF_EVT_TYPE_FLDOFF_IN |
        E_PH_HALRF_EVT_TYPE_FLDON_EX |
        E_PH_HALRF_EVT_TYPE_FLDOFF_EX |
        E_PH_HALRF_EVT_TYPE_TIMER0 |
        E_PH_HALRF_EVT_TYPE_TIMER1 |
        E_PH_HALRF_EVT_TYPE_TIMER2 |
        E_PH_HALRF_EVT_TYPE_TIMER3 |
        E_PH_HALRF_EVT_TYPE_TRANSMIT_ENDED |
        E_PH_HALRF_EVT_TYPE_RECEIVE_ENDED |
        E_PH_HALRF_EVT_TYPE_EXCHG_FAIL |
        E_PH_HALRF_EVT_TYPE_RECEIVE_BUFFER_OVERFLOW |
        E_PH_HALRF_EVT_TYPE_CMA_ACTIVATED |
        E_PH_HALRF_EVT_TYPE_RF_ACT_ERR_04 |
        E_PH_HALRF_EVT_TYPE_RX_SOF_DET |
        E_PH_HALRF_EVT_TYPE_INTERNAL_ERROR),            /**< All Events Enabled */
        E_PH_HALRF_EVT_TYPE_MAX = (E_PH_HALRF_EVT_TYPE_ALL + 1),  /**< Comparison Maximum */
        E_PH_HALRF_SIZEADJUST_EVT_TYPE = 0x7FFFFFFFU   /**< For 32bit Enum Adjustment */
}phhalRf_EventType_t;

/**
 * Event Detail Description
 */
typedef enum phhalRf_EventDetail{
    E_PH_HALRF_EVT_DETAIL_SUCCESS = 0,                /**< Success Status.*/
    E_PH_HALRF_EVT_DETAIL_RX_COLLISION = 1,           /**< Collision in the Receiver.*/
    E_PH_HALRF_EVT_DETAIL_RX_FRAME_ERROR = 2,         /**< Error in RX Framing.*/
    E_PH_HALRF_EVT_DETAIL_RX_WAIT_ERROR = 3,          /**< Error in RX Wait time.*/
    E_PH_HALRF_EVT_DETAIL_RX_NOT_STARTED = 4,         /**< Error in RX did not Start.*/
    E_PH_HALRF_EVT_DETAIL_RX_NOT_ENDED = 5,           /**< Error in RX did not End.*/
    E_PH_HALRF_EVT_DETAIL_RX_LATE_START = 6,          /**< Error RX Started after FDT and still recieving.*/
    E_PH_HALRF_EVT_DETAIL_FDT_ERROR = 7,              /**< Error RX Started after FDT  and completed.*/
    E_PH_HALRF_EVT_DETAIL_EXCHANGE_ERROR = 8,         /**< Error RX Started after FDT  and completed.*/
    E_PH_HALRF_EVT_DETAIL_RX_INCOMPLETE_BYTE = 9,     /**< Error RX has got incomplete byte.*/
    E_PH_HALRF_EVT_DETAIL_TX_NOT_STARTED = 10,        /**< Error the Transmitter did not Start.*/
    E_PH_HALRF_EVT_DETAIL_TX_NOT_ENDED = 11,          /**< Error the Transmitter did not End transmission.*/
    E_PH_HALRF_EVT_DETAIL_RFON_ERROR = 12,            /**< Generic Error in RF On.*/
    E_PH_HALRF_EVT_DETAIL_RFON_TIDT = 13,             /**< Error in RF On in Tidt External Field Detection.*/
    E_PH_HALRF_EVT_DETAIL_RFON_TADT = 14,             /**< Error in RF On in Tadt External Field Detection.*/
    E_PH_HALRF_EVT_DETAIL_ERROR = 15,                 /**< general Error.*/
    E_PH_HALRF_SIZEADJUST_EVT_DETAIL = 0x7FFFFFFFU    /**< For 32bit Enum Adjustment.*/
}phhalRf_EventDetail_t;

/**
 * RF Event Source Type
 */
typedef enum phhalRf_EventSource{
    E_PH_HALRF_EVT_SRC_ISR = 0,                  /**< Event was generated from the ISR */
    E_PH_HALRF_EVT_SRC_HAL = 1,                  /**< Source is HAL */
    E_PH_HALRF_EVT_SRC_NONE = 2,                 /**< No Source for Event or Broadcast Event */
    E_PH_HALRF_SIZEADJUST_EVT_SRC = 0x7FFFFFFFU  /**< For 32bit Enum Adjustment */
}phhalRf_EventSource_t;

/**
 * RF Event Destination Type
 */
typedef enum phhalRf_EventDestination{
    E_PH_HALRF_EVT_DEST_HAL = 0,                  /**< Event's Destination is HAL */
    E_PH_HALRF_EVT_DEST_APP = 1,                  /**< Event's Destination is Application */
    E_PH_HALRF_EVT_DEST_NONE = 2,                 /**< None Event Dest to Discard the Event */
    E_PH_HALRF_SIZEADJUST_EVT_DEST = 0x7FFFFFFFU  /**< For 32bit Enum Adjustment */
}phhalRf_EventDestination_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * @brief   This function initializes the Event for the Clif.
 * @return Status of the API
 * @maskedret #PH_ERR_RESOURCE_ERROR Unable to create the event group.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalRf_Event_Init(void);

/**
 * @brief   This function gives the Event handle initialized for CLif.
 * @param[out] pHandle Pointer which will point to the Event handle.
 * @return Status of the API
 * @maskedret #PH_ERR_RESOURCE_ERROR The event group is null.
 */
phStatus_t phhalRf_Event_GetEventHandle(void *pHandle);

/**
 * @brief   This function post the event.
 * @param[in] eEvtType   Event type.
 * @param[in] eSrc       Event source.
 * @param[in] eDest      Event destination.
 */
void phhalRf_Event_Post(
    phhalRf_EventType_t eEvtType,
    phhalRf_EventSource_t eSrc,
    phhalRf_EventDestination_t eDest);

/**
 * @brief   This function post the event with detail
 * @param[in] eEvtType   Event type.
 * @param[in] eSrc       Event source.
 * @param[in] eDest      Event destination.
 * @param[in] eDetail    Event detail.
 */
void phhalRf_Event_PostwithDetail(
    phhalRf_EventType_t eEvtType,
    phhalRf_EventSource_t eSrc,
    phhalRf_EventDestination_t eDest,
    phhalRf_EventDetail_t eDetail);

/**
 * @brief   This function consumes the event.
 * @param[in] eEvtType   Event type.
 * @param[in] eSrc       Event source.
 */
void phhalRf_Event_Consume(
    phhalRf_EventType_t eEvtType,
    phhalRf_EventSource_t eSrc);

/**
 * @brief   This function will wait for any given event.
 * @param[in] eEvtType       Event type.
 * @param[in] dwTimeoutCount Timeout value(ms).
 * @param[out] pRcvdEvt      Received events.
 * @return Status of the API.
 * @maskedret #PH_ERR_IO_TIMEOUT    No event occurred.
 */
phStatus_t phhalRf_Event_WaitAny(
    phhalRf_EventType_t eEvtType,
    uint32_t dwTimeoutCount,
    phhalRf_EventType_t *pRcvdEvt);

/**
 * @brief This function will wait for all the given events
 * @param[in] eEvtType       Event type.
 * @param[in] dwTimeoutCount Timeout value(ms).
 * @param[out] pRcvdEvt      Received events.
 * @return Return Values
 * @maskedret #PH_ERR_IO_TIMEOUT    No event occurred.
 */
phStatus_t phhalRf_Event_WaitAll(
    phhalRf_EventType_t eEvtType,
    uint32_t dwTimeoutCount,
    phhalRf_EventType_t *pRcvdEvt);

/**
 * @brief This function will set the details of the event
 *
 * @param[in] eEvtDetail     Event detail.
 */
void phhalRf_Event_SetDetail(phhalRf_EventDetail_t eEvtDetail);

/**
 * @brief   This function gives the event detail
 * @param[out] eEvent_Detail Pointer which will point to the detailed event variable.
 */
void phhalRf_Event_GetDetail(phhalRf_EventDetail_t * eEvent_Detail);

/**
 * @brief   This function de-initialize the events.
 * @return Status of the API
 * @maskedret #PH_ERR_RESOURCE_ERROR    In case the event group is null.
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalRf_Event_DeInit(void);

/***********************************************************************************************/
#endif /* NXPBUILD__PHHAL_RF */

#endif /* PHHALRF_EVENT_H */
