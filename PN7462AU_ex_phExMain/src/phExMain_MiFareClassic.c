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
 * phExMain_MiFareClassic.c : Implements the core logic that handles the MiFare Classic cards.
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

#include "phExMain_MiFareClassic.h"
#include "phpalMifare.h"
#include "phalMfc.h"
#include "phKeyStore.h"
#include "phUser.h"
#include "phpalI14443p3a.h"
#include "ph_Log.h"
#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/* define Key Store constants */
#define PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES 2
#define PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS 1
#define PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES 1
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* data parameter storage */
PH_NOINIT static phpalMifare_Sw_DataParams_t     spalMifare;
PH_NOINIT static phalMfc_Sw_DataParams_t         salMifareC;
PH_NOINIT static phKeyStore_Sw_DataParams_t      sKeyStore;
/* data parameter pointer storage */
PH_NOINIT static phpalMifare_Sw_DataParams_t     *ppalMifare;
PH_NOINIT static phalMfc_Sw_DataParams_t         *palMifareC;
PH_NOINIT static phKeyStore_Sw_DataParams_t      *pKeyStore;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExMain_MifareOperations(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t bBlockNo,
    uint8_t bKeyType, uint16_t wKeyNumber, uint16_t wKeyVersion);

static phStatus_t phExMain_KeySet(uint16_t wKeyNumber, uint16_t wKeyVersion, uint16_t wKeyStoreKeyType,
    uint8_t * pNewKey);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Mifare Classic card operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_MiFareClassic(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t statusTmp;
    phStatus_t status;

    /* variables used by the AL (Mifare) component */
    uint8_t bBlockNo;
    uint8_t bKeyType;
    uint16_t wKeyNumber;
    uint16_t wKeyVersion;

    /* variables used by the KeyStore component */
    uint16_t wKeyStoreKeyType;
    uint8_t pNewKey[12];         /* KeyA & KeyB */
    phKeyStore_Sw_KeyEntry_t        pKeyEntries[PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyEntries =      PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES;
    phKeyStore_Sw_KeyVersionPair_t  pKeyVersionPairs[PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS *
                                                     PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyVersionPairs = PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS;
    phKeyStore_Sw_KUCEntry_t        pKUCEntries[PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES];
    uint16_t wNoOfKUCEntries =      PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES;

    ppalMifare = &spalMifare;
    palMifareC = &salMifareC;
    pKeyStore = &sKeyStore;

    do
    {

        /** Initialize the protocol abstraction layer */
        /* use the the Mifare protocol, glue it togeter with the underlaying PAL component. */
        status = phpalMifare_Sw_Init(ppalMifare, sizeof(spalMifare), psDiscLoopParams->pHalDataParams,
            psDiscLoopParams->pPal14443p4DataParams);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Mifare_Sw_Init Failed\n");
            break;
        }

        /** Initialize the Key Store. */
        status = phKeyStore_Sw_Init(pKeyStore, sizeof(sKeyStore), pKeyEntries, wNoOfKeyEntries, pKeyVersionPairs,
            wNoOfKeyVersionPairs, pKUCEntries, wNoOfKUCEntries);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("KeyStore_Sw_Init Failed\n");
            break;
        }

        /** Initialize the application layer. */
        /* use the Mifare Classic application, glue it together with the PAL component */
        status = phalMfc_Sw_Init(palMifareC, sizeof(salMifareC), ppalMifare, pKeyStore);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Application layer init Failed\n");
            break;
        }

        wKeyNumber = 0;
        wKeyVersion = 0;
        wKeyStoreKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE;
        phUser_MemCpy(pNewKey, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE);
        status = phExMain_KeySet(wKeyNumber, wKeyVersion, wKeyStoreKeyType, pNewKey);

        bBlockNo = 0x04;
        bKeyType = PHPAL_MIFARE_KEYA;
        LOG_TXT("Using default Mifare Key\n");
        status = phExMain_MifareOperations(psDiscLoopParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Mifare Operations Failed\n");
            break;
        }

        wKeyNumber = 1;
        /** Set the Key and sore it in the Key Store */
        phUser_MemCpy(pNewKey, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE);
        status = phExMain_KeySet(wKeyNumber, wKeyVersion, wKeyStoreKeyType, pNewKey);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("KeySet Failed\n");
            break;
        }

        bBlockNo = 0x00;
        LOG_TXT("Using new Mifare Key\n");
        status = phExMain_MifareOperations(psDiscLoopParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Mifare Operations Failed\n");
            break;
        }
    }while(0);

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
/**
 * Performs Mifare Authentication, Read and write blocks.
 */
