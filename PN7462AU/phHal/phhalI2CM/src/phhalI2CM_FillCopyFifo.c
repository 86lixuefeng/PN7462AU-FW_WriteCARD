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
 * I2C Master FIFO functionality implementation.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-28 16:51:09 +0530 (Mon, 28 Sep 2015) $
 * $Author: Sugasi Rajesh (nxp69678) $
 * $Revision: 15513 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM
#include "phhalI2CM_FillCopyFifo.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#ifdef PHFL_WITH_CTC
#pragma CTC SKIP
#endif

#ifdef __QMORE__
#   undef ASM_INLINE
#   define ASM_INLINE
#endif

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
ASM_INLINE void ASM_NO_CMPILR_OPTI phhalI2CM_AsmFill_OR_CopyFifo(
        uint32_t *src PH_UNUSED,
        uint32_t *des PH_UNUSED,
        uint32_t len  PH_UNUSED)
{
#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC)
    /* Critical section, Interrupts are disabled using CPSID I & enabled at last using CPSIE I. */

    /* copy data from memory pointed by R0 (src) to Rn registers. */
    /* Copy data from Rn and write it to memory pointed by R1 (des). */

    __asm(
            "PUSH    {LR};"
            "PUSH    {R0-R7};"
            "DSB;"
            "CPSID I;"
            "CMP R2, #16;"
            "BNE 1f;" /* CHK_DW */
            "LDMIA R0, {R2, R3, R4, R5};"
            "STMIA R1, {R2, R3, R4, R5};"
            "B 3f;" /* RETURN */

            "1:" /* CHK_DW */
            "CMP R2, #8;"
            "BNE 2f;" /* CHK_W */
            "LDMIA R0, {R2, R3};"
            "STMIA R1, {R2, R3};"
            "B 3f;" /* RETURN */

            "2:" /* CHK_W */
            "CMP R2, #4;"
            "BNE 3f;" /* RETURN */
            "LDM R0, {R2};"
            "STM R1, {R2};"
            "3:" /* RETURN */
            "DSB;"
            "CPSIE I;"
            "ISB;"
            "POP    {R0-R7};"
            "POP    {PC};"
    );
#elif (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5)
    PRESERVE8
    THUMB

    EXPORT phhalI2CM_AsmFill_OR_CopyFifo
    PUSH {lr}
    PUSH {r0-r7}
    DSB
    CPSID I

    CMP r2, #16
    BNE CHK_DW
    LDMIA R0!, {R2, R3, R4, R5}
    STMIA R1!, {R2, R3, R4, R5}
    B RETURN

CHK_DW PROC
    CMP R2, #8
    BNE CHK_W
    LDMIA R0!, {R2, R3}
    STMIA R1!, {R2, R3}
    B RETURN
    ENDP

CHK_W PROC
    CMP R2, #4
    BNE RETURN
    LDM R0, {R2}
    STM R1, {R2}
    ENDP

RETURN PROC
    DSB
    CPSIE I
    ISB
    POP {r0-r7}
    POP {pc}
    ENDP
#else
#error Not supported compiler.
#endif /* PH_CONFIG_TARGET_PLATFORM_DS5 */
}

#ifdef PHFL_WITH_CTC
#pragma CTC ENDSKIP
#endif

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHHAL_I2CM */
