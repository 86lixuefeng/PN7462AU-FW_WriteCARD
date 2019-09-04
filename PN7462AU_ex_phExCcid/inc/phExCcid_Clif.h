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
 * phExCcid_Clif.h: phExCcid Clif based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */

#ifndef PHEXCCID_CLIF_H
#define PHEXCCID_CLIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phhalHw.h"
#include "phExCcid.h"
#include "phRtos.h"
#include "phhalPcr.h"
#include "phFlashBoot_Event.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXCCID_CLIF_RXBUFSIZE 256
#define PH_EXCCID_CLIF_TXBUFSIZE 256

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef enum{
    E_PHEXCCID_CLIF_NONE = 0xC0,
    E_PHEXCCID_CLIF_CMD_START,
    E_PHEXCCID_CLIF_CMD_STOP,
    E_PHEXCCID_CLIF_RSP_END,
    E_PHEXCCID_CLIF_RSP_IDLE
}phExCcid_Clif_MsgId_t;

typedef struct{
    phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry;
} phExCcid_Clif_Disc_t ;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExCcid_Clif_MsgId_t eClifMsgId;
    phExCcid_Clif_Disc_t sClifConfig;
} phExCcid_Clif_Msg_t ;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
#ifndef PHFL_ENABLE_STANDBY
extern phhalTimer_Timers_t *gpphExCcid_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * All the Hardware specific API's will be placed in this file.
 */

phStatus_t phExCcid_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry);
void phExCcid_Clif_HalInit(void);

phStatus_t phExCcidClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExCcidClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib);
uint16_t phExCcidClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExCcidClif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts);

#endif /* PHEXCCID_CLIF_H */
