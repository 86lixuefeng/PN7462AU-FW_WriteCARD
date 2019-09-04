/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */
/** @file
 *
 * Header for Host Interface Core
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 *
 */

#ifndef PHHALHIF_H
#define PHHALHIF_H

/**
 * \addtogroup phhalHif
 *
 * The HOST Interface APIs
 *
 * This component implements the Host Interface HAL functionalities for I2C Slave, SPI Slave and HSUART Slave
 *
 * @warning
 *
 *  If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *  checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *  and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *  failures.
 *
 *
 * @{
 *
 *
 * \dot
 *      digraph "Sample Usage"{
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *          phhalHif_AbortTransmission [ URL="\ref phhalHif_AbortTransmission"];
 *          phhalHif_ConfigureWaterLevel [ URL="\ref phhalHif_ConfigureWaterLevel"];
 *          phhalHif_DeInit [ URL="\ref phhalHif_DeInit"];
 *          phhalHif_DeInitRxBuffer [ URL="\ref phhalHif_DeInitRxBuffer"];
 *          phhalHif_Init [ URL="\ref phhalHif_Init"];
 *          phhalHif_InitRxBuffer [ URL="\ref phhalHif_InitRxBuffer"];
 *          phhalHif_ReleaseRxBuffer [ URL="\ref phhalHif_ReleaseRxBuffer"];
 *          phhalHif_Reset [ URL="\ref phhalHif_Reset"];
 *          phhalHif_Transmit [ URL="\ref phhalHif_Transmit"];
 *
 *          Start -> phhalHif_Init;
 *          phhalHif_Init -> phhalHif_Transmit ;
 *          phhalHif_Transmit -> phhalHif_DeInit [label = "After sending data"];
 *          phhalHif_Init -> phhalHif_InitRxBuffer;
 *          phhalHif_InitRxBuffer -> phhalHif_ReleaseRxBuffer [label = "After receiving data"];
 *          phhalHif_ReleaseRxBuffer -> phhalHif_DeInitRxBuffer -> phhalHif_DeInit;
 *
 *          //optionals
 *          edge [ style="dashed", color="blue" ];
 *          phhalHif_Init -> phhalHif_ConfigureWaterLevel;
 *          phhalHif_ConfigureWaterLevel -> phhalHif_Transmit;
 *          phhalHif_ConfigureWaterLevel -> phhalHif_InitRxBuffer;
 *
 *          //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *          phhalHif_Transmit -> phhalHif_AbortTransmission -> phhalHif_DeInit;
 *
 *          phhalHif_DeInit -> End;
 *          phhalHif_Reset -> End;
 *          phhalHif_AbortTransmission -> End;
 *      }
 * \enddot
 *
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_HIF
#include "ph_Status.h"
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 *  Callback Function pointer definition which will be called from Isr
 */
typedef void (*pphhalHif_Callback_t)(uint32_t , void *);

/**
 * This structure gives the various receive buffers available
 */
typedef enum phhalHif_BuffId
{
    /** \b 0x00 */ E_RX_BUFFER_ID0 = 0, /**< Identification of Buffer ID0 */
    /** \b 0x01 */ E_RX_BUFFER_ID1,     /**< Identification of Buffer ID1 */
    /** \b 0x02 */ E_RX_BUFFER_ID2,     /**< Identification of Buffer ID2 */
    /** \b 0x03 */ E_RX_BUFFER_ID3      /**< Identification of Buffer ID3 */
} phhalHif_BuffId_t;

/**
 * This structure gives the various buffer types
 */
typedef enum phhalHif_BuffType
{
    /** \b 0x00 */ E_RX_BUFFER_NORMAL, /**< General Buffer which is capable of receiving  */
    /** \b 0x01 */ E_RX_BUFFER_SHORT   /**< Buffer which is capable of only receiving if the frame is less than or equal to 3 bytes*/
} phhalHif_BuffType_t;


/**
 * This structure gives the various buffer formats supported
 */
typedef enum phhalHif_BuffFormat
{
    /** \b 0x00 */ E_BUFFER_FORMAT_FREE,   /**< Transparent */
    /** \b 0x01 */ E_BUFFER_FORMAT_FIXED,  /**< Header + Payload + Crc */
    /** \b 0x02 */ E_BUFFER_FORMAT_NATIVE  /**< Transparent with Size at 1st Word*/
} phhalHif_BuffFormat_t;

