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
 * SPI Master HAL implementation for SPI protocol transactions and usage.
 *
 * Project:  PN640
 *
 * $Date: 2015-09-04 17:19:56 +0530 (Fri, 04 Sep 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15087 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SPIM
#include "phhalSPIM.h"
#include "phhalSPIM_Int.h"
#include "phhalSPIM_Event.h"
#include "phhalNvic.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_spim.h"
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_SPIM_UPDATE_START_RX()                                                                   \
    do                                                                                                 \
    {                                                                                                  \
        wStartAddress = (uint16_t)(pbBuffer - (uint8_t *)(PHHAL_SPIM_MIN_RAM_ADDRESS));                \
        PH_REG_SET_FIELD_NS(SPIM_RX_BUFFER_REG, RX_START_ADDR, wStartAddress);                         \
        PH_REG_SET_FIELD(SPIM_RX_BUFFER_REG, RX_LENGTH, wLength);                                      \
        PH_REG_SET_BIT_WO(SPIM_CONTROL_REG, RX_START);                                                 \
    }while (0)

#define PHHAL_SPIM_UPDATE_START_TX()                                                                   \
    do                                                                                                 \
    {                                                                                                  \
        wStartAddress = (uint16_t)(pbBuffer - (uint8_t *)(PHHAL_SPIM_MIN_RAM_ADDRESS));                \
        PH_REG_SET_FIELD_NS(SPIM_TX_BUFFER_REG, TX_START_ADDR, wStartAddress);                         \
        PH_REG_SET_FIELD(SPIM_TX_BUFFER_REG, TX_LENGTH, wLength);                                      \
        PH_REG_SET_BIT_WO(SPIM_CONTROL_REG, TX_START);                                                 \
    }while (0)

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
typedef phStatus_t (phRomhalSPIM_ErrorCheck_t)(uint16_t wLength,  uint8_t *pbBuffer);
typedef phStatus_t (phRomhalSPIM_ReceiveConfig_t)( uint8_t bInitCRC, uint8_t bCRCInit, uint16_t wLength,
    uint8_t *pbBuffer, uint16_t wCRCPayLoadOffset, uint32_t dwConfigReg);
typedef phStatus_t (phRomhalSPIM_TransmitConfig_t)(uint8_t bInitCRC, uint8_t bTxAppendCRC, uint8_t bCRCInit,
    uint16_t wLength, uint8_t *pbBuffer, uint16_t wCRCPayLoadOffset, uint32_t dwConfigReg);
typedef phStatus_t (phRomhalSPIM_RegInit_t)(void);

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static uint8_t bActiveSlave[PHHAL_SPIM_MAX_NO_SLAVES];
static phhalSPIM_Configure_t sSlaveConfiguration[PHHAL_SPIM_MAX_NO_SLAVES];
static pphhalSPIM_CallbackFun_t pWaterLevelCallBackFunc;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#define phhalSPIM_ErrorCheck(wLength, pbBuffer)          \
    ((phRomhalSPIM_ErrorCheck_t *)PH_HAL_SPIM_ERRORCHECK_ADDR)(wLength, pbBuffer)

#define phRomhalSPIM_ReceiveConfig(bInitCRC, bCRCInit, wLength, pbBuffer, wCRCPayLoadOffset, dwConfigReg)    \
    ((phRomhalSPIM_ReceiveConfig_t *) PH_HAL_SPIM_RECEIVE_ADDR)(bInitCRC, bCRCInit, wLength, pbBuffer,        \
        wCRCPayLoadOffset, dwConfigReg)

#define phRomhalSPIM_TransmitConfig(bInitCRC,bTxAppendCRC, bCRCInit, wLength, pbBuffer, wCRCPayLoadOffset,dwConfigReg)\
    ((phRomhalSPIM_TransmitConfig_t *) PH_HAL_SPIM_TRANSMIT_ADDR)(bInitCRC, bTxAppendCRC, bCRCInit, wLength, pbBuffer, \
        wCRCPayLoadOffset,dwConfigReg)

#define phRomhalSPIM_RegInit()          ((phRomhalSPIM_RegInit_t *) PH_HAL_SPIM_REGINIT_ADDR)()

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalSPIM_Init(uint32_t dwSpimTransactionTimeOut PH_UNUSED)
{
    phStatus_t wStatus;
    uint8_t bLoopCount;

    /*Register for a Queue event for transaction complete. */
    wStatus = phhalSPIM_EventRegister(dwSpimTransactionTimeOut);
    /*check for errors*/
    PH_RETURN_ON_FAILURE(wStatus);

    for (bLoopCount=0; bLoopCount < PHHAL_SPIM_MAX_NO_SLAVES; bLoopCount++)
    {
        bActiveSlave[bLoopCount]=false;
        sSlaveConfiguration[bLoopCount].dwConfigReg = 0;
    }

    pWaterLevelCallBackFunc = NULL;

    /* Enable SPIM AHB clock. */
    PH_REG_SET_BIT(PCR_CLK_CFG_REG, CLOCK_SPIM_ENABLE);
    wStatus = phRomhalSPIM_RegInit();

    return wStatus;
}



phStatus_t phhalSPIM_DeInit(void)
{
    /* Disable SPIM clock. */
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG, IPCLOCK_SPIM_ENABLE);
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG, CLOCK_SPIM_ENABLE);

    /* Disable spi interrupt  in global interrupt register. */
    PH_REG_CLEAR_BITN(NVIC_ISER_REG, PHHAL_NVIC_SPI);

    phhalSPIM_EventUnRegister();
    return PH_ERR_SUCCESS;
}

