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
 * phExNFCCcid_Clif.c: Implements the Clif task and the logical operations related to contact less interface.
 *
 * Project:  PN7462AU
 *s
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phhalPcr.h"
#include "phhalRf.h"
#include "phpalFelica.h"
#include "phhalHw.h"
#include "phFlashBoot_Event.h"
#include "phExNFCCcid.h"
#include "phExNFCCcid_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phExNFCCcid_Poll.h"
#include "phCfg_EE.h"
#include "phExNFCCcid_Cfg.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phExNFCCcid_LED.h"
#include "phExNFCCcid_Log.h"
#include "ph_Reg.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4a.h"
#include "phpalI14443p4.h"
#include "phpalSli15693.h"
#include "phhalPmu.h"

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
#include "phpalI18000p3m3.h"
#include "phalI18000p3m3.h"
#endif

#include "phpalI18092mPI.h"
#include "phOsal.h"
#include "phlnLlcp.h"
#include "phnpSnep.h"
#include "phExNFCCcid_LLCP_Client.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "phExNFCCcid_UsbCcid.h"
#include "phKeyStore.h"
/*
 * All the Hardware specific API's will be placed in this file.
 */

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
phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */
PH_ALIGN(4) static uint8_t gphExNFCCcid_Rxbuf[PH_EXNFCCCID_CLIF_RXBUFSIZE] = {0x00};
PH_ALIGN(4) static uint8_t gphExNFCCcid_Txbuf[PH_EXNFCCCID_CLIF_TXBUFSIZE] = {0x00};

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */

PH_NOINIT static phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A;
PH_NOINIT static phpalI14443p4a_Sw_DataParams_t  gphpal_Sw_DataParams4A;
PH_NOINIT static phpalI14443p4_Sw_DataParams_t   gphpal_Sw_DataParams4;
PH_NOINIT static phpalFelica_Sw_DataParams_t     gphpal_Sw_DataParamsF;
PH_NOINIT static phpalI14443p3b_Sw_DataParams_t  gphpal_Sw_DataParamsB;
PH_NOINIT static phpalSli15693_Sw_DataParams_t   gphpal_Sw_DataParams15693;
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
PH_NOINIT static phpalI18000p3m3_Sw_DataParams_t gphpal_Sw_DataParams18000;
PH_NOINIT static phalI18000p3m3_Sw_DataParams_t  gphal_Sw_DataParams18000;
#endif

#ifdef PHFL_LLCP_SNEP_ENABLE
PH_NOINIT static phpalI18092mPI_Sw_DataParams_t  gphpal_Sw_DataParams18092;
phlnLlcp_Sw_DataParams_t        slnLlcp;                    /* LLCP component holder */
static uint8_t   aGt[36];
static uint8_t   bGtLength = 0;
PH_NOINIT static uint8_t   aAtrRes[64] = {0x0};
#endif

