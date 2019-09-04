/*******************************************************************************
 *
 *                Copyright (C), NXP Semiconductors
 *
 *                   (C) NXP Semiconductors
 *     All rights are reserved. Reproduction in whole or in part is
 *    prohibited without the written consent of the copyright owner.
 * NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                        arising from its use.
 *
 ********************************************************************************/
/********************************************************************************
 *
 * \file phSysSv_Data.h
 *
 * Prototype of CLIF System Services API.
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 17268 $ (v04.13.00)
 * $Date: 2016-01-09 17:26:06 +0530 (Sat, 09 Jan 2016) $
 *
 *******************************************************************************/

#ifndef PHSYSTEMSERVICES_DATA_H_
#define PHSYSTEMSERVICES_DATA_H_

/** \addtogroup phSysSv_CLIF
 *
 * \ingroup RomSysSvc
 *
 * \brief Prototype of the CLIF System Services API
 *
 * This module implements CLIF interface in ROM.
 * This module is not designed for multi user/process environment.
 *
 * @{
 */

/*********************************************************************************************************************/
/*   INCLUDES                                                                                                        */
/*********************************************************************************************************************/
#include <ph_Datatypes.h>
/*********************************************************************************************************************/
/*   GLOBAL DEFINES                                                                                                  */
/*********************************************************************************************************************/

/**
 * SIZE Definitions for the CLIF RAM Read-only region
 * The total size of the CMA region is 32bytes and would be used by #phhalSysSer_CLIF_CardModeActivate function.
 */
#define PH_SYSSV_CLIF_USER_RO_REGION_MAX                        8

/**
 * RAM Address of CLIF User Read-only Region for Card Mode Activate
 * This is an absolute Address that would be later used for comparison of the TX and RX Buffer pointers in
 * #phhalSysSer_CLIF_Config_TxRxBuffer function.
 *
 */
#define PH_SYSSV_CLIF_USER_RO_RAM_ADDRESS                       (PH_RAM_MEM_START)

/**
 * Minimum RAM Address for CLIF Buffer
 * This is an absolute Address that would be used for comparison of the TX and RX Buffer pointers in
 * #phhalSysSer_CLIF_Config_TxRxBuffer function.
 *
 */
#define PH_SYSSV_CLIF_USER_BMA_MIN_RAM_ADDRESS                  (PH_SYSSV_CLIF_USER_RO_RAM_ADDRESS)

/**
 * Maximum RAM Address for CLIF Buffer
 * This is an absolute Address that would be later used for comparison of the TX and RX Buffer pointers in
 * #phhalSysSer_CLIF_Config_TxRxBuffer function.
 *
 */
#define PH_SYSSV_CLIF_USER_BMA_MAX_RAM_ADDRESS                  (PH_RAM_START_ADDRESS + 0x00001FFFUL)

/**
 * CLIF Maximum value of Mifare Classic UID Size supported
 * This is a limit for UID size in phhalSysSer_CLIF_MF_Authenticate_Parameters_t.ab_Uid.
 * Currently only 7 byte UID is the
 * maximum supported for Mifare Classic cards.
 */
#define PH_SYSSV_CLIF_MAX_MIFARE_UID_SIZE                        7

/*********************************************************************************************************************/
/*   GLOBAL DATATYPES                                                                                                */
/*********************************************************************************************************************/

/**
 * CLIF Mifare Authentication Steps
 * This is used to control the Mifare Authentication sub-module in #phhalSysSer_CLIF_MF_Authenticate function.
 * It defines the various stages in the Mifare Crypto Authentication process.
 */
typedef enum phSysSv_Clif_MF_Authenticate_Step {
    /** \b 0x00 */ PH_SYSSV_CLIF_MFAUTH_DISABLE = 0,    /**< Disable the Mifare Authentication sub-module completely */
    /** \b 0x01 */ PH_SYSSV_CLIF_MFAUTH_LOADKEY,        /**< Initialize the Mifare Authentication sub-module.
                                                @note This step needs to be used before any Mifare Authentication
                                                command communication is started */
    /** \b 0x02 */ PH_SYSSV_CLIF_MFAUTH_PREPARE,        /**< Configure the Mifare Authentication sub-module to begin Mifare Crypto
                                                Encryption from the subsequent communication and to provide the
                                                @em RA , @em RB' and @em RB'' values used for subsequent steps in
                                                Mifare Authentication Process*/
    /** \b 0x03 */ PH_SYSSV_CLIF_MFAUTH_MAX,             /**< Comparison Maximum for this Enum type */
    PH_SYSSV_CLIF_MFAUTH_U32 =0x7FFFFFFF  /**< Ensure 32 bit width of this register */
}phSysSv_Clif_MF_Authenticate_Step_t;

/**
 * TX RX Buffer Settings
 * This provides the options to set either of the TX RX Buffers or Both at the same time.
 * This is used in phhalSysSer_CLIF_TxRxBuffer_Parameters_t.wTxRxBufferConfig for buffer configuration function
 * #phhalSysSer_CLIF_Config_TxRxBuffer
 * @note SC1058 - Adding feature to optionally set either TX or RX or Both
 */
typedef enum phSysSv_Clif_TxRxBuffer_Config {
    /** \b 0x00 */ PH_SYSSV_CLIF_TXRXBUF_CFG_SETBT = 0,  /**< Configure both TX Buffer and RX Buffer */
    /** \b 0x01 */ PH_SYSSV_CLIF_TXRXBUF_CFG_SETTX,    /**< Configure TX Buffer alone */
    /** \b 0x02 */ PH_SYSSV_CLIF_TXRXBUF_CFG_SETRX,    /**< Configure RX Buffer alone */
    /** \b 0x03 */ PH_SYSSV_CLIF_TXRXBUF_CFG_MAX           /**< Comparison Maximum for this Enum type */
}phSysSv_Clif_TxRxBuffer_Config_t;

