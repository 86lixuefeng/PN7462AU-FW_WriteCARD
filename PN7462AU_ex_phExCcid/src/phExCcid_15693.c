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
 * phExCcid_15693.c:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-04-21 11:46:50 +0530 (Tue, 21 Apr 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 11652 $ (v05.19.00)
 */

/*******************************************************************************************************************/
/**   INCLUDES                                                                                                      */
/*******************************************************************************************************************/
#include "phalICode.h"
#include "phpalSli15693.h"
#include "phUser.h"
#include "phExCcid_15693.h"
#include "ph_Log.h"
#ifdef NXPBUILD__PHPAL_SLI15693_SW

/*******************************************************************************************************************/
/**   FUNCTION DEFINITIONS                                                                                          */
/*******************************************************************************************************************/

/** Performs 15693 operations. */
phStatus_t phExCcid_15693(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	phalICode_Sw_DataParams_t *palI15693;
	phalICode_Sw_DataParams_t salI15693;
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
