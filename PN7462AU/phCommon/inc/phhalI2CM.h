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
 * The I2C Master APIs
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALI2CM_H
#define PHHALI2CM_H

/** \addtogroup phhalI2CM
 *
 * The I2C Master APIs
 *
 * This module implements I2CM interface Flash code HAL, specific to I2CM module.
 * This module is not designed for multi thread usage.
 *
 * @warning
 *
 *  If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *  checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *  and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *  failures.
 *
 *  @{
 *
 * \dot
 *
 *  digraph "Sample Usage" {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          Start;
 *          End;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalI2CM_Config [ URL="\ref phhalI2CM_Config"];
 *          phhalI2CM_DeInit[ URL="\ref phhalI2CM_DeInit"];
 *          phhalI2CM_GenCallReset [ URL="\ref phhalI2CM_GenCallReset"];
 *          phhalI2CM_Init [ URL="\ref phhalI2CM_Init"];
 *          //phhalI2CM_QueueRx [ URL="\ref phhalI2CM_QueueRx"];
 *          //phhalI2CM_QueueTx [ URL="\ref phhalI2CM_QueueTx"];
 *          phhalI2CM_Receive [ URL="\ref phhalI2CM_Receive"];
 *          phhalI2CM_Reset [ URL="\ref phhalI2CM_Reset"];
 *          phhalI2CM_SlaveCheck [ URL="\ref phhalI2CM_SlaveCheck"];
 *          //phhalI2CM_Start [ URL="\ref phhalI2CM_Start"];
 *          phhalI2CM_Transmit [ URL="\ref phhalI2CM_Transmit"];
 *
 *      //Logical Flow
 *          Start -> phhalI2CM_Init -> phhalI2CM_Config;
 *          phhalI2CM_Config -> phhalI2CM_Receive;
 *          phhalI2CM_Config -> phhalI2CM_Transmit;
 *          phhalI2CM_Receive -> phhalI2CM_DeInit;
 *          phhalI2CM_Transmit -> phhalI2CM_DeInit;
 *          phhalI2CM_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *          phhalI2CM_Config -> phhalI2CM_SlaveCheck [label = "If there are\nmultiple slaves"];
 *          phhalI2CM_SlaveCheck -> phhalI2CM_Receive;
 *          phhalI2CM_SlaveCheck -> phhalI2CM_Transmit;
 *          phhalI2CM_Config -> phhalI2CM_GenCallReset;
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *          phhalI2CM_Reset -> End;
 *
 *      }
 * \enddot
 *
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_I2CM
#include <ph_Status.h>
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/** Maximum of 1023 byte can be transmitted or received in a frame. */
#define PHHAL_I2CM_MAXIMUM_BYTE_COUNT                   (1024)


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * I2CM slave address type.
 * */
typedef enum{
    /** \b 0x00 */ E_I2CM_7BIT_ADDR_MODE, /**<I2C master 7 bit address mode*/
    /** \b 0x01 */ E_I2CM_10BIT_ADDR_MODE     /**<I2C master 10 bit address mode*/
}phhalI2CM_Addr_Options_t;

/** \addtogroup grpErr_HalI2CM
 *
 * Error codes for I2C Master HAL
 *
 * @{
 */
/**
 * I2C Master Interface Error Code ENUMs
 */
typedef enum{
    /** \b 0x80 */ PH_ERR_ADDR_NAK_ERROR  = (PH_ERR_CUSTOM_BEGIN), /**<Address Nack Error*/
    /** \b 0x82 */ PH_ERR_ARB_ERROR       =  0x82, /**<Arbitration Error*/
    /** \b 0x84 */ PH_ERR_DATA_NAK_ERROR  =  0x84, /**<Data Nack Error*/
    /** \b 0x88 */ PH_ERR_DATA_BUS_ERROR  =  0x88, /**<Bus Error*/
    /** \b 0x89 */ PH_ERR_I2C_ERROR, /**<I2C Error*/
    /** \b 0x8A */ PH_ERR_TXRX_STARTED_ERROR, /**<I2C transaction already going on*/
    /** \b 0x8B */ PH_ERR_QUEUE_NOT_EMPTY_ERROR    /**<Queue Not Empty*/
}phhalI2CM_ErrorCodes_t;
/** @} */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * Function Pointer to an API that configures the transaction related parameters
 */
typedef phStatus_t (phhalI2CM_DeviceConfig_t) (
		uint8_t bBaudRate, uint8_t bSdaHoldTime, phhalI2CM_Addr_Options_t eAddrType);
/**
 * Function Pointer to an API that resets I2C Master HW block which makes bus idle.
 */
