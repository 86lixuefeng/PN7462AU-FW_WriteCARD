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
 * phExNFCCcid_15693.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

/*******************************************************************************************************************/
/**   INCLUDES                                                                                                      */
/*******************************************************************************************************************/
#include "phalICode.h"
#include "phpalSli15693.h"
#include "phUser.h"
#include "phExNFCCcid_15693.h"
#include "ph_Log.h"
#ifdef NXPBUILD__PHPAL_SLI15693_SW
static phalICode_Sw_DataParams_t *palI15693;
static phalICode_Sw_DataParams_t salI15693;

/*******************************************************************************************************************/
/**   FUNCTION DEFINITIONS                                                                                          */
/*******************************************************************************************************************/

/** Performs 15693 operations. */
phStatus_t phExNFCCcid_15693(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    phStatus_t status = PH_ERR_INTERNAL_ERROR;

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
    } while(0);
    return status;
}
#endif
