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
 * phExMain_MiFareUltraLight.c : Implements the core logic that handles the MiFare Ultra Light cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-07-27 17:50:08 +0530 (Mon, 27 Jul 2015) $
 * $Author: ASHISH PAL (nxp79566) $
 * $Revision: 14159 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExMain_MiFareUltraLight.h"
#include "phpalMifare.h"
#include "phpalI14443p3a.h"
#include "phalMful.h"
#include "phUser.h"
#include "phExMain.h"
#include <phhalHw.h>
#include "ph_Log.h"
#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Mifare UltraLight card operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_MiFareUltraLight(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    /* data parameter storage */
    phpalMifare_Sw_DataParams_t     palMifare;
    phalMful_Sw_DataParams_t        alMifareUl;

    /* common variables */
    phStatus_t status;
    phStatus_t statusTmp;

    /* variables used by the PAL (ISO14443-3) component */
    uint8_t pUidOut[10];
    uint8_t bLenUidOut;

    /* variables used by the AL (Mifare UltraLight) component */
    uint8_t bAddress;
    uint8_t pOriginalBlockData[PH_EXMAIN_MIFAREULTRALIGHT_BLOCK_SIZE_UL * 4];
    uint8_t pNewBlockData[PH_EXMAIN_MIFAREULTRALIGHT_BLOCK_SIZE_UL];
    uint8_t pReadBlockData[PH_EXMAIN_MIFAREULTRALIGHT_BLOCK_SIZE_UL * 4];


    do
    {

        /** Initialize the protocol abstraction layer. */
        /* use the the Mifare protocol, glue it togeter with the underlaying PAL component. */
        status = phpalMifare_Sw_Init(&palMifare, sizeof(palMifare), psDiscLoopParams->pHalDataParams, psDiscLoopParams->pPal14443p4DataParams);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Initialize the application layer. */
        /* use the Mifare Classic application, glue it together with the PAL component */
        status = phalMful_Sw_Init(&alMifareUl, sizeof(alMifareUl), &palMifare, NULL, NULL, NULL);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Get the serial number. */
        status = phpalI14443p3a_GetSerialNo(psDiscLoopParams->pPal1443p3aDataParams, pUidOut, &bLenUidOut);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_AU8_D("UID",pUidOut,bLenUidOut);

        /* Read one MIFARE(R) Block, write one Block and restore the written Block. */
        phUser_MemCpy(pNewBlockData, "\x01\x02\x03\x04", sizeof(pNewBlockData));

        /* Read one block */
        bAddress = 6;

        /* refer Fig 15. READ doc MF0ICU1.pdf*/

        /** Read data from the card */
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, PH_EXMAIN_MIFAREULTRALIGHT_READTIMEOUT_DEFAULT_US);
        status = phalMful_Read(&alMifareUl, bAddress, pOriginalBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Read failed Block:", bAddress);
            break;
        }
        LOG_X32("Read successful Block:", bAddress);
        LOG_AU8_D("BlockDataRead",pOriginalBlockData,sizeof(pOriginalBlockData));

        /** Write data to the card. */
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, PH_EXMAIN_MIFAREULTRALIGHT_WRITETIMEOUT_DEFAULT_US);
        status = phalMful_Write(&alMifareUl, bAddress, pNewBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Write failed of Block:", bAddress);
            break;
        }
        LOG_X32("Write successful of Block:", bAddress);
        LOG_AU8_D("BlockDataWritten",pNewBlockData,sizeof(pNewBlockData));

        /* refer Fig 15. READ doc MF0ICU1.pdf*/

        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, PH_EXMAIN_MIFAREULTRALIGHT_READTIMEOUT_DEFAULT_US);
        status = phalMful_Read(&alMifareUl, bAddress, pReadBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Read failed Block:", bAddress);
            break;
        }
        LOG_X32("Read successful Block:", bAddress);
        LOG_AU8_D("BlockDataRead",pReadBlockData,sizeof(pReadBlockData));

        /*writing the data to card */
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, PH_EXMAIN_MIFAREULTRALIGHT_WRITETIMEOUT_DEFAULT_US);
        /* Restore old Block Data */
        status = phalMful_Write(&alMifareUl, bAddress, pOriginalBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Write failed of Block:", bAddress);
            break;
        }
        LOG_X32("Writing successful at address:", bAddress);
        LOG_AU8_D("BlockDataWritten",pOriginalBlockData,sizeof(pOriginalBlockData));
        /* refer Fig 15. READ doc MF0ICU1.pdf*/

        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, PH_EXMAIN_MIFAREULTRALIGHT_READTIMEOUT_DEFAULT_US);

        status = phalMful_Read(&alMifareUl, bAddress, pReadBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_X32("Read successful Block:", bAddress);
        LOG_AU8_D("BlockDataRead",pReadBlockData,sizeof(pReadBlockData));
    }while(0);

    /** Perform Halt */
    statusTmp = phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
    if (statusTmp != PH_ERR_SUCCESS)
    {
        LOG_TXT("HaltA Failed\n");
        status = statusTmp;
    }

    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