PH_NOINIT phhalTimer_Timers_t *gpphExNFCCcid_Clif_PollTimer;
extern phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#ifdef PHFL_LLCP_SNEP_ENABLE
static void phExNFCCcid_Clif_LLCP_Init();
static void phExNFCCcid_Clif_LLCP_Param(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif
static phStatus_t phExNFCCcid_Clif_MonitorCheckTxLdo();
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExNFCCcid_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
		                        phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry)
{
    phStatus_t wDiscLoopStatus = PHAC_DISCLOOP_NO_TECH_DETECTED;
    uint16_t wGtLength;
    phStatus_t eStatus;
    uint8_t bAts[64];
    uint16_t wMeasuredAgcValue;
    /* Check the External TVDD supply is OK if TxLdo is external */
	eStatus = phExNFCCcid_Clif_MonitorCheckTxLdo();
	if (eStatus != PH_ERR_SUCCESS)
	{
		phLED_SetPattern(gkphLED_TVDD_Fail);
		return PHAC_DISCLOOP_NO_TECH_DETECTED;
	}
	else
	{
		phLED_SetPattern(gkphLED_BootUp);
		phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
	}

	/* Initialize the parameters for polling/listen/active */
	phExNFCCcidClif_DiscLoopParamInit(psDiscLoopParams,bAts);

	wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
	wDiscLoopStatus &= PH_ERR_MASK;

	/* If LPCD returns object found, but the object is not valid card, then set bacup
	 * current AGC value for future reference.
	 */
	if(wDiscLoopStatus == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
	{
		/* Store the Agc Value in GPREG1 so that it is persited during standby */
		phhalHw_GetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE, &wMeasuredAgcValue);
		PH_REG_SET( PCR_GPREG1_REG, (uint32_t)wMeasuredAgcValue);
	}

    switch(wDiscLoopStatus)
    {
    /*Poll Device/Tech found. */
    case PHAC_DISCLOOP_DEVICE_ACTIVATED:
    	if (gphExNFCCcid_sUsb_Operation_Mode.bOperationMode == 0) {
         phhalTimer_Stop(pLedTimer);
         phExNFCCcid_All_LED_Off();
         phExNFCCcid_Poll_Main(psDiscLoopParams);
    	}
         break;

    /* Listen Device found. */
    case PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED:
    case PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED:
#ifdef PHFL_LLCP_SNEP_ENABLE
         LOG_TXT("Peer Detected\n");
         phhalTimer_Stop(pLedTimer);
         phExNFCCcid_All_LED_Off();
         phExNFCCcid_All_LED_On();
         slnLlcp.pPalI18092DataParams = &gphpal_Sw_DataParams18092;

         eStatus = phacDiscLoop_GetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, &wGtLength);

         bGtLength = (uint8_t)wGtLength;

         eStatus = phlnLlcp_Activate(&slnLlcp,
                                     &aAtrRes[PHLN_LLCP_ATR_RES_MIN_LEN],
                                     (bGtLength - PHLN_LLCP_ATR_RES_MIN_LEN),
                                     PHLN_LLCP_INITIATOR
                                     );
         phlnLlcp_DeInit(&slnLlcp);
         /* Reset the 18092 Protocol. */
         eStatus = phpalI18092mPI_ResetProtocol(&gphpal_Sw_DataParams18092);
#endif
        break;

    /* No Device/Error found. */
    case PHAC_DISCLOOP_FAILURE:
    case PHAC_DISCLOOP_NO_TECH_DETECTED:
    case PHAC_DISCLOOP_NO_DEVICE_RESOLVED:
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED:
    case PHAC_DISCLOOP_MULTI_TECH_DETECTED:
    case PHAC_DISCLOOP_EXTERNAL_RFON:
    case PHAC_DISCLOOP_COLLISION_PENDING:
         break;
    default:
         /* It is not expected to reach here. */
         //PH_USER_ASSERT(0);
         break;
    }

    /* Perform phExCcidClif_DeInit */
	/* phExCcidClif_DeInit(psDiscLoopParams.pHalDataParams); */
	(void)phhalHw_FieldOff(psDiscLoopParams->pHalDataParams);

	return (phStatus_t)wDiscLoopStatus;

}

