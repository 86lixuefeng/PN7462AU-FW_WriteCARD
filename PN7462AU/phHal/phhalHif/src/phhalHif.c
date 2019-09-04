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
 * Implementation of Host Interface Hal core functionalities which is common for
 * I2C-S, SPI-S, SMBUS, HSU.
 * Project:  PN640
 *
 * $Date: 2015-11-07 16:18:11 +0530 (Sat, 07 Nov 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 16300 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_HIF
#include "phhalHif_Int.h"
#include "PN7462AU/PN7462AU_hif.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phhalNvic.h"
#include "phUser.h"
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define PHHAL_HIF_RX_BUFFER_NUM      4    /**< Number of Rx Buffers available */
#define PHHAL_HIF_HEADER_SIZE_MAX    4    /**< Max Allowed Header Size */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/**
 * Function Pointer to an API that re-enable an already allocated rx buffer since it would been disabled upon
 * reception of a prev frame
 */
typedef phStatus_t (phRomhalHif_ReleaseConfig_t)(phhalHif_BuffId_t sBuffId);
/**
 * Function Pointer to an API that de-allocate an already allocated rxbuffer
 */
typedef phStatus_t (phRomhalHif_DeInitRxBuffer_t)(phhalHif_BuffId_t sBuffId);
/**
 * Function Pointer to an API that Configures the Water Level
 */
typedef phStatus_t (phRomhalHif_ConfigureWaterLevel_t)(uint16_t wWaterLevel, pphhalHif_Callback_t pCallBackPtr);
/**
 * Function Pointer to an API that allocate and enable one of 4 rx buffers for reception from host.
 */
typedef phStatus_t (phRomhalHif_InitRxBuffer_t)(phhalHif_BuffId_t sBuffId, uint16_t wMaxSize, uint32_t *pdwBuffAddr,
    uint8_t bShortFrameLen, uint8_t bHeaderOffset, phhalHif_BuffFormat_t bBufferType);
/**
 * Function Pointer to an API that Transmits the buffer over the previously selected Host interface
 */
typedef phStatus_t (phRomhalHif_Transmit_t)(uint32_t *pdwBuffer, uint16_t wLength, uint8_t bHeaderOffset,
    phhalHif_BuffFormat_t bBufferType);
/**
 * Function Pointer to an API that Enables the HIF Hardware
 */
typedef phStatus_t (phRomhalHif_RegInit_t)(phhalHif_Config_t *pConfig);

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

static phhalHif_RxBuffContext_t gRxBuffContext[PHHAL_HIF_RX_BUFFER_NUM];
static pphhalHif_Callback_t gpRxCallBackHandler[PHHAL_HIF_RX_BUFFER_NUM];
static pphhalHif_Callback_t gpTxCallBackHandler;
static pphhalHif_Callback_t gpWaterLevelCallBackHandler;
static pphhalHif_Callback_t gpErrorCallBackHandler;
static phhalHif_BuffFormat_t gBufferType;
static phhalHif_Config_t gHifConfig;
static uint8_t gphhalHif_bHeaderOffset;
static uint8_t gphhalHif_bShortFrameLen;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#define phRomhalHif_ReleaseConfig(sBuffId)   ((phRomhalHif_ReleaseConfig_t *)PH_HAL_HIF_RELEASERXBUFFER_ADDR)(sBuffId)

#define phRomhalHif_DeInitRxBuffer(sBuffId)  ((phRomhalHif_DeInitRxBuffer_t *) PH_HAL_HIF_DEINITRXBUFFER_ADDR)(sBuffId)

#define phRomhalHif_ConfigureWaterLevel(wWaterLevel, pCallBackPtr)     \
    ((phRomhalHif_ConfigureWaterLevel_t *)PH_HAL_HIF_CONFIGUREWATERLEVEL_ADDR)(wWaterLevel, pCallBackPtr)

#define phRomhalHif_InitRxBuffer(sBuffId, wMaxSize, pdwBuffAddr, bShortFrameLen, bHeaderOffset, bBufferType)     \
    ((phRomhalHif_InitRxBuffer_t *) PH_HAL_HIF_INITRXBUFFER_ADDR)(sBuffId, wMaxSize, pdwBuffAddr, bShortFrameLen, \
        bHeaderOffset, bBufferType)

#define phRomhalHif_Transmit(pdwBuffer, wLength, bHeaderOffset, bBufferType)        \
    ((phRomhalHif_Transmit_t *) PH_HAL_HIF_TRANSMIT_ADDR)(pdwBuffer, wLength, bHeaderOffset, bBufferType)

#define phRomhalHif_RegInit(pConfig)     ((phRomhalHif_RegInit_t *) PH_HAL_HIF_REGINIT_ADDR)(pConfig)

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalHif_Init(phhalHif_Config_t *pConfig, pphhalHif_Callback_t pErrorCallBack)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;


#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    if ((pConfig == NULL) || ((pConfig->eBufferType== E_BUFFER_FORMAT_FIXED) &&
        (( (!pConfig->bHeaderSize) || (pConfig->bHeaderSize > PHHAL_HIF_HEADER_SIZE_MAX) ))))
    {
        return PH_ERR(INVALID_PARAMETER, HIF);
    }
