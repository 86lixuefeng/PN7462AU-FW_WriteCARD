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
 * This module briefs all the prototypes of SPIM HAL.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALSPIM_H
#define PHHALSPIM_H

/** \addtogroup phhalSPIM
 *
 * The SPI Master APIs
 *
 * This module implements SPI interface Flash code HAL, specific to SPIM module.
 * This module is not designed for multi thread usage.
 *
 * @warning
 *
 *  If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *  checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *  and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *  failures.
 *
 *  @{
 *
 *
 *
 * \dot
 *      digraph "Sample Usage"{
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalSPIM_Configure [URL ="\ref phhalSPIM_Configure"];
 *          phhalSPIM_DeInit [URL ="\ref phhalSPIM_DeInit"];
 *          phhalSPIM_Init [URL ="\ref phhalSPIM_Init"];
 *          phhalSPIM_Receive [URL ="\ref phhalSPIM_Receive"];
 *          phhalSPIM_ReceiveContinue [URL ="\ref phhalSPIM_ReceiveContinue"];
 *          phhalSPIM_SetWaterLevel [URL ="\ref phhalSPIM_SetWaterLevel"];
 *          phhalSPIM_Transmit [URL ="\ref phhalSPIM_Transmit"];
 *          phhalSPIM_TransmitContinue [URL ="\ref phhalSPIM_TransmitContinue"];
 *
 *      //Logical Flow
 *          Start ->phhalSPIM_Init -> phhalSPIM_Configure;
 *          phhalSPIM_Configure -> phhalSPIM_Receive;
 *          phhalSPIM_Configure -> phhalSPIM_Transmit;
 *          phhalSPIM_Receive -> phhalSPIM_DeInit;
 *          phhalSPIM_Transmit -> phhalSPIM_DeInit;
 *          phhalSPIM_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *          phhalSPIM_Configure -> phhalSPIM_SetWaterLevel;
 *          phhalSPIM_SetWaterLevel -> phhalSPIM_Receive;
 *          phhalSPIM_SetWaterLevel -> phhalSPIM_Transmit;
 *          phhalSPIM_Receive -> phhalSPIM_ReceiveContinue -> phhalSPIM_DeInit;
 *          phhalSPIM_Transmit -> phhalSPIM_TransmitContinue -> phhalSPIM_DeInit;
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *          phhalSPIM_Reset -> End;
 *
 *      }
 * \enddot
 *
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SPIM
#include "ph_Status.h"


/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/** \verbatim
    MAX allowed baud rate define.
    Maximum baud rate possible for 0x07 is 6.78 MHz
    Bit values for baud rate vary from 0 to 7, see below for details.
    Bit         Baud Rate         Duty Cycle
    Value(b)     @27.12MHz          in %
    ======================================
    000           1.00              50.00
    001           1.51              50.00
    010           2.09              50.00
    011           2.47              50.00
    100           3.01              50.00
    101           4.52              50.00
    110           5.42              50.00
    111           6.78              50.00
    \endverbatim */
#define PHHAL_SPIM_MAX_BAUDRATE_VALUE    (7)

/**
 * MAX number of bytes allowed in a transaction.
 * Maximum packet size excluding CRC.
 */
#define PHHAL_SPIM_MAX_PAYLOAD_SIZE      (511)

/**
 * Maximum number of SPIM slaves supported by PN7462 is 2.
 */
#define PHHAL_SPIM_MAX_NO_SLAVES         (2)
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
 /**
  *  phhalSPIM_Modes_t specifies the SPIM mode of operation.
  */
typedef enum
{
    /** \b 0x0000 */    E_SPIM_MODE0 = 0,         /**< Mode = 0   CPOL = 0   CPHA = 0 */
    /** \b 0x0001 */    E_SPIM_MODE1,             /**< Mode = 1   CPOL = 0   CPHA = 1 */
    /** \b 0x0002 */    E_SPIM_MODE2,             /**< Mode = 2   CPOL = 1   CPHA = 0 */
    /** \b 0x0003 */    E_SPIM_MODE3              /**< Mode = 3   CPOL = 1   CPHA = 1 */
} phhalSPIM_Modes_t;

