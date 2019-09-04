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
 * phRtos_NoRtos.c:  <The purpose and scope of this file>
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
#include "ph_Platform.h"
#include "phRtos.h"

#ifdef PHFL_HALAPI_NO_RTOS
#include "phhalTimer.h"
#include "phUser.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/**
 * Configurable macros
 */
#define PH_RTOS_NORTOS_MAX_EVENTS_GROUPS           15 /** Max number of events that can be registered. */
#define PH_RTOS_NORTOS_OVERRIDE_SYSTICK            0  /** 1-> Do not use Systick for Timeout, infinite blocking.
                                                          0-> Use Systick For Timeout operations. */

#define PH_RTOS_NORTOS_BUSY_BIT                    0x80000000UL

#define PH_RTOS_NORTOS_MAX_TIMER_GROUPS            4 /** Cannot be increased as only 4 Hw Timers are available. */

#define PH_RTOS_NORTOS_MAX_SYSTICK                 800 /** max configurable is 838 milli secs. */

#define PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL           ( ( volatile uint32_t *) 0xe000e010 )
#define PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD           ( ( volatile uint32_t *) 0xe000e014 )
#define PH_RTOS_NORTOS_NVIC_SYSTICK_CUR_VAL        ( ( volatile uint32_t *) 0xe000e018 )
#define PH_RTOS_NORTOS_NVIC_SYSTICK_CLK            0x00000004
#define PH_RTOS_NORTOS_NVIC_SYSTICK_INT            0x00000002
#define PH_RTOS_NORTOS_NVIC_SYSTICK_ENABLE         0x00000001

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
typedef struct{
    phhalTimer_Timers_t *pTimerHandle;
    uint32_t dwTimerID;
    uint32_t dwTimePeriod;
    pphRtos_TimerCallback_t pxphRtosCallbackFunction;
    pphhalTimer_CallbackFun_t pxTimerCallBAck;
    phhalTimer_Mode_t eRunMode;
}phRtos_NoRtos_Timer_t;

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static uint32_t gdwaEvents[PH_RTOS_NORTOS_MAX_EVENTS_GROUPS];
PH_NOINIT static uint32_t gdwSysTickCount;
PH_NOINIT static phRtos_NoRtos_Timer_t gsaTimers[PH_RTOS_NORTOS_MAX_TIMER_GROUPS];
PH_NOINIT static uint8_t gbTimerBusyBits;

#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
PH_NOINIT static volatile uint8_t gbSysTimerExpired;
PH_NOINIT static volatile uint8_t gbSysTimerConfigured;
#endif

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void pphhalTimer_CallbackTimer0(void);
static void pphhalTimer_CallbackTimer1(void);
static void pphhalTimer_CallbackTimer2(void);
static void pphhalTimer_CallbackTimer3(void);

#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
static uint32_t phRtos_NoRtos_SetupSysTick(uint32_t dwBlockTime);
#endif

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
static const pphhalTimer_CallbackFun_t gkTimerCallBAck[] = {pphhalTimer_CallbackTimer0, pphhalTimer_CallbackTimer1,
    pphhalTimer_CallbackTimer2, pphhalTimer_CallbackTimer3};

phRtos_Status_t phRtos_Initialize(void)
{
    phUser_MemSet(gdwaEvents,0,sizeof(gdwaEvents));
    phUser_MemSet(gsaTimers,0,sizeof(gsaTimers));
    gdwSysTickCount = 0;
    gbTimerBusyBits = 0;
#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
    gbSysTimerExpired = false;
    gbSysTimerConfigured = false;
#endif
    return phRtos_Success;
}

void phRtos_TaskDelay(const uint32_t dwTicksToDelay)
{
    phUser_Wait(1000 * dwTicksToDelay);
}

