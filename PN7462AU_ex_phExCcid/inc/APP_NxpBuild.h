/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * Application specific Compile time Build Configuration
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-05 15:52:30 +0530 (Tue, 05 Jul 2016) $
 * $Author: nxp86397 $
 * $Revision: 3650 $ (v05.19.00)
 */

#ifndef APP_NXP_BUILD_H /* @TODO: Rename to Application name */
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
#define NXPBUILD__PHHAL_USB



#define NXPBUILD__PHHAL_HW_DUTHW                                  /**< Generic DUT HAL */
#define NXPBUILD__PHHAL_HW_PN7462AU                               /**< PN7462AU HAL definition */
#define NXPBUILD__PHHAL_RF_65X65
#ifdef NXPBUILD__PHHAL_RF_65X65                                       /* Enable the PCD shaping and TXOCP for 65x65 only */
    #define NXPBUILD__PHHAL_RF_DPC                                 /**< Tx Over Current Protection Feature. */
    #define NXPBUILD__PHHAL_RF_PCD_SHAPING                            /**< PCD Shaping Feature. */
#endif /* NXPBUILD__PHHAL_RF_65X65 */

#define NXPBUILD__PHPAL_I14443P3A_SW                                  /**< Software PAL ISO 14443-3A definition */
#define NXPBUILD__PHPAL_I14443P3B_SW                                  /**< Software PAL ISO 14443-3B definition */
#define NXPBUILD__PHPAL_I14443P4_SW                                   /**< Software PAL ISO 14443-4 definition */
#define NXPBUILD__PHPAL_I14443P4A_SW                                  /**< Software PAL ISO 14443-4A definition */
#define NXPBUILD__PHPAL_FELICA_SW                                     /**< Software PAL Felica */
#define NXPBUILD__PHPAL_I18000P3M3_SW
#define NXPBUILD__PHPAL_SLI15693_SW                                   /**< Software PAL Sli 15693 */
#define NXPBUILD__PHPAL_MIFARE_SW                                     /**< Software PAL MIFARE */

#define NXPBUILD__PH_KEYSTORE_SW                                      /**< Software KeyStore */

#define NXPBUILD__PHAC_DISCLOOP_SW                                    /**< Discovery loop software componenet */
#ifdef NXPBUILD__PHAC_DISCLOOP_SW
    #ifdef NXPBUILD__PHPAL_I14443P3A_SW
        #define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS
        #if defined(NXPBUILD__PHPAL_I14443P4A_SW) && defined(NXPBUILD__PHPAL_I14443P4_SW)
            #define NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
        #endif
    #endif /* NXPBUILD__PHPAL_I14443P3A_SW */
    #ifdef NXPBUILD__PHPAL_FELICA_SW
        #define NXPBUILD__PHAC_DISCLOOP_FELICA_TAGS
        #define NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    #endif /* NXPBUILD__PHPAL_FELICA_SW */
    #ifdef NXPBUILD__PHPAL_I14443P3B_SW
        #define NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS
        #ifdef NXPBUILD__PHPAL_I14443P4_SW
            #define NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS
        #endif
    #endif /* NXPBUILD__PHPAL_I14443P3B_SW */
    #ifdef NXPBUILD__PHPAL_SLI15693_SW
        #define NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    #endif /* NXPBUILD__PHPAL_SLI15693_SW */
    #ifdef NXPBUILD__PHPAL_I18000P3M3_SW
        #define NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    #endif /* NXPBUILD__PHPAL_I18000P3M3_SW */
#endif  /* NXPBUILD__PHAC_DISCLOOP_SW */

#ifdef NXPBUILD__PHPAL_FELICA_SW
    #define NXPBUILD__PHAL_FELICA_SW               /**< Software AL Felica */
#endif /* NXPBUILD__PHPAL_FELICA_SW */

#ifdef NXPBUILD__PHPAL_MIFARE_SW
    #if defined(NXPBUILD__PH_KEYSTORE_SW)
        #define NXPBUILD__PHAL_MFC_SW              /**< Software MIFARE Classic */
    #endif
    #define NXPBUILD__PHAL_MFDF_SW                 /**< Software MIFARE DESFire */
    #define NXPBUILD__PHAL_MFUL_SW                 /**< Software MIFARE Ultralight */
#endif

#ifdef NXPBUILD__PHPAL_SLI15693_SW
    #define NXPBUILD__PHAL_ICODE_SW                            /**< AL ICODE SW COMPONENT is included */
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    #define NXPBUILD__PHAL_I18000P3M3_SW                        /**< AL ISO18000p3m3 SW Component is included */
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

#ifdef PH_OSAL_FREERTOS
#   define PHFL_HALAPI_WITH_RTOS
#endif

#ifdef PH_OSAL_NULLOS
#   define PHFL_HALAPI_NO_RTOS
#endif

#ifdef __ICCARM__
#define configTOTAL_HEAP_SIZE           6000
#endif

/**
 * Use Case Specific Flag - Change Accordingly
 */
#define PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR  0   /**< Suspend Feature Value = 1 Feature Enabled  Value = 0 Feature disabled */
#define PH_EXCCID_USB_IF_USB_REMOTE_WAKEUP_FTR   0   /**< Remote Wake up Value = 1 Feature Enabled Value = 0 Feature disabled */
//#define PH_EXCCID_EXTERNAL_TVDD_3V_MON             /* Enable if TVDD 3V Monitor is selected */
//#define PH_EXCCID_USB_IF_COMPLIANCY                /* For USB Compliancy alone Enable */

#endif /* APP_NXP_BUILD_H */
