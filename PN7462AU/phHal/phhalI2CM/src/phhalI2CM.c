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
 * I2C Master HAL implementation for I2C protocol transactions and usage.
 *
 * Project:  PN640
 *
 * $Date: 2015-08-25 10:41:42 +0530 (Tue, 25 Aug 2015) $
 * $Author: Sugasi Rajesh (nxp69678) $
 * $Revision: 14700 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM
#include "phhalI2CM.h"
#include "phhalI2CM_Int.h"
#include "phhalI2CM_FillCopyFifo.h"
#include "phhalNvic.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_i2cm.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phhalI2CM_Event.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define phRomhalI2CM_RegInit(bTxThres, bRxThres)        \
    ((phRomhalI2CM_RegInit_t *) PH_HAL_I2CM_REGINIT_ADDR)(bTxThres, bRxThres)

#define phRomhalI2CM_RegDeInit()    ((phRomhalI2CM_RegDeInit_t *)PH_HAL_I2CM_REGDEINIT_ADDR)()

#define phhalI2CM_Settings(psTransaction)               \
    ((phRomhalI2CM_TransactConfig_t *) PH_HAL_I2CM_TRANSACTCONFIG_ADDR)(psTransaction)
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
typedef phStatus_t (phRomhalI2CM_RegInit_t)(uint8_t bTxThres, uint8_t bRxThres);

typedef phStatus_t (phRomhalI2CM_RegDeInit_t)(void);

typedef void (phRomhalI2CM_TransactConfig_t)(phhalI2CM_Transaction_t const *psTransaction);

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/* 1 -> indicates I2CM started, 0 -> indicates I2CM idle. */
static uint8_t gfI2CMTransState;

/* Count of Queued transactions. */
static uint8_t gbQueueCount;
static uint8_t gbTransCompletedCnt;
static uint8_t gbI2CMRetryCnt;

/* Total number of bytes ready for the transaction */
static uint16_t gwTotalQuedBytes;

/* Count of bytes which has been pushed into the Fifo in each queue */
static uint16_t gaNoOfBytesQuedInFifo[PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS];

/* Total number of bytes received in a transaction*/
static uint16_t gwRcvdBytes;

/* Array of transaction pointers to hold the queue transactions */
static phhalI2CM_Transaction_t gpsTransactionQueue[PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS];

/* I2CM Slave check of operation */
static uint8_t gfSlaveCheck;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
/**
 * Does I2CM sw cleabup.
 */
static void phhalI2CM_Cleanup( uint8_t bNumOfTrans );
/**
 * Fills the I2cm Fifo buffers.
 */
static void phhalI2CM_FillFifo( phhalI2CM_Transaction_t *psTransQ, uint16_t *wBytesQdInFifo );
/**
 * Generic Queue api to queue the i2cm transactions.
 */
static phStatus_t phhalI2CM_Queue( uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData );
/**
 * Fifo handler during I2cm receive operations. Used in handler mode only.
 */
static void phhalI2CM_RxThres_FIFOFull_Handler( phhalI2CM_Transaction_t *psTransQ, uint16_t *wRxBytes );
/**
 * Fifo handler during I2cm transmit operations. Used in handler mode only.
 */
static void phhalI2CM_TxThres_FIFOEmpty_Handler( phhalI2CM_Transaction_t *psTransQ, uint16_t *wBytesQdInFifo );

#ifndef NXPBUILD__PHHAL_I2CM_MULTI_TRANSACTION
    static phStatus_t phhalI2CM_Start(void);
#endif

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalI2CM_Init( uint8_t bTxThres, uint8_t bRxThres, uint32_t dwTicksToWait PH_UNUSED, uint8_t bRetryCount )
{
    phStatus_t wStatus;

    /* Register an event mechanism for I2CM transactions. */
    wStatus = phhalI2CM_EventRegister(dwTicksToWait);
    PH_RETURN_ON_FAILURE(wStatus);

    gbI2CMRetryCnt = bRetryCount;

    phRomhalI2CM_RegInit(bTxThres, bRxThres);

    phhalI2CM_Cleanup(PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS);

    return PH_ERR_SUCCESS;

}

phStatus_t phhalI2CM_DeInit( void )
{
    /* Disable I2C interrupt  in global interrupt register to avoid residual interrupts when reset*/
    PH_REG_CLEAR_BITN(NVIC_ISER_REG, PHHAL_NVIC_I2C);

    /* Safer side cleanup - Anyway done in Init */
    phhalI2CM_Cleanup(PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS);

    (void)phRomhalI2CM_RegDeInit();

    phhalI2CM_EventUnRegister();

    return PH_ERR_SUCCESS;
}

