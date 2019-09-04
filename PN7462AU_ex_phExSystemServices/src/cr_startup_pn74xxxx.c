//*****************************************************************************
// PN74xxxx Microcontroller Startup code for use with LPCXpresso IDE
//
// $Revision$ (v05.19.00)
//
//*****************************************************************************
//
// Copyright(C) NXP Semiconductors, 2015
// All rights reserved.
//
// Software that is described herein is for illustrative purposes only
// which provides customers with programming information regarding the
// LPC products.  This software is supplied "AS IS" without any warranties of
// any kind, and NXP Semiconductors and its licensor disclaim any and
// all warranties, express or implied, including all implied warranties of
// merchantability, fitness for a particular purpose and non-infringement of
// intellectual property rights.  NXP Semiconductors assumes no responsibility
// or liability for the use of the software, conveys no license or rights under any
// patent, copyright, mask work right, or any other intellectual property rights in
// or to any products. NXP Semiconductors reserves the right to make changes
// in the software without notification. NXP Semiconductors also makes no
// representation or warranty that such application will be suitable for the
// specified use without further testing or modification.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation is hereby granted, under NXP Semiconductors' and its
// licensor's relevant copyrights in the software, without fee, provided that it
// is used in conjunction with NXP Semiconductors microcontrollers.  This
// copyright, permission, and disclaimer notice must appear in all copies of
// this code.
//*****************************************************************************

#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
    extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
// Declaration of external SystemInit function
// extern void SystemInit(void);

//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
//
//*****************************************************************************
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);

//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
//*****************************************************************************
void TIMER_IRQHandler (void) ALIAS(IntDefaultHandler);
void RF_IRQHandler (void) ALIAS(IntDefaultHandler);
void EECTRL_IRQHandler (void) ALIAS(IntDefaultHandler);
void SWPTX_IRQHandler (void) ALIAS(IntDefaultHandler);
void HIF_IRQHandler (void) ALIAS(IntDefaultHandler);
void CTIF_IRQHandler (void) ALIAS(IntDefaultHandler);
void PMU_IRQHandler (void) ALIAS(IntDefaultHandler);
void SPIM_IRQHandler (void) ALIAS(IntDefaultHandler);
void I2CM_IRQHandler (void) ALIAS(IntDefaultHandler);
void PCR_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_Common_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_01_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_02_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_03_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_04_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_05_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_06_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_07_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_08_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_09_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_10_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_11_IRQHandler (void) ALIAS(IntDefaultHandler);
void GPIO_12_IRQHandler (void) ALIAS(IntDefaultHandler);

//*****************************************************************************
// The entry point for the application.
// __main() is the entry point for redlib based applications
// main() is the entry point for newlib based applications
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);
//*****************************************************************************
//
// External declaration for the pointer to the stack top from the Linker Script
//
//*****************************************************************************
extern void _vStackTop(void);

//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
extern void (* const g_pfnVectors[])(void);
__attribute__ ((used,section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    &_vStackTop,                     // The initial stack pointer
    ResetISR,                        // The reset handler
    NMI_Handler,                     // The NMI handler
    HardFault_Handler,               // The hard fault handler
    0,                               // Reserved
    0,                               // Reserved
    0,                               // Reserved
    0,                               // Reserved
    0,                               // Reserved
    0,                               // Reserved
    0,                               // Reserved
    SVC_Handler,                     // SVCall handler
    0,                               // Reserved
    0,                               // Reserved
    PendSV_Handler,                  // The PendSV handler
    SysTick_Handler,                 // The SysTick handler

    // MCU specific handlers
    TIMER_IRQHandler,                // TIMER       = 0
    0,                               // Reserved
    RF_IRQHandler,                   // RF          = 2
    EECTRL_IRQHandler,               // EECTRL      = 3
    SWPTX_IRQHandler,                // SWPTX       = 4
    0,                               // Reserved
    HIF_IRQHandler,                  // HIF         = 6
    CTIF_IRQHandler,                 // CTIF        = 7
    0,                               // Reserved
    PMU_IRQHandler,                  // PMU         = 9
    SPIM_IRQHandler,                 // SPIM        = 10
    I2CM_IRQHandler,                 // I2CM        = 11
    PCR_IRQHandler,                  // PCR         = 12
    GPIO_Common_IRQHandler,          // GPIO_Common = 13
    GPIO_01_IRQHandler,              // GPIO_01     = 14
    GPIO_02_IRQHandler,              // GPIO_02     = 15
    GPIO_03_IRQHandler,              // GPIO_03     = 16
    GPIO_04_IRQHandler,              // GPIO_04     = 17
    GPIO_05_IRQHandler,              // GPIO_05     = 18
    GPIO_06_IRQHandler,              // GPIO_06     = 19
    GPIO_07_IRQHandler,              // GPIO_07     = 20
    GPIO_08_IRQHandler,              // GPIO_08     = 21
    GPIO_09_IRQHandler,              // GPIO_09     = 22
    GPIO_10_IRQHandler,              // GPIO_10     = 23
    GPIO_11_IRQHandler,              // GPIO_11     = 24
    GPIO_12_IRQHandler,              // GPIO_12     = 25
    0,                               // Reserved    = 26
    0,                               // Reserved    = 27
    0,                               // Reserved    = 28
    0,                               // Reserved    = 29
    0,                               // Reserved    = 30
    0                                // Reserved    = 31
};

//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors")))
void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table". This table is
// created by the linker via the Code Red managed linker script mechanism. It
// contains the load address, execution address and length of each RW data
// section and the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void
ResetISR(void) {

    //
    // Copy the data sections from flash to SRAM.
    //
    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    // Load base address of Global Section Table
    SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }
    // At this point, SectionTableAddr = &__bss_section_table;
    // Zero fill the bss segment
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }

//    SystemInit();

#if defined (__cplusplus)
    //
    // Call C++ library initialisation
    //
    __libc_init_array();
#endif

#if defined (__REDLIB__)
    // Call the Redlib library, which in turn calls main()
    __main() ;
#else
    main();
#endif
    //
    // main() shouldn't return, but if it does, we'll just enter an infinite loop
    //
    while (1) {
        ;
    }
}

//*****************************************************************************
// Default exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void NMI_Handler(void)
{   while(1) { }
}
__attribute__ ((section(".after_vectors")))
void HardFault_Handler(void)
{   while(1) { }
}

__attribute__ ((section(".after_vectors")))
void SVC_Handler(void)
{   while(1) { }
}

__attribute__ ((section(".after_vectors")))
void PendSV_Handler(void)
{   while(1) { }
}

__attribute__ ((section(".after_vectors")))
void SysTick_Handler(void)
{   while(1) { }
}

//*****************************************************************************
//
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void)
{   while(1) { }
}
