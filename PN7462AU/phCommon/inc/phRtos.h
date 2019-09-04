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
 * Generic RTOS wrapper component
 *
 * phRtos.h:  Exposes RTOS functionality prototypes.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-20 11:36:03 +0530 (Wed, 20 Jan 2016) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 17427 $ (v04.13.00)
 */

#ifndef PHRTOS_H
#define PHRTOS_H

/** \addtogroup phRtos
 *
 * The RTOS Abstraction APIs.
 *
 *  @{
 */

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/**
 * USER CONFIGURABLE MACROS
 */
#define PH_RTOS_MESSAG_QUEUE     1      /**< Message Queues:  1=available, 0=not available */
#define PH_RTOS_SEMAPHORE        1      /**< Semaphore/Mutex:  1=available, 0=not available */
#define PH_RTOS_EVENTS           1      /**< Event Flags/Signals:  1=available, 0=not available */
#define PH_RTOS_TIMER            1      /**< Software Timers: 1=available, 0=not available */
#define PH_RTOS_SYSTICK          1      /**< SysTick features: 1=available, 0=not available */


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * RTOS task/thread handler.
 */
typedef void * phRtos_TaskHandle_t;
/**
 * RTOS Queue handler.
 */
typedef void * phRtos_QueueHandle_t;
/**
 * RTOS Semaphore handler.
 */
typedef void * phRtos_SemaphoreHandle_t;
/**
 * RTOS Mutex handler.
 */
typedef void * phRtos_MutexHandle_t;
/**
 * RTOS Event group handler.
 */
typedef void * phRtos_EventHandle_t;
/**
 * RTOS Timer handler.
 */
typedef void * phRtos_TimerHandle_t;
/**
 * RTOS Task signature.
 */
typedef void (*pphRtos_Task_t) (void *pvArguments);
/**
 * RTOS Timer Callback handler prototype.
 */
typedef void (*pphRtos_TimerCallback_t)(phRtos_TimerHandle_t timerHandle);

/**
 * RTOS Task priorities.
 * @note This is just for reference, priorities is not limited to 7. User can add/remove priorities.
 */
typedef enum  {
  priorityIdle          = 0,          /**< priority: idle (lowest) */
  priorityLow           = 1,          /**< priority: low */
  priorityBelowNormal   = 2,          /**< priority: below normal */
  priorityNormal        = 3,          /**< priority: normal (default) */
  priorityAboveNormal   = 4,          /**< priority: above normal */
  priorityHigh          = 5,          /**< priority: high */
  priorityBelowRealtime = 6,          /**< priority: below realtime (higher) */
  priorityRealtime      = 7           /**< priority: realtime (highest) */
} phRtos_Priority_t;

/**
 * Indicates the current state of the Task.
 */
typedef enum
{
    eTaskRunning = 0,   /**< A task is querying the state of itself, so must be running. */
    eTaskReady,         /**< The task being queried is in a read or pending ready list. */
    eTaskBlocked,       /**< The task being queried is in the Blocked state. */
    eTaskSuspended,     /**< Task being queried is in Suspended OR in Blocked state with an infinite time out. */
    eTaskDeleted        /**< The task being queried has been deleted. */
} phRtos_TaskState_t;

/**
 * Indicates the status of the RTOS operations.
 */
typedef enum{
    phRtos_Failure = 0, /**< Indicates the failure status */
    phRtos_Success = 1  /**< Indicates the success status*/
}phRtos_Status_t;

/**
 * Indicates the state of the RTOS kernel.
 */
typedef enum
{
    eKernelSuspended = 0,   /**< RTOS is suspended. */
    eKernelNotStarted,      /**< RTOS is not started.*/
    eKernelRunning         /**< RTOS is started and running. */
} phRtos_KernelState_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/* -------------------------------------------------------------------------------------
 * RTOS KERNEL RELATED API's
 * ------------------------------------------------------------------------------------- */

