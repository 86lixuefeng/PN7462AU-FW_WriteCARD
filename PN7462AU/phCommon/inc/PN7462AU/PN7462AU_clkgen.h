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
#ifndef PN7462AU_IP_CLKGEN_H_INCLUDED
#define PN7462AU_IP_CLKGEN_H_INCLUDED

#include "PN7462AU/PN7462AU_clkgen_rorw.h"



/**
* \name  CLKGEN Status Register (RO)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define CLKGEN_STATUS_REG                                                                           (0x40010000UL) /**< r--*/
#define CLKGEN_STATUS_REG_CLIF_CLOCK_PRESENCE_OK_MASK                                               (0x04000000UL) /**< r- - Indicates the status of clkgen_clif_pll_lock2_o signal.. */
#define CLKGEN_STATUS_REG_CLIF_CLOCK_PRESENCE_OK_POS                                                (26UL)
#define CLKGEN_STATUS_REG_XTAL_ACTIVATION_TIMEOUT_ERROR_MASK                                        (0x02000000UL) /**< r- - High if timeout for XTAL Activation is reached. */
#define CLKGEN_STATUS_REG_XTAL_ACTIVATION_TIMEOUT_ERROR_POS                                         (25UL)
#define CLKGEN_STATUS_REG_XTAL_DETECT_OK_MASK                                                       (0x01000000UL) /**< r- - Indicates the presence of clock signal on clk_xtal if xtal_detect_enable is set.. */
#define CLKGEN_STATUS_REG_XTAL_DETECT_OK_POS                                                        (24UL)
#define CLKGEN_STATUS_REG_XTAL_OSC_OK_MASK                                                          (0x00800000UL) /**< r- - MRA2 Only - high to indicate the clock is ready. */
#define CLKGEN_STATUS_REG_XTAL_OSC_OK_POS                                                           (23UL)
#define CLKGEN_STATUS_REG_CLIF_PLL_LOCK_OVERRIDEN_MASK                                              (0x00400000UL) /**< r- - MRA2 Only - NFCC1036 - High if pll_lock2 OR pll_bypass_lock2 is high. */
#define CLKGEN_STATUS_REG_CLIF_PLL_LOCK_OVERRIDEN_POS                                               (22UL)
#define CLKGEN_STATUS_REG_CLIF_CLK_IN_DETECT_DONE_MASK                                              (0x00200000UL) /**< r- - High if detection is done.. */
#define CLKGEN_STATUS_REG_CLIF_CLK_IN_DETECT_DONE_POS                                               (21UL)
#define CLKGEN_STATUS_REG_CLIF_CLK_IN_OK_MASK                                                       (0x00100000UL) /**< r- - PLL input clok detector ok signal. High if clk_in is present. */
#define CLKGEN_STATUS_REG_CLIF_CLK_IN_OK_POS                                                        (20UL)
#define CLKGEN_STATUS_REG_CLIF_PLL_LOCK2_MASK                                                       (0x00080000UL) /**< r- - Lock detector Output for 2nd PLL. */
#define CLKGEN_STATUS_REG_CLIF_PLL_LOCK2_POS                                                        (19UL)
#define CLKGEN_STATUS_REG_CLIF_PLL_LOCK1_MASK                                                       (0x00040000UL) /**< r- - Lock detector Output for 1st PLL. If lock1 is high then PLL2 should be enabled.. */
#define CLKGEN_STATUS_REG_CLIF_PLL_LOCK1_POS                                                        (18UL)
#define CLKGEN_STATUS_REG_XTAL_OK_MASK                                                              (0x00020000UL) /**< r- - High if XTAL Oscillator is Activated. */
#define CLKGEN_STATUS_REG_XTAL_OK_POS                                                               (17UL)
#define CLKGEN_STATUS_REG_XTAL_ENABLED_MASK                                                         (0x00010000UL) /**< r- - High if XTAL Oscillator is Enabled. */
#define CLKGEN_STATUS_REG_XTAL_ENABLED_POS                                                          (16UL)
#define CLKGEN_STATUS_REG_CLK_IN_EDGES_COUNTER_MASK                                                 (0x0000FF00UL) /**< r- - Input Clock Edges Counter Value. */
#define CLKGEN_STATUS_REG_CLK_IN_EDGES_COUNTER_POS                                                  (8UL)
#define CLKGEN_STATUS_REG_CLK_IN_DETECT_DONE_MASK                                                   (0x00000080UL) /**< r- - High if USB_PLL Input Clock is Detected. */
#define CLKGEN_STATUS_REG_CLK_IN_DETECT_DONE_POS                                                    (7UL)
#define CLKGEN_STATUS_REG_CLK_IN_OK_MASK                                                            (0x00000040UL) /**< r- - USB_PLL Input Clock Detector OK Status. High if CLK_IN is Present. */
#define CLKGEN_STATUS_REG_CLK_IN_OK_POS                                                             (6UL)
#define CLKGEN_STATUS_REG_USB_PLL_FR_MASK                                                           (0x00000020UL) /**< r- - USB_PLL Free Running Detector Status. */
#define CLKGEN_STATUS_REG_USB_PLL_FR_POS                                                            (5UL)
#define CLKGEN_STATUS_REG_USB_PLL_PACK_MASK                                                         (0x00000010UL) /**< r- - USB_PLL Post-Divider Ratio Change Acknowledge. */
#define CLKGEN_STATUS_REG_USB_PLL_PACK_POS                                                          (4UL)
#define CLKGEN_STATUS_REG_USB_PLL_NACK_MASK                                                         (0x00000008UL) /**< r- - USB_PLL Pre-Divider Ratio Change Acknowledge. */
#define CLKGEN_STATUS_REG_USB_PLL_NACK_POS                                                          (3UL)
#define CLKGEN_STATUS_REG_USB_PLL_MACK_MASK                                                         (0x00000004UL) /**< r- - USB_PLL Feedback Divider Ratio Change Acknowledge. */
#define CLKGEN_STATUS_REG_USB_PLL_MACK_POS                                                          (2UL)
#define CLKGEN_STATUS_REG_USB_PLL_LOCK_OVERRIDEN_MASK                                               (0x00000002UL) /**< r- - USB_PLL Lock Overriden Status. High if CLKGEN_STATUS_REG.USB_PLL_lock or CLKGEN_USB_PLL_GLOBAL_CONTROL_REG.USB_PLL_lock_bypass is high.. */
#define CLKGEN_STATUS_REG_USB_PLL_LOCK_OVERRIDEN_POS                                                (1UL)
#define CLKGEN_STATUS_REG_USB_PLL_LOCK_MASK                                                         (0x00000001UL) /**< r- - USB_PLL Lock Status. */
#define CLKGEN_STATUS_REG_USB_PLL_LOCK_POS                                                          (0UL)
#define CLKGEN_STATUS_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN HFO and XTAL Control Register (RW)
* <b>Reset value:</b> 0x00FFF001\n
* <b>Access:</b> rw-
* @{
*/
#define CLKGEN_HFO_XTAL_REG                                                                         (0x40010004UL) /**< rw-*/
#define CLKGEN_HFO_XTAL_REG_XTAL_ACTIVATION_TIMEOUT_MASK                                            (0x00FFF000UL) /**< rw - Set the XTAL Activation Timeout Value (in LFO Clock Cycles + 8) - default value > 10ms. */
#define CLKGEN_HFO_XTAL_REG_XTAL_ACTIVATION_TIMEOUT_POS                                             (12UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_DETECT_ENABLE_MASK                                                 (0x00000800UL) /**< rw - Enable the XTAL output clock presence detection if clk_in_detect_enable is low.. */
#define CLKGEN_HFO_XTAL_REG_XTAL_DETECT_ENABLE_POS                                                  (11UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE3_MASK                                                        (0x00000400UL) /**< rw - Controls XTAL Spare3. */
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE3_POS                                                         (10UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE2_MASK                                                        (0x00000200UL) /**< rw - Controls XTAL Spare2. */
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE2_POS                                                         (9UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE1_MASK                                                        (0x00000100UL) /**< rw - Controls XTAL Spare1. */
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE1_POS                                                         (8UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE0_MASK                                                        (0x00000080UL) /**< rw - Controls XTAL Pull Down if XTAL_CONTROL_SW='1'. */
#define CLKGEN_HFO_XTAL_REG_XTAL_SPARE0_POS                                                         (7UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_VOLTAGE_MUX_CLOCK_MASK                                             (0x00000040UL) /**< rw - Controls XTAL Voltage Mux. */
#define CLKGEN_HFO_XTAL_REG_XTAL_VOLTAGE_MUX_CLOCK_POS                                              (6UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_SEL_EXTERNAL_CLOCK_MASK                                            (0x00000020UL) /**< rw - Controls XTAL External Clock Selection if XTAL_CONTROL_SW='1'. */
#define CLKGEN_HFO_XTAL_REG_XTAL_SEL_EXTERNAL_CLOCK_POS                                             (5UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_ENABLE_MASK                                                        (0x00000010UL) /**< rw - Controls XTAL Enable if XTAL_CONTROL_SW='1'. */
#define CLKGEN_HFO_XTAL_REG_XTAL_ENABLE_POS                                                         (4UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_ENABLE_KICK_MASK                                                   (0x00000008UL) /**< rw - Controls XTAL Enable Kick if XTAL_CONTROL_SW='1'. */
#define CLKGEN_HFO_XTAL_REG_XTAL_ENABLE_KICK_POS                                                    (3UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_BYPASS_MASK                                                        (0x00000004UL) /**< rw - Controls XTAL Bypass if XTAL_CONTROL_SW='1'. */
#define CLKGEN_HFO_XTAL_REG_XTAL_BYPASS_POS                                                         (2UL)
#define CLKGEN_HFO_XTAL_REG_XTAL_CONTROL_SW_MASK                                                    (0x00000002UL) /**< rw - High to control the XTAL Oscilator by SW. */
#define CLKGEN_HFO_XTAL_REG_XTAL_CONTROL_SW_POS                                                     (1UL)
#define CLKGEN_HFO_XTAL_REG_HFO_ENABLE_MASK                                                         (0x00000001UL) /**< rw - Enables the HFO (Activated by default). */
#define CLKGEN_HFO_XTAL_REG_HFO_ENABLE_POS                                                          (0UL)
#define CLKGEN_HFO_XTAL_REG__RESET_VALUE                                                            (0x00FFF001UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN HFO Control Register (RW)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_HFO_TRIMM_REG                                                                        (0x40010008UL) /**< rwm*/
#define CLKGEN_HFO_TRIMM_REG_HFO_TRIMM_MASK                                                         (0x0000001FUL) /**< rw - HFO Trimming Value. */
#define CLKGEN_HFO_TRIMM_REG_HFO_TRIMM_POS                                                          (0UL)
#define CLKGEN_HFO_TRIMM_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN USB_PLL Control Register
* <b>Reset value:</b> 0x00F90001\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_USB_PLL_CONTROL_REG                                                                  (0x4001000CUL) /**< rwm*/
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_MNPSEL_MASK                                              (0x40000000UL) /**< rw - M,N,P selection values for the Soft Decoder: When Low, M=600,N=113,P=3 and when high, M=92,N=13,P=4. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_MNPSEL_POS                                               (30UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_CLKOUT_SELECT_MASK                                       (0x30000000UL) /**< rw - Selects the Output Clock. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_CLKOUT_SELECT_POS                                        (28UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_REF_CLK_SELECT_MASK                                      (0x0C000000UL) /**< rw - Selects the reference clock for USB PLL. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_REF_CLK_SELECT_POS                                       (26UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_LOCK_BYPASS_MASK                                         (0x02000000UL) /**< rw - Bypass the USB_PLL Lock Output Status. Set to '1' the CLKGEN_STATUS_REG.USB_PLL_lock_overriden status bit.. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_LOCK_BYPASS_POS                                          (25UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_MNP_DEC_SELECTION_MASK                                   (0x01000000UL) /**< rw - When High, M,N,P divider ratio are not coming from the Soft Decoder but from the CLKGEN_USB_PLL_MDEC_WO_SOFTDEC and CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC registers. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_MNP_DEC_SELECTION_POS                                    (24UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_INSELP_MASK                                              (0x00F80000UL) /**< rw - Select the Bandwidth (don't care if USB_PLL_BANDSEL='0'). */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_INSELP_POS                                               (19UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_INSELI_MASK                                              (0x00078000UL) /**< rw - Select the Bandwidth (don't care if USB_PLL_BANDSEL='0'). */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_INSELI_POS                                               (15UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_INSELR_MASK                                              (0x00007800UL) /**< rw - Select the Bandwidth (don't care if USB_PLL_BANDSEL='0'). */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_INSELR_POS                                               (11UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_BANDSEL_MASK                                             (0x00000400UL) /**< rw - Bandwidth Adjustement (to modify externally the bandwidth of the USB_PLL). */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_BANDSEL_POS                                              (10UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_PREQ_MASK                                                (0x00000200UL) /**< rw - USB_PLL Post-Divider Ratio Change Request. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_PREQ_POS                                                 (9UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_NREQ_MASK                                                (0x00000100UL) /**< rw - USB_PLL Pre-Divider Ratio Change Request. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_NREQ_POS                                                 (8UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_MREQ_MASK                                                (0x00000080UL) /**< rw - USB_PLL Feedback Divider Ratio Change Request. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_MREQ_POS                                                 (7UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_FRM_MASK                                                 (0x00000040UL) /**< rw - USB_PLL Free Running Mode. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_FRM_POS                                                  (6UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_SKEW_EN_MASK                                             (0x00000020UL) /**< rw - USB_PLL Skew Mode. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_SKEW_EN_POS                                              (5UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_DIRECTO_MASK                                             (0x00000010UL) /**< rw - Bypass of the USB_PLL Post-Divider. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_DIRECTO_POS                                              (4UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_DIRECTI_MASK                                             (0x00000008UL) /**< rw - Bypass of the USB_PLL Pre-Divider. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_DIRECTI_POS                                              (3UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_BYPASS_MASK                                              (0x00000004UL) /**< rw - Bypass of the USB_PLL (clkout=clkin). */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_BYPASS_POS                                               (2UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_CLKEN_MASK                                               (0x00000002UL) /**< rw - Enable the USB_PLL Output Clock. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_CLKEN_POS                                                (1UL)
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_PD_MASK                                                  (0x00000001UL) /**< rw - USB_PLL Into Power Down. */
#define CLKGEN_USB_PLL_CONTROL_REG_USB_PLL_PD_POS                                                   (0UL)
#define CLKGEN_USB_PLL_CONTROL_REG__RESET_VALUE                                                     (0x00F90001UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN M Dividers Ratio Values Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_USB_PLL_MDEC_WO_SOFTDEC_REG                                                          (0x40010010UL) /**< rwm*/
#define CLKGEN_USB_PLL_MDEC_WO_SOFTDEC_REG_USB_MDEC_WO_SOFTDEC_MASK                                 (0x0001FFFFUL) /**< rw - Divider Ratio Code for M-divider when Soft Decoder is not used. */
#define CLKGEN_USB_PLL_MDEC_WO_SOFTDEC_REG_USB_MDEC_WO_SOFTDEC_POS                                  (0UL)
#define CLKGEN_USB_PLL_MDEC_WO_SOFTDEC_REG__RESET_VALUE                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN N and P Dividers Ratio Values Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC_REG                                                     (0x40010014UL) /**< rwm*/
#define CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC_REG_USB_PDEC_WO_SOFTDEC_MASK                            (0x0000FC00UL) /**< rw - Divider Ratio Code for P-divider when Soft Decoder is not used. */
#define CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC_REG_USB_PDEC_WO_SOFTDEC_POS                             (10UL)
#define CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC_REG_USB_NDEC_WO_SOFTDEC_MASK                            (0x000003FFUL) /**< rw - Divider Ratio Code for N-divider when Soft Decoder is not used. */
#define CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC_REG_USB_NDEC_WO_SOFTDEC_POS                             (0UL)
#define CLKGEN_USB_PLL_NDEC_PDEC_WO_SOFTDEC_REG__RESET_VALUE                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Clif pll usage configurations
* <b>Reset value:</b> 0x02E3B190\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_CLIF_PLL1_CONTROL_REG                                                                (0x40010018UL) /**< rwm*/
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_LIMUP_OFF1_MASK                                       (0x08000000UL) /**< rw - Pulse Limiter for CLIF_PLL 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_LIMUP_OFF1_POS                                        (27UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FREQ_LIM1_MASK                                        (0x04000000UL) /**< rw - Frequency Limiter for CLIF_PLL 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FREQ_LIM1_POS                                         (26UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_SELP1_MASK                                            (0x03000000UL) /**< rw - pins to select the BW of CLIF_PLL 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_SELP1_POS                                             (24UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_SELI1_MASK                                            (0x00C00000UL) /**< rw - pins to select the BW of CLIF_PLL 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_SELI1_POS                                             (22UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_SELR1_MASK                                            (0x00300000UL) /**< rw - pins to select the BW of CLIF_PLL 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_SELR1_POS                                             (20UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST2_P1_MASK                                    (0x00080000UL) /**< rw - Enable functional CLIF_PLL chain test of divider P1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST2_P1_POS                                     (19UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST1_P1_MASK                                    (0x00040000UL) /**< rw - Enable functional divider test of divider P1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST1_P1_POS                                     (18UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_DIVP1_MASK                                            (0x0003F000UL) /**< rw - Feedback divider ratio P1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_DIVP1_POS                                             (12UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST2_M1_MASK                                    (0x00000800UL) /**< rw - Enable functional CLIF_PLL test chain of divider M1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST2_M1_POS                                     (11UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST1_M1_MASK                                    (0x00000400UL) /**< rw - Enable functional divider test of divider M1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST1_M1_POS                                     (10UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_DIVM1_MASK                                            (0x000003F8UL) /**< rw - Feedback divider ratio M1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_DIVM1_POS                                             (3UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_BYPASS_LOCK1_MASK                                     (0x00000004UL) /**< rw - Bypass of Lock1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_BYPASS_LOCK1_POS                                      (2UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST2_LOCK1_MASK                                 (0x00000002UL) /**< rw - Enable functional CLIF_PLL test chain of lock detector 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST2_LOCK1_POS                                  (1UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST1_LOCK1_MASK                                 (0x00000001UL) /**< rw - Enable functional divider test of lock detector 1. */
#define CLKGEN_CLIF_PLL1_CONTROL_REG_CLIF_PLL_FUNC_TEST1_LOCK1_POS                                  (0UL)
#define CLKGEN_CLIF_PLL1_CONTROL_REG__RESET_VALUE                                                   (0x02E3B190UL) /**< value after reset */
/**
* @}
*/


/**
* \name Clif pll usage configurations
* <b>Reset value:</b> 0x02E121E0\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_CLIF_PLL2_CONTROL_REG                                                                (0x4001001CUL) /**< rwm*/
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_LIMUP_OFF2_MASK                                       (0x08000000UL) /**< rw - Pulse Limiter for CLIF_PLL 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_LIMUP_OFF2_POS                                        (27UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FREQ_LIM2_MASK                                        (0x04000000UL) /**< rw - Frequency Limiter for CLIF_PLL 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FREQ_LIM2_POS                                         (26UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_SELP2_MASK                                            (0x03000000UL) /**< rw - pins to select the BW of CLIF_PLL 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_SELP2_POS                                             (24UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_SELI2_MASK                                            (0x00C00000UL) /**< rw - pins to select the BW of CLIF_PLL 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_SELI2_POS                                             (22UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_SELR2_MASK                                            (0x00300000UL) /**< rw - pins to select the BW of CLIF_PLL 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_SELR2_POS                                             (20UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST2_P2_MASK                                    (0x00080000UL) /**< rw - Enable functional CLIF_PLL chain test of divider P2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST2_P2_POS                                     (19UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST1_P2_MASK                                    (0x00040000UL) /**< rw - Enable functional divider test of divider P2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST1_P2_POS                                     (18UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_DIVP2_MASK                                            (0x0003F000UL) /**< rw - Feedback divider ratio P2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_DIVP2_POS                                             (12UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST2_M2_MASK                                    (0x00000800UL) /**< rw - Enable functional CLIF_PLL test chain of divider M2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST2_M2_POS                                     (11UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST1_M2_MASK                                    (0x00000400UL) /**< rw - Enable functional divider test of divider M2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST1_M2_POS                                     (10UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_DIVM2_MASK                                            (0x000003F8UL) /**< rw - Feedback divider ratio M2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_DIVM2_POS                                             (3UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_BYPASS_LOCK2_MASK                                     (0x00000004UL) /**< rw - Bypass of Lock2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_BYPASS_LOCK2_POS                                      (2UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST2_LOCK2_MASK                                 (0x00000002UL) /**< rw - Enable functional CLIF_PLL test chain of lock detector 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST2_LOCK2_POS                                  (1UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST1_LOCK2_MASK                                 (0x00000001UL) /**< rw - Enable functional divider test of lock detector 2. */
#define CLKGEN_CLIF_PLL2_CONTROL_REG_CLIF_PLL_FUNC_TEST1_LOCK2_POS                                  (0UL)
#define CLKGEN_CLIF_PLL2_CONTROL_REG__RESET_VALUE                                                   (0x02E121E0UL) /**< value after reset */
/**
* @}
*/


/**
* \name Clif pll integration configurations
* <b>Reset value:</b> 0x000000C8\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG                                                          (0x40010020UL) /**< rwm*/
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_CLK_IN_OK_BYPASS_MASK                           (0x00004000UL) /**< rw - CLIF pll clk_in detection override. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_CLK_IN_OK_BYPASS_POS                            (14UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_REF_CLK_SELECT_MASK                             (0x00003000UL) /**< rw - CLIF_PLL input clock selector. 00:clk; 01: clk_xtal; 10:clk_hfo_40; 11: clock recovery. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_REF_CLK_SELECT_POS                              (12UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_ENABLE_TESTOUT_MASK                             (0x00000800UL) /**< rw - high to enable the CLIF_PLL output to be on the SMX_IF_pad. Low for HiZ. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_ENABLE_TESTOUT_POS                              (11UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_CLK_DETECT_ENABLE_MASK                              (0x00000400UL) /**< rw - Enables CLIF_PLL input clock detector (clk_in). Higher prio than xtal_detect_enable.. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_CLK_DETECT_ENABLE_POS                               (10UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_INPUT_FREQ_SEL_MASK                             (0x00000380UL) /**< rw - Select input frequency for the CLIF_PLL: 13, 19,2, 24, 26, 38.4 or 52MHz. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_INPUT_FREQ_SEL_POS                              (7UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_CLOCK_SELECT_MASK                               (0x00000060UL) /**< rw - 00: CLIF_PLL output clock 2; 01:xtal clock;10: CLIF_PLL output clock;11: CLIF_PLL input clock. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_CLOCK_SELECT_POS                                (5UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_PLL_INPUT_BUFFER_BYPASS_MASK                             (0x00000010UL) /**< rw - high to bypass PLL input buffer( the buffer for clock going into USB pll and intPLL). */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_PLL_INPUT_BUFFER_BYPASS_POS                              (4UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_PLL_INPUT_BUFFER_ENABLE_MASK                             (0x00000008UL) /**< rw - Enable the PLL Input Buffer ( the buffer for clock going into USB pll and intPLL). */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_PLL_INPUT_BUFFER_ENABLE_POS                              (3UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_FUNC_TEST_N1_MASK                               (0x00000004UL) /**< rw - Enable functional divider test and CLIF_PLL test chain of divider M1. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_FUNC_TEST_N1_POS                                (2UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_DIVN1_MASK                                      (0x00000002UL) /**< rw - Pre-divider selection. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_DIVN1_POS                                       (1UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_ENABLE_MASK                                     (0x00000001UL) /**< rw - Enable the CLIF_PLL. */
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG_CLIF_PLL_ENABLE_POS                                      (0UL)
#define CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG__RESET_VALUE                                             (0x000000C8UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN Input clock Detector Control Register
* <b>Reset value:</b> 0x0000100D\n
* <b>Access:</b> rw-
* @{
*/
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG                                                     (0x40010024UL) /**< rw-*/
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_USB_PLL_CLK_IN_OK_BYPASS_MASK                       (0x00004000UL) /**< rw - usb pll clk_in detection override. */
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_USB_PLL_CLK_IN_OK_BYPASS_POS                        (14UL)
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_USB_CLK_DETECT_ENABLE_MASK                          (0x00002000UL) /**< rw - Divider Ratio Code for P-divider when Soft Decoder is not used. */
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_USB_CLK_DETECT_ENABLE_POS                           (13UL)
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_INPUT_USB_CLOCK_EDGES_NUMBER_MASK                   (0x00001FE0UL) /**< rw - Divider Ratio Code for N-divider when Soft Decoder is not used. */
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_INPUT_USB_CLOCK_EDGES_NUMBER_POS                    (5UL)
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_DETECTION_WINDOW_LENGTH_MASK                        (0x0000001FUL) /**< rw - Divider Ratio Code for N-divider when Soft Decoder is not used. */
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG_DETECTION_WINDOW_LENGTH_POS                         (0UL)
#define CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG__RESET_VALUE                                        (0x0000100DUL) /**< value after reset */
/**
* @}
*/


/**
* \name CLKGEN Digital Testbus Control Register
* <b>Reset value:</b> 0x0000000F\n
* <b>Access:</b> rw-
* @{
*/
#define CLKGEN_DIG_TESTBUS_CONTROL_REG                                                              (0x40010028UL) /**< rw-*/
#define CLKGEN_DIG_TESTBUS_CONTROL_REG_DIG_TB_O_ENABLE_MASK                                         (0x0000000FUL) /**< rw - Enables the Corresponding Digital Output Testbus on the observability pins (Module ID is output by default). */
#define CLKGEN_DIG_TESTBUS_CONTROL_REG_DIG_TB_O_ENABLE_POS                                          (0UL)
#define CLKGEN_DIG_TESTBUS_CONTROL_REG__RESET_VALUE                                                 (0x0000000FUL) /**< value after reset */
/**
* @}
*/


/**
* \name Clock presense for clif_pll
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define CLKGEN_CLOCK_PRESENCE_BYPASS_REG                                                            (0x4001002CUL) /**< rwm*/
#define CLKGEN_CLOCK_PRESENCE_BYPASS_REG_CLOCK_PRESENCE_BYPASS_VAL_MASK                             (0x00000002UL) /**< rw -  Value to apply to clif_pll_lock2_o signal when corresponding enable bit is set.. */
#define CLKGEN_CLOCK_PRESENCE_BYPASS_REG_CLOCK_PRESENCE_BYPASS_VAL_POS                              (1UL)
#define CLKGEN_CLOCK_PRESENCE_BYPASS_REG_CLOCK_PRESENCE_BYPASS_ENABLE_MASK                          (0x00000001UL) /**< rw - Enables to bypass the clif_pll_lock2_o signal to the value stored in clock_presence_bypass_val. */
#define CLKGEN_CLOCK_PRESENCE_BYPASS_REG_CLOCK_PRESENCE_BYPASS_ENABLE_POS                           (0UL)
#define CLKGEN_CLOCK_PRESENCE_BYPASS_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_CLKGEN_H_INCLUDED */