phStatus_t phhalI2CM_SlaveCheck( uint16_t wSlaveAddr )
{
    phStatus_t status;

    gfSlaveCheck = true;

    /* Queue the message and start I2CM. */
    status = phhalI2CM_Queue(wSlaveAddr, 0, NULL);
    PH_RETURN_ON_FAILURE(status);

    return phhalI2CM_Start();
}


#ifdef NXPBUILD__PHHAL_I2CM_MULTI_TRANSACTION
/**
 * Queue the message to be transmitted.
 */
phStatus_t phhalI2CM_QueueTx( uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData )
{
    PHHAL_I2CM_SET_MODE(E_I2CM_TX_MODE);
    return phhalI2CM_Queue(wSlaveAddr, wDataLength, pdwData);
}

/**
 * Queue the transaction to be received.
 */
phStatus_t phhalI2CM_QueueRx( uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData )
{
    PHHAL_I2CM_SET_MODE(E_I2CM_RX_MODE);
    return phhalI2CM_Queue(wSlaveAddr, wDataLength, pdwData);
}
#endif

#ifdef NXPBUILD__PHHAL_I2CM_MULTI_TRANSACTION
    phStatus_t phhalI2CM_Start(void)
#else
    static phStatus_t phhalI2CM_Start(void)
#endif
{
    uint32_t wTransactionCount;
    phStatus_t wStatus;

    /* Check whether I2C transaction started */
    if (gfI2CMTransState)
    {
        return PH_ERR(TXRX_STARTED_ERROR,I2CM);
    }

    /* Configure and start I2C master operations */
    gfI2CMTransState = true;
    gbTransCompletedCnt = 0;

    /* Perform the I2CM settings */
    phhalI2CM_Settings(&gpsTransactionQueue[gbTransCompletedCnt]);

    if (PHHAL_I2CM_GET_MODE())
    {/* for receiving */
        gwRcvdBytes = 0;
    }
    else /* for transmitting */
    {
        /* fill fifo */
        for (wTransactionCount = gbTransCompletedCnt; wTransactionCount < gbQueueCount; wTransactionCount++)
        {
            gaNoOfBytesQuedInFifo[wTransactionCount] = 0;

            /* check for words complete or fifo full */
            phhalI2CM_FillFifo(&gpsTransactionQueue[wTransactionCount], &gaNoOfBytesQuedInFifo[wTransactionCount]);
        }
    }

    /* Enable I2C master transaction */
    PH_REG_SET_BIT_WO(I2CMASTER_CONTROL_REG, I2C_ENABLE);


    /*Wait for Transactions to complete/Error, Blocking-call. */
    wStatus = (phStatus_t)(phhalI2CM_EventWait() & ((uint16_t)0xFFFE));
    return (uint16_t)(( wStatus == 0 )? PH_ERR_SUCCESS: wStatus | 0x80U | PH_COMP_I2CM);

}

phStatus_t phhalI2CM_Transmit( uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData)
{
    phStatus_t status=PH_ERR(INVALID_PARAMETER,I2CM);

    PHHAL_I2CM_SET_MODE(E_I2CM_TX_MODE);

    /* Queue the message and start I2CM. */
    gbQueueCount = 0;
    status = phhalI2CM_Queue(wSlaveAddr, wDataLength, pdwData);
    PH_RETURN_ON_FAILURE(status);

    return phhalI2CM_Start();
}

phStatus_t phhalI2CM_Receive( uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData)
{
    phStatus_t status=PH_ERR(INVALID_PARAMETER,I2CM);

    PHHAL_I2CM_SET_MODE(E_I2CM_RX_MODE);

    /* Queue the message and start I2CM. */
    gbQueueCount = 0;
    status = phhalI2CM_Queue(wSlaveAddr, wDataLength, pdwData);
    PH_RETURN_ON_FAILURE(status);

    return phhalI2CM_Start();
}

phStatus_t phhalI2CM_GenCallReset( void )
{
    /* Reset command byte. */
    uint32_t dwData[1] = { 0x06 };

    /* Perform General call. */
    return phhalI2CM_Transmit((uint16_t) 0x00, (uint16_t) 0x01, dwData);
}

