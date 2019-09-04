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
 * Prototype of CLIF RF HAL API.
 *
 * Project:  PN7462AU
 * \addtogroup RFHal
 *
 * The Low Level RF(CLIF) APIs
 *
 * @{
 *
 * This module implements Rf interface HAL in Flash.
 * This module in internally dependent on the \ref phhalSysSer module for ROM System Services calls
 * This module is designed and can be extended into multi-process environment.
 *
 *
 * $Date: 2016-11-24 12:59:45 +0530 (Thu, 24 Nov 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18760 $ (v04.13.00)
 *
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters
 *                  will not be checked for erroneous values in the HAL API. In such a build configuration use the API
 *                  with caution and pass valid parameters only. Invalid parameters will likely result in hardfaults or
 *                  other failures.
 *
 * "Sample Usage Initiator"
 *
 * \dot
 *      digraph {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalRf_Init [ URL="\ref phhalRf_Init"];
 *          phhalRf_DeInit [ URL="\ref phhalRf_DeInit"];
 *          phhalRf_FieldOff [ URL="\ref phhalRf_FieldOff"];
 *          phhalRf_FieldOn [ URL="\ref phhalRf_FieldOn"];
 *          phhalRf_LoadProtocol_Initiator [ URL="\ref phhalRf_LoadProtocol_Initiator"];
 *          phhalRf_PCD_SetExchgFdt [ URL="\ref phhalRf_PCD_SetExchgFdt"];
 *          phhalRf_SetConfig [ URL="\ref PHHAL_RF_CONFIG_RXWAIT_US"];
 *
 *      //Logical Flow
 *          Start ->phhalRf_Init -> phhalRf_LoadProtocol_Initiator;
 *          phhalRf_LoadProtocol_Initiator -> phhalRf_FieldOn;
 *          phhalRf_FieldOn -> phhalRf_PCD_SetExchgFdt;
 *          phhalRf_PCD_SetExchgFdt -> phhalRf_SetMinGuardTime;
 *          phhalRf_SetMinGuardTime -> phhalRf_PCD_Exchg;
 *          phhalRf_PCD_Exchg -> phhalRf_FieldOff;
 *          phhalRf_FieldOff -> phhalRf_DeInit;
 *          phhalRf_DeInit -> End;
 *
 *      }
 * \enddot
 *
 * "Sample Usage target"
 *
 * \dot
 *      digraph {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalRf_Init [ URL="\ref phhalRf_Init"];
 *          phhalRf_DeInit [ URL="\ref phhalRf_DeInit"];
 *          phhalRf_AutoColl [ URL="\ref phhalRf_AutoColl"];
 *          phhalRf_LoadProtocol_Target [ URL="\ref phhalRf_LoadProtocol_Target"];
 *          phhalRf_Receive [ URL="\ref phhalRf_Receive"];
 *          phhalRf_SetConfig [ URL="\ref phhalRf_SetConfig"];
 *          phhalRf_Transmit [ URL="\ref phhalRf_Transmit"];
 *          phhalRf_RegCallBack [ URL="\ref phhalRf_RegCallBack"];
 *
 *      //Logical Flow
 *          Start ->phhalRf_Init -> phhalRf_AutoColl;
 *          phhalRf_AutoColl -> phhalRf_Transmit;
 *          phhalRf_Transmit -> phhalRf_Receive;
 *          phhalRf_Receive ->  phhalRf_DeInit;
 *          phhalRf_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="blue" ];
 *          phhalRf_Receive -> phhalRf_LoadProtocol_Target [label = "Change Baud Rate."];
 *          phhalRf_LoadProtocol_Target -> phhalRf_Transmit;
 *
 *      //optionals
 *          edge [ style="dashed", color="purple" ];
 *          phhalRf_AutoColl -> phhalRf_SetConfig [label = "If external field is off."];
 *          phhalRf_SetConfig -> phhalRf_RegCallBack;
 *          phhalRf_RegCallBack -> phhalRf_AutoColl [label = "If External Field is Turned On"]
 *      }
 * \enddot
 *
 * "Sample Usage for Asynchronous ShutDown."
 *
 * \dot
 *      digraph {
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *          AnotherThread [label = "Another\nThread" ];
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalRf_Init [ URL="\ref phhalRf_Init"];
 *          phhalRf_DeInit [ URL="\ref phhalRf_DeInit"];
 *          phhalRf_FieldOff [ URL="\ref phhalRf_FieldOff"];
 *          phhalRf_FieldOn [ URL="\ref phhalRf_FieldOn"];
 *          phhalRf_LoadProtocol_Initiator [ URL="\ref phhalRf_LoadProtocol_Initiator"];
 *          phhalRf_PCD_SetExchgFdt [ URL="\ref phhalRf_PCD_SetExchgFdt"];
 *          phhalRf_SetConfig [ URL="\ref PHHAL_RF_CONFIG_RXWAIT_US"];
 *          phhalRf_PCD_Exchg [ URL="\ref phhalRf_PCD_Exchg"];
 *
 *      //Logical Flow
 *          Start ->phhalRf_Init -> phhalRf_LoadProtocol_Initiator;
 *          phhalRf_LoadProtocol_Initiator -> phhalRf_FieldOn;
 *          phhalRf_FieldOn -> phhalRf_PCD_SetExchgFdt;
 *          phhalRf_PCD_SetExchgFdt -> phhalRf_SetConfig;
 *          phhalRf_SetConfig -> phhalRf_PCD_Exchg;
 *          phhalRf_PCD_Exchg -> phhalRf_DeInit;
 *          phhalRf_DeInit -> End;
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *          AnotherThread -> phhalRf_SetIdleState [label = "Async Shutdown" ];
 *
 *          edge [ style="dashed", color="purple" ];
 *          phhalRf_SetIdleState -> phhalRf_PCD_Exchg [label = "Aborts the API\nif it was already\nrunning." ];
 *
 *      }
 * \enddot
 */

#ifndef PHHAL_RF_H
#define PHHAL_RF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_RF

#include "ph_Status.h"
#include "phhalRf_LP_Int.h"
#include "phSysSv_Data.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * @name CMA Entry State
 * @{
 */
#define PHHAL_RF_CMA_ENTRY_STATE_IDLE               (0x00)      /**< CMA Entry State as IDLE.*/
#define PHHAL_RF_CMA_ENTRY_STATE_HALT               (0x01)      /**< CMA Entry State as HALT.*/
/** @} */

/**
 * @name Mifare Crypto Keys
 * @{
 */
#define PHHAL_RF_MFC_KEYA_CMD                       (0x60)      /**< Key A.*/
#define PHHAL_RF_MFC_KEYB_CMD                       (0x61)      /**< Key B.*/
/** @} */

/**
 * Macro to Enable Transmission CRC
 */
#define PHHAL_RF_TXCRC_ON()  \
    PH_HALREG_SETBITN(CLIF_CRC_TX_CONFIG_REG,CLIF_CRC_TX_CONFIG_REG_TX_CRC_ENABLE_POS)

/**
 * Macro to Disable Transmission CRC
 */
#define PHHAL_RF_TXCRC_OFF() \
    PH_HALREG_CLEARBITN(CLIF_CRC_TX_CONFIG_REG,CLIF_CRC_TX_CONFIG_REG_TX_CRC_ENABLE_POS)

/**
 * Macro to Enable Reception CRC
 */
#define PHHAL_RF_RXCRC_ON()  \
    PH_HALREG_SETBITN(CLIF_CRC_RX_CONFIG_REG,CLIF_CRC_RX_CONFIG_REG_RX_CRC_ENABLE_POS)
/**
 * Macro to Disable Reception CRC
 */
#define PHHAL_RF_RXCRC_OFF() \
    PH_HALREG_CLEARBITN(CLIF_CRC_RX_CONFIG_REG,CLIF_CRC_RX_CONFIG_REG_RX_CRC_ENABLE_POS)

/**
 * Macro to Enable Transmission Parity
 */
#define PHHAL_RF_TXPARITY_ON() \
    PH_HALREG_SETBITN(CLIF_TX_FRAME_CONFIG_REG, \
        CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_ENABLE_POS)

/**
 * Macro to Disable Transmission Parity
 */
#define PHHAL_RF_TXPARITY_OFF() \
    PH_HALREG_CLEARBITN(CLIF_TX_FRAME_CONFIG_REG, \
        CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_ENABLE_POS)

/**
 * Macro to Enable Receive Parity
 */
#define PHHAL_RF_RXPARITY_ON() \
    PH_HALREG_SETBITN(CLIF_RX_CONFIG_REG, \
        CLIF_RX_CONFIG_REG_RX_PARITY_ENABLE_POS)

/**
 * Macro to Disable Receive Parity
 */
#define PHHAL_RF_RXPARITY_OFF() \
    PH_HALREG_CLEARBITN(CLIF_RX_CONFIG_REG, \
        CLIF_RX_CONFIG_REG_RX_PARITY_ENABLE_POS)

/**
 * Macro Get the Last Bits that were received
 */
#define PHHAL_RF_GET_RXLASTBITS() \
    (PH_HALREG_GETFIELDSHIFT(CLIF_RX_STATUS_REG, \
        CLIF_RX_STATUS_REG_RX_NUM_LAST_BITS_MASK,CLIF_RX_STATUS_REG_RX_NUM_LAST_BITS_POS))

/**
 *
 * @name RF Configurations
 *
 * @ref phhalRf_GetConfig and @ref phhalRf_SetConfig
 *
 * @{
 */
#define PHHAL_RF_CONFIG_PARITY                      0x0000U     /**< Configure Parity \ref phhalRf_Config_t. */
#define PHHAL_RF_CONFIG_TXCRC                       0x0001U     /**< Configure TxCrc \ref phhalRf_Config_t. */
#define PHHAL_RF_CONFIG_RXCRC                       0x0002U     /**< Configure RxCrc \ref phhalRf_Config_t. */
#define PHHAL_RF_CONFIG_TXLASTBITS                  0x0003U     /**< Configure number of valid bits of last Tx-Byte. */
#define PHHAL_RF_CONFIG_RXALIGN                     0x0005U     /**< Configure Rx-Aligned Bits. */
#define PHHAL_RF_CONFIG_RXWAIT_US                   0x0006U     /**< Guard Time */
#define PHHAL_RF_CONFIG_TXWAIT_US                   0x0007U     /**< Guard Time */
#define PHHAL_RF_CONFIG_TIMEOUT_VALUE_US            0x000DU     /**< Configure RC Timeout (in [us]). */
#define PHHAL_RF_CONFIG_ASK100                      0x000CU     /**< Configure ASK 100% \ref phhalRf_Config_t.*/
#define PHHAL_RF_CONFIG_TIMEOUT_VALUE_MS            0x000EU     /**< Configure RC Timeout (in [ms]). */
#define PHHAL_RF_CONFIG_CRCPRESET                   0x0016U     /**< Configure the CRC Preset Value */
#define PHHAL_RF_CONFIG_TXCRCTYPE                   0x0017U     /**< Configure the Tx CRC Type.*/
#define PHHAL_RF_CONFIG_RXCRCTYPE                   0x0018U     /**< Configure the Rx CRC Type.*/
#define PHHAL_RF_CONFIG_CRCINVERTED                 0x0019U     /**< Configure CRC Inverted \ref phhalRf_Config_t.*/
#define PHHAL_RF_CONFIG_JEWEL_MODE                  0x0020U     /**< Configure Jewel mode \ref phhalRf_Config_t. */
#define PHHAL_RF_CONFIG_POLL_GUARD_TIME_US          0x0035U     /**< Configure Poll Guard Time. */
#define PHHAL_RF_CONFIG_RXMULTIPLE                  0x0053U     /**< Configure Multiple Rx \ref phhalRf_Config_t. */
#define PHHAL_RF_CONFIG_CMA_STATE                   0x0055U     /**< Get Config for CMA state.*/
#define PHHAL_RF_CONFIG_MODINDEX                    0x0056U     /**< Configure modulation index \ref phhalRf_Config_t.*/

/** Configure operation mode for HAL. Values can be #PHHAL_RF_OPE_MODE_ISO or #PHHAL_RF_OPE_MODE_NFC
 * or #PHHAL_RF_OPE_MODE_EMVCO
 * */
#define PHHAL_RF_CONFIG_OPE_MODE                    0x005AU
#define PHHAL_RF_CONFIG_SET_EMD                     0x005EU     /**< Configure EMD Handling \ref phhalRf_Config_t. */
/**
 * Get the status of internal RF field. Returned values are #PH_ON or #PH_OFF.
 *
 * */
#define PHHAL_RF_CONFIG_INT_RF_ON                   0x005FU
#define PHHAL_RF_CONFIG_EXCHG_TIMEOUT_VALUE_US      0x0101U     /**< Configure the Exchange Timeout.*/
#define PHHAL_RF_CONFIG_NFCLD                       0x0102U     /**< Configure the NFC LD level.*/
#define PHHAL_RF_CONFIG_AGCVREF                     0x0103U     /**< Configure the AGC Reference value */

/** Configure the NFC IP1 \ref phhalRf_Config_t. Note NEVER CHANGE THE MACRO VALUE. */
#define PHHAL_RF_CONFIG_NFCIP1                      0x0106U
/** Configure the 15693 baud rate. Note NEVER CHANGE THE MACRO VALUE. */
#define PHHAL_RF_CONFIG_15693_RX_BAUDRATE           0x0107U
/** Configure the EPC TARI. Note NEVER CHANGE THE MACRO VALUE. */
#define PHHAL_RF_CONFIG_EPC_TX_TARI                 0x0108U
/** Configure the EPC tx symbol. Note NEVER CHANGE THE MACRO VALUE. */
#define PHHAL_RF_CONFIG_EPC_TX_SYMBOL               0x0109U
/** Configure the EPC rx baudrate. Note NEVER CHANGE THE MACRO VALUE. */
#define PHHAL_RF_CONFIG_EPC_RX_BAUDRATE             0x010AU
#define PHHAL_RF_CONFIG_HID                         0x010BU     /**< Configure the HID \ref phhalRf_Config_t*/
#define PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ            0x010CU     /**< Configure the HAL for External Field Off Irq. */
#define PHHAL_RF_CONFIG_RX_MAX_LENGTH               0x010DU     /**< Rx Maximum Capacity */
/** Configure the 15693 TX baud rate. Note NEVER CHANGE THE MACRO VALUE. */
#define PHHAL_RF_CONFIG_15693_TX_BAUDRATE           0x010EU


#define PHHAL_RF_CONFIG_CRC16                   0x0000U     /**< 16 bit CRC. */
#define PHHAL_RF_CONFIG_CRC5                    0x0001U     /**< 5 bit CRC. */

/** @} */

/**
 * @name EPCV2 Rx Baud Rates.
 * @{
 */
#define PHHAL_RF_CONFIG_VAL_EPCV2_SC424_2MP     0x0000U     /**< Configure for Subcarrier 424 and 2 Machester Period.*/
#define PHHAL_RF_CONFIG_VAL_EPCV2_SC424_4MP     0x0001U     /**< Configure for Subcarrier 424 and 4 Machester Period.*/
#define PHHAL_RF_CONFIG_VAL_EPCV2_SC848_2MP     0x0002U     /**< Configure for Subcarrier 848 and 2 Machester Period.*/
#define PHHAL_RF_CONFIG_VAL_EPCV2_SC848_4MP     0x0003U     /**< Configure for Subcarrier 848 and 2 Machester Period.*/
/** @} */

#define PHHAL_RF_OPE_MODE_EMVCO               0x01U /**< EMVCo Mode. */
#define PHHAL_RF_OPE_MODE_NFC                 0x02U /**< NFC Forum Mode. */
#define PHHAL_RF_OPE_MODE_ISO                 0x03U /**< ISO Mode. */
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * RF Transmission and Reception BitRates
 */
typedef enum phhalRf_BitRate{
    /** \b 0x00 */  E_PH_HALRF_BR_UNKNOWN = 0,
    /** \b 0x02 */  E_PH_HALRF_BR_26K = 2,      /**< 26k Bit Rate */
    /** \b 0x03 */  E_PH_HALRF_BR_53K = 3,      /**< 53k Bit Rate */
    /** \b 0x04 */  E_PH_HALRF_BR_106K = 4,     /**< 106k Bit Rate */
    /** \b 0x05 */  E_PH_HALRF_BR_212K = 5,     /**< 212k Bit Rate */
    /** \b 0x06 */  E_PH_HALRF_BR_424K = 6,     /**< 424k Bit Rate */
    /** \b 0x07 */  E_PH_HALRF_BR_848K = 7,     /**< 848k Bit Rate */
    /** \b 0x08 */  E_PH_HALRF_BR_MAX  = 8,     /**< Comparison Maximum for this Enum type */
    /** @internal */ E_PH_HALRF_SIZEADJUST_BR = 0x7FFFFFFFU /**< For 32bit Enum Adjustment */
}phhalRf_BitRate_t;

/**
 * Enable/Disabling Features and setting flag attributes
 */
typedef enum phhalRf_Config{
    /** \b 0x00 */ E_PH_HALRF_DISABLE,    /**< Configure to Disable */
    /** \b 0x01 */ E_PH_HALRF_ENABLE,     /**< Configure to Enable */
    /** \b 0x02 */ E_PH_HALRF_KEEP_ASIS,  /**< Keep it as it is */
    /** \b 0x03 */ E_PH_HALRF_DONE,       /**< Operation Done. */
    /** \b 0x04 */ E_PH_HALRF_CONFIG_MAX, /**< Comparison Maximum */
    /** @internal */ E_PH_HALRF_SIZEADJUST = 0x7FFFFFFFU   /**< For 32bit Enum Adjustment */
}phhalRf_Config_t;

/**
 * Rf Transceive parameter configuration
 */
typedef enum phhalRf_Tx_Rx{
    /** \b 0x00 */ E_PH_HALRF_TX,        /**< Transmit option */
    /** \b 0x01 */ E_PH_HALRF_RX,        /**< Receive option */
    /** \b 0x02 */ E_PH_HALRF_TX_RX_MAX, /**< Comparison Maximum */
    E_PH_HALRF_SIZEADJUST_Tx_Rx = 0x7FFFFFFFU   /**< For 32bit Enum Adjustment */
}phhalRf_Tx_Rx_t;

/**
 * Listen Mode Technologies
 */
typedef enum phhalRf_Listen_Tech{
    E_LISTEN_NONE = 0,                                      /**< Listen to None */
    E_LISTEN_PAS_A = 0x01,                                  /**< Listen Type A Passive only */
    E_LISTEN_B = 0x02,                                      /**< Type B Passive only */
    E_LISTEN_PAS_F = 0x04,                                  /**< Listen Type F Passive only */

    E_LISTEN_ACT_A = 0x100,                                 /**< Listen Type A Active only */
    E_LISTEN_ACT_F = 0x400,                                 /**< Listen Type F Active only */

    E_LISTEN_PAS_A_B = E_LISTEN_PAS_A | E_LISTEN_B,         /**< Listen Type A Passive and Type B Passive */

    E_LISTEN_PAS_A_PAS_F = E_LISTEN_PAS_A | E_LISTEN_PAS_F, /**< Listen Type A Passive and Type F Passive */

    E_LISTEN_PAS_F_B = E_LISTEN_PAS_F | E_LISTEN_B,         /**< Listen Type A Passive and Type B Passive */

    E_LISTEN_PAS_A_B_PAS_F =  E_LISTEN_PAS_A | E_LISTEN_B
    | E_LISTEN_PAS_F,                                       /**< Listen Type A Passive,Type B Passive & Type F Passive*/

    E_LISTEN_ACT_A_B = E_LISTEN_ACT_A | E_LISTEN_B,         /**< Listen Type A Active and Type B Passive */

    E_LISTEN_ACT_A_ACT_F = E_LISTEN_ACT_A | E_LISTEN_ACT_F, /**< Listen Type A Active and Type F Active */

    E_LISTEN_PAS_A_ACT_A = E_LISTEN_PAS_A | E_LISTEN_ACT_A, /**< Listen Type A Passive and Type A Active */

    E_LISTEN_ACT_F_B = E_LISTEN_ACT_F | E_LISTEN_B,         /**< Listen Type F Active and Type B Passive */

    E_LISTEN_PAS_A_ACT_F = E_LISTEN_PAS_A | E_LISTEN_ACT_F, /**< Listen Type A Passive and Type F Active */

    E_LISTEN_PAS_F_ACT_F = E_LISTEN_PAS_F | E_LISTEN_ACT_F, /**< Listen Type A Passive and Type F Active */

    E_LISTEN_ACT_A_PAS_F = E_LISTEN_ACT_A | E_LISTEN_PAS_F, /**< Listen Type A Active and Type F Passive */

    E_LISTEN_ALL = E_LISTEN_PAS_A | E_LISTEN_PAS_F
    | E_LISTEN_B | E_LISTEN_ACT_A
    | E_LISTEN_ACT_F,               /**< Listen all the Technologies. */

    E_SIZEADJUST_LISTEN_TECH = 0x7FFFFFFFU         /**< For 32bit Enum Adjustment */
}phhalRf_Listen_Tech_t;

/**
 * Transceiver State of CLIF
 */
typedef enum phhalRf_TransceiveState{
    /** \b 0x00 */ E_TRANSCEIVE_STATE_IDLE = 0,      /**< Idle state.*/
    /** \b 0x01 */ E_TRANSCEIVE_STATE_WAIT_TRANSMIT, /**< Transmitter is waiting for TX Wait to complete */
    /** \b 0x02 */ E_TRANSCEIVE_STATE_TRANSMITTING,  /**< Transmitter is actively transmitting data on RF */
    /** \b 0x03 */ E_TRANSCEIVE_STATE_WAIT_RECEIVE,  /**< Receiver is waiting for RX Wait to Complete*/
    /** \b 0x04 */ E_TRANSCEIVE_STATE_WAIT_FOR_DATA, /**< Receiver is waiting to receive data*/
    /** \b 0x05 */ E_TRANSCEIVE_STATE_RECEIVING,     /**< Receiver is actively receiving data on RF*/
    /** \b 0x06 */ E_TRANSCEIVE_STATE_WAIT_LP_BACK,  /**< Loop Back configuration for the Transceiver. */
    /** \b 0x07 */ E_TRANSCEIVE_STATE_LOOP_BACK,     /**< Transmitter or the Receiver is actively Transacting with RF*/
    /** \b 0x08 */ E_TRANSCEIVE_STATE_MAX,           /**< Comparison Maximum for this Enum type */
    E_SIZEADJUST_TRANSCEIVE_STATE = 0x7FFFFFFFU /**< For 32bit Enum Adjustment */
} phhalRf_TransceiveState_t;

/**
 * Communication Mode.
 */
typedef enum phhalRf_CommMode{
    /** \b 0x00 */ E_COMM_MODE_PASSIVE, /**< Passive Mode of Communication. */
    /** \b 0x01 */ E_COMM_MODE_ACTIVE,  /**< Active Mode of Communication. */
    /** \b 0x02 */ E_COMM_MODE_MAX      /**< Comparison Maximum for this Enum type */
} phhalRf_CommMode_t;

/**
 * CLIF CMA State
 */
typedef enum phhalRf_CMA_State{
    /** \b 0x00 */ E_PHHAL_RF_CMA_STATE_MODE_SET = 0, /**< Mode Detection. */
    /** \b 0x01 */ E_PHHAL_RF_CMA_STATE_IDLE,         /**< SENSE state. */
    /** \b 0x02 */ E_PHHAL_RF_CMA_STATE_READY1,       /**< RESOLUTION1 state. */
    /** \b 0x03 */ E_PHHAL_RF_CMA_STATE_READY2,       /**< RESOLUTION2 state. */
    /** \b 0x04 */ E_PHHAL_RF_CMA_STATE_READY3,       /**< RESOLUTION3 state. */
    /** \b 0x05 */ E_PHHAL_RF_CMA_STATE_ACTIVE,       /**< SELECTED state. */
    /** \b 0x06 */ E_PHHAL_RF_CMA_STATE_HALT,         /**< SLEEP state. */
    /** \b 0x07 */ E_PHHAL_RF_CMA_STATE_READY1x,      /**< RESOLUTION1* state. */
    /** \b 0x08 */ E_PHHAL_RF_CMA_STATE_READY2x,      /**< RESOLUTION2* state. */
    /** \b 0x09 */ E_PHHAL_RF_CMA_STATE_READY3x,      /**< RESOLUTION3* state. */
    /** \b 0x0A */ E_PHHAL_RF_CMA_STATE_ACTIVEx       /**< SELECTED* state. */
}phhalRf_CMA_State_t;

/**
 * CLIF Card Mode Activation Commands
 */
typedef enum phhalRf_CMA_Command{
    /** \b 0x00 */ PHHAL_RF_CMA_DISABLE = 0,   /**< Command to Disable the CMA Sub-Module */
    /** \b 0x01 */ PHHAL_RF_CMA_ENABLE,        /**< Command to Initialize the CMA Sub-Module and enable it */
    /** \b 0x02 */ PHHAL_RF_CMA_SETENTRYSTATE, /**< Command for setting the Entry state when Activation is performed */
    /** \b 0x03 */ PHHAL_RF_CMA_GETSTATE,      /**< Command for obtaining the Internal State of Activation operation */
    /** \b 0x04 */ PHHAL_RF_CMA_MAX            /**< Comparison Maximum for this Enum type */
}phhalRf_CMA_Command_t;

/**
 * Received Packet Parameters.
 */
typedef struct phhalRf_RxParams{
    uint8_t bRxCollPos; /**< Collision Position */
    uint8_t bRxBits;    /**< Number of Bits Received */
    uint8_t bRxFrames;  /**< In case of Multiple Reception Number of Frames Received */
    uint8_t bReserved;  /**< Padding */
    uint16_t wRxBytes;  /**< Number of Bytes Received */
    uint16_t wReserved; /**< Padding */
}phhalRf_RxParams_t;

/**
 * PICC Type B Parameter.
 */
typedef struct phhalRf_CardMode_TypeB_Param{
    uint8_t * pAtqb;        /**< Pointer pointer to ATQB. NOTE : The ATQB buffer has to be in RAM. */
    uint8_t bExtendedAtqb;  /**< Enable or Disable ExtendedAtqb */
}phhalRf_CardMode_TypeB_Param_t;

/**
 * PICC Type F Parameters.
 */
typedef struct phhalRf_CardMode_TypeF_Param{
    uint8_t * pSensfResp;   /**< Pointer pointing to Sense F Response. NOTE : The SENSF_RESP buffer has to be in RAM. */
    uint8_t bSensfRespLen;  /**< Sense F Response Length */
}phhalRf_CardMode_TypeF_Param_t;

/**
 * Protocol detected in Target mode.
 */
typedef enum phhalRf_ProtocolDetected{
    /** \b 0x00 */ E_ISO14443_4A,          /**< Type A Layer 4 Command Detected.*/
    /** \b 0x01 */ E_ISO18092,             /**< ISO 18092 Command Detected. */
    /** \b 0x02 */ E_TYPE_F,               /**< Type F Command Detected*/
    /** \b 0x03 */ E_ISO14443B,            /**< Type B Command Detected. */
    /** \b 0x04 */ E_PROTOCOLDETECTED_MAX  /**< Comparison Maximum for this Enum type */
}phhalRf_ProtocolDetected_t;

/**
 * Activated Target Parameter.
 */
typedef struct phhalRf_TargetActivatedParam{
    uint8_t * ppRxBuffer;                           /**< Received Command in Target Mode.*/
    uint16_t wRxLength;                             /**< Receive Command Length. */
    phhalRf_BitRate_t eBaudRateDetected;            /**< Baud Rate Detected by the Target.*/
    phhalRf_ProtocolDetected_t eProtocolDetected;   /**< Protocol Detected by the Target.*/
}phhalRf_TargetActivatedParam_t;

/**
 * Mifare Crypto Authentication Parameters.
 */
typedef struct phhalRf_MFCParams{
    uint8_t bKeyCmd;        /**< Key Command can be 0x60 for A type and 0x61 for B type */
    uint8_t bBlock;         /**< Block number to be authenticated */
    uint8_t bsizeUID;       /**< UID Size can be 4 or 7 bytes only */
    uint8_t bReserved;      /**< Padding */
    uint8_t * pUID;         /**< UID of card being authenticated must be 4 to 7 bytes long
                            @warning the UID Buffer @em pUID is assumed to be minimum 4bytes */
    uint8_t * pKey;         /**< Key of the Block being authenticated must be 6 bytes long
                            @warning the Key Buffer @em pKey is assumed to be of 6bytes */
}phhalRf_MFCParams_t;


/**
 * @name Parameter lengths for Autocoll
 *
 * @{
 *
 * See @ref  phhalRf_AutoCollParam_t and @ref phhalRf_AutoColl.
 */
#define PHHAL_RF_SENSE_RES_LEN                  0x02
#define PHHAL_RF_NFCID1_LEN                     0x03
#define PHHAL_RF_SEL_REP_LEN                    0x01
#define PHHAL_RF_F_PROTOCOL_SUP_BYTE_LEN        0x02
#define PHHAL_RF_NFCID2_LEN                     0x06
#define PHHAL_RF_PAD0_LEN                       0x02
#define PHHAL_RF_PAD1_LEN                       0x03
#define PHHAL_RF_MRTICHECK_LEN                  0x01
#define PHHAL_RF_MRTIUPDATE_LEN                 0x01
#define PHHAL_RF_PAD2_LEN                       0x01
#define PHHAL_RF_RD_LEN                         0x02

#define PHHAL_RF_PAD_LEN                        (PHHAL_RF_PAD0_LEN + PHHAL_RF_PAD1_LEN + PHHAL_RF_MRTICHECK_LEN \
    + PHHAL_RF_MRTIUPDATE_LEN + PHHAL_RF_PAD2_LEN + PHHAL_RF_RD_LEN)

#define PHHAL_RF_SYSTEM_CODE_LEN                0x02

/**
 * @}
 */

/**
 * Parameters for Target @ref phhalRf_AutoColl.
 */
typedef struct phhalRf_AutoCollParam {

    uint8_t pSenseRes[PHHAL_RF_SENSE_RES_LEN]; /**< Sense Response/ATQA for Type A */
    uint8_t pNfcId1[PHHAL_RF_NFCID1_LEN];      /**< UID / NFC ID 1 based on protocol */
    uint8_t bSelRes;                           /**< SAK / Select Response */
    /** Procol supported.
     *   -NFC DEP: 0x01 0xFE
     *   -T3T: 0x02 0xFE / Others
     */
    uint8_t bFProtSupp[PHHAL_RF_F_PROTOCOL_SUP_BYTE_LEN];
    uint8_t pNfcId2[PHHAL_RF_NFCID2_LEN];     /**< NFC ID2 */
    uint8_t pPad[PHHAL_RF_PAD_LEN];           /**< Pad Array for SenseF Response */
    uint8_t pSystemCode[PHHAL_RF_SYSTEM_CODE_LEN]; /**< System Code */
} phhalRf_AutoCollParam_t;      /**< @ref phhalRf_AutoCollParam  */

/**
 * RF ISR Call back function signature.
 */
typedef void ( *pphhalRf_CallbackFunc_t )( uint32_t );


/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */


/**
 * Number of entries of registers to be loaded for Initiator for transmission.
 */
extern const uint8_t gkphhalRf_InitiatorTxNumEntries[];
/**
 * Register address offset to be loaded for Initiator for transmission.
 */
extern const uint8_t gkphhalRf_InitiatorTxRegAddr[];


/** Masking is only enabled for DPC */
extern const uint8_t gkphhalRf_InitTxRegMaskIndex[];


/**
 * Adding the mask for Tx Ocp of  CLIF_ANA_TX_AMPLITUDE_REG
 */
extern const uint32_t gkphhalRf_InitTxRegMaskValue[];


/**
 * Number of entries of registers to be loaded for Initiator for reception.
 */
extern const uint8_t gkphhalRf_InitiatorRxNumEntries[];
/**
 * Register address offset to be loaded for Initiator for reception.
 */
extern const uint8_t gkphhalRf_InitiatorRxRegAddr[];

/**
 * Number of entries of registers to be loaded for Target for transmission.
 */
extern const uint8_t gkphhalRf_TargetTxNumEntries[];
/**
 * Register address offset to be loaded for Target for transmission.
 */
extern const uint8_t gkphhalRf_TargetTxRegAddr[];

/**
 * Number of entries of registers to be loaded for Target for reception.
 */
extern const uint8_t gkphhalRf_TargetRxNumEntries[];
/**
 * Register address offset to be loaded for Target for reception.
 */
extern const uint8_t gkphhalRf_TargetRxRegAddr[];

/**
 * Mask index for Target Mode.
 */
extern const uint8_t gkphhalRf_TarTxRegMaskIndex[];

/**
 * Adding the mask for APC of  CLIF_ANA_TX_AMPLITUDE_REG and CLIF_TX_CONTROL_REG
 */
extern const uint32_t gkphhalRf_TarTxRegMaskValue[];

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * This function configures the RF with basic setup values needed to start its operation.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_Init(void);

/**
 * This function shutdowns the RF block and restore the PCR values related to CLIF.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_DeInit(void);

#if PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_ARMGCC
phStatus_t phhalRf_Deinit(void)   __attribute__ ((deprecated ("Use phhalRf_DeInit instead.") ));
#elif PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_IAR
/* phhalRf_Deinit not exposed in IAR Compiler */
#elif PHFL_CONFIG_TARGET_PLATFORM == PH_CONFIG_TARGET_PLATFORM_DS5
/* phhalRf_Deinit not exposed in armcc/ds5 Compiler */
#endif



/**
 * This function will Load the Protocol settings for Initiator.
 *
 * @param[in] eTxNum Transmission Protocol \ref phhalRf_LP_I_Tx_t
 * @param[in] eRxNum Reception Protocol \ref phhalRf_LP_I_Rx_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid parameter passed.
 * @maskedret #PH_ERR_RESOURCE_ERROR Base Address are not configured.
 */
phStatus_t phhalRf_LoadProtocol_Initiator(phhalRf_LP_I_Tx_t eTxNum, phhalRf_LP_I_Rx_t eRxNum);

/**
 * This function will Load the Protocol settings for Target.
 *
 * @param[in] eTxNum Transmission Protocol \ref phhalRf_LP_T_Tx_t
 * @param[in] eRxNum Reception Protocol \ref phhalRf_LP_T_Rx_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid parameter passed.
 * @maskedret #PH_ERR_RESOURCE_ERROR Base Address are not configured.
 */
phStatus_t phhalRf_LoadProtocol_Target(phhalRf_LP_T_Tx_t eTxNum, phhalRf_LP_T_Rx_t eRxNum);

/**
 * This API set the RF Internal State to Idle thus terminating any ongoing communications.
 *
 * @param[in] bIrqEnable Option to enable or disable IDLE IRQ. \ref phhalRf_Config_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 *
 */
phStatus_t phhalRf_SetIdleState(phhalRf_Config_t bIrqEnable);

/**
 * This is used to configure the RF Driver to generate the field for the Required RF Protocol Conditions
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_FieldOn(void);

/**
 * This function clears the RF Driver Configuration and waits for the RF Field to turn off
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 *
 */
phStatus_t phhalRf_FieldOff(void);

/**
 * This function resets the RF field.
 *
 * @param[in] dwRfOffGuardTimeUs RF Off Guard Time(in us).
 * @param[in] dwRecoveryTimeUs RF Off Recovery Time(in us).
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_FieldReset(uint32_t dwRfOffGuardTimeUs, uint32_t dwRecoveryTimeUs);

/**
 * RF HAL External RF Field On Status Function
 *
 * @return External RF Field Status
 * @retval #PH_ERR_SUCCESS External field is present
 * @maskedret #PH_ERR_EXT_RF_ERROR The external field is off
 * @retval Other Depends on the underlying components
 *
 */
phStatus_t phhalRf_GetExtFieldStatus(void);

/**
 * This function would disable the Internal Mifare Crypto block and reset its configuration
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_MFC_Disable(void);

/**
 * This function is used to configure the RF Exchange Timing needed to perform communications in the Transceiver State.
 *
 * @param[in] dwFDTUs Frame Delay Time(in us).
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_PCD_SetExchgFdt(uint32_t dwFDTUs);

/**
 * This would perform exchange on the Rf.
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned).
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[in] wTxLength Transmit Length(bytes: max 260).
 * @param[out] pRxParams Receive Status data structure \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_IO_TIMEOUT Transmission does not completed.
 * @maskedret #PH_ERR_ABORTED User Aborted the transmission..
 * @maskedret #PH_ERR_IO_TIMEOUT Reception does not happen.
 * @maskedret #PH_ERR_BUFFER_OVERFLOW Rx buffer overflow
 * @maskedret #PH_ERR_INTEGRITY_ERROR Invalid packet received.
 * @maskedret #PH_ERR_PROTOCOL_ERROR Invalid response received.
 * @maskedret #PH_ERR_COLLISION_ERROR Collision detected.
 * @maskedret #PH_ERR_LENGTH_ERROR In case of Rx multiple, length error occurred.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_PCD_Exchg(
    const uint8_t * pTxBuffer,
    const uint8_t * pRxBuffer,
    const uint16_t wTxLength,
    phhalRf_RxParams_t * pRxParams
);

/**
 * This function internally configures the RF System for ISO14443-3A Short Frame configuration and then performs
 * #phhalRf_PCD_Exchg
 *
 * @param[in] pTxBuffer  Transmit Buffer(DWORD aligned).
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[out] pRxParams Receive Status data structure. \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_IO_TIMEOUT Transmission does not completed.
 * @maskedret #PH_ERR_ABORTED User Aborted the transmission..
 * @maskedret #PH_ERR_IO_TIMEOUT Reception does not happen.
 * @maskedret #PH_ERR_BUFFER_OVERFLOW Rx buffer overflow
 * @maskedret #PH_ERR_INTEGRITY_ERROR Invalid packet received.
 * @maskedret #PH_ERR_PROTOCOL_ERROR Invalid response received.
 * @maskedret #PH_ERR_COLLISION_ERROR Collision detected.
 * @maskedret #PH_ERR_LENGTH_ERROR In case of Rx multiple, length error occurred.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_PCD_ExchgISO14443A_ShortFrame(
    const uint8_t * pTxBuffer,
    const uint8_t * pRxBuffer,
    phhalRf_RxParams_t * pRxParams);

/**
 * This function internally configures the RF System for ISO14443-3A Standard Frame configuration and then performs
 * #phhalRf_PCD_Exchg
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned).
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[in] wTxLength Transmit Length(Bytes:  max 260)
 * @param[out] pRxParams Receive Status data structure. \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_IO_TIMEOUT Transmission does not completed.
 * @maskedret #PH_ERR_ABORTED User Aborted the transmission..
 * @maskedret #PH_ERR_IO_TIMEOUT Reception does not happen.
 * @maskedret #PH_ERR_BUFFER_OVERFLOW Rx buffer overflow
 * @maskedret #PH_ERR_INTEGRITY_ERROR Invalid packet received.
 * @maskedret #PH_ERR_PROTOCOL_ERROR Invalid response received.
 * @maskedret #PH_ERR_COLLISION_ERROR Collision detected.
 * @maskedret #PH_ERR_LENGTH_ERROR In case of Rx multiple, length error occurred.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_PCD_ExchgISO14443A_StandardFrame(
    const uint8_t * pTxBuffer,
    const uint8_t * pRxBuffer,
    const uint16_t wTxLength,
    phhalRf_RxParams_t * pRxParams);

/**
 * This function internally configures the RF System for ISO14443-3A AntiCollision or Select Frame configuration and
 * then performs #phhalRf_PCD_Exchg
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned).
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[in] wTxLength Transmit Length(Bytes: 260 max).
 * @param[in] bTxLastBits Transmit Bits (0 to 7 bits).
 * @param[in] bRxBitAlign Rx Bit Align(0 to 7 bits).
 * @param[out] pRxParams Receive Status data structure. \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_IO_TIMEOUT Transmission does not completed.
 * @maskedret #PH_ERR_ABORTED User Aborted the transmission..
 * @maskedret #PH_ERR_IO_TIMEOUT Reception does not happen.
 * @maskedret #PH_ERR_BUFFER_OVERFLOW Rx buffer overflow
 * @maskedret #PH_ERR_INTEGRITY_ERROR Invalid packet received.
 * @maskedret #PH_ERR_PROTOCOL_ERROR Invalid response received.
 * @maskedret #PH_ERR_COLLISION_ERROR Collision detected.
 * @maskedret #PH_ERR_LENGTH_ERROR In case of Rx multiple, length error occurred.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_PCD_ExchgISO14443A_ACFrame(
    const uint8_t * pTxBuffer,
    const uint8_t * pRxBuffer,
    const uint16_t wTxLength,
    const uint8_t bTxLastBits,
    const uint8_t bRxBitAlign,
    phhalRf_RxParams_t * pRxParams);

/**
 * This function helps to perform Multiple Reception for the Single Transmitted packet.
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned).
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[in] wTxLength Transmit Length(bytes: 260 max).
 * @param[out] pRxParams Receive Status data structure. \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_IO_TIMEOUT Transmission does not completed.
 * @maskedret #PH_ERR_ABORTED User Aborted the transmission..
 * @maskedret #PH_ERR_IO_TIMEOUT Reception does not happen.
 * @maskedret #PH_ERR_BUFFER_OVERFLOW Rx buffer overflow
 * @maskedret #PH_ERR_INTEGRITY_ERROR Invalid packet received.
 * @maskedret #PH_ERR_PROTOCOL_ERROR Invalid response received.
 * @maskedret #PH_ERR_COLLISION_ERROR Collision detected.
 * @maskedret #PH_ERR_LENGTH_ERROR Length error occurred.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_PCD_ExchgRxMultiple(
    const uint8_t * pTxBuffer,
    const uint8_t * pRxBuffer,
    const uint16_t wTxLength,
    phhalRf_RxParams_t * pRxParams);

/**
 * This function performs the necessary transactions and configurations for Mifare Authentication
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned) must of size 8bytes at least and must contain the
 * First Authentication command to begin the Authentication process
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[in] pMFCparams Mifare Authentication data for the Authentication Process. \ref phhalRf_MFCParams_t
 * @param[out] pRxParams Receive Status data structure. \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_AUTH_ERROR MF authentication fail.
 * @retval Other Depends on the underlying components.
 *
 */
phStatus_t phhalRf_PCD_ExchgMFC_Auth(
    uint8_t * pTxBuffer,
    uint8_t * pRxBuffer,
    phhalRf_MFCParams_t *pMFCparams,
    phhalRf_RxParams_t * pRxParams
);

/**
 * @brief This function set the value according to dwConfig
 *
 * @param[in] dwConfig Configuration to set.
 * @param[in] dwValue  Value to set.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid parameter passed.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_SetConfig(uint32_t dwConfig, uint32_t dwValue);

/**
 * @brief This function get the value according to dwConfig
 *
 * @param[in] dwConfig Configuration to get.
 * @param[in] dwValue  Value to get.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid parameter passed.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_GetConfig(uint32_t dwConfig, uint32_t * dwValue);

/**
 * This function will perform the AutoColl(Target mode) and return the Technology Listened.
 *
 * @param[in] psAutoCollParams Parameters for AutoColl. @ref phhalRf_AutoCollParam_t
 * @param[in] bPiccAState Entry state for PICC
 * @param[in] wMode Supported listen technologies, see @ref phhalRf_Listen_Tech_t
 * @param[out] ppRxBuffer Pointer to pointer which the HAL will point to the HAL Receive Buffer .
 * @param[out] pRxLength Length of ppRxBuffer
 * @param[out] pProtParams Return data about of Protocol detection
 * @return Status of the API
 *
 * @retval #PH_ERR_SUCCESS  Operation successful
 * @retval Other Depends on the underlying components
 */
phStatus_t phhalRf_AutoColl(phhalRf_AutoCollParam_t * const psAutoCollParams,
    const uint8_t bPiccAState,
    phhalRf_Listen_Tech_t wMode,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength,
    phhalRf_TargetActivatedParam_t * pProtParams);
/**
 * This function perform Transmit.
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned).
 * @param[in] wTxLength Transmit Length(bytes: 260 max).
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_Transmit(uint8_t * pTxBuffer, const uint16_t wTxLength);

/**
 * This function helps to perform Multiple Reception for the Single Transmitted packet.
 *
 * @param[out] ppRxBuffer Pointer to pointer which the HAL will point to the HAL Receive Buffer .
 * @param[out] pRxParams Receive Status data structure. \ref phhalRf_RxParams_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @maskedret #PH_ERR_IO_TIMEOUT Transmission does not completed.
 * @maskedret #PH_ERR_ABORTED User Aborted the transmission..
 * @maskedret #PH_ERR_IO_TIMEOUT Reception does not happen.
 * @maskedret #PH_ERR_BUFFER_OVERFLOW Rx buffer overflow
 * @maskedret #PH_ERR_INTEGRITY_ERROR Invalid packet received.
 * @maskedret #PH_ERR_PROTOCOL_ERROR Invalid response received.
 * @maskedret #PH_ERR_COLLISION_ERROR Collision detected.
 * @maskedret #PH_ERR_LENGTH_ERROR Length error occurred.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_Receive(uint8_t ** ppRxBuffer, phhalRf_RxParams_t * pRxParams);

/**
 * Utility wrapper of phhalRf_PCD_Exchg to send only EOF
 *
 * @param[out] pRxBuffer Receive Buffer(DWORD aligned).
 * @param[out] pRxParams Receive Status data structure \ref phhalRf_RxParams_t
 *
 * @return Same as @ref phhalRf_PCD_Exchg
 *
 */
phStatus_t phhalRf_PCD_Exchg_TxOnlyEOF(const uint8_t * pRxBuffer, phhalRf_RxParams_t * pRxParams);
/**
 * This function return the Internal Rf Field Status.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Internal Field ON
 * @maskedret #PH_ERR_RF_ERROR Internal Field OFF.
 */
phStatus_t phhalRf_GetIntFieldStatus(void);

/**
 * This function helps to perform Multiple Reception for the Single Transmitted packet.
 *
 * @param[in] pTxBuffer Transmit Buffer(DWORD aligned).
 * @param[in] wTxLength Transmit Length(bytes: 260 max).
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_PCD_Exchg_RxOnlySOF(
    const uint8_t * pTxBuffer,
    const uint16_t wTxLength);

/**
 * This function will reset the Target mode.
 *
 * @param[in] eCommMode Communication Mode in which the target is. \ref phhalRf_CommMode_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_ResetTarget(phhalRf_CommMode_t eCommMode);


/**
 * This function will reset the Initiator mode.
 *
 * @param[in] eCommMode Communication Mode in which the Initiator is. \ref phhalRf_CommMode_t
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS  Operation successful.
 * @retval Other Depends on the underlying components.
 */
phStatus_t phhalRf_ResetInitiator(phhalRf_CommMode_t eCommMode);

/**
 * @brief This function registers the call back function to be called from the
 * ISR on the specified interrupts.
 * @param[in] pCallBackFunc Function to be called from the ISR.
 * @param[in] dwInterrupts Interrupt bits on which the call back function shall be called.
 * @return Status of the operation.
 * @retval    #PH_ERR_SUCCESS Call back registered is successful
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been sent correctly
 */
phStatus_t phhalRf_RegCallBack(pphhalRf_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts);

/**
 *
 * This function is used to perform low power card detection before actual polling.
 *
 * The duration of the LPCD is controlled by the application using the EEPROM parameter.
 * This duration is effective only when DPC is disabled.
 *
 * See dwLCPDDurations parameter of \ref phCfg_EE_HW_RfInitUserEE::dwLCPDDurations
 *
 * It is the responsibility of the application (customer) to provide proper AGC reference
 * value and threshold for card detection based on respective system design. If the customer
 * passes 0xFFFF for the AGC reference value or for threshold, the HAL retrieves the value
 * from the EEPROM.
 *
 * @param[in] dwLPCDThreshold The Threshold for LPCD.
 * @param[in] dwLPCDRefValue The reference value for LPCD.
 *                           When DPC is enabled this value contains the Gear also.
 *                           (((DPC_GEAR & 0x0F) << 0x0A) | (AGC_VALUE & 0x3FF))
 *                           When DPC is disabled, this value have only Agc reference value.
 * @param[out] pdwLPCDAgcValue Current Agc Value.
 *                           When DPC is enabled this value contains the Gear also.
 *                           (((DPC_GEAR & 0x0F) << 0x0A) | (AGC_VALUE & 0x3FF))
 *                           When DPC is disabled, this value have only Agc value.
 *
 * @return Status of the operation
 * @retval #PH_ERR_SUCCESS LPCD is success. Something is detected in the proximity.
 * @maskedret #PH_ERR_FAILED Nothing is detected in the proximity.
 */
phStatus_t phhalRf_LPCD(uint32_t dwLPCDThreshold, uint32_t dwLPCDRefValue, uint32_t * pdwLPCDAgcValue);

/**
 * \brief : This function will wait for any event generated in Rf Hal
 * The user has to enable the IRQ using SetConfig before calling this function.
 *
 * @param[in] dwTimeOut Timeout for the events.
 */
phStatus_t phhalRf_HalEventWaitAny(uint32_t dwTimeOut);

/**
 * \brief : This function will consume all events generated in Rf Hal
 * Before calling the function \ref phhalRf_HalEventWaitAny call this function to consume all the events.
 */
phStatus_t phhalRf_HalEventConsumeAll(void);

/**
 * \brief : This function is used to get the AGC Value
 */
uint32_t phhalRf_GetAgcValue(void);

/**
 * \brief : This function is used for getting the version information
 *
 */
phStatus_t phhalRf_GetVersion(
    uint8_t *pVersionString,
    uint16_t *pVersionStringLen,
    uint16_t *pMajor,
    uint8_t *pMinor,
    uint16_t *pDev);

/**
 * RF HAL ISR Function
 *
 * This is the actual CLIF NVIC ISR handler that would be called from the IVT.
 * CLIF NVIC needs to be enabled exclusively to make the RF HAL to work
 */
void RF_IRQHandler(void);

#endif /* NXPBUILD__PHHAL_RF */
#endif /* PHHAL_RF_H */

/** @} */