static phStatus_t phExMain_MifareOperations(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t bBlockNo,
    uint8_t bKeyType, uint16_t wKeyNumber, uint16_t wKeyVersion)
{
    phStatus_t status;
    uint8_t pOriginalBlockData[PHAL_MFC_DATA_BLOCK_LENGTH];
    uint8_t pNewBlockData[PHAL_MFC_DATA_BLOCK_LENGTH];
    uint8_t pReadBlockData[PHAL_MFC_DATA_BLOCK_LENGTH];

    (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_MIFARECLASSIC_AUTHENTICATE_DEFAULT_US);
    status = phalMfc_Authenticate(palMifareC, bBlockNo, bKeyType, wKeyNumber, wKeyVersion,
        psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
    if(PH_ERR_SUCCESS == status)
    {
        LOG_X32("Authentication successful of Block:", bBlockNo);

        /* Read one MIFARE(R) Block, write one Block and restore the written Block. */
        phUser_MemCpy(pNewBlockData, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10",
            sizeof(pNewBlockData));

        do
        {

            /* Read one block */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_MIFARECLASSIC_READTIMEOUT_DEFAULT_MS);
            bBlockNo++;
            status = phalMfc_Read(palMifareC, bBlockNo, pOriginalBlockData);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_X32("Reading failed of Block:", bBlockNo);
                break;
            }
            LOG_X32("Reading successful of Block:", bBlockNo);
            LOG_AU8_D("ReadBlockData",pOriginalBlockData,sizeof(pOriginalBlockData));

            /* Write one block */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_MIFARECLASSIC_WRITETIMEOUT_DEFAULT_US);
            status = phalMfc_Write(palMifareC, bBlockNo, pNewBlockData);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_X32("Writing failed of Block:", bBlockNo);
                break;
            }
            LOG_X32("Writing successful of Block:", bBlockNo);
            LOG_AU8_D("WrittenBlockData",pNewBlockData,sizeof(pNewBlockData));
            /* Read one block */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_MIFARECLASSIC_READTIMEOUT_DEFAULT_MS);
            status = phalMfc_Read(palMifareC, bBlockNo, pReadBlockData);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_X32("Reading failed of Block:", bBlockNo);
                break;
            }
            LOG_X32("Reading successful of Block:", bBlockNo);
            LOG_AU8_D("BlockDataRead",pReadBlockData,sizeof(pReadBlockData));

            /* Restore old Block Data */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_MIFARECLASSIC_WRITETIMEOUT_DEFAULT_US);
            status = phalMfc_Write(palMifareC, bBlockNo, pOriginalBlockData);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_X32("Writing failed of Block:", bBlockNo);
                break;
            }
            LOG_X32("Writing successful of Block:", bBlockNo);
            LOG_AU8_D("WrittenBlockData",pOriginalBlockData,sizeof(pOriginalBlockData));
            /* Read one block */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_MIFARECLASSIC_READTIMEOUT_DEFAULT_MS);
            status = phalMfc_Read(palMifareC, bBlockNo, pReadBlockData);
            if(status != PH_ERR_SUCCESS)
            {
                LOG_X32("Reading failed of Block:", bBlockNo);
                break;
            }
            LOG_X32("Reading successful of Block:", bBlockNo);
            LOG_AU8_D("BlockDataRead",pReadBlockData,sizeof(pReadBlockData));

        }while(0);
    }
    else
    {
        LOG_TXT("Authentication fail\n");
    }
    return status;
}

/**
 * Performs Key Set and store it in the Key Store.
 */
static phStatus_t phExMain_KeySet(uint16_t wKeyNumber, uint16_t wKeyVersion, uint16_t wKeyStoreKeyType,
    uint8_t * pNewKey)
{
    phStatus_t status;
    status = phKeyStore_FormatKeyEntry(pKeyStore, wKeyNumber, wKeyStoreKeyType);
    if(status != PH_ERR_SUCCESS)
    {
        return status;;
    }

    /* Set the Key and sore it in the Key Store */
    status = phKeyStore_SetKey(pKeyStore, wKeyNumber, wKeyVersion, wKeyStoreKeyType, pNewKey, wKeyVersion);
        return status;
}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
