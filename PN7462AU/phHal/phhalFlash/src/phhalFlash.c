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
 * Page Flash Controller user mode HAL implementation for Page Flash configuration and access.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-07-03 11:07:00 +0530 (Fri, 03 Jul 2015) $
 * $Author: Sugasi Rajesh (nxp69678) $
 * $Revision: 13475 $ (v04.13.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_FLASH
#include "phUser.h"
#include "ph_Status.h"
#include "PN7462AU/PN7462AU_eectrl.h"
#include "ph_Reg.h"
#include "phhalFlash.h"
#include "phhalSysSer.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define FIND_128BYTE_START_BOUNDARY(x)   ((x) & (~(128UL - 1)))
#define IS_NOT_4BYTE_ALIGNED(x)          ((x) & 0x3)
#define IS_EVEN(x)                       (!((x) & 0x1))
#define TOTAL_NUM_FLASH                  (2)
#define PHHAL_HWFLASH_MEM_START_ADDRESS  (PH_PAGEFLASH_MEM_START)

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: 0 bytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static uint32_t phhalFlash_get_page_number(uint32_t dwAddress);
static void phhalFlash_memcopy_nonaligned(uint8_t *pDestAddress, uint8_t *pSrcAddress, uint32_t dwNumBytes);
static void phhalFlash_memcopy_dword(uint8_t *pDestAddress, uint8_t *pSrcAddress, uint32_t dwNumBytes);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalFlash_Init( uint32_t dwEnableFastMode, uint32_t dwEnableSkipProgramOnEraseFail )
{
    /* Configure Fast mode based on user preference */
    if (dwEnableFastMode)
    {
        /* Enable Fast mode access to Page Flash */
        PH_REG_SET_BIT(EE_CTRL,FAST_COD);
    }
    else
    {
        /* Disable Fast mode access to Page Flash */
        PH_REG_CLEAR_BIT(EE_CTRL,FAST_COD);
    }

    /* Based on user preference, enable/disable the "Skip Program phase on Erase failure" feature for Page Flash */
    if (dwEnableSkipProgramOnEraseFail)
    {
        /* Enable Fast mode access to Page Flash */
        PH_REG_SET_BIT(EE_CTRL,SKIPPRG_0_COD);
        PH_REG_SET_BIT(EE_CTRL,SKIPPRG_1_COD);
    }
    else
    {
        /* Disable Fast mode access to Page Flash */
        PH_REG_CLEAR_BIT(EE_CTRL,SKIPPRG_0_COD);
        PH_REG_CLEAR_BIT(EE_CTRL,SKIPPRG_1_COD);
    }

    return PH_ERR_SUCCESS;
}


phStatus_t phhalFlash_ReadBuffer( uint8_t *pBuffer, uint8_t *pFlash, uint32_t dwBytesToRead )
{
    /* Local variables */
    phStatus_t eStatus = PH_ERR_SUCCESS;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Validate input parameters */
        if (((uint32_t)pFlash < PHHAL_FLASH_USER_START_ADDRESS) ||
            ((uint32_t)pFlash > PHHAL_FLASH_USER_END_ADDRESS)  ||
            (!pBuffer) )
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        if ( (dwBytesToRead > ((PHHAL_FLASH_USER_END_ADDRESS - (uint32_t)pFlash) + 1)) ||
             (dwBytesToRead == 0) )
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }
#endif

        /* Copy the data from the Page Flash to the user buffer */
        if ( IS_NOT_4BYTE_ALIGNED((uint32_t)pFlash) || IS_NOT_4BYTE_ALIGNED((uint32_t)pBuffer) )
        {
            /* Due to the mis-alignment of the addresses, simple 4byte memory copy cannot be used. */
            phhalFlash_memcopy_nonaligned(pBuffer, pFlash, dwBytesToRead);
        }
        else
        {
            /* Since addresses are both Dword aligned, simple Dword memcopy can be used.
             * In case the length is a non-multiple of 4, the last few bytes will be copied using byte access */
            phhalFlash_memcopy_dword(pBuffer, pFlash, dwBytesToRead);
        }
    } while (0);

    return eStatus;
}