/**
 * Initialize the RTOS Kernel for creating objects.
 * @note Rtos initialize API must be called before using any Rtos API's.
 *
 * @return Status of the operation
 * @retval phRtos_Success If the operation completed successfully.
 * @retval phRtos_Failure If the operation uncompleted OR ended up with errors.
 */
extern phRtos_Status_t phRtos_Initialize(void);

/**
 * Starts the real time kernel.
 */
extern void phRtos_Start(void);

/**
 * Stops the real time kernel.  All the tasks will be deleted and execution then resumes from the point
 * where phRtos_Start() was called, as if phRtos_Start() had just returned.
 */
extern void phRtos_Stop(void);

/**
 * Get the RTOS kernel state.
 *
 * @return phRtos_KernelState_t.
 * @retval eKernelNotStarted RTOS is not started.
 * @retval eKernelRunning RTOS is started and running.
 * @retval eKernelSuspended RTOS is suspended.
 */
extern phRtos_KernelState_t phRtos_GetState(void);


/* -------------------------------------------------------------------------------------
 * RTOS TASK RELATED API's
 * ------------------------------------------------------------------------------------- */

/**
 * Create a Task and add it to Active Tasks and set it to state READY.
 *
 * @param[in] fpTaskFunction Pointer to the task entry function.
 * @param[in] szTaskName A descriptive name for the task.
 * @param[in] wStackDepth The size of the task stack in double words (4 bytes).
 * @param[in] pvTaskParameters Pointer that will be used as parameter for the task being created.
 * @param[in] dwPriority The priority at which the task should run.
 * @param[out] pOutTaskHandle Task handle by which the created task can be referenced.
 *
 * @return Status of the operation.
 * @retval phRtos_Success If the operation completed successfully.
 * @retval phRtos_Failure If the operation uncompleted OR ended up with errors.
 *
 */
extern phRtos_Status_t phRtos_TaskCreate(
    pphRtos_Task_t fpTaskFunction,
    const char * const szTaskName,
    uint16_t wStackDepth,
    void *pvTaskParameters,
    uint32_t dwPriority,
    phRtos_TaskHandle_t * pOutTaskHandle);

/**
 * Get Task name.
 *
 * @param[in] taskHandle Handle to the task to get name.
 * @return The text name of the task referenced by the handle taskHandle.
 */
extern char *phRtos_TaskGetName(phRtos_TaskHandle_t taskHandle);

/**
 * Suspends the specified task.
 *
 * @param[in] taskHandle Handle to the task to be suspended.
 */
extern void phRtos_TaskSuspend(phRtos_TaskHandle_t taskHandle);

/**
 * Resume the specified task.
 *
 * @param[in] taskHandle Handle to the task being readied.
 */
extern void phRtos_TaskResume(phRtos_TaskHandle_t taskHandle);

/**
 * Terminate the specified task. Remove task from the RTOS real time kernel's management.
 *
 * @param[in] taskHandle Handle to the task to get terminate.
 */
extern void phRtos_TaskDelete(phRtos_TaskHandle_t taskHandle);

/**
 * Perform context switch, pass control to the task that is in READY state.
 * @return void.
 */
extern void phRtos_TaskYield(void);

/**
 * Set specified priority to the task.
 *
 * @param[in] taskHandle Handle to the task to force the New priority.
 * @param[in] dwNewPriority The priority to which the task will be set.
 */
extern void phRtos_TaskSetPriority(phRtos_TaskHandle_t taskHandle, uint32_t dwNewPriority);

/**
 * Get the priority of the specified task.
 *
 * @param[in] taskHandle Handle to the task to be queried.
 * @return Priority of the task.
 */
extern uint32_t phRtos_TaskGetPriority(phRtos_TaskHandle_t taskHandle);

/**
 * Delay calling task for a given number of ticks. Actual time that the task remains blocked depends on the tick rate.
 *
 * @param[in] dwTicksToDelay The amount of time, in tick periods, that the calling task should block.
 */
extern void phRtos_TaskDelay(const uint32_t dwTicksToDelay);

