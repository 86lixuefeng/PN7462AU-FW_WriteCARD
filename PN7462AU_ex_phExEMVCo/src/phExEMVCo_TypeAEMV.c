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
 * phExEMVCo_TypeAEMV.c : Implements the core logic for EMVCo exchange operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-24 16:34:49 +0530 (Mon, 24 Aug 2015) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 14680 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExEMVCo_TypeAEMV.h"
#include "phpalMifare.h"
#include "phpalI14443p4.h"
#include "phUser.h"
#include "phpalI14443p4a.h"
#include "phExEMVCo_Log.h"
#include "phalMfdf.h"

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXEMVCO_CIF_MAX_NO_APDU 8
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

static const uint8_t bApduCommandArray[PH_EXEMVCO_CIF_MAX_NO_APDU][50] =
{
    /* Preparing select command for emv operation */
    { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59, 0x2E, 0x53,
      0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 },

    /* ADF specific select command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x10,0x10,0x00},

    /* Get Processing operation */
    {0x80,0xA8,0x00,0x00,0x02,0x83,0x00,0x00},

    /* Read record (sfi 2 record 1)operation */
    {0x00,0xB2,0x01,0x14,0x00},

    /* Read record (sfi 3 record 1)operation */
    {0x00,0xB2,0x01,0x1C,0x00},

    /* Read record (sfi 4 record 1)operation */
    {0x00,0xB2,0x01,0x24,0x00},

    /* Read record (sfi 4 record 2)operation */
    {0x00,0xB2,0x02,0x24,0x00},

    /* Preparing application cryptogram command */
    {0x80, 0xAE, 0x50, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x78, 0x14, 0x09, 0x01, 0x00,
    0x33, 0x71, 0x11, 0x85, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00 ,0x00},

};

static const uint8_t bApduCommandArrayLen[PH_EXEMVCO_CIF_MAX_NO_APDU]=
{
    /* Preparing select command for emv operation */
    20,
    /* ADF specific select command */
    13,
    /* Get Processing operation */
    8,
    /* Read record (sfi 2 record 1)operation */
    5,
    /* Read record (sfi 3 record 1)operation */
    5,
    /*  Read record (sfi 4 record 1)operation */
    5,
    /* Read record (sfi 4 record 2)operation */
    5,
    /* Preparing application cryptogram command */
    49
};

#if PHFL_LOG_ENABLE
static const char bApduInfo[PH_EXEMVCO_CIF_MAX_NO_APDU][50]=
{
   {"PPSE select command"},
   {"ADF specific select command"},
   {"Get processing operation command"},
   {"Performing read record command (sfi 2 record 1)"},
   {"Performing read record command (sfi 3 record 1)"},
   {"Performing read record command (sfi 4 record 1)"},
   {"Performing read record command (sfi 4 record 2)"},
   {"Generating application cryptogram"},
};

#endif

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Type A EMV operations entry point.
 * @param psDiscLoopParams : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExEMVCo_TypeAEMV(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;
    uint8_t bExitFlag = FALSE;
    uint8_t * ppRxBuffer;
    uint16_t pRxLength;
    uint8_t bApduCount = 0x00;

    do
    {
        status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_FWI,
                ((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bFwi);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_CID,
                ((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bCid);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        /* Set the buffer size to 256 */
        status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_FSI, 0x0802);
        if(status != PH_ERR_SUCCESS)
        {
            break;
        }

        LOG_TXT("Exchange at 106 Baud Rate\n");
        for(bApduCount = 0x00; bApduCount < PH_EXEMVCO_CIF_MAX_NO_APDU; bApduCount++)
        {
            /* Commands for emv operation */
            status = phpalI14443p4_Exchange(psDiscLoopParams->pPal14443p4DataParams,
                    PH_EXCHANGE_DEFAULT,
                    (uint8_t*)bApduCommandArray[bApduCount],
                    bApduCommandArrayLen[bApduCount],
                    &ppRxBuffer,
                    &pRxLength);

            if (status == PH_ERR_SUCCESS)
            {

                /* Check the select command response */
                if ((bApduCount == 0x00 ) && (pRxLength <= 2 ))
                {
                    /*  Not possible to have less than 2 bytes response */
                    LOG_TXT("ERROR: Not an EMV Card\n");
                    bExitFlag = TRUE;
                    break;
                }
                if ((ppRxBuffer[pRxLength - 2] != 0x90) || (ppRxBuffer[pRxLength - 1] != 0x00))
                {
                    LOG_TXT(bApduInfo[bApduCount]);
                    LOG_TXT(" failed\n");
                    bExitFlag = TRUE;
                    break;
                }
                else
                {
                    LOG_TXT(bApduInfo[bApduCount]);
                    LOG_TXT("\n");
                    LOG_AU8("Response:\n", ppRxBuffer, pRxLength);
                }
            }
            else
            {
                break;
            }
        }
        if(bExitFlag == TRUE)
        {
            status = PH_ERR_FAILED;
        }

#ifdef NXPBUILD__PH_CRYPTOSYM
        status = phalMfdf_FormatPICC(NULL);
#endif

    }while(0);
    /** De-select the card */
    (void)phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
    return status;
}

#endif
