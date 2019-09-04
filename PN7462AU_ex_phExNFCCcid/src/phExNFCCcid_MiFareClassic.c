/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * phExNFCCcid_MiFareClassic.c : Implements the core logic that handles the MiFare Classic cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExNFCCcid_MiFareClassic.h"
#include "phpalMifare.h"
#include "phalMfc.h"
#include "phKeyStore.h"
#include "ph_Log.h"
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/* define Key Store constants */
#define PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES 2
#define PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS 1
#define PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES 1
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
extern phpalMifare_Sw_DataParams_t     *ppalMifare;
extern phpalMifare_Sw_DataParams_t     spalMifare;

phalMfc_Sw_DataParams_t         *palMifareC;
phKeyStore_Sw_DataParams_t      *pKeyStore;

static phalMfc_Sw_DataParams_t         salMifareC;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phExNFCCcid_MiFareClassic(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;

    /* data parameter storage */
    phKeyStore_Sw_DataParams_t      sKeyStore;

    /* variables used by the KeyStore component */
    phKeyStore_Sw_KeyEntry_t        pKeyEntries[PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyEntries =      PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES;
    phKeyStore_Sw_KeyVersionPair_t  pKeyVersionPairs[PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS *
                                                     PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES];
    uint16_t wNoOfKeyVersionPairs = PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS;
    phKeyStore_Sw_KUCEntry_t        pKUCEntries[PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES];
    uint16_t wNoOfKUCEntries =      PH_EXNFCCCID_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES;

    palMifareC = &salMifareC;
    pKeyStore = &sKeyStore;

    do
    {

        /* initialise the 'protocol abstraction layer' PAL: */
        /* use the the Mifare protocol, glue it togeter with the underlaying PAL component. */
        status = phpalMifare_Sw_Init(ppalMifare,
            sizeof(spalMifare),
            psDiscLoopParams->pHalDataParams,
            psDiscLoopParams->pPal14443p4DataParams);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Mifare_Sw_Init Failed\n");
            break;
        }

        /* initialise the Key Store: */
        status = phKeyStore_Sw_Init(pKeyStore, sizeof(sKeyStore), pKeyEntries, wNoOfKeyEntries, pKeyVersionPairs,
            wNoOfKeyVersionPairs, pKUCEntries, wNoOfKUCEntries);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("KeyStore_Sw_Init Failed\n");
            break;
        }

        /* initialise the 'application layer' AL: */
        /* use the Mifare Classic application, glue it together with the PAL component */
        status = phalMfc_Sw_Init(palMifareC, sizeof(salMifareC), ppalMifare, pKeyStore);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Application layer init Failed\n");
            break;
        }
    }while(0);

    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif
