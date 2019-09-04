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
.*/

/** @file
 *
 * The Contact Interface (HAL) APIs
 *
 * \addtogroup phhalCt
 *
 * The Contact Interface (HAL) APIs
 *
 * The purpose and scope of this file is to implement CT HAL, Prototype of the CT Interface HAL
 * This module implements contact interface Api's in flash memory.
 * This module designed and can be extended to multi-process environment.
 * This module contains only the hardware abstraction Apis.
 * The user of this module can use these Apis for contact protocol(T=0 or T=1) development.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input
 *                  parameters will not be checked for erroneous values in the HAL API. In such a build configuration
 *                  use the API with caution and pass valid parameters only. Invalid parameters will likely result in
 *                  hardfaults or other failures.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-29 10:37:36 +0530 (Fri, 29 Jan 2016) $
 * $Author: Shikha Khandelwal (nxp82684) $
 * $Revision: 17616 $ (v04.13.00)
 *
 * @{
 *
 * \dot
 *
 *      digraph "Typical Usage" {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalCt_CardActivate [ URL="\ref phhalCt_CardActivate"];
 *          phhalCt_CardDeactivate [ URL="\ref phhalCt_CardDeactivate"];
 *          phhalCt_DeInit [ URL="\ref phhalCt_DeInit"];
 *          phhalCt_Init [ URL="\ref phhalCt_Init"];
 *          phhalCt_Receive [ URL="\ref phhalCt_Receive"];
 *          phhalCt_SetTimer [ URL="\ref phhalCt_SetTimer"];
 *          phhalCt_Transmit [ URL="\ref phhalCt_Transmit"];
 *          PHHAL_CT_STOP_TIMER [ URL="\ref PHHAL_CT_STOP_TIMER"];
 *      //Logical Flow
 *          Start ->phhalCt_Init -> phhalCt_CardActivate;
 *          phhalCt_CardActivate -> phhalCt_SetTimer;
 *          phhalCt_SetTimer -> phhalCt_Transmit;
 *          phhalCt_Transmit -> phhalCt_Receive;
 *          phhalCt_Receive -> phhalCt_Transmit;
 *          phhalCt_Receive -> PHHAL_CT_STOP_TIMER;
 *          PHHAL_CT_STOP_TIMER -> phhalCt_CardDeactivate;
 *          phhalCt_CardDeactivate -> phhalCt_DeInit;
 *          phhalCt_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 *
 */

#ifndef PHHAL_CT_H
#define PHHAL_CT_H

/* *****************************************************************************************************************
 * Includes
 * *****************************************************************************************************************.*/
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "ph_Status.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "PN7462AU_ROM3.h"
#include "ph_Reg.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * *****************************************************************************************************************.*/

/** Macros for 1.8V  VCC selection during ATR reception.*/
#define PHHAL_CT_VCC1M8                   0x02

/** Macros for 3V  VCC selection during ATR reception.*/
#define PHHAL_CT_VCC3                     0x01

/** Macros for 5V  VCC selection during ATR reception.*/
#define PHHAL_CT_VCC5                     0x00

/**
 * Set the timer in maximum allowed time in ATR between each character.
 */
#define PHHAL_CT_ATRMODE_MAXTIME           1

/** Set the timer in BWT mode configuration.*/
#define PHHAL_CT_APDUMODE_BWT              2

/** Set the timer in WWT mode configuration.*/
#define PHHAL_CT_APDUMODE_WWT              3

/** Set the timer in PPS Exchange mode configuration.*/
#define PHHAL_CT_PPSMODE_9600_ETU          4

/** Set the timer in the software triggered mode */
#define PHHAL_CT_SW_TRIGGER                 5

/** Macro to stop the timers, the timers will be stopped irrespective of any configuration and count.*/
#define PHHAL_CT_STOP_TIMER           PH_REG_SET(CT_TOC_REG, 0x00)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * *****************************************************************************************************************.*/
/**
 * This enum is used select the slot (Main slot or auxillary slot)
*/
typedef enum {
    /** \b 0x0000 */    E_MAIN_SLOT = 0x00,    /**< Selection for main slot of the CT interface */
    /** \b 0x0001 */    E_AUXILIARY_SLOT       /**< Selection for auxiliary slot of the CT interface with TDA */
}phhalCt_SlotType_t;


