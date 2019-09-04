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
* Build System Definitions for Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 18555 $ (v04.13.00)
* $Date: 2016-07-06 12:14:48 +0530 (Wed, 06 Jul 2016) $
*
* History:
*  MK: Generated 15. October 2009
*
*/

#ifndef PH_NXPBUILD_H
#define PH_NXPBUILD_H

/** \addtogroup ph_NxpBuild NXP Build
* \brief Controls build behaviour of components.
* @{
*/

/* NXPBUILD_DELETE: included code lines should be always removed from code */

/* NXP Build defines                            */
/* use #define to include components            */
/* comment out #define to exclude components    */


/* Check for application specific BUILD configurations. */
#if defined(PHFL_APP_NXBUILD_CONFIG)
#   include "APP_NxpBuild.h"
#elif defined(NXPBUILD_CUSTOMER_HEADER_INCLUDED)
#	include "ph_NxpBuild_App.h"
#else
#   include "ph_NxpBuild_Default.h"
#endif

#ifdef NXPBUILD__CFG_FROM_PF
#    ifdef NXPBUILD__CFG_FROM_EE
#        error "CFG can not be loaded both from PF and EE"
#    endif
#endif

#ifdef NXPBUILD__PHHAL_USB
#   ifdef NXPBUILD__PHHAL_HIF
#       error "HIF and USB can not be combined."
#   endif
#endif

#if !defined(PHFL_HALAPI_WITH_RTOS) && !defined(PHFL_HALAPI_NO_RTOS) && defined(NXPBUILD__PHHAL_HW_PN7462AU)
#	ifdef NXPBUILD__PH_OSAL_FREERTOS
#		define PHFL_HALAPI_WITH_RTOS
#	endif
#	ifdef NXPBUILD__PH_OSAL_NULLOS
#		define PHFL_HALAPI_NO_RTOS
#	endif
#endif

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
#	define NXPBUILD__PHHAL_RF
#	define NXPBUILD__PHHAL_TIMER
#	define NXPBUILD__PHHAL_PCR
#   define NXPBUILD__PHHAL_CLKGEN
#	define NXPBUILD__PHHAL_CRC
#	define NXPBUILD__PHHAL_PMU
#	define NXPBUILD__PHHAL_RNG
#	define NXPBUILD__CFG_FROM_EE
#   define NXPBUILD__PHHAL_WDT
#endif


#ifdef PHFL_ONLY_RF
#    undef NXPBUILD__PHPAL_CT
#    undef NXPBUILD__PHHAL_CT
#    undef NXPBUILD__PHHAL_FLASH
#endif

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

#ifdef PH_OSAL_FREERTOS
#   define PHFL_HALAPI_WITH_RTOS
#endif

#ifdef PH_OSAL_NULLOS
#   define PHFL_HALAPI_NO_RTOS
#endif

#ifndef PHFL_HALAPI_NO_RTOS
#   ifndef PHFL_HALAPI_WITH_RTOS
#       error "Either of PHFL_HALAPI_NO_RTOS or PHFL_HALAPI_WITH_RTOS should be defined"
#   endif
#endif

#if defined (NXPBUILD__PHBAL_REG_STUB)          || \
    defined (NXPBUILD__PHBAL_REG_LPCOPEN_SPI)   || \
    defined (NXPBUILD__PHBAL_REG_LPCOPEN_I2C)   || \
    defined (NXPBUILD__PHBAL_REG_SERIALWIN)     || \
    defined (NXPBUILD__PHBAL_REG_PCSCWIN)       || \
    defined (NXPBUILD__PHBAL_REG_RD70XUSBWIN)   || \
    defined (NXPBUILD__PHBAL_REG_PIPELIN)       || \
    defined (NXPBUILD__PHBAL_REG_SOCKETWIN)     || \
    defined (NXPBUILD__PHBAL_REG_RD710SAM)      || \
    defined (NXPBUILD__PHBAL_REG_VISA)          || \
    defined (NXPBUILD__PHBAL_REG_RDFPGAV6)      || \
    defined (NXPBUILD__PHBAL_REG_PI_SPI)        || \
    defined (NXPBUILD__PHBAL_REG_PI_I2C)        || \
    defined (NXPBUILD__PHBAL_REG_ABEND)
    #define NXPBUILD__PHBAL_REG                 /**< Generic BAL definition */
