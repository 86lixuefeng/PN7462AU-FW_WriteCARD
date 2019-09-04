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
 * phExRf Application specific Compile time Build Configuration that requires CLIF and utility HALS.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-19 17:33:22 +0530 (Mon, 19 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * r02.06.02_Internal $Revision: 18665 $ (v05.19.00)
 */

#ifndef APP_NXP_BUILD_H
#define APP_NXP_BUILD_H


/****************************************************************************************
 * HAL COMPONENTS
 ***************************************************************************************/
#define NXPBUILD__PHHAL_CLKGEN
#define NXPBUILD__PHHAL_PCR
#define NXPBUILD__PHHAL_PMU
#define NXPBUILD__PHHAL_RNG
#define NXPBUILD__PHHAL_TIMER
#define NXPBUILD__PHHAL_RF
#define NXPBUILD__PHHAL_LED
#define NXPBUILD__CFG_FROM_EE

#define NXPBUILD__CLIF_RM
//#define NXPBUILD__CLIF_CM
//#define NXPBUILD__CLIF_ACTIVE_TARGET
//#define NXPBUILD__CLIF_ACTIVE_INITIATOR

/* #define NXPBUILD__PHHAL_HW_PICC_B */                                   /**< PICC B Support for PN7462AU */
#define NXPBUILD__PHHAL_RF_DPC                                    /**< Tx Over Current Protection Feature. */
#define NXPBUILD__PHHAL_RF_PCD_SHAPING                                    /**< PCD Shaping Feature. */

/* #define NXPBUILD__PHHAL_HW_DUTHW */                                  /**< Generic DUT HAL */
/* #define NXPBUILD__PHHAL_HW_PN7462AU */                                /**< PN7462AU HAL definition */

#define PHFL_HALAPI_NO_RTOS

/* #define PHFL_ENABLE_STANDBY */
#define NXPBUILD__PH_OSAL_NULLOS /* Software OSAL for Free Rtos */
#define NXPBUILD__PH_OSAL_STUB


/* #define PHFL_ENABLE_EXTERNAL_TVDD_3V_MON */  /* Enable if TVDD 3V Monitor is selected */
#endif /* APP_NXP_BUILD_H */
