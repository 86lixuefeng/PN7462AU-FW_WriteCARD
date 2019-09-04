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
 * $Date: 2016-06-23 13:52:29 +0530 (Thu, 23 Jun 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18502 $ (v05.19.00)
 */

#ifndef PHEXRF_B_H
#define PHEXRF_B_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExRf_B_106_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
phStatus_t phExRf_B_212_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
phStatus_t phExRf_B_424_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
phStatus_t phExRf_B_848_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
phStatus_t phExRf_B_Prime_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);

#endif /* PHEXRF_B_H */