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
 * phExMainClif.h: phExMain Clif based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-04 17:19:56 +0530 (Fri, 04 Sep 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15087 $ (v05.19.00)
 */

#ifndef PHEXMAIN_CLIF_H
#define PHEXMAIN_CLIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phhalHw.h"
#include "phExMain.h"
#include "phhalPcr.h"
#include "phFlashBoot_Event.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXMAIN_CLIF_RXBUFSIZE 256
#define PH_EXMAIN_CLIF_TXBUFSIZE 256
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef enum{
    E_PHEXMAIN_CLIF_NONE = 0xC0,
    E_PHEXMAIN_CLIF_CMD_START,
    E_PHEXMAIN_CLIF_CMD_STOP,
    E_PHEXMAIN_CLIF_RSP_END,
    E_PHEXMAIN_CLIF_RSP_IDLE
}phExMain_Clif_MsgId_t;

typedef struct{
    phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry;
} phExMain_Clif_Disc_t ;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExMain_Clif_MsgId_t eClifMsgId;
    phExMain_Clif_Disc_t sClifConfig;
} phExMain_Clif_Msg_t ;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
#ifndef PHFL_ENABLE_STANDBY
extern phhalTimer_Timers_t *gpphExMain_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * All the Hardware specific API's will be placed in this file.
 */

phStatus_t phExMain_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry);
void phExMain_Clif_HalInit(void);

phStatus_t phExMainClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExMainClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib);
uint16_t phExMainClif_TestBus(void);
uint16_t phExMainClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExMainClif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts);

#endif /* PHEXMAIN_CLIF_H */
