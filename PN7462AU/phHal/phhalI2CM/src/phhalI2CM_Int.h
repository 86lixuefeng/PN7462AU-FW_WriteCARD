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
 * Internal declarations and macro definitions of I2CM hal.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALI2CM_INT_H
#define PHHALI2CM_INT_H

/** \addtogroup phhalI2CM I2C Master hardware abstraction layer.
 *
 * The I2C Master's INTERNAL APIs
 *
 * This module implements I2CM interface Flash code HAL, specific to I2CM module.
 * This module is not designed for multi thread usage.
 *  @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM
#include "PN7462AU/PN7462AU_i2cm.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PHHAL_I2CM_MAXIMUM_FIFO_SIZE                    (32)    /**< FIFO has a word(4 bytes) depth of 8. */
#define PHHAL_I2CM_NUMBER_OF_WORDS_IN_FIFO              (8)     /**< I2C Master has 8 FIFO's. */
#define PHHAL_I2CM_WORD_SIZE                            (4)     /**< Word size is 4 bytes*/
#define PHHAL_I2CM_DOUBLE_WORD_SIZE                     (8)     /**< Word size is 8 bytes*/
#define PHHAL_I2CM_QUAD_WORD_SIZE                       (16)    /**< Word size is 16 bytes*/
#define PHHAL_I2CM_TX_FIFO_THRES                        (0x02)  /**< Optimum Tx Threshold*/
#define PHHAL_I2CM_RX_FIFO_THRES                        (0x06)  /**< Optimum Rx Threshold*/

#ifdef NXPBUILD__PHHAL_I2CM_MULTI_TRANSACTION
#define PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS                8
#else
#define PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS                1
#endif

/**
 *  Macro to reset the I2C Core Block
 */
#define PHHAL_I2CM_RESET()        PH_REG_SET_BIT(I2CMASTER_CONFIG_REG, RESET_I2C_CORE);    \
        PH_REG_SET_BIT(I2CMASTER_CONFIG_REG, RESET_REG);

/**
 *  Macro to set Master Mode of operation Tx/Rx. 0-> indicates that the I2CM is in Master Tx mode. 1 -> Rx mode
 */
#define PHHAL_I2CM_SET_MODE(mode)            PH_REG_SET_FIELD_NS(I2CMASTER_CONFIG_REG, IP_MODE, mode)

/**
 *  Macro to set I2CM slave address type. 1 -> 10 bit addressing, type == 0 -> 7 bit addressing.
 */
#define PHHAL_I2CM_ADDR_TYPE(type)           PH_REG_SET_FIELD(I2CMASTER_CONFIG_REG, SLV_ADDRESSING, type)

/**
 *  Macro to set the I2C Slave 7 bit or 10 bit address.
 */
#define PHHAL_I2CM_SET_ADDR(addr)            PH_REG_SET_FIELD_NS(I2CMASTER_I2C_ADDRESS_REG, SLAVE_ADDRESS, addr)

/**
 *  Macro to set the required 8-bit Pre scale/Baud rate value.
 */
#define PHHAL_I2CM_SET_BAUD_RATE(baudRate)   PH_REG_SET_FIELD_NS(I2CMASTER_BAUDRATE_REG, BAUDRATE, baudRate)

/**
 *  Macro to set the minimum hold time of the SDA data generated on I2C sda bus.
 */
#define PHHAL_I2CM_SET_HOLD_TIME(holdTime)   PH_REG_SET_FIELD_NS(I2CMASTER_SDA_HOLD_REG, SDA_HOLD, holdTime)

/**
 *  Macro to get configured Master Mode of operation Tx/Rx. 0-> indicates that I2C Master is in Tx mode. 1 -> Rx mode.
 */
#define PHHAL_I2CM_GET_MODE()                PH_REG_GET_FIELD_NS(I2CMASTER_CONFIG_REG, IP_MODE)

/**
 *  Macro to get the configured I2C Slave bit address.
 */
#define PHHAL_I2CM_GET_ADDR()                PH_REG_GET_FIELD_NS(I2CMASTER_I2C_ADDRESS_REG, SLAVE_ADDRESS)

/**
 *  Re-enable the I2C master such that the transfer continuous upon Errors.
 */
#define PHHAL_I2CM_CONT_TRANS()              PH_REG_SET(I2CMASTER_CONTROL_REG, I2CMASTER_CONTROL_REG_I2C_ENABLE_MASK)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Holds I2CM transaction details
 */
typedef struct
{
    uint32_t *pdwData;    /**<Pointer to the data to be transmitted/received. Should be 4 byte aligned.  */
    uint16_t wDataLength; /**<Number of bytes to be transmitted or received. */
    uint16_t wSlaveAddr; /**<Number of bytes to be received in TransReceiver Operation. */
    uint16_t bWordCount; /**<Word count for the transaction. Internal use. */
    uint8_t bRetryCount; /**<I2CM HAL will handle transaction error (NAK, Bus Error, Arb error) with automatic
                            continuation up to retry count. After transaction completion if the Retry count is
                            Less than Set value; it indicates error occurred during I2C Transaction. */
} phhalI2CM_Transaction_t;

/**
 * I2CM operation modes.
 */
typedef enum
{
    E_I2CM_TX_MODE = 0,  /**<I2C master transactions mode-> Transmission*/
    E_I2CM_RX_MODE       /**<I2C master transactions mode-> reception*/
} phhalI2CM_Mode_Options_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * I2CM Hal Interrupt service routine.
 *
 * This API is part the Vector Table and directly called in the event of I2CM ISR.
 */
void I2CM_IRQHandler(void);
#endif /* NXPBUILD__PHHAL_I2CM */

#endif /* PHHALI2CM_INT_H */
