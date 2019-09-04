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
 * phExNFCCcid_Clif.h: phExCcid Clif based Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

#ifndef PHEXNFCCCID_CLIF_H
#define PHEXNFCCCID_CLIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phhalHw.h"
#include "phExNFCCcid.h"
#include "phRtos.h"
#include "phhalPcr.h"
#include "phFlashBoot_Event.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXNFCCCID_CLIF_RXBUFSIZE 256
#define PH_EXNFCCCID_CLIF_TXBUFSIZE 256

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

typedef enum{
    E_PHEXNFCCCID_CLIF_NONE = 0xC0,
    E_PHEXNFCCCID_CLIF_CMD_START,
    E_PHEXNFCCCID_CLIF_CMD_STOP,
    E_PHEXNFCCCID_CLIF_RSP_END,
    E_PHEXNFCCCID_CLIF_RSP_IDLE
}phExNFCCcid_Clif_MsgId_t;

typedef struct{
    phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry;
} phExNFCCcid_Clif_Disc_t ;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExNFCCcid_Clif_MsgId_t eClifMsgId;
    phExNFCCcid_Clif_Disc_t sClifConfig;
} phExNFCCcid_Clif_Msg_t ;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
#ifndef PHFL_ENABLE_STANDBY
extern phhalTimer_Timers_t *gpphExNFCCcid_Clif_PollTimer;
#endif

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * All the Hardware specific API's will be placed in this file.
 */
phStatus_t phExNFCCcid_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry);
void phExNFCCcid_Clif_HalInit(void);

phStatus_t phExNFCCcidClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExNFCCcidClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib);
uint16_t phExNFCCcidClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
void phExNFCCcidClif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts);

#endif /* PHEXNFCCCID_CLIF_H */
