/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2016
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
 * phExVCom_PollTypeA.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-10-19 16:08:50 +0530 (Wed, 19 Oct 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18733 $ (v05.19.00)
 */


#ifndef PHEXVCOM_POLLTYPEA_H_
#define PHEXVCOM_POLLTYPEA_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phExVCom.h"
#include "phFlashBoot_Event.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXVCOM_CLIF_RXBUFSIZE        256
#define PH_EXVCOM_CLIF_TXBUFSIZE        256

#define PH_EXVCOM_RF_MAX_NUM_TAG        10
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
typedef enum{
    E_PHEXVCOM_CLIF_NONE = 0xC0,
    E_PHEXVCOM_CLIF_CMD_START,
    E_PHEXVCOM_CLIF_CMD_STOP,
    E_PHEXVCOM_CLIF_RSP_END,
    E_PHEXVCOM_CLIF_RSP_IDLE
}phExVCom_Clif_MsgId_t;

typedef struct{
    /* Queue Mandatory Fields. */
    phFlashBoot_Event_Ids eSrcId;
    void *pvDes;
    /* Individual component items. */
    phExVCom_Clif_MsgId_t eClifMsgId;
} phExVCom_Clif_Msg_t ;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */
extern uint8_t gphExVCom_PollLoopLOGCount; /** Don't log Card Not Detected too much */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
void phExVCom_HalInit(void);
phStatus_t phExVCom_PalInit(void);
phStatus_t phExVCom_PollTypeA(uint32_t* pwNumOfTags);


#endif /* PHEXVCOM_POLLTYPEA_H_ */
