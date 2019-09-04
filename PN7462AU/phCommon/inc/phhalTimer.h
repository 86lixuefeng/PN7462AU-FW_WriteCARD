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
 * Timer HAL Implementation for Timer Module
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef  PHHALTIMER_H
#define  PHHALTIMER_H

/** \addtogroup phhalTimer
 *
 * The General Purpose Timer APIs.
 *
 * This module briefs all the prototypes of Timers hal.
 *
 * This module implements phhalTimer interface Flash code HAL, specific to Timer module.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters
 *                   will not be checked for erroneous values in the HAL API. In such a build configuration use the
 *                   API with caution and pass valid parameters only. Invalid parameters will likely result in
 *                   hardfaults or other failures.
 *
 * @{
 *
 *
 * \dot
 *      digraph "Sample Usage"{
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalTimer_Configure [URL ="\ref phhalTimer_Configure"];
 *          phhalTimer_DeInit [URL ="\ref phhalTimer_DeInit"];
 *          phhalTimer_GetStatus [URL ="\ref phhalTimer_GetStatus"];
 *          phhalTimer_Init [URL ="\ref phhalTimer_Init"];
 *          phhalTimer_ReleaseTimer [URL ="\ref phhalTimer_ReleaseTimer"];
 *          phhalTimer_RequestTimer [URL ="\ref phhalTimer_RequestTimer"];
 *          phhalTimer_Start [URL ="\ref phhalTimer_Start"];
 *          phhalTimer_Stop [URL ="\ref phhalTimer_Stop"];
 *
 *      //Logical Flow
 *          Start -> phhalTimer_Init -> phhalTimer_RequestTimer;
 *          phhalTimer_RequestTimer -> phhalTimer_Start -> phhalTimer_Stop;
 *          phhalTimer_Stop -> phhalTimer_Start;
 *          phhalTimer_Stop -> phhalTimer_DeInit;
 *          phhalTimer_Stop -> phhalTimer_ReleaseTimer;
 *          phhalTimer_ReleaseTimer -> phhalTimer_DeInit;
 *          phhalTimer_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *          phhalTimer_Start -> phhalTimer_GetStatus;
 *          phhalTimer_RequestTimer -> phhalTimer_Configure;
 *          phhalTimer_Configure -> phhalTimer_Start;
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 *
 *
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_TIMER
#include <ph_Status.h>
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/**
 * Maximum Timeout in micro seconds which can be configured for E_TIMER_2/E_TIMER_3 timers.
 */
#define PHHAL_TIMER_T2T3_MAX_US_TIMEOUT                              ( 200000000UL )

/**
 * Maximum Timeout in milli seconds which can be configured for E_TIMER_2/E_TIMER_3 timers.
 */
#define PHHAL_TIMER_T2T3_MAX_MS_TIMEOUT                              ( 200000UL )

/**
 * Maximum Timeout in seconds which can be configured for E_TIMER_2/E_TIMER_3 timers.
 */
#define PHHAL_TIMER_T2T3_MAX_SECS_TIMEOUT                            ( 200UL )

/**
 * Maximum Timeout in milli seconds which can be configured for E_TIMER_0/E_TIMER_1 timers.
 */
#define PHHAL_TIMER_T0T1_MAX_MS_TIMEOUT                              (1200UL)

/**
 * Maximum Timeout in seconds which can be configured for E_TIMER_4 in concatenation mode.
 */
#define PHHAL_TIMER_T0T1_MAX_SECS_TIMEOUT                            (4000UL)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Function pointer to the Timer Call back functions.
 */
typedef void ( *pphhalTimer_CallbackFun_t )( void );

/**
 * Specifies the Hardware Timer ID that hal uses for the user requested unit timer.
 *
 * @note E_TIMER_4 specifies that hal is using both T0 & T1 in concatenation mode.
 */
typedef enum phhalTimer_Id {
    /** \b 0x0000 */ E_TIMER_0,  /**< Timer0 */
    /** \b 0x0001 */ E_TIMER_1,  /**< Timer1 */
    /** \b 0x0002 */ E_TIMER_2,  /**< Timer2 */
    /** \b 0x0003 */ E_TIMER_3,  /**< Timer3 */
    /** \b 0x0004 */ E_TIMER_4,  /**< T0 & T1 concatenation mode, Timer0 & Timer1 are used.*/
    /** \b 0x0005 */ E_TIMER_MAX /**< Timer NOT available. */
} phhalTimer_Id_t;

/**
 * Specifies whether the Hardware Timer configured in Free running mode OR Single shot mode.
 */
typedef enum
{
    /** \b 0x0000 */ E_TIMER_SINGLE_SHOT, /**< Single Shot Timer Mode */
    /** \b 0x0001 */ E_TIMER_FREE_RUNNING /**< Free Shot Timer Mode */
}phhalTimer_Mode_t;

/**
 * Timer hal accepts micro sec/milli sec/sec timer requests.
 */
typedef enum phhalTimer_Units
{
    /** \b 0x0000 */ E_TUNIT_MICRO_SECS, /**< Units : Micro Seconds */
    /** \b 0x0001 */ E_TUNIT_MILLI_SECS, /**< Units : Milli Seconds */
    /** \b 0x0002 */ E_TUNIT_SECS        /**< Units : Seconds */
} phhalTimer_Units_t;

/**
 * Timer structure for the requested timer.
 *
 * @note phhalTimer_Timers_t structure should not be modified by the user application code.
 */