/**
 * This structure gives the available baud rates for HSU
 */
typedef enum phhalHif_Hsu_BaudRate
{
    /** \b 0x00 */ E_HSU_BAUDRATE_9_6K = 0, /**< Baud Rate 9600 */
    /** \b 0x01 */ E_HSU_BAUDRATE_19_2K,    /**< Baud Rate 19200 */
    /** \b 0x02 */ E_HSU_BAUDRATE_38_4K,    /**< Baud Rate 38400 */
    /** \b 0x03 */ E_HSU_BAUDRATE_57_6K,    /**< Baud Rate 57600 */
    /** \b 0x04 */ E_HSU_BAUDRATE_115_2K,   /**< Baud Rate 115200 */
    /** \b 0x05 */ E_HSU_BAUDRATE_230_4K,   /**< Baud Rate 230400 */
    /** \b 0x06 */ E_HSU_BAUDRATE_460_8K,   /**< Baud Rate 460800 */
    /** \b 0x07 */ E_HSU_BAUDRATE_921_6K,   /**< Baud Rate 921600 */
    /** \b 0x08 */ E_HSU_BAUDRATE_1288K,    /**< Baud Rate 1288000 */
    /** \b 0x09 */ E_HSU_BAUDRATE_2400K,    /**< Baud Rate 2400000 */
    /** \b 0x0A */ E_HSU_BAUDRATE_3500K,    /**< Baud Rate 3500000 */
    /** \b 0x0B */ E_HSU_BAUDRATE_3750K,    /**< Baud Rate 3750000 */
    /** \b 0x0C */ E_HSU_BAUDRATE_4000K,    /**< Baud Rate 4000000 */
    /** \b 0x0D */ E_HSU_BAUDRATE_5000K,    /**< Baud Rate 5000000 */
    /** \b 0x0E */ E_HSU_BAUDRATE_SEMIAUTO, /**< Baud Rate will be estimated, but will not be applied */
    /** \b 0x0F */ E_HSU_BAUDRATE_AUTO      /**< Baud Rate will be estimated and will be applied automatically */
} phhalHif_Hsu_BaudRate_t;

/**
 * This structure gives the various HIF modes available
 */
typedef enum phhalHif_Sel
{
    /** \b 0x00 */ E_HIF_DISABLE, /**< Hif is disabled */
    /** \b 0x01 */ E_HIF_I2C,     /**< Hif I2C slave is selected*/
    /** \b 0x02 */ E_HIF_SPI,     /**< Hif SPI slave is selected*/
    /** \b 0x03 */ E_HIF_HSU,     /**< Hif HSU slave is selected*/
    /** \b 0x04 */ E_HIF_USB      /**< Hif USB is selected */
} phhalHif_Sel_t;

/**
 * This structure gives the configuration parameters needed for HSU initialization
 */
typedef struct phhalHif_Hsu
{
    uint32_t bIsHsuBoot;                     /**< Will indicate whether the boot is because of HSU Activity */
    uint8_t bEOF;                           /**< Eof size - Maximum interbyte duration */
    uint8_t bStopBits;                      /**< No of stop bits */
    uint8_t bDummyBytes;                    /**< No of Dummy bytes - mainly used during standby phase */
    phhalHif_Hsu_BaudRate_t bBaudRate;    /**< Baud rate to be used */
} phhalHif_Hsu_t;

/**
 * This structure gives the configuration parameters needed for HIF initialization
 */
