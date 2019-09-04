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
 * Global data types.
 * $Author: Purnank G (ing05193) $
 * $Revision: 18765 $ (v04.13.00)
 * $Date: 2016-11-30 12:43:32 +0530 (Wed, 30 Nov 2016) $
 *
 * History:
 *
 */

#ifndef PH_DATATYPES_H
#define PH_DATATYPES_H

#ifndef PH_PLATFORM_H_
#   include <ph_Platform.h>
#endif

/** \addtogroup phDatatypes
 *
 * \brief Contains definitions for portable data types used within this firmware.
 *
 * The data types defined here have to be used instead of standard C data types (e.g. int, char)
 * to allow easier portability of the firmware.
 * @{
 */

#ifndef PH_CONFIG_H
#   include <ph_Config.h>
#endif

#if !defined(__cplusplus) || defined(__arm__)
#ifndef __BOOL_DEFINED
#define __BOOL_DEFINED 1
/**
* \brief Boolean type
*/
#ifndef false
#define false    0
#endif
#ifndef true
#define true     (!false)
#endif
typedef uint32_t bool;
#endif
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#ifndef NULL
#define NULL ((void*)0)  /**< Value used for NULL pointers */
#endif


/**
 * \brief Declaration of interrupt service routines.
 * This macro should be used for declaration of interrupt service routines. Example:
 * \code
PH_INTERRUPT(phSwp_Isr)
{
   ...
}
\endcode
 */
#define PH_INTERRUPT(ISR) void (ISR)(void)

#ifdef PHFL_ENABLE_STACK_CHECK
#   define PH_CALL_DIRECT_FUNCTION(fct) (fct)
#else
#   define PH_CALL_DIRECT_FUNCTION(fct)
#endif

#define STRINGIFY(a) #a

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5)
#    define PH_ALIGN(X)           __align(X)
#    define PH_NOINIT             __attribute__ ((section (".noinit")))
#    define PH_NOINIT_ALIGNED     __attribute__ ((aligned (4),section(".noinit")))
#    define PH_PACK_STRUCT_BEGIN  __packed
#    define PH_PACK_STRUCT_END
#    define PH_PLACE_FUNCTION_TO_SECTION(SECTIONNAME) \
                                  __attribute__ ((section (SECTIONNAME)))
#    define PH_UNUSED             __attribute__((unused))
#    define PH_USED               __attribute__((used))
#    define PH_WEAK               __attribute__((weak))
#elif (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC)
#    define PH_ALIGN(a)           __attribute__ ((aligned (a)))
#    define PH_NOINIT             __attribute__ ((section (".noinit")))
#    define PH_NOINIT_ALIGNED     __attribute__ ((aligned (4),section(".noinit")))
#    define PH_PACK_STRUCT_BEGIN
#    define PH_PACK_STRUCT_END    __attribute__ (( packed ))
#    define PH_PLACE_FUNCTION_TO_SECTION(SECTIONNAME)                  \
                                  __attribute__ ((section (SECTIONNAME)))
#    define PH_UNUSED             __attribute__((unused))
#    define PH_USED               __attribute__((used))
#    define PH_WEAK               __attribute__((weak))
#elif (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR)
#    define PH_ALIGN(X)           _Pragma(STRINGIFY(data_alignment = X))
#    define PH_NOINIT             __no_init
#    define PH_NOINIT_ALIGNED     _Pragma(STRINGIFY(data_alignment = 4)) __no_init
#    define PH_PLACE_FUNCTION_TO_SECTION(SECTION) \
                                  _Pragma(STRINGIFY(location = # SECTION))
#    define PH_PACK_STRUCT_BEGIN  __packed
#    define PH_PACK_STRUCT_END
#    define PH_UNUSED             /* Not used API parameters */
#    define PH_USED               /* TBU */
#    define PH_WEAK               __weak
#else /* PHFL_CONFIG_TARGET_PLATFORM */
#    error "Platform/configuration not supported"
#endif /* PHFL_CONFIG_TARGET_PLATFORM */

#if defined(__QMORE__) || defined(__DOXYGEN__)
#    undef PH_PLACE_FUNCTION_TO_SECTION
#    undef PH_PACK_STRUCT_BEGIN
#    undef PH_PACK_STRUCT_END
#    undef PH_UNUSED
#    undef PH_ALIGN
#    undef PH_NOINIT
#    undef PH_NOINIT_ALIGNED
#    undef __WFI

#    define PH_PLACE_FUNCTION_TO_SECTION(SECTIONNAME)
#    define PH_PACK_STRUCT_BEGIN
#    define PH_PACK_STRUCT_END
#    define PH_UNUSED
#    define PH_ALIGN(X)
#    define PH_NOINIT
#    define PH_NOINIT_ALIGNED
#    define __WFI()
#endif /*  __QMORE__ */

/**
 * \brief Pointer to a 32 bits register
 */
typedef volatile uint32_t * pReg32_t;


/* Satsfy the compiler regarding unused local variabled. */

#define UNUSED(PARAM) ((void)(PARAM))

/* Hint the compiler regarding unused parameters of the function.
 *
 * Usage,
 *
 *  void my_function(int UNUSED_PARAM(var))
 *
 */

#ifdef __UNUSED_PARAM
#   define UNUSED_PARAM UNUSED_PARAM
#else
#   define UNUSED_PARAM(PARAM)  PARAM __attribute__ ((__unused__))
#endif


/** @} */

#endif /* PH_DATATYPES_H */