#endif

/* HAL components */

#if defined (NXPBUILD__PHHAL_HW_RC523)        || \
    defined (NXPBUILD__PHHAL_HW_SAMAV2)       || \
    defined (NXPBUILD__PHHAL_HW_RD70X)        || \
    defined (NXPBUILD__PHHAL_HW_RDCARDSIM)    || \
    defined (NXPBUILD__PHHAL_HW_CALLBACK)     || \
    defined (NXPBUILD__PHHAL_HW_RC663)        || \
    defined (NXPBUILD__PHHAL_HW_RC632)        || \
    defined (NXPBUILD__PHHAL_HW_MFCCRYPTO)    || \
    defined (NXPBUILD__PHHAL_HW_RD710)        || \
    defined (NXPBUILD__PHHAL_HW_MP300)        || \
    defined (NXPBUILD__PHHAL_HW_RDFPGAV6)     || \
    defined (NXPBUILD__PHHAL_HW_PN7462AU)      || \
    defined (NXPBUILD__PHHAL_HW_DUT)          || \
    defined (NXPBUILD__PHHAL_HW_UHOST)        || \
    defined (NXPBUILD__PHHAL_HW_PN5180)       || \
    defined (NXPBUILD__PHHAL_HW_STUB)
    #define NXPBUILD__PHHAL_HW                  /**< Generic HAL definition */
#endif


#if defined (NXPBUILD__PHHAL_HW_DUTHW)
#   if ! defined(NXPBUILD__PHHAL_HW_PN7462AU)
#       error "Generic DUTHW needs specific HAL implementation, e.g. PN7462AU/PN518"
#   endif
#endif

#if ! defined(NXPBUILD__PHHAL_HW_PN7462AU)
#   if defined (NXPBUILD__PHHAL_HW_PICC_B)
#       error NXPBUILD__PHHAL_HW_PICC_B is not supported without NXPBUILD__PHHAL_HW_PN7462AU
#   endif
#endif

#if defined(NXPBUILD__PHHAL_HW_PN7462AU)
#	if ! defined NXPBUILD__PH_PN7462AU
#		define NXPBUILD__PH_PN7462AU
#	endif
#	if !defined NXPBUILD__PH_PLATFORM
#		define NXPBUILD__PH_PLATFORM
#	endif
#endif


/* PAL ISO 14443-3A components */

#if defined (NXPBUILD__PHPAL_I14443P3A_SW)          || \
    defined (NXPBUILD__PHPAL_I14443P3A_SAMAV2_X)    || \
    defined (NXPBUILD__PHPAL_I14443P3A_RD70X)       || \
    defined (NXPBUILD__PHPAL_I14443P3A_RD710)       || \
    defined (NXPBUILD__PHPAL_I14443P3A_DUT)
    #define NXPBUILD__PHPAL_I14443P3A           /**< Generic PAL ISO 14443-3A definition */
#endif


/* PAL ISO 14443-3B components */

#if defined (NXPBUILD__PHPAL_I14443P3B_SW)      || \
    defined (NXPBUILD__PHPAL_I14443P3B_DUT)
    #define NXPBUILD__PHPAL_I14443P3B           /**< Generic PAL ISO 14443-3B definition */
#endif
/* PAL ISO 14443-4A components */


#if defined (NXPBUILD__PHPAL_I14443P4A_SW)       || \
    defined (NXPBUILD__PHPAL_I14443P4A_SAMAV2_X) || \
    defined (NXPBUILD__PHPAL_I14443P4A_RD710)    || \
    defined (NXPBUILD__PHPAL_I14443P4A_DUT)
    #define NXPBUILD__PHPAL_I14443P4A           /**< Generic PAL ISO 14443-4A definition */
#endif

/* PAL ISO 14443-4 components */

#if defined (NXPBUILD__PHPAL_I14443P4_SW)       || \
    defined (NXPBUILD__PHPAL_I14443P4_SAMAV2_X) || \
    defined (NXPBUILD__PHPAL_I14443P4_RD710)    || \
    defined (NXPBUILD__PHPAL_I14443P4_DUT)
    #define NXPBUILD__PHPAL_I14443P4            /**< Generic PAL ISO 14443-4 definition */