/** \addtogroup grpErr_HalCt
 *
 * Error codes for CT HAL
 *
 * @{
 *
 * CT HAL Interface Error Code ENUMs
 */
/**
 * These enums are used by Hal Apis for indicating different error status codes.
 */
typedef enum {
    /** \b 0x81 */ PH_ERR_CT_MUTE_ERROR  = (PH_ERR_CUSTOM_BEGIN+0x0001),    /**< Card is muted.*/
    /** \b 0x82 */ PH_ERR_CT_EARLY_ERROR,           /**< Card is too early to answer.*/
    /** \b 0x83 */ PH_ERR_CT_PARITY_ERROR,          /**< Card has parity errors.*/
    /** \b 0x84 */ PH_ERR_CT_OVERUN_ERROR,          /**< Fifo is overrun.*/
    /** \b 0x85 */ PH_ERR_CT_FRAMING_ERROR,         /**< Framing error.*/
    /** \b 0x86 */ PH_ERR_CT_TEMPERATURE_LATCHED,   /**< Temperature latch error.*/
    /** \b 0x87 */ PH_ERR_CT_CARD_REMOVED,          /**< Card removed.*/
    /** \b 0x88 */ PH_ERR_CT_PROTECTION_LATCHED,    /**< Protection is latched.*/
    /** \b 0x89 */ PH_ERR_CT_TIME_OUT_ATR_20160ETU, /**< ATR long timeout 20160etu error .*/
    /** \b 0x8A */ PH_ERR_CT_TIME_OUT_ATR_10080ETU, /**< ATR char waiting timeout 10080etu error.*/
    /** \b 0x8B */ PH_ERR_CT_TIME_OUT_WWT_OR_BWT,   /**< BWT or WWT timeout error.*/
    /** \b 0x8C */ PH_ERR_CT_TIME_OUT_CWT,          /**< CWT timeout error.*/
    /** \b 0x8D */ PH_ERR_CT_CARD_ALREADY_ACTIVATED,/**< Card is already activated .*/
    /** \b 0x8E */ PH_ERR_CT_ATR_PARSER_ERROR,      /**< ATR parser error.*/
    /** \b 0x8F */ PH_ERR_CT_ATR_WARM_RESET_INDICATED,  /**< Warning from the ATR parser to issue the warm reset.*/
    /** \b 0x90 */ PH_ERR_CT_MAIN_CARD_PRESENT,     /**< Main slot card present.*/
    /** \b 0x91 */ PH_ERR_CT_MAIN_CARD_ABSENT,      /**< Main slot card absent.*/
    /** \b 0x92 */ PH_ERR_CT_PPS_EXCHANGE_ERROR,    /**< PPS Exchange is not successful.*/
    /** \b 0x93 */ PH_ERR_CT_CLASS_CHANGE_INDICATED, /**< Atr having class indicator came.*/
    /** \b 0x94 */ PH_ERR_CT_CLOCKSTOP_NOT_SUPPORTED, /**< Atr having not supported value for clock stop.*/
    /** \b 0x95 */ PH_ERR_CT_ATR_SPECIFIC_PARAMETER_UNSUPPORTED,     /**< Atr having Invalid Protocol for 7816 profile.*/
    /** \b 0x96 */ PH_ERR_PPS_EXCHANGE_NOT_REQUIRED, /**< PPS Exchange Api called with default or lower baudrate and first offered protocol.*/
    /** \b 0x97 */ PH_ERR_CT_ASYNCH_SHUTDOWN,        /**< Asynchronous shutdown of CT.*/
    /** \b 0x98 */ PH_ERR_CT_HAL_INVALID             /**< Invalid enumeration.*/
}phhalCt_ErrorCodes_t;
/** @} */

/**
 * This structure contains the necessary protocol parameters required by the Pal layer.
 */