typedef phStatus_t (phhalI2CM_Reset_t)(void);

/**
 * This API initializes the HAL Context of I2CM HAL, enables the clock, performs HW reset, initialization
 * of threshold
 * @param[in] bTxThres Defines the level of FIFO at which IRQ should be generated when I2CM Tx is Ongoing
 * @param[in] bRxThres Defines the level of FIFO at which IRQ should be generated when I2CM Rx is Ongoing
 * @param[in] dwTicksToWait Defines the common timeout to be used for Transmit/Recieve APIs.
 * @param[in] bRetryCount Defines the number of times a transaction to be retried before aborting on errors
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS if initialization is completed successfully.
 * @maskedret #PH_ERR_RESOURCE_ERROR - In RTOS environments, If the Event creation failed.
 */
phStatus_t phhalI2CM_Init(uint8_t bTxThres, uint8_t bRxThres, uint32_t dwTicksToWait, uint8_t bRetryCount);


/**
 * This API configures the transaction related parameters
 * @param[in] bBaudRate Defines the baudrate of I2C Bus for all further transactions
 * @param[in] bSdaHoldTime Defines the SDA Hold Time for I2C Bus for all further transactions
 * @param[in] eAddrType Defines the address type 7/10 bit address for all further transactions
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS Configuration completed successfully.
 */
#define phhalI2CM_Config(bBaudRate, bSdaHoldTime, eAddrType)        \
    ((phhalI2CM_DeviceConfig_t *) \
    		PH_HAL_I2CM_DEVICECONFIG_ADDR)\
            (bBaudRate, bSdaHoldTime, eAddrType)

/**
 * This API Performs I2C slave presence check on I2C bus for a specific 7 bit slave address
 * @note This API shall be called ( may be multiple times) for different slaves
 *       before performing transactions(Queue/Read/Write) to these slaves.
 * @note Calling this API in between any transaction may result in undefined behavior.
 *
 * @param[in] wSlaveAddr 7 bit Address of the slave to be checked
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the operation completed successfully, slave presence is detected.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT Operation didn't succeeded wit in specified dwTicksToWait time.
 * @maskedret #PH_ERR_ADDR_NAK_ERROR I2C Master returned with NAK (either slave is busy/not present).
 * @maskedret #PH_ERR_ARB_ERROR I2C Master looses the bus.
 * @maskedret #PH_ERR_DATA_NAK_ERROR Slave naked the data reception.
 * @maskedret #PH_ERR_DATA_BUS_ERROR I2C bus error.
 * @maskedret #PH_ERR_QUEUE_NOT_EMPTY_ERROR I2C queue is already filled with other Tx/Rx transactions.
 */
phStatus_t phhalI2CM_SlaveCheck(uint16_t wSlaveAddr);


#ifdef NXPBUILD__PHHAL_I2CM_MULTI_TRANSACTION
/**
 * This API is used to queue a Tx Transaction if application wishes to transmit multiple transactions
 * in one go without returning to application. All the transactions combined length should be less than
 * 32 bytes. Each transaction can have different slave addresses but either 7bit/10 bit.
 * This is used primarily for TDA usecase.
 *
 * @param[in] wSlaveAddr  Defines the 7 bit or 10 bit slave address
 * @param[in] wDataLength Defines the length of Tx Data to be transmitted in one transaction, max of 32 bytes
 * @param[in] pdwData     Defines the pointer to the Tx buffer and shall be 4 byte aligned
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the Tx queuing completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_QUEUE_NOT_EMPTY_ERROR I2C queue is already filled with other Tx/Rx transactions.
 */
phStatus_t phhalI2CM_QueueTx(uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData);


/**
 * This API is used to queue a Rx Transaction if application wishes to receive multiple transactions
 * in one go without returning to application. All the transactions combined length should be less than
 * 32 bytes. Each transaction can have different slave addresses but either 7bit/10 bit.
 * This is used primarily for TDA usecase.
 * @note Queue Tx and Queue Rx shall not be mixed before phhalI2CM_Start.
 * @param[in] wSlaveAddr  Defines the 7 bit or 10 bit slave address
 * @param[in] wDataLength Defines the length of Rx Data to be received in one transaction, max of 32 bytes
 * @param[out] pdwData     Defines the pointer to the Rx buffer and shall be 4 byte aligned
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the Rx queuing completed successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_QUEUE_NOT_EMPTY_ERROR I2C queue is already filled with other Tx/Rx transactions.
 */
phStatus_t phhalI2CM_QueueRx(uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData);


