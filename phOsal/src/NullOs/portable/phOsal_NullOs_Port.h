/*
 * phOsal_NullOs_Port.h
 *
 *  Created on: Feb 28, 2017
 *      Author: nxf18375
 */

#ifndef PHOSAL_NULLOS_PORT_H_
#define PHOSAL_NULLOS_PORT_H_

/** \defgroup phOsal_NullOs_Post NullOs_Port layer
* \brief NullOs implementation needs SysTick Timer for its functionality.
* This file defines functions used by the NullOs Implementation.
* User should provide implementation of these in respective platforms .
* @{
*/

/**
*
* \brief Tick Timer ISR callback which will be called by ISR handler implementation in Platform Layer.
* \retval  None
*/
typedef void (*pphOsal_TickTimerISRCallBck_t)(void);

/**
* \brief This function Initializes the Tick Timer.
*
* @param[in]   pTickTimerCallback   Callback required to be called from the Timer-ISR
*
*/
extern phStatus_t phOsal_InitTickTimer(pphOsal_TickTimerISRCallBck_t pTickTimerCallback);

/**
* \brief This function Starts the Tick Timer.
*
* @param[in]   dwTimeMilliSecs  Timer Time duration in Milli-sec.
*
*/
extern phStatus_t phOsal_StartTickTimer(uint32_t dwTimeMilliSecs);

/**
* \brief This function Stops the running Tick Timer.
*
*/
extern phStatus_t phOsal_StopTickTimer(void);

/**
* \brief This function if called, allows, instructions to be executed
* without being interrupted by any other code flow.
*
*/
extern void phOsal_EnterCriticalSection(void);

/**
* \brief This function restors the normal extecution of code flow, allows interrupts
* etc. to occur.
*
*/
extern void phOsal_ExitCriticalSection(void);

/**
* \brief This function put the processor to sleep
*
*/
extern void phOsal_Sleep(void);

/**
* \brief This function wakesup the processor.
*
*/
extern void phOsal_WakeUp(void);

/*@}*/
#endif /* PHOSAL_NULLOS_PORT_H_ */
