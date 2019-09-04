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

/** \file ph_MemoryMap.h
 *
 * This header file will contain information regarding the Memory Map of the PN7462AU System
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 *
 * History:
 *
 * */

#ifndef PH_MEMORYMAP_H
#define PH_MEMORYMAP_H

/* EEPROM MEMORY MAP */

/** Start address of the EEPROM; Physical. Not accessible to the user. */
#define PH_EEPROM_MEM_START         (0x201000UL)
#define PH_EEPROM_MEM_END           (0x201FFFUL) /**< End address of the EEPROM */
/** Start address of the User data region in the EEPROM. Accessible to the user */
#define PH_EEPROM_USERDATA_START    (0x201200UL)
/** End address of the User data region in the EEPROM. Accessible to the user */
#define PH_EEPROM_USERDATA_END      (PH_EEPROM_MEM_END)

/** PAGE FLASH MEMORY MAP */
#define PH_PAGEFLASH_MEM_START      (0x203000UL) /**< Start address of the PAGE FLASH */

/** End address of the PAGE FLASH; Physical. Not accessible to the user. */
#define PH_PAGEFLASH_MEM_END        (0x22AFFFUL)
#define PH_PAGEFLASH_USERDATA_START (0x203000UL) /**< Start address of the User data region in the PAGE FLASH */
/** End address of the User data region in the PAGE FLASH. Accessible to the user
 *
 * - For 158k Variant - 0x0022A7FF
 * - For 154k Variant - 0x002297FF
 * - For 80k  Variant - 0x00216FFF
 *
 */
#define PH_PAGEFLASH_USERDATA_END   (0x0022A7FF)

/** RAM MEMORY MAP */
#define PH_RAM_MEM_START            (0x00100020) /**< Start address of the RAM, accessible to the user */
#define PH_RAM_MEM_END              (0x00102EFF) /**< End address of the RAM, accessible to the user */
#endif /* PH_MEMORYMAP_H */
