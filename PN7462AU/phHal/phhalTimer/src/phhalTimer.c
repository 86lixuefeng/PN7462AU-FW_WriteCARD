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
 * Timer HAL implementation for Timer configuration and access.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-24 17:36:15 +0530 (Mon, 24 Aug 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14683 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_TIMER
#include "phhalTimer.h"
#include "PN7462AU/PN7462AU_regcore.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_timers.h"
#include "phhalNvic.h"
#include "phhalTimer_Int.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/**
 * Function Pointer to an API that Enables Timer IP Clock and resets Timer etc
 */
typedef phStatus_t (phRomHalTimer_Init_t)(phhalTimer_Timers_t psTimers[]);
/**
 * Function Pointer to an API that allocates valid timer context structure to the caller
 */
typedef phStatus_t (phRomHalTimer_Request_t)(phhalTimer_Units_t bUnits,  phhalTimer_Timers_t **ppTimer,
    phhalTimer_Timers_t psTimers[]);
/**
 * Function Pointer to an API that releases the acquired timer
 */
typedef phStatus_t (phRomHalTimer_Release_t)(phhalTimer_Timers_t * psTimer, phhalTimer_Timers_t psTimers[]);

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static phhalTimer_Timers_t psTimers[E_TIMER_MAX];

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#define phRomHalTimer_Init(psTimers)    ((phRomHalTimer_Init_t *)PH_HAL_TIMER_INIT_ADDR)(psTimers)

#define phRomHalTimer_Request(bUnits, ppTimer, psTimers)         \
    ((phRomHalTimer_Request_t *)PH_HAL_TIMER_REQUESTTIMER_ADDR)(bUnits, ppTimer, psTimers)

#define phRomHalTimer_Release(psTimer, psTimers)         \
    ((phRomHalTimer_Release_t *)PH_HAL_TIMER_RELEASETIMER_ADDR)(psTimer, psTimers)

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalTimer_Init(void)
{
    return phRomHalTimer_Init(psTimers);
}

phStatus_t phhalTimer_RequestTimer(phhalTimer_Units_t bUnits, phhalTimer_Timers_t **ppTimer)
{
    return phRomHalTimer_Request(bUnits, ppTimer, psTimers);
}

phStatus_t phhalTimer_ReleaseTimer(phhalTimer_Timers_t * psTimer)
{
    return phRomHalTimer_Release(psTimer, psTimers);
}

void TIMER_IRQHandler(void)
{
    uint32_t dwIntStatus;

    dwIntStatus = ( PH_REG_GET( TIMERS_INT_STATUS_REG ) & PH_REG_GET( TIMERS_INT_ENABLE_REG ) );

    if ( dwIntStatus & TIMERS_INT_STATUS_REG_TIMER0_TIMEOUT_STATUS_MASK )
    {
        /** Execute interrupt function attached to timer0  */
        psTimers[E_TIMER_0].pCallBackFunc();
    }

    if ( dwIntStatus & TIMERS_INT_STATUS_REG_TIMER1_TIMEOUT_STATUS_MASK )
    {
        /** Execute interrupt function attached to timer4 or timer1 */
        if (psTimers[E_TIMER_4].bBusy == true)
        {
            psTimers[E_TIMER_4].pCallBackFunc();
        }
        else
        {
            psTimers[E_TIMER_1].pCallBackFunc();
        }
    }

    if ( dwIntStatus & TIMERS_INT_STATUS_REG_TIMER2_TIMEOUT_STATUS_MASK )
    {
        /** Execute interrupt function attached to timer2 */
        psTimers[E_TIMER_2].pCallBackFunc();
    }

    if ( dwIntStatus & TIMERS_INT_STATUS_REG_TIMER3_TIMEOUT_STATUS_MASK )
    {
        /** Execute interrupt function attached to timer3 */
        psTimers[E_TIMER_3].pCallBackFunc();
    }
    /* Clear the interrupts. */
    PH_REG_SET( TIMERS_INT_CLR_STATUS_REG, dwIntStatus);
}

#endif /* NXPBUILD__PHHAL_TIMER */
