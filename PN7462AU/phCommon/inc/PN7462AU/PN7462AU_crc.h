/*******************************************************************************
 *
 *                Copyright (C), NXP Semiconductors
 *
 *                   (C) NXP Semiconductors
 *     All rights are reserved. Reproduction in whole or in part is
 *    prohibited without the written consent of the copyright owner.
 * NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                        arising from its use.
 *
 * *//** \file
 *
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14132 $ (v04.13.00)
 * $Date: 2015-07-27 09:47:10 +0530 (Mon, 27 Jul 2015) $
 *
 *******************************************************************************/
#ifndef PN7462AU_IP_CRC_H_INCLUDED
#define PN7462AU_IP_CRC_H_INCLUDED

#include "PN7462AU/PN7462AU_crc_rorw.h"



/**
* \name control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CRC_CONTROL_REG                                                                             (0x4000C000UL) /**< rw*/
#define CRC_CONTROL_REG_CRCMOD_MASK                                                                 (0x00000070UL) /**< rw - CRC mode of computation. */
#define CRC_CONTROL_REG_CRCMOD_POS                                                                  (4UL)
#define CRC_CONTROL_REG_CRCINV_MASK                                                                 (0x00000004UL) /**< rw - Invert input data . */
#define CRC_CONTROL_REG_CRCINV_POS                                                                  (2UL)
#define CRC_CONTROL_REG_CRC_ENABLE_MASK                                                             (0x00000001UL) /**< rw - enable crc. */
#define CRC_CONTROL_REG_CRC_ENABLE_POS                                                              (0UL)
#define CRC_CONTROL_REG__RESET_VALUE                                                                (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name data 32 register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CRC_DAT32_REG                                                                               (0x4000C004UL) /**< rw*/
#define CRC_DAT32_REG_CRCDAT32_MASK                                                                 (0xFFFFFFFFUL) /**< rw - 32 bits CRC value. */
#define CRC_DAT32_REG_CRCDAT32_POS                                                                  (0UL)
#define CRC_DAT32_REG__RESET_VALUE                                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name data 16 register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CRC_DAT16_REG                                                                               (0x4000C008UL) /**< rw*/
#define CRC_DAT16_REG_CRCDAT16_MASK                                                                 (0x0000FFFFUL) /**< rw - 16 bits CRC value. */
#define CRC_DAT16_REG_CRCDAT16_POS                                                                  (0UL)
#define CRC_DAT16_REG__RESET_VALUE                                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name data 8 register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CRC_DAT8_REG                                                                                (0x4000C00CUL) /**< rw*/
#define CRC_DAT8_REG_CRCDAT8_MASK                                                                   (0x000000FFUL) /**< rw - 8 bits CRC value. */
#define CRC_DAT8_REG_CRCDAT8_POS                                                                    (0UL)
#define CRC_DAT8_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name crc computed register
* <b>Reset value:</b> 0x0000FFFF\n
* <b>Access:</b> r-
* @{
*/
#define CRC_DAT_CALC_REG                                                                            (0x4000C010UL) /**< r-*/
#define CRC_DAT_CALC_REG_CRCDAT_COMPUTED_MASK                                                       (0xFFFFFFFFUL) /**< r- - 32 bits CRC computed value. */
#define CRC_DAT_CALC_REG_CRCDAT_COMPUTED_POS                                                        (0UL)
#define CRC_DAT_CALC_REG__RESET_VALUE                                                               (0x0000FFFFUL) /**< value after reset */
/**
* @}
*/


/**
* \name preload register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CRC_DAT_PRELOAD_REG                                                                         (0x4000C014UL) /**< rw*/
#define CRC_DAT_PRELOAD_REG_CRCDAT_PRELOAD_MASK                                                     (0xFFFFFFFFUL) /**< rw - 32 bits CRC preload value. */
#define CRC_DAT_PRELOAD_REG_CRCDAT_PRELOAD_POS                                                      (0UL)
#define CRC_DAT_PRELOAD_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name dft register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CRC_DFT_CONTROL_REG                                                                         (0x4000C018UL) /**< rw*/
#define CRC_DFT_CONTROL_REG_DIG_TB_O_ENABLE_MASK                                                    (0x000000FFUL) /**< rw - 32 bits testbus selection. */
#define CRC_DFT_CONTROL_REG_DIG_TB_O_ENABLE_POS                                                     (0UL)
#define CRC_DFT_CONTROL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_CRC_H_INCLUDED */
