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
 * phExDoorAccessClif.c: Implements the Clif task and the logical operations related to contact less interface.
 *
 * Project:  PN7462AU
 *s
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <phExDoorAccess.h>
#include <phExDoorAccess_Cfg.h>
#include <phExDoorAccess_Clif.h>
#include <phExDoorAccess_Poll.h>
#include <phExDoorAccess_Utils.h>
#include "ph_Datatypes.h"

#include "phRtos.h"

#include "phhalPcr.h"
#include "phhalRf.h"
#include "phpalFelica.h"
#include "phhalHw.h"
#include "phFlashBoot_Event.h"
#include "phhalNvic.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phCfg_EE.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phhalPmu.h"
#include "phalT1T.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4a.h"
#include "phpalI14443p4.h"
#include "phpalI18000p3m3.h"
#include "phpalSli15693.h"
#include "phpalI18092mPI.h"
#include "phalI18000p3m3.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "phExDoorAccess_Log.h"

#ifdef PHFL_ENABLE_LLCPSNEP
#include "phpalI18092mPI.h"
#include "phOsal.h"
#include <phlnLlcp.h>
#include <phnpSnep.h>
#include "phExDoorAccessLLCP_Client.h"
#include "PN7462AU/PN7462AU_clif.h"
#endif

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

PH_NOINIT static uint8_t gphExDoorAccess_Rxbuf[PH_EXDOORACCESS_CLIF_RXBUFSIZE];
PH_NOINIT static uint8_t gphExDoorAccess_Txbuf[PH_EXDOORACCESS_CLIF_TXBUFSIZE];
phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */


PH_NOINIT static phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A;
PH_NOINIT static phpalI14443p4a_Sw_DataParams_t  gphpal_Sw_DataParams4A;
PH_NOINIT static phpalI14443p4_Sw_DataParams_t gphpal_Sw_DataParams4;
PH_NOINIT static phalT1T_Sw_DataParams_t gphpal_Sw_DataParamsT1T;
PH_NOINIT static phpalI14443p3b_Sw_DataParams_t  gphpal_Sw_DataParamsB;
PH_NOINIT static phpalFelica_Sw_DataParams_t gphpal_Sw_DataParamsF;
PH_NOINIT static phpalSli15693_Sw_DataParams_t gphpal_Sw_DataParams15693;
PH_NOINIT static phpalI18000p3m3_Sw_DataParams_t gphpal_Sw_DataParams18000;
PH_NOINIT static phalI18000p3m3_Sw_DataParams_t gphal_Sw_DataParams18000;
PH_NOINIT static phpalI18092mPI_Sw_DataParams_t  gphpal_Sw_DataParams18092;

#ifdef PHFL_ENABLE_LLCPSNEP
phlnLlcp_Sw_DataParams_t        slnLlcp;                    /* LLCP component holder */
static uint8_t   aGt[36];
static uint8_t   bGtLength = 0;
PH_NOINIT static uint8_t   aAtrRes[64] = {0};

#endif

static const uint8_t gkphExDoorAccess_NFCID3[10] = {0x03, 0x50, 0x71, 0xEF, 0x1F, 0xEE, 0xE9, 0x58, 0xA9, 0xF9};

