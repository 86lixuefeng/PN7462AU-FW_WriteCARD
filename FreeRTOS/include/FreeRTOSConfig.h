/*
    FreeRTOS V6.0.0 - Copyright (C) 2009 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/


/******************************************************************************
    See http://www.freertos.org/a00110.html for an explanation of the
    definitions contained in this file.
******************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(PHFL_APP_NXBUILD_CONFIG) || defined(NXPBUILD_CUSTOMER_HEADER_INCLUDED)
/* if compiled as a sub folder for Application projects */
#   include <ph_NxpBuild.h>
#endif

#ifdef __GNUC__
#  include <stdint.h>
#endif

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *----------------------------------------------------------*/

#if defined(__GNUC__) || defined (__ARMCC_VERSION) || defined (__ICCARM__)
/* In case some assembler gets smart to include this file, it would not
 * complain about extern */
extern uint32_t SystemCoreClock;
#endif


// Added below defines as it was required if Timers are enabled

#define configUSE_PREEMPTION        1
#define configUSE_IDLE_HOOK         0
#define configMAX_PRIORITIES        ( 6 )
#define configUSE_TICK_HOOK         0
#define configCPU_CLOCK_HZ          ( ( unsigned long ) SystemCoreClock )
#define configTICK_RATE_HZ          ( ( portTickType ) 1000 )
#define configMINIMAL_STACK_SIZE    ( ( unsigned short ) 100 )
#define configSUPPORT_STATIC_ALLOCATION 0
#if defined (__PN74XXXX__) || defined (__PN73XXXX__)
#   if ( configSUPPORT_STATIC_ALLOCATION == 1 )
#      error "Not supported for PN7462AU"
#   else
#       ifndef configTOTAL_HEAP_SIZE
#      		define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 5 * 1024 ) )
#       endif
#   endif
#else
#   if ( configSUPPORT_STATIC_ALLOCATION == 1 )
#      define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 0 * 1024 ) )
#   else
#      define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 12 * 1024 ) )
#   endif
#endif

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
#define configSUPPORT_DYNAMIC_ALLOCATION    0
#else
#define configSUPPORT_DYNAMIC_ALLOCATION    1
#endif
#define configMAX_TASK_NAME_LEN     ( 25 )
#define configUSE_TRACE_FACILITY    0
#define configUSE_16_BIT_TICKS      0
#define configIDLE_SHOULD_YIELD     1
#define configUSE_CO_ROUTINES       0
#define configUSE_MUTEXES           1

#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_ALTERNATIVE_API       0
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_RECURSIVE_MUTEXES     0
#define configQUEUE_REGISTRY_SIZE       8
#define configGENERATE_RUN_TIME_STATS   0
#define configUSE_MALLOC_FAILED_HOOK    1

/* Software timer definitions. */
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       ( 5 )
#if defined (__PN74XXXX__) || defined (__PN73XXXX__)
/* Since PN7462 I2cm, RF, SPIM, CT and other hals are all Blocking calls.
 * Shall use the event mechanism in ISR contexts. This would lead to Message posting to
 * Timer Daemon Task at run time, message queue length of 2 is not enough.
 * So making the queue length to 8.
 *  */
#    define configTIMER_QUEUE_LENGTH        8
#else
#    define configTIMER_QUEUE_LENGTH        2
#endif

#define configTIMER_TASK_STACK_DEPTH    ( 100 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_uxTaskGetStackHighWaterMark 0

#define INCLUDE_xEventGroupSetBitFromISR    1
#define INCLUDE_xTimerPendFunctionCall      1
#define INCLUDE_xSemaphoreGetMutexHolder    1

/* Use the system definition, if there is one */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS       __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS       5        /* 32 priority levels */
#endif

/* The lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY     ( 31 << (8 - configPRIO_BITS) )
/* Priority 5, or 160 as only the top three bits are implemented. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( 5 << (8 - configPRIO_BITS) )

#ifdef CORE_M3
/*
 * Use the Cortex-M3 optimisations, rather than the generic C implementation.
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif   /* CORE_M3 */

#ifdef PH_OSAL_FREERTOS
#   define vPortSVCHandler     SVC_Handler
#   define xPortPendSVHandler  PendSV_Handler
#   define xPortSysTickHandler SysTick_Handler
#endif

/*
#define configASSERT( x )   if( ( x ) == 0 ) {printf("File %s Line %d :", __FILE__, __LINE__); }
*/

#endif /* FREERTOS_CONFIG_H */
