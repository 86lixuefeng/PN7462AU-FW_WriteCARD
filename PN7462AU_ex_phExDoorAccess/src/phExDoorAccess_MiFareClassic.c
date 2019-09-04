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
 * phExDoorAccess_MiFareClassic.c : Implements the core logic that handles the MiFare Classic cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <phExDoorAccess_MiFareClassic.h>
#include <phExDoorAccess_Utils.h>
#include "ph_Datatypes.h"

#include "phpalMifare.h"
#include "phalMfc.h"
#include "phKeyStore.h"
#include "phUser.h"
#include "phpalI14443p3a.h"
#include "ph_Log.h"
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/* define Key Store constants */
#define PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES 2
#define PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS 1
#define PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES 1
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

extern uint8_t gphExDoorAccess_Utils_SysHsuTxBuffer[200];
extern uint16_t str_index;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExDoorAccess_MifareOperations(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t bBlockNo,
    uint8_t bKeyType, uint16_t wKeyNumber, uint16_t wKeyVersion);

static phStatus_t phExDoorAccess_KeySet(uint16_t wKeyNumber, uint16_t wKeyVersion, uint16_t wKeyStoreKeyType,
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
phStatus_t phExDoorAccess_MiFareClassic(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
    phKeyStore_Sw_KeyEntry_t        pKeyEntries[PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyEntries =      PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES;
    phKeyStore_Sw_KeyVersionPair_t  pKeyVersionPairs[PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS *
                                                     PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyVersionPairs = PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS;
    phKeyStore_Sw_KUCEntry_t        pKUCEntries[PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES];
    uint16_t wNoOfKUCEntries =      PH_EXDOORACCESS_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES;

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
            break;
        }

        /** Initialize the Key Store. */
        status = phKeyStore_Sw_Init(pKeyStore, sizeof(sKeyStore), pKeyEntries, wNoOfKeyEntries, pKeyVersionPairs,
            wNoOfKeyVersionPairs, pKUCEntries, wNoOfKUCEntries);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /** Initialize the application layer. */
        /* use the Mifare Classic application, glue it together with the PAL component */
        status = phalMfc_Sw_Init(palMifareC, sizeof(salMifareC), ppalMifare, pKeyStore);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        wKeyNumber = 0;
        wKeyVersion = 0;
        wKeyStoreKeyType = PH_KEYSTORE_KEY_TYPE_MIFARE;
 //       phUser_MemCpy(pNewKey, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01", PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE);
           phUser_MemCpy(pNewKey, "\x01\x02\x03\x04\x05\x06\xBC\xBC\xBC\xBC\xBC\xBC", PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE);
			
				status = phExDoorAccess_KeySet(wKeyNumber, wKeyVersion, wKeyStoreKeyType, pNewKey);

        bBlockNo = 0x07;
        bKeyType = PHPAL_MIFARE_KEYB;
        /*Using default Mifare Key*/
        status = phExDoorAccess_MifareOperations(psDiscLoopParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }
    }while(0);

    statusTmp = phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
    if (statusTmp != PH_ERR_SUCCESS)
    {
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
static phStatus_t phExDoorAccess_MifareOperations(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t bBlockNo,
    uint8_t bKeyType, uint16_t wKeyNumber, uint16_t wKeyVersion)
{
    phStatus_t status;
    uint8_t pOriginalBlockData[PHAL_MFC_DATA_BLOCK_LENGTH];

    (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXDOORACCESS_MIFARECLASSIC_AUTHENTICATE_DEFAULT_US);

    status = phalMfc_Authenticate(palMifareC, bBlockNo, bKeyType, wKeyNumber, wKeyVersion,
        psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

    if(PH_ERR_SUCCESS == status)
    {
        /*Glow Green LED to indicate the successful authentication*/
        phExDoorAccess_Utils_Glow_Authetication_Success_LED();
        do
        {
            /* Read one block */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXDOORACCESS_MIFARECLASSIC_READTIMEOUT_DEFAULT_MS);    
						/*设置密码(keyA keyB不可见 )*/
            uint8_t pKey[]= {0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x77,0x8f,0x69,0xBC,0xBC,0xBC,0xBC,0xBC,0xBC};
						phalMfc_Write(palMifareC,0x07,pKey);
						__NOP();
						__NOP();
						
						uint8_t pValue[16]= {0x01,';',0x03,';',0x06,';',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
						uint32_t val = 10000;
						pValue[12]=(val&(0xff<<24))>>24;
						pValue[13]=(val&(0xff<<16))>>16;
						pValue[14]=(val&(0xff<<8))>>8;
						pValue[15]=(val&(0xff<<0))>>0;
					  phalMfc_Write(palMifareC,0x06,pValue);
						__NOP();
						__NOP();
						
						status = phalMfc_Read(palMifareC, 0x06, pOriginalBlockData);
            if(status != PH_ERR_SUCCESS)
            {
                break;
            }
            phExDoorAccess_Utils_Fill_Tx_Buffer(" \r\nSET Data AS:");
            phExDoorAccess_Utils_FillUID_Tx_Buffer(pOriginalBlockData,sizeof(pOriginalBlockData));
						
						__NOP();
						__NOP();
						status = phalMfc_Read(palMifareC, 0x07, pOriginalBlockData);
						phExDoorAccess_Utils_Fill_Tx_Buffer(" \r\nSET Key AS:");
						phExDoorAccess_Utils_FillUID_Tx_Buffer(pOriginalBlockData,sizeof(pOriginalBlockData));
        }while(0);
    }
    else
    {
        /*Authentication failure*/
        phExDoorAccess_Utils_Glow_Authetication_Failure_LED();
    }
    return status;
}

/**
 * Performs Key Set and store it in the Key Store.
 */
static phStatus_t phExDoorAccess_KeySet(uint16_t wKeyNumber, uint16_t wKeyVersion, uint16_t wKeyStoreKeyType,
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
#endif
