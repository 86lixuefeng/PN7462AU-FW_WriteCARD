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
 * \file phFlashBoot_InitArm_GNU.c
 *
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 18454 $ (v04.13.00)
 * $Date: 2016-06-20 12:21:14 +0530 (Mon, 20 Jun 2016) $
 *
 */


#include <ph_Datatypes.h>

#include "PN7462AU_Ver.h"
#include "ph_MemoryMap.h"

#ifdef __ICCARM__
    #pragma language=extended
    #pragma segment="CSTACK"
#endif

/*

       +-------------------+ := (PH_RAM_MEM_END + 1) = __top_SRAM11 = INITIAL_MSP
       | MSP (for ISR)     |
       |                   |  /PHFL_MSP_SIZE/
       |                   |
       +-------------------+ := (PH_RAM_MEM_END + 1) - /PHFL_MSP_SIZE/ = *gkpphFlashBoot_InitialPSP
       | PSP               |
       | (grows down)      |
       |                   |
       |                   |
       + . . . . . . . . . +  Invisible boundary, no protection.
       |                   |
       |                   |
       | (grows up)        |
       | HEAP (malloc,etc.)|
       +-------------------+ := _pvHeapStart
       |                   |
       | BSS,DATA          |
       |                   |
       +-------------------+ := PH_RAM_MEM_START (0x00100020)

*/


#ifndef PHFL_MSP_SIZE
#   define PHFL_MSP_SIZE        (400)
#endif /* PHFL_MSP_SIZE */


/* Initial MSP. */
#define INITIAL_MSP ( (void *) (1+ PH_RAM_MEM_END))

/* Vector table size for PN7462AU */
#define VECTOR_COUNT (16 + 32)

#ifdef __CODE_RED
#   define FLASH_BOOT_START ResetISR
#elif PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
#   define FLASH_BOOT_START __iar_program_start
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
#   define FLASH_BOOT_START phFlashBoot_BootISR
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
#   define FLASH_BOOT_START Reset_Handler
#endif

extern void FLASH_BOOT_START(void);
void UnExpected_IRQHandler(void);

