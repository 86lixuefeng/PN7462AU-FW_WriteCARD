/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *        All rights are reserved. Reproduction in whole or in part is
 *       prohibited without the written consent of the copyright owner.
 *   NXP reserves the right to make changes without notice at any time.
 *  NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 *  copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/**
 * \file phFlashBoot_armcc.c
 *
 * Boot up code for ARMCC based compilers.
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 16352 $ (v04.13.00)
 * $Date: 2015-11-10 12:34:22 +0530 (Tue, 10 Nov 2015) $
 *
 */

#include "ph_Datatypes.h"
#include "ph_MemoryMap.h"

#if (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5)

__asm void Reset_Handler(void) {

__initial_sp EQU (1+ PH_RAM_MEM_END)
				IMPORT __main
				IMPORT main
				IMPORT gkpphFlashBoot_InitialPSP
				EXPORT __initial_sp
				//; Disable interrupts during system boot
				CPSID i
				// B .
				// Set PSP as default stack, MSP is used for ISRs only
				LDR     R0, =gkpphFlashBoot_InitialPSP
				LDR     R1, [R0]
				MSR     PSP, R1
				MOVS    R0, #2
				MSR     CONTROL, R0
				ISB

				BL      main
				ALIGN
}

__asm void phFlashBoot_HardFault_Handler(void) {
                IMPORT  phFlashBoot_HardFault_HandlerC

                MOVS   R0, #4
                MOV    R1, LR
                TST    R0, R1
                BEQ    _MSP
                MRS    R0, PSP
                BL      phFlashBoot_HardFault_HandlerC
_MSP
                MRS    R0, MSP
                BL      phFlashBoot_HardFault_HandlerC
}


void phFlashBoot_ZIBSS(void)
{
    /* Expected to be done post __main */
}
#endif /* (PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5 ) */


