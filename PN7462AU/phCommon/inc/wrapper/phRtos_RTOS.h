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
 * phRtos_RTOS.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-26 11:34:37 +0530 (Wed, 26 Aug 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14749 $ (v04.13.00)
 */

#ifndef phRtos_RTOS_H_
#define phRtos_RTOS_H_

#ifndef PHFL_HALAPI_WITH_RTOS
#   error "This file should not be included without RTOS"
#endif

#ifdef PHFL_HALAPI_NO_RTOS
#   error "This file should not be included with NO_RTOS"
#endif

/* RTOS_PORTING_DISCLAIMER */

/* **************************************************************************
 *   Includes
 * ************************************************************************** */
#include "ph_Datatypes.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "portmacro.h"
#include "timers.h"

/* **************************************************************************
 * MACROS/Defines
 * ************************************************************************** */

#define PHRTOS_MAX_DELAY            portMAX_DELAY

/*Interrupt Control and State Register*/
#define PHRTOS_NVIC_INT_CTRL        ( (volatile uint32_t *) 0xe000ed04 )
#define PHRTOS_NVIC_SYSTICK_CUR_VAL ( (volatile uint32_t *) 0xe000e018 )
#define PHRTOS_NVIC_VECTACTIVE      0x0000003FU
#define phRtos_IsIrqContext()                                           \
    (*(PHRTOS_NVIC_INT_CTRL) & PHRTOS_NVIC_VECTACTIVE)

/* **************************************************************************
 * RTOS KERNEL RELATED API's
 * ************************************************************************** */
#define phRtos_Initialize()

#define phRtos_Start()    (void)vTaskStartScheduler()

#define phRtos_Stop()     (void)vTaskEndScheduler()

#define phRtos_GetState() xTaskGetSchedulerState()


/* **************************************************************************
 * RTOS TASK RELATED API's
 * ************************************************************************** */
#define phRtos_TaskCreate(fpTaskFunction,                               \
    szNameOfTask,                                                       \
    StackDepth,                                                         \
    TaskParameters,                                                     \
    TaskPriority,                                                       \
    outTaskHandle)                                                      \
    (phRtos_Status_t)xTaskCreate(                           \
        fpTaskFunction,                                                 \
        szNameOfTask,                                                   \
        StackDepth,                                                     \
        TaskParameters,                                                 \
        TaskPriority,                                                   \
        outTaskHandle)

#define phRtos_TaskGetName(taskHandle)        \
    pcTaskGetTaskName(taskHandle)

#define phRtos_TaskSuspend(taskHandle)        \
    vTaskSuspend(taskHandle)

#define phRtos_TaskResume(taskHandle)                                   \
    ((phRtos_IsIrqContext() != 0 )                                      \
     ?                                                                  \
     portYIELD_FROM_ISR(                                       \
         (phRtos_Status_t)xTaskResumeFromISR(              \
             (TaskHandle_t)(taskHandle)))              \
     :                                                                  \
     vTaskResume((TaskHandle_t)(taskHandle)))


#define phRtos_TaskDelete(taskHandle)           \
    vTaskDelete(taskHandle)

#define phRtos_TaskYield()                      \
    taskYIELD()

#define phRtos_TaskSetPriority(taskHandle, dwNewPriority) \
    vTaskPrioritySet(taskHandle, dwNewPriority)

#define phRtos_TaskGetPriority(taskHandle)      \
    (uint32_t)uxTaskPriorityGet(taskHandle)

#define phRtos_TaskDelay(dwTicksToDelay)        \
    vTaskDelay(dwTicksToDelay)

#define phRtos_TaskGetState(taskHandle)         \
    eTaskGetState(taskHandle)

#define phRtos_TaskSuspendAll()                 \
    vTaskSuspendAll()

#define phRtos_TaskResumeAll()                  \
    (void)xTaskResumeAll()