static const uint8_t gkphExDoorAccess_NFCID2[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

#ifndef PHFL_ENABLE_STANDBY
PH_NOINIT phhalTimer_Timers_t *gpphExDoorAccess_Clif_PollTimer;
#endif

extern phhalTimer_Timers_t *pLedTimer;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExDoorAccess_Clif_MonitorCheckTxLdo();

#ifdef PHFL_ENABLE_LLCPSNEP
static void phExDoorAccess_Clif_LLCP_Init();
static void phExDoorAccess_Clif_LLCPParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);
#endif

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Contact less based card operations entry point.
 * @param eDiscLoopEntry : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExDoorAccess_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry)
{
    phStatus_t wDiscLoopStatus = PHAC_DISCLOOP_NO_TECH_DETECTED;
    uint16_t wStatus = PH_ERR_INTERNAL_ERROR;

    uint16_t wMeasuredAgcValue;
    uint16_t wGtLength;
    uint8_t bAts[64] ;

    phLED_SetPattern(gkphLED_BootUp);

	/* Initialize the parameters for polling/listen/active */
	phExDoorAccess_Clif_DiscLoopParamInit(psDiscLoopParams,bAts);

    if(psDiscLoopParams->bPasPollTechCfg || psDiscLoopParams->bActPollTechCfg)
    {
        /** LPCD configuration */
        if(psDiscLoopParams->bLpcdEnabled)
        {
            /** Configure if value needs to be different from default */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_DURATION, 30);
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD, 5);

            /** Read the Last measured value from GPREG and set the reference value. */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE,
                (uint16_t)PH_REG_GET( PCR_GPREG1_REG));
        }
    }

    wStatus = phExDoorAccess_Clif_MonitorCheckTxLdo();
    if (wStatus != PH_ERR_SUCCESS)
	{
		phhalTimer_Stop(pLedTimer);
		phExDoorAccess_Utils_Glow_Error_LEDS();
		return PHAC_DISCLOOP_NO_TECH_DETECTED;
	}

    wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
    wDiscLoopStatus &= PH_ERR_MASK;

    /** If LPCD returns object found, but the object is not valid card, then set backup
     * current AGC value for future reference.
     */
    if(wDiscLoopStatus == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
    {
        /** Store the Agc Value in GPREG1 so that it is persisted during standby */
        phhalHw_GetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE,
                &wMeasuredAgcValue);
        PH_REG_SET( PCR_GPREG1_REG, (uint32_t)wMeasuredAgcValue);
    }

    switch(wDiscLoopStatus)
    {
    /** Poll Device/Tech found. */
    case PHAC_DISCLOOP_DEVICE_ACTIVATED:
        phLED_SetPattern(gkphLED_Clif_Detect);
        phExDoorAccess_Poll_Main(psDiscLoopParams);
        break;

        /* Listen Device found. */
    case PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED:
    case PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED:
        //LOG_TXT("Peer Detected\n");
        phLED_SetPattern(gkphLED_Clif_Detect);
        {
            /* Assign the PAL 18092 Initiator Parameters once LLPC should be activated in Initiator Mode. */
            slnLlcp.pPalI18092DataParams = &gphpal_Sw_DataParams18092;

            /* Get the ATR_RES Length. */
            wStatus = phacDiscLoop_GetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, &wGtLength);

            bGtLength = (uint8_t)wGtLength;

            /* Active LLCP with the received ATR_RES in Initiator Mode. */
            wStatus = phlnLlcp_Activate(&slnLlcp,
                                        &aAtrRes[PHLN_LLCP_ATR_RES_MIN_LEN],
                                        (bGtLength - PHLN_LLCP_ATR_RES_MIN_LEN),
                                        PHLN_LLCP_INITIATOR
                                        );
            /* Release all the resources acquired by LLCP stack. */
            phlnLlcp_DeInit(&slnLlcp);

            /* Reset the 18092 Protocol. */
            wStatus = phpalI18092mPI_ResetProtocol(&gphpal_Sw_DataParams18092);

            phExDoorAccess_Utils_Glow_Error_LEDS();
        }
        break;

        /** No Device/Error found. */
    case PHAC_DISCLOOP_FAILURE:
    case PHAC_DISCLOOP_NO_TECH_DETECTED:
    case PHAC_DISCLOOP_NO_DEVICE_RESOLVED:
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED :
    case PHAC_DISCLOOP_MULTI_TECH_DETECTED:
    case PHAC_DISCLOOP_EXTERNAL_RFON:
    case PHAC_DISCLOOP_COLLISION_PENDING:
        break;
    default:
        /** It is not expected to reach here. */
        //PH_USER_ASSERT(0);
    	break;
    }

    /** Perform field off */
    (void)phhalHw_FieldOff(psDiscLoopParams->pHalDataParams);

    return (phStatus_t)wDiscLoopStatus;
}

