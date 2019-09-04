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
 * Interface for the EEPROM Controller HAL for EEPROM access.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALEEPROM_H
#define PHHALEEPROM_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_EEPROM
#include "ph_Status.h"
#include "ph_MemoryMap.h"
#include "PN7462AU_ROM3.h"

/**
 * @addtogroup phhalEeprom
 *
 * The EEPROM Controller Read/Write APIs
 *
 * @warning It is impossible to have more than one write or read of EEPROM at any moment of time.
 *
 * @warning This module uses the @ref phUser_Wait() API. Hence the user is expected to initialise this mechanism
 *          by calling @ref phUser_WaitInit() before calling any API from this module.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *          checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *          and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *          failures.
 *
 * @{
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
 *          phUser_WaitInit [URL ="\ref phUser_WaitInit"];
 *          phhalEeprom_DeInit [URL ="\ref phhalEeprom_DeInit"];
 *          phhalEeprom_FormatRegion [URL ="\ref phhalEeprom_FormatRegion"];
 *          phhalEeprom_HwReset [URL ="\ref phhalEeprom_HwReset"];
 *          phhalEeprom_Init [URL ="\ref phhalEeprom_Init"];
 *          phhalEeprom_ReadBuffer [URL ="\ref phhalEeprom_ReadBuffer"];
 *          phhalEeprom_WriteBuffer [URL ="\ref phhalEeprom_WriteBuffer"];
 *          phhalEeprom_WritePage [URL ="\ref phhalEeprom_WritePage"];
 *
 *      //Logical Flow
 *          Start -> phUser_WaitInit -> phhalEeprom_Init;
 *          phhalEeprom_Init -> phhalEeprom_ReadBuffer;
 *          phhalEeprom_Init -> phhalEeprom_WriteBuffer;
 *          phhalEeprom_Init -> phhalEeprom_WritePage;
 *          phhalEeprom_Init -> phhalEeprom_FormatRegion;
 *          phhalEeprom_ReadBuffer -> phhalEeprom_DeInit;
 *          phhalEeprom_WriteBuffer -> phhalEeprom_DeInit;
 *          phhalEeprom_WritePage -> phhalEeprom_DeInit;
 *          phhalEeprom_FormatRegion -> phhalEeprom_DeInit;
 *          phhalEeprom_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *          phhalEeprom_HwReset -> End;
 *
 *      }
 * \enddot
 *
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/** \name EEPROM Access Ranges.
 *
 * @{
 */

#define PHHAL_EEPROM_DATA_START_ADDRESS (PH_EEPROM_USERDATA_START)
#define PHHAL_EEPROM_DATA_END_ADDRESS   (PH_EEPROM_USERDATA_END)
#define PHHAL_EEPROM_PAGE_SIZE          (64U)
#define PHHAL_EEPROM_USERPAGE_START     \
	((PH_EEPROM_USERDATA_START - PH_EEPROM_MEM_START)\
			/(PHHAL_EEPROM_PAGE_SIZE))
#define PHHAL_EEPROM_USERPAGE_END       \
	((PH_EEPROM_USERDATA_END - PH_EEPROM_MEM_START)/\
			(PHHAL_EEPROM_PAGE_SIZE))

/** @} */

/** \name EEPROM HAL specific Error Codes.
 *
 * @{ */

/** EEPROM is busy in previous write operation */
#define PHHAL_ERR_EEPROM_BUSY    (0x007B)

/** @} */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** @copybrief phhalEeprom_Init
 * @see phhalEeprom_Init
 */
typedef phStatus_t (phhalEeprom_Init_t)(uint8_t bEnableFastMode);
 /** @copybrief phhalEeprom_Init
  * @see phhalEeprom_Init
  */
typedef phStatus_t (phhalEeprom_DeInit_t)(void);
/** @copybrief phhalEeprom_Init
 * @see phhalEeprom_Init
 */