typedef struct phhalHif_Config
{

    /** For Cofiguring the I2C,SPI and HSU register */
    union phhalHif_IntfConfig
    {
        /** BIT 7-5 - Die Revision ID \n
            BIT 4   - Enable Device ID \n
            BIT 3   - Reset Enable \n
            BIT 2   - Enable HS Mode \n
            BIT 0-1 - Two Bit Slave Address */
        uint8_t sI2cConfig;

        /** BIT 1 - CPHA \n
            BIT 0 - CPOL */
        uint8_t sSpiConfig;

        phhalHif_Hsu_t sHsuConfig;        /**< For configuring HSU when Hif is selected as HSU */
    } sConfig;                            /**< I2C/SPI/HSU Interface Config */

    uint16_t bTimeout;                    /**< Inter character Tx Timeout in steps of 3.6us */
    phhalHif_Sel_t eInterface;            /**< Selected HIF interface */
    phhalHif_BuffFormat_t eBufferType;    /**< Free or Fixed or Native */
    uint8_t bShortFrameLen;               /**< Number of bytes representing ShortFrame */
    uint8_t bStoreErrData;                /**< Store error data or discard error data */
    uint8_t bHeaderSize;                  /**< Header Size in Fixed Format */

} phhalHif_Config_t;

/**
 * This structure gives the configuration which are used to store the context for all the initialized receive buffers
 */
typedef struct phhalHif_RxBuffContext
{
    uint32_t *pdwBuffAddr;        /**< 32Byte aligned Buffer pointer where the data will be saved */
    uint32_t IsDataAvailable;   /**< Represents whether the data is available or not available */
    uint32_t dwDataLength;      /**< No of bytes received in the buffer */
    uint16_t dwMaxSize;         /**< Max size that the Buffer can receive */
} phhalHif_RxBuffContext_t;

/** \addtogroup grpErr_HalHif
 *
 * Error codes for HIF HAL
 *
 * @{
 */
 /** HIF HAL Interface Error Code ENUMs
 */
typedef enum phhalHif_ErrorCodes {
    PH_ERR_HIF_TX_ONGOING = (PH_ERR_CUSTOM_BEGIN+1),  /**< When previous transmission is still ongoing */
    PH_ERR_HIF_RX_ONGOING,                            /**< When reception is ongoing */
    PH_ERR_HIF_TX_BUFFER_LOCKED,                      /**< TX Buffer is locked by the H/w */
    PH_ERR_HIF_RX_BUFFER_LOCKED,                      /**< RX Buffer is locked by the H/w */
    PH_ERR_HIF_TX_PREFETCH_FAILED,                    /**< Prefetch Failed while initiating transmission */
    PH_ERR_HIF_RX_BUFFER_ALREADY_ENABLED,              /**< RX Buffer is already enabled */
    PH_ERR_HIF_USB_PVDD_3V_NOK                          /**< PVDD of 3V is not available for USB */
} phhalHif_ErrorCodes_t;
/** @} */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * Function Pointer to an API that Stops HIF IP Clock
 */
typedef phStatus_t (phhalHif_DeInit_t)(void);
/**
 * Function Pointer to an API that Aborts the transmission
 */
typedef phStatus_t (phhalHif_AbortTransmission_t)(void);

/**
 * This API initializes HIF HAL Context, enables NVIC Interrupt, and enables the HIF HW
 * @param[in] pConfig Pointer to the HIF Configuration Structure.
 * @param[in] pErrorCallBack Pointer to Call back function called by HAL on any HIF errors
 * @return    Returns the status of HIF HAL Initialization
 * @retval    #PH_ERR_SUCCESS when the initialization is successfully done.
 * @maskedret #PH_ERR_INVALID_PARAMETER if any of the configuration parameter is invalid
 */
phStatus_t phhalHif_Init(
                    phhalHif_Config_t *pConfig,
                    pphhalHif_Callback_t pErrorCallBack);


/**
 * This API transmits a buffer of wLength over the previously selected & configured host interface. Non blocking API.
 * @param[in] pdwBuffer Pointer to double word (4 byte) aligned memory
 * @param[in] wLength Length of the buffer to be transmitted Maximum 1024 in for Fixed format and 256 for Free Format
 * @param[in] pCallBackPtr Pointer to the callback function to be called upon tx completion.
 * @return    Returns the status of initiating transmission.
 * @retval    #PH_ERR_SUCCESS if the transmission is successfully is completed
 * @maskedret #PH_ERR_INVALID_PARAMETER if the callback function is not passed
 * @maskedret #PH_ERR_HIF_TX_ONGOING if the hardware is in process of transmitting the data
 * @maskedret #PH_ERR_HIF_TX_BUFFER_LOCKED if there is already a transmission initiated it is pending to completed
 * @maskedret #PH_ERR_HIF_TX_PREFETCH_FAILED if the prefetch of the data fails
 *
 */
