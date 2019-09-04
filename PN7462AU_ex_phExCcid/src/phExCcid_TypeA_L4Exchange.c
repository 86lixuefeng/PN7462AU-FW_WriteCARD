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
 * phExCcid_TypeA_L4Exchange.c : Implements the core logic for the Type A level 4 exchange operations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-06-23 17:00:55 +0530 (Thu, 23 Jun 2016) $
 * $Author: nxp86397 $
 * $Revision: 3576 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExCcid_TypeA_L4Exchange.h"
#include "phpalMifare.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "ph_Log.h"
#include "phalMfdf.h"

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
phalMfdf_Sw_DataParams_t mfdf;
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
#define MAX_RETRY_COUNT   8

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
//extern uint8_t gphExCcid_UsbCcid_Ats[20];
extern phpalMifare_Sw_DataParams_t     *ppalMifare;
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExCcid_TypeA_L4Exchange(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	uint8_t cid;
	uint8_t cid_enabled;
	uint8_t nad_supported;
	uint8_t fwi;
	uint8_t fsdi;
	uint8_t fsci;
	phStatus_t status = PH_ERR_SUCCESS;


    do
    {
    	    phpalI14443p4a_GetProtocolParams(psDiscLoopParams->pPal1443p4aDataParams,
    										 &cid_enabled,
    										 &cid,
    										 &nad_supported,
    										 &fwi,
    										 &fsdi,
    										 &fsci);

    	    phpalI14443p4_SetProtocol(psDiscLoopParams->pPal14443p4DataParams,
    	    					      cid_enabled,
    	    						  cid,
									  nad_supported,
									  0x00,
									  fwi,
									  fsdi,
									  fsci);
    	    status = phpalMifare_Sw_Init(ppalMifare, sizeof(phpalMifare_Sw_DataParams_t), psDiscLoopParams->pHalDataParams, psDiscLoopParams->pPal14443p4DataParams);
		    if(status != PH_ERR_SUCCESS)
		    {
			    break;
		    }

		    status = phalMfdf_Sw_Init(&mfdf,
		        			 sizeof(phalMfdf_Sw_DataParams_t),
							 ppalMifare,
						     NULL,
						     NULL,
						     NULL,
							 psDiscLoopParams->pHalDataParams);
		    if(status != PH_ERR_SUCCESS)
		    {
		    	break;
		    }

    }while(0);

    return status;
}
#endif