#endif

/* PAL ISO 18092 Target component */

#if defined (NXPBUILD__PHPAL_I14443P4MC_SW)
    #define NXPBUILD__PHPAL_I14443P4MC             /**< Generic PAL ISO 18092 target definition */
#endif

/* PAL MIFARE components */

#if defined (NXPBUILD__PHPAL_MIFARE_STUB)       || \
    defined (NXPBUILD__PHPAL_MIFARE_SW)         || \
    defined (NXPBUILD__PHPAL_MIFARE_SAMAV2_X)   || \
    defined (NXPBUILD__PHPAL_MIFARE_RD710)      || \
    defined (NXPBUILD__PHPAL_MIFARE_DUT)
    #define NXPBUILD__PHPAL_MIFARE              /**< Generic PAL MIFARE definition */
#endif

/* PAL ISO15693 ICode SLI components */

#if defined (NXPBUILD__PHPAL_SLI15693_SW)       || \
    defined (NXPBUILD__PHPAL_SLI15693_DUT)      || \
    defined (NXPBUILD__PHPAL_SLI15693_STUB)
    #define NXPBUILD__PHPAL_SLI15693            /**< Generic PAL Sli 15693 definition */
#endif

/* PAL Felica components */

/*#define HANDLE_MULTIPLE_FELICA_CARD_RESP */   /**< Enable handling of multiple Felica card response */

#if defined (NXPBUILD__PHPAL_FELICA_SW)         || \
    defined (NXPBUILD__PHPAL_FELICA_DUT)
    #define NXPBUILD__PHPAL_FELICA              /**< Generic PAL Felica definition */
#endif

/* PAL ICode EPC/UID components */

#if defined (NXPBUILD__PHPAL_EPCUID_SW)     || \
    defined (NXPBUILD__PHPAL_EPCUID_RD70X)
    #define NXPBUILD__PHPAL_EPCUID              /**< Generic PAL ICode EPC/UID definition */
#endif

/* PAL I18000p3m3 components */

#if defined (NXPBUILD__PHPAL_I18000P3M3_SW)     || \
    defined (NXPBUILD__PHPAL_I18000P3M3_DUT)
    #define    NXPBUILD__PHPAL_I18000P3M3            /**< Generic PAL I18000p3m3 definition */
#endif

/* PAL ISO 18092 components */

#if defined (NXPBUILD__PHPAL_I18092MPI_SW)      || \
    defined (NXPBUILD__PHPAL_I18092MPI_DUT)
    #define NXPBUILD__PHPAL_I18092MPI           /**< Generic PAL ISO 18092 definition */
#endif

/* PAL ISO 18092 Target component */

#if defined (NXPBUILD__PHPAL_I18092MT_SW)
    #define NXPBUILD__PHPAL_I18092MT             /**< Generic PAL ISO 18092 target definition */
#endif

/* AC dicover loop component */

#if defined (NXPBUILD__PHAC_DISCLOOP_SW)
    #define NXPBUILD__PHAC_DISCLOOP             /**< Generic dicovery loop component */
#endif


#if defined (NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS) || \
    defined (NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS) || \
    defined (NXPBUILD__PHAC_DISCLOOP_TYPEA_JEWEL_TAGS)|| \
	defined (NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_TAGS)
    #define NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS				/**< Detect Passive TypeA Tags/Peers */
#endif

#if defined (NXPBUILD__PHAC_DISCLOOP_FELICA_TAGS) || \
   defined (NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS)
   #define NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
#endif

#if defined (NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS) || \
    defined (NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS) || \
    defined (NXPBUILD__PHAC_DISCLOOP_TYPEB_BPRIME_TAGS)
    #define NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS          /**< Detect TypeB Tags */
#endif

#if defined (NXPBUILD__PHAC_DISCLOOP_TYPEA_TARGET_PASSIVE) || \
    defined (NXPBUILD__PHAC_DISCLOOP_TYPEA_TARGET_ACTIVE) || \
    defined (NXPBUILD__PHAC_DISCLOOP_TYPEF212_TARGET_PASSIVE)|| \
	defined (NXPBUILD__PHAC_DISCLOOP_TYPEF212_TARGET_ACTIVE) || \
	defined (NXPBUILD__PHAC_DISCLOOP_TYPEF424_TARGET_PASSIVE) || \
	defined (NXPBUILD__PHAC_DISCLOOP_TYPEF424_TARGET_ACTIVE)
    #define NXPBUILD__PHAC_DISCLOOP_TARGET			   /**< Configure Target Mode */
