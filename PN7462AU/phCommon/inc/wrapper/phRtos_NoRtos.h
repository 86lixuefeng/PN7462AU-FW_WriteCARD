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
 * phRtos_NoRtos.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-20 12:32:13 +0530 (Wed, 20 Jan 2016) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 17432 $ (v04.13.00)
 */

#ifndef PHRTOS_NORTOS_H_
#define PHRTOS_NORTOS_H_

#ifdef PHFL_HALAPI_WITH_RTOS
#   error "This file should not be included with RTOS"
#endif

#ifndef PHFL_HALAPI_NO_RTOS
#   error "This file should not be included without NO_RTOS"
#endif



/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_RTOS_NORTOS_SYSTICK_CTRL       ( (volatile uint32_t *) 0xe000e010 )
#define PH_RTOS_NORTOS_SYSTICK_CUR_VAL    ( (volatile uint32_t *) 0xe000e018 )

#define PHRTOS_MAX_DELAY     (0xFFFFFFFFFUL)

/* -------------------------------------------------------------------------------------
 * RTOS KERNEL RELATED API's
 * ------------------------------------------------------------------------------------- */

#define phRtos_Start()

#define phRtos_Stop()

#define phRtos_GetState()


/* -------------------------------------------------------------------------------------
 * RTOS TASK RELATED API's
 * ------------------------------------------------------------------------------------- */
#define phRtos_TaskCreate(fpTaskFunction, szTaskName, wStackDepth, pvTaskParameters, dwPriority, pOutTaskHandle)


#define phRtos_TaskGetName(taskHandle)

#define phRtos_TaskSuspend(taskHandle)

#define phRtos_TaskResume(taskHandle)

#define phRtos_TaskDelete(taskHandle)

#define phRtos_TaskYield()

#define phRtos_TaskSetPriority(taskHandle, dwNewPriority)

#define phRtos_TaskGetPriority(taskHandle)

#define phRtos_TaskGetState(taskHandle)

#define phRtos_TaskSuspendAll()

#define phRtos_TaskResumeAll()


/* -------------------------------------------------------------------------------------
 * RTOS QUEUES RELATED API's
 * ------------------------------------------------------------------------------------- */

#define phRtos_QueueCreate(dwQLength, dwItemSize)                    NULL

#define phRtos_QueueSend(queueHandle, pItemToBeQueued, dwBlockTime)  phRtos_NoRtos_ReturnSuccess()

#define phRtos_QueueReceive(queueHandle, pvBuffer, dwBlockTime)    phRtos_NoRtos_ReturnSuccess()

#define phRtos_QueueReset(queueHandle)                             phRtos_NoRtos_ReturnSuccess()

#define phRtos_QueueDelete(queueHandle)


/* -------------------------------------------------------------------------------------
 * RTOS SEMAPHORES RELATED API's
 * ------------------------------------------------------------------------------------- */

#define phRtos_SemaphoreCreate(dwMaxCount, dwInitialCount)        NULL

#define phRtos_SemaphoreGive(semaphoreHandle)                     phRtos_NoRtos_ReturnSuccess()

#define phRtos_SemaphoreTake(semaphoreHandle, dwBlockTime)        phRtos_NoRtos_ReturnSuccess()

#define phRtos_SemaphoreDelete(semaphoreHandle)


/* -------------------------------------------------------------------------------------
 * RTOS MUTEX RELATED API's
 * ------------------------------------------------------------------------------------- */
#define phRtos_MutexCreate()                                       NULL

/**
 * Mutex type semaphores cannot be used from within interrupt service routines.
 */
#define phRtos_MutexTake(mutexHandle, dwBlockTime)                 phRtos_NoRtos_ReturnSuccess()

/**
 * Mutex type semaphores cannot be used from within interrupt service routines.
 */
#define phRtos_MutexGive(mutexHandle)                              phRtos_NoRtos_ReturnSuccess()

#define phRtos_MutexHolder(mutexHandle)                            NULL

#define phRtos_MutexDelete(mutexHandle)


/* -------------------------------------------------------------------------------------
 * RTOS EVENT FLAGS/SIGNALS RELATED API's
 * ------------------------------------------------------------------------------------- */