/**
 * This API starts the previously queued transactions.
 * @note This API is used only if phhalI2CM_QueueTx/phhalI2CM_QueueRx is called previously without which
 * the behaviour is undefined.
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT Operation didn't succeeded within specified dwTicksToWait time.
 * @maskedret #PH_ERR_ADDR_NAK_ERROR I2C Master returned with NAK (either slave is busy/not present).
 * @maskedret #PH_ERR_ARB_ERROR I2C Master looses the bus.
 * @maskedret #PH_ERR_DATA_NAK_ERROR Slave naked the data reception.
 * @maskedret #PH_ERR_DATA_BUS_ERROR I2C bus error.
 */
phStatus_t phhalI2CM_Start(void);

#endif


/**
 * This API performs one transmit transaction for a maximum of 1024 bytes
 * @param[in] wSlaveAddr  Defines the 7 bit or 10 bit slave address
 * @param[in] wDataLength Defines the length of Tx Data to be transmitted in one transaction and shall be
 * less than PHHAL_I2CM_MAXIMUM_BYTE_COUNT
 * @param[in] pdwData     Defines the pointer to the Tx buffer and  shall be 4 byte aligned
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT Operation didn't succeeded within specified dwTicksToWait time.
 * @maskedret #PH_ERR_ADDR_NAK_ERROR I2C Master returned with NAK (either slave is busy/not present).
 * @maskedret #PH_ERR_ARB_ERROR I2C Master looses the bus.
 * @maskedret #PH_ERR_DATA_NAK_ERROR Slave naked the data reception.
 * @maskedret #PH_ERR_DATA_BUS_ERROR I2C bus error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_QUEUE_NOT_EMPTY_ERROR I2C queue is already filled with other Tx/Rx transactions.
 */
phStatus_t phhalI2CM_Transmit(uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData);



/**
 * This API performs one receive transaction for a maximum of 1024 bytes
 * @param[in] wSlaveAddr  Defines the 7 bit or 10 bit slave address
 * @param[in] wDataLength Defines the length of Rx Data to be received in one transaction and shall be
 * less than PHHAL_I2CM_MAXIMUM_BYTE_COUNT
 * @param[out] pdwData     Defines the pointer to the Rx buffer and shall be 4 byte aligned
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT Operation didn't succeeded within specified dwTicksToWait time.
 * @maskedret #PH_ERR_ADDR_NAK_ERROR I2C Master returned with NAK (either slave is busy/not present).
 * @maskedret #PH_ERR_ARB_ERROR I2C Master looses the bus.
 * @maskedret #PH_ERR_DATA_NAK_ERROR Slave naked the data reception.
 * @maskedret #PH_ERR_DATA_BUS_ERROR I2C bus error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_QUEUE_NOT_EMPTY_ERROR I2C queue is already filled with other Tx/Rx transactions.
 */
phStatus_t  phhalI2CM_Receive(uint16_t wSlaveAddr, uint16_t wDataLength, uint32_t *pdwData);


/**
 * This API performs I2C Bus Reset that resets all attached slaves.
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS If the operation completed successfully.
 * @maskedret #PH_ERR_TXRX_STARTED_ERROR I2C Master transaction ongoing error.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT Operation didn't succeeded within specified dwTicksToWait time.
 * @maskedret #PH_ERR_ADDR_NAK_ERROR I2C Master returned with NAK (either slave is busy/not present).
 * @maskedret #PH_ERR_ARB_ERROR I2C Master looses the bus.
 * @maskedret #PH_ERR_DATA_NAK_ERROR Slave naked the data reception.
 * @maskedret #PH_ERR_DATA_BUS_ERROR I2C bus error.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid passing parameter error.
 * @maskedret #PH_ERR_QUEUE_NOT_EMPTY_ERROR I2C queue is already filled with other Tx/Rx transactions.
 */
phStatus_t phhalI2CM_GenCallReset(void);


/**
 * This API performs the I2C Master HW block reset which makes bus idle.
 * @note This API is used asynchronously to abort any hanged transaction.
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS Operation completed successfully.
 */
#define phhalI2CM_Reset()    \
	((phhalI2CM_Reset_t *) \
			PH_HAL_I2CM_RESET_ADDR)()

/**
 * This API de-initializes the I2CM HAL and HW.
 * @note No APIs other than phhalI2CM_Init() shall be called after this API
 * @return Status of the Operation
 *
 * @return #PH_ERR_SUCCESS Operation completed successfully.
 */
phStatus_t phhalI2CM_DeInit(void);

#endif /* NXPBUILD__PHHAL_I2CM */
/** @} */
#endif /* end PHHALI2CM_H */
