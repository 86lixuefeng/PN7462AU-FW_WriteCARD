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
 * phExPos_Ct.h: phExPos CT based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */


#ifndef PHEXEMVCO_CT_H
#define PHEXEMVCO_CT_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phhalCt.h"
#include "phpalCt.h"
#include "ph_Status.h"
#include "phRtos.h"
#include "phFlashBoot_Event.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Enumeration to different commands for system tasks
 */
typedef enum{
    E_PH_PERFORM_NONE,
    E_PH_PERFORM_CARD_ACT, /**< Perform card activation. */
    E_PH_PERFORM_CARD_DE_ACT /**< Perform card De-activation. */
}phExPos_EventCtCmds_t;


/**
 * Structure required to handle the message queues from ISR to the tasks
 */
typedef struct{

    phFlashBoot_Event_Ids eSrcId ;      /**< Event coming from the source */
    void *pvDes;                        /**< Pointer to the destination */
    phExPos_EventCtCmds_t eCommands;    /**< Command ID's for the destination to take action */
    uint32_t dwMsg[4];                  /**< CT task may have messages flowing from system task and CT module etc.
                                             with different data streams.So its better to allocate a generic memory
                                             array where all the data streams can fit. */
}phExPos_EventCtMsg_t;
/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */
extern phRtos_QueueHandle_t gphExPos_Ct_Queue;

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExPos_Ct_Example_App(void);
void phExPos_CtTask(void *pvParams);
phStatus_t phExPos_CtTaskInit(void *pSysMsgQueue);
void phExPos_CtPostMsg(phExPos_EventCtCmds_t eCtCmd, void *pDes);

#endif /* PHEXEMVCO_CT_H */
