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
 * \addtogroup phpalCt
 *
 * The Contact Interface (PAL) APIs
 *
 * The purpose and scope of this file is to implement CT PAL, Prototype of the CT Interface PAL
 * This module implements contact interface Api's in flash memory.
 * This module designed and can be extended to multi-process environment.
 * This module contains protocol layer abstraction Apis which will call Hardware Abstraction Apis Internally.
 * The user of this module can use these Apis for contact protocol(T=0 or T=1) Communication.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input
 *                  parameters will not be checked for erroneous values in the API. In such a build configuration use
 *                  the API with caution and pass valid parameters only. Invalid parameters will likely result in
 *                  hardfaults or other failures.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:56:04 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17074 $ (v04.13.00)
 * @{
 *
 * \dot
 *
 *      digraph "Sample Usage" {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phpalCt_ActivateCard [ URL="\ref phpalCt_ActivateCard"];
 *          phpalCt_DeInit [ URL="\ref phpalCt_DeInit"];
 *          phpalCt_Init [ URL="\ref phpalCt_Init"];
 *          phpalCt_SetNAD [ URL="\ref phpalCt_SetNAD"];
 *          phpalCt_SetProtocol [ URL="\ref phpalCt_SetProtocol"];
 *          phpalCt_Transceive [ URL="\ref phpalCt_Transceive"];
 *
 *      //Logical Flow
 *      Start -> phpalCt_Init;
 *      phpalCt_Init -> phpalCt_ActivateCard;
 *      phpalCt_ActivateCard -> phpalCt_Transceive
 *      phpalCt_Transceive -> phpalCt_DeInit[label = "Call the CT HAL Deactivate directly"];
 *      phpalCt_DeInit -> End;
 *
 *      //optionals
 *      edge [ style="dashed", color="blue" ];
 *          phpalCt_ActivateCard -> phpalCt_SetProtocol;
 *          phpalCt_SetProtocol -> phpalCt_Transceive;
 *          phpalCt_ActivateCard -> phpalCt_SetNAD;
 *          phpalCt_SetNAD -> phpalCt_Transceive;
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 */

#ifndef PHPAL_CT_H
#define PHPAL_CT_H
/* *****************************************************************************************************************
 * Includes
 * **************************************************************************************************************** */
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHPAL_CT
#include "phhalCt.h"
#include "ph_Status.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * **************************************************************************************************************** */
/**
 *Maximum Allowed Atr bytes for EMVCo Profile.
 */
#define PHPAL_CT_EMV_MAX_ATR_BYTES           32
/**
 *Maximum Allowed Atr bytes for 7816 Profile.
 */
#define PHPAL_CT_7816_MAX_ATR_BYTES          33
/**
 * Maximum possible Apdu size in T=0 protocol
 */
#define PHPAL_CT_MAX_APDU_SIZE_T0       261
/**
 * Minimum possible Apdu size in T=0 protocol
 */
#define PHPAL_CT_MIN_APDU_SIZE_T0       4

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * **************************************************************************************************************** */
/**
 * Enum for different protocol types
 */
typedef enum phpalCt_ProtocolType
{
    E_PHPAL_CT_T0 = 0x00,        /**< T=0 protocol  */
    E_PHPAL_CT_T1 = 0x01,        /**< T=1 protocol */
    E_PHPAL_CT_BOTH_T0_T1,       /**< T=0 and T=1 both protocol supported. */
    E_PHPAL_CT_INVALID_PROT = 0xFF        /**< Invalid protocol selection */
} phpalCt_ProtocolType_t;

/**
 * Enumeration for the different class of the card
 */
typedef enum
{
    E_PHPAL_CT_VCC5             = 0x00,         /**< Class A is selected */
    E_PHPAL_CT_VCC3             = 0x01,         /**< Class B is selected */
    E_PHPAL_CT_VCC1M8           = 0x02,         /**< Class C is selected */
    E_PHPAL_CT_INVALID_CLASS    = 0xFF          /**< Invalid class selected */
} phpalCt_ClassType_t;

/** \addtogroup grpErr_PalCt
 *
 * Error codes for CT PAL
 *
 * @{
 *
 * CT PAL Interface Error ENUMs */

 /**
  * These enums are used by PAL Apis for indicating different error status codes.
  */
typedef enum {
    PH_ERR_CT_PROTOCOL_NOT_SUPPORTED = (PH_ERR_CUSTOM_BEGIN+0x0001), /**< Selected protocol not supported */
    PH_ERR_CT_RETRY_COUNT_FAILURE,      /**< Terminal has tried retry for maximum retry count. */
    PH_ERR_CT_INS_COMMAND_ERROR,        /**< Invalid procedure byte received.  */
    PH_ERR_CT_EXT_APDU_NOT_SUPPORTED,   /**< Extended Apdu is passed for T=0 protocol.*/
    PH_ERR_CT_RESYNCH_SUCCESS,          /**< Resynchronisation is done successfully in 7816 profile.*/
    PH_ERR_CT_CHAINING_ABORT_REQUESTED, /**< Abort chaining Request is received from card in T= 1.*/
    PH_ERR_CT_DADSAD_NOT_SUPPORTED,     /**<SAD or DAD is not supported for NAD.*/
    PH_ERR_CT_PAL_SUCCESS_TX_CHAINING,  /**< last TX block transmitted successfully, Send next Tx Chain block */
    PH_ERR_CT_PAL_SUCCESS_RX_CHAINING,  /**< last RX block received successfully, RX Chaining in progress. One or More Blocks to be received */
    PH_ERR_CT_PAL_INVALID               /**< Invalid Enum */
} phpalCt_ErrorCodes_t;
/** @} */

/**
 * Enum for Options for Split Chaining Transceive API
 */
typedef enum phpalCt_TransceiveOption
{
    E_PHPAL_CT_TXRX_DEFAULT = 0x00,    /**< Data to be Transmitted followed by Data Receive, without Chaining in Tx */
    E_PHPAL_CT_TX_CHAINING  = 0x01,    /**< Data to be transmitted with Chaining, and will be followed by another Tx data Block */
    E_PHPAL_CT_RX_CHAINING  = 0x02     /**< Data to be received with Chaining (May or May not be followed by more RX blocks) */
} phpalCt_TransceiveOption_t;

/**
 * Structure for protocol related parameters,extracted from card's ATR
 */
typedef struct phpalCt_DataParams {
    uint8_t *pbAtrBuffer;           /**< [out] Pointer to Received ATR buffer */
    uint8_t bAtrReceivedLength;     /**< [out] It will store Received ATR's length*/
    uint8_t bSizeOfATRbuffer;       /**< [out] It will specify the size of ATR buffer which will user provide */
    uint8_t bCardSupportedProt;     /**< [out] It will store the supported protocol according to ATR */
    uint8_t bSupportedClass;        /**< [out] Supported class is returned in this variable where
                                       0x00 -> 5V, 0x01-> 3.3V, 0x02-> 1.8V */
} phpalCt_DataParams_t;

/* *****************************************************************************************************************
 * Extern Variables
 * **************************************************************************************************************** */
/* This variable is made extern to use in EMVCo loop back application.*/
extern uint8_t gphpalCt_ProtSel; /**< Stores final protocol selected for communication.*/
extern uint8_t gphpalCt_BIFSC; /**< Global variable to store IFSC value from ATR for PAL.*/
extern uint8_t gphpalCt_BEmvEn;/**< Global variable to store EMVCo or NonEMVCo profile for PAL.*/
extern uint8_t gphpalCt_BSadDad;/**< NAD for  card to terminal communication.*/
extern uint8_t gphpalCt_BDadSad;/**< NAD for terminal to card communication.*/
extern uint8_t gphpalCt_BCRCPresent; /**< Global variable to tell the presence of CRC.*/

/* *****************************************************************************************************************
 * Function Prototypes
 * **************************************************************************************************************** */

/**
 * This function is used to initialize the CT IP, this is just a wrapper over the CT phhalCt_Init Api.
 * @param[in] psCtInitParams   Pointer to Structure which contains Ct initialization parameters.
 * @retval #PH_ERR_SUCCESS   Initialization of CT IP is successful.
 * @retval #PH_ERR_FAILED    Initialization of CT IP failed.
 */
phStatus_t phpalCt_Init(phhalCt_InitParam_t *psCtInitParams);

/**
 * Set the CT pal configurations to the desired value.
 *
 * @param[in] eConfig CT PAL configurations.
 * @param[in] dwValue Desired value to be set the mentioned CT PAL configuration.
 * @return Status for CT set configurations.
 *
 * @retval    #PH_ERR_SUCCESS    operation is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER   for invalid parameter.
 */
phStatus_t phpalCt_SetConfig(phhalCt_Configs_t eConfig, uint32_t dwValue);

/**
 *This Function does a cold reset/activate of the card for ATR reception, the API
 *performs three iterations for different class selection starting with the CLASS A internally.
 *
 * @param[out]   pDataParams  Pointer to the context structure #phpalCt_DataParams_t of the PAL layer.
 *            Ignore this structure's member values,when return code is other than success or early error.
 *
 * @return  The return value can come from the CT hal layer as well when the activation is not successful.
 *          In this case directly the same return code is passed to the application layer.
 * @retval  #PH_ERR_SUCCESS   If the cold activation is success and ATR is received
 * @retval  #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 */
phStatus_t phpalCt_ActivateCard(phpalCt_DataParams_t *pDataParams);
/**
 *
 *This Function is used to set the Transmission protocol to T=0 or T=1 from the upper layer
 *
 * @param[in]   eProtocolType   #E_PHPAL_CT_T0 or #E_PHPAL_CT_T1 to be passed from the upper layer
 *
 * @retval #PH_ERR_SUCCESS   If the protocol set successfully
 * @retval #PH_ERR_CT_PROTOCOL_NOT_SUPPORTED  If Unsupported protocol parameter is passed
 */
phStatus_t phpalCt_SetProtocol(phpalCt_ProtocolType_t eProtocolType);

/**
 * This function is used to set NAD byte for T=1 communication for 7816.
 * @note : Calling this Api is optional, If User will not call this api then by default NAD byte will be set to'0x00'.
 * @param[in]   bDADSAD   Value for setting NAD for terminal to card communication,bit 8 and 4 should be set as 0.
 *
 * @retval  #PH_ERR_SUCCESS   If NAD is set successfully.
 * @maskedret #PH_ERR_CT_DADSAD_NOT_SUPPORTED   If DAD and SAD both have same nonzero value.
 *                                              If bit 8 or bit 4 is set to 1.
 */
phStatus_t phpalCt_SetNAD(uint8_t bDADSAD);

/**
 *
 * This Api calls the appropriate APi's of the underlying protocol for a T0 or T1 transcieve.
 * The transceive api internally uses the T=0 or T=1 protocol selection and routes the Apdu accordingly.
 * Use this API if the transceive operation is expected to be atomic, ie: the complete Tx data is available
 * and given as input to the CT PAL Transceive API. The Transceive API will return the full Rx data
 * (card response). Any necessary chaining in Tx or Rx direction, will be done internally by the CT PAL.
 *
 * @param[in] pbTransmitBuff   Pointer to the transmit buffer
 * @param[in]  dwTransmitSize  Size of the bytes to be transmitted
 * @param[out]  pbReceiveBuff  Pointer to the receive buffer
 * @param[out]  pwReceiveSize  Pointer to the receive buffer size
 *
 * @retval #PH_ERR_SUCCESS  The Apdu transmitted successfully and received the response successfully.
 * @retval #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 *
 * @maskedret    #PH_ERR_CT_RETRY_COUNT_FAILURE   If Retry count has been exceeded than allowable value.
 * @maskedret    #PH_ERR_CT_INS_COMMAND_ERROR     Improper INS is received from card.
 * @maskedret    #PH_ERR_CT_RESYNCH_SUCCESS   If Resynchronisation happened for 7816 profile.
 * @maskedret    #PH_ERR_CT_CHAINING_ABORT_REQUESTED   If Chaining Abortion request is received from card
 * @maskedret    #PH_ERR_CT_EXT_APDU_NOT_SUPPORTED    If Extended command Apdu is passed for T0 protocol.
 */
phStatus_t phpalCt_Transceive( uint8_t* pbTransmitBuff, uint32_t dwTransmitSize,
                               uint8_t* pbReceiveBuff,  uint16_t* pwReceiveSize);

/**
 *
 * This Api calls the appropriate APi's of the underlying protocol for a T0 or T1 transcieve.
 * The transceive api internally uses the T=0 or T=1 protocol selection and routes the Apdu accordingly.
 * This Transceive API supports Split Tx and RX chaining in T=1 transceive operations.
 * This API takes an additional input parameter "eOption" which is an Enum Constant indicating the expected
 * behaviour of the Tranceive operation.
 *
 * eOption can be passed the values given below. The Error Code returned will dictate the next course of
 * action, as detailed below:
 *
 * E_PHPAL_CT_TXRX_DEFAULT : Complete C-APDU is provided in Transmit Buffer.
 *                           In case of T=1, any Tx chaining, if necessary, will be done internally by the CT PAL
 *                           before this function call returns.
 *                           In case TX chaining was already in progress, Transmit buffer contains the last
 *                           block of the C-APDU.
 *                           Expected error Codes:
 *                           - PH_ERR_SUCCESS : C-APDU sent successfully. Complete R-APDU received successfully
 *                           - PH_ERR_CT_PAL_SUCCESS_RX_CHAINING : C-APDU sent successfully. Part of R-APDU received
 *                             successfully. More RX blocks expected. User needs to call
 *                             phpalCt_Transceive_SplitChaining() with eOption "E_PHPAL_CT_RX_CHAINING"
 *                           - Any other Status : Error Occured. Check error code for more details.
 *
 * E_PHPAL_CT_TX_CHAINING : First chunk or intermediate chunk of the C-APDU (but Not the last chunk) is provided in the
 *                          Transmit Buffer. Tx Chaining must be done.
 *                          Not relevant for T=0 Tranceive operation. Only for T=1.
 *                          Expected error Codes:
 *                          - PH_ERR_CT_PAL_SUCCESS_TX_CHAINING : given C-APDU chunk sent successfully. User needs to
 *                            call phpalCt_Transceive_SplitChaining() to sent next block.
 *                          - Any other Status : Error Occured. Check error code for more details. In this case, even
 *                            PH_ERR_SUCCESS is not expected and should be considered as an error.
 *
 * E_PHPAL_CT_RX_CHAINING : RX chaining ongoing. Transmit Buffer contains nothing. CT PAL needs to receive a chunk of the
 *                          RAPDU from the card.
 *                          Not relevant for T=0 Tranceive operation. Only for T=1.
 *                          Expected error Codes:
 *                          - PH_ERR_SUCCESS : Last chunk of the R-APDU received successfully.
 *                          - PH_ERR_CT_PAL_SUCCESS_RX_CHAINING : One chunk of R-APDU received successfully. More RX
 *                            blocks expected. User needs to call phpalCt_Transceive_SplitChaining() with eOption
 *                            "E_PHPAL_CT_RX_CHAINING"
 *                          - Any other Status : Error Occured. Check error code for more details.
 *
 *
 * @param[in] pbTransmitBuff  Pointer to the transmit buffer
 * @param[in] dwTransmitSize  Size of the bytes to be transmitted
 * @param[out] pbReceiveBuff  Pointer to the receive buffer
 * @param[out] pwReceiveSize  Pointer to the receive buffer size
 * @param [in] eOption  Enum Constant indicating the expected behaviour of the Tranceive operation.

 * @retval #PH_ERR_SUCCESS  The Apdu transmitted successfully and received the response successfully.
 * @retval #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 *
 * @maskedret    #PH_ERR_CT_RETRY_COUNT_FAILURE    If Retry count has been exceeded than allowable value.
 * @maskedret    #PH_ERR_CT_INS_COMMAND_ERROR    Improper INS is received from card.
 * @maskedret    #PH_ERR_CT_RESYNCH_SUCCESS    If Resynchronisation happened for 7816 profile.
 * @maskedret    #PH_ERR_CT_CHAINING_ABORT_REQUESTED    If Chaining Abortion request is received from card
 * @maskedret    #PH_ERR_CT_EXT_APDU_NOT_SUPPORTED    If Extended command Apdu is passed for T0 protocol.
 * @maskedret    #PH_ERR_CT_PAL_SUCCESS_RX_CHAINING    RX Chaining ongoing. Call phpalCt_Transceive_SplitChaining() with
 *                                                     eOption "E_PHPAL_CT_RX_CHAINING" to receive next block.
 * @maskedret    #PH_ERR_CT_PAL_SUCCESS_TX_CHAINING    TX Chaining Ongoing. Call phpalCt_Transceive_SplitChaining() with
 *                                                     eOption "E_PHPAL_CT_TX_CHAINING" to send next block.
 */
phStatus_t phpalCt_Transceive_SplitChaining(uint8_t* pbTransmitBuff, uint32_t dwTransmitSize,
                                            uint8_t* pbReceiveBuff,  uint16_t* pwReceiveSize,
                                            phpalCt_TransceiveOption_t eOption
                                            );

/**
 * This function is used to deinitialize the CT IP and disable the NVIC for contact interface
 * @retval       #PH_ERR_SUCCESS  Deinit is done successfully
 * @retval       #PH_ERR_FAILED   Deinit is failed
 */
phStatus_t phpalCt_DeInit(void);
#endif /* NXPBUILD__PHPAL_CT */
/** @}.*/
#endif /* PHPAL_CT_H */