#endif

    gBufferType = pConfig->eBufferType;
    if (gBufferType == E_BUFFER_FORMAT_FIXED)
    {
        gphhalHif_bHeaderOffset = PHHAL_HIF_HEADER_SIZE_MAX-(pConfig->bHeaderSize);
        gphhalHif_bShortFrameLen = pConfig->bShortFrameLen;
    }
    else
    {
        gphhalHif_bHeaderOffset = 0;
        gphhalHif_bShortFrameLen = 0;
    }

    eStatus = phRomhalHif_RegInit(pConfig);

    if(eStatus == PH_ERR_SUCCESS)
    {
        gpWaterLevelCallBackHandler = NULL;

        /* Store the Callback, so that it can be executed when ever the error occurs */
        gpErrorCallBackHandler =  pErrorCallBack;

        phUser_MemCpy( &gHifConfig, pConfig, sizeof(phhalHif_Config_t) );
    }

    return eStatus;
}

phStatus_t phhalHif_Transmit(uint32_t *pdwBuffer, uint16_t wLength, pphhalHif_Callback_t pCallBackPtr)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if (pCallBackPtr == NULL)
        {
            eStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }
#endif

        if ( PH_REG_TEST_BIT(HOSTIF_STATUS_REG,TX_BUFFER_LOCK) )
        {
            /* The SW status interprets the HW Status of Buffer Lock as Tx Going*/
            eStatus = PH_ERR_HIF_TX_ONGOING;
            break;
        }

        if ( PH_REG_TEST_BIT(HOSTIF_DATA_READY_STATUS_REG, TX_DATA_READY))
        {
            /* The SW status interprets the HW Status of Tx Data Ready as Tx Initiated but not yet completed */
            eStatus = PH_ERR_HIF_TX_BUFFER_LOCKED;
            break;
        }

        gpTxCallBackHandler = pCallBackPtr;

        phRomhalHif_Transmit(pdwBuffer, wLength, gphhalHif_bHeaderOffset, gBufferType);

    }while(0);

    return  PH_ADD_COMPCODE(eStatus, PH_COMP_HIF);
}

phStatus_t phhalHif_InitRxBuffer(phhalHif_BuffId_t sBuffId, uint16_t wMaxSize,
    uint32_t *pdwBuffAddr, pphhalHif_Callback_t pCallBackPtr)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if (pCallBackPtr == NULL)
        {
            eStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }
#endif

        /* Check whether the buffer is already enabled -
         * if already enabled, it has to be disabled before configuring it back
         */
        if ( !PH_REG_TEST_BITN_SC(HOSTIF_BUFFER_RX0_CFG_REG + ((uint32_t)sBuffId*4),
            HOSTIF_BUFFER_RX0_CFG_REG_RX0_BUFFER_DISABLE_POS) )
        {
            eStatus = PH_ERR_HIF_RX_BUFFER_ALREADY_ENABLED;
            break;
        }


        gpRxCallBackHandler[sBuffId] = pCallBackPtr;
        gRxBuffContext[sBuffId].dwMaxSize = wMaxSize;
        gRxBuffContext[sBuffId].pdwBuffAddr = pdwBuffAddr;
        gRxBuffContext[sBuffId].IsDataAvailable = false;
        gRxBuffContext[sBuffId].dwDataLength = 0;

        eStatus = phRomhalHif_InitRxBuffer(sBuffId, wMaxSize, pdwBuffAddr,  gphhalHif_bShortFrameLen,
            gphhalHif_bHeaderOffset, gBufferType);

    }while(0);

    return  PH_ADD_COMPCODE(eStatus, PH_COMP_HIF);
}


phStatus_t phhalHif_ConfigureWaterLevel(uint16_t wWaterLevel, pphhalHif_Callback_t pCallBackPtr)
{

    phRomhalHif_ConfigureWaterLevel(wWaterLevel, pCallBackPtr);
    /* Save the callback reference, so that it can be called whenever Waterlevel is reached */
    gpWaterLevelCallBackHandler = pCallBackPtr;

    return  PH_ERR_SUCCESS;
}


