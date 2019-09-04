/*
 *
 *                Copyright (C), NXP Semiconductors
 *
 *                   (C) NXP Semiconductors 2014,2015
 *     All rights are reserved. Reproduction in whole or in part is
 *    prohibited without the written consent of the copyright owner.
 * NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                        arising from its use.
 *
*/


/** @file
 *
 * This file contains the implementation of T=0 protocol.
 *Project: PN7462AU
 *
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12183 $ (v04.13.00)
 * $Date: 2014-07-12
 *
 *History:
 *V0.1 |2014-07-20 | Created initial Draft
 *
 */

#ifndef PHPALCT_T0_H
#define PHPALCT_T0_H
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
 #include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHPAL_CT

/* ******************************************************************************************************************
 * Global Defines
 * ****************************************************************************************************************** */
/**
 * Macro for Class Byte in T=0 protocol header.
 */
#define PHPAL_CT_CLASS      0x00
/**
 * Macro for INS Byte in T=0 protocol header.
 */
#define PHPAL_CT_INS        0x01
/**
 * Macro for P1 Byte in T=0 protocol header.
 */
#define PHPAL_CT_P1         0x02
/**
 * Macro for P2 Byte in T=0 protocol header.
 */
#define PHPAL_CT_P2         0x03
/**
 * Macro for P3 Byte in T=0 protocol header.
 */
#define PHPAL_CT_P3         0x04
/**
 * Macro for maximum length INF.
 */
#define PHPAL_CT_MAX_LENGTH 256

/* ******************************************************************************************************************
 * Type Definitions
 * ****************************************************************************************************************** */

/* *******************************************************************************************************************
 * Extern Variables
 * ****************************************************************************************************************** */

/* *******************************************************************************************************************
 * Function Prototypes
 * ****************************************************************************************************************** */
/**
 * @brief This function transmits the Apdu in T=0 protocol and returns the response to the application.
 * @note  Pal transceive api calls this Api internally after selecting T=0 protocol.
 *
 * @param pbTransmitBuff(in) - Pointer to the transmit buffer
 * @param dwTransmitSize(in) - Size of the bytes to be transmitted
 * @param pbReceiveBuff(out) - Pointer to the receive buffer
 * @param dwReceiveSize(out) - Pointer to the receive buffer size
 * @return
 */
phStatus_t phpalCt_T0_Transcieve(
        uint8_t* pbTransmitBuff, uint32_t dwTransmitSize,uint8_t* pbReceiveBuff,  uint16_t* pwReceiveSize);

#endif /* NXPBUILD__PHPAL_CT */

#endif /* PHPALCT_T0_H */
