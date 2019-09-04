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
 * <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-04 16:39:24 +0530 (Mon, 04 May 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12183 $ (v05.19.00)
 */

#ifndef PHEXMAIN_MIFAREULTRALIGHTC_H
#define PHEXMAIN_MIFAREULTRALIGHTC_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phacDiscLoop.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/* define Key Store constants */
#define PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYENTRIES        3
#define PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KEYVERSIONPAIRS   1
#define PH_EXMAIN_MIFAREULTRALIGHTC_NUMBER_OF_KUCENTRIES        1
#define PH_EXMAIN_MIFAREULTRALIGHTC_BLOCK_SIZE                  16

/* define MIFARE(R) UltraLightC Keys */
#define PH_EXMAIN_MIFAREULTRALIGHTC_KEY     (const char*)       \
                                            ("\x49\x45\x4D\x4B\x41\x45\x52\x42\x21\x4E\x41\x43\x55\x4F\x59\x46")

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
#define PH_EXMAIN_MIFAREULTRALIGHTC_SAK      0x00
/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExMain_MiFareUltraLightC(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams);

#endif /* PHEXMAIN_MIFAREULTRALIGHTC_H */
