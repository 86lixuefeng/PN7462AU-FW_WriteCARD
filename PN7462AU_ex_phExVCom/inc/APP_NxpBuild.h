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
 * phExMain Application specific Compile time Build Configuration that requires CT, CLIF, PALS and utility HALS.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-10-20 10:18:10 +0530 (Thu, 20 Oct 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18736 $ (v05.19.00)
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
#define NXPBUILD__PHHAL_USB
/* #define NXPBUILD__CFG_FROM_PF */

/** MicroController Platform component code. */
#define NXPBUILD__PH_PLATFORM

#define NXPBUILD__CLIF_RM

#define NXPBUILD__PHHAL_RF_65X65

#ifdef NXPBUILD__PHHAL_RF_65X65   /* Enable the PCD shaping and DPC for 65x65 only */

#define NXPBUILD__PHHAL_RF_DPC          /**< Tx Over Current Protection Feature. */
#define NXPBUILD__PHHAL_RF_PCD_SHAPING     /**< PCD Shaping Feature. */

#endif /* NXPBUILD__PHHAL_RF_65X65 */

#define NXPBUILD__PHHAL_HW_DUTHW                    /**< Generic DUT HAL */
#define NXPBUILD__PHHAL_HW_PN7462AU                 /**< PN7462AU HAL definition */

//#define configTOTAL_HEAP_SIZE 5500

/* This example can be run only with RTOS */
#ifdef PH_OSAL_FREERTOS
#   define PHFL_HALAPI_WITH_RTOS
#endif

#ifdef PH_OSAL_NULLOS
#   define PHFL_HALAPI_NO_RTOS
#endif

/* #define PHFL_ENABLE_STANDBY */
/* #define PHFL_ENABLE_EXTERNAL_TVDD_3V_MON */      /* Enable if TVDD 3V Monitor is selected */
#define NXPRDLIB_REM_GEN_INTFS

#define NXPBUILD__PH_OSAL_FREERTOS               /* Software OSAL for Free Rtos */
#define NXPBUILD__PH_OSAL_STUB


#endif /* APP_NXP_BUILD_H */