void phExNFCCcid_Clif_HalInit(void)
{
	uint16_t status;

	/* Clear the RX buffer */
	phUser_MemSet((void *)&gphExNFCCcid_Rxbuf[0], 0x00, PH_EXNFCCCID_CLIF_RXBUFSIZE);
	/** Clear the TX buffer */
	phUser_MemSet((void *)&gphExNFCCcid_Txbuf[0], 0x00, PH_EXNFCCCID_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
			gphExNFCCcid_Txbuf,
            sizeof(gphExNFCCcid_Txbuf),
			gphExNFCCcid_Rxbuf,
            sizeof(gphExNFCCcid_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}

phStatus_t phExNFCCcidClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	/* ISO14443-3a PAL, T1T AL initialization if polling for Type A is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        phUser_MemSet((void *)&gphpal_Sw_DataParams3A, 0x00, sizeof(phpalI14443p3a_Sw_DataParams_t));

        phUser_MemSet((void *)&gphpal_Sw_DataParams4A, 0x00, sizeof(phpalI14443p4a_Sw_DataParams_t));

        if(phpalI14443p3a_Sw_Init(&gphpal_Sw_DataParams3A, sizeof( gphpal_Sw_DataParams3A), &sHal)
            != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        if(phpalI14443p4a_Sw_Init(&gphpal_Sw_DataParams4A, sizeof( gphpal_Sw_DataParams4A), &sHal)
            != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }

        psDiscLoopParams->pPal1443p3aDataParams = (void *)&gphpal_Sw_DataParams3A;
        psDiscLoopParams->pPal1443p4aDataParams = (void *)&gphpal_Sw_DataParams4A;

    }

    /* ISO14443-3b PAL initialization if polling for Type B is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParamsB, 0x00, sizeof(phpalI14443p3b_Sw_DataParams_t));

        if(phpalI14443p3b_Sw_Init(&gphpal_Sw_DataParamsB, sizeof(gphpal_Sw_DataParamsB), &sHal)
            != PH_ERR_SUCCESS){
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal1443p3bDataParams = (void *)&gphpal_Sw_DataParamsB;
    }

    /* ISO14443-4 PAL initialization if polling for Type A or Type B is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & ((1 << PHAC_DISCLOOP_TECH_TYPE_A) | (1 << PHAC_DISCLOOP_TECH_TYPE_B)))
    {
        phUser_MemSet((void *)&gphpal_Sw_DataParams4, 0x00, sizeof(phpalI14443p4_Sw_DataParams_t));
        if(phpalI14443p4_Sw_Init(&gphpal_Sw_DataParams4, sizeof( gphpal_Sw_DataParams4), &sHal)
            != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal14443p4DataParams = (void *)&gphpal_Sw_DataParams4;
    }
    /* FeliCa PAL initialization if polling for Type F is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & ((1 << PHAC_DISCLOOP_TECH_TYPE_F212) | (1 << PHAC_DISCLOOP_TECH_TYPE_F424)))
    {
        phUser_MemSet((void *)&gphpal_Sw_DataParamsF, 0x00, sizeof(phpalFelica_Sw_DataParams_t));

        if(phpalFelica_Sw_Init(&gphpal_Sw_DataParamsF, sizeof(gphpal_Sw_DataParamsF), &sHal)
            != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPalFelicaDataParams = (void *)&gphpal_Sw_DataParamsF;
    }

    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_V))
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParams15693, 0x00, sizeof(phpalSli15693_Sw_DataParams_t));
        /** 15693 software initialisation for layer 3 */
        if(phpalSli15693_Sw_Init(&gphpal_Sw_DataParams15693, sizeof(gphpal_Sw_DataParams15693), &sHal)
            != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPalSli15693DataParams = (void *)&gphpal_Sw_DataParams15693;
    }

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_18000P3M3))
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParams18000, 0x00, sizeof(phpalI18000p3m3_Sw_DataParams_t));
        /** 18000 software initialisation for layer 3 */
        if(phpalI18000p3m3_Sw_Init(&gphpal_Sw_DataParams18000, sizeof(gphpal_Sw_DataParams18000), &sHal)
           != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal18000p3m3DataParams = (void *)&gphpal_Sw_DataParams18000;

        if(phalI18000p3m3_Sw_Init(&gphal_Sw_DataParams18000, sizeof(gphal_Sw_DataParams18000), &gphpal_Sw_DataParams18000)
           != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pAl18000p3m3DataParams = (void *)&gphal_Sw_DataParams18000;
    }
#endif
#ifdef PHFL_LLCP_SNEP_ENABLE
    if(psDiscLoopParams->bActPollTechCfg)
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParams18092, 0x00, sizeof(phpalI18092mPI_Sw_DataParams_t));

        if(phpalI18092mPI_Sw_Init(&gphpal_Sw_DataParams18092, sizeof( gphpal_Sw_DataParams18092),
                                   &sHal)  != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }

        psDiscLoopParams->pPal18092mPIDataParams = (void *)&gphpal_Sw_DataParams18092;

        phExNFCCcid_Clif_LLCP_Init();
        phExNFCCcid_Clif_LLCP_Param(psDiscLoopParams);
    }
#endif
    return PH_ERR_SUCCESS;
}


void phExNFCCcidClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib)
{
    phStatus_t wStatus = PH_ERR_INTERNAL_ERROR;

    do
    {
        /*switching off the rf field*/
        wStatus = phhalHw_FieldOff(phhalHwClifRdLib);

        if (wStatus != PH_ERR_SUCCESS)
        {
            break;
        }

        /* De-Initializing the bare metal HAL */
        wStatus = (phStatus_t) phhalRf_DeInit ();

        if (wStatus != PH_ERR_SUCCESS)
        {
            break;
        }

    }while(0);

}

uint16_t phExNFCCcidClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	uint16_t wStatus = PH_ERR_INTERNAL_ERROR;

	wStatus = phacDiscLoop_Sw_Init(psDiscLoopParams,
	                               sizeof(phacDiscLoop_Sw_DataParams_t),
	                               &sHal
	                              );

	if(wStatus != PH_ERR_SUCCESS)
	{
		return PH_ERR_INTERNAL_ERROR;
	}

	 /* Link the global HalDataParams to the pDiscLoop params.
	  * NOTE: gphphhalHw_Nfc_Ic_DataParams must be initialized by this time. */
	 psDiscLoopParams->pHalDataParams = (void *)&sHal;

	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, false);
	 if(wStatus != PH_ERR_SUCCESS)
	 {
		 return PH_ERR_INTERNAL_ERROR;
	 }


	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, 0
			 | PHAC_DISCLOOP_POS_BIT_MASK_A
			 | PHAC_DISCLOOP_POS_BIT_MASK_B
			 | PHAC_DISCLOOP_POS_BIT_MASK_F212
			 | PHAC_DISCLOOP_POS_BIT_MASK_F424
			 | PHAC_DISCLOOP_POS_BIT_MASK_V
			 | PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3
				);

	 if (wStatus != PH_ERR_SUCCESS)
	 {
		 return PH_ERR_INTERNAL_ERROR;
	 }
