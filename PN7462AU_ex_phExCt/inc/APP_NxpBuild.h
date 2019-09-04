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
 * phExCt Application specific Compile time Build Configuration which requires mainly CT and other utility HALS.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-06-25 11:17:24 +0530 (Thu, 25 Jun 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 13199 $ (v05.19.00)
 */

#ifndef APP_NXP_BUILD_H
#define APP_NXP_BUILD_H

/****************************************************************************************
 * HAL COMPONENTS
 ***************************************************************************************/
#define NXPBUILD__PHHAL_CT
#define NXPBUILD__PHHAL_PCR
#define NXPBUILD__PHHAL_PMU
#define NXPBUILD__CFG_FROM_EE
/* #define NXPBUILD__CFG_FROM_PF */
#define NXPBUILD__PHHAL_TIMER
#define NXPBUILD__PHHAL_LED
#define NXPBUILD__PHHAL_CLKGEN


/* #define PHFL_ENABLE_STANDBY */

#define PHFL_HALAPI_NO_RTOS


/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */


#endif /* APP_NXP_BUILD_H */