/**
 * Performs the Clif HAL initialization.
 */
void phExDoorAccess_Clif_HalInit(void)
{
    uint16_t status;
	/** Clear the RX buf */
    phUser_MemSet((void *)&gphExDoorAccess_Rxbuf[0], 0x00, PH_EXDOORACCESS_CLIF_RXBUFSIZE);
    /** Clear the TX buf */
    phUser_MemSet((void *)&gphExDoorAccess_Txbuf[0], 0x00, PH_EXDOORACCESS_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
			gphExDoorAccess_Txbuf,
            sizeof(gphExDoorAccess_Txbuf),
			gphExDoorAccess_Rxbuf,
            sizeof(gphExDoorAccess_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}


/**
 * Performs Clif PAL initialization.
 *
 * @param psDiscLoopParams : Discovery loop parameters.
 * @return Status of the operation.
 */
phStatus_t phExDoorAccess_Clif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{

    /** ISO14443-3a PAL, T1T AL initialization if polling for Type A is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
    {
        phUser_MemSet((void *)&gphpal_Sw_DataParams3A, 0x00, sizeof(phpalI14443p3a_Sw_DataParams_t));
        phUser_MemSet((void *)&gphpal_Sw_DataParamsT1T, 0x00, sizeof(phalT1T_Sw_DataParams_t));
        phUser_MemSet((void *)&gphpal_Sw_DataParams4A, 0x00, sizeof(phpalI14443p4a_Sw_DataParams_t));

        if(phpalI14443p3a_Sw_Init(&gphpal_Sw_DataParams3A, sizeof( gphpal_Sw_DataParams3A),
                &sHal)!= PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        if(phpalI14443p4a_Sw_Init(&gphpal_Sw_DataParams4A, sizeof( gphpal_Sw_DataParams4A),
                &sHal)  != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        if( phalT1T_Sw_Init(&gphpal_Sw_DataParamsT1T,   sizeof( gphpal_Sw_DataParamsT1T),
                &gphpal_Sw_DataParams3A)  != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }

        psDiscLoopParams->pPal1443p3aDataParams = (void *)&gphpal_Sw_DataParams3A;
        psDiscLoopParams->pPal1443p4aDataParams = (void *)&gphpal_Sw_DataParams4A;

        psDiscLoopParams->pAlT1TDataParams = (void *)&gphpal_Sw_DataParamsT1T;

    }

    /** ISO14443-3b PAL initialization if polling for Type B is enabled */
    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParamsB, 0x00, sizeof(phpalI14443p3b_Sw_DataParams_t));

        if(phpalI14443p3b_Sw_Init(&gphpal_Sw_DataParamsB, sizeof(gphpal_Sw_DataParamsB), &sHal)
            != PH_ERR_SUCCESS){
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal1443p3bDataParams = (void *)&gphpal_Sw_DataParamsB;
    }

    /** ISO14443-4 PAL initialization if polling for Type A or Type B is enabled */
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
    /** FeliCa PAL initialization if polling for Type F is enabled */
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
        if(phpalSli15693_Sw_Init(&gphpal_Sw_DataParams15693, sizeof(gphpal_Sw_DataParams15693),
                &sHal) != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPalSli15693DataParams = (void *)&gphpal_Sw_DataParams15693;
    }

    if(psDiscLoopParams->bPasPollTechCfg & (1 << PHAC_DISCLOOP_TECH_TYPE_18000P3M3))
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParams18000, 0x00, sizeof(phpalI18000p3m3_Sw_DataParams_t));
        /** 18000 software initialisation for layer 3 */
        if(phpalI18000p3m3_Sw_Init(&gphpal_Sw_DataParams18000, sizeof(gphpal_Sw_DataParams18000),
                &sHal)  != PH_ERR_SUCCESS)
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

    /** Active configuration */
    if(psDiscLoopParams->bActPollTechCfg)
    {
        (void) phUser_MemSet((void *)&gphpal_Sw_DataParams18092, 0x00, sizeof(phpalI18092mPI_Sw_DataParams_t));

        if(phpalI18092mPI_Sw_Init(&gphpal_Sw_DataParams18092, sizeof( gphpal_Sw_DataParams18092),
                &sHal)  != PH_ERR_SUCCESS)
        {
            return PH_ERR_INTERNAL_ERROR;
        }
        psDiscLoopParams->pPal18092mPIDataParams = (void *)&gphpal_Sw_DataParams18092;

    }

#ifdef PHFL_ENABLE_LLCPSNEP
    phExDoorAccess_Clif_LLCP_Init();
    phExDoorAccess_Clif_LLCPParamInit(psDiscLoopParams);
#endif
    return PH_ERR_SUCCESS;
}

