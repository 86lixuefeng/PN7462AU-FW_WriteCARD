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
 * phExMain_18000p3m3.c:  <The purpose and scope of this file>
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
#include "phalI18000p3m3.h"
#include "phExMain_18000p3m3.h"
#include "ph_Log.h"
#include <phhalHw.h>

#ifdef NXPBUILD__CLIF_RM
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXMAIN_WORD_COUNT          0x01
#define PH_EXMAIN_MEM_BANK            0x03
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

/** Performs 18000 operations. */
phStatus_t phExMain_18000p3m3(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    uint16_t wRxLength = 0;
    uint8_t *pRxbuffer;
    uint8_t bWordPtr = 0x00;
    uint8_t pTxbuffer[2] = {0x01, 0x02};

    do
    {

        /** performing read operation */
        status = phalI18000p3m3_Read(psDiscLoopParams->pAl18000p3m3DataParams,
            PH_EXMAIN_MEM_BANK,
            &bWordPtr,
            0x00,
            PH_EXMAIN_WORD_COUNT,
            &pRxbuffer,
            &wRxLength
        );
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Read failed Block:", PH_EXMAIN_MEM_BANK);
            break;
        }
        LOG_X32("Read successful of Block:", PH_EXMAIN_MEM_BANK);
        LOG_AU8_D("Read BlockData",pRxbuffer,wRxLength);
        /** performing read operation */
        status = phalI18000p3m3_Write(psDiscLoopParams->pAl18000p3m3DataParams,
                                      PHAL_I18000P3M3_AC_USE_COVER_CODING,
                                      PH_EXMAIN_MEM_BANK,
                                      &bWordPtr,
                                      0x00,
                                      pTxbuffer
                                      );
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Write failed of Block:", PH_EXMAIN_MEM_BANK);
            break;
        }
        LOG_X32("Write successful of Block:", PH_EXMAIN_MEM_BANK);
        LOG_AU8_D("written BlockData",pTxbuffer,sizeof(pTxbuffer));
        /** performing read operation */
        /** Set config for timeout */
        /*      refer ISO18000-3M3:Table 4 — Tag to interrogator link( M2 Tag:6b)*/


        status = phalI18000p3m3_Read(psDiscLoopParams->pAl18000p3m3DataParams,
                                    PH_EXMAIN_MEM_BANK,
                                    &bWordPtr,
                                    0x00,
                                    PH_EXMAIN_WORD_COUNT,
                                    &pRxbuffer,
                                    &wRxLength
                                    );
        if(status != PH_ERR_SUCCESS)
        {
            LOG_X32("Read failed Block:", PH_EXMAIN_MEM_BANK);
            break;
        }
        LOG_AU8_D("Read BlockData",pRxbuffer,wRxLength);


    } while(0);
    return status;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS*/
#endif /* NXPBUILD__CLIF_RM*/
