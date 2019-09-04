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
 * $Date: 2016-09-20 12:38:43 +0530 (Tue, 20 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18667 $ (v05.19.00)
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
#define NXPBUILD__PHHAL_SYNCCT
#define NXPBUILD__CFG_FROM_EE
/* #define NXPBUILD__CFG_FROM_PF */

/** MicroController Platform component code. */
#define NXPBUILD__PH_PLATFORM

#define NXPBUILD__CLIF_RM
/* #define NXPBUILD__CLIF_CM */
/* #define NXPBUILD__CLIF_ACTIVE_TARGET */
/* #define NXPBUILD__CLIF_ACTIVE_INITIATOR */

#define NXPBUILD__PHHAL_RF_65X65

#ifdef NXPBUILD__PHHAL_RF_65X65   /* Enable the PCD shaping and DPC for 65x65 only */

#define NXPBUILD__PHHAL_RF_DPC          /**< Tx Over Current Protection Feature. */
#define NXPBUILD__PHHAL_RF_PCD_SHAPING     /**< PCD Shaping Feature. */

#endif /* NXPBUILD__PHHAL_RF_65X65 */

#define NXPBUILD__PHHAL_HW_DUTHW                                  /**< Generic DUT HAL */
#define NXPBUILD__PHHAL_HW_PN7462AU                                /**< PN7462AU HAL definition */


#define NXPBUILD__PH_KEYSTORE_SW                /**< Software KeyStore */
#define NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS /**< Detect 18000p3m3 Tags */
#define NXPBUILD__PHAC_DISCLOOP_SW               /**< Discovery loop software componenet */
#define NXPBUILD__PHAC_DISCLOOP_TARGET
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_JEWEL_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEB_BPRIME_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
#define NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS        /**< Detect TypeV Tags */
#define NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE
#define NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE
#define NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE
#define NXPBUILD__PHAL_FELICA_SW                /**< Software AL Felica */
#define NXPBUILD__PHAL_ICODE_SW                 /**< Software AL ICODE */
#define NXPBUILD__PHAL_I18000P3M3_SW             /**< Software AL I18000p3m3 */
#define NXPBUILD__PHAL_MFC_SW                   /**< Software MIFARE Classic */
#define NXPBUILD__PHAL_MFDF_SW                   /**< Software MIFARE DESFire */
#define NXPBUILD__PHAL_MFUL_SW                  /**< Software MIFARE Ultralight */
#define NXPBUILD__PHAL_T1T_SW                   /**< Software Type 1 tag Component */
#define NXPBUILD__PHAL_TOP_SW                   /**< Software Tag Operation Component*/
#define NXPBUILD__PHCE_T4T_SW                   /**< Software Card Emulation definition */
#define NXPBUILD__PHPAL_FELICA_SW               /**< Software PAL Felica */
#define NXPBUILD__PHPAL_I14443P3A_SW            /**< Software PAL ISO 14443-3A definition */
#define NXPBUILD__PHPAL_I14443P3B_SW            /**< Software PAL ISO 14443-3B definition */
#define NXPBUILD__PHPAL_I14443P4_SW             /**< Software PAL ISO 14443-4 definition */
#define NXPBUILD__PHPAL_I14443P4A_SW            /**< Software PAL ISO 14443-4A definition */
#define NXPBUILD__PHPAL_I14443P4MC_SW              /**< Software implementation of 14443-4 Card mode */
#define NXPBUILD__PHPAL_I18000P3M3_SW
#define NXPBUILD__PHPAL_I18092MPI_SW             /**< Software PAL ISO 18092 definition */
#define NXPBUILD__PHPAL_I18092MT_SW    /**< Software implementation of ISO 18092 target mode */
#define NXPBUILD__PHPAL_MIFARE_SW                                 /**< Software PAL MIFARE */
#define NXPBUILD__PHPAL_SLI15693_SW              /**< Software PAL Sli 15693 */

#define configTOTAL_HEAP_SIZE 6000

#ifdef PH_OSAL_FREERTOS
#   define PHFL_HALAPI_WITH_RTOS
#endif

#ifdef PH_OSAL_NULLOS
#   define PHFL_HALAPI_NO_RTOS
#endif

/* #define PHFL_ENABLE_STANDBY */
/* #define PHFL_ENABLE_EXTERNAL_TVDD_3V_MON */  /* Enable if TVDD 3V Monitor is selected */
#define NXPRDLIB_REM_GEN_INTFS

/* NxpRdLib OSAL configurations are in project defined symbols -D.
 * */

#define NXPBUILD__PH_PN7462AU
#endif /* APP_NXP_BUILD_H */