phStatus_t phhalFlash_WritePage( uint32_t dwPageNumber, uint8_t *pBuffer )
{
    /* Local variables */
    phStatus_t eStatus = PH_ERR_SUCCESS;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Validate input parameters */
        if (
#if (PHHAL_FLASH_USER_PAGE_START != 0)
                (dwPageNumber < PHHAL_FLASH_USER_PAGE_START) ||
#endif
                (dwPageNumber > PHHAL_FLASH_USER_PAGE_END) )
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        if (!pBuffer)
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        /* Given addresses must be 4byte aligned. */
        if (IS_NOT_4BYTE_ALIGNED((uint32_t)pBuffer))
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }
#endif
        /* Call System Service routine to perform the Flash page write operation */
        eStatus = (phStatus_t)phhalSysSer_SetFlashProgram(dwPageNumber, (void *)pBuffer);
    } while (0);

    return eStatus;
}


phStatus_t phhalFlash_WriteBuffer( uint8_t *pBuffer, uint8_t *pFlash, uint32_t dwBytesToWrite )
{
    /* Local variables */
    uint32_t dwBytesRemaining = dwBytesToWrite;
    uint32_t dwInnerLoopTempAddr;
    uint32_t dwTempAddr;
    uint32_t dwStartAddress;
    uint32_t dwEndAddress;
    uint32_t dwBlock128End;
    uint32_t dwPageNumber;
    uint8_t bBuffIndex = 0;
    phStatus_t eStatus = PH_ERR_SUCCESS;

    /* System service routine will be used to do the Flash page write, one page at a time. Declare a
     * "PHHAL_FLASH_PAGE_SIZE"(128) byte buffer which will filled with the data to write to flash and passed to the
     * system service write page routine.
     */
    uint32_t bPage_data[PHHAL_FLASH_PAGE_SIZE/(sizeof(uint32_t))];

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if (((uint32_t)pFlash < PHHAL_FLASH_USER_START_ADDRESS) ||
            ((uint32_t)pFlash > PHHAL_FLASH_USER_END_ADDRESS)  ||
            (!pBuffer))
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        /* Given addresses must be 4byte aligned.*/
        if (IS_NOT_4BYTE_ALIGNED((uint32_t)pFlash) || IS_NOT_4BYTE_ALIGNED((uint32_t)pBuffer))
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        if ( (dwBytesToWrite > ((PHHAL_FLASH_USER_END_ADDRESS - (uint32_t)pFlash) + 1)) ||
            (dwBytesToWrite == 0) ||
            ((dwBytesToWrite % 4) != 0) )
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }
#endif

        dwStartAddress = (uint32_t)pFlash;
        dwEndAddress = (uint32_t)pFlash + dwBytesToWrite - 1;
        dwTempAddr = dwStartAddress;
        while (dwBytesRemaining)
        {
            /* Find the page in Flash, within which the address resides. This page will be written in this iteration.*/
            dwPageNumber = phhalFlash_get_page_number(dwTempAddr);
            /* PAGEFLASH must be written 128 bytes at a time.*/
            /* Find the 128 byte Start boundary address*/
            dwInnerLoopTempAddr = FIND_128BYTE_START_BOUNDARY(dwTempAddr);
            /* Find the 128 byte end boundary address*/
            dwBlock128End = dwInnerLoopTempAddr + PHHAL_FLASH_PAGE_SIZE - 1;

            /* Within a 128 byte block, read the existing data (which should not be changed, in the page write)
             * using 4byte access. Unlike EEPROM, Untouched data must be read and written back to avoid erasure during
             * programming phase.*/
            bBuffIndex = 0;

            while (dwInnerLoopTempAddr <= dwBlock128End)
            {
                if ((dwInnerLoopTempAddr >= dwStartAddress) && (dwInnerLoopTempAddr <= dwEndAddress))
                {
                    bPage_data[bBuffIndex++] = *((uint32_t *)pBuffer);
                    pBuffer += 4;
                    dwBytesRemaining -= 4;
                }
                else
                {
                    /* For addresses within the page being written, but not within the range of addresses the user wants
                     * to write to,simply copy existing data, so that the same data will be written back and thus be
                     * preserved.
                     */
                    bPage_data[bBuffIndex++] = *((volatile uint32_t *)dwInnerLoopTempAddr);
                }

                dwInnerLoopTempAddr += 4;
            }

            /* Call System Service routine to perform the Flash page write operation.
             * If System Service routine for writing the page in flash returns with failure, pass the return code to
             * the user.
             */
            eStatus = phhalSysSer_SetFlashProgram(dwPageNumber, (void *)bPage_data);
            PH_BREAK_ON_FAILURE(eStatus);

            dwTempAddr = dwBlock128End + 1;
        }
    } while (0);

    return eStatus;
}


