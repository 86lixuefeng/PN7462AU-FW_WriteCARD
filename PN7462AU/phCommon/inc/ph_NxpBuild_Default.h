/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
 * Default selection of components
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 18525 $ (v04.13.00)
 * $Date: 2016-07-01 16:31:15 +0530 (Fri, 01 Jul 2016) $
 *
 */

#ifndef PH_NXPBUILD_DEFAULT_H
#define PH_NXPBUILD_DEFAULT_H

#if ! defined(PHFL_HALAPI_WITH_RTOS) && ! defined(PHFL_HALAPI_NO_RTOS)
/* Let's fall back to either With Or Without RTOS By default if
 * Build System has not defined this by now */
#   define PHFL_HALAPI_WITH_RTOS
#endif

/****************************************************************************************
 * HAL COMPONENTS
 ***************************************************************************************/
#define NXPBUILD__PHHAL_CLKGEN
#define NXPBUILD__PHHAL_CRC
#define NXPBUILD__PHHAL_CT
#define NXPBUILD__PHHAL_EEPROM
#define NXPBUILD__PHHAL_FLASH
#define NXPBUILD__PHHAL_HIF
/* #define NXPBUILD__PHHAL_USB */

#define NXPBUILD__PHHAL_I2CM
/** User configurable macro to enable multi transaction support by queueing. */
#define NXPBUILD__PHHAL_I2CM_MULTI_TRANSACTION

#define NXPBUILD__PHHAL_PCR
#define NXPBUILD__PHHAL_PMU
#define NXPBUILD__PHHAL_RNG
#define NXPBUILD__PHHAL_SPIM
#define NXPBUILD__PHPAL_CT
#define NXPBUILD__PHHAL_TIMER
#define NXPBUILD__PHHAL_WDT
#define NXPBUILD__PHHAL_RF
#define NXPBUILD__PHHAL_LED
#define NXPBUILD__PHHAL_SYNCCT
#define NXPBUILD__CFG_FROM_EE
/* #define NXPBUILD__CFG_FROM_PF */

/* #define PHHAL_SPIM_NSS_ENABLED */

/** MicroController Platform component code. */
#define NXPBUILD__PH_PLATFORM

#ifdef NXPBUILD__PHHAL_RF
    #define NXPBUILD__CLIF_RM
    #define NXPBUILD__CLIF_CM
    #define NXPBUILD__CLIF_ACTIVE_TARGET
    #define NXPBUILD__CLIF_ACTIVE_INITIATOR

    /* #define NXPBUILD__PHHAL_HW_PICC_B */                                   /**< PICC B Support for PN7462AU */
    #define NXPBUILD__PHHAL_RF_DPC                                         /**< DPC Feature. */
    #define NXPBUILD__PHHAL_RF_PCD_SHAPING                                    /**< PCD Shaping Feature. */
    #define NXPBUILD__PHHAL_RF_APC                                            /**< Auto - Power Control Feature. */
    #define NXPBUILD__PHHAL_RF_DPLL                                           /**< .*/
    #define NXPBUILD__PHHAL_RF_EMD_2_5                                      /**< EMD according to 2.5 spec. */
#endif /* NXPBUILD__PHHAL_RF */

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
#define NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS  /**< Detect TypeV Tags */
#define NXPBUILD__PHAL_FELICA_SW                /**< Software AL Felica */
#define NXPBUILD__PHAL_ICODE_SW                 /**< Software AL ICODE */
#define PHAL_ICODE_ENABLE_CHAINING              /**< ICode Chaining is implemented */
#define NXPBUILD__PHAL_I18000P3M3_SW             /**< Software AL I18000p3m3 */
#define NXPBUILD__PHAL_MFC_SW                   /**< Software MIFARE Classic */
#define NXPBUILD__PHAL_MFDF_SW                   /**< Software MIFARE DESFire */
#define NXPBUILD__PHAL_MFUL_SW                  /**< Software MIFARE Ultralight */
#define NXPBUILD__PHAL_T1T_SW
#define NXPBUILD__PHCE_T4T_SW
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
#define NXPBUILD__PHAL_TOP_SW
#define NXPBUILD__PHLN_LLCP_SW                  /**< Software LLCP  */
#define NXPBUILD__PHNP_SNEP_SW                  /**< Software SNEP  */

/* #define NXPBUILD__PH_CRYPTOSYM */
/* #define NXPBUILD__PH_CRYPTORNG */

/* #define PHFL_ENABLE_STANDBY */

#ifdef PHFL_HALAPI_WITH_RTOS
#   define NXPBUILD__PH_OSAL_FREERTOS /* Software OSAL for Free Rtos */
#   define NXPBUILD__PH_OSAL_STUB
#else
#   define NXPBUILD__PH_OSAL_NULLOS /* Software OSAL for Free Rtos */
#   define NXPBUILD__PH_OSAL_STUB
#endif

/** @}
* end of ph_NxpBuild
*/


#endif /* PH_NXPBUILD_H */
