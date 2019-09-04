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
 * phRtos_OpenRtos.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-06-24 10:50:38 +0530 (Fri, 24 Jun 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18506 $ (v04.13.00)
 */


/* **************************************************************************
 * Includes
 * ************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phUser.h"

#ifdef PHFL_HALAPI_WITH_RTOS

/* RTOS_PORTING_DISCLAIMER */

#include "portmacro.h"


/* **************************************************************************
 * Internal Definitions
 * ************************************************************************** */


#define PHRTOS_NVIC_SYSTICK_CTRL ( (volatile uint32_t *) 0xe000e010 )
#define PHRTOS_NVIC_PENDSVCLR    0x08000000


/* **************************************************************************
 * Type Definitions
 * ************************************************************************** */

/* **************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ************************************************************************** */

/* **************************************************************************
 * Private Functions Prototypes
 * ************************************************************************** */

/* **************************************************************************
 * Public Functions
 * ************************************************************************** */

phRtos_EventHandle_t phRtos_EventGroupCreate(void) {
    return (phRtos_EventHandle_t) xEventGroupCreate();
}



void phRtos_DisableSysTickTimer(void)
{
    (*(PHRTOS_NVIC_SYSTICK_CTRL) &= (~0x01U));
}

void phRtos_EnableSysTickTimer(void)
{
    (*(PHRTOS_NVIC_SYSTICK_CTRL) |= 0x01U);
}

void phRtos_ClearPendSvInterrupt(void)
{
    (*(PHRTOS_NVIC_INT_CTRL) = PHRTOS_NVIC_PENDSVCLR);
}

phRtos_Status_t phRtos_RTOS_QueueSend(
        phRtos_QueueHandle_t queueHandle,
        const void * pItemToBeQueued,
        uint32_t NumberOfTicksToWait)
{
    BaseType_t IsWakingUpFrmHigherPrioTsk = pdFALSE;
    phRtos_Status_t eStatus;

    if( phRtos_IsIrqContext() )
    {
        eStatus = (phRtos_Status_t)xQueueSendFromISR(
                queueHandle,
                pItemToBeQueued,
                &IsWakingUpFrmHigherPrioTsk);
        portYIELD_FROM_ISR(IsWakingUpFrmHigherPrioTsk);
    }
    else
    {
        eStatus = (phRtos_Status_t)xQueueSend(
                queueHandle, pItemToBeQueued, NumberOfTicksToWait);
    }

    return eStatus;
}

phRtos_Status_t phRtos_RTOS_QueueReceive(
        phRtos_QueueHandle_t queueHandle,
        void *pvBuffer,
        uint32_t NumberOfTicksToWait)
{
    BaseType_t IsWakingUpFrmHigherPrioTsk = pdFALSE;
    phRtos_Status_t eStatus;

    if( phRtos_IsIrqContext() )
    {
        eStatus = (phRtos_Status_t)xQueueReceiveFromISR(
                queueHandle, pvBuffer, &IsWakingUpFrmHigherPrioTsk);
        portYIELD_FROM_ISR(IsWakingUpFrmHigherPrioTsk);
    }
    else
    {
        eStatus = (phRtos_Status_t)xQueueReceive(
                queueHandle, pvBuffer, NumberOfTicksToWait);
    }

    return eStatus;
}


phRtos_Status_t phRtos_RTOS_SemaphoreGive(
        phRtos_SemaphoreHandle_t semaphoreHandle)
{
    BaseType_t IsWakingUpFrmHigherPrioTsk = pdFALSE;
    phRtos_Status_t eStatus;

    if( phRtos_IsIrqContext() )
    {
        eStatus = (phRtos_Status_t)xSemaphoreGiveFromISR(
                semaphoreHandle, &IsWakingUpFrmHigherPrioTsk);
        portYIELD_FROM_ISR(IsWakingUpFrmHigherPrioTsk);
    }
    else
    {
        eStatus = (phRtos_Status_t)xSemaphoreGive(
                semaphoreHandle);
    }

    return eStatus;
}

phRtos_Status_t phRtos_RTOS_SemaphoreTake(
        phRtos_SemaphoreHandle_t semaphoreHandle, uint32_t dwBlockTime)
{
    BaseType_t IsWakingUpFrmHigherPrioTsk = pdFALSE;
    phRtos_Status_t eStatus;

    if( phRtos_IsIrqContext() )
    {
        eStatus = (phRtos_Status_t)xSemaphoreTakeFromISR(
                semaphoreHandle, &IsWakingUpFrmHigherPrioTsk);
        portYIELD_FROM_ISR(IsWakingUpFrmHigherPrioTsk);
    }
    else
    {
        eStatus = (phRtos_Status_t)xSemaphoreTake(
                semaphoreHandle, dwBlockTime);
    }
    return eStatus;
}

phRtos_Status_t phRtos_RTOS_EventGroupSetBits(
        phRtos_EventHandle_t eventHandle, const uint32_t BitsToSet )
{
    BaseType_t IsWakingUpFrmHigherPrioTsk = pdFALSE;
    phRtos_Status_t eStatus = phRtos_Success;

    if( phRtos_IsIrqContext() )
    {
        eStatus = (phRtos_Status_t)xEventGroupSetBitsFromISR(
                eventHandle, BitsToSet, &IsWakingUpFrmHigherPrioTsk);
        portYIELD_FROM_ISR(IsWakingUpFrmHigherPrioTsk);
    }
    else
    {
        xEventGroupSetBits(eventHandle, BitsToSet);
    }
    return eStatus;
}

void vApplicationTickHook(void) {
#ifdef PHFL_HALAPI_NO_RTOS
    /* Not with RTOS, no way this API gets called */
    while (1) {
       __WFI();
    }
#endif
}

/* **************************************************************************
 * Private Functions
 * ************************************************************************** */

#endif /* PHFL_HALAPI_WITH_RTOS */
