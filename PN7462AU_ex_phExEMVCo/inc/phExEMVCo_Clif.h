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
 * phExEMVCo_Clif.h: phExEMVCo Clif based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-07 10:18:21 +0530 (Mon, 07 Sep 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15113 $ (v05.19.00)
 */

#ifndef PHEXEMVCO_CLIF_H
#define PHEXEMVCO_CLIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phhalHw.h"
#include "phExEMVCo.h"
#include "phRtos.h"
#include "phhalPcr.h"
#include "phFlashBoot_Event.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXEMVCO_CLIF_RXBUFSIZE 256
#define PH_EXEMVCO_CLIF_TXBUFSIZE 256
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef enum{
    E_PHEXMAIN_CLIF_NONE = 0xC0,
    E_PHEXMAIN_CLIF_CMD_START,
    E_PHEXMAIN_CLIF_CMD_STOP,
    E_PHEXMAIN_CLIF_RSP_END,
    E_PHEXMAIN_CLIF_RSP_IDLE
} phExEMVCo_Clif_MsgId_t;

typedef struct{
    phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry;
} phExEMVCo_Clif_Disc_t ;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExEMVCo_Clif_MsgId_t eClifMsgId;
    phExEMVCo_Clif_Disc_t sClifConfig;
} phExEMVCo_Clif_Msg_t ;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
extern phRtos_QueueHandle_t gphExEMVCo_Clif_Queue;
#ifndef PHFL_ENABLE_STANDBY
extern phhalTimer_Timers_t *gpphExEMVCo_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * All the Hardware specific API's will be placed in this file.
 */

phStatus_t phExEMVCo_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
                              phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry);
void phExEMVCo_ClifTask(void *pvParams);
phStatus_t phExEMVCo_ClifTaskInit(phhalPcr_BootReason_t eBootReason, void *pDes);
void phExEMVCo_ClifPostMsg(phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry, phExEMVCo_Clif_MsgId_t eClifCmd, void *pDes);

phStatus_t phExEMVCoClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExEMVCoClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib);
void phExEMVCoClif_HalInit(void);
uint16_t phExEMVCoClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,uint8_t* pbAts);
void phExEMVCoEmvcoRfReset(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXEMVCO_CLIF_H */