void I2CM_IRQHandler( void )
{
    uint32_t dwIntStsReg = (uint32_t)(
        PH_REG_GET(I2CMASTER_INT_STATUS_REG) & PH_REG_GET(I2CMASTER_INT_ENABLE_REG));

    /* Indicates that the FIFO threshold is reached while I2C transmission is on-going */
    /* Indicates that the FIFO empty condition is reached while I2C transmission is on-going */
    if (dwIntStsReg & (I2CMASTER_INT_STATUS_REG_TX_FIFO_THRES_MASK | I2CMASTER_INT_STATUS_REG_FIFO_EMPTY_MASK))
    {
        phhalI2CM_TxThres_FIFOEmpty_Handler(&gpsTransactionQueue[gbTransCompletedCnt],
            &gaNoOfBytesQuedInFifo[gbTransCompletedCnt]);

        /* Clear FIFO threshold status , Clear FIFO empty status */
        PH_REG_SET(I2CMASTER_INT_CLR_STATUS_REG,
            I2CMASTER_INT_STATUS_REG_TX_FIFO_THRES_MASK | I2CMASTER_INT_STATUS_REG_FIFO_EMPTY_MASK);
    }

    /* Indicates that the FIFO threshold is reached while I2C reception is on-going */
    /* Indicates that the FIFO full condition is reached while I2C reception is on-going */
    if (dwIntStsReg & (I2CMASTER_INT_STATUS_REG_RX_FIFO_THRES_MASK | I2CMASTER_INT_STATUS_REG_FIFO_FULL_MASK))
    {
        phhalI2CM_RxThres_FIFOFull_Handler(&gpsTransactionQueue[gbTransCompletedCnt], &gwRcvdBytes);

        /* Clear FIFO threshold status */
        PH_REG_SET(I2CMASTER_INT_CLR_STATUS_REG,
            I2CMASTER_INT_STATUS_REG_RX_FIFO_THRES_MASK | I2CMASTER_INT_STATUS_REG_FIFO_FULL_MASK);
    }

    /*check for I2C NAK/Arbitration/Bus Errors*/
    if (dwIntStsReg
        & (I2CMASTER_INT_STATUS_REG_ARB_FAILURE_MASK | I2CMASTER_INT_STATUS_REG_NACK_MASK
            | I2CMASTER_INT_STATUS_REG_I2C_BUS_ERROR_MASK))
    {
        /* Clear the error interrupts & clear transaction complete status. */
        PH_REG_SET(I2CMASTER_INT_CLR_STATUS_REG, dwIntStsReg | I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_MASK);

        /* Stop transaction and return error if retry count is 0. */
        if (gpsTransactionQueue[gbTransCompletedCnt].bRetryCount)
        {
            gpsTransactionQueue[gbTransCompletedCnt].bRetryCount--;

            /*Continue transaction upon error if retry count is set. */
            PHHAL_I2CM_CONT_TRANS();
        }
        else
        {
            /* for reception operations only. */
            if (PHHAL_I2CM_GET_MODE())
            {
                /* Copy the words from FIFO */
                phhalI2CM_RxThres_FIFOFull_Handler(&gpsTransactionQueue[gbTransCompletedCnt], &gwRcvdBytes);
                /* Clear rxd databytes count for next transaction */
                gwRcvdBytes = 0;
            }
            /* check for I2c slave address Nack. */
            if ((PH_REG_GET(I2CMASTER_BYTECOUNT_STATUS_REG) == 0x00)
                && (dwIntStsReg & I2CMASTER_INT_STATUS_REG_NACK_MASK))
            {
                /* clear the Gen/Data Nak and update address Nak.*/
                dwIntStsReg |= PH_ERR_ADDR_NAK_ERROR;
                dwIntStsReg &= (~I2CMASTER_INT_STATUS_REG_NACK_MASK);
            }

            /*Report I2CM All Transactions Completed Event. */
            phhalI2CM_EventHandler(gbTransCompletedCnt,
                dwIntStsReg & (I2CMASTER_INT_STATUS_REG_ARB_FAILURE_MASK | I2CMASTER_INT_STATUS_REG_NACK_MASK
                        | I2CMASTER_INT_STATUS_REG_I2C_BUS_ERROR_MASK | PH_ERR_ADDR_NAK_ERROR));
            phhalI2CM_Cleanup(gbQueueCount);
        }
        return ;
    }

     /* Indicates an I2C master completed the I2C transmission or I2C reception*/
    if (dwIntStsReg & I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_MASK)
    {
        if (PHHAL_I2CM_GET_MODE()) /* for reception operations only. */
        {
            /* Copy the words from FIFO */
            phhalI2CM_RxThres_FIFOFull_Handler(&gpsTransactionQueue[gbTransCompletedCnt], &gwRcvdBytes);
            /* Clear rxd databytes count for next transaction */
            gwRcvdBytes = 0;
        }

        gbTransCompletedCnt++;

        /* Check for pending transactions */
        if (gbQueueCount - gbTransCompletedCnt)
        {
            /* Load next transaction's slave address, byte count. */
            phhalI2CM_Settings(&gpsTransactionQueue[gbTransCompletedCnt]);

            /* Enable I2C master for next transaction */
            PH_REG_SET_BIT_WO(I2CMASTER_CONTROL_REG, I2C_ENABLE);
        }
        else /*end of all queued transactions*/
        {
            /*Report I2CM All Transactions Completed Event. */
            phhalI2CM_EventHandler(gbTransCompletedCnt - 1, dwIntStsReg & I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_MASK);
            phhalI2CM_Cleanup(gbQueueCount);
        }
        /* clear transaction complete status*/
        PH_REG_SET(I2CMASTER_INT_CLR_STATUS_REG, dwIntStsReg);
    }
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static void phhalI2CM_FillFifo( phhalI2CM_Transaction_t *psTransQ, uint16_t *wBytesQdInFifo )
{
    uint8_t bEmptyFifoLevel, bFillBytes;

    /* Check for FIFO full */
    if (PH_REG_TEST_BITN(I2CMASTER_STATUS_REG, I2CMASTER_STATUS_REG_FIFO_FULL_STATUS_POS))
    {
        return ;
    }

    /* check for fifo level & get Fifo Empty number of words. */
    bEmptyFifoLevel = (uint8_t)(PHHAL_I2CM_NUMBER_OF_WORDS_IN_FIFO
        - PH_REG_GET_FIELD(I2CMASTER_STATUS_REG, FIFO_LEVEL));

    if (bEmptyFifoLevel > psTransQ->bWordCount)
    {
        bEmptyFifoLevel = (uint8_t) psTransQ->bWordCount;
    }

    bEmptyFifoLevel *= PHHAL_I2CM_WORD_SIZE;

    /*check for pending bytes. */
    while (bEmptyFifoLevel != 0)
    { /* check for Number of words*/
        if (!(bEmptyFifoLevel % PHHAL_I2CM_QUAD_WORD_SIZE))
        {
            bFillBytes = PHHAL_I2CM_QUAD_WORD_SIZE;
        }
        else if (!(bEmptyFifoLevel % PHHAL_I2CM_DOUBLE_WORD_SIZE))
        {
            bFillBytes = PHHAL_I2CM_DOUBLE_WORD_SIZE;
        }
        else
        {
            bFillBytes = PHHAL_I2CM_WORD_SIZE;
        }
        /*Fill fifo using LDm & STM instructions.*/
        phhalI2CM_AsmFill_OR_CopyFifo((uint32_t *) &psTransQ->pdwData[*wBytesQdInFifo / PHHAL_I2CM_WORD_SIZE],
            (uint32_t *) I2CMASTER_TXDATA1_REG, bFillBytes);
        bEmptyFifoLevel -= bFillBytes;
        *wBytesQdInFifo += bFillBytes;
    }
}

/**
 *Function Name     : phhalI2CM_RxThres_FIFOFull_Handler
 *Description       : Handles I2C master operations when FIFO Threshold/Full reached during reception operations.
 *Note:             : This function is for I2CM HAL internal use only.
 *                     It should not be called directly by the user API's.
 */
static void phhalI2CM_RxThres_FIFOFull_Handler( phhalI2CM_Transaction_t *psTransQ, uint16_t *wRxBytes )
{
    uint8_t bFifoLevel, bFillBytes;

    /* Check for FIFO empty */
    if (PH_REG_TEST_BITN(I2CMASTER_STATUS_REG, I2CMASTER_STATUS_REG_FIFO_EMPTY_STATUS_POS))
    {
        return ;
    }
    /* Get the fifo level in word size. */
    bFifoLevel = (uint8_t)(PH_REG_GET_FIELD(I2CMASTER_STATUS_REG, FIFO_LEVEL));

    if (bFifoLevel > psTransQ->bWordCount)
    {
        bFifoLevel = (uint8_t) psTransQ->bWordCount;
    }

    bFifoLevel *= PHHAL_I2CM_WORD_SIZE;

    /*check for pending bytes. */
    while (bFifoLevel != 0)
    { /* check for Number of words*/
        if (!(bFifoLevel % PHHAL_I2CM_QUAD_WORD_SIZE))
        {
            bFillBytes = PHHAL_I2CM_QUAD_WORD_SIZE;
        }
        else if (!(bFifoLevel % PHHAL_I2CM_DOUBLE_WORD_SIZE))
        {
            bFillBytes = PHHAL_I2CM_DOUBLE_WORD_SIZE;
        }
        else
        {
            bFillBytes = PHHAL_I2CM_WORD_SIZE;
        }
        /*Copy fifo using LDm & STM instructions.*/
        phhalI2CM_AsmFill_OR_CopyFifo((uint32_t *) I2CMASTER_RXDATA1_REG,
            (uint32_t *) &psTransQ->pdwData[*wRxBytes / PHHAL_I2CM_WORD_SIZE], bFillBytes);
        bFifoLevel -= bFillBytes;
        *wRxBytes += bFillBytes;
    }
}

/**
 *Function Name     : phhalI2CM_TxThres_FIFOEmpty_Handler
 *Description       : Handles I2C master operations when FIFO Threshold/Empty reached during transmission operations.
 *Note:             : This function is for I2CM HAL internal use only.
 *                    It should not be called directly by the user API's.
 */
static void phhalI2CM_TxThres_FIFOEmpty_Handler( phhalI2CM_Transaction_t *psTransQ, uint16_t *wBytesQdInFifo )
{
    /* Check for the bytes to be queued into FIFO for transmission */
    if ((*wBytesQdInFifo >= *((uint32_t *) I2CMASTER_BYTECOUNT_CONFIG_REG)))
    {
        return ;
    }

    phhalI2CM_FillFifo(psTransQ, wBytesQdInFifo);
}

static void phhalI2CM_Cleanup( uint8_t bNumOfTrans )
{
    uint32_t bLoopCount = 0;

    /* Clear all the static global variables used. */
    gbTransCompletedCnt = 0;
    gfSlaveCheck = false;
    gwRcvdBytes = 0;
    gbQueueCount = 0;
    gfI2CMTransState = false;
    gwTotalQuedBytes = 0;
    for (bLoopCount = 0; bLoopCount < bNumOfTrans; bLoopCount++)
    {
        gaNoOfBytesQuedInFifo[bLoopCount] = 0;
    }

    /* Flush Fifo */
    PH_REG_SET_BIT(I2CMASTER_CONFIG_REG, FIFO_FLUSH);
}

static phStatus_t phhalI2CM_Queue(uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData)
{
    /* Check whether I2C transaction already going on */
    if (gfI2CMTransState)
        return  PH_ERR(TXRX_STARTED_ERROR, I2CM);

    /* Discard parameter checks if it is Slave check. */
    if (gfSlaveCheck == false)
    {
        /* Check if the FIFO is already full OR max no of Transactions reached from the previous queues */
        if ((gwTotalQuedBytes >= PHHAL_I2CM_MAXIMUM_FIFO_SIZE)
            || (gbQueueCount >= PHHAL_I2CM_MAX_QUEUE_TRANSACTIONS))
        {
            return PH_ERR(QUEUE_NOT_EMPTY_ERROR,I2CM);
        }
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the NULL pointers */
        /* Check whether the byte count is more than 1024 data bytes that can be transmitted in a transaction */
        if ((NULL == pdwData) || (0 == wDataLength) || (wDataLength > PHHAL_I2CM_MAXIMUM_BYTE_COUNT))
        {
            return PH_ERR(INVALID_PARAMETER,I2CM);
        }
#endif
    }

    /* Queue the transaction. */
    gpsTransactionQueue[gbQueueCount].bRetryCount = gbI2CMRetryCnt;
    gpsTransactionQueue[gbQueueCount].wDataLength = wDataLength;
    gpsTransactionQueue[gbQueueCount].pdwData = pdwData;
    gpsTransactionQueue[gbQueueCount].wSlaveAddr = wSlaveAddr;

    gpsTransactionQueue[gbQueueCount].bWordCount = (uint16_t)(wDataLength / PHHAL_I2CM_WORD_SIZE);
    if ((wDataLength % PHHAL_I2CM_WORD_SIZE))
    {
        gpsTransactionQueue[gbQueueCount].bWordCount++;
    }

    gwTotalQuedBytes += gpsTransactionQueue[gbQueueCount].bWordCount * PHHAL_I2CM_WORD_SIZE;

    /*Increment transaction queue count*/
    gbQueueCount++;

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHHAL_I2CM */