#endif

#if defined (NXPBUILD__PH_OSAL_LPC12XX) || \
    defined (NXPBUILD__PH_OSAL_LPC17XX) || \
    defined (NXPBUILD__PH_OSAL_WINOS)   || \
    defined (NXPBUILD__PH_OSAL_FREERTOS) || \
    defined (NXPBUILD__PH_OSAL_LINUX)    || \
    defined (NXPBUILD__PH_OSAL_NULLOS)
    #define NXPBUILD__PH_OSAL                           /**< Generic OSAL definition */
#endif

/* AL Felica components */

#if defined (NXPBUILD__PHAL_FELICA_SW)
    #define NXPBUILD__PHAL_FELICA               /**< Generic AL Felica definition */
#endif

/* AL MIFARE Classic components */

#if defined (NXPBUILD__PHAL_MFC_SW)         || \
    defined (NXPBUILD__PHAL_MFC_SAMAV2_X)
    #define NXPBUILD__PHAL_MFC                  /**< Generic AL MIFARE Classic definition */
#endif

/* AL MIFARE Plus components */

#if defined (NXPBUILD__PHAL_MFP_SW)         || \
    defined (NXPBUILD__PHAL_MFP_SAMAV2)     || \
    defined (NXPBUILD__PHAL_MFP_SAMAV2_X)
    #define NXPBUILD__PHAL_MFP                  /**< Generic AL MIFARE Plus definition */
#endif

/* AL MIFARE DESFire Components */

#if defined (NXPBUILD__PHAL_MFDF_SW)         || \
    defined (NXPBUILD__PHAL_MFDF_SAMAV2)     || \
    defined (NXPBUILD__PHAL_MFDF_SAMAV2_X)
    #define NXPBUILD__PHAL_MFDF                  /**< Generic AL MIFARE Desfire definition */
#endif


#if defined (NXPBUILD__PHAL_MFDFEV2_SW)
    #define NXPBUILD__PHAL_MFDFEV2                  /**< Generic AL MIFARE Desfire EV2 definition */
#endif

/* AL MIFARE Ultralight components */

#if defined (NXPBUILD__PHAL_MFUL_SW)        || \
    defined (NXPBUILD__PHAL_MFUL_SAMAV2)    || \
    defined (NXPBUILD__PHAL_MFUL_SAMAV2_X)
    #define NXPBUILD__PHAL_MFUL                 /**< Generic AL MIFARE Ultralight definition */
#endif


/* AL T1T components */

#if defined (NXPBUILD__PHAL_T1T_SW)
    #define NXPBUILD__PHAL_T1T                 /**< Generic AL Type 1 Tag definition */
#endif


/* AL TOP components */

#if defined (NXPBUILD__PHAL_TOP_SW)
    #define NXPBUILD__PHAL_TOP                 /**< Generic AL Tag Operations */
#endif

/* CE T4AT Card Emulation components */

#if defined (NXPBUILD__PHCE_T4T_SW)
    #define NXPBUILD__PHCE_T4T                 /**< Generic HCE */
#endif

/* AL Virtual Card Architecture components */

#if defined (NXPBUILD__PHAL_VCA_SW)         || \
    defined (NXPBUILD__PHAL_VCA_SAMAV2)     || \
    defined (NXPBUILD__PHAL_VCA_SAMAV2_X)
    #define NXPBUILD__PHAL_VCA                  /**< Generic AL Virtual Card Architecture definition */
#endif


/* DL FPGA Box components */

#if defined (NXPBUILD__PHDL_RDFPGA_V6)
    #define NXPBUILD__PHDL_RDFPGA               /**< Generic FPGA Box reader definition */
#endif


/* DL Amplifier components */

#if defined (NXPBUILD__PHDL_AMP_AR75A250)
    #define NXPBUILD__PHDL_AMP                  /**< Generic Amplifier definition */
#endif

/* DL Thermostream components */

#if defined (NXPBUILD__PHDL_THSTRM_TP04030)
    #define NXPBUILD__PHDL_THSTRM                /**< Generic Thermostream definition */
