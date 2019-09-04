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
 * phExCcid_Felica.c : Implements the core logic that handles the Felica cards.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExCcid_Felica.h"
#include "phpalFelica.h"
#include "phalFelica.h"
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

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExCcid_Felica(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    /* ******************************************************************************* */
    /* DECLARATION */
    /* ******************************************************************************* */
    /* data parameter storage */
    phalFelica_Sw_DataParams_t      alFelica;

    /* common variables */
    phStatus_t status;

    /* variables used by the PAL component */
    uint8_t bRxLength;
    uint8_t pUidOut[16];

    do
    {
        /* ******************************************************************************* */
        /* INITIALISATION */
        /* ******************************************************************************* */
        /* initialise the 'application layer' AL: */
        /* use the Felica application, glue it together with the PAL component */
        status = phalFelica_Sw_Init(&alFelica, sizeof(alFelica), psDiscLoopParams->pPalFelicaDataParams);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("Sw_Init Failed\n");
            break;
        }

        /* ******************************************************************************* */
        /* CARD COMMUNICATION */
        /* ******************************************************************************* */

        status = phpalFelica_GetSerialNo(psDiscLoopParams->pPalFelicaDataParams, pUidOut, &bRxLength);
        if(status != PH_ERR_SUCCESS)
        {
            LOG_TXT("GetSerialNo Failed\n");
            break;
        }

    }while(0);
    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
