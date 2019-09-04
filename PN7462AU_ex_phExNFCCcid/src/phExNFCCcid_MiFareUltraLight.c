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
 * phExNFCCcid_MiFareUltraLight.c : Implements the core logic that handles the MiFare Ultra Light cards.
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

#include "phExNFCCcid_MiFareUltraLight.h"
#include "phpalMifare.h"
#include "phpalI14443p3a.h"
#include "phalMful.h"
#include "phUser.h"
#include "phExNFCCcid.h"
#include "ph_Log.h"

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
phalMful_Sw_DataParams_t               *palMifareUl;
static phalMful_Sw_DataParams_t        alMifareUl;

extern phpalMifare_Sw_DataParams_t     *ppalMifare;
extern phpalMifare_Sw_DataParams_t     spalMifare;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExNFCCcid_MiFareUltraLight(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    /* ******************************************************************************* */
    /* DECLARATION */
    /* ******************************************************************************* */

    /* common variables */
    phStatus_t status;
    uint8_t *   pRxBuffer;
    uint16_t    wRxLength;
    uint8_t     bFrame[PHAL_MFUL_DES_BLOCK_SIZE+1];

    palMifareUl = &alMifareUl;

    do
    {
        /* ******************************************************************************* */
        /* INITIALISATION */
        /* ******************************************************************************* */

        /* initialise the 'protocol abstraction layer' PAL: */
        /* use the the Mifare protocol, glue it togeter with the underlaying PAL component. */
        status = phpalMifare_Sw_Init(ppalMifare, sizeof(spalMifare), psDiscLoopParams->pHalDataParams, psDiscLoopParams->pPal14443p4DataParams);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /* initialise the 'application layer' AL: */
        /* use the Mifare Classic application, glue it together with the PAL component */
        status = phalMful_Sw_Init(palMifareUl, sizeof(alMifareUl), ppalMifare, NULL, NULL, NULL);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /* ******************************************************************************* */
        /* CARD COMMUNICATION */
        /* ******************************************************************************* */
        /* build the authentication request */
        bFrame[0] = 0x1A; //PHAL_MFUL_CMD_AUTH;
        bFrame[1] = 0x00;
        /* transmit the request */
        status =  phpalMifare_ExchangeL3(palMifareUl->pPalMifareDataParams,
                                         PH_EXCHANGE_DEFAULT,
                                         bFrame,
                                         2,
                                         &pRxBuffer,
                                         &wRxLength
                                        );

        if ((status != PH_ERR_SUCCESS))
            break;

    }while(0);

    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
