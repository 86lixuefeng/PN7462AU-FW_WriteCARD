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
#ifndef PN7462AU_IP_PMU_H_INCLUDED
#define PN7462AU_IP_PMU_H_INCLUDED

#include "PN7462AU/PN7462AU_pmu_rorw.h"



/**
* \name Indicating status of sequence or operation completion on different blocks: tempsensor/dcdc/ldo
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define PMU_STATUS_REG                                                                              (0x40008000UL) /**< r-m*/
#define PMU_STATUS_REG_MLDO_LOWPOWER_VBATBUF_MASK                                                   (0x01000000UL) /**< r- - high to indicate that the vbat ldo is in lowpower. */
#define PMU_STATUS_REG_MLDO_LOWPOWER_VBATBUF_POS                                                    (24UL)
#define PMU_STATUS_REG_SVDD_CURLIM_ACT_MASK                                                         (0x00800000UL) /**< r- - high to indicate that regulator is limiting the current. */
#define PMU_STATUS_REG_SVDD_CURLIM_ACT_POS                                                          (23UL)
#define PMU_STATUS_REG_TXLDO_5VMON_OK_MASK                                                          (0x00400000UL) /**< r- - TXLDO 5V monitor output high/low.. */
#define PMU_STATUS_REG_TXLDO_5VMON_OK_POS                                                           (22UL)
#define PMU_STATUS_REG_POK_VBUSP_MASK                                                               (0x00200000UL) /**< r- - output of vbusp monitor. */
#define PMU_STATUS_REG_POK_VBUSP_POS                                                                (21UL)
#define PMU_STATUS_REG_POK_VBUSMON2_MASK                                                            (0x00100000UL) /**< r- - output of vbus monitor2. */
#define PMU_STATUS_REG_POK_VBUSMON2_POS                                                             (20UL)
#define PMU_STATUS_REG_POK_PVDDOUT_MASK                                                             (0x00080000UL) /**< r- - output of pvdd_out monitor. */
#define PMU_STATUS_REG_POK_PVDDOUT_POS                                                              (19UL)
#define PMU_STATUS_REG_TEMPSENS_CAL_ERROR_MASK                                                      (0x00040000UL) /**< r- - Indicates a calibration error. Goes high if:- fine is at maximum value and tempsens_error_irq is not high. */
#define PMU_STATUS_REG_TEMPSENS_CAL_ERROR_POS                                                       (18UL)
#define PMU_STATUS_REG_TEMPSENS_CAL_OK_MASK                                                         (0x00020000UL) /**< r- - Temperature sensor calibration ok. Means calibration is done for current sensor. Note that this signal will rise for each trimmed sensor. Set and cleared by hardware.. */
#define PMU_STATUS_REG_TEMPSENS_CAL_OK_POS                                                          (17UL)
#define PMU_STATUS_REG_SCVDD_OVERLOAD_MASK                                                          (0x00000800UL) /**< r- - high when Ron is incremented. */
#define PMU_STATUS_REG_SCVDD_OVERLOAD_POS                                                           (11UL)
#define PMU_STATUS_REG_DCDC_OVERLOAD_MASK                                                           (0x00000400UL) /**< r- - high when Ron is decremented. */
#define PMU_STATUS_REG_DCDC_OVERLOAD_POS                                                            (10UL)
#define PMU_STATUS_REG_TXLDO_DET_OUT_MASK                                                           (0x000003F8UL) /**< r- - [6:0] - Output of State Machine detection . */
#define PMU_STATUS_REG_TXLDO_DET_OUT_POS                                                            (3UL)
#define PMU_STATUS_REG_TXLDO_DET_SM_OK_MASK                                                         (0x00000004UL) /**< r- - Signal that indicates to Digital & SW that State Machine work is done . */
#define PMU_STATUS_REG_TXLDO_DET_SM_OK_POS                                                          (2UL)
#define PMU_STATUS_REG_TXLDO_TVDD_OK_MASK                                                           (0x00000002UL) /**< r- - Signal that indicates State Machine work is done. */
#define PMU_STATUS_REG_TXLDO_TVDD_OK_POS                                                            (1UL)
#define PMU_STATUS_REG_TXLDO_DET_MASK                                                               (0x00000001UL) /**< r- - Indicates the detection threshold . */
#define PMU_STATUS_REG_TXLDO_DET_POS                                                                (0UL)
#define PMU_STATUS_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Enabling comparators and bang gaps
* <b>Reset value:</b> 0x0000040E\n
* <b>Access:</b> rwm
* @{
*/
#define PMU_BG_MON_CONTROL_REG                                                                      (0x40008004UL) /**< rwm*/
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDD_M_1V8_COMP_MASK                                          (0x00000400UL) /**< rw - Enable for PVDD_M 1.8V comparator. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDD_M_1V8_COMP_POS                                           (10UL)
#define PMU_BG_MON_CONTROL_REG_VBUSPMON_THRESHOLD_MASK                                              (0x00000300UL) /**< rw - 00 for 2.7V, 01 for 3V and 10 for 3.9V. */
#define PMU_BG_MON_CONTROL_REG_VBUSPMON_THRESHOLD_POS                                               (8UL)
#define PMU_BG_MON_CONTROL_REG_VBUSMON2_THRESHOLD_MASK                                              (0x00000080UL) /**< rw - 0 for 2.5V and 1 for 4V. */
#define PMU_BG_MON_CONTROL_REG_VBUSMON2_THRESHOLD_POS                                               (7UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_VBUSPMON_MASK                                                 (0x00000040UL) /**< rw - enable for vbusp monitor. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_VBUSPMON_POS                                                  (6UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_VBUSMON2_MASK                                                 (0x00000020UL) /**< rw - enable for vbus monitor2. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_VBUSMON2_POS                                                  (5UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDDOUTMON_MASK                                               (0x00000010UL) /**< rw - enable for pvddout monitor. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDDOUTMON_POS                                                (4UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDD_M_3V_COMP_MASK                                           (0x00000008UL) /**< rw - high to enable 3v comparator for PVDD_M. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDD_M_3V_COMP_POS                                            (3UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDD_3V_COMP_MASK                                             (0x00000004UL) /**< rw - high to enable 3v comparator for PVDD. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_PVDD_3V_COMP_POS                                              (2UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_BG_BUFFER_MASK                                                (0x00000002UL) /**< rw - high to enable reference for memories. */
#define PMU_BG_MON_CONTROL_REG_ENABLE_BG_BUFFER_POS                                                 (1UL)
#define PMU_BG_MON_CONTROL_REG_ENABLE_SVDD_VDHF_MASK                                                (0x00000001UL) /**< rw - high to enable SVDD supply from vdhf (switch between dvdd and svdd). */
#define PMU_BG_MON_CONTROL_REG_ENABLE_SVDD_VDHF_POS                                                 (0UL)
#define PMU_BG_MON_CONTROL_REG__RESET_VALUE                                                         (0x0000040EUL) /**< value after reset */
/**
* @}
*/


/**
* \name Register bits for TXLDO sequence for reader and card detection
* <b>Reset value:</b> 0x04000000\n
* <b>Access:</b> rwm
* @{
*/
#define PMU_TXLDO_CONTROL_REG                                                                       (0x40008008UL) /**< rwm*/
#define PMU_TXLDO_CONTROL_REG_TXLDO_ENABLE_LP2_MASK                                                 (0x10000000UL) /**< rw - Enable the 2nd regulator. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_ENABLE_LP2_POS                                                  (28UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_TVDD_VAL_TRIM_MASK                                              (0x0E000000UL) /**< rw - Trimming bits for TVDD Value at 4.5V. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_TVDD_VAL_TRIM_POS                                               (25UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_RSTN_SOURCE_SEL_MASK                                            (0x01000000UL) /**< rw - Source selection bit of TXLDO_digi resetn. 0: resetn source = rst_pcr_system_n 1: resetn source = (~rst_pcr_system_n  or txldo_enable). */
#define PMU_TXLDO_CONTROL_REG_TXLDO_RSTN_SOURCE_SEL_POS                                             (24UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_OVERCURRENT_EN_MASK                                             (0x00800000UL) /**< rw - high to enable overcurrent detection. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_OVERCURRENT_EN_POS                                              (23UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_SELECT_ANT_MASK                                                 (0x00600000UL) /**< rw - selection of antenna size 0: large 1: small. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_SELECT_ANT_POS                                                  (21UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_TEST_EN_MASK                                                    (0x00100000UL) /**< rw - Signal used only when testing TXLDO. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_TEST_EN_POS                                                     (20UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_TRIM_MASK                                                       (0x000F8000UL) /**< rw - Trim Signal for TXLDO current detection. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_TRIM_POS                                                        (15UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_DET_IN_MASK                                                     (0x00007F00UL) /**< rw - Selection of Current Detection value. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_DET_IN_POS                                                      (8UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_SELECT_MASK                                                     (0x000000E0UL) /**< rw - Selection of Tvdd supply 0: tvdd = 3.1V 1: tvdd = 2.7V. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_SELECT_POS                                                      (5UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_LIMITER_EN_MASK                                                 (0x00000010UL) /**< rw - Activates the current limiter . */
#define PMU_TXLDO_CONTROL_REG_TXLDO_LIMITER_EN_POS                                                  (4UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_LOW_POWER_EN_MASK                                               (0x00000008UL) /**< rw - Activates the low-power mode. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_LOW_POWER_EN_POS                                                (3UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_EN_DAC_SM_MASK                                                  (0x00000004UL) /**< rw - Selects current measurement using state machine. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_EN_DAC_SM_POS                                                   (2UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_DETECTOR_EN_MASK                                                (0x00000002UL) /**< rw - Activates the Current Detection. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_DETECTOR_EN_POS                                                 (1UL)
#define PMU_TXLDO_CONTROL_REG_TXLDO_ENABLE_MASK                                                     (0x00000001UL) /**< rw - Enable the whole block TXLDO  if seq not used. */
#define PMU_TXLDO_CONTROL_REG_TXLDO_ENABLE_POS                                                      (0UL)
#define PMU_TXLDO_CONTROL_REG__RESET_VALUE                                                          (0x04000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name LDO configurations : Enabling and current limit information
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define PMU_LDO_CONTROL_REG                                                                         (0x4000800CUL) /**< rwm*/
#define PMU_LDO_CONTROL_REG_SPARE_VCC_TRIM_MASK                                                     (0x00000F80UL) /**< rw - vcc trim bits. Currently unused. */
#define PMU_LDO_CONTROL_REG_SPARE_VCC_TRIM_POS                                                      (7UL)
#define PMU_LDO_CONTROL_REG_DCDC_ADD_MULT_MASK                                                      (0x00000040UL) /**< rw - Used to increment the multipliers of RON1 by 1. Useful after validation of current capability.. */
#define PMU_LDO_CONTROL_REG_DCDC_ADD_MULT_POS                                                       (6UL)
#define PMU_LDO_CONTROL_REG_VBUSP_HI_MASK                                                           (0x00000020UL) /**< rw - CT channel detects the presence and starts the CT sequence. HIGH: VBUSP > 3.9V. LOW: VBUSP < =3.9V.. */
#define PMU_LDO_CONTROL_REG_VBUSP_HI_POS                                                            (5UL)
#define PMU_LDO_CONTROL_REG_SVDD_ENABLE_SOFT_START_MASK                                             (0x00000010UL) /**< rw - To be filled from datasheet. */
#define PMU_LDO_CONTROL_REG_SVDD_ENABLE_SOFT_START_POS                                              (4UL)
#define PMU_LDO_CONTROL_REG_DISABLE_VCC_IPROT_MASK                                                  (0x00000008UL) /**< rw - Required to disable vcc current protection . */
#define PMU_LDO_CONTROL_REG_DISABLE_VCC_IPROT_POS                                                   (3UL)
#define PMU_LDO_CONTROL_REG_CT_VCC_IPROT_MASK                                                       (0x00000004UL) /**< r- - To be filled from datasheet. */
#define PMU_LDO_CONTROL_REG_CT_VCC_IPROT_POS                                                        (2UL)
#define PMU_LDO_CONTROL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Test bus selection
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG                                                            (0x40008010UL) /**< rwm*/
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG_ANA_TB_ENABLE_MASK                                         (0x00FF0000UL) /**< rw - analog testbus testpage selection. */
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG_ANA_TB_ENABLE_POS                                          (16UL)
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG_DIG_TB_O_ENABLE_MASK                                       (0x0000FF00UL) /**< rw - output digital testbus testpage selection. */
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG_DIG_TB_O_ENABLE_POS                                        (8UL)
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG_DIG_TB_I_ENABLE_MASK                                       (0x000000FFUL) /**< rw - input digital testbus testpage selection. */
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG_DIG_TB_I_ENABLE_POS                                        (0UL)
#define PMU_DFT_DYNAMIC_TEST_CONTROL_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name DFT test control configurations
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define PMU_DFT_STATIC_TEST_CONTROL_REG                                                             (0x40008014UL) /**< rwm*/
#define PMU_DFT_STATIC_TEST_CONTROL_REG_TXLDO_VDHFSUPVDD_MUX_MASK                                   (0x00000020UL) /**< rw - high to enable test path for dsldo_vbatsupvdd. */
#define PMU_DFT_STATIC_TEST_CONTROL_REG_TXLDO_VDHFSUPVDD_MUX_POS                                    (5UL)
#define PMU_DFT_STATIC_TEST_CONTROL_REG_TXLDO_VDHFSUPVDD_TEST_MASK                                  (0x00000010UL) /**< rw - txldo_vbatsupvdd signal for test. */
#define PMU_DFT_STATIC_TEST_CONTROL_REG_TXLDO_VDHFSUPVDD_TEST_POS                                   (4UL)
#define PMU_DFT_STATIC_TEST_CONTROL_REG_POK_VBAT_MUX_MASK                                           (0x00000008UL) /**< rw - high to enable test path for pok_vbat. */
#define PMU_DFT_STATIC_TEST_CONTROL_REG_POK_VBAT_MUX_POS                                            (3UL)
#define PMU_DFT_STATIC_TEST_CONTROL_REG_POK_VBAT_TEST_MASK                                          (0x00000004UL) /**< rw - pok_vbat signal for test. */
#define PMU_DFT_STATIC_TEST_CONTROL_REG_POK_VBAT_TEST_POS                                           (2UL)
#define PMU_DFT_STATIC_TEST_CONTROL_REG_MLDO_BG_OK_TEST_MASK                                        (0x00000002UL) /**< rw - test signal. */
#define PMU_DFT_STATIC_TEST_CONTROL_REG_MLDO_BG_OK_TEST_POS                                         (1UL)
#define PMU_DFT_STATIC_TEST_CONTROL_REG_MLDO_BG_OK_MUX_MASK                                         (0x00000001UL) /**< rw - high to enable the test signal of dsldo_bg_ok. */
#define PMU_DFT_STATIC_TEST_CONTROL_REG_MLDO_BG_OK_MUX_POS                                          (0UL)
#define PMU_DFT_STATIC_TEST_CONTROL_REG__RESET_VALUE                                                (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt(current limit or temperature) clear enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PMU_INTERRUPT_CLR_ENABLE_REG                                                                (0x4000BFD8UL) /**< -wm*/
#define PMU_INTERRUPT_CLR_ENABLE_REG_VBUSMON2_LOW_IRQ_CLEAR_ENABLE_MASK                             (0x00000100UL) /**< -x - Vbus monitor 2 going low interrupt clear enable. */
#define PMU_INTERRUPT_CLR_ENABLE_REG_VBUSMON2_LOW_IRQ_CLEAR_ENABLE_POS                              (8UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_TXLDO_5V_MON_IRQ_CLEAR_ENABLE_MASK                             (0x00000080UL) /**< -x - Txldo 5v monitor irq clear enable                                                                                                                                     . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_TXLDO_5V_MON_IRQ_CLEAR_ENABLE_POS                              (7UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_VCC_ILIM_ACT_IRQ_CLEAR_ENABLE_MASK                             (0x00000040UL) /**< -x - vcc current limiter active irq clear enable. Automatically cleared after 2 cycles if set by software                                                                  . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_VCC_ILIM_ACT_IRQ_CLEAR_ENABLE_POS                              (6UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_SVDD_CURLIM_ACT_IRQ_CLEAR_ENABLE_MASK                          (0x00000020UL) /**< -x - svdd current limiter active irq clear enable.Automatically cleared after 2 cycles if set by software                                                                  . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_SVDD_CURLIM_ACT_IRQ_CLEAR_ENABLE_POS                           (5UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_PVDD_IRQ_CLEAR_ENABLE_MASK                                     (0x00000010UL) /**< -x - pvdd irq clear enable.Automatically cleared after 2 cycles if set by software                                                                                         . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_PVDD_IRQ_CLEAR_ENABLE_POS                                      (4UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_DCDC_OVERLOAD_IRQ_CLEAR_ENABLE_MASK                            (0x00000008UL) /**< -x - dcdc overload clear enable.Automatically cleared after 2 cycles if set by software                                                                                    . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_DCDC_OVERLOAD_IRQ_CLEAR_ENABLE_POS                             (3UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_SCVDD_OVERLOAD_IRQ_CLEAR_ENABLE_MASK                           (0x00000004UL) /**< -x - scvdd overload clear enable.Automatically cleared after 2 cycles if set by software                                                                                   . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_SCVDD_OVERLOAD_IRQ_CLEAR_ENABLE_POS                            (2UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_CLEAR_ENABLE_MASK                        (0x00000002UL) /**< -x - txldo overcurrent irq clear enable.Automatically cleared after 2 cycles if set by software                                                                            . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_CLEAR_ENABLE_POS                         (1UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG_TEMPSENS_ERROR_IRQ_CLEAR_ENABLE_MASK                           (0x00000001UL) /**< -x - temperature sensor calibration irq clear enable.Automatically cleared after 2 cycles if set by software                                                               . */
#define PMU_INTERRUPT_CLR_ENABLE_REG_TEMPSENS_ERROR_IRQ_CLEAR_ENABLE_POS                            (0UL)
#define PMU_INTERRUPT_CLR_ENABLE_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt(current limit or temperature) set enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PMU_INTERRUPT_SET_ENABLE_REG                                                                (0x4000BFDCUL) /**< -wm*/
#define PMU_INTERRUPT_SET_ENABLE_REG_VBUSMON2_LOW_IRQ_SET_ENABLE_MASK                               (0x00000100UL) /**< -x - Vbus monitor 2 going low interrupt set enable. */
#define PMU_INTERRUPT_SET_ENABLE_REG_VBUSMON2_LOW_IRQ_SET_ENABLE_POS                                (8UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_TXLDO_5V_MON_IRQ_SET_ENABLE_MASK                               (0x00000080UL) /**< -x - TXLDO 5V MON irq set enable                                                                                                                                           . */
#define PMU_INTERRUPT_SET_ENABLE_REG_TXLDO_5V_MON_IRQ_SET_ENABLE_POS                                (7UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_VCC_ILIM_ACT_IRQ_SET_ENABLE_MASK                               (0x00000040UL) /**< -x - vcc current limiter active irq set enable. Automatically cleared after 2 cycles if set by software                                                                    . */
#define PMU_INTERRUPT_SET_ENABLE_REG_VCC_ILIM_ACT_IRQ_SET_ENABLE_POS                                (6UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_SVDD_CURLIM_ACT_IRQ_SET_ENABLE_MASK                            (0x00000020UL) /**< -x - svdd current limiter active irq set enable.Automatically cleared after 2 cycles if set by software                                                                    . */
#define PMU_INTERRUPT_SET_ENABLE_REG_SVDD_CURLIM_ACT_IRQ_SET_ENABLE_POS                             (5UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_PVDD_IRQ_SET_ENABLE_MASK                                       (0x00000010UL) /**< -x - pvdd irq set enable. Automatically cleared after 2 cycles if set by software                                                                                          . */
#define PMU_INTERRUPT_SET_ENABLE_REG_PVDD_IRQ_SET_ENABLE_POS                                        (4UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_DCDC_OVERLOAD_IRQ_SET_ENABLE_MASK                              (0x00000008UL) /**< -x - dcdc overload irq set enable.Automatically cleared after 2 cycles if set by software                                                                                  . */
#define PMU_INTERRUPT_SET_ENABLE_REG_DCDC_OVERLOAD_IRQ_SET_ENABLE_POS                               (3UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_SCVDD_OVERLOAD_IRQ_SET_ENABLE_MASK                             (0x00000004UL) /**< -x - scvdd overload irq set enable.Automatically cleared after 2 cycles if set by software                                                                                 . */
#define PMU_INTERRUPT_SET_ENABLE_REG_SCVDD_OVERLOAD_IRQ_SET_ENABLE_POS                              (2UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_SET_ENABLE_MASK                          (0x00000002UL) /**< -x - txldo_overcurrent irq set enable.Automatically cleared after 2 cycles if set by software                                                                              . */
#define PMU_INTERRUPT_SET_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_SET_ENABLE_POS                           (1UL)
#define PMU_INTERRUPT_SET_ENABLE_REG_TEMPSENS_ERROR_IRQ_SET_ENABLE_MASK                             (0x00000001UL) /**< -x - temperature sensor irq set enable.Automatically cleared after 2 cycles if set by software                                                                             . */
#define PMU_INTERRUPT_SET_ENABLE_REG_TEMPSENS_ERROR_IRQ_SET_ENABLE_POS                              (0UL)
#define PMU_INTERRUPT_SET_ENABLE_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt(current limit or temperature) status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define PMU_INTERRUPT_STATUS_REG                                                                    (0x4000BFE0UL) /**< r-m*/
#define PMU_INTERRUPT_STATUS_REG_VBUSMON2_LOW_IRQ_STATUS_MASK                                       (0x00000100UL) /**< r- - Vbus monitor 2 going low interrupt status. */
#define PMU_INTERRUPT_STATUS_REG_VBUSMON2_LOW_IRQ_STATUS_POS                                        (8UL)
#define PMU_INTERRUPT_STATUS_REG_TXLDO_5V_MON_IRQ_STATUS_MASK                                       (0x00000080UL) /**< r- - TXLDO_5V_MON_IRQ status register. .Automatically cleared after 2 cycles if set by software                                                                            . */
#define PMU_INTERRUPT_STATUS_REG_TXLDO_5V_MON_IRQ_STATUS_POS                                        (7UL)
#define PMU_INTERRUPT_STATUS_REG_VCC_ILIM_ACT_IRQ_STATUS_MASK                                       (0x00000040UL) /**< r- - vcc current limiter active irq status. Automatically cleared after 2 cycles if by software                                                                            . */
#define PMU_INTERRUPT_STATUS_REG_VCC_ILIM_ACT_IRQ_STATUS_POS                                        (6UL)
#define PMU_INTERRUPT_STATUS_REG_SVDD_CURLIM_ACT_IRQ_STATUS_MASK                                    (0x00000020UL) /**< r- - svdd current limiter active irq status.Automatically cleared after 2 cycles if by software                                                                            . */
#define PMU_INTERRUPT_STATUS_REG_SVDD_CURLIM_ACT_IRQ_STATUS_POS                                     (5UL)
#define PMU_INTERRUPT_STATUS_REG_PVDD_IRQ_STATUS_MASK                                               (0x00000010UL) /**< r- - pvdd irq status. Automatically cleared after 2 cycles if by software                                                                                                  . */
#define PMU_INTERRUPT_STATUS_REG_PVDD_IRQ_STATUS_POS                                                (4UL)
#define PMU_INTERRUPT_STATUS_REG_DCDC_OVERLOAD_IRQ_STATUS_MASK                                      (0x00000008UL) /**< r- - dcdc overload irq status.Automatically cleared after 2 cycles if by software                                                                                          . */
#define PMU_INTERRUPT_STATUS_REG_DCDC_OVERLOAD_IRQ_STATUS_POS                                       (3UL)
#define PMU_INTERRUPT_STATUS_REG_SCVDD_OVERLOAD_IRQ_STATUS_MASK                                     (0x00000004UL) /**< r- - scvdd overload irq status.Automatically cleared after 2 cycles if by software                                                                                         . */
#define PMU_INTERRUPT_STATUS_REG_SCVDD_OVERLOAD_IRQ_STATUS_POS                                      (2UL)
#define PMU_INTERRUPT_STATUS_REG_TXLDO_OVERCURRENT_IRQ_STATUS_MASK                                  (0x00000002UL) /**< r- - txldo_overcurrent irq status.Automatically cleared after 2 cycles if by software                                                                                      . */
#define PMU_INTERRUPT_STATUS_REG_TXLDO_OVERCURRENT_IRQ_STATUS_POS                                   (1UL)
#define PMU_INTERRUPT_STATUS_REG_TEMPSENS_ERROR_IRQ_STATUS_MASK                                     (0x00000001UL) /**< r- - temperature sensor irq status.Automatically cleared after 2 cycles if by software                                                                                     . */
#define PMU_INTERRUPT_STATUS_REG_TEMPSENS_ERROR_IRQ_STATUS_POS                                      (0UL)
#define PMU_INTERRUPT_STATUS_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt(current limit or temperature) enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define PMU_INTERRUPT_ENABLE_REG                                                                    (0x4000BFE4UL) /**< r-m*/
#define PMU_INTERRUPT_ENABLE_REG_VBUSMON2_LOW_IRQ_ENABLE_MASK                                       (0x00000100UL) /**< r- - Vbus monitor 2 going low interrupt enable. */
#define PMU_INTERRUPT_ENABLE_REG_VBUSMON2_LOW_IRQ_ENABLE_POS                                        (8UL)
#define PMU_INTERRUPT_ENABLE_REG_TXLDO_5V_MON_IRQ_ENABLE_MASK                                       (0x00000080UL) /**< r- - TXLDO_5V_MON_IRQ enable status register. .Automatically cleared after 2 cycles if set by software                                                                     . */
#define PMU_INTERRUPT_ENABLE_REG_TXLDO_5V_MON_IRQ_ENABLE_POS                                        (7UL)
#define PMU_INTERRUPT_ENABLE_REG_VCC_ILIM_ACT_IRQ_ENABLE_MASK                                       (0x00000040UL) /**< r- - vcc current limiter active irq enable. Automatically cleared after 2 cycles if by software                                                                            . */
#define PMU_INTERRUPT_ENABLE_REG_VCC_ILIM_ACT_IRQ_ENABLE_POS                                        (6UL)
#define PMU_INTERRUPT_ENABLE_REG_SVDD_CURLIM_ACT_IRQ_ENABLE_MASK                                    (0x00000020UL) /**< r- - svdd current limiter active irq enable.Automatically cleared after 2 cycles if by software                                                                            . */
#define PMU_INTERRUPT_ENABLE_REG_SVDD_CURLIM_ACT_IRQ_ENABLE_POS                                     (5UL)
#define PMU_INTERRUPT_ENABLE_REG_PVDD_IRQ_ENABLE_MASK                                               (0x00000010UL) /**< r- - pvdd irq enable. Automatically cleared after 2 cycles if by software                                                                                                  . */
#define PMU_INTERRUPT_ENABLE_REG_PVDD_IRQ_ENABLE_POS                                                (4UL)
#define PMU_INTERRUPT_ENABLE_REG_DCDC_OVERLOAD_IRQ_ENABLE_MASK                                      (0x00000008UL) /**< r- - dcdc overload irq enable.Automatically cleared after 2 cycles if by software                                                                                          . */
#define PMU_INTERRUPT_ENABLE_REG_DCDC_OVERLOAD_IRQ_ENABLE_POS                                       (3UL)
#define PMU_INTERRUPT_ENABLE_REG_SCVDD_OVERLOAD_IRQ_ENABLE_MASK                                     (0x00000004UL) /**< r- - scvdd overload irq enable.Automatically cleared after 2 cycles if by software                                                                                         . */
#define PMU_INTERRUPT_ENABLE_REG_SCVDD_OVERLOAD_IRQ_ENABLE_POS                                      (2UL)
#define PMU_INTERRUPT_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_ENABLE_MASK                                  (0x00000002UL) /**< r- - txldo_overcurrent irq enable.Automatically cleared after 2 cycles if by software                                                                                      . */
#define PMU_INTERRUPT_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_ENABLE_POS                                   (1UL)
#define PMU_INTERRUPT_ENABLE_REG_TEMPSENS_ERROR_IRQ_ENABLE_MASK                                     (0x00000001UL) /**< r- - temperature sensor irq enable.Automatically cleared after 2 cycles if by software                                                                                     . */
#define PMU_INTERRUPT_ENABLE_REG_TEMPSENS_ERROR_IRQ_ENABLE_POS                                      (0UL)
#define PMU_INTERRUPT_ENABLE_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt(current limit or temperature) clear status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PMU_INTERRUPT_CLR_STATUS_REG                                                                (0x4000BFE8UL) /**< -wm*/
#define PMU_INTERRUPT_CLR_STATUS_REG_VBUSMON2_LOW_IRQ_CLEAR_STATUS_MASK                             (0x00000100UL) /**< -x - Vbus monitor 2 going low interrupt clear status. */
#define PMU_INTERRUPT_CLR_STATUS_REG_VBUSMON2_LOW_IRQ_CLEAR_STATUS_POS                              (8UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_TXLDO_5V_MON_IRQ_CLEAR_STATUS_MASK                             (0x00000080UL) /**< -x - TXLDO_5V_MON_IRQ clear status register. .Automatically cleared after 2 cycles if set by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_TXLDO_5V_MON_IRQ_CLEAR_STATUS_POS                              (7UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_VCC_ILIM_ACT_IRQ_CLEAR_STATUS_MASK                             (0x00000040UL) /**< -x - vcc current limiter active irq clear status.Automatically cleared after 2 cycles if clear by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_VCC_ILIM_ACT_IRQ_CLEAR_STATUS_POS                              (6UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_SVDD_CURLIM_ACT_IRQ_CLEAR_STATUS_MASK                          (0x00000020UL) /**< -x - svdd current limiter active irq clear status.Automatically cleared after 2 cycles if clear by software                                                                . */
#define PMU_INTERRUPT_CLR_STATUS_REG_SVDD_CURLIM_ACT_IRQ_CLEAR_STATUS_POS                           (5UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_PVDD_IRQ_CLEAR_STATUS_MASK                                     (0x00000010UL) /**< -x - pvdd irq clear status.Automatically cleared after 2 cycles if clear by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_PVDD_IRQ_CLEAR_STATUS_POS                                      (4UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_DCDC_OVERLOAD_IRQ_CLEAR_STATUS_MASK                            (0x00000008UL) /**< -x - dcdc overload irq clear status.Automatically cleared after 2 cycles if clear by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_DCDC_OVERLOAD_IRQ_CLEAR_STATUS_POS                             (3UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_SCVDD_OVERLOAD_IRQ_CLEAR_STATUS_MASK                           (0x00000004UL) /**< -x - scvdd overload irq clear status.Automatically cleared after 2 cycles if clear by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_SCVDD_OVERLOAD_IRQ_CLEAR_STATUS_POS                            (2UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_TXLDO_OVERCURRENT_IRQ_CLEAR_STATUS_MASK                        (0x00000002UL) /**< -x - txldo_overcurrent irq clear status.Automatically cleared after 2 cycles if set by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_TXLDO_OVERCURRENT_IRQ_CLEAR_STATUS_POS                         (1UL)
#define PMU_INTERRUPT_CLR_STATUS_REG_TEMPSENS_ERROR_IRQ_CLEAR_STATUS_MASK                           (0x00000001UL) /**< -x - temp sensor irq clear status.Automatically cleared after 2 cycles if set by software. */
#define PMU_INTERRUPT_CLR_STATUS_REG_TEMPSENS_ERROR_IRQ_CLEAR_STATUS_POS                            (0UL)
#define PMU_INTERRUPT_CLR_STATUS_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt(current limit or temperature) set status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PMU_INTERRUPT_SET_STATUS_REG                                                                (0x4000BFECUL) /**< -wm*/
#define PMU_INTERRUPT_SET_STATUS_REG_VBUSMON2_LOW_IRQ_SET_STATUS_MASK                               (0x00000100UL) /**< -x - Vbus monitor 2 going low interrupt set status. */
#define PMU_INTERRUPT_SET_STATUS_REG_VBUSMON2_LOW_IRQ_SET_STATUS_POS                                (8UL)
#define PMU_INTERRUPT_SET_STATUS_REG_TXLDO_5V_MON_IRQ_SET_STATUS_MASK                               (0x00000080UL) /**< -x - TXLDO_5V_MON_IRQ set status register. .Automatically cleared after 2 cycles if set by software                                                                      . */
#define PMU_INTERRUPT_SET_STATUS_REG_TXLDO_5V_MON_IRQ_SET_STATUS_POS                                (7UL)
#define PMU_INTERRUPT_SET_STATUS_REG_VCC_ILIM_ACT_IRQ_SET_STATUS_MASK                               (0x00000040UL) /**< -x - vcc current limiter active irq set status.Automatically cleared after 2 cycles if set by software                                                                 . */
#define PMU_INTERRUPT_SET_STATUS_REG_VCC_ILIM_ACT_IRQ_SET_STATUS_POS                                (6UL)
#define PMU_INTERRUPT_SET_STATUS_REG_SVDD_CURLIM_ACT_IRQ_SET_STATUS_MASK                            (0x00000020UL) /**< -x - svdd current limiter active irq set status.Automatically cleared after 2 cycles if set by software                                                                . */
#define PMU_INTERRUPT_SET_STATUS_REG_SVDD_CURLIM_ACT_IRQ_SET_STATUS_POS                             (5UL)
#define PMU_INTERRUPT_SET_STATUS_REG_PVDD_IRQ_SET_STATUS_MASK                                       (0x00000010UL) /**< -x - pvdd irq set status.Automatically cleared after 2 cycles if set by software                                                                                       . */
#define PMU_INTERRUPT_SET_STATUS_REG_PVDD_IRQ_SET_STATUS_POS                                        (4UL)
#define PMU_INTERRUPT_SET_STATUS_REG_DCDC_OVERLOAD_IRQ_SET_STATUS_MASK                              (0x00000008UL) /**< -x - dcdc overload irq set status.Automatically cleared after 2 cycles if set by software                                                                              . */
#define PMU_INTERRUPT_SET_STATUS_REG_DCDC_OVERLOAD_IRQ_SET_STATUS_POS                               (3UL)
#define PMU_INTERRUPT_SET_STATUS_REG_SCVDD_OVERLOAD_IRQ_SET_STATUS_MASK                             (0x00000004UL) /**< -x - scvdd overload irq set status.Automatically cleared after 2 cycles if set by software                                                                             . */
#define PMU_INTERRUPT_SET_STATUS_REG_SCVDD_OVERLOAD_IRQ_SET_STATUS_POS                              (2UL)
#define PMU_INTERRUPT_SET_STATUS_REG_TXLDO_OVERCURRENT_IRQ_SET_STATUS_MASK                          (0x00000002UL) /**< -x - txldo_overcurrent irq set status.Automatically cleared after 2 cycles if set by software                                                                            . */
#define PMU_INTERRUPT_SET_STATUS_REG_TXLDO_OVERCURRENT_IRQ_SET_STATUS_POS                           (1UL)
#define PMU_INTERRUPT_SET_STATUS_REG_TEMPSENS_ERROR_IRQ_SET_STATUS_MASK                             (0x00000001UL) /**< -x - temp sensor irq set status.Automatically cleared after 2 cycles if set by software                                                                                  . */
#define PMU_INTERRUPT_SET_STATUS_REG_TEMPSENS_ERROR_IRQ_SET_STATUS_POS                              (0UL)
#define PMU_INTERRUPT_SET_STATUS_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_PMU_H_INCLUDED */
