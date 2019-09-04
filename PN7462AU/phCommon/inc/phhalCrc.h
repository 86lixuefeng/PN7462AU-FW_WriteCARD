/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * CRC HAL Implementation for CRC Module
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALCRC_H
#define PHHALCRC_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CRC

/** \addtogroup phhalCRC
 *
 * The Cyclic Redundancy Check APIs
 *
 * This module implements CRC interface Flash code HAL, specific to CRC module.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters
 *                   will not be checked for erroneous values in the HAL API. In such a build configuration use the
 *                   API with caution and pass valid parameters only. Invalid parameters will likely result in
 *                   hardfaults or other failures.
 *  @{
 *
 *
 * \dot
 *      digraph "Sample Usage" {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalCrc_CalculateCrc [URL ="\ref phhalCrc_CalculateCrc"];
 *          phhalCrc_CheckCrc [URL ="\ref phhalCrc_CheckCrc"];
 *
 *
 *      //Logical Flow
 *          Start -> phhalCrc_CalculateCrc -> End;
 *          Start -> phhalCrc_CheckCrc -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="green" ];
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 *
 *
 */

#include "ph_Status.h"
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Specifies the 16/32 bit CRC Mode and LSB/MSB First input
 */
typedef enum phhalCrc_Modes{
    /** \b 0x00 */ E_CRC16_MSB_FIRST = 0, /**< CRC16 MSB First Input */
    /** \b 0x01 */ E_CRC16_LSB_FIRST,     /**< CRC16 LSB First Input */
    /** \b 0x02 */ E_CRC32_MSB_FIRST,     /**< CRC32 MSB First Input */
    /** \b 0x03 */ E_CRC32_LSB_FIRST,     /**< CRC32 LSB First Input */
    /** \b 0x04 */ E_CRC_MODE_MAX         /**< @internal */
} phhalCrc_Modes_t;

/**
 * Function Pointer to an API that calculates CRC
 */
typedef phStatus_t (phhalCrc_CalculateCrc_t)( uint32_t dwCrcPreload, phhalCrc_Modes_t bCRCMode, uint8_t *pbBuffer,
    uint32_t dwDataLength, uint32_t *pdwCrcValue );
/**
 * Function Pointer to an API that checks CRC
 */
typedef phStatus_t (phhalCrc_CheckCrc_t)( uint32_t dwCrcPreload, phhalCrc_Modes_t bCRCMode, uint8_t *pbBuffer,
    uint32_t dwDataLength);

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/**
 * This API calculates the CRC16/32 with LSB/MSB First for a arbitrary length of buffer
 * @param[in] dwCrcPreload Initial Preset Value.
 * @param[in] bCRCMode CRC16/32 and MSB/LSB First.
 * @param[in] pbBuffer Pointer to buffer over which CRC is calculated.
 * @param[in] dwDataLength Length of the buffer on which CRC is calculated
 * @param[out] pdwCrcValue Pointer to a uint32 variable that will hold the CRC16/32 result
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS If CRC is calculated successfully .
 * @maskedret #PH_ERR_INVALID_PARAMETER - Invalid passing parameter error.
 */
#define phhalCrc_CalculateCrc(dwCrcPreload, bCRCMode, pbBuffer, dwDataLength, pdwCrcValue)         \
    ((phhalCrc_CalculateCrc_t *)\
    		PH_HAL_CRC_CALCULATECRC_ADDR)\
			(dwCrcPreload, bCRCMode, pbBuffer, dwDataLength, pdwCrcValue)

/**
 * This API checks the CRC16/32 with LSB/MSB First for a arbitrary length of buffer with provided CRC
 * The provided CRC is padded to the end of the buffer.
 * @param[in] dwCrcPreload Initial Preset Value.
 * @param[in] bCRCMode CRC16/32 and MSB/LSB First.
 * @param[in] pbBuffer Pointer to buffer containing contents as well as CRC of the content
 * @param[in] dwDataLength Length of the buffer on which CRC is checked
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS If CRC is Check is success .
 * @maskedret #PH_ERR_INVALID_PARAMETER - Invalid passing parameter error.
 * @maskedret #PH_ERR_FAILED - If CRC Check is failed.
 */
#define phhalCrc_CheckCrc(dwCrcPreload, bCRCMode, pbBuffer, dwDataLength)        \
    ((phhalCrc_CheckCrc_t *)\
    		PH_HAL_CRC_CHECKCRC_ADDR)\
			(dwCrcPreload, bCRCMode, pbBuffer, dwDataLength)

#endif /* NXPBUILD__PHHAL_CRC */
/** @} */
#endif /* PHHALCRC_H */
