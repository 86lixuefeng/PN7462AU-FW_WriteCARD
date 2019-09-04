/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Status Code definitions for Reader Library Framework.
* $Author: Deepa Mer (NXP67695) $
* $Revision: 18573 $ (v04.13.00)
* $Date: 2016-08-02 15:04:03 +0530 (Tue, 02 Aug 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PH_STATUS_H
#define PH_STATUS_H

#ifndef NXPBUILD_DELETE
#include <ph_NxpBuild.h>
#endif /* NXPBUILD_DELETE */


#include <ph_Datatypes.h>

/**
 * Default 16bit status code
 *
 * - High byte: Category (group) Identifier.
 * - Low byte : Error Specifier.
 *
 * See @ref ph_Error for the list of values
 */
typedef uint16_t phStatus_t;

#ifdef _WIN32
#pragma warning(push)           /* PRQA S 3116 */
#pragma warning(disable:4001)   /* PRQA S 3116 */
#include <string.h>
#pragma warning(pop)            /* PRQA S 3116 */
#else
#include <string.h>
#endif

#ifdef NXPBUILD__PH_LOG
#include <phLog.h>
#endif

/**
 *
 * \addtogroup grp_Success_Err_Code
 *
 * Success Conditions
 *
 * @{
 */
#define PH_ERR_SUCCESS                  0x0000U /**< Returned in case of no error. */
#define PH_ERR_SUCCESS_CHAINING         0x0071U /**< Rx chaining is not complete, further action needed. */
#define PH_ERR_SUCCESS_ACK              0x0072U /**< Received ACK packet for a previous transmitted packet. */
#define PH_ERR_SUCCESS_INCOMPLETE_BYTE  0x0073U /**< An incomplete byte was received. */
#define PH_ERR_SUCCESS_DESELECTED       0x0074U /**< DSL is sent for de-selection of target. */
#define PH_ERR_SUCCESS_RELEASED         0x0075U /**< RLS is sent for release of target. */
#define PH_ERR_SUCCESS_INFO_RECEIVED    0x0076U /**< Received I PDU in response for an I PDU. */
/** @} */

/** \Osal Errors
*/
/*@{*/
#define PH_ERR_OSAL_ERROR               0x0027U /**< Error occurred during initialization. */
#define PH_ERR_OSAL_FAILURE             0x0028U /**< OSAL failed to perform the requested operation. */
/*@}*/

/** \Event Errors
*/
/*@{*/
#define PH_ERR_EVENT_ERROR               0x0029U /**< Error occurred during initialization. */
#define PH_ERR_EVENT_FAILURE             0x0030U /**< Event failed to perform the requested operation. */


/**
 *
 * \addtogroup grp_Comm_Err_Code
 *
 * Communication related error codes
 *
 * @{
 *
 */
#define PH_ERR_IO_TIMEOUT               0x0001U /**< No reply received, e.g. PICC removal. */
#define PH_ERR_INTEGRITY_ERROR          0x0002U /**< Wrong CRC or parity detected. */
#define PH_ERR_COLLISION_ERROR          0x0003U /**< A collision occurred. */
#define PH_ERR_BUFFER_OVERFLOW          0x0004U /**< Attempt to write beyond buffer size. */
#define PH_ERR_FRAMING_ERROR            0x0005U /**< Invalid frame format. */
#define PH_ERR_PROTOCOL_ERROR           0x0006U /**< Received response violates protocol. */
#define PH_ERR_AUTH_ERROR               0x0007U /**< Authentication error. */
#define PH_ERR_READ_WRITE_ERROR         0x0008U /**< A Read or Write error occurred in RAM/ROM or Flash. */
#define PH_ERR_TEMPERATURE_ERROR        0x0009U /**< The RC sensors signal over heating. */
#define PH_ERR_RF_ERROR                 0x000AU /**< Error due to RF. */
#define PH_ERR_INTERFACE_ERROR          0x000BU /**< An error occurred in RC communication. */
#define PH_ERR_LENGTH_ERROR             0x000CU /**< A length error occurred. */
#define PH_ERR_RESOURCE_ERROR           0x000DU /**< An resource error. */
#define PH_ERR_TX_NAK_ERROR             0x000EU /**< TX Rejected sanely by the counterpart. */
#define PH_ERR_RX_NAK_ERROR             0x000FU /**< RX request Rejected sanely by the counterpart. */
#define PH_ERR_EXT_RF_ERROR             0x0010U /**< Error due to External RF. */
#define PH_ERR_NOISE_ERROR              0x0011U /**< EMVCo EMD Noise Error. */
#define PH_ERR_ABORTED                  0x0012U /**< Used when HAL ShutDown is called. */
#define PH_ERR_INTERNAL_ERROR           0x007FU /**< An internal error occurred. */
/** @} */

/**
 *
 * \addtogroup grp_Generic_Err_Code
 *
 * @{
 */

/**  \name Parameter & Command Errors */
#define PH_ERR_INVALID_DATA_PARAMS      0x0020U /**< Invalid data parameters supplied (layer id check failed). */
#define PH_ERR_INVALID_PARAMETER        0x0021U /**< Invalid parameter supplied. */
#define PH_ERR_PARAMETER_OVERFLOW       0x0022U /**< Reading/Writing a parameter would produce an overflow. */
#define PH_ERR_UNSUPPORTED_PARAMETER    0x0023U /**< Parameter not supported. */
#define PH_ERR_UNSUPPORTED_COMMAND      0x0024U /**< Command not supported. */
#define PH_ERR_USE_CONDITION            0x0025U /**< Condition of use not satisfied. */
#define PH_ERR_KEY                      0x0026U /**< A key error occurred. */
#define PH_ERR_OSAL_ERROR               0x0027U /**< Error occurred during initialization. */

#define PH_ERR_OPERATION_TIMEDOUT       0x0028U /**< General Error: Ongoing operation timed out */

/** This Layer is Not initialized, hence initialization required. */
#define PH_ERR_NOT_INITIALISED          0x0031U

#define PH_ERR_MEMORY_ERROR             0x0040U /**< Unsuccessful Read/Write operation */

/** The Un-registration command has failed because the user wants to unregister on
    an element for which he was not registered */
#define PH_ERR_NOT_REGISTERED           0x0034U

#define PH_ERR_FAILED                   0x00FFU /**< Generic Failure */


/**
 * \name Running Modes of Reader Library
 */
/** @{*/
#define RD_LIB_MODE_EMVCO               0x01U /**< EMVCo Mode. */
#define RD_LIB_MODE_NFC                 0x02U /**< NFC Forum Mode. */
#define RD_LIB_MODE_ISO                 0x03U /**< ISO Mode. */
/** @}*/

/**
* \name Boolean Values
*/
/** @{*/
#define PH_ON                           0x0001U /**< Enabled. */
#define PH_OFF                          0x0000U /**< Disabled. */
#define PH_SUPPORTED                    0x0001U /**< Supported. */
#define PH_UNSUPPORTED                  0x0000U /**< Not Supported. */
/** @}*/

/**
* \name Configuration
*/
/** @{*/
#define PH_CONFIG_CUSTOM_BEGIN          0xE000U /**< Starting number of custom configs. */
/** @}*/

/** @} */ //  ends

/**
 * \addtogroup phCompCodes
 *
 * @{
 *
 */
#define PH_COMP_MASK                    0xFF00U /**< Component Mask for status code and component ID. */
#define PH_COMPID_MASK                  0x00FFU /**< ID Mask for component ID. */
#define PH_ERR_MASK                     0x00FFU /**< Error Mask for status code. */
#define PH_ERR_CUSTOM_BEGIN             0x0080U /**< Starting number of custom error codes. */
#define PH_COMP_GENERIC                 0x0000U /**< Generic Component Code. */
#define PH_COMP_BAL                     0x0100U /**< BAL Component Code. */
#define PH_COMP_HAL                     0x0200U /**< HAL Component Code. */
#define PH_COMP_PAL_ISO14443P3A         0x0300U /**< ISO14443-3A PAL-Component Code. */
#define PH_COMP_PAL_ISO14443P3B         0x0400U /**< ISO14443-3B PAL-Component Code. */
#define PH_COMP_PAL_ISO14443P4A         0x0500U /**< ISO14443-4A PAL-Component Code. */
#define PH_COMP_PAL_ISO14443P4          0x0600U /**< ISO14443-4 PAL-Component Code. */
#define PH_COMP_PAL_MIFARE              0x0700U /**< MIFARE(R) PAL-Component Code. */
#define PH_COMP_PAL_FELICA              0x0800U /**< Open FeliCa PAL-Component Code. */
#define PH_COMP_PAL_EPCUID              0x0900U /**< ICode EPC/UID PAL-Component Code. */
#define PH_COMP_PAL_SLI15693            0x0A00U /**< ICode SLI/ISO15693 PAL-Component Code. */
#define PH_COMP_PAL_I18000P3M3          0x0B00U /**< ISO18000-3 Mode3 PAL-Component Code. */
#define PH_COMP_PAL_I18092MPI           0x0C00U /**< ISO18092 passive initiator mode PAL-Component Code. */
#define PH_COMP_PAL_I18092MT            0x0D00U /**< ISO18092 target mode PAL-Component Code. */
#define PH_COMP_PAL_GENERALTARGET       0x0E00U /**< General Target/Listen mode protocol. */
#define PH_COMP_PAL_I14443P4MC          0x0F00U /**< ISO 14443-4 Card mode protocol. */
#define PH_COMP_AL_MFC                  0x1000U /**< MIFARE(R) Classic AL-Component Code. */
#define PH_COMP_AL_MFUL                 0x1100U /**< MIFARE(R) Ultralight AL-Component Code. */
#define PH_COMP_AL_MFP                  0x1200U /**< MIFARE(R) Plus AL-Component Code. */
#define PH_COMP_AL_VCA                  0x1300U /**< Virtual Card Architecture AL-Component Code. */
#define PH_COMP_AL_FELICA               0x1400U /**< Open FeliCa AL-Component Code. */
#define PH_COMP_AL_ICODE                0x1500U /**< ICode AL-Component Code. */
#define PH_COMP_AL_I18000P3M3           0x1800U /**< ISO18000-3 Mode3 AL-Component Code. */
#define PH_COMP_AL_MFDF                 0x1900U /**< MIFARE DESFIRE EV1 AL Component Code. */
#define PH_COMP_AL_MFDFEV2              0x2000U /**< MIFARE DESFIRE EV2 AL Component Code. */
#define PH_COMP_AL_P40CMDPRIV           0x1A00U /**< P40 command libraryAL-Component Code. */
#define PH_COMP_AL_P40CMDPUB            0x1B00U /**< P40 command libraryAL-Component Code. */
#define PH_COMP_AL_T1T                  0x1C00U /**< T1T AL component code  */
#define PH_COMP_AL_TOP                  0x1D00U /**< Tag Operation AL component code  */
#define PH_COMP_DL_AMP                  0x3000U /**< Amplifier DL-Component Code. */
#define PH_COMP_DL_THSTRM               0x3100U /**< Thermostream DL-Component Code. */
#define PH_COMP_DL_OSCI                 0x3200U /**< Oscilloscope DL-Component Code. */
#define PH_COMP_DL_RDFPGA               0x3300U    /**< Reader FPGA Box DL-Component Code. */
#define PH_COMP_DL_MSTAMPOSC            0x3400U /**< Master Amplifier Oscilloscope DL-Component Code. */
#define PH_COMP_DL_STEPPER              0x3500U /**< Stepper DL-Component Code. */
#define PH_COMP_AC_DISCLOOP             0x4000U /**< Discovery Loop. */
#define PH_COMP_CE_T4T                  0x5000U /**< Card Emulation T4T. */
#define PH_COMP_LN_LLCP                 0x6000U /**< LLCP code. */
#define PH_COMP_NP_SNEP                 0x7000U /**< SNEP Code. */
#define PH_COMP_CIDMANAGER              0xE000U /**< Cid Manager Component Code. */
#define PH_COMP_CRYPTOSYM               0xE100U /**< CryptoSym Component Code. */
#define PH_COMP_KEYSTORE                0xE200U /**< KeyStore Component Code. */
#define PH_COMP_TOOLS                   0xE300U /**< Tools Component Code. */
#define PH_COMP_CRYPTORNG               0xE400U /**< CryptoRng Component Code. */
#define PH_COMP_LOG                     0xEF00U /**< Log Component Code. */
#define PH_COMP_OSAL                    0xF000U /**< OS AL component code. */
#define PH_COMP_PLATFORM                0xF100U /**< MicroController Platform component code. */
#define PH_COMP_DRIVER                  0xF100U /**< DRIVER component code. */
#define PH_COMP_EVENT                   0xF200U /**< Event component code. */
/** @} */

/** \addtogroup phCompCodesMASK
 *
 * Error Code Macros
 *
 * @{
 */
/**
 * Macro for error handling of underlying layer and exit.
 */


/** Checks whether the Status code is success if not performs function return with error code.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 */
#define PH_CHECK_SUCCESS(STATUS)         {if ((STATUS) != PH_ERR_SUCCESS) {return (STATUS);}}

/** Checks whether the Status code is success if not breaks out of the loop.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 */
#define PH_CHECK_SUCCESS_BREAK(STATUS)         {if ((STATUS) != PH_ERR_SUCCESS) {break;}}

/** Calls the specified function and checks the status, performs function return if the status is not PH_ERR_SUCCESS.
 * @param[in] STATUS Status variable to be checked against PH_ERR_SUCCESS after calling the specified function FUNC.
 * @param[in] FUNC Function to be called and check the status.
 */
#define PH_CHECK_SUCCESS_FCT(STATUS,FUNC) {(STATUS) = (FUNC); PH_CHECK_SUCCESS(STATUS);}

/** Calls the specified function and checks the status, breaks out of the loop if the status is not PH_ERR_SUCCESS.
 * @param[in] STATUS Status variable to be checked against PH_ERR_SUCCESS after calling the specified function FUNC.
 * @param[in] FUNC Function to be called and check the status.
 */
#define PH_CHECK_SUCCESS_FCT_BREAK(STATUS,FUNC) \
		{\
			(STATUS) = (FUNC); \
			PH_CHECK_SUCCESS_BREAK(STATUS);}

#define PH_CHECK_ABORT(status)         {if ((status & PH_ERR_MASK) == PH_ERR_ABORTED) {return (status);}}
#define PH_CHECK_ABORT_FCT(status,fct) {(status) = (fct); PH_CHECK_ABORT(status);}


/** Checks whether the Status code is success if not performs function return with error code.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 */
#define PH_RETURN_ON_FAILURE(STATUS)         {if ((STATUS) != PH_ERR_SUCCESS) {return (STATUS);}}

/** Checks whether the Status code is success if not breaks out of the loop.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 */
#define PH_BREAK_ON_FAILURE(STATUS)         {if ((STATUS) != PH_ERR_SUCCESS) {break;}}

/** Checks whether the Status code is success, if yes then performs function return with SUCCESS code.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 */
#define PH_RETURN_ON_SUCCESS(STATUS)         {if ((STATUS) == PH_ERR_SUCCESS) {return (STATUS);}}

/** Checks whether the Status code is success, if yes then breaks out of the loop.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 */
#define PH_BREAK_ON_SUCCESS(STATUS)         {if ((STATUS) == PH_ERR_SUCCESS) {break;}}

/**
 * Checks whether the status code is success, if yes, then breaks out of the loop with status as ERROR.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 * @param[in] ERROR Error to be updated to STATUS.
 */
#define PH_BREAK_ON_SUCCESS_WITH_ERROR(STATUS, ERROR)           \
    if ((STATUS) == PH_ERR_SUCCESS)                             \
    {                                                           \
        (STATUS) = PH_ERR_ ## ERROR;                            \
        break;                                                  \
    }

/**
 * Checks whether the status code is success, if not, then breaks out of the loop with status as ERROR.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 * @param[in] ERROR Error to be updated to STATUS.
 */
#define PH_BREAK_ON_FAILURE_WITH_ERROR(STATUS, ERROR)           \
    if ((STATUS) != PH_ERR_SUCCESS)                             \
    {                                                           \
        (STATUS) = PH_ERR_ ## ERROR;                            \
        break;                                                  \
    }

/**
 * Checks whether the status code is success, if yes, then returns from the function with ERROR.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 * @param[in] ERROR Error to be returned.
 * @param[in] COMP Component from which return is initiated.
 */
#define PH_RETURN_ON_SUCCESS_WITH_ERROR(STATUS, ERROR, COMP)    \
    if ((STATUS) == PH_ERR_SUCCESS)                               \
    {                                                           \
        return PH_ERR(ERROR, COMP);                             \
    }

/**
 * Checks whether the status code is success, if not, then returns from the function with ERROR.
 * @param[in] STATUS Status to be checked against PH_ERR_SUCCESS.
 * @param[in] ERROR Error to be returned.
 * @param[in] COMP Component from which return is initiated.
 */
#define PH_RETURN_ON_FAILURE_WITH_ERROR(STATUS, ERROR, COMP)    \
    if ((STATUS) != PH_ERR_SUCCESS)                             \
    {                                                           \
        return PH_ERR(ERROR, COMP);                             \
    }

/**
 * Checks whether the CONDITION is TRUE, if yes, then break from the loop with STATUS as ERROR.
 * @param[in] CONDITION Condition to be checked.
 * @param[in] STATUS Status to be updated.
 * @param[in] ERROR Error to be updated to STATUS.
 */
#define PH_BREAK_IF_TRUE_WITH_ERROR(CONDITION, STATUS, ERROR)   \
    if (CONDITION)                                              \
    {                                                           \
        (STATUS) = PH_ERR_ ## ERROR;                            \
        break;                                                  \
    }

/**
 * Checks whether the CONDITION is TRUE, if yes, then returns from the function with ERROR.
 * @param[in] CONDITION Condition to be checked.
 * @param[in] ERROR Error to be returned.
 * @param[in] COMP Component from which return is initiated.
 */
#define PH_RETURN_IF_TRUE_WITH_ERROR(CONDITION, ERROR, COMP)    \
    if (CONDITION)                                              \
    {                                                           \
        return PH_ERR(ERROR, COMP);                             \
    }

/**
 *  Macro for component-error-code adding of current layer.
 *
 * It is recommended to use this macro at Run Time and PH_ERR at compile time.
 */
#define PH_ADD_COMPCODE(stat, code)                                             \
    ((phStatus_t)(((stat) == PH_ERR_SUCCESS) || ((stat) & PH_COMP_MASK)         \
     ? (stat) : (((stat) & PH_ERR_MASK) | ((code) & PH_COMP_MASK))))


/** Returns Status code masked with Component Code
 * @param[in] ERR Error code part of @ref ph_Error
 * @param[in] COMP Component code part of @ref phCompCodes
 */
#define PH_ERR(ERR,COMP)                                                \
     (((PH_COMP_ ## COMP)) | ((PH_ERR_ ## ERR) & (PH_ERR_MASK )))
/** @}*/

/**
*  Macro for component-error-code adding of current layer.
*/
#define PH_ADD_COMPCODE(stat, code)     ((phStatus_t)(((stat) == PH_ERR_SUCCESS) || ((stat) & PH_COMP_MASK) ? (stat) : (((stat) & PH_ERR_MASK) | ((code) & PH_COMP_MASK))))

/** Return status for Values known at compile time. For variable status, use \ref PH_ADD_COMPCODE */
#define PH_ADD_COMPCODE_FIXED(stat,code) ((phStatus_t)((stat) & PH_ERR_MASK) | ((code) & PH_COMP_MASK))

/**
 *  Macro to get the component code or id using a pointer to a memory.
 *
 *  \param pId pointer to DataParams
 *
 *  \see phCompCodes
 */
#define PH_GET_COMPCODE(pId)            ( (*((uint16_t*)(pId))) & PH_COMP_MASK )
/** Get the component ID
 *
 * e.g. SW Implementation Layer or HW Layer
 *
 * \param pId Pointer to the DataParams
 *
 */
#define PH_GET_COMPID(pId)              ( (uint8_t)(*((uint16_t*)(pId))) )


/** @}*/

/**
 *
 * \addtogroup ph_Status
 *
 * @{
 */

/** \name Exchange Options
*/
/*@{*/
/**
Default exchange mode.
Combine with #PH_EXCHANGE_BUFFERED_BIT and
#PH_EXCHANGE_LEAVE_BUFFER_BIT to perform buffering.
\c ISO14443-4/ISO18092 specific:
Performs Tx/Rx chaining with the card.
Returns #PH_ERR_SUCCESS_CHAINING when RxBuffer is full and
does not ACK the last received block.
*/
#define PH_EXCHANGE_DEFAULT             0x0000U

/**
\c ISO14443-4/ISO18092 specific:
Chains given data to card/Target.
Combine with #PH_EXCHANGE_BUFFERED_BIT and
#PH_EXCHANGE_LEAVE_BUFFER_BIT to perform buffering.
Does not receive any data.
*/
#define PH_EXCHANGE_TXCHAINING          0x0001U

/**
\c ISO14443-4/ISO18092 specific:
Starts transmission with and R(ACK) block and
performs Rx chaining with the Card/Target.
Returns #PH_ERR_SUCCESS_CHAINING when RxBuffer is full and
does not ACK the last received block.
*/
#define PH_EXCHANGE_RXCHAINING          0x0002U

/**
\c ISO14443-4/ISO18092 specific:
Starts transmission with and R(ACK) block and
performs Rx chaining with the card.
Completes Rx chaining with the card if RxBuffer is full.
*/
#define PH_EXCHANGE_RXCHAINING_BUFSIZE  0x0003U

/** Mask for the above modes. All other bits are reserved. */
#define PH_EXCHANGE_MODE_MASK           0x000FU

/** Mask for custom exchange bits. These 4 bits can be used for custom exchange behavior. */
#define PH_EXCHANGE_CUSTOM_BITS_MASK    0x00F0U

/**
Does not clear the internal buffer before operation.
If this bit is set and data is transmitted,
the contents of the internal buffer are sent first.
*/
#define PH_EXCHANGE_LEAVE_BUFFER_BIT    0x4000U

/**
Buffers Tx-Data into internal buffer
instead of transmitting it.
*/
#define PH_EXCHANGE_BUFFERED_BIT        0x8000U

/**
The given data is the first part of a stream.
(Helper definition for buffering).
*/
#define PH_EXCHANGE_BUFFER_FIRST   (PH_EXCHANGE_DEFAULT | PH_EXCHANGE_BUFFERED_BIT)

/**
The given data is a subsequent part of a stream.
(Helper definition for buffering).
*/
#define PH_EXCHANGE_BUFFER_CONT    (PH_EXCHANGE_DEFAULT | PH_EXCHANGE_BUFFERED_BIT | PH_EXCHANGE_LEAVE_BUFFER_BIT)

/**
The given data is the last part of a stream.
(Helper definition for buffering).
*/
#define PH_EXCHANGE_BUFFER_LAST    (PH_EXCHANGE_DEFAULT | PH_EXCHANGE_LEAVE_BUFFER_BIT)

/**
Default transmit mode.
Combine with #PH_TRANSMIT_BUFFERED_BIT and
#PH_TRANSMIT_LEAVE_BUFFER_BIT to perform buffering.
*/
#define PH_TRANSMIT_DEFAULT             0x0000U

/**
Perform Transmit Chaining.
Combine with #PH_TRANSMIT_BUFFERED_BIT and
#PH_TRANSMIT_LEAVE_BUFFER_BIT to perform buffering.
*/
#define PH_TRANSMIT_TXCHAINING          0x0001U

/** Mask for the above modes. All other bits are reserved. */
#define PH_TRANSMIT_MODE_MASK           0x000FU

/**
Does not clear the internal buffer before transmit operation.
If this bit is set and data is transmitted,
the contents of the internal buffer are sent first.
*/
#define PH_TRANSMIT_LEAVE_BUFFER_BIT    0x4000U

/**
Buffers Tx-Data into internal buffer
instead of transmitting it.
*/
#define PH_TRANSMIT_BUFFERED_BIT        0x8000U

/**
The given data is the first part of a stream.
(Helper definition for buffering).
*/
#define PH_TRANSMIT_BUFFER_FIRST        (PH_TRANSMIT_DEFAULT | PH_TRANSMIT_BUFFERED_BIT)

/**
The given data is a subsequent part of a stream.
(Helper definition for buffering).
*/
#define PH_TRANSMIT_BUFFER_CONT         (PH_TRANSMIT_DEFAULT | PH_TRANSMIT_BUFFERED_BIT | PH_TRANSMIT_LEAVE_BUFFER_BIT)

/**
The given data is the last part of a stream.
(Helper definition for buffering).
*/
#define PH_TRANSMIT_BUFFER_LAST         (PH_TRANSMIT_DEFAULT | PH_TRANSMIT_LEAVE_BUFFER_BIT)

/**
Perform Re-transmit of data stored in HAL Tx buffer.
Should not be combined with #PH_TRANSMIT_BUFFERED_BIT and #PH_TRANSMIT_LEAVE_BUFFER_BIT.
This option should be used only in target / PICC mode.
*/
#define PH_TRANSMIT_PREV_FRAME          0x1000U

/**
Default exchange mode.
\c ISO14443-4/ISO18092 specific:
Performs Rx chaining with the card.
Returns #PH_ERR_SUCCESS_CHAINING when RxBuffer is full and
does not ACK the last received block.
*/
#define PH_RECEIVE_DEFAULT              0x0000U

/**
\c ISO14443-4/ISO18092 specific:
Starts transmission of ACK block and
performs Rx chaining with the card.
Returns #PH_ERR_SUCCESS_CHAINING when RxBuffer is full and
does not ACK the last received block.
*/
#define PH_RECEIVE_CHAINING             0x0001U

/**
\c ISO14443-4/ISO18092 specific:
Starts transmission with ACK block and
performs Rx chaining with the card.
Completes Rx chaining with the card if RxBuffer is full.
*/
#define PH_RECEIVE_CHAINING_BUFSIZE     0x0002U
/*@}*/

/** \name Log Interface
*/
/*@{*/
#ifdef NXPBUILD__PH_LOG

#define PH_LOG_HELPER_ALLOCATE_PARAMNAME(pParam) PH_LOG_ALLOCATE_PARAMNAME(pParam)
#define PH_LOG_HELPER_ALLOCATE_TEXT(pIdent, pText) PH_LOG_ALLOCATE_TEXT(pIdent, pText)
#define PH_LOG_HELPER_ADDSTRING(bLogType,pString) phLog_AddString(pDataParams, (bLogType), (pString))
#define PH_LOG_HELPER_ADDPARAM_UINT8(bLogType,pName,pParam) phLog_AddParam_Uint8(pDataParams, (bLogType), (pName), (pParam))
#define PH_LOG_HELPER_ADDPARAM_UINT16(bLogType,pName,pParam) phLog_AddParam_Uint16(pDataParams, (bLogType), (pName), (pParam))
#define PH_LOG_HELPER_ADDPARAM_UINT32(bLogType,pName,pParam) phLog_AddParam_Uint32(pDataParams, (bLogType), (pName), (pParam))
#define PH_LOG_HELPER_ADDPARAM_BUFFER(bLogType,pName,pParam,wLength) phLog_AddParam_Buffer(pDataParams, (bLogType),(pName),(pParam),(wLength))
#define PH_LOG_HELPER_EXECUTE(bOption) phLog_Execute(pDataParams, (bOption))

#else

#define PH_LOG_HELPER_ALLOCATE_PARAMNAME(pParam)
#define PH_LOG_HELPER_ALLOCATE_TEXT(pIdent, pText)
#define PH_LOG_HELPER_ADDSTRING(bLogType,pString)
#define PH_LOG_HELPER_ADDPARAM_UINT8(bLogType,pName,pParam)
#define PH_LOG_HELPER_ADDPARAM_UINT16(bLogType,pName,pParam)
#define PH_LOG_HELPER_ADDPARAM_UINT32(bLogType,pName,pParam)
#define PH_LOG_HELPER_ADDPARAM_BUFFER(bLogType,pName,pParam,wLength)
#define PH_LOG_HELPER_EXECUTE(bOption)

#endif
/*@}*/

/** @} */
/* end of ph_Status group */

/** \name Debug Definitions
*/
/*@{*/
#ifdef NXPBUILD__PH_DEBUG
#define PH_ASSERT_NULL(a) while((a) == NULL);
#   define PH_ASSERT_NULL_PARAM(a,comp_code) {if((a) == NULL) {return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, (comp_code));}}
#   define PH_ASSERT_NULL_DATA_PARAM(dataparams,comp_code) {if((dataparams) == NULL) {return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, (comp_code));}}
#else
#define PH_ASSERT_NULL(a)
#   define PH_ASSERT_NULL_PARAM(a,comp_code) {if((a) == NULL) {return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, (comp_code));}}
#   define PH_ASSERT_NULL_DATA_PARAM(dataparams,comp_code) {if((dataparams) == NULL) {return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, (comp_code));}}
#endif /* NXPBUILD__PH_DEBUG */
/*@}*/

/** Explicity denote that this variable is not used intetionally and avoid compiler warning */
#define PH_UNUSED_VARIABLE(x) ((void)x)



/**
 * \addtogroup phCompCodesIC
 *
 *
 * @note For components not Part of this list, use @ref PH_COMP_HAL
 *       as the component code.
 *
 * @{
 */

#define PH_COMP_RF                      0x4F00U /**< For'R' 'F' ==> RF Component Code (Low Layer Clif Hal.) */
#define PH_COMP_CRC                     PH_COMP_HAL /**< Cyclic Redunduncy Check */
#define PH_COMP_HIF                     0x8100 /**< Host '1'nterface HAL */
#define PH_COMP_I2CM                    0x8200 /**< I'2'C Master HAL */
#define PH_COMP_SPIM                    0x8300 /**< SPI Master HAL */
#define PH_COMP_RNG                     0x8400 /**< Random Number Generator HAL. */
#define PH_COMP_USB                     0x8500 /**< U5B HAL */
#define PH_COMP_PCR                     0x8600 /**< Power Control and Reset HAL*/
#define PH_COMP_PMU                     0x8700 /**< Power Management Unit HAL */
#define PH_COMP_TIMER                   0x8800 /**< General Purpose Timers */
#define PH_COMP_CLKGEN                  0x8900 /**< General Purpose Timers */
#define PH_COMP_BOOT                    0x8B00 /**< 'B'oot Flow HAL */
#define PH_COMP_SYSSVR                  0x8C00 /**< System Servi'C'es */
#define PH_COMP_E2PF                    0x8E00 /**< 'E'2Prom and Page Flash Read/Write HAL */
#define PH_COMP_HAL_CT                  0xC000 /**< CT HAL */
#define PH_COMP_PAL_CT                  0xCA00 /**< CT P'A'L */
#define PH_COMP_HAL_TDA                 0xCD00 /**< T'D'A PAL */
#define PH_COMP_HAL_SYNC_CT             0xCE00 /**< CT HAL */
/** @} */



#endif /* PH_STATUS_H */