/**
 * Obtain the state of the specified task.
 *
 * @param[in] taskHandle Handle to the task to be queried.
 *
 * @return State of the task.
 * @retval eTaskRunning A task is querying the state of itself, so must be running.
 * @retval eTaskReady The task being queried is in a read or pending ready list.
 * @retval eTaskBlocked The task being queried is in the Blocked state.
 * @retval eTaskSuspended Task being queried is in Suspended OR in Blocked state with an infinite time out.
 * @retval eTaskDeleted The task being queried has been deleted.
 */
extern phRtos_TaskState_t phRtos_TaskGetState(phRtos_TaskHandle_t taskHandle);

/**
 * Suspends the scheduler, context switches will not occur while the scheduler is suspended.
 */
extern void phRtos_TaskSuspendAll(void);

/**
 * Resumes scheduler activity after it was suspended by a call to phRtos_TaskSuspendAll().
 */
extern void phRtos_TaskResumeAll(void);

/**
 * Get the free available stack for the specified task.
 *
 * @param[in] taskHandle Handle to the task to be queried.
 * @return Available free stack in double words (4 bytes).
 */
extern uint32_t phRtos_TaskGetStackWaterMark(phRtos_TaskHandle_t taskHandle);

/* -------------------------------------------------------------------------------------
 * RTOS QUEUES RELATED API's
 * ------------------------------------------------------------------------------------- */
#if (PH_RTOS_MESSAG_QUEUE == 1)

/**
 * Creates a new queue instance and returns a handle for the queue.
 *
 * @param[in] dwQLength The maximum number of items that the queue can contain.
 * @param[in] dwItemSize The number of bytes each item in the queue will require.
 * @return If successfully then a handle to the newly created queue is returned else NULL is returned.
 */
extern phRtos_QueueHandle_t phRtos_QueueCreate(const uint32_t dwQLength, const uint32_t dwItemSize);

/**
 * Post an item on the specified queue.
 *
 * @param[in] queueHandle Handle to the queue on which the item is to be posted.
 * @param[in] pItemToBeQueued A pointer to the item that is to be placed on the queue.
 * @param[in] dwBlockTime The maximum amount of time in ticks the task should block to place item on the queue.
 * @return Status of the operation.
 * @retval phRtos_Success If the operation completed successfully.
 * @retval phRtos_Failure If the operation uncompleted OR ended up with errors.
 */
extern phRtos_Status_t phRtos_QueueSend(
    phRtos_QueueHandle_t queueHandle,
    const void * const pItemToBeQueued,
    uint32_t dwBlockTime);

/**
 * Receive an item from a queue.
 *
 * @param[in] queueHandle Handle to the queue from which the item is to be received.
 * @param[in] pvBuffer Pointer to the buffer into which the received item will be copied.
 * @param[in] dwBlockTime The maximum amount of time in ticks the task should block waiting for an item to receive.
 * @return Status of the operation.
 * @retval phRtos_Success If an item was successfully received from the queue.
 * @retval phRtos_Failure If the operation timed out OR ended up with errors.
 */
extern phRtos_Status_t phRtos_QueueReceive(phRtos_QueueHandle_t queueHandle,
    void * const pvBuffer, uint32_t dwBlockTime);

/**
 * Reset a queue back to its original empty state.
 *
 * @param[in] queueHandle Handle to the queue to be reset.
 * @return Status of the operation.
 * @retval phRtos_Success If the queue is successfully reset.
 * @retval phRtos_Failure If the queue could not be reset.
 */
extern phRtos_Status_t phRtos_QueueReset(phRtos_QueueHandle_t queueHandle);

/**
 * Delete the specified Queue.
 *
 * @param[in] queueHandle Handle to the queue to be deleted.
 */
extern void phRtos_QueueDelete(phRtos_QueueHandle_t queueHandle);

#endif /* PH_RTOS_MESSAG_QUEUE */


/* -------------------------------------------------------------------------------------
 * RTOS SEMAPHORES RELATED API's
 * ------------------------------------------------------------------------------------- */
#if (PH_RTOS_SEMAPHORE == 1)