typedef struct
{
    uint8_t bNegotiableMode; /**< Used for set negotiable mode in 7816 profile in absence of TA2.*/
    uint8_t bFlagT15TAValue; /**< Used for storing value of first TA after setting flag 15.*/
    uint8_t bCRCPresent;     /**< Used to set if CRC will be used for 7816 profile.*/
    uint8_t bIFSC;           /**< Stores Card's maximum information field size in a block.*/
/** In Negotiable mode stores the fidi value as it comes in TA1 byte of atr,
 *  In specific mode,In EMVCo or if TA1 is absent then  stores baudrate which is going
 * to be use for further.*/
    uint8_t bFiDi;
    uint8_t bProtSelT0;      /**< Used for setting if T=0 protocol will be supported.*/
    uint8_t bProtSelT1;      /**< Used for setting if T=1 protocol will be supported.*/
    uint8_t bWI;             /**< Used for setting WI value for WWT calculation for T0 protocol.*/
    uint8_t bBWI;            /**< Used for setting BWI value for BWT calculation for T1 protocol.*/
    uint8_t bCWI;            /**< Used for setting CWI value for CWT calculation for T1 protocol.*/
}phhalCt_ProtocolParams_t;

/**
 * This structure contains the necessary initialization parameters for CtHal.
 */
typedef struct phhalCt_InitParam_t
{
    uint8_t bPullupSetStatus;          /**< [in] If logic set to 1 Pull up else pull down config.*/
    uint8_t bCardPresConnectorStatus;  /**< [in] If Logic 1 Normally open else normally close config.*/
    uint8_t bAutoCTDeactivationEnable; /**< [in] Used to Specify if Auto deactivation is enable.*/
    uint8_t bSlewRateConfig;           /**< [in] To rise from 0 to its selected value (see ct_pcr_reg register),
                                            the time needed by VCC depends on the capacitance.*/
}phhalCt_InitParam_t; /**< CT Init params */

/**
 * This structure contains all the parameters which will be use or update in PPS Exchange.
 */
typedef struct
{
    uint8_t bProtSelT0;          /**< [out] Used for setting if T=0 protocol is supported in the end of PPS.*/
    uint8_t bProtSelT1;          /**< [out] Used for setting if T=1 protocol is supported in the end of PPS.*/
    uint8_t bNegotiableProtocol; /**< [in]  Used for setting protocol for negotiation.*/
    uint8_t bFiDi;               /**< [in/out] Used to set baud rate for PPS negotiation,also updates value as per after PPS exchange.*/
    uint8_t bWI;                 /**< [in] WI value for WWT calculation for T=0 protocol after PPS Exchange.*/
    uint8_t bBWI;                /**< [in] BWI value for BWT calculation for T=1 protocol after PPS Exchange.*/
    uint8_t bCWI;                /**< [in] CWI value for CWT calculation for T=1 protocol after PPS Exchange.*/
}phhalCt_PPSParamType_t;


/** Run time configurable parameters.  @sa phpalCt_SetConfig */
typedef enum
{
    E_EMV_ENABLE = 0              /**< [in] If Logic 1 EMVCo standard in use, else 7816.*/
} phhalCt_Configs_t;

/**
 * CT ISR Call back function signature.
 */
typedef void ( *pphhalCt_CallbackFunc_t )( uint32_t );

/* *****************************************************************************************************************
 * Extern Variables
 * *****************************************************************************************************************.*/

/* *****************************************************************************************************************
 * Function Prototypes
 * *****************************************************************************************************************.*/
/**
 * Typedef for a function pointer to CT HAL API phhalCt_SetTransmissionProtocol()
 */
typedef phStatus_t (phhalCt_SetTransmissionProtocol_t)(uint8_t bCardProtocol);

/**
 * The CT HAL Initialization shall perform soft reset of the CT HW, reset the CT HAL Context, configure the CT IP for
 * card presence switch type, PU/PD of card presence switch, configure CT specific PCR registers and enable CT
 * interrupt in NVIC.
 *
 * @param[in] psCtInitParams  Pointer to Structure which contains Ct initialization parameters.
 * @return Status for CT IP Initialization.
 *
 * @retval    #PH_ERR_SUCCESS  Initialization of CT IP is successful.
 * @maskedret #PH_ERR_FAILED   Initialization of CT IP failed.
 */
phStatus_t phhalCt_Init(phhalCt_InitParam_t *psCtInitParams);