#define phRtos_TaskGetStackWaterMark(taskHandle)     \
    (sizeof(portSTACK_TYPE) * uxTaskGetStackHighWaterMark(taskHandle))


/* **************************************************************************
 * RTOS QUEUES RELATED API's
 * ************************************************************************** */

#define phRtos_QueueCreate(                     \
    dwQLength, dwItemSize)                      \
    xQueueCreate(                   \
        dwQLength, dwItemSize)

#define phRtos_QueueSend(                       \
    queueHandle, pItemToBeQueued, dwBlockTime)         \
    phRtos_RTOS_QueueSend(                      \
        queueHandle, pItemToBeQueued, dwBlockTime)

#define phRtos_QueueReceive(                    \
    queueHandle, pvBuffer, dwBlockTime)              \
    phRtos_RTOS_QueueReceive(                   \
        queueHandle, pvBuffer, dwBlockTime)

#define phRtos_QueueReset(queueHandle)               \
    xQueueReset(queueHandle)

#define phRtos_QueueDelete(queueHandle)              \
    vQueueDelete(queueHandle)


/* **************************************************************************
 * RTOS SEMAPHORES RELATED API's
 * ************************************************************************** */

#define phRtos_SemaphoreCreate(dwMaxCount, dwInitialCount) \
    xSemaphoreCreateCounting(dwMaxCount, dwInitialCount)

#define phRtos_SemaphoreGive(semaphoreHandle)        \
    phRtos_RTOS_SemaphoreGive(semaphoreHandle)

#define phRtos_SemaphoreTake(semaphoreHandle, dwBlockTime) \
    phRtos_RTOS_SemaphoreTake(semaphoreHandle, dwBlockTime)

#define phRtos_SemaphoreDelete(semaphoreHandle)      \
    vSemaphoreDelete(semaphoreHandle)


/* **************************************************************************
 * RTOS MUTEX RELATED API's
 * ************************************************************************** */
#define phRtos_MutexCreate()                         \
    xSemaphoreCreateMutex()

/**
 * Mutex type semaphores cannot be used from within interrupt service routines.
 */
#define phRtos_MutexTake(mutexHandle, dwBlockTime)   \
    xSemaphoreTake(mutexHandle, dwBlockTime)

/**
 * Mutex type semaphores cannot be used from within interrupt service routines.
 */
#define phRtos_MutexGive(mutexHandle)                \
    xSemaphoreGive(mutexHandle)

#define phRtos_MutexHolder(mutexHandle)              \
    xSemaphoreGetMutexHolder(mutexHandle)

#define phRtos_MutexDelete(mutexHandle)              \
    vSemaphoreDelete(mutexHandle)


/* **************************************************************************
 * RTOS EVENT FLAGS/SIGNALS RELATED API's
 * ************************************************************************** */

/**
 * This function cannot be called from an interrupt service routines.
 */
#define phRtos_EventGroupWaitBits(                                      \
    eventHandle,                                                        \
        dwBitsToWait,                                                   \
        dwClearOnExit,                                                  \
        bWaitForAllBits,                                                \
    dwTicksToWait)                                                      \
    xEventGroupWaitBits(                                         \
        eventHandle,                                                    \
        dwBitsToWait,                                                   \
        dwClearOnExit,                                                  \
        bWaitForAllBits,                                                \
        dwTicksToWait)

#define phRtos_EventGroupSetBits(                           \
    eventHandle,                                            \
    dwBitsToSet)                                            \
    phRtos_RTOS_EventGroupSetBits(                          \
        eventHandle,                                        \
        dwBitsToSet)

#define phRtos_EventGroupClearBits(                                     \
    eventHandle,                                                        \
    dwBitsToClear)                                                      \
    ((uint32_t)                                                         \
     ((phRtos_IsIrqContext() != 0 )                                     \
      ?                                                                 \
      (uint32_t)xEventGroupClearBitsFromISR(                     \
          (eventHandle),                                                \
          (dwBitsToClear))                                              \
      : (uint32_t)xEventGroupClearBits(                          \
          (eventHandle),                                                \
          (dwBitsToClear))))

