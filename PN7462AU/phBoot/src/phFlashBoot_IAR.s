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
 * Boot file for IAR based build system.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-28 16:51:09 +0530 (Mon, 28 Sep 2015) $
 * $Author: Sugasi Rajesh (nxp69678) $
 * $Revision: 15513 $
 */


        MODULE  ?cstartup

        PUBLIC  __iar_program_start
        EXTERN  __cmain
        EXTERN  gkpphFlashBoot_InitialPSP
        EXTERN  __vector_table
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp

        SECTION .after_vectors:CODE:REORDER(2)
        THUMB

__iar_program_start:
        CPSID i
        LDR     R0, =gkpphFlashBoot_InitialPSP
        LDR     R1, [R0]
        msr     PSP, R1
        movs    R0, #2
        msr     CONTROL, R0
        isb

        FUNCALL __iar_program_start, __iar_init_core
        BL      __iar_init_core
        FUNCALL __iar_program_start, __iar_init_vfp
        BL      __iar_init_vfp

        FUNCALL __iar_program_start, __cmain
        BL      __cmain

        REQUIRE __vector_table

        PUBLIC phFlashBoot_HardFault_Handler
        EXTERN phFlashBoot_HardFault_HandlerC
phFlashBoot_HardFault_Handler:
        MOVS   R0, #4
        MOV    R1, LR
        TST    R0, R1
        BEQ    _MSP
        MRS    R0, PSP
        B      phFlashBoot_HardFault_HandlerC
_MSP:
        MRS    R0, MSP
        B      phFlashBoot_HardFault_HandlerC

        END

