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
 * phExRfPCDA.h: Defines and declarations for Poll and detect Type A Tags
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-08 18:04:02 +0530 (Tue, 08 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 16888 $ (v05.19.00)
 */

#ifndef PHEXRFPCDA_A_H
#define PHEXRFPCDA_A_H


/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phExRfPCDA.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXRFPCDA_A_UID_FRAME_BIT0                 (6)
#define PH_EXRFPCDA_A_UID_FRAME_BIT1                 (7)

#define PH_EXRFPCDA_A_UID_FRAME_BIT0_MASK            \
    (1 << PH_EXRFPCDA_A_UID_FRAME_BIT0)
#define PH_EXRFPCDA_A_UID_FRAME_BIT1_MASK            \
    (1 << PH_EXRFPCDA_A_UID_FRAME_BIT1)

#define PH_EXRFPCDA_A_MIFARECLASSIC_1K_SAK      0x08
#define PH_EXRFPCDA_A_MIFARECLASSIC_4K_SAK      0x18
#define PH_EXRFPCDA_A_NFCDEP_SUPPORT            0x40

#define PH_EXRFPCDA_A_TAGTYPE    0x60
#define PH_EXRFPCDA_A_TYPE2TAG   0x00
#define PH_EXRFPCDA_A_TYPE4TAG   0x20

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

/** Poll for Type A Cards
 *
 *
 * @param pTxBuffer Application allocated Tx Buffer
 * @param pRxBuffer Application allocated Rx Buffer
 *
 * @return Status of Polling / Lower layer status
 */
phStatus_t phExRfPCDA_A_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);

#endif /* PH_EXRFPCDA_A_H */
