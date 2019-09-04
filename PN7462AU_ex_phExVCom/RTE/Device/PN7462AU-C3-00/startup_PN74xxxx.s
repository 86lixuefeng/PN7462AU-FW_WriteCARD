; 
;                  Copyright (c), NXP Semiconductors
; 
;                     (C)NXP Semiconductors
;        All rights are reserved. Reproduction in whole or in part is
;       prohibited without the written consent of the copyright owner.
;   NXP reserves the right to make changes without notice at any time.
;  NXP makes no warranty, expressed, implied or statutory, including but
; not limited to any implied warranty of merchantability or fitness for any
; particular purpose, or that the use will not infringe any third party patent,
;  copyright or trademark. NXP must not be liable for any loss or damage
;                          arising from its use.
; 
; //-------- <<< Use Configuration Wizard in Context Menu >>> ------------------


;   <o> Main Stack size (in Bytes) <256-2048:8>
;   <i> This is the stack that would be used by interrupts

MSP_Size      EQU              400
	
;   <o> Process Stack size (in Bytes) <256-10240:8>
;       <i> This is General Stack during course of execution.
;       <i> If you are using HEAP, then this should be in lower range
;       <i> because tasks would be run from within the heap
	
PSP_Size      EQU             2048
	
;   <o> HEAP Size (in Bytes) <0-10240:8>
;       <i> Generally, RTOS and library would use this HEAP
;       <i> If you are not using RTOS, then this can be in lower range.
;       <i> else, depending on application, this should be in 
;       <i> higher range to accomodate various objects created 
;       <i> for/by RTOS.

Heap_Size     EQU             5000

; // <<< end of configuration section >>>

;
;       +-------------------+ :=  __initial_msp
;       | MSP (for ISR)     |
;       |                   |  /MSP_Size/
;       |                   |
;       +-------------------+ := __initial_psp = __initial_sp = (__initial_msp - /MSP_Size/)
;       | PSP               |
;       | PSP               |
;       | (grows down)      | /PSP_Size/
;       |                   |
;       |                   |
;       |                   | 
;       +-------------------+ := Stack_Mem = (__initial_psp - PSP_Size)
;       |                   | := __heap_limit
;       |                   |
;       |                   |
;       |                   | /Heap_Size/
;       |                   |
;       | (grows up)        |
;       | HEAP (malloc,etc.)|
;       +-------------------+ := __heap_base = Heap_Mem
;       |                   |
;       | BSS,DATA          |
;       |                   |
;       +-------------------+ := PH_RAM_MEM_START (0x00100020)
;

__initial_msp EQU       0x00102F00
__initial_psp EQU   (__initial_msp  - MSP_Size)
__initial_sp  EQU    __initial_psp
Stack_Mem     EQU   (__initial_psp - PSP_Size)

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_msp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     TIMER_IRQHandler          ;  0
                DCD     0                         ;  1
                DCD     RF_IRQHandler             ;  2
                DCD     0                         ;  3
                DCD     0                         ;  4
                DCD     0                         ;  5
                DCD     HIF_IRQHandler            ;  6
                DCD     CTIF_IRQHandler           ;  7
                DCD     0                         ;  8
                DCD     PMU_IRQHandler            ;  9
                DCD     SPIM_IRQHandler           ; 10
                DCD     I2CM_IRQHandler           ; 11
                DCD     PCR_IRQHandler            ; 12
                DCD     GPIO_Common_IRQHandler    ; 13
                DCD     GPIO_01_IRQHandler        ; 14
                DCD     GPIO_02_IRQHandler        ; 15
                DCD     GPIO_03_IRQHandler        ; 16
                DCD     GPIO_04_IRQHandler        ; 17
                DCD     GPIO_05_IRQHandler        ; 18
                DCD     GPIO_06_IRQHandler        ; 19
                DCD     GPIO_07_IRQHandler        ; 20
                DCD     GPIO_08_IRQHandler        ; 21
                DCD     GPIO_09_IRQHandler        ; 22
                DCD     GPIO_10_IRQHandler        ; 23
                DCD     GPIO_11_IRQHandler        ; 24
                DCD     GPIO_12_IRQHandler        ; 25
                DCD     0                         ; 26
                DCD     0                         ; 27
                DCD     0                         ; 28
                DCD     0                         ; 29
                DCD     0                         ; 30
                DCD     0                         ; 31

verter
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
				CPSID   i
                LDR     R0, =__initial_psp
                MSR     PSP, R0
                MOVS    R0, #2
                MSR     CONTROL, R0
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT TIMER_IRQHandler           [WEAK]
                EXPORT RF_IRQHandler              [WEAK]
                EXPORT HIF_IRQHandler             [WEAK]
                EXPORT CTIF_IRQHandler            [WEAK]
                EXPORT PMU_IRQHandler             [WEAK]
                EXPORT SPIM_IRQHandler            [WEAK]
                EXPORT I2CM_IRQHandler            [WEAK]
                EXPORT PCR_IRQHandler             [WEAK]
                EXPORT GPIO_Common_IRQHandler     [WEAK]
                EXPORT GPIO_01_IRQHandler         [WEAK]
                EXPORT GPIO_02_IRQHandler         [WEAK]
                EXPORT GPIO_03_IRQHandler         [WEAK]
                EXPORT GPIO_04_IRQHandler         [WEAK]
                EXPORT GPIO_05_IRQHandler         [WEAK]
                EXPORT GPIO_06_IRQHandler         [WEAK]
                EXPORT GPIO_07_IRQHandler         [WEAK]
                EXPORT GPIO_08_IRQHandler         [WEAK]
                EXPORT GPIO_09_IRQHandler         [WEAK]
                EXPORT GPIO_10_IRQHandler         [WEAK]
                EXPORT GPIO_11_IRQHandler         [WEAK]
                EXPORT GPIO_12_IRQHandler         [WEAK]


TIMER_IRQHandler
RF_IRQHandler
HIF_IRQHandler
CTIF_IRQHandler
PMU_IRQHandler
SPIM_IRQHandler
I2CM_IRQHandler
PCR_IRQHandler
GPIO_Common_IRQHandler
GPIO_01_IRQHandler
GPIO_02_IRQHandler
GPIO_03_IRQHandler
GPIO_04_IRQHandler
GPIO_05_IRQHandler
GPIO_06_IRQHandler
GPIO_07_IRQHandler
GPIO_08_IRQHandler
GPIO_09_IRQHandler
GPIO_10_IRQHandler
GPIO_11_IRQHandler
GPIO_12_IRQHandler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = __initial_psp
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
