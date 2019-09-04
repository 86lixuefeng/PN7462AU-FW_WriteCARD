/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** @file
 * Implementation of boot up routine for Flash
 * $Author: Purnank G (ing05193) $
 * $Revision: 18502 $ (v04.13.00)
 * $Date: 2016-06-23 13:52:29 +0530 (Thu, 23 Jun 2016) $
 *
 * Project:  PN7462AU
 */

#ifndef PHFL_FLASH_BOOT_H
#define PHFL_FLASH_BOOT_H 1

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ph_Datatypes.h>
#include "ph_Status.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phhalPcr.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/** Clear boot reason
 *
 * This is essential to be done on Boot up. */
#define PH_FLASHBOOT_BTRNS_CLR_BOOT_REASON() \
	(PH_REG_SET(PCR_CTRL_REG, \
			PCR_CTRL_REG_CLR_BOOT_REGS_MASK))

/* Profiling Time to let the TxLdo Start on Boot. */
#define PH_FLASHBOOT_TXLDO_START_TIME_US				400
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/** Default entry point for a binary/test application
 *
 * Target application should define this function.
 */
extern void phFlashBoot_Main(void);

/**
 * phFlashBoot_ZIBSS Zero Initialize BSS
 */
void phFlashBoot_ZIBSS(void);

/** Identify the Boot reason */
phhalPcr_BootReason_t phFlashBoot_GetBtRns(void);

/** Human readable/inspectable Hard fault handler,  called from ISR */
void phFlashBoot_HardFault_HandlerC(
    unsigned long *args);

#endif /* PHFL_FLASH_BOOT_H */
