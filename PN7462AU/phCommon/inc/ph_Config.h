/*
 *                  Copyright (c), NXP Semiconductors
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
 * Project specific configuration.
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 *
 * History:
 *
 */

#ifndef PH_CONFIG_H
#define PH_CONFIG_H

/**
 * @name Platforms
 *
 * @{
 *
 */
/** Compiling code for KEIL/AMRCC/DS5  */
#define PH_CONFIG_TARGET_PLATFORM_DS5    1

/** Compiling code for gnu-arm-gcc / LPCXpresso */
#define PH_CONFIG_TARGET_PLATFORM_ARMGCC 3
/** Compiling code for IAR */
#define PH_CONFIG_TARGET_PLATFORM_IAR    4


/* 0828
    ISO C90 Conformance limits
    [L] Maximum '#if...' nesting exceeds 8 levels - program does not conform strictly to ISO:C90.
    REFERENCE - ISO:C90-5.2.4.1 Translation Limits */

#ifndef PHFL_CONFIG_TARGET_PLATFORM
#   ifdef __ARMCC_VERSION
#       define PHFL_CONFIG_TARGET_PLATFORM PH_CONFIG_TARGET_PLATFORM_DS5
#   endif
#endif /* PHFL_CONFIG_TARGET_PLATFORM  */
#ifndef PHFL_CONFIG_TARGET_PLATFORM
#   ifdef __GNUC__
#       define PHFL_CONFIG_TARGET_PLATFORM PH_CONFIG_TARGET_PLATFORM_ARMGCC
#   endif
#endif /* PHFL_CONFIG_TARGET_PLATFORM  */
#ifndef PHFL_CONFIG_TARGET_PLATFORM
#   ifdef __ICCARM__
#       define PHFL_CONFIG_TARGET_PLATFORM PH_CONFIG_TARGET_PLATFORM_IAR
#   endif
#endif
#ifndef PHFL_CONFIG_TARGET_PLATFORM
#   error Why here?  Platform should have been identified by now.
#endif

/**
 * @ingroup ph_Status
 *
 * @{
 *
 * @name Parameter Check
 */
/** User may use APIs against the constraints of the API itself.
 *
 * E.g. Pass NULL to a parameter which is never allowed and
 * the result is a blind failure in most cases.
 *
 * Since many parameters would be checked, the code size would drastically increase.  */
#define PH_PARAM_CHECK_VERBOSE        2
/** SW May go wrong, but if only user makes mistake like Memory/Resource Leaks.
 *
 * Good SW Programming practicies will never result in error conditions that fall under this category.
 *
 * Since some parameters would not be checked, the code size would be lower as compared to #PH_PARAM_CHECK_VERBOSE
 * */
#define PH_PARAM_CHECK_SW_RESOURCE    1
/** Nothing would go wrong
 *
 * Since SW Conditions, like event creation, malloc failed, etc. would never be checked,
 * the code size would be much lower as compared to #PH_PARAM_CHECK_SW_RESOURCE, but
 * all the failures would blindly go un-noticed. */
#define PH_PARAM_CHECK_NONE           0

/** Default parameter check.
 *
 * Expected to be set to default value by the make files OR the build system.
 * Should be set to one of #PH_PARAM_CHECK_NONE, #PH_PARAM_CHECK_SW_RESOURCE or #PH_PARAM_CHECK_VERBOSE.
 */
#ifndef PHFL_PARAM_CHECK
#   define PHFL_PARAM_CHECK PH_PARAM_CHECK_SW_RESOURCE
#endif

/** @} */

#endif /* PH_CONFIG_H */