#define phRtos_EventGroupWaitBits(eventHandle, dwBitsToWait, dwClearOnExit, bWaitForAllBits, dwTicksToWait)     \
    phRtos_NoRtos_EventGroupWaitBits(eventHandle, dwBitsToWait, dwClearOnExit, bWaitForAllBits, dwTicksToWait)

#define phRtos_EventGroupSetBits(eventHandle, dwBitsToSet)    phRtos_NoRtos_EventGroupSetBits(eventHandle, dwBitsToSet)


#define phRtos_EventGroupClearBits(eventHandle, dwBitsToClear)              \
    phRtos_NoRtos_EventGroupClearBits(eventHandle, dwBitsToClear)

#define phRtos_EventGroupGetBits(eventHandle)       ( *((uint32_t *)(eventHandle)) )

#define phRtos_EventGroupDelete(eventHandle)        ( *((uint32_t *)(eventHandle)) = 0)


/* -------------------------------------------------------------------------------------
 * RTOS TIMERS RELATED API's
 * ------------------------------------------------------------------------------------- */

#define phRtos_TimerCreate(pdwTimerID, dwTimePeriodInMilliSecs, bAutoReload, fpCallbackFunction)            \
    phRtos_NoRtos_TimerCreate(dwTimerID, dwTimePeriodInMilliSecs, bAutoReload, fpCallbackFunction )

#define phRtos_TimerGetID(timerHandle)                   phRtos_NoRtos_TimerGetID(timerHandle)

#define phRtos_TimerStart(timerHandle, dwTicksToWait)    phRtos_NoRtos_TimerStart(timerHandle, dwBlockTime)

#define phRtos_TimerStop(timerHandle, dwTicksToWait)     phRtos_NoRtos_TimerStop(timerHandle, dwBlockTime)

#define phRtos_TimerChangePeriod(timerHandle, dwNewPeriod, TicksToWait)                 \
    phRtos_NoRtos_TimerChangePeriod(timerHandle, dwNewPeriod, dwBlockTime)

#define phRtos_TimerReset(timerHandle, dwBlockTime)      phRtos_NoRtos_TimerReset(timerHandle, dwBlockTime)

#define phRtos_TimerDelete(timerHandle, dwTicksToWait)   phRtos_NoRtos_TimerDelete(timerHandle, dwBlockTime)

/* -------------------------------------------------------------------------------------
 * RTOS SYSTICK RELATED API's
 * ------------------------------------------------------------------------------------- */
#define phRtos_SysTickGetCount()    phRtos_NoRtos_SysTickGetCount()

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
extern void * phRtos_NoRtos_EventGroupCreate(void);
extern uint32_t phRtos_NoRtos_EventGroupClearBits(void * eventHandle, const uint32_t dwBitsToClear);
extern uint32_t phRtos_NoRtos_EventGroupWaitBits(volatile void * volatile eventHandle, const uint32_t dwBitsToWait,
    const bool bClearOnExit, const bool bWaitForAllBits, uint32_t dwBlockTime);
extern uint32_t phRtos_NoRtos_SysTickGetCount(void);
extern phRtos_Status_t phRtos_NoRtos_EventGroupSetBits(void * eventHandle, const uint32_t dwBitsToSet);
extern phRtos_Status_t phRtos_NoRtos_ReturnSuccess(void);
extern phRtos_TimerHandle_t phRtos_NoRtos_TimerCreate(uint32_t dwTimerID, uint32_t dwTimePeriodInMilliSecs, bool bAutoReload,
    pphRtos_TimerCallback_t fpCallbackFunction);
extern uint32_t phRtos_NoRtos_TimerGetID(phRtos_TimerHandle_t timerHandle);
extern phRtos_Status_t phRtos_NoRtos_TimerStart(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);
extern phRtos_Status_t phRtos_NoRtos_TimerStop(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);
extern  phRtos_Status_t phRtos_NoRtos_TimerChangePeriod(phRtos_TimerHandle_t timerHandle, uint32_t dwNewPeriod,
    uint32_t dwBlockTime);
extern phRtos_Status_t phRtos_NoRtos_TimerReset(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);
extern phRtos_Status_t phRtos_NoRtos_TimerDelete(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);
extern void phRtos_NoRtos_SysTickHandler(void);

#endif /* PHRTOS_NORTOS_H_ */