/**
 * Performs Clif De-initialization.
 * @param phhalHwClifRdLib : Hw Clif RdLib data parameters.
 */
void phExDoorAccess_Clif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib)
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

        /** Deinitialisng the bare metal HAL */
        wStatus = (phStatus_t) phhalRf_DeInit ();

        if (wStatus != PH_ERR_SUCCESS)
        {
            break;
        }

    }while(0);

}

uint16_t phExDoorAccess_Clif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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


    /** Link the global HalDataParams to the pDiscLoop params. */
     /* NOTE: gphphhalHw_Nfc_Ic_DataParams must be initialized by this time. */
    psDiscLoopParams->pHalDataParams = (void *)&sHal;

    psDiscLoopParams->sTargetParams.bRetryCount = 5;

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, true);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    /* Clear all the technlogies first */
    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, 0x00);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, 0x00);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0x00);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }
    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, 0x00);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, 0x00
            | PHAC_DISCLOOP_POS_BIT_MASK_A
            | PHAC_DISCLOOP_POS_BIT_MASK_B
            | PHAC_DISCLOOP_POS_BIT_MASK_F212
            | PHAC_DISCLOOP_POS_BIT_MASK_F424
            | PHAC_DISCLOOP_POS_BIT_MASK_V
            | PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3
          );
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

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0x00
            | PHAC_DISCLOOP_ACT_POS_BIT_MASK_106
            | PHAC_DISCLOOP_ACT_POS_BIT_MASK_212
            | PHAC_DISCLOOP_ACT_POS_BIT_MASK_424);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    /*If checking for type F */
    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_LRI, 3);

    /*If checking for type A */
    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_LRI, 3);

    return wStatus;
}

void phExDoorAccess_Clif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts)
{
    psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts = pbAts;

    phUser_MemCpy(&psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm[0], gkphExDoorAccess_NFCID3, 10);
}

static void phExDoorAccess_Clif_LLCPParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.bAtrResLength = 0;

	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bDid = 0;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bNadEnable = 0;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bNad = 0,

	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bGiLength = 0x00;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bAtrResLength = 0;
	phUser_MemCpy(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gkphExDoorAccess_NFCID2, 10);

	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pGi          = (uint8_t *)aGt;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bGiLength    = bGtLength;
	psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pAtrRes      = aAtrRes;

	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pGi          = (uint8_t *)aGt;
	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.bGiLength    = bGtLength;
	psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pAtrRes      = aAtrRes;
}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#ifdef PHFL_ENABLE_LLCPSNEP
static void phExDoorAccess_Clif_LLCP_Init()
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
#endif

static phStatus_t phExDoorAccess_Clif_MonitorCheckTxLdo()
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXDOORACCESS_CFG_USETXLDO_EXTERNAL)
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