typedef phStatus_t (phhalEeprom_ReadBuffer_t)( uint8_t *pbBuffer, uint8_t *pbEeprom, uint32_t dwBytesToRead);
/** @copybrief phhalEeprom_WritePage
 * @see phhalEeprom_WritePage
 */
typedef phStatus_t (phhalEeprom_WritePage_t)(uint32_t dwPageNumber, uint8_t *pbBuffer);
/** @copybrief phhalEeprom_WriteBuffer
 * @see phhalEeprom_WriteBuffer
 */
typedef phStatus_t (phhalEeprom_WriteBuffer_t)(uint8_t *pbBuffer, uint8_t *pbEeprom, uint32_t dwBytesToWrite);
/** @copybrief phhalEeprom_FormatRegion
 * @see phhalEeprom_FormatRegion
 */
typedef phStatus_t (phhalEeprom_FormatRegion_t)(uint8_t *pbEeprom, uint32_t dwBytesToFill, uint32_t dwPattern32Bit);
/** @copybrief phhalEeprom_HwReset
 * @see phhalEeprom_HwReset
 */
typedef phStatus_t (phhalEeprom_HwReset_t)(void);

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/** This function initializes the EEPROM HAL.
 *
 * @warning The EEPROM memory is Powered up. Normally this takes 100us. The user must not perform any EEPROM access
 *          for a period of 100us after calling the Init API of EEPROM HAL.
 * @note This Function must be called once before using other APIs in this module.
 *
 * @note This EEPROM HAL uses the @ref phUser_Wait() API. Hence the user is expected to initialise this mechanism
 *       by calling @ref phUser_WaitInit() before calling phhalEeprom_Init().
 *
 * @param[in] bEnableFastMode 8bit unsigned integer used as Boolean value to enable or disable Fast mode for EEPROM
 *            access. In Fast mode, there are less wait states during the Write and read access operations to EEPROM.
 *            However, there is no change in the duration of the program phase for write operations.
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS always
 */
#define phhalEeprom_Init(bEnableFastMode)          \
	((phhalEeprom_Init_t *)\
			PH_HAL_EEPROM_INIT_ADDR)\
			(bEnableFastMode)

/** This function de-initializes the EEPROM HAL.
 *
 * @warning This function must be called only when we are sure EEPROM will no longer be used, since EEPROM memory will
 *          be powered down in this API.
 *
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS - always
 */

#define phhalEeprom_DeInit()        \
	((phhalEeprom_DeInit_t *)\
			PH_HAL_EEPROM_DEINIT_ADDR)()


/** This function reads EEPROM data.
 *
 * EEPROM memory can be read like RAM, using pointers. The value that this API adds is that it takes care of
 * avoiding memory access violations, even when one or both of the addresses are mis-aligned. Additionally, parameter
 * checking is also done, if enabled in the build.
 *
 * @note This function reads EEPROM data without ECC error reporting. Upper Software layer must take care of
 *       validating the read data, if desired.
 *
 * @param[out] pbBuffer Pointer to buffer where data will be copied
 * @param[in] pbEeprom Pointer to EEPROM address to be read
 * @param[in] dwBytesToRead Number of bytes to be read.  Must not be zero.
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS if EEPROM Read Operation completed successfully(Error checking not done by the API).
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid and parameter checking is enabled in build.
 */
#define phhalEeprom_ReadBuffer(pbBuffer, pbEeprom, dwBytesToRead)        \
    ((phhalEeprom_ReadBuffer_t *)\
    		PH_HAL_EEPROM_READBUFFER_ADDR)\
			(pbBuffer, pbEeprom, dwBytesToRead)


