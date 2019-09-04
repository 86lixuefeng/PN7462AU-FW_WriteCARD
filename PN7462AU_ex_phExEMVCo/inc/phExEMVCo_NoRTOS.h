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
 * phExEMVCo_NoRtos.h : header file for non rtos of phExEMVCo
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-25 12:56:17 +0530 (Mon, 25 May 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12705 $ (v05.19.00)
 */

#ifndef PHEXEMVCO_NORTOS_H
#define PHEXEMVCO_NORTOS_H
#ifdef PHFL_HALAPI_NO_RTOS

#include "phacDiscLoop.h"

#define PH_EXEMVCO_NORTOS_CLIF_RXBUFSIZE 256

void phExEMVCo_NoRTOS_Perform_CardPolling();
void phExEMVCo_NoRTOS_Polling();
#endif/*close PHFL_HALAPI_NO_RTOS*/

#endif /* PHEXEMVCO_NORTOS_H */
