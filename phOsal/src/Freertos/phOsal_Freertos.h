/*
 * phOsal_Freertos.h
 *
 *  Created on: May 12, 2016
 *      Author: nxp69678
 */

#ifndef PHOSAL_FREERTOS_H
#define PHOSAL_FREERTOS_H

#include <FreeRTOS.h>

#define PHOSAL_FREERTOS_ALL_EVENTS      0x00FFFFFF

#define PHOSAL_MAX_DELAY      portMAX_DELAY

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
    #define PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */


/**
 * xPortIsInsideInterrupt is required by FreeRTOS OSAL wrapper. FreeRTOS port provides xPortIsInsideInterrupt definition for
 * Cortex-M3, M4 only. For cortex-M0 xPortIsInsideInterrupt is implemented here.
 */
#if (defined(CORE_M0) || defined(__PN74XXXX__) || defined (__PN73XXXX__))

/*Interrupt Control and State Register*/
#define PHOSAL_NVIC_INT_CTRL        ( (volatile uint32_t *) 0xe000ed04 )
#define PHOSAL_NVIC_VECTACTIVE      (0x0000003FU)

#define xPortIsInsideInterrupt()    (((*(PHOSAL_NVIC_INT_CTRL) & PHOSAL_NVIC_VECTACTIVE ) == 0)? pdFALSE : pdTRUE)

#endif /* (defined(CORE_M0) || defined(__PN74XXXX__) || defined (__PN73XXXX__)) */



#endif /* PHOSAL_FREERTOS_H */
