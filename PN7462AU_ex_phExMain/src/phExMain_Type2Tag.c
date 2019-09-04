/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * phExMain_Type2Tag.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-29 11:20:15 +0530 (Thu, 29 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18680 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phpalMifare.h"
#include "phpalI14443p4.h"
#include "phUser.h"
#include "phpalI14443p4a.h"
#include "phpalI14443p3a.h"
#include "phalTop.h"
#include "phhalHw.h"
#include "ph_Log.h"
#include "phExMain_Type2Tag.h"

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
extern PH_NOINIT uint8_t gphExMain_pAtr[1024];
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExMain_Type2TagOp(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,phalTop_Sw_DataParams_t *palTop)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    //uint8_t bData[20];
    uint8_t bWData[12];
    uint16_t wLength = 0x00;

    do
    {
        (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_TYPE2TAG_READTIMEOUT_DEFAULT_MS);
        /** reading the data from card*/
        status = phalTop_ReadNdef(palTop,
            gphExMain_pAtr,
            &wLength
        );

        if((status == PH_ERR_SUCCESS) || ((status & PH_ERR_MASK) == PH_ERR_PROTOCOL_ERROR))
        {
            if(status == PH_ERR_SUCCESS)
            {
                LOG_AU8_D("Readdata",&bData[0],wLength);
            }
            else
            {
                LOG_TXT_D("Card is in Initialised state so writing the ndef message \n");
            }

            phUser_MemCpy(bWData, "\xD1\x01\x08\x55\x00\x6E\x78\x70\x2E\x63\x6F\x6D", sizeof(bWData));
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_TYPE2TAG_WRITETIMEOUT_DEFAULT_MS);
            status = phalTop_WriteNdef(palTop,
                bWData,
                sizeof(bWData)
            );
            if(status != PH_ERR_SUCCESS)
            {
                LOG_TXT_D("WriteFailed\n");
                break;
            }
            LOG_AU8_D("Writtendata\n",&bWData[0],sizeof(bWData));
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, PH_EXMAIN_TYPE2TAG_READTIMEOUT_DEFAULT_MS);
            status = phalTop_ReadNdef(palTop,
                gphExMain_pAtr,
                &wLength
            );

            if(status != PH_ERR_SUCCESS)
            {
                LOG_TXT_D("ReadFailed\n");
                break;
            }
            LOG_AU8_D("Readdata\n",&gphExMain_pAtr[0],wLength);
        }
        else
        {
            LOG_TXT_D("ReadFailed\n");
            break;
        }

        /*
         * For now ignoring the deselect status coz learnt that the UL card is taking more time reply back
         */
        /** De-select the card. */
        (void)phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
        LOG_TXT_D("Card de-selected\n");
    }while(0);


    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