/**
 * Set the CT hal configurations to the desired value.
 *
 * @param[in] eConfig CT HAl configurations.
 * @param[in] dwValue Desired value to be set the mentioned CT hal configuration.
 * @return Status for CT set configurations.
 *
 * @retval    #PH_ERR_SUCCESS    operation is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER   for invalid parameter.
 */
phStatus_t phhalCt_SetConfig(phhalCt_Configs_t eConfig, uint32_t dwValue);

/**
 * This function is used to switch the slot from main slot to auxiliary slot or vice versa.
 * @note   By default Main slot will be selected.
 * @param[in]  eSlot_Index    Slot number (Main slot or auxiliary slot).
 *                        - #E_MAIN_SLOT  for main slot selection.
 *                        - #E_AUXILIARY_SLOT  for auxiliary slot selection.
 *
 *
 *
 * @retval    #PH_ERR_SUCCESS The slot has been switched successfully.
 */
phStatus_t phhalCt_SwitchSlot(phhalCt_SlotType_t eSlot_Index);

/**
 * This function is used to check if card is present in the main slot or auxiliary slot.
 * @return Card Presence Status.
 *
 * @maskedret #PH_ERR_CT_MAIN_CARD_PRESENT  If Main slot is chosen and card is present.
 * @maskedret #PH_ERR_CT_MAIN_CARD_ABSENT   If Main slot is chosen and card is absent.
 */

phStatus_t phhalCt_CheckCardPres(void);

/**
 * This function is used to performs a cold activation.Configures the timers according to the EMVCo or ISO7816
 * depending on the selected profiles, selects the voltage class and starts the activation sequence.
 * If success, returns the ATR to the user.The Api also parses the ATR according to the EMVCo/ISO7816 specifications
 * and issues warm reset or de-activation of the card wherever required.
 *
 * @param[in]    bVccSel   Card Vcc Selection [#PHHAL_CT_VCC1M8 :1.8v,#PHHAL_CT_VCC3: 3v or #PHHAL_CT_VCC5: 5v]
 * @param[out]   pbAtrBuffer   Pointer to the ATR buffer where the ATR bytes will be stored.Atr Buffer should not be
 *                             less than 32 bytes for EMVCo and 33 bytes for 7816.
 * @param[out]   pbAtrSize   Pointer to which the count of ATR bytes is copied
 * @param[out]   psAtrPalParameters  Pointer to structure containing Pal's ATR parameter.
 * @return      Status code for card activation.
 *
 * @retval    #PH_ERR_SUCCESS  Cold activation successful and ATR bytes are received successfully
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT    Api timed out
 * @maskedret #PH_ERR_INVALID_PARAMETER     Parameters are invalid
 * @maskedret #PH_ERR_CT_MAIN_CARD_ABSENT   Card is absent in the main slot
 * @maskedret #PH_ERR_CT_CARD_ALREADY_ACTIVATED    Card is already activated
 * @maskedret #PH_ERR_CT_MUTE_ERROR                Card is muted
 * @maskedret #PH_ERR_CT_EARLY_ERROR               Card has answered early
 * @maskedret #PH_ERR_CT_PARITY_ERROR       Card has parity error while receiving the ATR
 * @maskedret #PH_ERR_CT_OVERUN_ERROR       Fifo is over run while receiving the ATR
 * @maskedret #PH_ERR_CT_FRAMING_ERROR      Framing error while receiving the ATR
 * @maskedret #PH_ERR_CT_TEMPERATURE_LATCHED  This error bit is set when a temperature latch occurs
 * @maskedret #PH_ERR_CT_PROTECTION_LATCHED   This error bit is set when a protection is latched
 * @maskedret #PH_ERR_CT_CLASS_CHANGE_INDICATED     When prootocol 15 is present with its TA value in 7816.
 * @maskedret #PH_ERR_CT_ATR_SPECIFIC_PARAMETER_UNSUPPORTED     In 7816 profile,in Specific mode either baud rate
 *                                                            or protocol is unsupported.
 * @maskedret #PH_ERR_CT_ATR_PARSER_ERROR   ATR parser failed, ATR is inconsistent with the specification.For 7816
 *                                              profile, user can try for warm reset after this error code.
 */
phStatus_t phhalCt_CardActivate(uint8_t bVccSel, uint8_t* pbAtrBuffer,uint8_t* pbAtrSize,
                                phhalCt_ProtocolParams_t  *psAtrPalParameters);