phStatus_t phhalHif_Transmit(uint32_t *pdwBuffer, uint16_t wLength,
                    pphhalHif_Callback_t pCallBackPtr);

/**
 * This API is used to configure the number of received bytes at which an interrupt would be raised by HW to FW.
 * This feature is used to inform other modules to be prepared of any incoming frame.
 * @param[in] wWaterLevel  Waterlevel threshold
 * @param[in] pCallBackPtr Callback function to be called upon receiving the configured threshold bytes.
 * @return #PH_ERR_SUCCESS. Always returns success.
 */
phStatus_t phhalHif_ConfigureWaterLevel(uint16_t wWaterLevel, pphhalHif_Callback_t  pCallBackPtr);
/**
 * This API is used to abort a transmission that has been initiated by the application.
 * @return #PH_ERR_SUCCESS. Always returns success.
 */
#define phhalHif_AbortTransmission()          \
	((phhalHif_AbortTransmission_t *)\
			PH_HAL_HIF_ABORTTRANSMISSION_ADDR)()

/**
 * This API will allocate and enable one of 4 rx buffers for reception from host.
 * @param[in] sBuffId      Buffer ID of the register which should be initialized
 * @param[in] wMaxSize     Max Length of the data which can be received by the buffer
 * @param[in] pdwBuffAddr  Pointer to the 32byte aligned buffer to which the data has to be saved
 * @param[in] pCallBackPtr Pointer to the function which should be called when reception happens
 * @return Returns the status of rx buffer initialization
 * @retval    #PH_ERR_SUCCESS if the rx buffer is successfully enabled for reception
 * @maskedret #PH_ERR_INVALID_PARAMETER if the paramters passed by application is invalid
 * @maskedret #PH_ERR_HIF_RX_BUFFER_ALREADY_ENABLED if the rx buffer is already enabled.
 */
phStatus_t phhalHif_InitRxBuffer(phhalHif_BuffId_t sBuffId, uint16_t wMaxSize,
                        uint32_t *pdwBuffAddr, pphhalHif_Callback_t pCallBackPtr);


/**
 * This API will de-allocate an already allocated rxbuffer
 * @param[in] sBuffId Buffer ID of the register which should be De-initialized
 * @return #PH_ERR_SUCCESS is always returned.
 */
phStatus_t phhalHif_DeInitRxBuffer(phhalHif_BuffId_t  sBuffId);

/**
 * This API will re-enable an already allocated rx buffer since it would been disabled upon reception of a prev frame.
 * @param[in] sBuffId  Buffer ID of the register which should be Released
 * @return Returns the status of releasing the buffer
 * @retval    #PH_ERR_SUCCESS if the rx buffer is successfully re-enabled
 * @maskedret #PH_ERR_HIF_RX_ONGOING if the rx buffer is being used by HW for an ongoing reception.
 */
phStatus_t phhalHif_ReleaseRxBuffer(phhalHif_BuffId_t  sBuffId);

/**
 * This API is used to perform a reset of the HIF HW in case of any undeterministic behaviour
 * @note: This API can be called from an asynchronous context
 * @return Return the status of de-init and init API.
 * @retval    #PH_ERR_SUCCESS if the reset is done successfully
 */
phStatus_t phhalHif_Reset(void);


/**
 * This API will de-initialise the HIF HAL, stop the HIF IP Clock,
 * reset all buffer registers, disable the selection of HIF, disable NVIC interrupt.
 * @return Returns the status of de-initialization
 * @retval    #PH_ERR_SUCCESS if de-initialization is successful
 * @maskedret #PH_ERR_FAILED if HIF Clock is not enabled
 * @maskedret #PH_ERR_HIF_RX_BUFFER_LOCKED if there is Reception ongoing and the buffer is locked by the hardware
 *
 */
#define phhalHif_DeInit()      \
	((phhalHif_DeInit_t *)\
			PH_HAL_HIF_DEINIT_ADDR)()

#endif /* NXPBUILD__PHHAL_HIF */
/** @} */
#endif /* PHHALHIF_H */