phStatus_t phhalSPIM_SetWaterLevel(uint16_t wWaterLevel, pphhalSPIM_CallbackFun_t pCallBackFunc)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    if (    pCallBackFunc == NULL){
        return PH_ERR(INVALID_PARAMETER,SPIM);
    }
#endif
    pWaterLevelCallBackFunc = pCallBackFunc;
    PH_REG_SET_FIELD_NS(SPIM_WATERLEVEL_REG, WATERLEVEL, wWaterLevel);

    return PH_ERR_SUCCESS;
}

phStatus_t phhalSPIM_Configure( uint8_t bSlaveSelect, uint8_t bMsbFirst, phhalSPIM_Modes_t eMode, uint8_t bBaudRate,
    uint8_t bNssPulse, uint8_t bNssPol )
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    do
    {
        /* Check if the bus is free */
        if (PH_REG_TEST_BIT(SPIM_STATUS_REG, TX_ONGOING) || PH_REG_TEST_BIT(SPIM_STATUS_REG, RX_ONGOING))
        {
            wStatus = PH_ERR_TXRX_ONGOING;
            break;
        }
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the maximum allowed baud rate value */
        if ((bBaudRate > PHHAL_SPIM_MAX_BAUDRATE_VALUE) || (bSlaveSelect >= PHHAL_SPIM_MAX_NO_SLAVES))
        {
            wStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }
#endif

        sSlaveConfiguration[bSlaveSelect].dwConfigReg = ((uint32_t)bBaudRate << SPIM_CONFIG_REG_BAUDRATE_POS)
                                        | ((uint32_t)bSlaveSelect << SPIM_CONFIG_REG_SLAVE_SELECT_POS)
                                        | ((uint32_t)bMsbFirst << SPIM_CONFIG_REG_MSB_FIRST_POS)
                                        | (((uint32_t)eMode & 0x01) << SPIM_CONFIG_REG_CPHA_POS)
                                        | (((uint32_t)eMode >> 0x01) << SPIM_CONFIG_REG_CPOL_POS)
                                        | (bNssPulse << SPIM_CONFIG_REG_NSS_PULSE_POS)
                                        | (bNssPol << SPIM_CONFIG_REG_NSS_POLARITY_POS);

        bActiveSlave[bSlaveSelect] = true;

        /* If slave 1 is selected config GPIO1 as SPIM.NSS1. */
        if(bSlaveSelect)
        {
            PH_REG_SET_BIT(PCR_SPIM_REG, SPIM_NSS1_EN);
        }

    }while (0);

    return PH_ADD_COMPCODE(wStatus,PH_COMP_SPIM);
}


phStatus_t phhalSPIM_Receive( uint8_t bSlaveSelect, uint8_t bInitCRC, uint8_t bCRCInit, uint16_t wLength,
    uint8_t *pbBuffer, uint16_t wCRCPayLoadOffset )
{
    phStatus_t wStatus;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the maximum allowed baud rate value */
        if (bSlaveSelect >= PHHAL_SPIM_MAX_NO_SLAVES)
        {
            wStatus = (PH_ERR_INVALID_PARAMETER| PH_COMP_SPIM);
            break;
        }
#endif
        /* Check slave 0 OR slave 1 is configured */
        if (bActiveSlave[bSlaveSelect] == false)
        {
            wStatus = (PH_ERR_NOT_CONFIGURED | PH_COMP_SPIM);
            break;
        }

        wStatus = phRomhalSPIM_ReceiveConfig(bInitCRC, bCRCInit, wLength, pbBuffer, wCRCPayLoadOffset,
            sSlaveConfiguration[bSlaveSelect].dwConfigReg);
        PH_BREAK_ON_FAILURE(wStatus);

        /*Wait for Transactions to complete/Error, Blocking-call. */
        wStatus= phhalSPIM_EventWait();

    }while (0);

    return wStatus;
}