/**
 * System Services CLIF Mifare Authentication Sub-Module Control Data Structure
 * This is used to configure the Mifare Authentication Sub-Module and is used in various steps of the
 * Mifare Authentication process to provide required data for transactions.
 * This is used as parameter in #phhalSysSer_CLIF_MF_Authenticate function.
 */
typedef struct phSysSv_Clif_MF_Authenticate_Parameters {
    uint8_t ab_Key[6];   /**< Mifare Key                   6 Bytes */
    uint8_t ab_Uid[7];   /**< Mifare UID                   7 Bytes */
    uint8_t buidsz;      /**< Mifare UID Size              1 Bytes */
    uint8_t _bReserved;  /**< Dummy Padding                1 Bytes */
    uint8_t _bReserved1; /**< Dummy Padding                1 Bytes */
    uint32_t dwRA;       /**< RA UnEncrypted 32 bit data   4 Bytes */
    uint32_t dwRB;       /**< RB UnEncrypted 32 bit data   4 Bytes */
    uint32_t dwRB1;      /**< RB' UnEncrypted 32 bit data  4 Bytes */
    uint32_t dwRB11;     /**< RB'' UnEncrypted 32 bit data 4 Bytes */
} phSysSv_Clif_MF_Authenticate_Parameters_t;

/**
 * System Services CLIF Transmit and Receive Buffer Configuration Data Structure
 * This is used to specify the TX and RX buffer locations, and the Maximum RX data capacity. It also provides optional
 * TX and/or RX Buffer configuration as needed by the Application.
 * This is used as parameter in #phhalSysSer_CLIF_Config_TxRxBuffer function.
 * @warning See more from #phhalSysSer_CLIF_Config_TxRxBuffer on the caution observed for setting the TX RX buffers
 * @note SC1058 - Adding feature to optionally set either TX or RX or Both
 * Adjusting size to accommodate new field in same space
 */
typedef PH_PACK_STRUCT_BEGIN struct phSysSv_Clif_TxRxBuffer_Parameters {
    void *pTxBuffer;           /**< TX Buffer Pointer                                        4 Bytes */
    void *pRxBuffer;           /**< RX Buffer Pointer                                        4 Bytes */
    uint16_t wRxBuffer_max;    /**< RX Buffer Maximum Size Absolute Value but DWORD Aligned  2 Bytes */
    uint16_t wTxRxBufferConfig;/**< TX RX Buffer Initialization option                       2 Bytes */
} PH_PACK_STRUCT_END phSysSv_Clif_TxRxBuffer_Parameters_t;

/**
 * System Services CLIF Card Mode Activate Sub-Module Control Data Structure
 *
 * This is used to configure the Card Mode Activate functionality and also obtain the internal states of the Activation
 * sub-module.
 * This is used as parameter in #phhalSysSer_CLIF_CardModeActivate function.
 *
 * @note SC1053 - Can't Initialize the TX/RX buffer if Tranceive is Enabled
 */
typedef PH_PACK_STRUCT_BEGIN struct phSysSv_Clif_CardMode_TypeA_Param {
    uint8_t bAtqa[2];      /**< ATQA                                                2 Bytes */
    uint8_t bSak;          /**< SAK                                                 1 Byte  */
    uint8_t bEntryState;   /**< CMA Entry State - Set or Get depending on command   1 Byte  */
    uint8_t bUid[3];       /**< UID 3Byte UID[0]=08 for Random UID                  3 Bytes */
    uint8_t bCurrentState; /**< CMA Current State                                   1 Byte  */
} PH_PACK_STRUCT_END phSysSv_Clif_CardMode_TypeA_Param_t; /**< Typedef for phSysSv_Clif_CardMode_TypeA_Param */

/**
 * Set Up Base Addressed for Load Protocol.
 */
typedef PH_PACK_STRUCT_BEGIN struct phSysSv_Clif_LP_BaseAddr {
    /** Number of Entries for Transmit Load Protocol.*/
    uint8_t * pTxNumEntries;
    /** Tx Register Address to be configured in Load Protocol.*/
    uint8_t * pTxRegAddr;
    /** Register Values to be updated to respective registers for Tx Load Protocol.*/
    uint32_t * pTxRegValue;
    /** Mask Index to be applied on the respective Register. */
    uint8_t * pTxRegMaskIndex;
    /** Mask Value with which the register have to be masked */
    uint32_t * pTxRegMaskValue;
    /** Number of Entries for Receive Load Protocol.*/
    uint8_t * pRxNumEntries;
    /** Rx Register Address to be configured in Load Protocol.*/
    uint8_t * pRxRegAddr;
    /** Register Values to be updated to respective registers for Rx Load Protocol.*/
    uint32_t * pRxRegValue;
    /** Mask Index to be applied on the respective Register. */
    uint8_t * pRxRegMaskIndex;
    /** Mask Value with which the register have to be masked */
    uint32_t * pRxRegMaskValue;
} PH_PACK_STRUCT_END phSysSv_Clif_LP_BaseAddr_t, /**< CLIF Load Protocol entries */
phSysSv_Clif_LP_BaseAddr_I_t,  /**< CLIF Load Protocol entries for Initiator Mode  */
phSysSv_Clif_LP_BaseAddr_T_t;  /**< CLIF Load Protocol entries for Target Mode */


/* //////////////////////////////////////////////////////////////////////////////////////////////////////// */

/** @} */

#endif /* PHSYSTEMSERVICES_DATA_H_ */