/** \addtogroup grpErr_HalSPIM
 *
 * Error codes for SPI Master HAL
 *
 * @{
 */
/**
 * SPI Master specific errors.
 */
typedef enum
{
    /** \b 0x81 */    PH_ERR_AHB    =    (PH_ERR_CUSTOM_BEGIN+1),     /**< AHB Error */
    /** \b 0x82 */    PH_ERR_AHB_ADDR ,                               /**< AHB Address Error */
    /** \b 0x83 */    PH_ERR_TXRX_ONGOING,                            /**< TXRX Ongoing */
    /** \b 0x84 */    PH_ERR_NOT_CONFIGURED,                          /**< SPIM Not Configured */
    /** \b 0x85 */    PH_ERR_INVALID_BUFFER_ADDRESS,                  /**< Invalid Buffer Address */
    /** \b 0x86 */    PH_ERR_SPIM_FREE                                /**< SPIM is Free */
}phhalSPIM_ErrorCodes_t;
/** @} */

/**
 *  Callback Function pointer definition which will be called from Isr
 */
typedef void (*pphhalSPIM_CallbackFun_t)(void);

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * This API initializes HAL context and enables SPIM Clock and Interrupt
 * @note This API needs to be called before using SPIM functions
 * @param[in] dwSpimTimeOut TimeOut parameter for SPI Transactions (For Tx & Rx) related operations.
 * @retval    #PH_ERR_SUCCESS if initialization is completed successfully.
 * @maskedret #PH_ERR_RESOURCE_ERROR - In RTOS environments, If the Event creation failed.
 */
phStatus_t phhalSPIM_Init(uint32_t dwSpimTimeOut);


/**
 * This API de-initializes HAL context and disables SPIM Clock and Interrupt
 * @note No other SPIM HAL APIs except phhalSPIM_Init shall be called after this API
 * @retval #PH_ERR_SUCCESS Operation completed successfully.
 */
phStatus_t phhalSPIM_DeInit(void);


/**
 * This API configures the SPI master for subsequent transactions
 * @param[in] bSlaveSelect Slave select, if false, slave 0 is selected, if true, slave 1 is selected.(value is either
 *                         ZERO or ONE)
 * @param[in] bMsbFirst    MSB or LSB selection flag. If false LSB transmitted first and if true MSB transmitted
 *                         first.(value is either ZERO or ONE)
 * @param[in] eMode        SPIM modes for CPOL & CPHA
 * @param[in] bBaudRate    Baud rate value, allowed values 0 to PHHAL_SPIM_MAX_BAUDRATE_VALUE
 * @param[in] bNssPulse    If false no pulse is generated between the bytes and if true a pulse is generated between
 *                         the bytes.(value is either ZERO or ONE)
 * @param[in] bNssPol      If false the polarity is low and if true polarity is high.(value is either ZERO or ONE)
 * @retval #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_ONGOING SPI Master transaction ongoing error.
 */
phStatus_t phhalSPIM_Configure(uint8_t bSlaveSelect, uint8_t bMsbFirst, phhalSPIM_Modes_t eMode, uint8_t bBaudRate,
    uint8_t bNssPulse, uint8_t bNssPol);

