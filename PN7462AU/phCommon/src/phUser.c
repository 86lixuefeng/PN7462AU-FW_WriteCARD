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
 * Common tools and utils.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-06-22 11:25:52 +0530 (Wed, 22 Jun 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 18485 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Config.h"
#include "ph_Status.h"
#include "phUser.h"
#include "ph_Log.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h> /* MemCpy and family */

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#include <phhalTimer.h>

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
#if PHFL_LOG_ENABLE

#define ENTRIES_IN_ONE_ROW                     16  /* Should be multiple of 4 */
#define ENTRIES_IN_ONE_COLUMN_OF_A_ROW          4  /* Should be a factor of ENTRIES_IN_ONE_ROW */
#endif


#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
#    define VOLATILE_LOOP_COUNTER volatile
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
#    define VOLATILE_LOOP_COUNTER volatile
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
#    define VOLATILE_LOOP_COUNTER /* Not needed */
#else
#    eror Unsupported platform
#endif


/* Compenstation needed for allocation / release of timers */
#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
#   define PH_USER_WAIT_TIMER_LATENCY             80
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
#   define PH_USER_WAIT_TIMER_LATENCY             80
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
#   define PH_USER_WAIT_TIMER_LATENCY             80
#else
#    error Platform not calibrated.
#endif

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static uint32_t gphUserLoopsPerUsShift;
uint32_t volatile gCriticalSectionSemaphore;

#if PHFL_CONFIG_TARGET_PLATFORM != PH_CONFIG_TARGET_PLATFORM_DS5
/* System Clock Frequency (Core Clock) */
uint32_t SystemCoreClock = 20000000;
#endif



/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phUser_WaitTimerExpired(void);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phUser_WaitInit(phUser_CpuSpeed_t eCpuSpeed)
{
    phhalTimer_Init();

    switch(eCpuSpeed)
    {
    case E_COMMON_CPUSPEED_5MHZ:
    case E_COMMON_CPUSPEED_10MHZ:
        PH_USER_ASSERT(0); /* Not Supported */
    default:
        /* 20MHz */
        gphUserLoopsPerUsShift = 2;
        break;
    }
}

/* Wait based on SW Precision */
void phUser_Wait(uint32_t dwUSec)
{
    VOLATILE_LOOP_COUNTER uint32_t dwUSec_vol = dwUSec;
    dwUSec_vol <<= gphUserLoopsPerUsShift;

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
    dwUSec_vol >>= 2; /* Compensate based on Instruction */
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
    dwUSec_vol >>= 2; /* Compensate based on Instruction */
#elif  PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
    dwUSec_vol >>= 1; /* Compensate based on Instruction */
#else
    /* No compensation needed */
#endif

    while(dwUSec_vol)
    {
        /* to be qmore compliant, to the subtract not in the loop header */
        dwUSec_vol--;
    }
}

/* Wait based on HW Timers, if available */
void phUser_HwWait(uint32_t dwUSec) {
    phStatus_t wStatus;
    phhalTimer_Timers_t * gpphUserTimer;
    uint32_t  dwTimerCount = dwUSec;
    if ( dwUSec <  PH_USER_WAIT_TIMER_LATENCY)
    {
        /* Too short delay to even think about timer */
        phUser_Wait(dwUSec);
        return;
    }
    __phUser_EnterCriticalSection();
    wStatus = phhalTimer_RequestTimer(E_TUNIT_MICRO_SECS, &gpphUserTimer);
    __phUser_ExitCriticalSection();
    if ( wStatus != PH_ERR_SUCCESS) {
        /* Timer not awilable, so we just return */
        phUser_Wait(dwUSec);
        return;
    }
    do {
        wStatus = phhalTimer_Configure(gpphUserTimer,
            dwUSec - PH_USER_WAIT_TIMER_LATENCY,
            &phUser_WaitTimerExpired);
        if ( wStatus != PH_ERR_SUCCESS)
        {
            phUser_Wait(dwUSec);
            break;
        }
        wStatus = phhalTimer_Start(gpphUserTimer, E_TIMER_SINGLE_SHOT);
        if ( wStatus != PH_ERR_SUCCESS)
        {
            phUser_Wait(dwUSec);
            break;
        }
        while (dwTimerCount != 0) {
            /* Timer is still running */
            phhalTimer_GetStatus(gpphUserTimer, &dwTimerCount);
        }
    } while (0);
    /* Multiple returns in this function */
    phhalTimer_ReleaseTimer(gpphUserTimer);
}


void phUser_MemCpy(void* pvDst, const void* pvSrc, uint32_t dwLength)
{
	PH_USER_ASSERT(NULL != pvDst);
	PH_USER_ASSERT(NULL != pvSrc);
	/* PH_USER_ASSERT(0 != dwLength);, in some dynamic cases, Length may be 0 */
	(void)memcpy(pvDst, pvSrc, dwLength);
}

uint32_t phUser_MemCmp(const void* pvBuf, const void* pvCmpBuf, uint32_t dwLength)
{
	PH_USER_ASSERT(NULL != pvBuf);
	PH_USER_ASSERT(NULL != pvCmpBuf);
	/* PH_USER_ASSERT(0 != dwLength);, in some dynamic cases, Length may be 0 */
    return (uint32_t)memcmp(pvBuf,pvCmpBuf,dwLength);
}

void phUser_MemSet(void* pvBuf, uint32_t dwu8Val, uint32_t dwLength)
{
	PH_USER_ASSERT(NULL != pvBuf);
	PH_USER_ASSERT(0 != dwLength);
	(void)memset(pvBuf, (int)dwu8Val, dwLength);
}

#ifdef PHFL_WITH_CTC
    /* phUser_Tst_CTC_CoveragePoints would be defined and used from
     * tst/pvSrc/phUser_Tst_CTC.c */
#else
    uint16_t phUser_Tst_CTC_CoveragePoints(void) {
        return 0;
    }
#endif

#if PHFL_LOG_ENABLE
    void phUser_Log_AU8(uint8_t *inBuf, const uint32_t inBufLen)
        {
            uint32_t i;

            printf("\tLen=%d\n\t\t", inBufLen);

            for (i = 0; i < inBufLen; i++) {
                printf(" %02X", inBuf[i]);

                if (((i + 1) % ENTRIES_IN_ONE_COLUMN_OF_A_ROW) == 0) {
                    printf("\t");
                }
                if (((i + 1) % ENTRIES_IN_ONE_ROW) == 0) {
                    printf("\n\t\t");
                }
            }
            printf("\n");
        }

#endif

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static void phUser_WaitTimerExpired(void) {
    __NOP();
}