#ifdef PHFL_LLCP_SNEP_ENABLE
     wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0
	            		| PHAC_DISCLOOP_ACT_POS_BIT_MASK_212
	            		| PHAC_DISCLOOP_ACT_POS_BIT_MASK_424);
#else
     wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0);
#endif
	 if (wStatus != PH_ERR_SUCCESS)
	 {
		 return PH_ERR_INTERNAL_ERROR;
	 }

	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, 0x00);
	 if (wStatus != PH_ERR_SUCCESS)
	 {
		 return PH_ERR_INTERNAL_ERROR;
	 }

	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, 0x00);
	 if (wStatus != PH_ERR_SUCCESS)
	 {
		return PH_ERR_INTERNAL_ERROR;
	 }

	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_TYPEF_P2P_LRI, 3);
	 if(wStatus != PH_ERR_SUCCESS)
	 {
		return PH_ERR_INTERNAL_ERROR;
	 }

	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_TYPEA_P2P_LRI, 3);
	 if(wStatus != PH_ERR_SUCCESS)
	 {
		 return PH_ERR_INTERNAL_ERROR;
	 }

	 wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0
	           | PHAC_DISCLOOP_POS_BIT_MASK_A
	           | PHAC_DISCLOOP_POS_BIT_MASK_B
	           | PHAC_DISCLOOP_POS_BIT_MASK_F212
	           | PHAC_DISCLOOP_POS_BIT_MASK_F424
	           | PHAC_DISCLOOP_POS_BIT_MASK_V
	           );
	 if(wStatus != PH_ERR_SUCCESS)
	 {
		 return PH_ERR_INTERNAL_ERROR;
	 }

	 if(psDiscLoopParams->bPasPollTechCfg || psDiscLoopParams->bActPollTechCfg)
	 {
		 /* LPCD configuration */
		 if(psDiscLoopParams->bLpcdEnabled)
		 {
			 /* Configure if value needs to be different from default */
			 (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_DURATION, 1500);
			 (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD, 60);

			 /* Read the Last measured value from GPREG and set the reference value. */
			 (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE,
					 (uint16_t)PH_REG_GET( PCR_GPREG1_REG));
		 }
	 }
	 return wStatus;
}

void phExNFCCcidClif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts)
{
	psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts = pbAts;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#ifdef PHFL_LLCP_SNEP_ENABLE
static void phExNFCCcid_Clif_LLCP_Init()
{
	(void) phUser_MemSet((void *)&slnLlcp, 0x00, sizeof(phlnLlcp_Sw_DataParams_t));
	slnLlcp.sLocalLMParams.wMiu = 0x00;    /* 128 bytes only */
    slnLlcp.sLocalLMParams.wWks = 0x11;    /* SNEP & LLCP */
    slnLlcp.sLocalLMParams.bLto = 0xFF;
    slnLlcp.sLocalLMParams.bOpt = 0x02;

    slnLlcp.sLocalLMParams.bAvailableTlv = PHLN_LLCP_TLV_MIUX_MASK | PHLN_LLCP_TLV_WKS_MASK |
	                                       PHLN_LLCP_TLV_LTO_MASK | PHLN_LLCP_TLV_OPT_MASK;

    /* Initialize LLCP component */
    phlnLlcp_Sw_Init(&slnLlcp, sizeof(phlnLlcp_Sw_DataParams_t), aGt, &bGtLength);
}

static void phExNFCCcid_Clif_LLCP_Param(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pGi          = (uint8_t *)aGt;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bGiLength    = bGtLength;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pAtrRes      = aAtrRes;

	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pGi          = (uint8_t *)aGt;
	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.bGiLength    = bGtLength;
	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pAtrRes      = aAtrRes;
}
#endif
static phStatus_t phExNFCCcid_Clif_MonitorCheckTxLdo()
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXNFCCCID_USETXLDO_EXTERNAL)
	{
		/* Perform the Monitor Check of TxLdo */
		eStatus = phhalPmu_TxLdoMonitorCheck();
	}
	else
	{
		return PH_ERR_SUCCESS;
	}
	return eStatus;
}
