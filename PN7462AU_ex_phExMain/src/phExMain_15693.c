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
 * phExMain_15693.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-20 12:38:43 +0530 (Tue, 20 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18667 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */


#include "phalICode.h"
#include "phpalSli15693.h"
#include "phExMain_15693.h"
#include "ph_Log.h"
#include "phhalRf.h"

#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXMAIN_FLAG                 0x26
#define PH_EXMAIN_AFI                  0x00
#define PH_EXMAIN_BLOCK                0x03
#define PH_EXMAIN_NO_OF_BLOCK          0x01
#define PH_EXMAIN_MASK_BIT_LEN         0x00

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

/** Performs 15693 operations. */
phStatus_t phExMain_15693(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    uint8_t *pRxbuffer;
    uint8_t pTxBuffer[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t pUidout[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t baMask[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t bUidlength;
    uint16_t bDataLength = 0x00;
    phalICode_Sw_DataParams_t *palI15693;
    phalICode_Sw_DataParams_t salI15693;

    palI15693 = &salI15693;

    do
    {
        /** initialising application layer */
        status = phalICode_Sw_Init(palI15693,
                sizeof(salI15693),
            psDiscLoopParams->pPalSli15693DataParams,
            NULL,
            NULL,
            NULL);

        if( status!= PH_ERR_SUCCESS){
            break;
        }

        /** performing read single block */
        status = phalICode_ReadSingleBlock(palI15693,
            PHAL_ICODE_OPTION_ON,
            PH_EXMAIN_BLOCK,
            &pRxbuffer,
            &bDataLength
        );

        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Read failed Block:", PH_EXMAIN_BLOCK);
            break;
        }
        LOG_AU8_D("BlockData",pRxbuffer+1, (bDataLength - 1));

        bDataLength  = 0x04;
        /** writing single block */
        status = phalICode_WriteSingleBlock(palI15693,
            PHAL_ICODE_OPTION_OFF,
            PH_EXMAIN_BLOCK,
            pTxBuffer,
            bDataLength
        );

        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Write failed of Block:", PH_EXMAIN_BLOCK);
            break;
        }
        LOG_X32("Write successful of Block:", PH_EXMAIN_BLOCK);
        LOG_AU8_D("Data Written ",pTxBuffer,bDataLength);
        /** performing read single block to check the data written*/
        status = phalICode_ReadSingleBlock(palI15693,
            PHAL_ICODE_OPTION_ON,
            PH_EXMAIN_BLOCK,
            &pRxbuffer,
            &bDataLength
        );
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Read failed Block:", PH_EXMAIN_BLOCK);
            break;
        }
        LOG_AU8_D("BlockData",pRxbuffer+1,(bDataLength-1));


        /** Performing inventory read on 15693 */
        status = phpalSli15693_InventoryRead(psDiscLoopParams->pPalSli15693DataParams,
            PH_EXMAIN_FLAG,
            PH_EXMAIN_AFI,
            baMask,
            PH_EXMAIN_MASK_BIT_LEN,
            PH_EXMAIN_BLOCK,
            PH_EXMAIN_NO_OF_BLOCK,
            pUidout,
            &bUidlength,
            pRxbuffer,
            &bDataLength
        );

        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Inventory Read failed Block:", PH_EXMAIN_BLOCK);
            LOG_TXT("Card Does Not Support Inventory Read!!!\n");
            LOG_TXT("Please use NXP Manufactured ICODE SLIX Cards!!!\n");
            break;
        }
        LOG_AU8_D("Inventory Read BlockData",pRxbuffer, bDataLength);


        phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_ASK100, PH_OFF);

        /* Performing fast inventory read on 15693 with higher baud rate */
        status = phpalSli15693_FastInventoryRead(psDiscLoopParams->pPalSli15693DataParams,
                psDiscLoopParams->sTypeVTargetInfo.bFlag | PHPAL_SLI15693_FLAG_NBSLOTS | PHPAL_SLI15693_FLAG_INVENTORY,
                PH_EXMAIN_AFI,
                NULL,
                0,
                PH_EXMAIN_BLOCK,
                PH_EXMAIN_NO_OF_BLOCK,
                pUidout,
                &bUidlength,
                pRxbuffer,
                &bDataLength);

        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Fast Inventory Read failed Block:", PH_EXMAIN_BLOCK);
            LOG_TXT("Card Does Not Support Fast Inventory Read!!!\n");
            LOG_TXT("Please use NXP Manufactured ICODE SLIX Cards!!!\n");
            break;
        }
        LOG_AU8_D("Fast Inventory Read BlockData",pRxbuffer, bDataLength);

    } while(0);
    return status;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS */
#endif /* NXPBUILD__CLIF_RM*/