/**
 * This function is used to performs a warm activation.
 * Configures the timers according to the EMVCo, if success, returns the ATR to the user.
 * @param[in]   pbAtrBuffer  Pointer to the ATR buffer where the ATR bytes will be stored
 * @param[out]  pbAtrSize    Pointer to which the count of ATR bytes is copied
 * @param[out]  psAtrPalParameters   Pointer to structure which will fetch ATR parameters for PAL Layer.
 * @return      Status code for warm activation.
 * @note       User can call this api independently but after phhalCt_CardActivate Api only.
 *
 * @retval    #PH_ERR_SUCCESS  Cold activation successful and ATR bytes are received successfully.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT  Api timed out.
 * @maskedret #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 * @maskedret #PH_ERR_CT_MAIN_CARD_ABSENT  Card is absent in the slot.
 * @maskedret #PH_ERR_CT_CARD_ALREADY_ACTIVATED  Card is already activated.
 * @maskedret #PH_ERR_CT_MUTE_ERROR  Card is muted.
 * @maskedret #PH_ERR_CT_EARLY_ERROR  Card has answered early.
 * @maskedret #PH_ERR_CT_PARITY_ERROR  Card has parity error while receiving the ATR.
 * @maskedret #PH_ERR_CT_OVERUN_ERROR  Fifo is over run while receiving the ATR.
 * @maskedret #PH_ERR_CT_FRAMING_ERROR   Framing error while receiving the ATR.
 * @maskedret #PH_ERR_CT_TEMPERATURE_LATCHED  This error bit is set when a temperature latch occurs.
 * @maskedret #PH_ERR_CT_PROTECTION_LATCHED   This error bit is set when a protection is latched.
 * @maskedret #PH_ERR_CT_ATR_PARSER_ERROR    ATR parser failed, ATR is inconsistent with the specification.
 */
phStatus_t phhalCt_WarmReset(uint8_t *pbAtrBuffer, uint8_t *pbAtrSize, phhalCt_ProtocolParams_t  *psAtrPalParameters);

/**
 * This function is used to set the baud rate, calculate the timing values for BWT,WWT and CWT etc.
 * @param[in] bFidiValue  Fi and Di value which are used to calculate PDR register values and Clock divider values.
 * @param[in] bWI         Waiting integer according to atr ,use for WWT calculation.
 * @param[in] bBWI        Block Waiting integer according to atr ,use for BWT calculation.
 * @param[in] bCWI        Character Waiting integer according to atr ,use for CWT calculation.
 * @return    Status code for Baud Rate Setting.
 *
 * @retval    #PH_ERR_SUCCESS   Setting the baud rate is successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 */
phStatus_t phhalCt_SetBaudRate(uint8_t bFidiValue,uint8_t bWI,uint8_t bBWI ,uint8_t bCWI);

/**
 * This function is used to set the protocol T=0 or T=1.
 * @param[in] bCardProtocol - Protocol value, if set to 1 ,T=1 protocol else for other values T=0 will get select.
 * @note    There is no checking for supported protocol in this api.User should pass only supported protocol as per Atr.
 *        - Calling this api is optional to user,because activation and PPS exchange api calls this api itself in hal,
 *          But If card atr supports both protocol then user can call this api after activation with his own choice
 *          protocol.But User should not call this api after PPS Exchange with other than agreed protocol.
 *
 * @return    Status code for setting transmission protocol.
 *
 * @retval    #PH_ERR_SUCCESS   Protocol bit has been successfully set as per parameter.
 */

#define phhalCt_SetTransmissionProtocol(bCardProtocol)          \
    ((phhalCt_SetTransmissionProtocol_t *)						\
		PH_HAL_CT_SETTRANSMISSIONPROTOCOL_ADDR)\
        (bCardProtocol)

