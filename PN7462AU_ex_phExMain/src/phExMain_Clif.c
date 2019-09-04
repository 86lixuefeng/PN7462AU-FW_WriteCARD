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
 * phExMainClif.c: Implements the Clif task and the logical operations related to contact less interface.
 *
 * Project:  PN7462AU
 *s
 * $Date: 2016-12-01 14:14:28 +0530 (Thu, 01 Dec 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18767 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phRtos.h"

#include "ph_Log.h"
#include "ph_Status.h"
#include "phacDiscLoop.h"
#include "phalI18000p3m3.h"
#include "phalT1T.h"
#include "phExMain.h"
#include "phExMain_ActIni.h"
#include "phExMain_CardMode.h"
#include "phExMain_Cfg.h"
#include "phExMain_Clif.h"
#include "phExMain_Log.h"
#include "phExMain_PasIni.h"
#include "phExMain_PasTgt.h"
#include "phExMain_Poll.h"
#include "phFlashBoot_Event.h"
#include "phhalHw.h"
#include "phhalPcr.h"
#include "phhalRf.h"
#include "phhalTimer.h"
#include "phLED.h"
#include "phpalFelica.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p3b.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "phpalI18000p3m3.h"
#include "phpalI18092mPI.h"
#include "phpalSli15693.h"
#include "phUser.h"
#include "phhalSysSer.h"
#include "ph_Reg.h"
#include "phCfg_EE.h"
#include "phhalPmu.h"
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
#define PH_EXMAIN_TESTBUS1_SELECT                                   0x01
#define PH_EXMAIN_TESTBUS2_SELECT                                   0x02
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */
PH_NOINIT static uint8_t gphExMain_Rxbuf[PH_EXMAIN_CLIF_RXBUFSIZE];
PH_NOINIT static uint8_t gphExMain_Txbuf[PH_EXMAIN_CLIF_TXBUFSIZE];

#if defined(NXPBUILD__PH_KEYSTORE_SW)
phKeyStore_Sw_DataParams_t sKeyStore;
#   define PDATAPARAMS_SKEYSTORE    (&sKeyStore)
#else /* NXPBUILD__PH_KEYSTORE_SW */
#   define PDATAPARAMS_SKEYSTORE (NULL)
#endif /* NXPBUILD__PH_KEYSTORE_SW */


#ifdef NXPBUILD__CLIF_RM
PH_NOINIT phpalI14443p3a_Sw_DataParams_t  gphpal_Sw_DataParams3A;
PH_NOINIT static phpalI14443p4a_Sw_DataParams_t  gphpal_Sw_DataParams4A;
PH_NOINIT static phpalI14443p4_Sw_DataParams_t gphpal_Sw_DataParams4;
PH_NOINIT static phalT1T_Sw_DataParams_t gphpal_Sw_DataParamsT1T;
PH_NOINIT static phpalI14443p3b_Sw_DataParams_t  gphpal_Sw_DataParamsB;
PH_NOINIT static phpalFelica_Sw_DataParams_t gphpal_Sw_DataParamsF;
PH_NOINIT static phpalSli15693_Sw_DataParams_t gphpal_Sw_DataParams15693;
PH_NOINIT static phpalI18000p3m3_Sw_DataParams_t gphpal_Sw_DataParams18000;
PH_NOINIT static phalI18000p3m3_Sw_DataParams_t gphal_Sw_DataParams18000;
#endif /* NXPBUILD__CLIF_RM*/

#ifdef NXPBUILD__CLIF_CM
PH_NOINIT static phSysSv_Clif_CardMode_TypeA_Param_t gphExMain_A;
PH_NOINIT static phhalRf_CardMode_TypeF_Param_t gphExMain_F;
static const uint8_t gkphExMain_SensFRes[] = {0x12, 0x01, 0x01, 0x27, 0x00, 0x68, 0x53, 0xDD, 0xA6, 0xCA, 0x00,
    0xF0, 0x00, 0x00, 0x02, 0x06, 0x03, 0x00};
static const uint8_t gkphExMain_bSystemCode[2] = {0xFF,0xFF};
#endif /* NXPBUILD__CLIF_CM*/

PH_NOINIT uint8_t gphExMain_pAtr[1024];
#if defined (NXPBUILD__CLIF_ACTIVE_INITIATOR) || defined (NXPBUILD__CLIF_ACTIVE_TARGET)
PH_NOINIT static phpalI18092mPI_Sw_DataParams_t  gphpal_Sw_DataParams18092 = {0};

static const uint8_t gkphExMain_GeneralBytesI[22] = {0x46, 0x66, 0x6D, 0x01, 0x01, 0x11,0x02, 0x02,
                                                    0x07, 0xFF, 0x03, 0x02, 0x00, 0x13,
                                                    0x04, 0x01, 0x64, 0x07, 0x01,0x03, 0x5C, 0xE7};

static const uint8_t gkphExMain_NFCID3[10] = {0x03, 0x50, 0x71, 0xEF, 0x1F, 0xEE, 0xE9, 0x58, 0xA9, 0xF9};

static const uint8_t gkphExMain_NFCID2[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

//PH_NOINIT static uint8_t gphExMain_pAtr[128];
PH_NOINIT static uint8_t gphExMain_pGi[22];
#endif /* defined (NXPBUILD__CLIF_ACTIVE_INITIATOR) || defined (NXPBUILD__CLIF_ACTIVE_TARGET) */

uint8_t gphExMain_Clif_PollLoopLOGCount; /** Don't log Card Not Detected too much */
#ifndef PHFL_ENABLE_STANDBY
PH_NOINIT phhalTimer_Timers_t *gpphExMain_Clif_PollTimer;
#endif
static phStatus_t phExMain_Clif_TxLdoMonitorCheck(void);
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Contact less based card operations entry point.
 * @param eDiscLoopEntry : Discovery loop data params.
 * @return Status of the operation.
 */
phStatus_t phExMain_ClifMain(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams,
                             phacDiscLoop_Sw_EntryPoints_t eDiscLoopEntry)
{
    phStatus_t wDiscLoopStatus = PHAC_DISCLOOP_NO_TECH_DETECTED;
    uint16_t wStatus = PH_ERR_INTERNAL_ERROR;

#ifdef NXPBUILD__CLIF_RM
    uint16_t wMeasuredAgcValue;
#endif /* NXPBUILD__CLIF_RM*/

#ifdef NXPBUILD__CLIF_CM
    uint16_t wListenBaudRate = 0x00;
#endif /* NXPBUILD__CLIF_CM*/

    phLED_SetPattern(gkphLED_BootUp);

#ifdef NXPBUILD__CLIF_RM
    if ( 0 == gphExMain_Clif_PollLoopLOGCount ) {
        LOG_TXT("Entering Polling mode..\n");
    }
    gphExMain_Clif_PollLoopLOGCount++; /** Let it roll back. */

    /* Check the External TVDD supply is OK if TxLdo is external */
    wStatus = phExMain_Clif_TxLdoMonitorCheck();
    if (wStatus != PH_ERR_SUCCESS)
    {
        phLED_SetStatus(LED_R);
        LOG_TXT("phExMain: No External TVDD Present \n");
        return PHAC_DISCLOOP_NO_TECH_DETECTED;
    }

    wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
    wDiscLoopStatus &= PH_ERR_MASK;
    phExMain_Log_DiscLoop(wDiscLoopStatus, gphExMain_Clif_PollLoopLOGCount);
    /** If LPCD returns object found, but the object is not valid card, then set backup
     * current AGC value for future reference.
     */
    if(wDiscLoopStatus == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
    {
        /** Store the Agc Value in GPREG1 so that it is persisted during standby */
        (void)phhalHw_GetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE,
                &wMeasuredAgcValue);
        PH_REG_SET( PCR_GPREG1_REG, (uint32_t)wMeasuredAgcValue);
    }

    switch(wDiscLoopStatus)
    {
    /** Poll Device/Tech found. */
    case PHAC_DISCLOOP_DEVICE_ACTIVATED:
        gphExMain_Clif_PollLoopLOGCount = 0;
        phLED_SetPattern(gkphLED_Clif_Detect);
#ifdef NXPBUILD__CLIF_RM
        phExMain_Poll_Main(psDiscLoopParams);
#endif /* NXPBUILD__CLIF_RM*/
        break;

        /** Listen Device found. */
    case PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED:
        phLED_SetPattern(gkphLED_Clif_Detect);
#ifdef NXPBUILD__CLIF_ACTIVE_INITIATOR
        wStatus = phExMain_ActIni(wDiscLoopStatus, psDiscLoopParams);
        LOG_TXT("Peer Detected.1\n");
        if(wStatus == PH_ERR_SUCCESS)
        {
            phLED_SetPattern(gkphLED_Clif_Pass);
        }
        else
        {
            phLED_SetPattern(gkphLED_Clif_Fail);
        }
#endif /* NXPBUILD__CLIF_ACTIVE*/
        break;

    case PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED:
        phLED_SetPattern(gkphLED_Clif_Detect);
        wStatus = phExMain_PasIni(psDiscLoopParams);
        LOG_TXT("Peer Detected\n");
        if(wStatus == PH_ERR_SUCCESS)
        {
            phLED_SetPattern(gkphLED_Clif_Pass);
        }
        else
        {
            phLED_SetPattern(gkphLED_Clif_Fail);
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
    case PHAC_DISCLOOP_MERGED_SEL_RES_FOUND:
    case PH_ERR_INTERNAL_ERROR:
#endif /* NXPBUILD__CLIF_RM*/
#ifdef NXPBUILD__CLIF_CM

        if(psDiscLoopParams->bPasLisTechCfg || psDiscLoopParams->bActLisTechCfg)
        {
            eDiscLoopEntry = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;

            wDiscLoopStatus = phhalHw_PN7462AU_SetListenParam(psDiscLoopParams->pHalDataParams,
                    gphExMain_A.bAtqa,gphExMain_A.bUid,gphExMain_A.bSak,
                    FALSE,(uint8_t *)(&gkphExMain_SensFRes[4]),(uint8_t *)(&gkphExMain_SensFRes[10]),
                    (uint8_t *)gkphExMain_bSystemCode);
        }
        wDiscLoopStatus = phacDiscLoop_Run(psDiscLoopParams, (uint8_t)eDiscLoopEntry);
        wDiscLoopStatus &= PH_ERR_MASK;
        phExMain_Log_DiscLoop(wDiscLoopStatus, gphExMain_Clif_PollLoopLOGCount);
        switch(wDiscLoopStatus)
        {
            /** Listen Device found. */
            case PHAC_DISCLOOP_ACTIVATED_BY_PEER:
            {
                LOG_TXT("LISTEN Peer Detected\n");
                phLED_SetPattern(gkphLED_Clif_Detect);
                (void)phhalHw_GetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_TXDATARATE_FRAMING, &wListenBaudRate);
                switch(wListenBaudRate & PHHAL_HW_RF_FRAMING_OPTION_MASK)
                {
                    case PHHAL_HW_RF_TYPE_A_FRAMING:
                        if (psDiscLoopParams->sTargetParams.pRxBuffer[1] == 0xD4)
                        {

                            wStatus = phExMain_PasTgt(psDiscLoopParams);
                        }
                        else
                        {
                            wStatus = phExMain_CardMode(wDiscLoopStatus, psDiscLoopParams);
                        }
                    break;

                    case PHHAL_HW_RF_TYPE_ACTIVE_FRAMING:
                        wStatus = phExMain_PasTgt(psDiscLoopParams);
                    break;
                    default:
                    break;
                }
                if(wStatus == PH_ERR_SUCCESS)
                {
                    phLED_SetPattern(gkphLED_Clif_Pass);
                }
                else
                {
                    phLED_SetPattern(gkphLED_Clif_Fail);
                }
            }
            break;

            case PHAC_DISCLOOP_DEVICE_ACTIVATED:
            case PHAC_DISCLOOP_FAILURE:
            case PHAC_DISCLOOP_NO_TECH_DETECTED:
            case PHAC_DISCLOOP_NO_DEVICE_RESOLVED:
            case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED :
            case PHAC_DISCLOOP_MULTI_TECH_DETECTED:
            case PHAC_DISCLOOP_EXTERNAL_RFOFF:
            case PHAC_DISCLOOP_COLLISION_PENDING:
            case PHAC_DISCLOOP_MERGED_SEL_RES_FOUND:
                break;

            default:
                /** It is not expected to reach here. */
                PH_USER_ASSERT(0);
            }
#endif /* NXPBUILD__CLIF_CM*/

#ifdef NXPBUILD__CLIF_RM
        break;

    default:
        /** It is not expected to reach here. */
        PH_USER_ASSERT(0);

    }

    /** Perform field off */
        (void)phhalHw_FieldOff(psDiscLoopParams->pHalDataParams);
#endif /* NXPBUILD__CLIF_RM*/

    return (phStatus_t)wDiscLoopStatus;
}

/**
 * Performs Clif PAL initialization.
 *
 * @param psDiscLoopParams : Discovery loop parameters.
 * @return Status of the operation.
 */
phStatus_t phExMainClif_PalInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
{
#ifdef NXPBUILD__CLIF_RM
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
    if((psDiscLoopParams->bPasPollTechCfg) && ((1 << PHAC_DISCLOOP_TECH_TYPE_A) || (1 << PHAC_DISCLOOP_TECH_TYPE_B)))
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
#endif /* NXPBUILD__CLIF_RM*/
#ifdef NXPBUILD__CLIF_ACTIVE_INITIATOR
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

#endif /* NXPBUILD__CLIF_ACTIVE_INITIATOR */
    return PH_ERR_SUCCESS;
}

/**
 * Performs Clif De-initialization.
 * @param phhalHwClifRdLib : Hw Clif RdLib data parameters.
 */
void phExMainClif_DeInit(phhalHw_PN7462AU_DataParams_t *phhalHwClifRdLib)
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

/**
 * Test bus configuration
 */
uint16_t phExMainClif_TestBus(void)
{
    uint16_t wStatus = PH_ERR_INTERNAL_ERROR;
    /* GPIO4 = tx_active.
     * GPIO5 = rx_active. */
    uint8_t aTestBus1Config[] = {0x02, PH_EXMAIN_TESTBUS1_SELECT};
    uint8_t aTestBus2Config[] = {0x06, PH_EXMAIN_TESTBUS2_SELECT};

    /* Enable the Digital Test Bus. */
    wStatus = phhalSysSer_CLIF_Config_DigitalTestBus((void *)aTestBus1Config);
    if (wStatus != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
        return PH_ERR_INTERNAL_ERROR;
    }

    wStatus = phhalSysSer_CLIF_Config_DigitalTestBus((void *)aTestBus2Config);
    if (wStatus != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
        return PH_ERR_INTERNAL_ERROR;
    }
    return wStatus;
}

uint16_t phExMainClif_DiscLoopConfig(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
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
     /* NOTE: sHal must be initialized by this time. */
    psDiscLoopParams->pHalDataParams = (void *)&sHal;

    psDiscLoopParams->sTargetParams.bRetryCount = 5;

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, false);
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


#ifdef NXPBUILD__CLIF_RM

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
#endif /* NXPBUILD__CLIF_RM */

#ifdef NXPBUILD__CLIF_CM

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, 0x00
            | PHAC_DISCLOOP_POS_BIT_MASK_A
            | PHAC_DISCLOOP_POS_BIT_MASK_F212
            | PHAC_DISCLOOP_POS_BIT_MASK_F424);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

#endif /* NXPBUILD__CLIF_CM */


#ifdef NXPBUILD__CLIF_ACTIVE_TARGET

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, 0x00
            | PHAC_DISCLOOP_POS_BIT_MASK_A
            | PHAC_DISCLOOP_POS_BIT_MASK_F212
            | PHAC_DISCLOOP_POS_BIT_MASK_F424);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }

#endif /* NXPBUILD__CLIF_ACTIVE_TARGET */

#ifdef NXPBUILD__CLIF_ACTIVE_INITIATOR

    wStatus = phacDiscLoop_SetConfig(psDiscLoopParams,PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0x00
            | PHAC_DISCLOOP_POS_BIT_MASK_A
            | PHAC_DISCLOOP_POS_BIT_MASK_F212
            | PHAC_DISCLOOP_POS_BIT_MASK_F424);
    if(wStatus != PH_ERR_SUCCESS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }


#endif /* NXPBUILD__CLIF_ACTIVE_INITIATOR */


    if(psDiscLoopParams->bPasPollTechCfg || psDiscLoopParams->bActPollTechCfg)
    {
        /** LPCD configuration */
        if(psDiscLoopParams->bLpcdEnabled)
        {
            /** Configure if value needs to be different from default */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_DURATION, 1500);
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD, 60);

            /** Read the Last measured value from GPREG and set the reference value. */
            (void)phhalHw_SetConfig(psDiscLoopParams->pHalDataParams, PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE,
                (uint16_t)PH_REG_GET( PCR_GPREG1_REG));
        }
    }
    return wStatus;
}

void phExMainClif_DiscLoopParamInit(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams, uint8_t* pbAts)
{

#ifdef NXPBUILD__CLIF_RM

    psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts = pbAts;

#endif /* NXPBUILD__CLIF_RM*/

#ifdef NXPBUILD__CLIF_CM
    gphExMain_A.bAtqa[0] = 0x44;
    gphExMain_A.bAtqa[1] = 0x33;
    gphExMain_A.bSak = 0x40;
    gphExMain_A.bEntryState = PHHAL_RF_CMA_ENTRY_STATE_IDLE;
    gphExMain_A.bUid[0] = 0x01;
    gphExMain_A.bUid[1] = 0x02;
    gphExMain_A.bUid[2] = 0x03;
    gphExMain_A.bCurrentState = 0x00;

    gphExMain_F.bSensfRespLen = gkphExMain_SensFRes[0];
    gphExMain_F.pSensfResp = (uint8_t *)gkphExMain_SensFRes;

#endif /* NXPBUILD__CLIF_CM*/

#if defined (NXPBUILD__CLIF_ACTIVE_INITIATOR) || defined (NXPBUILD__CLIF_ACTIVE_TARGET)
    psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pGi = gphExMain_pGi;
    phUser_MemCpy(psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pGi, gkphExMain_GeneralBytesI, 22);
    phUser_MemCpy(&psDiscLoopParams->sTypeFTargetInfo.aTypeFTag[0].aIDmPMm[0], gkphExMain_NFCID3, 10);
    psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.pAtrRes = gphExMain_pAtr;
    psDiscLoopParams->sTypeFTargetInfo.sTypeF_P2P.bAtrResLength = 0;

    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bDid = 0;
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bLri = 0;
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bNadEnable = 0;
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bNad = 0,
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pGi = gphExMain_pGi;
    phUser_MemCpy(psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pGi, gkphExMain_GeneralBytesI, 22);
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bGiLength = 0x00;
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.pAtrRes= gphExMain_pAtr;
    psDiscLoopParams->sTypeATargetInfo.sTypeA_P2P.bAtrResLength = 0;
    phUser_MemCpy(psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gkphExMain_NFCID2, 10);
#endif /* defined (NXPBUILD__CLIF_ACTIVE_INITIATOR) || defined (NXPBUILD__CLIF_ACTIVE_TARGET) */

}

void phExMain_Clif_HalInit(void)
{
	phStatus_t status;

    /** Clear the RX buf */
    phUser_MemSet((void *)&gphExMain_Rxbuf[0], 0x00, PH_EXMAIN_CLIF_RXBUFSIZE);
    /** Clear the TX buf */
    phUser_MemSet((void *)&gphExMain_Txbuf[0], 0x00, PH_EXMAIN_CLIF_TXBUFSIZE);

    /** Clear the HW data params. */
    phUser_MemSet((void *)&sHal, 0x00, sizeof(phhalHw_PN7462AU_DataParams_t));

    status = phhalHw_PN7462AU_Init(&sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
            PDATAPARAMS_SKEYSTORE,
            gphExMain_Txbuf,
            sizeof(gphExMain_Txbuf),
            gphExMain_Rxbuf,
            sizeof(gphExMain_Rxbuf)
    );

    if(status != PH_ERR_SUCCESS)
   	{
   		/** It is not expected to reach here. */
   		PH_USER_ASSERT(0);
   	}

}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
static phStatus_t phExMain_Clif_TxLdoMonitorCheck(void)
{
    phStatus_t eStatus;
    /* Check if the TxLDO is configured as External */
    if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXMAIN_USETXLDO_EXTERNAL)
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
