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
 * phExPos_Clif.h: phExPos Clif based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */

#ifndef PHEXPOS_CLIF_H
#define PHEXPOS_CLIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phhalHw.h"
#include "phExPos.h"
#include "phRtos.h"
#include "phhalPcr.h"
#include "phFlashBoot_Event.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXPOS_CLIF_RXBUFSIZE 256
#define PH_EXPOS_CLIF_TXBUFSIZE 256
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef enum{
    E_PHEXMAIN_CLIF_NONE = 0xC0,
    E_PHEXMAIN_CLIF_CMD_START,
    E_PHEXMAIN_CLIF_CMD_STOP,
    E_PHEXMAIN_CLIF_RSP_END,
    E_PHEXMAIN_CLIF_RSP_IDLE
} phExPos_Clif_MsgId_t;

typedef struct{
    phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry;
} phExPos_Clif_Disc_t ;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExPos_Clif_MsgId_t eClifMsgId;
    phExPos_Clif_Disc_t sClifConfig;
} phExPos_Clif_Msg_t ;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
extern phRtos_QueueHandle_t gphExPos_Clif_Queue;
#ifndef PHFL_ENABLE_STANDBY
extern phhalTimer_Timers_t *gpphExPos_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * All the Hardware specific API's will be placed in this file.
 */

phStatus_t phExPos_ClifMain(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry);
void phExPos_ClifTask(void *pvParams);
phStatus_t phExPos_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
void phExPos_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry, phExPos_Clif_MsgId_t eClifCmd, void *pDes);
void phExPosClif_HalInit(void);
#endif /* PHEXPOS_CLIF_H */