phRtos_EventHandle_t phRtos_EventGroupCreate(void )
{
    uint32_t dwEvents;
    phRtos_EventHandle_t pEvent;

    /* Critical section */
    __phUser_EnterCriticalSection();

    /* Check for available event groups */
    for(dwEvents=0; dwEvents < PH_RTOS_NORTOS_MAX_EVENTS_GROUPS; dwEvents++)
    {
        if( !(gdwaEvents[dwEvents] & PH_RTOS_NORTOS_BUSY_BIT) ){
            gdwaEvents[dwEvents] = PH_RTOS_NORTOS_BUSY_BIT;
            break;
        }
    }

    /* No more events are available.
     * Increase PH_RTOS_NORTOS_MAX_EVENTS_GROUPS count to avail more number of EventGroups */
    if(dwEvents == PH_RTOS_NORTOS_MAX_EVENTS_GROUPS){
        pEvent = NULL;
    }
    else
    {
        pEvent = &gdwaEvents[dwEvents];
    }

    __phUser_ExitCriticalSection();
    return pEvent;
}

uint32_t phRtos_NoRtos_EventGroupClearBits(void * eventHandle, const uint32_t dwBitsToClear )
{
    uint32_t dwEventBits;

    /* Critical section */
    __phUser_EnterCriticalSection();

    dwEventBits = *((uint32_t *)eventHandle);
    *((uint32_t *)eventHandle) &= (~dwBitsToClear);

    __phUser_ExitCriticalSection();

    return dwEventBits;
}

phRtos_Status_t phRtos_NoRtos_EventGroupSetBits(void * eventHandle, const uint32_t dwBitsToSet )
{
    phRtos_Status_t eStatus;
    /* Critical section */
    __phUser_EnterCriticalSection();
    /* Set the event */
    __SEV();
    eStatus = ( ( *((uint32_t *)(eventHandle)) |= (dwBitsToSet) ) == 0 ) ? phRtos_Failure : phRtos_Success;

    __phUser_ExitCriticalSection();

    return eStatus;
}


#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
static uint32_t phRtos_NoRtos_SetupSysTick(uint32_t dwBlockTime)
{

	uint32_t dwTimeToConfig;

	/* Check whether the systick is already configured and running. */
	if(gbSysTimerConfigured)
		return dwBlockTime;


    if(dwBlockTime > PH_RTOS_NORTOS_MAX_SYSTICK)
    {
    	dwTimeToConfig = PH_RTOS_NORTOS_MAX_SYSTICK;
    }
    else
    {
    	dwTimeToConfig = dwBlockTime;
    }

    /*Disable systick*/
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = 0x0;

    /* Configure SysTick count down value to interrupt at the requested time. */
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD) = ( 20000 * dwTimeToConfig) - 1UL;

    /*Clear the current count value*/
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CUR_VAL) = 0x0;

    /*Enable systick timer with Interrupt enabled*/
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = PH_RTOS_NORTOS_NVIC_SYSTICK_CLK | PH_RTOS_NORTOS_NVIC_SYSTICK_INT
        | PH_RTOS_NORTOS_NVIC_SYSTICK_ENABLE;

    /* Systick is configured. */
    gbSysTimerConfigured = true;

    return (dwBlockTime - dwTimeToConfig);
}
#endif

uint32_t phRtos_NoRtos_EventGroupWaitBits(volatile void * volatile eventHandle, const uint32_t dwBitsToWait,
    const bool bClearOnExit, const bool bWaitForAllBits, uint32_t dwBlockTime )
{
    uint32_t bEventBits;
    uint32_t dwTimeToWait = dwBlockTime;

#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
 gbSysTimerExpired = false;
 gbSysTimerConfigured = false;
#endif

    while(1)
    {
        if((bWaitForAllBits && ( (*((uint32_t *)eventHandle) & dwBitsToWait ) == dwBitsToWait))
            || (!bWaitForAllBits && ((*((uint32_t *)eventHandle) & dwBitsToWait)))
			|| (!dwTimeToWait))
        {
#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
            /*Disable systick*/
            *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = 0x0;
#endif
            break;
        }

#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
        if(dwBlockTime != 0xFFFFFFFF)
        {
            if((dwBlockTime == 0) && (gbSysTimerExpired == true))
            {
                /*Disable systick*/
                *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = 0x0;
                break; /* Timer expired. */
            }

            gbSysTimerExpired = false;
            /* Configure Systick timer. */
            dwBlockTime = phRtos_NoRtos_SetupSysTick(dwBlockTime);

        }
#endif

        /* Wait for interrupts/events to occur */
        __WFE();
    }


    bEventBits = *((uint32_t *)eventHandle);

    /* if set clear the events. */
    if(bClearOnExit){
        *((uint32_t *)eventHandle) &= (~dwBitsToWait);
    }

    return bEventBits;
}

