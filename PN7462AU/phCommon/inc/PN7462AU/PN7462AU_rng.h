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
#ifndef PN7462AU_IP_RNG_H_INCLUDED
#define PN7462AU_IP_RNG_H_INCLUDED

#include "PN7462AU/PN7462AU_rng_rorw.h"



/**
* \name  status register
* <b>Reset value:</b> 0x00008F00\n
* <b>Access:</b> r-
* @{
*/
#define RNG_STATUS_REG                                                                              (0x40018000UL) /**< r-*/
#define RNG_STATUS_REG_RNG_MASK                                                                     (0x0000FF00UL) /**< r- -  RNG number. */
#define RNG_STATUS_REG_RNG_POS                                                                      (8UL)
#define RNG_STATUS_REG_RNG_READY_MASK                                                               (0x00000004UL) /**< r- - high if a new random value is available for read. */
#define RNG_STATUS_REG_RNG_READY_POS                                                                (2UL)
#define RNG_STATUS_REG_RNG_SEED_ERROR_MASK                                                          (0x00000002UL) /**< r- - high if TRNG timeout reached. Indicates an error in seeding process.. */
#define RNG_STATUS_REG_RNG_SEED_ERROR_POS                                                           (1UL)
#define RNG_STATUS_REG_RNG_SEED_READY_MASK                                                          (0x00000001UL) /**< r- - high if seed process is done and RNG is running, generating a new data every cycle.. */
#define RNG_STATUS_REG_RNG_SEED_READY_POS                                                           (0UL)
#define RNG_STATUS_REG__RESET_VALUE                                                                 (0x00008F00UL) /**< value after reset */
/**
* @}
*/


/**
* \name  control register
* <b>Reset value:</b> 0x00002600\n
* <b>Access:</b> rw
* @{
*/
#define RNG_CONTROL_REG                                                                             (0x40018004UL) /**< rw*/
#define RNG_CONTROL_REG_TRNG_STARTUP_TIME_MASK                                                      (0x0000FF00UL) /**< rw -  trng startup time to bypass TRNG seed. */
#define RNG_CONTROL_REG_TRNG_STARTUP_TIME_POS                                                       (8UL)
#define RNG_CONTROL_REG_RNG_ENABLE_MASK                                                             (0x00000001UL) /**< rw -  enable RNG, used for gating main clock. */
#define RNG_CONTROL_REG_RNG_ENABLE_POS                                                              (0UL)
#define RNG_CONTROL_REG__RESET_VALUE                                                                (0x00002600UL) /**< value after reset */
/**
* @}
*/


/**
* \name  preload register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define RNG_DFT_DYNAMIC_TEST_CONTROL_REG                                                            (0x40018008UL) /**< rw*/
#define RNG_DFT_DYNAMIC_TEST_CONTROL_REG_TRNG_ENABLE_TEST_ANA_OUT_MASK                              (0x00000008UL) /**< rw - enable analog test output if high. */
#define RNG_DFT_DYNAMIC_TEST_CONTROL_REG_TRNG_ENABLE_TEST_ANA_OUT_POS                               (3UL)
#define RNG_DFT_DYNAMIC_TEST_CONTROL_REG_DIG_TB_O_EN_MASK                                           (0x00000007UL) /**< rw - digital testbus out enable. */
#define RNG_DFT_DYNAMIC_TEST_CONTROL_REG_DIG_TB_O_EN_POS                                            (0UL)
#define RNG_DFT_DYNAMIC_TEST_CONTROL_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_RNG_H_INCLUDED */