#endif

/* DL Oscilloscope components */

#if defined (NXPBUILD__PHDL_OSCI_LW64XI)
    #define NXPBUILD__PHDL_OSCI                /**< Generic Oscilloscope definition */
#endif

/* DL Master Amplifier Oscilloscope components */

#if defined (NXPBUILD__PHDL_OSCI) && defined (NXPBUILD__PHDL_AMP) && \
    defined (NXPBUILD__PHDL_MSTAMPOSC_AR75A250) || \
    defined (NXPBUILD__PHDL_MSTAMPOSC_ISOSETUP) && defined (NXPBUILD__PHDL_RDFPGA)
    #define NXPBUILD__PHDL_MSTAMPOSC           /**< Generic Master Amplifier Oscilloscope definition */
#endif

/* DL Stepper components */

#if defined (NXPBUILD__PHDL_STEPPER_V1)
    #define NXPBUILD__PHDL_STEPPER                  /**< Generic Stepper definition */
#endif

/* AL ICODE components */

#if defined (NXPBUILD__PHAL_ICODE_SW)
    #define NXPBUILD__PHAL_ICODE               /**< Generic AL ICode definition */
#endif

/* AL ISO 18000P3M3 components */

#if defined (NXPBUILD__PHAL_I18000P3M3_SW)
    #define NXPBUILD__PHAL_I18000P3M3           /**< Generic AL I18000p3m3 definition */
#endif

/* AL P40 PrivateCommands components */

#if defined (NXPBUILD__PHAL_P40CMDPRIV_SW)
    #define NXPBUILD__PHAL_P40CMDPRIV                  /**< Generic AL P40 PrivateCommand definition */
#endif

/* AL P40 PublicCommand components */

#if defined (NXPBUILD__PHAL_P40CMDPUB_SW)
    #define NXPBUILD__PHAL_P40CMDPUB                  /**< Generic AL P40 PublicCommand definition */
#endif

/* CID Manager components */

#if defined (NXPBUILD__PH_CIDMANAGER_SW)       || \
    defined (NXPBUILD__PH_CIDMANAGER_RD710)
    #define NXPBUILD__PH_CIDMANAGER             /**< Generic CID Manager definition */
#endif


/* Crypto components */

#if defined (NXPBUILD__PH_CRYPTOSYM_SW)        || \
    defined (NXPBUILD__PH_CRYPTOSYM_STUB)
    #define NXPBUILD__PH_CRYPTOSYM                 /**< Generic Crypto Sym definition */
#endif

/* CryptoRng components */

#if defined (NXPBUILD__PH_CRYPTORNG_SW)       || \
    defined (NXPBUILD__PH_CRYPTORNG_STUB)
    #define NXPBUILD__PH_CRYPTORNG              /**< Generic CryptoRng definition */
#endif

/* KeyStore components */

#if defined (NXPBUILD__PH_KEYSTORE_SW)      || \
    defined (NXPBUILD__PH_KEYSTORE_SAMAV2)  || \
    defined (NXPBUILD__PH_KEYSTORE_RC663)   || \
    defined (NXPBUILD__PH_KEYSTORE_RC632)   || \
    defined (NXPBUILD__PH_KEYSTORE_RD710)
    #define NXPBUILD__PH_KEYSTORE               /**< Generic KeyStore definition */
#endif


/* LN components */

#if defined (NXPBUILD__PHLN_LLCP_SW)
    #define NXPBUILD__PHLN_LLCP               /**< Generic Link layer definition */
#endif

/* SNEP components */

#if defined (NXPBUILD__PHNP_SNEP_SW)
    #define NXPBUILD__PHNP_SNEP                 /**< Generic HAL definition */
#endif


#if defined(NXPBUILD__PH_LPC1769)      || \
	defined(NXPBUILD__PH_LPC11U68)     || \
	defined(NXPBUILD__PH_RASPBERRY_PI)
	#define  PH_PLATFORM_HAS_ICFRONTEND
#endif

/* Log component */

/*#define NXPBUILD__PH_LOG*/

/* Enable/disable Debugging */
/*#define NXPBUILD__PH_DEBUG*/

/** @}
* end of ph_NxpBuild
*/

#endif /* PH_NXPBUILD_H */
