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
 * Internal declarations and macro definitions of SPIM hal.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALSPIM_INT_H
#define PHHALSPIM_INT_H

/** \addtogroup phhalSPIM SPI Master hardware abstraction layer.
 *
 * \brief This module briefs all the INTERNAL APIs of SPI Master
 *
 * This module implements SPI interface Flash code HAL, specific to SPIM module.
 * This module is not designed for multi user/process environment.
 *  @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SPIM

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/**
 * RAM start address
 */
#define PHHAL_SPIM_MIN_RAM_ADDRESS                (0x00100000)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 * phhalSPIM_Modes_t specifies the SPIM mode of operation.
 */
typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t bSlaveSelect;              /**< Slave select if false slave 0 is selected, if true slave 1 is selected */
    uint8_t bInitCRC;                  /**< Flag to initialize internal CRC to TX_CRC_INIT or RX_CRC_INIT */
    uint8_t bTxAppendCRC;              /**< Flag to say appending of CRC is to be done for the transmitted bytes or not,
                                       if true then CRC is to be appended, if false then CRC not to be appended */
    uint8_t bCRCInit;                  /**< Transmit CRC initial value */
    uint16_t wLength;                  /**< Length of bytes to be transmitted */
    uint8_t *pbBuffer;                 /**< Pointer to the buffer to transmit or receive */
    uint16_t wCRCPayLoadOffset;        /**< Number of bytes to be skipped for CRC computation */
}PH_PACK_STRUCT_END phhalSPIM_SetRxTx_t;

/**
 * SPIM register configurations.
 */
typedef struct {
    uint32_t dwConfigReg;

#ifdef PHHAL_SPIM_NSS_ENABLED
    uint8_t bNssValue;    /**< Actual NSS value.*/
    uint8_t bNssCtrl;     /**< NSS control value. If false no effect on Nss value and if true override the NSS value*/
#endif

}phhalSPIM_Configure_t;

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/**
 * phhalSPIM_Isr SPIM interrupt service routine.
 *
 * This API is part the Vector Table and directly called in the event of SPIM ISR.
 */
 void SPIM_IRQHandler(void);
#endif /* NXPBUILD__PHHAL_SPIM */

#endif /* end PHHALSPIM_H */