uint32_t phRtos_NoRtos_SysTickGetCount(void)
{
    uint32_t dwSysTickCount;

    /* Critical section. */
    __phUser_EnterCriticalSection();
    dwSysTickCount = gdwSysTickCount;
    __phUser_ExitCriticalSection();

    return dwSysTickCount ;

}

void SysTick_Handler(void )
{
    /* Critical section. */
    __phUser_EnterCriticalSection();

    gdwSysTickCount++;

#if !PH_RTOS_NORTOS_OVERRIDE_SYSTICK
    gbSysTimerExpired = true;
    gbSysTimerConfigured = false;
    /* Set the event */
    __SEV();
#endif
    __phUser_ExitCriticalSection();
}

phRtos_Status_t phRtos_NoRtos_ReturnSuccess(void ) {
    return phRtos_Success;
}

void SVC_Handler(void);
void SVC_Handler(void){
    __phUser_EnterCriticalSection();
    while(1) {
        /* Block infinitely, we should have never reached here. */
        __WFI();
    }
}

void PendSV_Handler(void);
void PendSV_Handler(void){
    __phUser_EnterCriticalSection();
    while(1){
        /* Block infinitely, we should have never reached here. */
        __WFI();
    }
}

static void pphhalTimer_CallbackTimer0(void )
{
    gsaTimers[0].pxphRtosCallbackFunction(&gsaTimers[0]);
}

static void pphhalTimer_CallbackTimer1(void )
{
    gsaTimers[1].pxphRtosCallbackFunction(&gsaTimers[1]);
}

static void pphhalTimer_CallbackTimer2(void )
{
    gsaTimers[2].pxphRtosCallbackFunction(&gsaTimers[2]);
}

static void pphhalTimer_CallbackTimer3(void )
{
    gsaTimers[3].pxphRtosCallbackFunction(&gsaTimers[3]);
}

