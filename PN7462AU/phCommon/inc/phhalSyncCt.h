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
 * Project:  PN7462AU
 *
 * $Date: 2015-09-23 17:59:43 +0530 (Wed, 23 Sep 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15442 $ (v04.13.00)
 *
 */

#ifndef PHHALSYNCCT_H
#define PHHALSYNCCT_H
/**
 * \addtogroup phhalSyncCt
 *
 * The Synchornous CT (HAL) APIs
 *
 * The purpose and scope of this file is to implement SYNC CT HAL, Prototype of the Sync CT Interface HAL
 * This module implements contact interface Api's for synchronous profile in flash memory.
 * Contact ip can be used either in synchronous mode or asynchronous mode.
 * This module designed and can be extended to multi-process environment
 * This module contains only the hardware abstraction Api's
 * The user of this module can use these Api's for contact protocol(S9 and S10) development
 *
 * @{
 *
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SYNCCT
#include "ph_Status.h"
#include "phUser.h"
#include "PN7462AU/PN7462AU_ctip.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/** Enums for Selecting activation type for card.*/
typedef enum
{
    /** \b 0x00 */ E_TYPE1,       /**< Synchronous Type 1 card.*/
    /** \b 0x01 */ E_TYPE2,       /**< Synchronous Type 2 card.*/
    /** \b 0x02 */ E_INVALID_TYPE /**< Invalid Enum.*/
}phhalSyncCt_ActivationType_t;

/** \addtogroup grpErr_HalSyncCT
 *
 * Error codes for Synchronous CT HAL
 *
 * @{
 */
/**
 *  These enums are used by Sync Ct HAL Apis for indicating different error status codes.
 */
typedef enum {

    /** \b 0x81 */  PH_ERR_SYNC_CT_CARD_ABSENT = (PH_ERR_CUSTOM_BEGIN+0x0001),      /**< Main slot card absent */
    /** \b 0x82 */  PH_ERR_SYNC_CT_CARD_PRESENT,      /**<  Main slot card present.*/
    /** \b 0x83 */  PH_ERR_SYNC_CT_ACTIVATION_FAILED, /**<  Card Activation is not successful.*/
    /** \b 0x84 */  PH_ERR_SYNC_CT_HAL_INVALID        /**< Invalid enumeration */
}phhalSyncCt_ErrorCodes_t;

/** @} */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * This function is used to initialize the CT IP for synchronus card profile.
 * @param[in]      bPullupSetStatus        If logic set to 1 Pull up else  pull down config.
 * @param[in]      bCardPresConnectorStatus  If Logic 1 Normally open else normally close config.
 * @param[in]      bSlewRateConfig           To rise from 0 to its selected value (see ct_pcr_reg register), the time
                                                    needed by VCC depends on the capacitance.
 * @retval    #PH_ERR_SUCCESS  Initialization of CT IP is successful.
 * @retval    #PH_ERR_FAILED   Initialization of CT IP failed.
 */
phStatus_t phhalSyncCt_Init(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,uint8_t bSlewRateConfig);

/**
 * This function is used to check if card is present in the main slot.
 * @maskedret #PH_ERR_SYNC_CT_CARD_PRESENT  If Main slot is chosen and card is present.
 * @maskedret #PH_ERR_SYNC_CT_CARD_ABSENT   If Main slot is chosen and card is absent.
 */
phStatus_t phhalSyncCt_CheckCardPres(void);

/**
  *
  * This function is used to performs activation.Configures the Setting according to Type 1 first if not successful then
  * according to Type 2.If success, returns the ATR to the user.
  *
  * @param[out]   pbAtrBuffer   Pointer to the ATR buffer where the ATR header bytes will be stored
  * @param[in]  eActivationType   Enum for Specific Class Activation.
  *          - E_TYPE1    Activate with type 1 operating conditions.
  *          - E_TYPE2    Activate with type 2 operating conditions.
  *
  * @retval    #PH_ERR_SUCCESS   Activation successful and ATR bytes are received successfully
  * @retval    #PH_ERR_FAILED  Activation not successful.
  * @maskedret #PH_ERR_SYNC_CT_CARD_ABSENT  Card is absent in the slot.
  * @maskedret #PH_ERR_SYNC_CT_CARD_PRESENT  Card is present in slot.
  */
phStatus_t phhalSyncCt_CardActivate(uint8_t* pbAtrBuffer , phhalSyncCt_ActivationType_t eActivationType);

/**
 * This function sets the clock state according to given value.
 * @param[in]  bClockValue  When set to 1, sets clock at high, When set to 0 sets clock at low.
 * @retval    #PH_ERR_SUCCESS  Clock is stopped or started successfully
 */
phStatus_t phhalSyncCt_SetClock(uint8_t bClockValue);
/**
  *
  * This function is used to get bit from card.
  * @param[out]    pbReceivedbit    point to received bit from card.
  *
  * @retval    #PH_ERR_SUCCESS  If bit is received successfully.
  *
  */
phStatus_t phhalSyncCt_GetSyncBit(uint8_t *pbReceivedbit);


/**
  * This function is used to set bit in uart for sending it to card.
  * @param[in]     bTransmitBit   It indicates the bit to transmit.
  * @retval    #PH_ERR_SUCCESS  If bit is set successfully.
  */
phStatus_t phhalSyncCt_SetSyncBit(uint8_t bTransmitBit);


/**
  * This function is used to set transmission or reception mode for uart.
  * @param[in]   bValue  If set to logic 1 ,transmit mode and if set to logic 0 ,Reception mode.
  * @retval  #PH_ERR_SUCCESS  If transmit or receive mode is set successfully.
  *
  */

phStatus_t  phhalSyncCt_SetTRMode(uint8_t bValue);


/**
  * This function is used to Receive byte from card.
  * @param[out] pbReceiveByte   Pointer to Byte received from card.
  * @retval #PH_ERR_SUCCESS   Byte have been received successfully
*/
phStatus_t  phhalSyncCt_ReceiveByte(uint8_t *pbReceiveByte);


/**
  * This function is used to send byte  to card.
  * @param[in]  bSendByte  It indicates the byte to transmit.
  * @retval #PH_ERR_SUCCESS   Bytes have been transmitted successfully
*/
phStatus_t phhalSyncCt_TransmitByte(uint8_t bSendByte);



/**
 *This function is used to set value for reset contact pin  for contact card.
 *@param[in]  bValue   It indicates value to be set for reset pin.
 *@retval #PH_ERR_SUCCESS  If CTIP is reset successfully.
 */
phStatus_t phhalSyncCt_SetReset(uint8_t bValue);


/**
  * This function is used to Send n number of  Clock cycles to card.
  * @param[in]  dwCount   It indicates the number of clock cycle to be send.
  * @retval #PH_ERR_SUCCESS If N number of clocks are send successfully.
 */
phStatus_t phhalSyncCt_Send_Nbclocks(uint32_t dwCount);


/**
  * This function is used to deactivate the card.
  *
  * @retval    #PH_ERR_SUCCESS  Card is deactivated successfully
  */
phStatus_t phhalSyncCt_CardDeactivate(void);


/**
  * This function is used to reset CT-IP for synchronous card profile.
  * @retval    #PH_ERR_SUCCESS  If IP is reset successfully.
  */

phStatus_t phhalSyncCt_Reset(void);


/**
 * This function does the DeInitialization of the CT IP for synchronus card profile.
 * @retval    #PH_ERR_SUCCESS  De-Initialization operation completed successfully.
 * @retval    #PH_ERR_FAILED  De-Initialization failed
 */
phStatus_t phhalSyncCt_DeInit(void);




#endif /* NXPBUILD__PHHAL_SYNCCT */
/** @} */
#endif /* PHHALSYNCCT_H */
