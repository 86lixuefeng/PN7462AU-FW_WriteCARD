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
 * Holds the event sources and message structure type that will be posted to application.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-09 14:23:35 +0530 (Sat, 09 Jan 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17259 $ (v04.13.00)
 */

#ifndef PHFL_FLASH_BOOT_EVENT_H
#define PHFL_FLASH_BOOT_EVENT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"


/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Event sources through which the events/flags are posted.
 */
typedef enum{
    E_PH_NONE  = 0xA0,          /**< None  */
    E_PH_BOOT,                  /**< Boot  */
    E_PH_CT,                    /**< CT Hal component id. To support asynchronous events. */
    E_PH_CT_TASK,               /**< CT_TASK  */
    E_PH_HIF,                   /**< HIF  */
    E_PH_I2CM,                  /**< I2CM  */
    E_PH_PCR,                   /**< PCR  */
    E_PH_PMU,                   /**< PMU  */
    E_PH_SPIM,                  /**< SPIM  */
    E_PH_USB,                   /**< USB  */
    E_PH_CLIF,                  /**< Clif  */
    E_PH_TIMER,                 /**< Timer  */
    E_PH_SYS,                   /**< System  */
    E_PH_ERROR = -1             /**< Error  */
}phFlashBoot_Event_Ids;


/**
 * Generic message structure for posting the messages To and From using RTOS Queues.
 */
typedef struct phFlashBoot_Event_SysMsg{
    /* Queue Mandatory Fields. */
    /** Indicates the source of the message posted. */
    phFlashBoot_Event_Ids eSrcId ;
    /** Indicates the destination to which the response message to be posted. */
    void *pvDes;
    /** Application data field.
     *
     * SysTask may have messages flowing from all modules with different data streams (like structures, enums etc.).
     * It is better to allocate a generic memory array where all the data streams can fit. To start with, assigned
     * 8 bytes, word aligned. */
    uint32_t dwMsg[2];
}phFlashBoot_Event_SysMsg_t; /**< Boot event */



/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

#endif /* PHFL_FLASH_BOOT_EVENT_H */
