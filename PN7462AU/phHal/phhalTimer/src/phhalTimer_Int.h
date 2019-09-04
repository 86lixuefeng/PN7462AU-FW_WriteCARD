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
 * Timer HAL internal definitions.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALTIMER_INT_H
#define PHHALTIMER_INT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_TIMER
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define     PHHAL_HWTIMER_REGS_SIZE                (12)           /**< Each Timer has 3 registers - 3 *4bytes = 12 */
#define     PHHAL_HWTIMER_TIMERS_MIN_STEP          (1)            /**< All Timers minimum value is 1 */
#define     PHHAL_HWTIMER_TIMER0AND1_MAX_STEP      (0x00000FFFUL) /**< Timer0/1 are 12 bits, hence a max of 0xFFF */
#define     PHHAL_HWTIMER_TIMER2AND3_MAX_STEP      (0xFFFFFFFFUL) /**< Timer2/3 are 32 bits, hence max of 0xFFFFFFFF */
/**
 * To calculate the TimeOut register value of T0 & T1 in milli sec units.
 * NOTE: While using T0/T1 timers we have error time of max 330us.
 * Register Value * .33ms = dwTimeOut MilliSecond value (OR)
 * Register Value = dwTimeOut Millisecond Value * 100/33
 * Maximum dwTimeOut that can be requested is 1200 milliseconds
 */
#define PHHAL_HWTIMER_T0T1_CALC_MS_TIMEOUT_VAL(dwTimeOut, dwT01Reg)           \
                              ( (dwT01Reg) = ( ((dwTimeOut) * 100UL)/ 33UL) )

/**
 * To calculate the TimeOut register value of T0 & T1 in sec units.
 * dwTimeOut is in seconds units. T0 register takes a value equivalent to 1000ms(1s)
 * T1 register takes value of required number of seconds.
 * Maximum dwTimeOut can be 4000 seconds
 */
#define PHHAL_HWTIMER_T0T1_CALC_SECS_TIMEOUT_VAL(dwTimeOut, dwT0Reg)          \
                PHHAL_HWTIMER_T0T1_CALC_MS_TIMEOUT_VAL(1000, (dwT0Reg));

/**
 *
 * Timer T2/T3 1 micro (i.e. 1000ns )delay with 50ns tick is 1000ns/50ns = 20
 * dwTimeout is in microseconds = Register Value * 50ns/1000ns (OR)
 * Register Value = dwTimeout * 1000/50 ns = dwTimeout *20
 */
#define PHHAL_HWTIMER_T2T3_CALC_US_TIMEOUT_VAL(dwTimeout, dwT23Reg)      ( (dwT23Reg) = (dwTimeout) * 20UL )

/**
 *
 * Timer T2/T3 1 milli (i.e. 1000000ns )delay with 50ns tick is 1000000ns/50ns = 20000
 * dwTimeout is in milliseconds = Register Value * 50ns/1000000ns (OR)
 * Register Value = dwTimeout * 1000000/50 ns = dwTimeout *20000
 */
#define PHHAL_HWTIMER_T2T3_CALC_MS_TIMEOUT_VAL(dwTimeout, dwT23Reg)     ( (dwT23Reg) = (dwTimeout) * 20000UL )

/**
 *
 * Timer T2/T3 1 sec (i.e. 1000000000ns )delay with 50ns tick is 1000000000ns/50ns = 20000000
 * dwTimeout is in seconds = Register Value * 50ns/1000000000ns (OR)
 * Register Value = dwTimeout * 1000000000/50 ns = dwTimeout *20000000
 */
#define PHHAL_HWTIMER_T2T3_CALC_SECS_TIMEOUT_VAL(dwTimeout, dwT23Reg)      ( (dwT23Reg) = (dwTimeout) * 20000000UL )


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Structure to Hardware Timer Registers.
 */
typedef struct
{
    uint32_t dwCtrlReg;         /**< Timer Control register */
    uint32_t dwTimeoutReg;      /**< Timeout register */
    uint32_t dwCountReg;        /**< Count register */
}phhalTimer_Regs_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * Timers interrupt service routine.
 *
 * This API is part the Vector Table and directly called in the event of Timer ISR.
 */
void TIMER_IRQHandler(void);

#endif /* NXPBUILD__PHHAL_TIMER */
#endif /* PHHALTIMER_INT_H  */