phStatus_t phhalFlash_FormatRegion( uint8_t *pFlash, uint32_t dwBytesToWrite, uint32_t dwFormatpattern )
{
    /* Local variables */
    uint32_t dwBytesRemaining = dwBytesToWrite;
    uint32_t dwInnerLoopTempAddr;
    uint32_t dwTempAddr;
    uint32_t dwStartAddress;
    uint32_t dwEndAddress;
    uint32_t dwBlock128End;
    uint32_t dwPageNumber;
    uint8_t bBuffIndex = 0;
    phStatus_t eStatus = PH_ERR_SUCCESS;

    /* System service routine will be used to do the Flash page write, one page at a time. Declare a
     * "PHHAL_FLASH_PAGE_SIZE"(128) byte buffer which will filled with the data to write to flash and passed to the
     * system service write page routine.
     */
    uint32_t bPage_data[PHHAL_FLASH_PAGE_SIZE/(sizeof(uint32_t))];

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Validate input parameters */
        if (((uint32_t)pFlash < PHHAL_FLASH_USER_START_ADDRESS) ||
            ((uint32_t)pFlash > PHHAL_FLASH_USER_END_ADDRESS) )
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        /* Given addresses must be 4byte aligned.*/
        if (IS_NOT_4BYTE_ALIGNED((uint32_t)pFlash))
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }

        if ( (dwBytesToWrite > ((PHHAL_FLASH_USER_END_ADDRESS - (uint32_t)pFlash) + 1)) ||
            (dwBytesToWrite == 0) ||
            ((dwBytesToWrite % 4) != 0) )
        {
            eStatus = PH_ERR(INVALID_PARAMETER,E2PF);
            break;
        }
#endif
        dwStartAddress = (uint32_t)pFlash;
        dwEndAddress = (uint32_t)pFlash + dwBytesToWrite - 1;
        dwTempAddr = dwStartAddress;
        while (dwBytesRemaining)
        {
            /* Find the page in Flash, within which the address resides. This page will be written in this iteration. */
            dwPageNumber = phhalFlash_get_page_number(dwTempAddr);
            /* PAGEFLASH must be written 128 bytes at a time.*/
            /* Find the 128 byte Start boundary address*/
            dwInnerLoopTempAddr = FIND_128BYTE_START_BOUNDARY(dwTempAddr);
            /* Find the 128 byte end boundary address*/
            dwBlock128End = dwInnerLoopTempAddr + PHHAL_FLASH_PAGE_SIZE - 1;

            /* Within a 128 byte block, read the existing data (which should not be changed, in the page write) using
             * 4byte access. Unlike EEPROM, Untouched data must be read and written back to avoid erasure during the
             * programming phase.
             */
            bBuffIndex = 0;

            while (dwInnerLoopTempAddr <= dwBlock128End)
            {
                if ((dwInnerLoopTempAddr >= dwStartAddress) && (dwInnerLoopTempAddr <= dwEndAddress))
                {
                    bPage_data[bBuffIndex++] = dwFormatpattern;
                    dwBytesRemaining -= 4;
                }
                else
                {
                    /* For addresses within the page being written, but not within the range of addresses the user
                     * wants to write to, simply copy existing data, so that the same data will be written back and
                     * thus be preserved.
                     */
                    bPage_data[bBuffIndex++] = *((volatile uint32_t *)dwInnerLoopTempAddr);
                }
                dwInnerLoopTempAddr += 4;
            }

            /* Call System Service routine to perform the Flash page write operation.
             * If System Service routine for writing the page in flash returns with failure, pass the return code to
             * the user */
            eStatus = phhalSysSer_SetFlashProgram(dwPageNumber, (void *)bPage_data);
            PH_BREAK_ON_FAILURE(eStatus);

            dwTempAddr = dwBlock128End + 1;
        }
    } while (0);

    return eStatus;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/* This function is used to calculate the page number of the page in Flash, inside which the given address resides.
 */