phStatus_t phhalSPIM_ReceiveContinue(uint16_t wLength, uint8_t *pbBuffer)
{
    phStatus_t wStatus;
    uint16_t wStartAddress = 0x00;

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    uint8_t bSlaveSelect = (uint8_t)PH_REG_TEST_BITN(SPIM_CONFIG_REG,SPIM_CONFIG_REG_SLAVE_SELECT_POS);
#endif


    do
    {

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if (0 == wLength){
            wStatus = (PH_ERR_INVALID_PARAMETER | PH_COMP_SPIM);
            break;
        }
        /* Check slave 0 OR slave 1 is configured */
        if (bActiveSlave[bSlaveSelect] == false){
            wStatus = (PH_ERR_NOT_CONFIGURED | PH_COMP_SPIM);
            break;
        }
#endif

        wStatus =phhalSPIM_ErrorCheck(wLength, pbBuffer);
        PH_BREAK_ON_FAILURE(wStatus);

        PHHAL_SPIM_UPDATE_START_RX();

        /* Event handler. */
        /*Wait for Transactions to complete/Error, Blocking-call. */
        wStatus = phhalSPIM_EventWait();
    }while (0);

    return wStatus;
}

phStatus_t phhalSPIM_Transmit( uint8_t bSlaveSelect, uint8_t bInitCRC, uint8_t bTxAppendCRC, uint8_t bCRCInit,
    uint16_t wLength, uint8_t *pbBuffer, uint16_t wCRCPayLoadOffset )
{
    phStatus_t wStatus;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the maximum allowed baud rate value */
        if (bSlaveSelect >= PHHAL_SPIM_MAX_NO_SLAVES)
        {
            wStatus = (PH_ERR_INVALID_PARAMETER| PH_COMP_SPIM);
            break;
        }
#endif
        /* Check slave 0 OR slave 1 is configured */
        if (bActiveSlave[bSlaveSelect] == false){
            wStatus = (PH_ERR_NOT_CONFIGURED | PH_COMP_SPIM);
            break;
        }

        wStatus = phRomhalSPIM_TransmitConfig(bInitCRC, bTxAppendCRC, bCRCInit, wLength, pbBuffer,
            wCRCPayLoadOffset, sSlaveConfiguration[bSlaveSelect].dwConfigReg);
        PH_BREAK_ON_FAILURE(wStatus);

        /*Wait for Transactions to complete/Error, Blocking-call. */
        wStatus = phhalSPIM_EventWait();
    }while(0);

    return wStatus;
}

phStatus_t phhalSPIM_TransmitContinue( uint16_t wLength, uint8_t *pbBuffer)
{
    phStatus_t wStatus;
    uint16_t wStartAddress = 0x00;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    uint8_t bSlaveSelect = (uint8_t)PH_REG_TEST_BITN(SPIM_CONFIG_REG,SPIM_CONFIG_REG_SLAVE_SELECT_POS);
#endif

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check the length is proper */
        if (0 == wLength){
            wStatus = (PH_ERR_INVALID_PARAMETER | PH_COMP_SPIM);
            break;
        }
        /* Check slave 0 OR slave 1 is configured */
        if (bActiveSlave[bSlaveSelect] == false){
            wStatus = (PH_ERR_NOT_CONFIGURED | PH_COMP_SPIM);
            break;
        }
#endif
        wStatus = phhalSPIM_ErrorCheck(wLength, pbBuffer);
        PH_BREAK_ON_FAILURE(wStatus);

        PHHAL_SPIM_UPDATE_START_TX();

        /* Polling mode */
        /*Wait for Transactions to complete/Error, Blocking-call. */
        wStatus = phhalSPIM_EventWait();
    }while (0);

    return wStatus;
}

/*
 *Function Name: phhalSPIM_Isr
 *Description: Interrupt service routine for the SPI master.
 *
 *Input Parameters: None
 *Output Parameters: None
 *
 *Note: This function is for SPIM HAL internal use only. It should not be called directly by the user API's.
 */
void SPIM_IRQHandler(void)
{
    uint32_t dwIntStsReg = (uint32_t)(PH_REG_GET(SPIM_INT_STATUS_REG) & PH_REG_GET(SPIM_INT_ENABLE_REG));

    if (dwIntStsReg & SPIM_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_MASK){
        /* Asynchronous event, call user call back if registered. */
        if (pWaterLevelCallBackFunc != NULL){
            pWaterLevelCallBackFunc();
        }
    }

    /* expected that Water level with EOT/EOR may come up at once. */
    if (dwIntStsReg & (SPIM_INT_STATUS_REG_AHB_ADDR_ERROR_STATUS_MASK
        | SPIM_INT_STATUS_REG_AHB_ERROR_STATUS_MASK
        | SPIM_INT_STATUS_REG_EOT_STATUS_MASK
        | SPIM_INT_STATUS_REG_EOR_STATUS_MASK))
    {

        /* Raise the event for higher layer and exit.
         * No need to send the water level status, since already call back is raised. */
        phhalSPIM_EventHandler( (phStatus_t)(dwIntStsReg & (~SPIM_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_MASK)));
    }

    /* Clear the interrupts. */
    PH_REG_SET(SPIM_INT_CLR_STATUS_REG, dwIntStsReg);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_SPIM */