/**
 * This API is used to read data from SPI Slave
 * @param[in] bSlaveSelect      Slave select, if ZERO, slave 0 is selected, if ONE, slave 1 is selected.
 *                              Value is either ZERO or ONE
 * @param[in] bInitCRC          Flag to initialize internal CRC RX_SET_CRC.
 * @param[in] bCRCInit          RX CRC init value. Only used if fInitCRC is set.
 * @param[in] wLength           Length of bytes to be received.
 * @param[out] pbBuffer         Pointer to the buffer to receive
 * @param[in] wCRCPayLoadOffset Number of bytes to be skipped for CRC computation
 * @retval #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_INVALID_BUFFER_ADDRESS Invalid buffer address passing error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_ONGOING SPI Master transaction ongoing error.
 * @maskedret #PH_ERR_NOT_CONFIGURED SPI Master is not configured error.
 */
phStatus_t phhalSPIM_Receive(uint8_t bSlaveSelect, uint8_t bInitCRC, uint8_t bCRCInit, uint16_t wLength,
    uint8_t *pbBuffer, uint16_t wCRCPayLoadOffset);

/**
 * This API is used to continue to read data from the slave device specified using phhalSPIM_Receive.
 * @param[in] wLength  Length of bytes to be received.
 * @param[out] pbBuffer Pointer to the buffer to receive.
 * @retval #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_INVALID_BUFFER_ADDRESS Invalid buffer address passing error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_ONGOING SPI Master transaction ongoing error.
 * @maskedret #PH_ERR_NOT_CONFIGURED SPI Master is not configured error.
 */
phStatus_t phhalSPIM_ReceiveContinue(uint16_t wLength, uint8_t *pbBuffer);

/**
 * This API is used to write data to the SPI slave device, transmits the Bytes from the specified buffer.
 * @param[in] bSlaveSelect       Slave select, if false, slave 0 is selected, if true, slave 1 is selected.(value is either
 *                               ZERO or ONE)
 * @param[in] bInitCRC           Flag to initialize internal CRC to TX_CRC_INIT or RX_CRC_INIT.
 * @param[in] bTxAppendCRC       Flag to say appending of CRC is to be done for the transmitted bytes or not,
 *                               if true then CRC is appended, else not appended.(value is either ZERO or ONE)
 * @param[in] bCRCInit           Transmit CRC initial value.
 * @param[in] wLength            Length of bytes to be transmitted.
 * @param[in] pbBuffer           Pointer to the buffer to transmit.
 * @param[in] wCRCPayLoadOffset  Number of bytes to be skipped for CRC computation.
 * @retval #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_INVALID_BUFFER_ADDRESS Invalid buffer address passing error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_ONGOING SPI Master transaction ongoing error.
 * @maskedret #PH_ERR_NOT_CONFIGURED SPI Master is not configured error.
 */
phStatus_t phhalSPIM_Transmit(uint8_t bSlaveSelect, uint8_t bInitCRC, uint8_t bTxAppendCRC, uint8_t bCRCInit,
    uint16_t wLength, uint8_t *pbBuffer, uint16_t wCRCPayLoadOffset);

/**
 * This API is used to write data to the slave device specified using phhalSPIM_Write.
 * @param[in] wLength  Length of bytes to be transmitted.
 * @param[in] pbBuffer Pointer to the buffer to transmit.
 * @retval #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_INVALID_BUFFER_ADDRESS Invalid buffer address passing error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_ONGOING SPI Master transaction ongoing error.
 * @maskedret #PH_ERR_NOT_CONFIGURED SPI Master is not configured error.
 */
phStatus_t phhalSPIM_TransmitContinue(uint16_t wLength, uint8_t *pbBuffer);

/**
 * This API sets the water level to trigger an interrupt if specified number of bytes received in incoming frame, or
 * sent in out coming frame.
 * @param[in]  wWaterLevel   SPIM water level value upon which callback will be called.
 * @param[in]  pCallBackFunc Pointer to user call back function on water mark.
 * @retval #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 */
phStatus_t phhalSPIM_SetWaterLevel(uint16_t wWaterLevel, pphhalSPIM_CallbackFun_t pCallBackFunc);

#endif /* NXPBUILD__PHHAL_SPIM */
/** @} */
#endif /* end PHHALSPIM_H */