#define phRtos_EventGroupGetBits(eventHandle)   \
    phRtos_EventGroupClearBits(                 \
        eventHandle,                            \
        0)

#define phRtos_EventGroupDelete(eventHandle)    \
    vEventGroupDelete(eventHandle)


/* **************************************************************************
 * RTOS TIMERS RELATED API's
 * ************************************************************************** */

/**
 * No usecase of software timers in example apps since PN7462AU has 4 general purpose timers.
 */
#define phRtos_TimerCreate(                     \
    dwTimerID,                                  \
    dwTimePeriodInTicks,                        \
    bAutoReload,                                \
    fpCallbackFunction)                         \
    xTimerCreate(                   \
        "",                                     \
        dwTimePeriodInTicks,                    \
        bAutoReload,                            \
        (void *)(dwTimerID),                    \
        fpCallbackFunction )

#define phRtos_TimerGetID(timerHandle)          \
    (uint32_t) pvTimerGetTimerID(timerHandle)

#define phRtos_TimerStart(                      \
    timerHandle,                                \
    dwBlockTime)                                \
    xTimerStart(                    \
        timerHandle,                            \
        dwBlockTime )

#define phRtos_TimerStop(                       \
    timerHandle,                                \
    dwBlockTime)                                \
    xTimerStop(                     \
        timerHandle,                            \
        dwBlockTime )

#define phRtos_TimerChangePeriod(               \
    timerHandle,                                \
    dwNewPeriod,                                \
    dwBlockTime)                                \
    xTimerChangePeriod(           \
        timerHandle,                            \
        dwNewPeriod,                            \
        dwBlockTime )

#define phRtos_TimerReset(                      \
    timerHandle,                                \
    dwBlockTime)                                \
    xTimerReset(                    \
        timerHandle,                            \
        dwBlockTime )

#define phRtos_TimerDelete(                     \
    timerHandle,                                \
    dwBlockTime)                                \
    xTimerDelete(                   \
        timerHandle,                            \
        dwBlockTime )


/* **************************************************************************
 * RTOS SYSTICK RELATED API's
 * ************************************************************************** */

#define phRtos_SysTickGetCount()                                        \
    ((phRtos_IsIrqContext() != 0 )                                      \
     ?                                                                  \
     xTaskGetTickCountFromISR()                                \
     :                                                                  \
     xTaskGetTickCount())

/* **************************************************************************
 * Types/Structure Declarations
 * ************************************************************************** */

/* **************************************************************************
 *   Extern Variables
 * ************************************************************************** */

/* **************************************************************************
 *   Function Prototypes
 * ************************************************************************** */

extern phRtos_Status_t phRtos_RTOS_QueueSend(
    phRtos_QueueHandle_t queueHandle,
    const void * pItemToBeQueued,
    uint32_t TicksToWait);
phRtos_Status_t phRtos_RTOS_QueueReceive(
    phRtos_QueueHandle_t queueHandle,
    void *pvBuffer,
    uint32_t TicksToWait);
extern phRtos_Status_t phRtos_RTOS_SemaphoreGive(
    phRtos_SemaphoreHandle_t semaphoreHandle);
extern phRtos_Status_t phRtos_RTOS_SemaphoreTake(
    phRtos_SemaphoreHandle_t semaphoreHandle,
    uint32_t dwBlockTime);
extern phRtos_Status_t phRtos_RTOS_EventGroupSetBits(
    phRtos_EventHandle_t eventHandle,
    const uint32_t BitsToSet);
extern void phRtos_DisableSysTickTimer(void);
extern void phRtos_EnableSysTickTimer(void);
extern void phRtos_ClearPendSvInterrupt(void);
extern void vApplicationMallocFailedHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationStackOverflowHook(
    TaskHandle_t pxTask,
    char *pcTaskName);

#endif /* phRtos_RTOS_H_ */