extern void NMI_Handler(void);
extern void phFlashBoot_HardFault_Handler(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void TIMER_IRQHandler(void);
extern void RF_IRQHandler(void);
extern void HIF_IRQHandler(void);
extern void CTIF_IRQHandler(void);
extern void PMU_IRQHandler(void);
extern void SPIM_IRQHandler(void);
extern void I2CM_IRQHandler(void);
extern void PCR_IRQHandler(void);
extern void GPIO_Common_IRQHandler(void);
extern void GPIO_01_IRQHandler(void);
extern void GPIO_02_IRQHandler(void);
extern void GPIO_03_IRQHandler(void);
extern void GPIO_04_IRQHandler(void);
extern void GPIO_05_IRQHandler(void);
extern void GPIO_06_IRQHandler(void);
extern void GPIO_07_IRQHandler(void);
extern void GPIO_08_IRQHandler(void);
extern void GPIO_09_IRQHandler(void);
extern void GPIO_10_IRQHandler(void);
extern void GPIO_11_IRQHandler(void);
extern void GPIO_12_IRQHandler(void);

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
#    define VECTOR_TABLE_NAME __vector_table
#    define ISR_ALIAS(ISR)\
    void ISR(void)   __attribute__ ((weak, alias ("UnExpected_IRQHandler")));
#elif PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
#       define VECTOR_TABLE_NAME __vector_table
#   define ISR_ALIAS(ISR)\
    _Pragma(STRINGIFY(weak ISR = UnExpected_IRQHandler))
#elif PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
#       define VECTOR_TABLE_NAME __Vectors
#   define ISR_ALIAS(ISR)\
    __weak void ISR(void)            __attribute__((alias("UnExpected_IRQHandler")));

#endif

/* An interrupt function neither takes in any parameter, nor returns anything */
typedef void( *intfunc )( void );

struct phFlashBoot_BootVectorVersion {
    const void * initial_msp;
    const intfunc vtTable[VECTOR_COUNT - 1];
    /* Depending on uses cases like secondary FW, this may be Length/Checksum/etc. */
    const uint32_t dwVTMagic;
    const char szVersionString[60];
    /**
     * Optional placeholder for Check Sum that may be added by build system
     *
     * Note: Current implementation does not use it. */
};

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
    #pragma location = ".intvec"
#endif
#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
    PH_PLACE_FUNCTION_TO_SECTION(".isr_vector")
#endif
#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
    PH_PLACE_FUNCTION_TO_SECTION("RESET")
#endif

const struct phFlashBoot_BootVectorVersion VECTOR_TABLE_NAME = {
    /*  Core Level - CM0 */
    INITIAL_MSP,                    /*  The initial stack pointer. */
    {
        &FLASH_BOOT_START,          /* Reset Handler (Entry point of program) */
        &NMI_Handler,               /* The NMI handler. Used for WatchDog Timer */
        &phFlashBoot_HardFault_Handler, /* The hard fault handler */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        &SVC_Handler,            /* SVCall */
        NULL,                       /* Reserved */
        NULL,                       /* Reserved */
        &PendSV_Handler,            /* PendSV */
        &SysTick_Handler,           /* SysTick */
        &TIMER_IRQHandler,          /* TIMER       = 0  */
        NULL,                       /* Reserved    = 1  */
        &RF_IRQHandler,             /* CLIF        = 2  */
        NULL,   /* EECTRL (IRQ is not used in HAL) = 3  */
        NULL,                       /* Reserved    = 4  */
        NULL,                       /* Reserved    = 5  */
        &HIF_IRQHandler,            /* HIF         = 6  */
        &CTIF_IRQHandler,           /* CTIF        = 7  */
        NULL,                       /* Reserved    = 8  */
        &PMU_IRQHandler,            /* PMU         = 9  */
        &SPIM_IRQHandler,           /* SPIM        = 10 */
        &I2CM_IRQHandler,           /* I2CM        = 11 */
        &PCR_IRQHandler,            /* PCR         = 12 */
        &GPIO_Common_IRQHandler,    /* GPIO Common = 13 */
        &GPIO_01_IRQHandler,        /* GPIO 01     = 14 */
        &GPIO_02_IRQHandler,        /* GPIO 02     = 15 */
        &GPIO_03_IRQHandler,        /* GPIO 03     = 16 */
        &GPIO_04_IRQHandler,        /* GPIO 04     = 17 */
        &GPIO_05_IRQHandler,        /* GPIO 05     = 18 */
        &GPIO_06_IRQHandler,        /* GPIO 06     = 19 */
        &GPIO_07_IRQHandler,        /* GPIO 07     = 20 */
        &GPIO_08_IRQHandler,        /* GPIO 08     = 21 */
        &GPIO_09_IRQHandler,        /* GPIO 09     = 22 */
        &GPIO_10_IRQHandler,        /* GPIO 10     = 23 */
        &GPIO_11_IRQHandler,        /* GPIO 11     = 24 */
        &GPIO_12_IRQHandler,        /* GPIO 12     = 25 */
        NULL,                       /* Reserved    = 26 */
        NULL,                       /* Reserved    = 27 */
        NULL,                       /* Reserved    = 28 */
        NULL,                       /* Reserved    = 29 */
        NULL,                       /* Reserved    = 30 */
        NULL                        /* Reserved    = 31 */
    },
    ~0,
    { PN7462AU_VER_FILEDESCRIPTION }
};

PH_PLACE_FUNCTION_TO_SECTION(".after_vectors")
void UnExpected_IRQHandler(void)
{
    /* Since application enabled this IRQ without providing any handler
     * for the same, halt here */
    while ( 1 ) {
        __WFI();
    }
}

const uint32_t gkpphFlashBoot_InitialPSP = ( 1 + PH_RAM_MEM_END  - PHFL_MSP_SIZE);


ISR_ALIAS(NMI_Handler)
/* ISR_ALIAS(SVC_Handler) RTOS or phRtos should provide this */
/* ISR_ALIAS(PendSV_Handler) RTOS or phRtos should provide this  */
/* ISR_ALIAS(SysTick_Handler)  RTOS or phRtos should provide this */
ISR_ALIAS(TIMER_IRQHandler)
ISR_ALIAS(RF_IRQHandler)
ISR_ALIAS(HIF_IRQHandler)
ISR_ALIAS(CTIF_IRQHandler)
ISR_ALIAS(PMU_IRQHandler)
ISR_ALIAS(SPIM_IRQHandler)
ISR_ALIAS(I2CM_IRQHandler)
ISR_ALIAS(PCR_IRQHandler)
ISR_ALIAS(GPIO_Common_IRQHandler)
ISR_ALIAS(GPIO_01_IRQHandler)
ISR_ALIAS(GPIO_02_IRQHandler)
ISR_ALIAS(GPIO_03_IRQHandler)
ISR_ALIAS(GPIO_04_IRQHandler)
ISR_ALIAS(GPIO_05_IRQHandler)
ISR_ALIAS(GPIO_06_IRQHandler)
ISR_ALIAS(GPIO_07_IRQHandler)
ISR_ALIAS(GPIO_08_IRQHandler)
ISR_ALIAS(GPIO_09_IRQHandler)
ISR_ALIAS(GPIO_10_IRQHandler)
ISR_ALIAS(GPIO_11_IRQHandler)
ISR_ALIAS(GPIO_12_IRQHandler)
