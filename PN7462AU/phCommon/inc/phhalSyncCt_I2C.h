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
 * \addtogroup phhalSyncCt_I2C
 *
 * The purpose and scope of this file is to implement SYNC CT HAL, Prototype of the Sync CT Interface HAL
 * This module implements contact interface Api's for synchronous profile in flash memory.
 * Contact ip can be used either in synchronous mode or asynchronous mode.
 * This module designed and can be extended to multi-process environment
 * This module contains only the hardware abstraction Api's
 * The user of this module can use these Api's for contact protocol(I2C) development
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-04-07 16:11:35 +0530 (Thu, 07 Apr 2016) $
 * $Author: Stevenson James (NXP66090) $
 * $Revision: 18223 $ (v04.13.00)
 *
 * @{
 */

#ifndef PHHALSYNCCT_I2C_H
#define PHHALSYNCCT_I2C_H

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
/** Enums for different stages for I2C Communication.*/
typedef enum
{
    E_STATE_0,
    E_STATE_1,
    E_STATE_2,
    E_STATE_3,
    E_LAST_STATE
} phhalSyncCt_I2C_StateType_t;

/** \addtogroup grpErr_HalSyncCT
 * @{
 */
/**
 * These enums are used by I2C Sync CT HAL APIs for indicating different error status codes.
 */
typedef enum {
    /** When provided Buffer is smaller than maximum possible receive size*/
    PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL  = (PH_ERR_CUSTOM_BEGIN+0x0021)
}phhalSyncCt_I2C_ErrorCodeType_t;
/** @} */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * This function is used to power up I2C card which consists IP initialization and card activation.
 * @pre The I2C card must be present in the slot before calling this API. phhalSyncCt_CheckCardPres() can be used to
 * check pre-condition.
 *
 * @param[in]    bPullupSetStatus          If logic set to 1 Pull up else  pull down config.
 * @param[in]    bCardPresConnectorStatus  If Logic 1 Normally open else normally close config.
 * @param[in]    bSlewRateConfig           To rise from 0 to its selected value (see ct_pcr_reg register), the time
                                                    needed by VCC depends on the capacitance.
 *
 * @retval    #PH_ERR_SUCCESS  Always.
 */
phStatus_t phhalSyncCt_I2CPowerup(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,
                                  uint8_t bSlewRateConfig);

/**
 * This function is used to request or receive data from I2C card which works in Extended mode
 * and can get data from random address location specified in user data buffer.
 *
 * @param[in]    bI2CExtended        It should be 1 to use I2C in extended mode.
 * @param[out]    pbReceiveBuf   Pointer to Receive Buffer in which bytes will store from card.
 * @param[in]    pbDataBuffer   Pointer to User data buffer in which Slave address ,Word address information and number of
 *                          bytes to read information should be stored in given manner :
 *                          - Slave address    = pbDataBuffer[offset + 0].
 *                          - Word address msb = pbDataBuffer[offset + 1].
 *                          - Word address lsb = pbDataBuffer[offset + 2].
 *                          - Data count msb      = pbDataBuffer[offset + 3].
 *                          - Data count lsb      = pbDataBuffer[offset + 4].
 *
 *
 * @retval    #PH_ERR_SUCCESS  If All data is read successfully.
 * @retval    #PH_ERR_FAILED   If All data is not read successfully.
 * @maskedret  #PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL  If Number of bytes to Read is more than 512.
 */
phStatus_t phhalSyncCt_I2CExtendedRequestData(uint8_t bI2CExtended ,uint8_t *pbReceiveBuf ,uint8_t *pbDataBuffer);



/**
 * This function is used to request or receive data from I2C card which does not work in extended mode
 * and can get data from random address location specified in user data buffer.
 *
 * @param[out]   pbReceiveBuf   Pointer to Receive Buffer in which bytes will store from card.
 * @param[in]    pbDataBuffer   Pointer to User data buffer in which Slave address and  number of bytes to
 *                          read information should be stored in given manner :
 *                          - Slave address = pbDataBuffer[offset + 0].
 *                          - Word address  = pbDataBuffer[offset + 1].
 *                          - Data count    = pbDataBuffer[offset + 2].
 *
 * @retval    #PH_ERR_SUCCESS  If All data is read successfully.
 * @retval    #PH_ERR_FAILED   If All data is not read successfully.
 * @maskedret  #PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL  If Number of bytes to Read is more than 512.
 */
phStatus_t phhalSyncCt_I2CRequestData(uint8_t *pbReceiveBuf ,uint8_t *pbDataBuffer);


/**
 * This function is used to Send data to I2C card.
 * @param[in]     pbDataBuffer  pointer to data buffer which should consist slave address, word address, number of payload data bytes
 *                              and payload data bytes to  write.
 * @param[in]     dwDataNumber  Number of data bytes to send to card.
 *
 * @retval    #PH_ERR_SUCCESS  If Data is send successfully to card.
 * @retval    #PH_ERR_SUCCESS  If Data is  not send successfully to card.
 * @maskedret  #PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL   If Number of bytes to write is more than 512.
 */
phStatus_t phhalSyncCt_I2CSendData(uint8_t *pbDataBuffer ,uint32_t dwDataNumber);



/** @} */
#endif /* NXPBUILD__PHHAL_SYNCCT */
#endif /* PHHALSYNCCT_I2C_H */
