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
 * phExPos_TypeAEMV.c : Implements the core logic for Pos exchange operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExPos_TypeAEMV.h"
#include "phpalMifare.h"
#include "phpalI14443p4.h"
#include "phUser.h"
#include "phpalI14443p4a.h"
#include "phExPos_Log.h"
#include "phalMfdf.h"

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

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExPos_TypeAEMV(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
    phStatus_t status;
       do
       {

   		status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_FWI, ((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bFwi);
   		if(status != PH_ERR_SUCCESS)
   		{
   			break;
   		}

   		status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_CID, ((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bCid);
   		if(status != PH_ERR_SUCCESS)
   		{
   			break;
   		}


		status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_FSI, 0x0802);//((phpalI14443p4a_Sw_DataParams_t*)(psDiscLoopParams->pPal1443p4aDataParams))->bFsci);
		if(status != PH_ERR_SUCCESS)
		{
			break;

		}
		status = phpalI14443p4_SetConfig(psDiscLoopParams->pPal14443p4DataParams, PHPAL_I14443P4_CONFIG_MAXRETRYCOUNT, 8);
		if(status != PH_ERR_SUCCESS)
		{
			break;
		}


       }while(0);
    return status;
}

#endif