/**
 * This function is used to set the WWT for T=0 protocol,BWT for T=1 protocol according to the mode selected.
 * @param[in] dwMode  The mode can be BWT mode for T=1 protocol, WWT for T=0 protocol.
 *                     Different possible modes are:
 *                     - #PHHAL_CT_ATRMODE_MAXTIME To set the ATR maximum timeout (Used only internal)during activation.
 *                     - #PHHAL_CT_APDUMODE_BWT To set the BWT timeout for T=1 protocol.
 *                     - #PHHAL_CT_APDUMODE_WWT To set the WWT timeout for T=0 protocol.
 *                     - #PHHAL_CT_PPSMODE_9600_ETU To set timer for PPS Exchange.
 * @param[in]  dwWTX  WTX value to be multiplied to the BWT value in case of T=1 protocol.
 *                     - For #PHHAL_CT_APDUMODE_BWT it should be 1 or greater than 1 .
 *                     - For #PHHAL_CT_ATRMODE_MAXTIME/PHHAL_CT_APDUMODE_WWT/PHHAL_CT_PPSMODE_9600_ETU it can be
 *                       any value, and this value is not used.
 * @return     Status code for Set Timer.
 *
 * @retval    #PH_ERR_SUCCESS  Timer values has been applied successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER   Parameters are invalid.
 */
phStatus_t phhalCt_SetTimer(uint32_t dwMode, uint32_t dwWTX);

/**
 * This function is used for PPS request and PPS response handling in negotiable mode.
 * This function also applies new baud rate and new protocol according to negotiated values.
 * @note    Before Calling this Api user has to call #phhalCt_SetTimer with #PHHAL_CT_PPSMODE_9600_ETU mode.
 *        - Parity error retry count for PPS Exchange is used is 7.
 *        - For Errorneous Status code, Structure parameters will be reset to 0.But for #PH_ERR_PPS_EXCHANGE_NOT_REQUIRED
 *          structure parameter will have value same as input.
 *
 * @param[in]   psPPSParams  Pointer to structure containing parameter for PPS Exchange.
 *                - In the structure,bFidi and bNegotiableProtocol is input values for PPS exchange request,
 *                  bFidi will be updated with supported FiDi after PPS.
 *                - bWI,bBWI,bCWI will be input and will have same values as we get from phhalCt_ProtocolParams_t.
 *                - bProtSelT0 ,bProtSelT1 will be output as per what protocol will be selected after PPS exchange.
 *
 * @return Status code for PPS Exchange operation.
 *
 * @retval    #PH_ERR_SUCCESS  If Correct PPS Response has been received successfully,structure parameters
 *                             will be updated as per PPS Response .
 * @maskedret #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 * @maskedret #PH_ERR_CT_PPS_EXCHANGE_ERROR    If Wrong PPS response has been received.
 * @maskedret #PH_ERR_PPS_EXCHANGE_NOT_REQUIRED  If PPS Exchange is called with default baudrate and
 *                                               first offered protocol.
 *
 */
phStatus_t phhalCt_PPSRequestHandling(phhalCt_PPSParamType_t  *psPPSParams);

/**
 * This function is used to transmit data to the card.
 * @param[in]    pbTransmitData  Pointer to the transmit buffer.
 * @param[in]    wTransmitSize   Number of bytes to be transmitted to the card.
 * @return       Status code for transmit operation.
 *
 * @retval    #PH_ERR_SUCCESS   Bytes have been transmitted successfully.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT   Api timed out.
 * @maskedret #PH_ERR_INVALID_PARAMETER   Parameters are invalid.
 * @maskedret #PH_ERR_CT_PARITY_ERROR   Card has nacked the bytes.
 * @maskedret #PH_ERR_CT_OVERUN_ERROR   Fifo is over run while transmitting the bytes.
 * @maskedret #PH_ERR_CT_FRAMING_ERROR   Framing error while transmitting the bytes.
 * @maskedret #PH_ERR_CT_TEMPERATURE_LATCHED   This error bit is set when a temperature latch occurs.
 * @maskedret #PH_ERR_CT_PROTECTION_LATCHED  This error bit is set when a protection is latched.
 */
phStatus_t phhalCt_Transmit(uint8_t *pbTransmitData,uint16_t wTransmitSize);