phRtos_TimerHandle_t phRtos_NoRtos_TimerCreate(uint32_t dwTimerID, uint32_t dwTimePeriodInMilliSecs, bool bAutoReload,
     pphRtos_TimerCallback_t fpCallbackFunction )
 {
     uint8_t i;

     if(fpCallbackFunction == NULL)
     {
         return NULL;
     }

     for(i = 0; i < PH_RTOS_NORTOS_MAX_TIMER_GROUPS; i++)
     {
         if((gbTimerBusyBits & (1 << i)) == 0){
             break;
         }
     }
     /** All the timers are occupied. */
     if(i == 4){
         return NULL;
     }

     if(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS, &gsaTimers[i].pTimerHandle)){
         return NULL;
     }

     /** Initialize the timer handler. */
     gbTimerBusyBits |= (1 << i);
     gsaTimers[i].dwTimePeriod = dwTimePeriodInMilliSecs;
     gsaTimers[i].dwTimerID = dwTimerID;
     gsaTimers[i].pxphRtosCallbackFunction = fpCallbackFunction;
     gsaTimers[i].pxTimerCallBAck = gkTimerCallBAck[i];
     gsaTimers[i].eRunMode = (bAutoReload == true)? E_TIMER_FREE_RUNNING: E_TIMER_SINGLE_SHOT;

     return &gsaTimers[i];
 }

 uint32_t phRtos_NoRtos_TimerGetID(phRtos_TimerHandle_t timerHandle)
 {
     if(timerHandle == NULL)
     {
         return 0;
     }
     return ((phRtos_NoRtos_Timer_t *)timerHandle)->dwTimerID;
 }

 phRtos_Status_t phRtos_NoRtos_TimerStart(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime)
 {
     phRtos_NoRtos_Timer_t *psTimerHandler = ((phRtos_NoRtos_Timer_t *)timerHandle);

     (void)dwBlockTime;
     if(timerHandle == NULL)
     {
         return phRtos_Failure;
     }

     if(phhalTimer_Configure(psTimerHandler->pTimerHandle,  psTimerHandler->dwTimePeriod,
         psTimerHandler->pxTimerCallBAck))
     {
         return phRtos_Failure;
     }

     if(phhalTimer_Start(psTimerHandler->pTimerHandle, psTimerHandler->eRunMode))
     {
         return phRtos_Failure;
     }

     return phRtos_Success;
 }

 phRtos_Status_t phRtos_NoRtos_TimerStop(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime)
 {
     phRtos_NoRtos_Timer_t *psTimerHandler = ((phRtos_NoRtos_Timer_t *)timerHandle);

     (void)dwBlockTime;
     if(timerHandle == NULL)
     {
         return phRtos_Failure;
     }

     if(phhalTimer_Stop(psTimerHandler->pTimerHandle))
     {
         return phRtos_Failure;
     }

     return phRtos_Success;
 }

 phRtos_Status_t phRtos_NoRtos_TimerChangePeriod(phRtos_TimerHandle_t timerHandle, uint32_t dwNewPeriod,
      uint32_t dwBlockTime)
 {
     phRtos_NoRtos_Timer_t *psTimerHandler = ((phRtos_NoRtos_Timer_t *)timerHandle);

     (void)dwBlockTime;
     if(timerHandle == NULL)
     {
         return phRtos_Failure;
     }

     psTimerHandler->dwTimePeriod = dwNewPeriod;
     if(phhalTimer_Configure(psTimerHandler->pTimerHandle, dwNewPeriod, psTimerHandler->pxTimerCallBAck))
     {
         return phRtos_Failure;
     }

     return phRtos_Success;
 }

 phRtos_Status_t phRtos_NoRtos_TimerReset(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime)
 {
     phRtos_NoRtos_Timer_t *psTimerHandler = ((phRtos_NoRtos_Timer_t *)timerHandle);

     (void)dwBlockTime;
     if(timerHandle == NULL)
     {
         return phRtos_Failure;
     }

     if(phhalTimer_Stop(psTimerHandler->pTimerHandle))
     {
         return phRtos_Failure;
     }

     if(phhalTimer_Start(psTimerHandler->pTimerHandle, psTimerHandler->eRunMode))
     {
         return phRtos_Failure;
     }
     return phRtos_Success;
 }

 phRtos_Status_t phRtos_NoRtos_TimerDelete(phRtos_TimerHandle_t timerHandle, uint32_t dwBlockTime)
 {
     uint8_t i;
     phRtos_NoRtos_Timer_t *psTimerHandler = ((phRtos_NoRtos_Timer_t *)timerHandle);

     (void)dwBlockTime;
     if(timerHandle == NULL)
     {
         return phRtos_Failure;
     }

     if(phhalTimer_ReleaseTimer(psTimerHandler->pTimerHandle))
     {
         return phRtos_Failure;
     }

     for(i = 0; i < PH_RTOS_NORTOS_MAX_TIMER_GROUPS; i++)
     {
         if(&gsaTimers[i] == psTimerHandler)
         {
             break;
         }
     }
     /** Release the assigned timer. */
     gbTimerBusyBits &= (~(1 << i));

     return phRtos_Success;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* PHFL_HALAPI_NO_RTOS */