typedef PH_PACK_STRUCT_BEGIN struct phhalTimer_Timers
{
    uint32_t                  dwTimePeriod;    /**< Timer register timeout value. */
    uint32_t                  dwTimeOutRegVal; /**< Timeout register value for the specified Time period. */
    pphhalTimer_CallbackFun_t pCallBackFunc;   /**< CallBack function will be called if the TimeOut is reached. */
    phhalTimer_Id_t           bTimerId;        /**< ID of the timer allocated */
    phhalTimer_Units_t        bUnits;          /**< Timer units micro/milli/secs. */
    uint8_t                   bBusy;           /**< If true then the timer is allocated & busy, cannot be
                                                    allocated to other tasks. **DO NOT MODIFY**. */
}PH_PACK_STRUCT_END phhalTimer_Timers_t;       /**< Timer Config.*/

/**
 * Function Pointer to an API that Disables Timer IP Clock
 */
typedef phStatus_t (phhalTimer_DeInit_t)(void);
/**
 * Function Pointer to an API that Configures the acquired Timer
 */
typedef phStatus_t (phhalTimer_Configure_t)(phhalTimer_Timers_t * psTimer,  uint32_t dwTimePeriod,
    pphhalTimer_CallbackFun_t pCallBackFunc);
/**
 * Function Pointer to an API that Starts the Timer
 */
typedef phStatus_t (phhalTimer_Start_t)(phhalTimer_Timers_t * psTimer, phhalTimer_Mode_t eRunMode);
/**
 * Function Pointer to an API that Stops the specified Timer
 */
typedef phStatus_t (phhalTimer_Stop_t)(phhalTimer_Timers_t * psTimer);
/**
 * Function Pointer to an API that gets the current Timer Count in units
 */
typedef phStatus_t (phhalTimer_GetStatus_t)(phhalTimer_Timers_t * psTimer, uint32_t *pdwTimerCount);


/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * Initializes the timer module's global and static variables, enables Timer NVIC Interrupt.
 * Timer IP clock, resets timer etc.
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS if initialization is completed successfully.
 */
phStatus_t phhalTimer_Init( void );

/**
 * This API Clears Timer NVIC Interrupt, disables Timer IP clock.
 * @return Status of the Operation.
 *
 * @retval    #PH_ERR_SUCCESS on Success
 */
#define phhalTimer_DeInit()       \
	((phhalTimer_DeInit_t *)\
			PH_HAL_TIMER_DEINIT_ADDR)()

/**
 * This API allocates valid timer context structure to the caller if the requested time unit is available.
 * The caller of the API is expected to acquire a mutex before calling this API
 * else returns NULL.
 * @param[in] bUnits Timer tick units.
 * @param[out] ppTimer Timer handle returned by the HAL.
 * @return Status of the Operation
 * @retval    #PH_ERR_SUCCESS Requested Timer is allocated successfully .
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_RESOURCE_ERROR All the Timers are Busy.
 */
phStatus_t phhalTimer_RequestTimer(phhalTimer_Units_t bUnits,  phhalTimer_Timers_t **ppTimer);


/**
 * This API configures the acquired timer with timeout duration and callback to be called upon timer expiry.
 *
 * @param[in] psTimer Handle to the Timer.
 * @param[in] dwTimePeriod Timeout period.
 * @param[in] pCallBackFunc Call back function on Timer dwTimePeriod timeout.
 * @return Status of the Operation
 * @retval    #PH_ERR_SUCCESS Requested operation is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_INTERNAL_ERROR It is not expected unless malfunction.
 */
#define phhalTimer_Configure(psTimer,  dwTimePeriod, pCallBackFunc)            \
    ((phhalTimer_Configure_t *)	\
    		PH_HAL_TIMER_CONFIGURE_ADDR)\
            (psTimer,  dwTimePeriod, pCallBackFunc)

/**
 * This API Starts the Timer either in Free Running mode or Single Shot mode.
 *
 * @param[in] psTimer Handle to the Timer.
 * @param[in] eRunMode Timer to run in Single shot mode OR free running mode.
 * @return Status of the Operation
 * @retval    #PH_ERR_SUCCESS Requested operation is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 */
#define phhalTimer_Start(psTimer, eRunMode)    \
	((phhalTimer_Start_t *)\
			PH_HAL_TIMER_START_ADDR)\
            (psTimer, eRunMode)

/**
 * This API Stops the specified timer.
 *
 * @param[in] psTimer Handle to the Timer.
 * @return Status of the Operation
 * @retval    #PH_ERR_SUCCESS Requested operation is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 */
#define phhalTimer_Stop(psTimer)             \
	((phhalTimer_Stop_t *)\
			PH_HAL_TIMER_STOP_ADDR)\
            (psTimer)

/**
 * This API Gets the current Timer Count in units.
 * @param[in] psTimer Handle to the Timer.
 * @param[out] pdwTimerCount Updates with the current count of the Timer in units.
 * @return Status of the Operation
 * @retval    #PH_ERR_SUCCESS Requested operation is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 */
#define phhalTimer_GetStatus(psTimer, pdwTimerCount)         \
    ((phhalTimer_GetStatus_t *)\
    		PH_HAL_TIMER_GETSTATUS_ADDR)\
            (psTimer, pdwTimerCount)

/**
 * This API Releases the acquired timer back to the HAL
 * The caller of the API is expected to acquire a mutex before calling this API
 * @param[in] psTimer Handle to the Timer.
 * @return Status of the Operation
 * @retval    #PH_ERR_SUCCESS Requested Timer is released successfully else failure.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 */
phStatus_t phhalTimer_ReleaseTimer(phhalTimer_Timers_t * psTimer);


#endif /* NXPBUILD__PHHAL_TIMER */
/** @} */
#endif
