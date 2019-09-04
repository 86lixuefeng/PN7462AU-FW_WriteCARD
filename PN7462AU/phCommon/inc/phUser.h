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
 * Common tools and utils
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-11 13:30:09 +0530 (Mon, 11 Jul 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18565 $ (v04.13.00)
 */

#ifndef PHUSER_H
#define PHUSER_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/** Helper macro to supress compiler warning for unused variables */
#define PH_USER_UNUSED(PARAM) (PARAM) = (PARAM)

/** User Assertion */
#define PH_USER_ASSERT(x)      if( (x) == 0) { for(;;) { __WFI(); } }


extern uint32_t volatile gCriticalSectionSemaphore;

/** \brief  Enable IRQ Interrupts

  This function enables IRQ interrupts by clearing the I-bit in the CPSR.
  Can only be executed in Privileged modes.
 */

#define __phUser_ExitCriticalSection()                                                          \
    do{                                                                                               \
        /* Do not Enter the below logic if the __phUser_ExitCriticalSection is called without */    \
        /* calling __phUser_EnterCriticalSection */                                                \
        if (gCriticalSectionSemaphore != 0)                                                         \
        {                                                                                           \
            gCriticalSectionSemaphore--;                                                            \
            /* Exit the Critical Section only if it is the last time. */                            \
            if (gCriticalSectionSemaphore == 0)                                                     \
            {                                                                                       \
                __ENABLE_IRQ();                                                                     \
            }                                                                                       \
        }                                                                                           \
    }while(0)

#define __phUser_EnterCriticalSection()                                                         \
    do{                                                                                     \
        gCriticalSectionSemaphore++;                                                                \
        /* Enter the Critical Section only if it is the first time. */                              \
        if (gCriticalSectionSemaphore == 1)                                                         \
        {                                                                                           \
            __DISABLE_IRQ();                                                                        \
        }                                                                                           \
    }while(0)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/** \name CPU speed
 * @{
 */
typedef enum
{
    E_COMMON_CPUSPEED_20MHZ = 0, /**< CPU Clock is 20MHZ. Default and the only supported value */
    E_COMMON_CPUSPEED_10MHZ = 1, /**< CPU Clock is 10MHZ. @warning this value is Not supported */
    E_COMMON_CPUSPEED_5MHZ  = 2 /**< CPU Clock is 5MHZ. @warning this value is Not supported */
} phUser_CpuSpeed_t;
/**
 * @}
 */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * User memcpy for Cortex M0.
 *
 * @param pvDst Destination Address
 * @param pvSrc Source Address
 * @param dwLength Number of bytes to copy.
 */

void phUser_MemCpy(void* pvDst, const void* pvSrc, uint32_t dwLength);
/**
 * User MemCompare for Cortex M0
 *
 * @param pvBuf Buffer 0
 * @param pvCmpBuf Buffer 1
 * @param dwLength Lenght to be compared
 * @return Comparision status
 *
 * @retval 0 if both buffers are equal
 *
 */
uint32_t phUser_MemCmp(const void* pvBuf, const void* pvCmpBuf, uint32_t dwLength);
/**
 * Set default value to Memory
 *
 * @param pvBuf input buffer
 * @param dwu8Val value to be set. Note: only Lower 8 bits are used to set the pvBuf
 * @param dwLength dwLength of the buffer
 */
void phUser_MemSet(void* pvBuf, uint32_t dwu8Val , uint32_t dwLength);

/**
 * Initialize delay divider for Wait Loop.  See \ref phUser_Wait
 *
 * @warning only eCpuSpeed == E_COMMON_CPUSPEED_20MHZ is supported
 *
 * @param eCpuSpeed see \ref phUser_CpuSpeed_t
 */
void phUser_WaitInit(phUser_CpuSpeed_t eCpuSpeed);

/**
 * Helper macro to wait for pre decided micro seconds.
 *
 * @warning the delay is not precisely calibrated.
 * @warning \ref phUser_WaitInit has to be called before calling this function.
 *
 * @param dwUSec micro seconds to wait for
 *
 */
void phUser_Wait(uint32_t dwUSec);


/**
 * Helper macro to wait for pre decided micro seconds.
 *
 * @warning The delay is dependent on HW Timers. If the delay is less than
 *          PH_USER_WAIT_TIMER_LATENCY uSecs (defined in phUser.c),
 *          the implementation falls back on @ref phUser_Wait
 *
 * @warning This API uses HW Timers, but still does not promise really precise waits.  If a really really precise delay is needed, the test application / example
 *          application is expected to use HW Timers directly, and increase the priority of Timer ISR to ensure that precision of such delay.
 *
 * @warning If HW Timers are not available, implementation falls back on @ref phUser_Wait
 *
 * @warning \ref phUser_WaitInit has to be called before calling this function.
 *
 * @param dwUSec micro seconds to wait for
 *
 */
void phUser_HwWait(uint32_t dwUSec);


#ifdef PHFL_PROFILING_ENABLED
#   error Not supported
#endif /* PHFL_PROFILING_ENABLED */

#endif /* PHUSER_H */
