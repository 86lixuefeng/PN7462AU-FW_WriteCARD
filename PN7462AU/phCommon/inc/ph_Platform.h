/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** \file
 *
 * Platform Specific file Header.
 *
 * This file identifies and resolves platform specific build time dependencies.
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 18511 $ (v04.13.00)
 * $Date: 2016-06-27 11:22:25 +0530 (Mon, 27 Jun 2016) $
 *
 * History:
 *
 */

#ifndef PH_PLATFORM_PN7462AU_H
#define PH_PLATFORM_PN7462AU_H

#include <ph_Config.h>

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5) || \
    (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC) || \
    (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR)
#   include <stdio.h>
#   include <stdlib.h>
#   include <stdarg.h>
#   include <stdint.h>
#endif

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
#   include "intrinsics.h"
#endif

#ifndef __QMORE__
#   include <string.h> /* Required for memcpy invocation by NxpRLib */
#endif


#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5)
    #define __NOP()           __nop()
    #define __WFI()           __wfi()
    #define __WFE             __wfe
    #define __SEV             __sev
    #define __DSB()           __dsb()
    #define __ENABLE_IRQ      __enable_irq
    #define __DISABLE_IRQ     __disable_irq
    #define ASM_FN            __asm
#endif /* (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5) */

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR)

    #define __NOP             __no_operation
    #define __ENABLE_IRQ      __enable_interrupt
    #define __DISABLE_IRQ     __disable_interrupt

#endif /* PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5 */

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC)
	/** Helper macro for portability to say that this function would use in-line assembly */
    #   define ASM_FN

	/** Helper macro for portability to say that this function would use in-line assembly */
    #ifdef __QMORE__
    #   define FORCE_INLINE
    #else
    #   define FORCE_INLINE __attribute__(( always_inline ))
    #endif

    /*
        Explicitly undefined
        [U] The identifier '%s' is reserved for use by the library.
        REFERENCE - ISO:C90-7.1.3 Reserved Identifiers */


    /** \brief  No Operation

        No Operation does nothing. This instruction can be used for code alignment purposes.
     */
    FORCE_INLINE
    static inline void __NOP(void)
    {
      __asm volatile ("nop");
    }


    /** \brief  Wait For Interrupt

        Wait For Interrupt is a hint instruction that suspends execution
        until one of a number of events occurs.
     */
    #define __WFI()             \
      __asm volatile ("wfi")

    /** \brief wait for event
     *
     */

    #define __WFE() __asm volatile ("wfe")

    /** \brief send event
     *
     */

    #define __SEV() __asm volatile ("sev")
    /** \brief Data synchronization
     *
     */
    FORCE_INLINE static inline void __DSB(void)
    {
      __asm volatile ("dsb");
    }

    /** \brief  Enable IRQ Interrupts

      This function enables IRQ interrupts by clearing the I-bit in the CPSR.
      Can only be executed in Privileged modes.
     */

    #define __ENABLE_IRQ() __asm volatile ("cpsie i")

    #define __DISABLE_IRQ() __asm volatile ("cpsid i")


#endif /* (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC) */

#endif /* PH_PLATFORM_PN7462AU_H */
