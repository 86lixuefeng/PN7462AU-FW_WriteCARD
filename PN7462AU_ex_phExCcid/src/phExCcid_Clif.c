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
 * phExCcid_Clif.c: Implements the Clif task and the logical operations related to contact less interface.
 *
 * Project:  PN7462AU
 *s
 * $Date: 2016-07-05 15:53:53 +0530 (Tue, 05 Jul 2016) $
 * $Author: nxp86397 $
 * $Revision: 3651 $ (v05.19.00)
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
#include "phExCcid.h"
#include "phExCcid_Clif.h"
#include "phacDiscLoop.h"
#include "ph_Status.h"
#include "phExCcid_Poll.h"
#include "phCfg_EE.h"
#include "phExCcid_Cfg.h"
#include "phUser.h"
#include "ph_Log.h"
#include "phExCcid_LED.h"
#include "phExCcid_Log.h"
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

#include "phExCcid_UsbCcid.h"
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
static uint8_t gphExCcid_Rxbuf[PH_EXCCID_CLIF_RXBUFSIZE] = {0};
static uint8_t gphExCcid_Txbuf[PH_EXCCID_CLIF_TXBUFSIZE] = {0};

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */


PH_NOINIT static phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A;
PH_NOINIT static phpalI14443p4a_Sw_DataParams_t  gphpal_Sw_DataParams4A;
PH_NOINIT static phpalI14443p4_Sw_DataParams_t gphpal_Sw_DataParams4;
PH_NOINIT static phpalFelica_Sw_DataParams_t gphpal_Sw_DataParamsF;
PH_NOINIT static phpalI14443p3b_Sw_DataParams_t  gphpal_Sw_DataParamsB;
PH_NOINIT static phpalSli15693_Sw_DataParams_t gphpal_Sw_DataParams15693;
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
PH_NOINIT static phpalI18000p3m3_Sw_DataParams_t gphpal_Sw_DataParams18000;
PH_NOINIT static phalI18000p3m3_Sw_DataParams_t gphal_Sw_DataParams18000;
#endif

PH_NOINIT phhalTimer_Timers_t *gpphExCcid_Clif_PollTimer;

extern phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phExCcid_Clif_MonitorCheckTxLdo();

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phExCcid_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
                             phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry)
{
    phStatus_t wDiscLoopStatus = PHAC_DISCLOOP_NO_TECH_DETECTED;
    uint16_t wMeasuredAgcValue;
    phStatus_t eStatus;

    /* Check the External TVDD supply is OK if TxLdo is external */
	eStatus = phExCcid_Clif_MonitorCheckTxLdo();
	if (eStatus != PH_ERR_SUCCESS)
	{
		phLED_SetPattern(gkphLED_TVDD_Fail);
		return PHAC_DISCLOOP_NO_TECH_DETECTED;
	}
	else
	{
       phLED_SetPattern(gkphLED_BootUp);
	}

    wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
    wDiscLoopStatus &= PH_ERR_MASK;

    /* If LPCD returns object found, but the object is not valid card, then set bacup
     * current AGC value for future reference.
     */
    if(wDiscLoopStatus == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
    {
        /* Store the Agc Value in GPREG1 so that it is persisted during standby*/
        phhalHw_GetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE, &wMeasuredAgcValue);
        PH_REG_SET( PCR_GPREG1_REG, (uint32_t)wMeasuredAgcValue);
    }

    switch(wDiscLoopStatus)
    {
    /*Poll Device/Tech found. */
    case PHAC_DISCLOOP_DEVICE_ACTIVATED:
        phhalTimer_Stop(pLedTimer);
        phExCcid_All_LED_Off();
        phExCcid_Poll_Main(psDiscLoopParams);
        break;

        /* Listen Device found. */
    case PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED:
        LOG_TXT("Peer Detected\n");

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


void phExCcid_Clif_HalInit(void)
{
	uint16_t status;

	/* Clear the RX buffer */
    phUser_MemSet((void *)&gphExCcid_Rxbuf[0], 0x00, PH_EXCCID_CLIF_RXBUFSIZE);
    /* Clear the TX buffer*/
    phUser_MemSet((void *)&gphExCcid_Txbuf[0], 0x00, PH_EXCCID_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
			gphExCcid_Txbuf,
            sizeof(gphExCcid_Txbuf),
			gphExCcid_Rxbuf,
            sizeof(gphExCcid_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}



phStatus_t phExCcidClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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

    return PH_ERR_SUCCESS;
}

void phExCcidClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib)
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

        /* Deinitialisng the bare metal HAL */
        wStatus = (phStatus_t) phhalRf_DeInit ();

        if (wStatus != PH_ERR_SUCCESS)
        {
            break;
        }

    }while(0);

}

uint16_t phExCcidClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
    psDiscLoopParams->pHalDataParams = &sHal;

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, false);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    /* Clear all the technologies first */
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

    if(psDiscLoopParams->bPasPollTechCfg || psDiscLoopParams->bActPollTechCfg)
    {
        /* LPCD configuration*/
        if(psDiscLoopParams->bLpcdEnabled)
        {
            /* Configure if value needs to be different from default */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_DURATION, 1500);
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD, 60);

            /* Read the Last measured value from GPREG and set the reference value.*/
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE,
                (uint16_t)PH_REG_GET( PCR_GPREG1_REG));
        }
    }
    return wStatus;
}

void phExCcidClif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts)
{
	psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts = pbAts;
}

static phStatus_t phExCcid_Clif_MonitorCheckTxLdo()
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXCCID_USETXLDO_EXTERNAL)
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