/** This function writes a single full 64byte EEPROM page, with data provided by the user.
 *
 * @note EEPROM pages are numbered from 0 to 63, but in User mode, EEPROM pages that can be used range
 *       from "PHHALHW_EEPROM_USERPAGE_START" to "PHHALHW_EEPROM_USERPAGE_END"
 * @note The Write is synchronous i.e.Polling based. The API will return only after the write operation completes.
 *
 * @param[in] dwPageNumber Valid index of a 64byte page in the EEPROM
 * @param[in] pbBuffer Pointer to buffer containing data to be written to EEPROM. Must be 4 byte aligned
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS if EEPROM Write Operation completed successfully.
 * @maskedret #PH_ERR_READ_WRITE_ERROR HW related errors.
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid and parameter checking is enabled in build.
 * @maskedret #PHHAL_ERR_EEPROM_BUSY if another EEPROM write is already ongoing.
 */
#define phhalEeprom_WritePage(dwPageNumber, pbBuffer)           \
    ((phhalEeprom_WritePage_t *)\
    		PH_HAL_EEPROM_WRITEPAGE_ADDR)\
			(dwPageNumber, pbBuffer)


/** This function writes data to EEPROM.
 *
 * This API can be used to write data of any valid size(even mis-aligned) to any valid address in the
 * EEPROM(even mis-aligned addresses).
 *
 * @note The Write is synchronous i.e. The API will return only after the write operation completes.
 * @note This API is not optimized for Page writes. phhalEeprom_WritePage() is a better choice when writing to a
 *       single page in the EEPROM.
 *
 * @param[in] pbBuffer Pointer to buffer to be copied - can be in RAM or EEPROM
 * @param[in] pbEeprom Pointer to EEPROM buffer where data will be copied
 * @param[in] dwBytesToWrite Number of bytes to be written. Must not be zero.
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS if EEPROM Write Operation completed successfully.
 * @maskedret #PH_ERR_READ_WRITE_ERROR HW related errors
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid and parameter checking is enabled in build.
 * @maskedret #PHHAL_ERR_EEPROM_BUSY if another EEPROM write is already ongoing.
 */
#define phhalEeprom_WriteBuffer(pbBuffer, pbEeprom, dwBytesToWrite)         \
    ((phhalEeprom_WriteBuffer_t*)\
    		PH_HAL_EEPROM_WRITEBUFFER_ADDR)\
			(pbBuffer, pbEeprom, dwBytesToWrite)


/** This function fills a specified region in the EEPROM with a format pattern.
 *
 * @note The format is synchronous i.e. The API will return only after the write operation completes.
 *
 * @param[in] pbEeprom 4byte aligned address in EEPROM starting from which, the pattern will be filled.
 * @param[in] dwBytesToFill Total number of bytes to be written. Must be multiple of 4. Must not be zero.
 * @param[in] dwPattern32Bit A 32 bit pattern that will be written to the specified region in the EEPROM
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS if EEPROM Write Operation completed successfully.
 * @maskedret #PH_ERR_READ_WRITE_ERROR HW related errors
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid and parameter checking is enabled in build.
 * @maskedret #PHHAL_ERR_EEPROM_BUSY if another EEPROM write is already ongoing.
 */
#define phhalEeprom_FormatRegion(pbEeprom, dwBytesToFill, dwPattern32Bit)        \
    ((phhalEeprom_FormatRegion_t *)\
    		PH_HAL_EEPROM_FORMATREGION_ADDR)\
			(pbEeprom, dwBytesToFill, dwPattern32Bit)


/** This function resets the EEPROM controller hardware.
 *
 * The purpose of this API is to reset the EEPROM controller. This is useful if a write program phase never completes
 * and the write operation needs to be aborted. A typical use case would be to use this macro inside the watch dog timer
 * handler, when a EEPROM write operation is ongoing.
 *
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS always
 */
#define phhalEeprom_HwReset()         \
	((phhalEeprom_HwReset_t *)\
			PH_HAL_EEPROM_HWRESET_ADDR)()


/** @} */
#endif /* NXPBUILD__PHHAL_EEPROM */
#endif /* PHHALEEPROM_H */