/**
 * This function is used to receive the data from card.
 * @param[out] pbReceiveData  Pointer to the receive buffer.
 * @param[in] wReceiveSize  Number of bytes to be received from the card.
 * @return Status code for receive operation.
 *
 * @retval    #PH_ERR_SUCCESS   Bytes have been received successfully.
 *
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT  Api timed out.
 * @maskedret #PH_ERR_INVALID_PARAMETER   Parameters are invalid.
 * @maskedret #PH_ERR_CT_PARITY_ERROR   Card has parity errors while sending the bytes.
 * @maskedret #PH_ERR_CT_OVERUN_ERROR   Fifo is over run while receiving the bytes.
 * @maskedret #PH_ERR_CT_FRAMING_ERROR   Framing error while receiving the bytes.
 * @maskedret #PH_ERR_CT_TEMPERATURE_LATCHED  This error bit is set when a temperature latch occurs.
 * @maskedret #PH_ERR_CT_PROTECTION_LATCHED   This error bit is set when a protection is latched.
 * @maskedret #PH_ERR_CT_TIME_OUT_WWT_OR_BWT   BWT/CWT timer elapsed in case of T=1 or
 *                                              WWT timer elapsed in case of T=0.
 */
phStatus_t phhalCt_Receive(uint8_t *pbReceiveData,uint16_t wReceiveSize);

/**
 * This function is used to deactivate the card.
 * @return    Status code for deactivation.
 *
 * @retval    #PH_ERR_SUCCESS   Card is deactivated successfully.
 *
 * @maskedret #PH_ERR_CT_MAIN_CARD_ABSENT  If Card is absent in main slot.
 */
phStatus_t phhalCt_CardDeactivate(void);

/**
 * This function does the DeInitialization of the CT IP.
 * @return    Status code for deactivation.
 *
 * @retval    #PH_ERR_SUCCESS De-Initialization operation completed successfully.
 *
 * @maskedret #PH_ERR_FAILED  De-Initialization failed.
 */
phStatus_t phhalCt_DeInit(void);

/**
 * This function performs a soft reset of the CT IP registers.
 * @note    After this call user has to call the Init Api again, without which the global variables will not be reset
 *
 * @retval    #PH_ERR_SUCCESS  Successfully reset the CT module.
 * @maskedret #PH_ERR_FAILED   Resetting the CT module failed.
 */

phStatus_t phhalCt_Reset(void);

/**
 * This function starts or stops the clock according to the ISO7816 feature.
 * @param[in]  bClockStop   When set to 1, stops the clock. When set to 0, enables the card clock
 * @param[in]  bLowHigh  When set to 1, the clock will stop at HIGH position.
 *                       When set to 0, the clock will stop at LOW.
 *
 * @retval    #PH_ERR_SUCCESS   Clock is stopped or started successfully.
 * @maskedret #PH_ERR_INVALID_PARAMETER   Parameters are invalid.
 */
phStatus_t phhalCt_ClockStartStop(uint8_t bClockStop, uint8_t bLowHigh);

/**
 * This function returns the address of the hal buffer.
 * @param[in] ppbBufferPointer  Pointer in which the address of the hal buffer is returned.
 *
 * @retval    #PH_ERR_SUCCESS The address is successfully returned.
 */
phStatus_t phhalCt_GetBufferPointer(uint8_t ** ppbBufferPointer);

/**
 * This Api is used to asynchronously shut down the CT module/deactivate.
 * This Api can be called from any system level tasks to deactivate the CT asynchronously.
 *
 * @retval None
 */
void phhalCt_AsyncShutDown(void);

/**
 * @brief This function registers the call back function to be called from the
 * ISR on the specified interrupts.
 *
 * @param[in] pCallBackFunc  Function to be called from the ISR.
 * @param[in] dwInterrupts   Interrupt bits on which the call back function shall be called
 *                          (In case of CT only the CT_USR2_REG (UART Status 2) interrupt
 *                          register will be sent with the call back).
 *
 * @return Status of the operation.
 * @retval    #PH_ERR_SUCCESS    Call back registered is successful
 * @maskedret #PH_ERR_INVALID_PARAMETER  Parameter has not been sent correctly
 */
phStatus_t phhalCt_RegCallBack(pphhalCt_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts);

/**
 * This API is the ISR for CTIF.
 *
 * This API is part the Vector Table and direclty called in the event of CT ISR.
 */
void CTIF_IRQHandler(void);

#endif /* NXPBUILD__PHHAL_CT.*/

/** @}.*/
#endif /* PHHAL_CT_H.*/