static uint32_t phhalFlash_get_page_number(uint32_t dwAddress)
{
    return ((dwAddress - PHHAL_HWFLASH_MEM_START_ADDRESS)/PHHAL_FLASH_PAGE_SIZE);
}


/* This function copies data from source buffer to destination buffer, taking care to avoid mis-aligned address
 *  access. To ensure this, destination buffer is always accessed using byte access.
*/
static void phhalFlash_memcopy_nonaligned( uint8_t *pbDestAddress, uint8_t *pbSrcAddress, uint32_t dwNumBytes )
{
    /* Local variables */
    uint32_t dwByteIndex = 0;
    uint32_t dwTempData32;
    uint32_t dwInnerLoop;

    /* Read will be done using 32bit(4byte) access for faster access.*/
    /* If address is not 4byte aligned, first non-aligned bytes will be read using byte access.*/
    while (IS_NOT_4BYTE_ALIGNED((uint32_t)pbSrcAddress + dwByteIndex) && (dwByteIndex < dwNumBytes))
    {
        pbDestAddress[dwByteIndex] = ((volatile uint8_t *)pbSrcAddress)[dwByteIndex];
        ++dwByteIndex;
    }

    /* Read remaining data using 32bit(4byte) access*/
    while ((dwNumBytes - dwByteIndex) > 3)
    {
        /*  Read data using 4byte access, into a temporary variable */
        dwTempData32 = *((volatile uint32_t *)(pbSrcAddress + dwByteIndex));
        /* Copy the data byte by byte to the user buffer. To avoid mis-aligned access, destination buffer will always be
           accessed using byte access.*/
        dwInnerLoop = 0;
        while (dwInnerLoop < sizeof(uint32_t))
        {
            pbDestAddress[dwByteIndex++] = ((uint8_t *)&dwTempData32)[dwInnerLoop++];
        }
    }

    /* In case bytes are left to be read, read it using byte access */
    while (dwNumBytes > dwByteIndex)
    {
        pbDestAddress[dwByteIndex] = ((volatile uint8_t *)pbSrcAddress)[dwByteIndex];
        ++dwByteIndex;
    }

    return;
}


/* This function can be used to copy data from source buffer to destination buffer, only if both the buffers start at
 *  DWord aligned addresses.
*/
static void phhalFlash_memcopy_dword( uint8_t *pbDestAddress, uint8_t *pbSrcAddress, uint32_t dwNumBytes )
{
    /* Local variables */
    uint32_t dwIndex;
    uint32_t dwNumDwords = dwNumBytes / 4;
    uint32_t dwNumBytesRemaining = dwNumBytes % 4;

    /* Read data using 32bit(4byte) access*/
    for (dwIndex = 0; dwIndex < dwNumDwords; ++dwIndex)
    {
        /* Read data using 4byte access, into a temporary variable */
        ((volatile uint32_t *)pbDestAddress)[dwIndex] = ((volatile uint32_t *)pbSrcAddress)[dwIndex];
    }

    /* In case bytes are left to be read, read it using byte access */
    for (dwIndex = (dwNumBytes - dwNumBytesRemaining); dwIndex < dwNumBytes; ++dwIndex)
    {
        /* Read data using 1 byte access */
        ((volatile uint8_t *)pbDestAddress)[dwIndex] = ((volatile uint8_t *)pbSrcAddress)[dwIndex];
    }

    return;
}

#endif /* NXPBUILD__PHHAL_FLASH */
