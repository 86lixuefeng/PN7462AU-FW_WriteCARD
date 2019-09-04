/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * phExCcid_Ct.h: phExCcid CT based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */


#ifndef PHEXCCID_CT_H
#define PHEXCCID_CT_H

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
}phExCcid_Ct_EventCmds_t;


/**
 * Structure required to handle the message queues from ISR to the tasks
 */
typedef struct{

    phFlashBoot_Event_Ids eSrcId ;      /**< Event coming from the source */
    void *pvDes;                        /**< Pointer to the destination */
    phExCcid_Ct_EventCmds_t eCommands;    /**< Command ID's for the destination to take action */
    uint32_t dwMsg;                  /**< CT task may have messages flowing from system task and CT module etc.
                                             with different data streams.So its better to allocate a generic memory
                                             array where all the data streams can fit. */
}phExCcid_Ct_EventMsg_t;
/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExCcid_Ct_Example_App(void);

#endif /* PHEXCCID_CT_H */
