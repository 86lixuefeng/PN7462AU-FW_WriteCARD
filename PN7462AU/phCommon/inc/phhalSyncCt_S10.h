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
 *
 * \addtogroup phhalSyncCt_S10
 *
 * The purpose and scope of this file is to implement S10 synchronous card api.
 * This module implements S10 synchronous card Api's in flash memory.
 * This module designed and can be extended to multi-process environment
 * This module contains only the hardware abstraction Api's
 * The user of this module can use these Api's for S10 Synchronous card.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 *
 * @{
 */

#ifndef PHHALSYNCCT_S10_H
#define PHHALSYNCCT_S10_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SYNCCT
#include "ph_Status.h"
#include "phhalSyncCt.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */



/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * This function is used to power up S9 card which consists IP initialization and card activation.
 * @param[in]     bPullupSetStatus         If logic set to 1 Pull up else  pull down config.
 * @param[in]    bCardPresConnectorStatus   If Logic 1 Normally open else normally close config.
 * @param[in]    bSlewRateConfig            To rise from 0 to its selected value (see ct_pcr_reg register), the time
                                                    needed by VCC depends on the capacitance.
 * @param[out]   pbAtrBuffer  Pointer to the ATR buffer where the ATR header bytes will be stored
 *
 * @retval    #PH_ERR_SUCCESS  Activation for S10 card is successful.
 * @retval    #PH_ERR_FAILED   Activation for S10 card is not successful.
 * @maskedret #PH_ERR_SYNC_CT_CARD_ABSENT  If card is absent in main slot.
 */
phStatus_t phhalSyncCt_S10Powerup(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,
                                                                        uint8_t bSlewRateConfig ,uint8_t *pbAtrBuffer);



/**
 * This function is used to process command send from terminal to card.
 * @param[in]   pbDataBuffer   It points to  DataBuffer in which commands are stored.
 * @param[in]   dwOffset  Offset should be fixed before calling this api .
 *                              Offset will indicate towards the location having control,address and data byte.
 * @param[in]   dwByteCount  Number of byte count to be write in card other than control data address byte.
 *
 * @retval PH_ERR_SUCCESS  Command processing(read/write operations)are done successfully.
 * @retval PH_ERR_FAILED  Command processing is not done successfully.
 *
 */
phStatus_t phhalSyncCt_S10_ProcessCommand(uint8_t *pbDataBuffer,uint32_t dwOffset, uint32_t dwByteCount);




#endif /* NXPBUILD__PHHAL_SYNCCT */
/** @} */
#endif /* PHHALSYNCCT_S10_H */
