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
 * phExRf.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-06-23 13:52:29 +0530 (Thu, 23 Jun 2016) $
 * $Author: Purnank G (ing05193) $
 * r02.06.02_Internal $Revision: 18502 $ (v05.19.00)
 */

#ifndef PHEXRF_H
#define PHEXRF_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"


/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PH_EXRF_CFG_SWD_DNLD_PIN                     8
#define PH_EXRF_ENABLE_LPCD                          0

/* custom Error codes */
#define PH_ERR_EXRF_CARD_NOT_FOUND                      0x81

#define PH_EXRF_TX_BUFSIZE            256
#define PH_EXRF_RX_BUFSIZE            256

#define PH_CONTINUE_ON_FAILURE(STATUS)         {if ((STATUS) != PH_ERR_SUCCESS) {continue;}}


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */



#endif /* PHEXRF_H */