phStatus_t phhalHif_DeInitRxBuffer(phhalHif_BuffId_t sBuffId)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    eStatus = phRomhalHif_DeInitRxBuffer(sBuffId);
    gpRxCallBackHandler[sBuffId] = NULL;
    phUser_MemSet((void *)&gRxBuffContext[sBuffId], 0, (uint32_t)sizeof(phhalHif_RxBuffContext_t));

    return  eStatus;
}

phStatus_t phhalHif_ReleaseRxBuffer(phhalHif_BuffId_t sBuffId)
{
    phStatus_t eStatus ;


    eStatus = phRomhalHif_ReleaseConfig(sBuffId);

    if(eStatus == PH_ERR_SUCCESS)
    {
        gRxBuffContext[sBuffId].IsDataAvailable = false;
        gRxBuffContext[sBuffId].dwDataLength = 0;
    }

    return  eStatus;
}


phStatus_t phhalHif_Reset(void)
{
    phStatus_t eStatus;

    eStatus = phhalHif_DeInit();

    if (eStatus == PH_ERR_SUCCESS)
    {
        eStatus = phhalHif_Init(&gHifConfig,gpErrorCallBackHandler);
    }

    return eStatus;
}

/* NXPBUILD__PHHAL_HIF and NXPBUILD__PHHAL_USB are mutually exclusive.
 * This ISR is only for HIF=I2C,SPI or HSU. For USB, this API would
 * never be used/exercised */

PH_INTERRUPT(HIF_IRQHandler)
{
    uint32_t dwIntStatus;
    uint16_t wLength = 0;
    uint32_t bCount;
    dwIntStatus = ( PH_REG_GET(HOSTIF_INT_STATUS_REG) & PH_REG_GET(HOSTIF_INT_ENABLE_REG) );

    /* Call the Generic Irq handler if the ISR is for other reason than EOT or EOR */
    if (dwIntStatus & ~(HOSTIF_INT_STATUS_REG_RX0_EOR_STATUS_MASK
        | HOSTIF_INT_STATUS_REG_RX1_EOR_STATUS_MASK
        | HOSTIF_INT_STATUS_REG_RX2_EOR_STATUS_MASK
        | HOSTIF_INT_STATUS_REG_RX3_EOR_STATUS_MASK
        | HOSTIF_INT_STATUS_REG_EOT_STATUS_MASK
        | HOSTIF_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_MASK) )
    {
        gpErrorCallBackHandler(dwIntStatus, NULL);
    }

    /* if there is EOR on any of the RX buffer */
    for (bCount = 0; bCount < PHHAL_HIF_RX_BUFFER_NUM; bCount++)
    {
        if (dwIntStatus & ((uint32_t)(1<<bCount)))
        {

            if (gBufferType == E_BUFFER_FORMAT_FREE)
            {
                wLength = (uint16_t)PH_REG_GET_FIELD_NS_SC( (HOSTIF_BUFFER_RX0_LEN_REG + ((uint32_t)bCount*4)),
                    HOSTIF_BUFFER_RX0_LEN_REG_RX0_LENGTH_MASK);
            }
            gpRxCallBackHandler[bCount]( (uint32_t)wLength, gRxBuffContext);
        }
    }

    /* if the transmission is completed */
    if (dwIntStatus & HOSTIF_INT_STATUS_REG_EOT_STATUS_MASK)
    {
        if (gBufferType == E_BUFFER_FORMAT_FREE)
        {
            wLength = (uint16_t)PH_REG_GET(HOSTIF_BUFFER_TX_LEN_REG);
        }
        gpTxCallBackHandler( (uint32_t) wLength, NULL);

        /* TX Data ready will be cleared automatically by the hardware once the Tx completes */

        /* Disable the TX interrupt as it will be enabled again when TX starts */
        PH_REG_SET_BIT_WO(HOSTIF_INT_CLR_ENABLE_REG,EOT_CLR_ENABLE);
    }

    if (dwIntStatus & HOSTIF_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_MASK)
    {
        wLength = (uint16_t)PH_REG_GET(HOSTIF_WATERLEVEL_REG);
        gpWaterLevelCallBackHandler((uint32_t)wLength,NULL);
    }

    /* Clear the status of the interrupts which are set */
    PH_REG_SET(HOSTIF_INT_CLR_STATUS_REG,dwIntStatus);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_HIF */