/**
 * Create and Initialize a Semaphore object used for managing resources.
 *
 * @param[in] uxMaxCount The maximum count value that can be reached.
 * @param[in] uxInitialCount The count value assigned to the semaphore when it is created.
 * @return Handle to the created semaphore. Null if the semaphore could not be created.
 */
extern phRtos_SemaphoreHandle_t phRtos_SemaphoreCreate(const uint32_t uxMaxCount, const uint32_t uxInitialCount);

/**
 * Release a Semaphore token.
 *
 * @param[in] semaphoreHandle Handle to the semaphore being released.
 * @return Status of the operation.
 * @retval phRtos_Success If the semaphore was released.
 * @retval phRtos_Failure If an error occurred.
 */
extern phRtos_Status_t phRtos_SemaphoreGive(phRtos_SemaphoreHandle_t semaphoreHandle);

/**
 * To obtain a semaphore.
 *
 * @param[in] semaphoreHandle Handle to the semaphore being taken.
 * @param[in] dwBlockTime The time in ticks to wait for the semaphore to become available.
 * @return Status of the operation.
 * @retval phRtos_Success If the semaphore was obtained.
 * @retval phRtos_Failure If dwBlockTime expired without the semaphore becoming available.
 */
extern phRtos_Status_t phRtos_SemaphoreTake(phRtos_SemaphoreHandle_t semaphoreHandle, uint32_t dwBlockTime);

/**
 * Delete a semaphore.
 *
 * @param[in] semaphoreHandle Handle to the semaphore to be deleted.
 */
extern void phRtos_SemaphoreDelete(phRtos_SemaphoreHandle_t semaphoreHandle);


/* -------------------------------------------------------------------------------------
 * RTOS MUTEX RELATED API's
 * ------------------------------------------------------------------------------------- */
/**
 * Create and Initialize a Mutex object.
 *
 * @return Handle to the created Mutex. Null if the Mutex could not be created.
 */
extern phRtos_MutexHandle_t phRtos_MutexCreate(void);

/**
 * To obtain a Mutex.
 *
 * @param[in] mutexHandle Handle to the Mutex being taken.
 * @param[in] dwBlockTime The time in ticks to wait for the Mutex to become available.
 * @return Status of the operation.
 * @retval phRtos_Success If the Mutex was obtained.
 * @retval phRtos_Failure If dwBlockTime expired without the Mutex becoming available.
 */
extern phRtos_Status_t phRtos_MutexTake(phRtos_MutexHandle_t mutexHandle, uint32_t dwBlockTime);

/**
 * Release a acquired Mutex.
 *
 * @param[in] mutexHandle Handle to the Mutex being released.
 * @return Status of the operation.
 * @retval phRtos_Success If the mutexHandle was released.
 * @retval phRtos_Failure If an error occurred.
 */
extern phRtos_Status_t phRtos_MutexGive(phRtos_MutexHandle_t mutexHandle);

/**
 * Get the Mutex holder.
 *
 * @param[in] mutexHandle Handle to the Mutex to be queried.
 * @return Returns the current mutex holder. If mutexHandle is not held by a task, return NULL.
 */
extern phRtos_TaskHandle_t phRtos_MutexHolder(phRtos_MutexHandle_t mutexHandle);

/**
 * Delete a Mutex.
 *
 * @param[in] mutexHandle Handle to the Mutex to be deleted.
 */
extern void phRtos_MutexDelete(phRtos_MutexHandle_t mutexHandle);

#endif /* PH_RTOS_SEMAPHORE */


/* -------------------------------------------------------------------------------------
 * RTOS EVENT FLAGS/SIGNALS RELATED API's
 * ------------------------------------------------------------------------------------- */
#if (PH_RTOS_EVENTS == 1)

/**
 * Create a new event group.
 * @note Only first 24 bits can be used for event signaling usage.
 *
 * @return Returns a handle to the event group created else NULL.
 */
extern phRtos_EventHandle_t phRtos_EventGroupCreate(void);

