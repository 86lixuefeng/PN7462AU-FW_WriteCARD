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
 * Interface for the FLASH Controller HAL for PageFlash Access.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALFLASH_H
#define PHHALFLASH_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_FLASH
#include "ph_Status.h"
#include "ph_MemoryMap.h"

/** @addtogroup phhalFlash
 *
 * The PAGEFLASH Read/Write APIs
 *
 * @note: Physically PN7462AU has two different  instances of Flash. Flash 0,
 *
 * FLASH HAL exposes the PAGEFLASH as a single Flash memory, not as separate Flash 0, Flash 1 memory banks.
 * It is possible to have only one write or read of PAGEFLASH at any moment of time.
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
 *          phhalFlash_FormatRegion [URL ="\ref phhalFlash_FormatRegion"];
 *          phhalFlash_Init [URL ="\ref phhalFlash_Init"];
 *          phhalFlash_ReadBuffer [URL ="\ref phhalFlash_ReadBuffer"];
 *          phhalFlash_WriteBuffer [URL ="\ref phhalFlash_WriteBuffer"];
 *          phhalFlash_WritePage [URL ="\ref phhalFlash_WritePage"];
 *
 *          phhalSysSer_SetFlashProgram  [URL = "\ref phhalSysSer_SetFlashProgram"];
 *
 *      //Logical Flow
 *          Start -> phhalFlash_Init;
 *          phhalFlash_Init -> phhalFlash_ReadBuffer;
 *          phhalFlash_Init -> phhalFlash_WriteBuffer;
 *          phhalFlash_Init -> phhalFlash_WritePage;
 *          phhalFlash_Init -> phhalFlash_FormatRegion;
 *
 *          phhalFlash_ReadBuffer -> End;
 *          phhalFlash_WriteBuffer -> End;
 *          phhalFlash_WritePage -> End;
 *          phhalFlash_FormatRegion -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="green" ];
 *          phhalFlash_WriteBuffer -> phhalSysSer_SetFlashProgram [label = "Uses"];
 *          phhalFlash_WritePage -> phhalSysSer_SetFlashProgram [label = "Uses"];
 *          phhalFlash_FormatRegion -> phhalSysSer_SetFlashProgram [label = "Uses"];
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 *
 */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/* PAGE FLASH PROPERTIES */
#define PHHAL_FLASH_USER_START_ADDRESS  (PH_PAGEFLASH_USERDATA_START) /**< Page Flash User Region Start Address */
#define PHHAL_FLASH_USER_END_ADDRESS    (PH_PAGEFLASH_USERDATA_END)   /**< Page Flash User Region End Address */
#define PHHAL_FLASH_PAGE_SIZE           (128UL)                       /**< Page Flash Page size */
#define PHHAL_FLASH_USER_PAGE_START     ((PH_PAGEFLASH_USERDATA_START - PH_PAGEFLASH_MEM_START)/    \
                                          (PHHAL_FLASH_PAGE_SIZE))    /**< Page Flash User Region Start Page number */
#define PHHAL_FLASH_USER_PAGE_END       ((PH_PAGEFLASH_USERDATA_END - PH_PAGEFLASH_MEM_START)/      \
                                          (PHHAL_FLASH_PAGE_SIZE))    /**< Page Flash User Region End Page number */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/** This function initializes the Page Flash HAL.
 *
 * @note This Function must be called once before using other APIs in this module.
 *
 * @param[in] dwEnableFastMode Double Word used as Boolean value to enable or disable Fast mode for Page Flash access.
 * @param[in] dwEnableSkipProgramOnEraseFail Double word used as boolean value to enable or disable the "Skip Program
 *            on Erase Fail" feature for Page Flash access.
 *
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS always
*/
phStatus_t phhalFlash_Init( uint32_t dwEnableFastMode, uint32_t dwEnableSkipProgramOnEraseFail );

/**
 * This function reads PAGEFLASH memory data.
 *
 * Page Flash memory can be read like RAM, using pointers. The value that this API adds is that it takes care of
 * avoiding memory access violations, even when one or both of the addresses are mis-aligned. Additionally, parameter
 * checking can be enabled.
 *
 * @param[out] pbBuffer Pointer to buffer where read data will be copied.
 * @param[in] pbFlash Valid address of a byte in the PAGEFLASH.
 * @param[in] dwBytesToRead Number of bytes to be read.
 * @return Status of the Operation.
 *
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid.
 * @retval    #PH_ERR_SUCCESS if PAGEFLASH Read Operation completed successfully.
 */
phStatus_t phhalFlash_ReadBuffer( uint8_t *pbBuffer, uint8_t *pbFlash, uint32_t dwBytesToRead );

/** This function writes a single full 128byte PAGEFLASH page.
 *
 * @note FLASH pages are numbered from 0 onwards, but in User mode, Page flash pages that can be used range
 *       from #PHHAL_FLASH_USER_PAGE_START to #PHHAL_FLASH_USER_PAGE_END
 * @note The Write is synchronous i.e.Polling based. The API will return only after the write operation completes.
 * @note This API internally uses a System Service to write each Page in the Flash.
 *
 * @param[in] dwPageNumber Valid index of a 128byte page in the PAGEFLASH.
 * @param[in] pbBuffer Pointer to buffer containing data to be written to PAGEFLASH. Must be 4byte aligned address.
 * @return Status of the Operation.
 *
 * @retval    #PH_ERR_SUCCESS if PAGEFLASH Write Operation completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid and parameter checking is enabled in build.
 * @retval System Service error code in case call to System service fails.
 */
phStatus_t phhalFlash_WritePage( uint32_t dwPageNumber, uint8_t *pbBuffer );

/** This function writes data to PAGEFLASH.
 *
 * This API can be used to write data of any valid size(even mis-aligned) to any valid address in the
 * PageFlash(even mis-aligned addresses).
 * @note The Write is synchronous i.e. The API will return only after the write operation completes.
 * @note This API internally uses a System Service to write each Page in the Flash.
 *
 * @param[in] pbBuffer pointer to buffer containing data to be written to PAGEFLASH. Must be 4byte aligned address.
 * @param[in] pbFlash Valid address of a byte in the PAGEFLASH. Must be 4byte aligned address.
 * @param[in] dwBytesToWrite number of bytes to be written. Must be Non zero, multiple of 4.
 * @return Status of the Operation.
 *
 * @retval    #PH_ERR_SUCCESS if PAGEFLASH Write Operation completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid.
 * @retval System Service error code in case call to System service fails.
 */
phStatus_t phhalFlash_WriteBuffer( uint8_t *pbBuffer, uint8_t *pbFlash, uint32_t dwBytesToWrite );

/** This function fills a specified region in the PAGEFLASH with a format pattern.
 *
 * @note The Write is synchronous i.e. The API will return only after the write operation completes.
 * @note This API internally uses a System Service to write each Page in the Flash.
 *
 * @param[in] pbFlash Valid address of a byte in the PAGEFLASH. Must be 4byte aligned address.
 * @param[in] dwBytesToWrite Number of bytes to be written. Must be Non zero, multiple of 4.
 * @param[in] dwFormatPattern 32bit pattern to fill the given region in Flash.
 * @return Status of the Operation.
 *
 * @retval    #PH_ERR_SUCCESS if PAGEFLASH Format Operation completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER if parameters passed are invalid.
 * @retval System Service error code in case call to System service fails.
 */
phStatus_t phhalFlash_FormatRegion( uint8_t *pbFlash, uint32_t dwBytesToWrite, uint32_t dwFormatPattern );

/** @} */
#endif /* NXPBUILD__PHHAL_FLASH */
#endif /* PHHALFLASH_H */
