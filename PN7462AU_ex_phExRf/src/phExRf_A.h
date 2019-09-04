/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * test.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-04-21 11:46:50 +0530 (Tue, 21 Apr 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 11652 $ (v05.19.00)
 */

#ifndef PHEXRF_A_H
#define PHEXRF_A_H


/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PHEXRF_A_UID_FRAME_BIT0                 (6)
#define PHEXRF_A_UID_FRAME_BIT1                 (7)

#define PHEXRF_A_UID_FRAME_BIT0_MASK            (1 << 6)
#define PHEXRF_A_UID_FRAME_BIT1_MASK            (1 << 7)

#define PHEXRF_A_MIFARECLASSIC_1K_SAK      0x08
#define PHEXRF_A_MIFARECLASSIC_4K_SAK      0x18
#define PHEXRF_A_NFCDEP_SUPPORT            0x40

#define PHEXRF_A_TAGTYPE    0x60
#define PHEXRF_A_TYPE2TAG   0x00
#define PHEXRF_A_TYPE4TAG   0x20

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

phStatus_t phExRf_A_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);

phStatus_t phExRf_A_CM(phhalRf_TargetActivatedParam_t * peTargetParam, uint8_t *pTxBuffer, uint8_t *pRxBuffer,
    uint16_t wMaxLen);
#endif /* PHEXRF_A_H */