/**
 * Wait for one or more Events/Signals Flags to become signaled for the specified event group.
 *
 * @param[in] eventHandle The event group in which the bits are being tested.
 * @param[in] dwBitsToWait A bitwise value that indicates the bit or bits to test inside the event group.
 * @param[in] bClearOnExit If true then any bits within uxBitsToWait that are set will be cleared. If false then the
 *                     bits set in the event group are not altered.
 * @param[in] bWaitForAllBits If true then wait until all specified event flags set else for any single flag.
 * @param[in] dwBlockTime The time in ticks to wait for one/all (depending on the xWaitForAllBits value) of the bits
 *                    specified by uxBitsToWaitFor to become set.
 * @return Returns the value of the event group at the time either the bits being waited for became set,
 *         or the block time expired.
 */
extern uint32_t phRtos_EventGroupWaitBits(phRtos_EventHandle_t eventHandle, const uint32_t dwBitsToWait,
    const bool bClearOnExit, const bool bWaitForAllBits, uint32_t dwBlockTime);

/**
 * Set the specified bits within an event group.
 * @note Only first 24 bits can set or cleared, upper 8-bits are used for internal usage.
 *
 * @param eventHandle The event group in which the bits are to be set.
 * @param dwBitsToSet A bitwise value that indicates the bit or bits to set.
 *
 * @return Status of the operation
 * @retval phRtos_Success If the operation completed successfully.
 * @retval phRtos_Failure If the operation uncompleted OR ended up with errors.
 */
extern phRtos_Status_t phRtos_EventGroupSetBits(phRtos_EventHandle_t eventHandle, const uint32_t dwBitsToSet);

/**
 * Clear the specified bits within an event group.
 * @note Only first 24 bits can set or cleared, upper 8-bits are used for internal usage.
 * @param eventHandle The event group in which the bits are to be cleared.
 * @param dwBitsToClear A bitwise value that indicates the bit or bits to clear.
 * @return The value of the event group before the specified bits were cleared.
 */
extern uint32_t phRtos_EventGroupClearBits(phRtos_EventHandle_t eventHandle, const uint32_t dwBitsToClear);

/**
 * Returns the current value of the bits in an event group.
 *
 * @param eventHandle The event group being queried.
 * @return The event group bits at the time phRtos_EventGroupGetBits() was called.
 */
extern uint32_t phRtos_EventGroupGetBits(phRtos_EventHandle_t eventHandle);

/**
 * Delete specified event group
 *
 * @param eventHandle The event group being deleted.
 */
void phRtos_EventGroupDelete(phRtos_EventHandle_t eventHandle);

#endif /* PH_RTOS_EVENTS */


/* -------------------------------------------------------------------------------------
 * RTOS TIMERS RELATED API's
 * ------------------------------------------------------------------------------------- */
#if (PH_RTOS_TIMER == 1)
/**
 * Creates a new timer instance. Returns a handle by which the new timer can be referenced.
 *
 * @param dwTimerID : User assigned timer Id.
 * @param dwTimePeriodInTicks : The timer period in Ticks.
 * @param bAutoReload : If set to pdTRUE then the timer will expire repeatedly with a frequency set by
 * the xTimerPeriodInTicks parameter. If uxAutoReload is set to pdFALSE then the timer will be a one-shot timer.
 * @param fpCallbackFunction : The function to call when the timer expires.
 * @return If the timer is successfully created then a handle to the newly created timer is returned. If the timer
 * cannot be created then NULL is returned.
 */
extern phRtos_TimerHandle_t phRtos_TimerCreate(uint32_t dwTimerID, uint32_t dwTimePeriodInTicks, bool bAutoReload,
    pphRtos_TimerCallback_t fpCallbackFunction);
/**
 * Returns the ID assigned to the timer.
 * @param timerHandle : The timer being queried.
 * @return The ID assigned to the timer being queried.
 */
extern uint32_t phRtos_TimerGetID(phRtos_TimerHandle_t timerHandle);

/**
 * starts a timer that was previously created using the phRtos_TimerCreate() API function.
 *
 * @param timerHandle : The handle of the timer being started.
 * @param dwBlockTime : Specifies the time in ticks that the calling task should be held in the Blocked state to wait
 * for the start command to be successful.
 * @return phRtos_Failure will be returned if the start command could not be success even after dwBlockTime ticks
 * had passed. phRtos_Success will be returned if the command is successful.
 */
