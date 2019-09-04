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
 * phExEMVCo Application specific Compile time Build Configuration that requires CT, CLIF, PALS and utility HALS.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */

#ifndef APP_NXP_BUILD_H
#define APP_NXP_BUILD_H


/****************************************************************************************
 * HAL COMPONENTS
 ***************************************************************************************/
#define NXPBUILD__PHHAL_CLKGEN
#define NXPBUILD__PHHAL_CT
#define NXPBUILD__PHHAL_PCR
#define NXPBUILD__PHHAL_PMU
#define NXPBUILD__PHHAL_RNG
#define NXPBUILD__PHPAL_CT
#define NXPBUILD__PHHAL_TIMER
#define NXPBUILD__PHHAL_RF
#define NXPBUILD__PHHAL_LED
#define NXPBUILD__CFG_FROM_EE
#define NXPBUILD__PHHAL_HIF

#define NXPBUILD__CLIF_RM
#define NXPBUILD__PHHAL_RF_65X65

#ifdef NXPBUILD__PHHAL_RF_65X65   /* Enable the PCD shaping and TXOCP for 65x65 only */

#define NXPBUILD__PHHAL_RF_DPC                                    /**< Tx Over Current Protection Feature. */
#define NXPBUILD__PHHAL_RF_PCD_SHAPING                                    /**< PCD Shaping Feature. */

#endif

#define NXPBUILD__PHHAL_RF_EMD_2_5

#define NXPBUILD__PHHAL_HW_DUTHW                                  /**< Generic DUT HAL */
#define NXPBUILD__PHHAL_HW_PN7462AU                                /**< PN7462AU HAL definition */

#define NXPBUILD__PHAC_DISCLOOP_SW               /**< Discovery loop software componenet */
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS
#define NXPBUILD__PHAL_T1T_SW
#define NXPBUILD__PHPAL_I14443P3A_SW            /**< Software PAL ISO 14443-3A definition */
#define NXPBUILD__PHPAL_I14443P3B_SW            /**< Software PAL ISO 14443-3B definition */
#define NXPBUILD__PHPAL_I14443P4_SW             /**< Software PAL ISO 14443-4 definition */
#define NXPBUILD__PHPAL_I14443P4A_SW            /**< Software PAL ISO 14443-4A definition */


/* #define PHFL_ENABLE_STANDBY */

//#define PHFL_ENABLE_TVDD_3V_MON     /* Enable if TVDD 3V Monitor is selected */

#ifdef PH_OSAL_FREERTOS
#   define PHFL_HALAPI_WITH_RTOS
#endif

#ifdef PH_OSAL_NULLOS
#   define PHFL_HALAPI_NO_RTOS
#endif

#define NXPBUILD__PH_PN7462AU
#define NXPBUILD__PH_PLATFORM
#define NXPBUILD__PH_KEYSTORE_SW                /**< Software KeyStore */
/** @}
* end of App_NxpBuild
*/
#endif /* APP_NXP_BUILD_H */
