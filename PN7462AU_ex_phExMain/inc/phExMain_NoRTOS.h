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
 * phExMain_NoRtos.h : header file for non rtos of phExMain
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-10 11:04:42 +0530 (Thu, 10 Sep 2015) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 15215 $ (v05.19.00)
 */

#ifndef PHEXMAIN_NORTOS_H
#define PHEXMAIN_NORTOS_H
#ifdef PHFL_HALAPI_NO_RTOS

#include "phacDiscLoop.h"

#define PH_EXMAIN_NORTOS_CLIF_RXBUFSIZE 256

#endif/*close PHFL_HALAPI_NO_RTOS*/

#endif /* PHEXMAIN_NORTOS_H */
