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
 * \addtogroup phhalSyncCt_S9
 *
 * The purpose and scope of this file is to implement S9 Synchronous card Apis.
 * This module implements S9 synchronous card Api's in flash memory.
 * This module designed and can be extended to multi-process environment
 * The user of this module can use these Api's for contact protocol(T=0 or T=1) development
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-09 14:23:35 +0530 (Sat, 09 Jan 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17259 $ (v04.13.00)
 *
 * @{
 */

#ifndef PHHALSYNCCT_S9_H
#define PHHALSYNCCT_S9_H

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
/** \addtogroup grpErr_HalSyncCT
 * @{
 */
/**
 * Enums for Returned Error code for S9 profile card.
 */
typedef enum {
    /** \b 0x91 */ PH_ERR_SYNC_CT_BUFFER_TOO_SMALL  = (PH_ERR_CUSTOM_BEGIN+0x0011), /**< When Buffer size is smaller than                                                                            maximum possible recieve size.*/
    /** \b 0x92 */ PH_ERR_SYNC_CT_WRITING_WITHOUT_PIN_VERIFIED,/**< When User wants to write in card without verifying pin.*/
    /** \b 0x93 */ PH_ERR_SYNC_CT_PROTECTED_BYTE, /**< When User want to change some protected byte.*/
    /** \b 0x94 */ PH_ERR_SYNC_CT_WRITE_ERROR, /**< When writing in card is not done successfully.*/
    /** \b 0x95 */ PH_ERR_SYNC_CT_PIN_NOT_VERIFIED, /**< Pin is not verified.*/
    /** \b 0x96 */ PH_ERR_SYNC_CT_ERROR_COUNTER_PROTECTED, /**< Pin verification retry count has been locked.*/
    /** \b 0x97 */ PH_ERR_SYNC_CT_PIN_CODE_ERROR, /**< Provided Pin is incorrect.*/
    /** \b 0x98 */ PH_ERR_SYNC_CT_PROTECTED_BIT_ALREADY_SET /**< Protected bit is already set */
}phhalSyncCt_S9_ErrorCodes_t;
/** @} */
/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * This function is used to power up S9 card which consists IP initialization and card activation.
 * @param[in]      bPullupSetStatus         If logic set to 1 Pull up else  pull down config.
 * @param[in]      bCardPresConnectorStatus   If Logic 1 Normally open else normally close config.
 * @param[in]      bSlewRateConfig            To rise from 0 to its selected value (see ct_pcr_reg register), the time
 * @param[out]      pbAtrBuffer  Pointer to the ATR buffer where the ATR header bytes will be stored
 *
 * @retval    #PH_ERR_SUCCESS  Activation for S9 card is successful.
 *            #PH_ERR_FAILED   Activation for S9 card is not successful.
 * @maskedret # PH_ERR_SYNC_CT_CARD_ABSENT  If card is absent in main slot.
 */
phStatus_t phhalSyncCt_S9Powerup(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,
                                                                uint8_t bSlewRateConfig,uint8_t *pbAtrBuffer);


/**
 * This function is used to read data from card in 8 bit mode(unprotected mode).
 * @param[in]  dwCardAddress   card Address from where bits are needed to read.
 * @param[in] dwByteCount   Number of byte count to read.
 * @param[out] pbDataBuffer  Pointer to the buffer where the bytes will be stored,Size of this buffer should be 512 bytes.
 *
 * @retval    #PH_ERR_SUCCESS  If given number of bytes have been read successfully.
 * @maskedret #PH_ERR_SYNC_CT_BUFFER_TOO_SMALL  If dwByteCount is greater than 512(maximum number byte to read).
 */
phStatus_t phhalSyncCt_S9_Read8bits(uint32_t dwCardAddress, uint32_t dwByteCount ,uint8_t *pbDataBuffer);


/**
 * This function is used to read data from card in 9 bit mode(protected mode).
 * @param[in]  dwCardAddress   card Address from where bits are needed to read.
 * @param[in]  dwByteCount   Number of byte count to read.
 * @param[out]  pbDataBuffer  Pointer to the buffer where the bytes will be stored,Size of this buffer should be 512 bytes.
 *
 * @retval    #PH_ERR_SUCCESS  If given number of bytes have been read successfully.
 *@maskedret  #PH_ERR_SYNC_CT_BUFFER_TOO_SMALL  If dwByteCount is greater than 512(maximum number byte to read).
 */
phStatus_t phhalSyncCt_S9_Read9bits(uint32_t dwCardAddress, uint32_t dwByteCount ,uint8_t *pbDataBuffer);


/**
 * This function is used to write data from user buffer to card address.
 * @param[in]  bWriteType  It indicates for what mode we want to write in card.
 *                          - 0x31   For Write in  9 bit mode (protected mode)
 *                          - 0x33   For Write in  8 bit mode (unprotected mode)
 *
 * @param[in]  dwOffset  offset in user buffer from where bytes are needed to start writing in card.
 * @param[in]  dwCardAddress   card Address where bits are needed to write.
 * @param[in]  dwByteCount   Number of byte count to write.
 * @param[in]  pbDataBuffer  Pointer to the user data  buffer.
 *
 * @retval    #PH_ERR_SUCCESS  If given number of bytes have been write successfully.
 * @maskedret #PH_ERR_SYNC_CT_WRITING_WITHOUT_PIN_VERIFIED  If writing is attempted without pin is verified.
 * @maskedret #PH_ERR_SYNC_CT_PROTECTED_BYTE  If writing at protected byte place.
 * @maskedret #PH_ERR_SYNC_CT_WRITE_ERROR   If given number of bytes have not been write successfully.
 */
phStatus_t phhalSyncCt_S9_Write(uint8_t bWriteType, uint32_t dwOffset,
                                uint32_t dwCardAddress, uint32_t dwByteCount , uint8_t *pbDataBuffer);
/**
 *This function is used to verify pin code.
 *@param[in]  pbPinCodeBuffer - This points to pin code buffer having pin code of 2 byte length for card.
 *@retval     #PH_ERR_SUCCESS  If pin is verified.
 *@maskedret  #PH_ERR_SYNC_CT_PIN_NOT_VERIFIED   If pin is not verified.
 *@maskedret  #PH_ERR_SYNC_CT_ERROR_COUNTER_PROTECTED  If number for retry count for pin is finished.
 *@maskedret  #PH_ERR_SYNC_CT_PIN_CODE_ERROR If pin is wrong.
 */

phStatus_t phhalSyncCt_S9_VerifyPin(uint8_t *pbPinCodeBuffer);


/**
 * This function is used to compare byte with card location to user buffer location.
 * @param[in]   dwCardAddress  Address of byte to compare in Card.
 * @param[in]  dwOffset   Offset of byte to be compare in user data buffer.
 * @param[in]  pbDataBuffer   Pointer to user data buffer
 *
 * @retval PH_ERR_SUCCESS  If compare has done successfully
 * @maskedret  PH_ERR_SYNC_CT_PROTECTED_BIT_ALREADY_SET  when byte in card is protected.
 * @maskedret  PH_ERR_SYNC_CT_PIN_NOT_VERIFIED   Pin is not verified so comparison can not take place.
 *
 */
phStatus_t phhalSyncCt_S9_Compare(uint32_t dwCardAddress,uint32_t dwOffset ,uint8_t *pbDataBuffer);



#endif /* PHHALSYNCCT_S9_H */
/** @} */
#endif /* NXPBUILD__PHHAL_SYNCCT */
