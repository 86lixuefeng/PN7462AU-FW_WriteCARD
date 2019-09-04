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
 * phExMain_Felica.c : Implements the core logic that handles the Felica cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-11-14 15:41:23 +0530 (Mon, 14 Nov 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18751 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExMain_Felica.h"
#include "phpalFelica.h"
#include "phalFelica.h"
#include "phUser.h"
#include "ph_Log.h"

#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
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
 * Example application's Felica card operations entry point.
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_Felica(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    /* data parameter storage */
    phalFelica_Sw_DataParams_t      alFelica;

    /* common variables */
    phStatus_t status;

    /* variables used by the AL component */
    uint8_t pServiceList[2];
    uint8_t bNumServices;
    uint8_t bTxNumBlocks;
    uint8_t bRxNumBlocks;
    uint8_t pBlockList[3];
    uint8_t bBlockListLength;
    uint8_t pBlockData[16];

    do
    {
        /** Initialize the application layer. */
        /* use the Felica application, glue it together with the PAL component */
        status = phalFelica_Sw_Init(&alFelica, sizeof(alFelica), psDiscLoopParams->pPalFelicaDataParams);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Sw_Init Failed\n");
            break;
        }

        phpalFelica_SetSerialNo(psDiscLoopParams->pPalFelicaDataParams, psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm);

        /** Read one block */
        bTxNumBlocks = 1;
        bNumServices = 1;
        pServiceList[0] = 0x0b;
        pServiceList[1] = 0x00;
        bBlockListLength = 2;
        pBlockList[0] = 0x80;
        pBlockList[1] = 0x01;
        status = phalFelica_Read(&alFelica, bNumServices, pServiceList, bTxNumBlocks, pBlockList, bBlockListLength,
            &bRxNumBlocks, pBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Read Failed\n");
            break;
        }
        LOG_TXT_D("Reading successful");
        LOG_AU8_D("BlockData",pBlockData,sizeof(pBlockData));

        /** Write one block */
        pServiceList[0] = 0x09;
        pServiceList[1] = 0x00;
        phUser_MemCpy(pBlockData, "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF", 16);
        pBlockData[4] = 0xD3;
        status = phalFelica_Write(&alFelica, bNumServices, pServiceList, bTxNumBlocks, pBlockList, bBlockListLength,
            pBlockData);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Write Failed\n");
            break;
        }
        LOG_TXT("Read-Write successful\n");
    }while(0);
    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
