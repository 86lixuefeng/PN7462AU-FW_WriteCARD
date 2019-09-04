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
 * Contact Interface : Hardware Abstraction Layer
 * \addtogroup phhalCt
 *
 * The purpose and scope of this file is to contain the definitions and Api's which are internal to the CT module
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters will not be
 *                  checked for erroneous values in the HAL API. In such a build configuration use the API with caution
 *                  and pass valid parameters only. Invalid parameters will likely result in hardfaults or other
 *                  failures.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-29 10:21:08 +0530 (Fri, 29 Jan 2016) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 17615 $ (v04.13.00)
 *
 * @{
 */

#ifndef PHHALCT_INT_H
#define PHHALCT_INT_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "ph_Status.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "phhalCt_Event.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* Common Macros used in both profile EMVCo and 7816.*/
/**
 * CT HAL internal buffer maximum size,
 * A software FIFO of 261 bytes is used in the HAL for synchronization with 32 byte hardware buffer.
 */
#define PHHAL_CT_MAXBUFSIZE                261

/** Macro for Default clock value 4.92MHz for activation.*/
#define PHHAL_CT_CARD_DEFAULT_CLOCK      0x05
/**
 * Macros for CT Timer TOC register configuration value, Timer 1 is stopped, and Timers 3 and 2 form a 16-bit Timer and
 * is started on the first start bit detected on pin I/O.
 */
#define PHHAL_CT_T1STOP_T23STARTONIO_CONFIG    0x71

/**
 * TOC register value for the software trigger
 */
#define PHHAL_CT_T23_SOFTWARE_TRIG_CONFIG    0x61

/**
 * Macros for CT Timer TOC register configuration value, Timer 1,2,3 wired as 24 bit timer. The timer is started on
 * first  start bit, and then on each subsequent bit.
 */
#define PHHAL_CT_T123START_BIT_CONFIG    0x7C
/**
 * Timer 1 starts on 1st Start bit on I/O line, Timer 2 and 3 starts on every start bit on I/O line, this is used during
 * the ATR reception.
 */
#define PHHAL_CT_T1START_T23START_CONFIG    0x75
/**
 * Macro for T=0 protocol.
 */
#define PHHAL_CT_PROTOCOL_T0            0x00

/**
 * Macro for T=1 protocol.
 */
#define PHHAL_CT_PROTOCOL_T1            0x01

/**
 * Macro for T=15 protocol.
 */
#define PHHAL_CT_PROTOCOL_T15           0x0F

/**
 * Macro for T=14 protocol.
 */
#define PHHAL_CT_PROTOCOL_T14           0x0E

/**
 * Masking for extracting last four bits of any byte.
 */
#define PHHAL_CT_LSB_NIBBLE_MASK          0x0F
/**
 * Special value for TC1.
 */
#define PHHAL_CT_SPECIAL_TC1_VALUE        0xFF
/**
 *  Maximum possible value IFSC for 7816 and EMVCo.
 */
#define PHHAL_CT_MAX_IFSC            0xFE
/**
 * Default value for FiDi.
 */
#define PHHAL_CT_DEFAULT_FIDI              0x11
/**
 * UnSupported TC2 for Atr Byte.
 */
#define PHHAL_CT_UNSUPPORTED_TC2            0x00



/* EMVCo Profile Specific Macros.*/
/**
 * Macros for CT Timer TOR register value of 10081 etu MSB started during ATR reception, Loading the value of 0x2761
 *  into TOR register will give the value of 10081 ETU's.
 */
#define PHHAL_CT_MSB_ETU10080_VALUE       0x27

/**
 * Macros for CT Timer TOR register value of 10081 etu LSB started during ATR reception, Loading the value of 0x2761
 *  into TOR register will give the value of 10081 ETU's.
 */
#define PHHAL_CT_LSB_ETU10080_VALUE       0x61

/** The maximum allowed Character waiting Index value in EMVCO mode.*/
#define PHHAL_CT_EMVCO_CWI_MAX            0x05

/** The minimum allowed Character waiting Index value in EMVCO/7816 mode.*/
#define PHHAL_CT_MIN_CWI          0x00

/** The maximum allowed Block waiting Index value in EMVCO mode.*/
#define PHHAL_CT_EMVCO_BWI_MAX            0x04
/**
 * Minimum supported value for FiDi in TA1 for EMVCo.
 */
#define PHHAL_CT_EMVCO_FIDI_MIN_VAL       0x11

/**
 * Maximum supported value for FiDi in TA1 for EMVCo.
 */
#define PHHAL_CT_EMVCO_FIDI_MAX_VAL       0x13
/**
 *  Minimum possible value for IFSC in EMVCo.
 */
#define PHHAL_CT_EMVCO_MIN_IFSC        0x10
/**
 *  Minimum possible value for IFSC in EMVCo.
 */
#define PHHAL_CT_EMVCO_MAX_SUPPORTED_TC1        0x1E
/**
 * EMVCo Supported TC2.
 */
#define PHHAL_CT_EMVCO_SUPPORTED_TC2      0x0A
/**
 * EMVCo Supported TC3.
 */
#define PHHAL_CT_EMVCO_SUPPORTED_TC3      0x00
/**
 * EMVCo max supported ATR Byte count.
 */
#define PHHAL_CT_EMVCO_MAX_ATR_BYTE        32
/**
 * TOR1 timer value for atr mode in EMVCo.
 */
#define PHHAL_CT_EMV_ATRMODE_TOR1_VAL      0xF0

/**
 * TO1 timer maximum allowable count for to be 20160 in case of EMVCo.
 */
#define PHHAL_CT_ATR_TOR1_TIMER_MAX_COUNT   0x54
/**
 * Early timer value for EMVCo is 380(200+180).
 */
#define PHHAL_CT_EMVCO_EARLY_TIMER_VALUE       0xB4

/**
 * Mute timer value for EMVCO is 42000.
 */
#define PHHAL_CT_EMVCO_MUTE_TIMER_LSB_VALUE    0x11
#define PHHAL_CT_EMVCO_MUTE_TIMER_MSB_VALUE    0xA4


/* 7816 profile specific Macros.*/

/**
 * Macros for CT Timer TOR register value of 9600 etu MSB started during ATR reception, Loading the value of 0x2580
 * into TOR register will give the value of 9600 ETU's.
 */
#define PHHAL_CT_MSB_ETU9600_VALUE        0x25

/**
 * Macros for CT Timer TOR register value of 9600 etu LSB started during ATR reception, Loading the value of 0x2580
 * into TOR register will give the value of 9600 ETU's.
 */
#define PHHAL_CT_LSB_ETU9600_VALUE        0x81
/**
 * The maximum parity error count value,Set the number of allowed repetitions in reception or transmission mode before logic 1;
 * setting pe in ct_usr1_reg. The value 000 indicates that, if only one parity error has occurred, bit pe is set at
 *  the value 111 indicates that bit pe will be set at logic 1 after 8 parity errors.
 */
#define PHHAL_CT_MAXPARITY_ERROR_COUNT      7
/**
 * Parity error reset.
 */
#define PHHAL_CT_RESET_PARITY_ERR_COUNT     0x00
/**
 * Specific mode byte MASK in TA1 byte.
 */
#define PHHAL_CT_BIT5_MASK                0x10
/**
 * Changable/Non changable mode byte MASK in TA1 byte.
 */
#define PHHAL_CT_BIT8_MASK                0x80
/** The maximum allowed Block waiting Index value in 7816 mode */
#define PHHAL_CT_7816_BWI_MAX            0x09
/**
 * FIDI table Dimension Length.
 */
#define PHHAL_CT_FIDI_TAB_LEN             108
/**
 *  Minimum possible value for IFSC in 7816.
 */
#define PHHAL_CT_7816_MIN_IFSC          0x01
/**
 * CRC presence check
 */
#define PHHAL_CT_CRC_PRESENCE_CHECK_MASK       0x01
/**
 * ISO7816 max supported ATR Byte count.
 */
#define PHHAL_CT_7816_MAX_ATR_BYTE             33
/**
 * Early timer value for 7816 is 400(200+200).
 */
#define PHHAL_CT_7816_EARLY_TIMER_VALUE         0xC8

/**
 * Mute timer value for 7816 is 40000.
 */
#define PHHAL_CT_7816_MUTE_TIMER_LSB_VALUE      0x41
#define PHHAL_CT_7816_MUTE_TIMER_MSB_VALUE      0x9C


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * *****************************************************************************************************************.*/

/**
 * This Structure contains Atr parameters which will be used during Atr parsing.
 */
typedef struct
{
    phhalCt_ProtocolParams_t *psAtrPalParams;   /**< Pointer to atr parameter structure which will be used in PAL.*/
    uint8_t bInvalidAtr;       /**< Used to set flag if Atr is not according to standard.*/
    uint8_t bTCKByte;          /**< Used during ATR parser as TCK byte presence depends on TD1 byte.*/
    uint8_t bValueofNInTC1;   /**< Used to store extra guard time indicated by atr.*/
    uint8_t bEarlyEventFlag ;  /**< It will be update if early event occurs.*/
    uint8_t bFlagT15;              /**< Used for storing flag 15 information for 7816 if TD byte is as 0x0F.*/
    uint8_t bFlagT15TAPresent;     /**< Used for set presence of TA if flag 15 is present.*/
    uint8_t bWarmResetState;        /**< Used to check the warm reset status in card Warm reset.*/
    uint8_t bInvalidTA1;            /**< Used to set flag if unsupported baudrate indicated in specific mode. */
    uint8_t bInvalidTD1;           /**< Used to set flag if unsupported protocol indicated in specific mode. */
    uint8_t bLastOfferedProt;
}phhalCt_AtrParameterType_t;

/**
 * Clock configuration structure having specified values for Pdr,Clk division factor for particular FiDi.
 */
typedef struct
{
    uint16_t wPdrRegValue;    /**< Used to store pdr register value for different baud rates.*/
    uint8_t  bFiDi;           /**< Used to store value for FiDi as per Atr.*/
    uint8_t  bClockDivider;   /**< Clock divider value according to FiDi.*/
    uint8_t  bDValue;         /**< D value according to Di as per FIDI table.*/
}phhalCt_ClockUart_t;

/**
 * Structure for indicating Atr byte processing function.
 */
typedef struct
{
    phStatus_t (*InterfaceChars)(uint8_t, uint8_t ,phhalCt_AtrParameterType_t *);/**< typedef function pointer
                                                                                      for atr interface
                                                                                      character's processing function.*/
    uint8_t T; /**< Atr switch count  which will change after processing till each TD byte.*/
}phhalCt_AtrType;

/* *****************************************************************************************************************
 * Extern Variables
 * *****************************************************************************************************************.*/
extern phhalCt_EventType_t gphhalCt_InEvent;
extern phhalCt_SlotType_t  gphhalCt_ESlotType;
extern uint8_t gphhalCt_DriverBuff[PHHAL_CT_MAXBUFSIZE];
extern uint8_t gphhalCt_BEmvEn;

/* Following variables are used for the card activation. */
extern uint8_t gphhalCt_BActivationState;
extern uint8_t gphhalCt_BTimerCount;
extern uint8_t gphhalCt_BFirstOfferedProt;
extern uint8_t gphhalCt_BFiDi;
extern uint8_t gphhalCt_BCurrentFiDi;

/* Following variables are used to hold the different waiting times. */
extern uint32_t gphhalCt_DwCharacterWaitingTime;
extern uint32_t gphhalCt_DwWaitingTime;
extern uint32_t gphhalCt_DwBlockWaitingTime;

/* Following variables are used to control the transmit and receive.*/
extern uint8_t  gphhalCt_BTransmitComplete;
extern uint16_t gphhalCt_WPendingBytes;
extern uint8_t  gphhalCt_BCWTFlag;
extern uint16_t gphhalCt_WReceiveOffset;
extern volatile uint16_t gphhalCt_WDataCount;
extern uint16_t gphhalCt_WReceiveSize;
extern volatile uint8_t gphhalCt_BParityErr;
extern uint8_t gphhalCt_BLastByteTransmit;
extern uint8_t gphhalCt_BDisableDCDC;

extern const phhalCt_ClockUart_t gkphhalCt_BPreScalar[];
/* *****************************************************************************************************************
 * Function Prototypes
 * *****************************************************************************************************************.*/

/**
 * This function will clear the all global variables in CT HAL.
 */
void phhalCt_ClearGLobalVar(void);

/**
 * This function is used to set the default values to the  global variables used for protocol parameters.
 */
void phhalCt_SetDefaultValues( phhalCt_AtrParameterType_t  *psAtrParams);

/**
 * Handling of all common event.
 */
phStatus_t phhalCt_HandleCommonEvent(void);

#endif /* NXPBUILD__PHHAL_CT.*/

/** @}.*/
#endif /* PHHALCT_INT_H.*/
