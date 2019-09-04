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
 * NxpRdLibEx_MiFare.c
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

#include "phExMain_MiFareUltraLightC.h"
#include "phpalMifare.h"
#include "phpalI14443p3a.h"
#include "phalMful.h"
#include "phUser.h"
#include "phKeyStore.h"
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
 * Example application's Mifare UltraLightC card operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_MiFareUltraLightC(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phpalMifare_Sw_DataParams_t     palMifare;
    phalMful_Sw_DataParams_t        alMifareUl;

    phKeyStore_Sw_DataParams_t      KeyStore;

    /* common variables */
    phStatus_t status;

    /* variables used by the PAL (ISO14443-3) component */
    uint8_t pUidOut[10];
    uint8_t bLenUidOut;

    /* variables used by the AL (Mifare UltraLight) component */
    uint16_t wKeyNumber;
    uint16_t wKeyVersion;
    uint8_t bAddress;
    uint8_t pOriginalBlockData[PH_EXMAIN_MIFAREULTRALIGHTC_BLOCK_SIZE];
    uint8_t pNewBlockData[PH_EXMAIN_MIFAREULTRALIGHTC_BLOCK_SIZE];
    uint8_t pReadBlockData[PH_EXMAIN_MIFAREULTRALIGHTC_BLOCK_SIZE];

    /* variables used by the KeyStore component */
    uint16_t wKeyStoreKeyType;
    uint8_t pNewKey[PH_KEYSTORE_KEY_TYPE_AES128_SIZE];
    phKeyStore_Sw_KeyEntry_t        pKeyEntries[PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyEntries =      PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES;
    phKeyStore_Sw_KeyVersionPair_t  pKeyVersionPairs[PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS *
                                                     PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyVersionPairs = PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS;
    phKeyStore_Sw_KUCEntry_t        pKUCEntries[PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES];
    uint16_t wNoOfKUCEntries =      PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES;

    do
    {

        /** Initialize the protocol abstraction layer. */
        /* use the the Mifare protocol, glue it togeter with the underlaying PAL component. */
        status = phpalMifare_Sw_Init(&palMifare, sizeof(palMifare),psDiscLoopParams->pHalDataParams, psDiscLoopParams->pPal14443p4DataParams);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Initialize the Key Store. */
        status = phKeyStore_Sw_Init(&KeyStore, sizeof(KeyStore), pKeyEntries, wNoOfKeyEntries,
            pKeyVersionPairs, wNoOfKeyVersionPairs, pKUCEntries, wNoOfKUCEntries);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** load the Key to the Store. */
        /* Note: If You use Key number 0x00, be aware that in SAM this Key is the 'Host authentication key' !!! */
        wKeyNumber = 1;
        wKeyVersion = 0;
        wKeyStoreKeyType = PH_KEYSTORE_KEY_TYPE_2K3DES;
        status = phKeyStore_FormatKeyEntry(&KeyStore, wKeyNumber, wKeyStoreKeyType);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Set the Key and store it in the Key Store */
        phUser_MemCpy(pNewKey, PH_EXMAIN_MIFAREULTRALIGHTC_KEY, PH_KEYSTORE_KEY_TYPE_AES128_SIZE);
        status = phKeyStore_SetKey(&KeyStore, wKeyNumber, wKeyVersion, wKeyStoreKeyType, pNewKey, wKeyVersion);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Initialize the application layer. */
        /* use the Mifare Classic application, glue it together with the PAL component */
        status = phalMful_Sw_Init(&alMifareUl, sizeof(alMifareUl), &palMifare, &KeyStore, NULL, NULL);
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
        LOG_AU8("UID",pUidOut,bLenUidOut);

        /** Perform Authentication. */
        status = phalMful_UlcAuthenticate(&alMifareUl, wKeyNumber, wKeyVersion);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Read one MIFARE(R) Block, write the Block and restore the written Block. */
        phUser_MemCpy(pNewBlockData, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10",
            sizeof(pNewBlockData));

        /* Read one block */
        bAddress = 6;
        status = phalMful_Read(&alMifareUl, bAddress, pOriginalBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_X32("Reading successful at address:", bAddress);
        LOG_AU8("BlockData",pOriginalBlockData,sizeof(pOriginalBlockData));

        /* Write one block */
        status = phalMful_Write(&alMifareUl, bAddress, pNewBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_X32("Writing successful at address:", bAddress);

        status = phalMful_Read(&alMifareUl, bAddress, pReadBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_X32("Reading successful at address:", bAddress);
        LOG_AU8("BlockData",pReadBlockData,sizeof(pReadBlockData));

        /* Restore old Block Data */
        status = phalMful_Write(&alMifareUl, bAddress, pOriginalBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_X32("Writing successful at address:", bAddress);

        status = phalMful_Read(&alMifareUl, bAddress, pReadBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
        LOG_X32("Reading successful at address:", bAddress);
        LOG_AU8("BlockData",pReadBlockData,sizeof(pReadBlockData));

    }while(0);

    status = phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);

    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