extern phRtos_Status_t phRtos_TimerStart(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);

/**
 * Stops a timer that was previously started using phRtos_TimerStart().
 *
 * @param timerHandle : The handle of the timer being stopped.
 * @param dwBlockTime : Specifies the time in ticks that the calling task should be held in the Blocked state to wait
 * for the stop command to be successful.
 * @return phRtos_Failure will be returned if the stop command could not be success even after dwBlockTime ticks
 * had passed. phRtos_Success will be returned if the command is successful.
 */
extern phRtos_Status_t phRtos_TimerStop(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);

/**
 * Changes the period of a timer that was previously created using the phRtos_TimerCreate() API function.
 *
 * @param timerHandle : The handle of the timer that is having its period changed.
 * @param dwNewPeriod : The new period for timerHandle.
 * @param dwBlockTime : Specifies the time in ticks that the calling task should be held in the Blocked state to wait
 * for the change period command to be successful.
 * @return phRtos_Failure will be returned if the change period command could not be success even after dwBlockTime
 * ticks had passed. phRtos_Success will be returned if the command is successful.
 */
extern phRtos_Status_t phRtos_TimerChangePeriod(phRtos_TimerHandle_t timerHandle, uint32_t dwNewPeriod,
    uint32_t dwBlockTime);

/**
 * Re-starts a timer that was previously created using the phRtos_TimerCreate() API function.
 *
 * @param timerHandle : The handle of the timer being reset/restarted.
 * @param dwBlockTime : Specifies the time in ticks that the calling task should be held in the Blocked state
 * to wait for the reset command to be successful.
 * @return phRtos_Failure will be returned if the reset command could not be success even after dwBlockTime
 * ticks had passed. phRtos_Success will be returned if the command is successful.
 */
extern phRtos_Status_t phRtos_TimerReset(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);

/**
 * Deletes a timer that was previously created using the phRtos_TimerCreate() API function.
 *
 * @param timerHandle : The handle of the timer being deleted.
 * @param dwBlockTime : Specifies the time in ticks that the calling task should be held in the Blocked state
 * to wait for the delete command to be successful.
 * @return phRtos_Failure will be returned if the delete command could not be success even after dwBlockTime
 * ticks had passed. phRtos_Success will be returned if the command is successful.
 */
extern phRtos_Status_t phRtos_TimerDelete(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime);

#endif /* PH_RTOS_TIMER */


/* -------------------------------------------------------------------------------------
 * RTOS SYSTICK RELATED API's
 * ------------------------------------------------------------------------------------- */
#if (PH_RTOS_SYSTICK == 1)

/**
 * Returns the Systick count.
 *
 * @return The count of ticks since phRtos_Start was called.
 */
extern uint32_t phRtos_SysTickGetCount(void);

#endif /* PH_RTOS_SYSTICK */


/**
 * RTOS wrapper.
 */

#ifdef PHFL_HALAPI_WITH_RTOS
#   include "wrapper/phRtos_RTOS.h"
#endif /* PHFL_HALAPI_WITH_RTOS */

#ifdef PHFL_HALAPI_NO_RTOS
#   include "wrapper/phRtos_NoRtos.h"
#endif /* PHFL_HALAPI_NO_RTOS */

#ifndef PHFL_HALAPI_WITH_RTOS
#   ifndef PHFL_HALAPI_NO_RTOS
#       error "Either of PHFL_HALAPI_WITH_RTOS or PHFL_HALAPI_NO_RTOS should be defined"
#   endif
#endif

#ifdef PHFL_HALAPI_WITH_RTOS
#   ifdef PHFL_HALAPI_NO_RTOS
#       error "Both of PHFL_HALAPI_WITH_RTOS and PHFL_HALAPI_NO_RTOS should not be defined"
#   endif
#endif

/** @} */

#endif /* PHRTOS_H */
