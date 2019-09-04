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
 * phExDoorAccessClif.h: phExDoorAccess Clif based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */

#ifndef PHEXDOORACCESS_CLIF_H
#define PHEXDOORACCESS_CLIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <phExDoorAccess.h>
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phhalHw.h"
#include "phhalPcr.h"
#include "phFlashBoot_Event.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXDOORACCESS_CLIF_RXBUFSIZE 256
#define PH_EXDOORACCESS_CLIF_TXBUFSIZE 256
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef enum{
    E_PHEXDOORACCESS_CLIF_NONE = 0xC0,
    E_PHEXDOORACCESS_CLIF_CMD_START,
    E_PHEXDOORACCESS_CLIF_CMD_STOP,
    E_PHEXDOORACCESS_CLIF_RSP_END,
    E_PHEXDOORACCESS_CLIF_RSP_IDLE
}phExDoorAccess_Clif_MsgId_t;

typedef struct{
    phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry;
} phExDoorAccess_Clif_Disc_t ;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExDoorAccess_Clif_MsgId_t eClifMsgId;
    phExDoorAccess_Clif_Disc_t sClifConfig;
} phExDoorAccess_Clif_Msg_t ;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
#ifndef PHFL_ENABLE_STANDBY
extern phhalTimer_Timers_t *gpphExDoorAccess_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * All the Hardware specific API's will be placed in this file.
 */

phStatus_t phExDoorAccess_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry);
void phExDoorAccess_Clif_HalInit(void);

phStatus_t phExDoorAccess_Clif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExDoorAccess_Clif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib);
uint16_t phExDoorAccess_Clif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExDoorAccess_Clif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts);
#endif /* PHEXDOORACCESS_CLIF_H */
