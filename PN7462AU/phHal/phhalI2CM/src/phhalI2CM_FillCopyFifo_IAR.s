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
 * I2C Master FIFO functionality implementation for IAR
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-28 16:51:09 +0530 (Mon, 28 Sep 2015) $
 * $Author: Sugasi Rajesh (nxp69678) $
 * $Revision: 15513 $
 */


        MODULE  ?i2cm

        PUBLIC  phhalI2CM_AsmFill_OR_CopyFifo

        SECTION .text:CODE:REORDER(2)
        THUMB

phhalI2CM_AsmFill_OR_CopyFifo:

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
    PUSH    {LR}
    PUSH    {R0-R7}
    DSB
    CPSID I
    CMP R2, #16
    BNE CHK_DW /* CHK_DW */
    LDMIA R0!, {R2, R3, R4, R5}
    STMIA R1!, {R2, R3, R4, R5}
    B RETURN /* RETURN */

CHK_DW:: /* CHK_DW */
    CMP R2, #8
    BNE CHK_W /* CHK_W */
    LDMIA R0!, {R2, R3}
    STMIA R1!, {R2, R3}
    B RETURN /* RETURN */

CHK_W: /* CHK_W */
    CMP R2, #4
    BNE RETURN /* RETURN */
    LDM R0!, {R2}
    STM R1!, {R2}
RETURN: /* RETURN */
    DSB
    CPSIE I
    ISB
    POP    {R0-R7}
    POP    {PC}

    END

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
