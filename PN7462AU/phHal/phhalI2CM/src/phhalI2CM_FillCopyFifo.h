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
 * I2C Master FIFO functionality.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-11-06 17:25:34 +0530 (Fri, 06 Nov 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 16269 $ (v04.13.00)
 */

#ifndef PHHALI2CM_FILLCOPYFIFO_H
#define PHHALI2CM_FILLCOPYFIFO_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5)
#   define ASM_INLINE            ASM_FN
#   define ASM_NO_CMPILR_OPTI
#endif

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC)
#   define ASM_INLINE         ASM_FN __attribute__((never_inline))
#   define ASM_NO_CMPILR_OPTI __attribute__((naked))
#endif

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR)
#   define ASM_FN
#   define ASM_INLINE
#   define ASM_NO_CMPILR_OPTI
#endif

#ifdef __QMORE__
#   undef  ASM_INLINE
#   define ASM_INLINE
#   undef  ASM_NO_CMPILR_OPTI
#   define ASM_NO_CMPILR_OPTI
#endif

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * phhalI2CM_AsmFill_OR_CopyFifo implements memcpy functionality, except that len can be 4/8/16 bytes only.
 * It falls in critical section since it uses LDM & STM instructions.
 * It will be faster as compared to generic  memcpy function.
 *
 * @param src source address
 * @param des Destination address
 * @param len Length (should be 4 , 8, 16 only.)
 *
 * @warning
 *
 *       -#. This function falls in critical section.
 *       -#. Parameter checking for len is not done.
 *       -#. Overlapping is not checked.
 *
 */

extern ASM_INLINE void ASM_NO_CMPILR_OPTI phhalI2CM_AsmFill_OR_CopyFifo(
        uint32_t *src,
        uint32_t *des,
        uint32_t len);


#endif /* NXPBUILD__PHHAL_I2CM */

#endif /* PHHALI2CM_FILLCOPYFIFO_H_ */
