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
#ifndef PN7462AU_IP_PCR_H_INCLUDED
#define PN7462AU_IP_PCR_H_INCLUDED

#include "PN7462AU/PN7462AU_pcr_rorw.h"



/**
* \name General purpose register 0 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG0_REG                                                                              (0x40024000UL) /**< rw-*/
#define PCR_GPREG0_REG_PCR_GPREG0_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG0_REG_PCR_GPREG0_POS                                                               (0UL)
#define PCR_GPREG0_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General purpose register 1 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG1_REG                                                                              (0x40024004UL) /**< rw-*/
#define PCR_GPREG1_REG_PCR_GPREG1_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG1_REG_PCR_GPREG1_POS                                                               (0UL)
#define PCR_GPREG1_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General purpose register 2 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG2_REG                                                                              (0x40024008UL) /**< rw-*/
#define PCR_GPREG2_REG_PCR_GPREG2_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG2_REG_PCR_GPREG2_POS                                                               (0UL)
#define PCR_GPREG2_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name System configuration like Hostif selection and CT enabling
* <b>Reset value:</b> 0x00000100\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_SYS_REG                                                                                 (0x4002400cUL) /**< rw-*/
#define PCR_SYS_REG_AUTOMATIC_CT_DEACT_MASK                                                         (0x00000800UL) /**< rw - Enables automatic initiation of ct deactivation sequence when VBUSP voltage goes below programmed range.. */
#define PCR_SYS_REG_AUTOMATIC_CT_DEACT_POS                                                          (11UL)
#define PCR_SYS_REG_AUTOMATIC_HPD_MASK                                                              (0x00000400UL) /**< rw - Enables PCR to go automatically into HPD state when the VBUS voltage goes below programmed voltage of 2.3V/2.7V. */
#define PCR_SYS_REG_AUTOMATIC_HPD_POS                                                               (10UL)
#define PCR_SYS_REG_PVDD_INT_MASK                                                                   (0x00000200UL) /**< rw - Indicatres that PVDD is being supplied using internal PVDD LDO. */
#define PCR_SYS_REG_PVDD_INT_POS                                                                    (9UL)
#define PCR_SYS_REG_ENABLE_CT_MASK                                                                  (0x00000100UL) /**< rw - enables the Contact interface.. */
#define PCR_SYS_REG_ENABLE_CT_POS                                                                   (8UL)
#define PCR_SYS_REG_PVDD_M_IRQ_VAL_MASK                                                             (0x00000080UL) /**< rw - Selects the PVDD_M voltage trigger level . */
#define PCR_SYS_REG_PVDD_M_IRQ_VAL_POS                                                              (7UL)
#define PCR_SYS_REG_PVDD_M_IRQ_EN_MASK                                                              (0x00000040UL) /**< rw - Enables the PVDD_M IRQ. */
#define PCR_SYS_REG_PVDD_M_IRQ_EN_POS                                                               (6UL)
#define PCR_SYS_REG_PVDD_IRQ_VAL_MASK                                                               (0x00000020UL) /**< rw - Selects the PVDD voltage trigger level . */
#define PCR_SYS_REG_PVDD_IRQ_VAL_POS                                                                (5UL)
#define PCR_SYS_REG_PVDD_IRQ_EN_MASK                                                                (0x00000010UL) /**< rw - Enables the PVDD IRQ. */
#define PCR_SYS_REG_PVDD_IRQ_EN_POS                                                                 (4UL)
#define PCR_SYS_REG_HOSTIF_SW_REGCONTROL_EN_MASK                                                    (0x00000008UL) /**< rw - Enabled control of USB D+,D- from ATX_A/B registers. */
#define PCR_SYS_REG_HOSTIF_SW_REGCONTROL_EN_POS                                                     (3UL)
#define PCR_SYS_REG_HIF_SELECTION_MASK                                                              (0x00000007UL) /**< rw - Host Interface Selection. */
#define PCR_SYS_REG_HIF_SELECTION_POS                                                               (0UL)
#define PCR_SYS_REG__RESET_VALUE                                                                    (0x00000100UL) /**< value after reset */
/**
* @}
*/


/**
* \name PMU interface. For LDO, bandgap,DCDC configuration and sequences
* <b>Reset value:</b> 0x0217010C\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PMU_REG                                                                                 (0x40024010UL) /**< rw-*/
#define PCR_PMU_REG_PBF_CONST_LOAD_VAL_MASK                                                         (0xE0000000UL) /**< rw - configuration bits for constant load on vdhf. */
#define PCR_PMU_REG_PBF_CONST_LOAD_VAL_POS                                                          (29UL)
#define PCR_PMU_REG_PBF_EN_CONST_LOAD_MASK                                                          (0x10000000UL) /**< rw - power down signal to connect/disconnect a constant load to vdhf. */
#define PCR_PMU_REG_PBF_EN_CONST_LOAD_POS                                                           (28UL)
#define PCR_PMU_REG_VBATMON_OVERRIDE_VAL_MASK                                                       (0x08000000UL) /**< rw - 0 for 2.7V and 1 for 2.3V. */
#define PCR_PMU_REG_VBATMON_OVERRIDE_VAL_POS                                                        (27UL)
#define PCR_PMU_REG_VBATMON_OVERRIDE_EN_MASK                                                        (0x04000000UL) /**< rw - enable for vbus monitor. */
#define PCR_PMU_REG_VBATMON_OVERRIDE_EN_POS                                                         (26UL)
#define PCR_PMU_REG_PD_PBF_FIELDSENS_MASK                                                           (0x02000000UL) /**< rw - Enable for pbf_pd_fieldsens. */
#define PCR_PMU_REG_PD_PBF_FIELDSENS_POS                                                            (25UL)
#define PCR_PMU_REG_BG_TRIM_A_MASK                                                                  (0x01000000UL) /**< rw - bandgap trim bit . */
#define PCR_PMU_REG_BG_TRIM_A_POS                                                                   (24UL)
#define PCR_PMU_REG_BG_TRIM_B_MASK                                                                  (0x00800000UL) /**< rw - bandgap trim bit . */
#define PCR_PMU_REG_BG_TRIM_B_POS                                                                   (23UL)
#define PCR_PMU_REG_BG_TRIM_C_MASK                                                                  (0x00400000UL) /**< rw - bandgap trim bit . */
#define PCR_PMU_REG_BG_TRIM_C_POS                                                                   (22UL)
#define PCR_PMU_REG_BG_TRIM_D_MASK                                                                  (0x00200000UL) /**< rw - bandgap trim bit . */
#define PCR_PMU_REG_BG_TRIM_D_POS                                                                   (21UL)
#define PCR_PMU_REG_SVDD_ENABLE_SOFT_START_MASK                                                     (0x00100000UL) /**< rw - enables soft start mode in svddldo. */
#define PCR_PMU_REG_SVDD_ENABLE_SOFT_START_POS                                                      (20UL)
#define PCR_PMU_REG_ENABLE_SVDD_VBAT_MASK                                                           (0x00080000UL) /**< rw - high to enable SVDD supply from vbat (regulator between vbat and svdd). */
#define PCR_PMU_REG_ENABLE_SVDD_VBAT_POS                                                            (19UL)
#define PCR_PMU_REG_SVDD_LIMITER_DISABLE_MASK                                                       (0x00040000UL) /**< rw - Disable SVDD Limiter. */
#define PCR_PMU_REG_SVDD_LIMITER_DISABLE_POS                                                        (18UL)
#define PCR_PMU_REG_MLDO_LOWPOWER_BG_VAL_MASK                                                       (0x00020000UL) /**< rw - value of mldo bandgap lowpower. */
#define PCR_PMU_REG_MLDO_LOWPOWER_BG_VAL_POS                                                        (17UL)
#define PCR_PMU_REG_MLDO_LOWPOWER_BG_EN_MASK                                                        (0x00010000UL) /**< rw - controls mldo bandgap lowpower signals.. */
#define PCR_PMU_REG_MLDO_LOWPOWER_BG_EN_POS                                                         (16UL)
#define PCR_PMU_REG_MLDO_LOWPOWER_VAL_MASK                                                          (0x00008000UL) /**< rw - value of mldo_lowpower signal. */
#define PCR_PMU_REG_MLDO_LOWPOWER_VAL_POS                                                           (15UL)
#define PCR_PMU_REG_MLDO_LOWPOWER_EN_MASK                                                           (0x00004000UL) /**< rw - controls mldo lowpower signals. */
#define PCR_PMU_REG_MLDO_LOWPOWER_EN_POS                                                            (14UL)
#define PCR_PMU_REG_DCDC_OFF_MASK                                                                   (0x00002000UL) /**< rw - To set the DC-DC in high impedance state enabling the testing of VCCLDO. VUP will be forced from outside.. */
#define PCR_PMU_REG_DCDC_OFF_POS                                                                    (13UL)
#define PCR_PMU_REG_TXLDO_STANDBY_CLEAR_MASK                                                        (0x00000800UL) /**< rw - TXLDO clear standby mode. */
#define PCR_PMU_REG_TXLDO_STANDBY_CLEAR_POS                                                         (11UL)
#define PCR_PMU_REG_TXLDO_ENABLE_STANDBY_MASK                                                       (0x00000400UL) /**< rw - TXLDO enable standby mode. */
#define PCR_PMU_REG_TXLDO_ENABLE_STANDBY_POS                                                        (10UL)
#define PCR_PMU_REG_MLDO_TRIM_IREF_SWP_MASK                                                         (0x00000200UL) /**< rw - compensation bit for swp pad. */
#define PCR_PMU_REG_MLDO_TRIM_IREF_SWP_POS                                                          (9UL)
#define PCR_PMU_REG_IBIAS_TRIMM_MASK                                                                (0x000001F0UL) /**< rw - IBIAS Trimm Value. */
#define PCR_PMU_REG_IBIAS_TRIMM_POS                                                                 (4UL)
#define PCR_PMU_REG_PVDDLDO_MODE_MASK                                                               (0x0000000CUL) /**< rw - Selects pvddldo mode normal/low power/soft start/power down. */
#define PCR_PMU_REG_PVDDLDO_MODE_POS                                                                (2UL)
#define PCR_PMU_REG__RESET_VALUE                                                                    (0x0217010CUL) /**< value after reset */
/**
* @}
*/


/**
* \name CLIF configuration
* <b>Reset value:</b> 0x00004032\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_RFLD_REG                                                                                (0x40024014UL) /**< rw-*/
#define PCR_RFLD_REG_RFLD_MASK_MODE_MASK                                                            (0x00010000UL) /**< rw - RFU. */
#define PCR_RFLD_REG_RFLD_MASK_MODE_POS                                                             (16UL)
#define PCR_RFLD_REG_RFLD_ENVDET_BOOST_MASK                                                         (0x00008000UL) /**< rw - Higher bias current for the env-detector. */
#define PCR_RFLD_REG_RFLD_ENVDET_BOOST_POS                                                          (15UL)
#define PCR_RFLD_REG_RFLD_COMP_BOOST_MASK                                                           (0x00004000UL) /**< rw - Higher bias current for comparator. */
#define PCR_RFLD_REG_RFLD_COMP_BOOST_POS                                                            (14UL)
#define PCR_RFLD_REG_RFLD_BIAS_ADPT_ENABLE_MASK                                                     (0x00002000UL) /**< rw - Enable of automatic bias current regulation. */
#define PCR_RFLD_REG_RFLD_BIAS_ADPT_ENABLE_POS                                                      (13UL)
#define PCR_RFLD_REG_RFLD_DRV_ENABLE_MASK                                                           (0x00001000UL) /**< rw - Enable the chopper clock driver for the RF Level Detector. . */
#define PCR_RFLD_REG_RFLD_DRV_ENABLE_POS                                                            (12UL)
#define PCR_RFLD_REG_RFLD_FILTER_ENABLE_MASK                                                        (0x00000800UL) /**< rw - Enable the chopper fillter for RF Level Detector. */
#define PCR_RFLD_REG_RFLD_FILTER_ENABLE_POS                                                         (11UL)
#define PCR_RFLD_REG_RFLD_COMP_ENABLE_MASK                                                          (0x00000400UL) /**< rw - Enable of RFLD comparator. */
#define PCR_RFLD_REG_RFLD_COMP_ENABLE_POS                                                           (10UL)
#define PCR_RFLD_REG_RFLD_ENVDET_ENABLE_MASK                                                        (0x00000200UL) /**< rw - Enable of RFLD envelope detector. */
#define PCR_RFLD_REG_RFLD_ENVDET_ENABLE_POS                                                         (9UL)
#define PCR_RFLD_REG_RFLD_VREF_ENABLE_MASK                                                          (0x00000100UL) /**< rw - Enable for RFLD reference voltage generator. */
#define PCR_RFLD_REG_RFLD_VREF_ENABLE_POS                                                           (8UL)
#define PCR_RFLD_REG_RFLD_REF_LO_MASK                                                               (0x000000F0UL) /**< rw - Higher Reference Value for RF Level Detector. */
#define PCR_RFLD_REG_RFLD_REF_LO_POS                                                                (4UL)
#define PCR_RFLD_REG_RFLD_REF_HI_MASK                                                               (0x0000000FUL) /**< rw - Lower Reference Value for RF Level Detector. */
#define PCR_RFLD_REG_RFLD_REF_HI_POS                                                                (0UL)
#define PCR_RFLD_REG__RESET_VALUE                                                                   (0x00004032UL) /**< value after reset */
/**
* @}
*/


/**
* \name Temperature sensor calibration information
* <b>Reset value:</b> 0x00058888\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_TEMP_REG                                                                                (0x40024018UL) /**< rw-*/
#define PCR_TEMP_REG_TEMP_ENABLE_CAL_1_MASK                                                         (0x02000000UL) /**< rw - Enable Calibration of Temperature Sensor 2. */
#define PCR_TEMP_REG_TEMP_ENABLE_CAL_1_POS                                                          (25UL)
#define PCR_TEMP_REG_TEMP_ENABLE_CAL_0_MASK                                                         (0x01000000UL) /**< rw - Enable Calibration of Temperature Sensor 1. */
#define PCR_TEMP_REG_TEMP_ENABLE_CAL_0_POS                                                          (24UL)
#define PCR_TEMP_REG_TEMP_ENABLE_HYST_1_MASK                                                        (0x00800000UL) /**< rw - Enable Hystere of Temperature Sensor 2. */
#define PCR_TEMP_REG_TEMP_ENABLE_HYST_1_POS                                                         (23UL)
#define PCR_TEMP_REG_TEMP_ENABLE_HYST_0_MASK                                                        (0x00400000UL) /**< rw - Enable Hystere of Temperature Sensor 1. */
#define PCR_TEMP_REG_TEMP_ENABLE_HYST_0_POS                                                         (22UL)
#define PCR_TEMP_REG_TEMP_ENABLE_1_MASK                                                             (0x00200000UL) /**< rw - Enable Temp Sensor 2. */
#define PCR_TEMP_REG_TEMP_ENABLE_1_POS                                                              (21UL)
#define PCR_TEMP_REG_TEMP_ENABLE_0_MASK                                                             (0x00100000UL) /**< rw - Enable Temp Sensor 1. */
#define PCR_TEMP_REG_TEMP_ENABLE_0_POS                                                              (20UL)
#define PCR_TEMP_REG_TEMP_DELTA_1_MASK                                                              (0x000C0000UL) /**< rw - selects temperatur threshold detection for temperature sensor 2. */
#define PCR_TEMP_REG_TEMP_DELTA_1_POS                                                               (18UL)
#define PCR_TEMP_REG_TEMP_DELTA_0_MASK                                                              (0x00030000UL) /**< rw - selects temperatur threshold detection for temperature sensor 1. */
#define PCR_TEMP_REG_TEMP_DELTA_0_POS                                                               (16UL)
#define PCR_TEMP_REG_TEMP_CAL_FINE_1_MASK                                                           (0x0000F000UL) /**< rw - trim value fine for temperatre sensor 2. */
#define PCR_TEMP_REG_TEMP_CAL_FINE_1_POS                                                            (12UL)
#define PCR_TEMP_REG_TEMP_CAL_COURSE_1_MASK                                                         (0x00000F00UL) /**< rw - trim value course for temperature sensor 2. */
#define PCR_TEMP_REG_TEMP_CAL_COURSE_1_POS                                                          (8UL)
#define PCR_TEMP_REG_TEMP_CAL_FINE_0_MASK                                                           (0x000000F0UL) /**< rw - trim value fine for temperatre sensor 1. */
#define PCR_TEMP_REG_TEMP_CAL_FINE_0_POS                                                            (4UL)
#define PCR_TEMP_REG_TEMP_CAL_COURSE_0_MASK                                                         (0x0000000FUL) /**< rw - trim value course for temperature sensor 1. */
#define PCR_TEMP_REG_TEMP_CAL_COURSE_0_POS                                                          (0UL)
#define PCR_TEMP_REG__RESET_VALUE                                                                   (0x00058888UL) /**< value after reset */
/**
* @}
*/


/**
* \name Configuring Wakeup source for Standby and Suspend
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_HOSTIF_WAKEUP_CFG_REG                                                                   (0x4002401cUL) /**< rw-*/
#define PCR_HOSTIF_WAKEUP_CFG_REG_I2C_ADDR_MASK                                                     (0x000001FCUL) /**< rw - I2C address for wakeup. */
#define PCR_HOSTIF_WAKEUP_CFG_REG_I2C_ADDR_POS                                                      (2UL)
#define PCR_HOSTIF_WAKEUP_CFG_REG_EN_SMBUS_ADDRESS_RESP_MASK                                        (0x00000002UL) /**< rw - Enable SMBUS address ACK response. */
#define PCR_HOSTIF_WAKEUP_CFG_REG_EN_SMBUS_ADDRESS_RESP_POS                                         (1UL)
#define PCR_HOSTIF_WAKEUP_CFG_REG_EN_INTERFACE_MASK                                                 (0x00000001UL) /**< rw - Enable Wakeup Host Interface. */
#define PCR_HOSTIF_WAKEUP_CFG_REG_EN_INTERFACE_POS                                                  (0UL)
#define PCR_HOSTIF_WAKEUP_CFG_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Configuring Wakeup source for Standby and Suspend
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_WAKEUP_CFG_REG                                                                          (0x40024020UL) /**< rw-*/
#define PCR_WAKEUP_CFG_REG_EN_ADV_RFLD_MASK                                                         (0x00400000UL) /**< rw - Enable Advanced RF Level Detector. */
#define PCR_WAKEUP_CFG_REG_EN_ADV_RFLD_POS                                                          (22UL)
#define PCR_WAKEUP_CFG_REG_EN_VBUS_LOW_MASK                                                         (0x00200000UL) /**< rw - Enable wakeup when vbus goes low. */
#define PCR_WAKEUP_CFG_REG_EN_VBUS_LOW_POS                                                          (21UL)
#define PCR_WAKEUP_CFG_REG_WUC_VALUE_MASK                                                           (0x001FF800UL) /**< rw - Wakeup Timer Value. */
#define PCR_WAKEUP_CFG_REG_WUC_VALUE_POS                                                            (11UL)
#define PCR_WAKEUP_CFG_REG_EN_TVDD_MON_MASK                                                         (0x00000400UL) /**< rw - Enable wakeup from TVDD 5V monitor . */
#define PCR_WAKEUP_CFG_REG_EN_TVDD_MON_POS                                                          (10UL)
#define PCR_WAKEUP_CFG_REG_EN_INT_AUX_MASK                                                          (0x00000200UL) /**< rw - Enable wakeup is TDA (CTUART) gives an level high interrupt . */
#define PCR_WAKEUP_CFG_REG_EN_INT_AUX_POS                                                           (9UL)
#define PCR_WAKEUP_CFG_REG_EN_CT_PR_MASK                                                            (0x00000100UL) /**< rw - Enables wakeup if card is detected.. */
#define PCR_WAKEUP_CFG_REG_EN_CT_PR_POS                                                             (8UL)
#define PCR_WAKEUP_CFG_REG_EN_SVDD_LIMITER_MASK                                                     (0x00000080UL) /**< rw - Enables wakeup if SVDD current limiter is risen. */
#define PCR_WAKEUP_CFG_REG_EN_SVDD_LIMITER_POS                                                      (7UL)
#define PCR_WAKEUP_CFG_REG_EN_PVDD_LIMITER_MASK                                                     (0x00000040UL) /**< rw - Enables wakeup if PVDD current limiter is risen. */
#define PCR_WAKEUP_CFG_REG_EN_PVDD_LIMITER_POS                                                      (6UL)
#define PCR_WAKEUP_CFG_REG_EN_GPIO_INT_MASK                                                         (0x00000020UL) /**< rw - Enables wakeup if GPIO gives any input. */
#define PCR_WAKEUP_CFG_REG_EN_GPIO_INT_POS                                                          (5UL)
#define PCR_WAKEUP_CFG_REG_EN_TEMP1_MASK                                                            (0x00000010UL) /**< rw - Enable Temperature 1 Error Wakeup. */
#define PCR_WAKEUP_CFG_REG_EN_TEMP1_POS                                                             (4UL)
#define PCR_WAKEUP_CFG_REG_EN_TEMP0_MASK                                                            (0x00000008UL) /**< rw - Enable Temperature 0 Error Wakeup. */
#define PCR_WAKEUP_CFG_REG_EN_TEMP0_POS                                                             (3UL)
#define PCR_WAKEUP_CFG_REG_EN_RFLDT_MASK                                                            (0x00000004UL) /**< rw - Enable Wakeup RF Level Detector. */
#define PCR_WAKEUP_CFG_REG_EN_RFLDT_POS                                                             (2UL)
#define PCR_WAKEUP_CFG_REG_EN_SWP_MASK                                                              (0x00000002UL) /**< rw - Enable Wakeup SWP. */
#define PCR_WAKEUP_CFG_REG_EN_SWP_POS                                                               (1UL)
#define PCR_WAKEUP_CFG_REG_EN_WUC_MASK                                                              (0x00000001UL) /**< rw - Enable Wakeup Timer. */
#define PCR_WAKEUP_CFG_REG_EN_WUC_POS                                                               (0UL)
#define PCR_WAKEUP_CFG_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Configuring Wakeup source for Standby and Suspend
* <b>Reset value:</b> 0x000000FF\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPIO_WAKEUP_CFG_REG                                                                     (0x40024024UL) /**< rw-*/
#define PCR_GPIO_WAKEUP_CFG_REG_GPIO_WAKEUP_ENABLE_MASK                                             (0x00000FFFUL) /**< rw - enables wakeup by the corresponding gpio 0-->gpio1 1-->gpio2 , e.t.c.. */
#define PCR_GPIO_WAKEUP_CFG_REG_GPIO_WAKEUP_ENABLE_POS                                              (0UL)
#define PCR_GPIO_WAKEUP_CFG_REG__RESET_VALUE                                                        (0x000000FFUL) /**< value after reset */
/**
* @}
*/


/**
* \name Bootup register with important values to be checked during bootup
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_BOOT_REG                                                                                (0x40024028UL) /**< rw-*/
#define PCR_BOOT_REG_ARM_FIREWALL_EXCEPTION_MASK                                                    (0x80000000UL) /**< rw - Indicates arm firewall exception occurred.. */
#define PCR_BOOT_REG_ARM_FIREWALL_EXCEPTION_POS                                                     (31UL)
#define PCR_BOOT_REG_USB_VBUS_OK_MASK                                                               (0x40000000UL) /**< r- - Indicator for USB_VBUS is ok. */
#define PCR_BOOT_REG_USB_VBUS_OK_POS                                                                (30UL)
#define PCR_BOOT_REG_POK_VBUS_MASK                                                                  (0x20000000UL) /**< r- - Indicator when VBUS > VBUSCritical when VBUSMonitor is enabled. */
#define PCR_BOOT_REG_POK_VBUS_POS                                                                   (29UL)
#define PCR_BOOT_REG_POK_PVDD_M_3V_MASK                                                             (0x10000000UL) /**< r- - Indicator for more than 3V at PVDD_M pin. */
#define PCR_BOOT_REG_POK_PVDD_M_3V_POS                                                              (28UL)
#define PCR_BOOT_REG_POK_PVDD_3V_MASK                                                               (0x08000000UL) /**< r- - Indicator for more than 3V at PVDD pin. */
#define PCR_BOOT_REG_POK_PVDD_3V_POS                                                                (27UL)
#define PCR_BOOT_REG_STBY_PREV_REASON_MASK                                                          (0x03C00000UL) /**< r- - Standby Prevention Reason. */
#define PCR_BOOT_REG_STBY_PREV_REASON_POS                                                           (22UL)
#define PCR_BOOT_REG_BOOT_REASON_MASK                                                               (0x003FFFFCUL) /**< r- - Bootup Reason. */
#define PCR_BOOT_REG_BOOT_REASON_POS                                                                (2UL)
#define PCR_BOOT_REG_POK_PVDD_M_MASK                                                                (0x00000002UL) /**< r- - Indicator if PVDD_m is available. */
#define PCR_BOOT_REG_POK_PVDD_M_POS                                                                 (1UL)
#define PCR_BOOT_REG_POK_PVDD_MASK                                                                  (0x00000001UL) /**< r- - Indicator if PVDD is available. */
#define PCR_BOOT_REG_POK_PVDD_POS                                                                   (0UL)
#define PCR_BOOT_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Control register to enable standby/suspend/soft-restart/clearing boot register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_CTRL_REG                                                                                (0x4002402CUL) /**< rw-*/
#define PCR_CTRL_REG_USB_VBUS_PULLDOWN_MASK                                                         (0x00000020UL) /**< rw - Enables the internal pulldown resistance to pulldown the USB_VBUS. */
#define PCR_CTRL_REG_USB_VBUS_PULLDOWN_POS                                                          (5UL)
#define PCR_CTRL_REG_CLR_BOOT_REGS_MASK                                                             (0x00000010UL) /**< -x - Clearing Standby Prevention and Bootup register values in the PCR_BOOT_REG register. */
#define PCR_CTRL_REG_CLR_BOOT_REGS_POS                                                              (4UL)
#define PCR_CTRL_REG_SUSPEND_MASK                                                                   (0x00000004UL) /**< -x - Enables entering suspend mode. */
#define PCR_CTRL_REG_SUSPEND_POS                                                                    (2UL)
#define PCR_CTRL_REG_STANDBY_MASK                                                                   (0x00000002UL) /**< -x - Entering Standby Mode. */
#define PCR_CTRL_REG_STANDBY_POS                                                                    (1UL)
#define PCR_CTRL_REG_SOFT_RESET_MASK                                                                (0x00000001UL) /**< -x - Trigger Soft Reset Source. */
#define PCR_CTRL_REG_SOFT_RESET_POS                                                                 (0UL)
#define PCR_CTRL_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Enable, gating, division value selection for clocks going to different Ips
* <b>Reset value:</b> 0x008FFE08\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_CLK_CFG_REG                                                                             (0x40024030UL) /**< rw-*/
#define PCR_CLK_CFG_REG_EECTRL_SYS_GATING_ENABLE_MASK                                               (0x40000000UL) /**< rw - EEPROM controller system clock gating enable. */
#define PCR_CLK_CFG_REG_EECTRL_SYS_GATING_ENABLE_POS                                                (30UL)
#define PCR_CLK_CFG_REG_EECTRL_PF_GATING_ENABLE_MASK                                                (0x20000000UL) /**< rw - EEPROM controller page flash clock gating enable. */
#define PCR_CLK_CFG_REG_EECTRL_PF_GATING_ENABLE_POS                                                 (29UL)
#define PCR_CLK_CFG_REG_EECTRL_EEPROM_GATING_ENABLE_MASK                                            (0x10000000UL) /**< rw - EEPROM controller automatic clock gating enable. */
#define PCR_CLK_CFG_REG_EECTRL_EEPROM_GATING_ENABLE_POS                                             (28UL)
#define PCR_CLK_CFG_REG_IPCLOCK_CTIF_ENABLE_MASK                                                    (0x08000000UL) /**< rw - Contact interface IP clock enable. */
#define PCR_CLK_CFG_REG_IPCLOCK_CTIF_ENABLE_POS                                                     (27UL)
#define PCR_CLK_CFG_REG_IPCLOCK_HSUART_ENABLE_MASK                                                  (0x04000000UL) /**< rw - High speed uart IP clock enable. */
#define PCR_CLK_CFG_REG_IPCLOCK_HSUART_ENABLE_POS                                                   (26UL)
#define PCR_CLK_CFG_REG_IPCLOCK_SPIM_ENABLE_MASK                                                    (0x02000000UL) /**< rw - SPI master uart IP clock enable. */
#define PCR_CLK_CFG_REG_IPCLOCK_SPIM_ENABLE_POS                                                     (25UL)
#define PCR_CLK_CFG_REG_IPCLOCK_I2CM_ENABLE_MASK                                                    (0x01000000UL) /**< rw - I2C master uart IP clock enable. */
#define PCR_CLK_CFG_REG_IPCLOCK_I2CM_ENABLE_POS                                                     (24UL)
#define PCR_CLK_CFG_REG_CLOCK_CTIF_ENABLE_MASK                                                      (0x00800000UL) /**< rw - Enable contact source for Contact interface. */
#define PCR_CLK_CFG_REG_CLOCK_CTIF_ENABLE_POS                                                       (23UL)
#define PCR_CLK_CFG_REG_I2CM_CLOCK_GATING_ENABLE_MASK                                               (0x00400000UL) /**< rw - Enable clock source for I2C master. */
#define PCR_CLK_CFG_REG_I2CM_CLOCK_GATING_ENABLE_POS                                                (22UL)
#define PCR_CLK_CFG_REG_CPU_CLKREQ_ENABLE_MASK                                                      (0x00200000UL) /**< rw - Enable  the automatic clock request for ROM and RAM via the CPU. */
#define PCR_CLK_CFG_REG_CPU_CLKREQ_ENABLE_POS                                                       (21UL)
#define PCR_CLK_CFG_REG_AUTOMATIC_CLOCKSTOP_AT_IDLE_ENABLE_MASK                                     (0x00100000UL) /**< rw - Enable automatic clock gating for CRC, EECTRL, RNG and ROM when cpu is in idle mode. */
#define PCR_CLK_CFG_REG_AUTOMATIC_CLOCKSTOP_AT_IDLE_ENABLE_POS                                      (20UL)
#define PCR_CLK_CFG_REG_CLOCK_SPIM_ENABLE_MASK                                                      (0x00080000UL) /**< rw - enable clock source for SPIM. */
#define PCR_CLK_CFG_REG_CLOCK_SPIM_ENABLE_POS                                                       (19UL)
#define PCR_CLK_CFG_REG_CLOCK_DFT_ENABLE_MASK                                                       (0x00040000UL) /**< rw - enable clock source for DFT. */
#define PCR_CLK_CFG_REG_CLOCK_DFT_ENABLE_POS                                                        (18UL)
#define PCR_CLK_CFG_REG_CLOCK_HOSTIF_ENABLE_MASK                                                    (0x00020000UL) /**< rw - enable clock source for HOSTIF. */
#define PCR_CLK_CFG_REG_CLOCK_HOSTIF_ENABLE_POS                                                     (17UL)
#define PCR_CLK_CFG_REG_CLOCK_TIMER_ENABLE_MASK                                                     (0x00010000UL) /**< rw - enable clock source for TIMER. */
#define PCR_CLK_CFG_REG_CLOCK_TIMER_ENABLE_POS                                                      (16UL)
#define PCR_CLK_CFG_REG_CLOCK_CRC_ENABLE_MASK                                                       (0x00008000UL) /**< rw - enable clock source for CRC. */
#define PCR_CLK_CFG_REG_CLOCK_CRC_ENABLE_POS                                                        (15UL)
#define PCR_CLK_CFG_REG_CLOCK_CLKGEN_ENABLE_MASK                                                    (0x00004000UL) /**< rw - enable clock source for CLKGEN. */
#define PCR_CLK_CFG_REG_CLOCK_CLKGEN_ENABLE_POS                                                     (14UL)
#define PCR_CLK_CFG_REG_CLOCK_SWP_ENABLE_MASK                                                       (0x00002000UL) /**< rw - enable clock source for SWP. */
#define PCR_CLK_CFG_REG_CLOCK_SWP_ENABLE_POS                                                        (13UL)
#define PCR_CLK_CFG_REG_CLOCK_RNG_ENABLE_MASK                                                       (0x00001000UL) /**< rw - enable clock source for RNG. */
#define PCR_CLK_CFG_REG_CLOCK_RNG_ENABLE_POS                                                        (12UL)
#define PCR_CLK_CFG_REG_CLOCK_CLIF_ENABLE_MASK                                                      (0x00000800UL) /**< rw - enable clock source for CLIF. */
#define PCR_CLK_CFG_REG_CLOCK_CLIF_ENABLE_POS                                                       (11UL)
#define PCR_CLK_CFG_REG_LFO_EN_MASK                                                                 (0x00000400UL) /**< rw - Enable LFO. */
#define PCR_CLK_CFG_REG_LFO_EN_POS                                                                  (10UL)
#define PCR_CLK_CFG_REG_LFO_TRIMM_MASK                                                              (0x000003F0UL) /**< rw - Trimm Value for LFO. */
#define PCR_CLK_CFG_REG_LFO_TRIMM_POS                                                               (4UL)
#define PCR_CLK_CFG_REG_EN_SWIO_CLK_MASK                                                            (0x00000008UL) /**< rw - Enables the SWIO clock. */
#define PCR_CLK_CFG_REG_EN_SWIO_CLK_POS                                                             (3UL)
#define PCR_CLK_CFG_REG_SELECT_SCR_CTSEQ_MASK                                                       (0x00000004UL) /**< rw - Selects the clock source for the system clock generation 0 ... clkXTAL (27.12 MHz) 1 ... clkPLL/2 (24MHz) . */
#define PCR_CLK_CFG_REG_SELECT_SCR_CTSEQ_POS                                                        (2UL)
#define PCR_CLK_CFG_REG__RESET_VALUE                                                                (0x008FFE08UL) /**< value after reset */
/**
* @}
*/


/**
* \name Select lines for Clock muxes , between different frequencies or between different division values
* <b>Reset value:</b> 0x00080B00\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_CLK_CFG2_REG                                                                            (0x40024034UL) /**< rw-*/
#define PCR_CLK_CFG2_REG_CTRL_TXLDO_CLK_MASK                                                        (0x00180000UL) /**< rw - TXLDO clock division select. 00->LFO/1 , 01-> LFO/2 , 10->LFO/4 , 11->LFO/8. */
#define PCR_CLK_CFG2_REG_CTRL_TXLDO_CLK_POS                                                         (19UL)
#define PCR_CLK_CFG2_REG_EXT_CLK_SEL_MASK                                                           (0x00040000UL) /**< rw - Selects between XTAL clock and External clock. 0...XTAL , 1...external clock. */
#define PCR_CLK_CFG2_REG_EXT_CLK_SEL_POS                                                            (18UL)
#define PCR_CLK_CFG2_REG_HSUART_IP_CLKSEL_MASK                                                      (0x00030000UL) /**< rw - Selects ip clock divider value for hsuart.  00...xtal/1  01...xtal/2         10...xtal/4        11...RFU. */
#define PCR_CLK_CFG2_REG_HSUART_IP_CLKSEL_POS                                                       (16UL)
#define PCR_CLK_CFG2_REG_SPIM_IP_CLKSEL_MASK                                                        (0x0000C000UL) /**< rw - Selects ip clock divider value for spim.  00...xtal/1  01...xtal/2         10...xtal/4        11...RFU. */
#define PCR_CLK_CFG2_REG_SPIM_IP_CLKSEL_POS                                                         (14UL)
#define PCR_CLK_CFG2_REG_I2CM_IP_CLKSEL_MASK                                                        (0x00003000UL) /**< rw - Selects ip clock divider value for spim.  00...xtal/1  01...xtal/2         10...xtal/4        11...RFU. */
#define PCR_CLK_CFG2_REG_I2CM_IP_CLKSEL_POS                                                         (12UL)
#define PCR_CLK_CFG2_REG_CTSEQ_CLKSEL_MASK                                                          (0x00000800UL) /**< rw - 0...+10     1...+82. */
#define PCR_CLK_CFG2_REG_CTSEQ_CLKSEL_POS                                                           (11UL)
#define PCR_CLK_CFG2_REG_EE_EEPROM_CLKSEL_MASK                                                      (0x00000400UL) /**< rw - Selects the divider value for the lfo clock for the EEPROM module           0 ... LFO/1      1...lfo/4. */
#define PCR_CLK_CFG2_REG_EE_EEPROM_CLKSEL_POS                                                       (10UL)
#define PCR_CLK_CFG2_REG_EE_PF_FIX_CLKSEL_MASK                                                      (0x00000200UL) /**< rw - Selects between divided xtal value and gpio1 test clock       0 ... gpio1clk/1 (only for test purpose)     1...xtal-clk/32. */
#define PCR_CLK_CFG2_REG_EE_PF_FIX_CLKSEL_POS                                                       (9UL)
#define PCR_CLK_CFG2_REG_EE_PF_VAR_CLKSEL_MASK                                                      (0x000001C0UL) /**< rw - Selects between gpio1 clock and xtal divided clocks for PageFlash module.          0 ... gpio1clk/1 (only for test purpose)       1 ... Xtal-clk/4      2 ... Xtal-clk/8       3 ... Xtal-clk/16  4...Xtal-clk/32           5--7 RFU. */
#define PCR_CLK_CFG2_REG_EE_PF_VAR_CLKSEL_POS                                                       (6UL)
#define PCR_CLK_CFG2_REG_SPARE_CELL_CLK_CFGL_MASK                                                   (0x00000030UL) /**< rw - Spare cells to be used for clock config. */
#define PCR_CLK_CFG2_REG_SPARE_CELL_CLK_CFGL_POS                                                    (4UL)
#define PCR_CLK_CFG2_REG_SYSTEM_CLOCK_SEL_MASK                                                      (0x00000003UL) /**< rw - Selects the divider for the system clock . 00...Divby1    01...DIVby2   10...DIVby4     11...RESERVED. */
#define PCR_CLK_CFG2_REG_SYSTEM_CLOCK_SEL_POS                                                       (0UL)
#define PCR_CLK_CFG2_REG__RESET_VALUE                                                               (0x00080B00UL) /**< value after reset */
/**
* @}
*/


/**
* \name Store value on input pasd into this register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define PCR_PADIN_REG                                                                               (0x40024038UL) /**< r--*/
#define PCR_PADIN_REG_PADIN_SWPM_DWP_RX_MASK                                                        (0x08000000UL) /**< r- - Input value to be overridden as input for SWPM DWP RX pad. */
#define PCR_PADIN_REG_PADIN_SWPM_DWP_RX_POS                                                         (27UL)
#define PCR_PADIN_REG_PADIN_CLK_AUX_MASK                                                            (0x04000000UL) /**< r- - Input value for CLK_AUX. */
#define PCR_PADIN_REG_PADIN_CLK_AUX_POS                                                             (26UL)
#define PCR_PADIN_REG_PADIN_IO_AUX_MASK                                                             (0x02000000UL) /**< r- - Input value for IO_AUX. */
#define PCR_PADIN_REG_PADIN_IO_AUX_POS                                                              (25UL)
#define PCR_PADIN_REG_PADIN_INT_AUX_MASK                                                            (0x01000000UL) /**< r- - Input value for INT_AUX. */
#define PCR_PADIN_REG_PADIN_INT_AUX_POS                                                             (24UL)
#define PCR_PADIN_REG_PADIN_GPIO12_MASK                                                             (0x00800000UL) /**< r- - Input value for GPIO12. */
#define PCR_PADIN_REG_PADIN_GPIO12_POS                                                              (23UL)
#define PCR_PADIN_REG_PADIN_GPIO11_MASK                                                             (0x00400000UL) /**< r- - Input value for GPIO11. */
#define PCR_PADIN_REG_PADIN_GPIO11_POS                                                              (22UL)
#define PCR_PADIN_REG_PADIN_GPIO10_MASK                                                             (0x00200000UL) /**< r- - Input value for GPIO10. */
#define PCR_PADIN_REG_PADIN_GPIO10_POS                                                              (21UL)
#define PCR_PADIN_REG_PADIN_GPIO9_MASK                                                              (0x00100000UL) /**< r- - Input value for GPIO9. */
#define PCR_PADIN_REG_PADIN_GPIO9_POS                                                               (20UL)
#define PCR_PADIN_REG_PADIN_GPIO8_MASK                                                              (0x00080000UL) /**< r- - Input value for GPIO8. */
#define PCR_PADIN_REG_PADIN_GPIO8_POS                                                               (19UL)
#define PCR_PADIN_REG_PADIN_GPIO7_MASK                                                              (0x00040000UL) /**< r- - Input value for GPIO7. */
#define PCR_PADIN_REG_PADIN_GPIO7_POS                                                               (18UL)
#define PCR_PADIN_REG_PADIN_GPIO6_MASK                                                              (0x00020000UL) /**< r- - Input value for GPIO6. */
#define PCR_PADIN_REG_PADIN_GPIO6_POS                                                               (17UL)
#define PCR_PADIN_REG_PADIN_GPIO5_MASK                                                              (0x00010000UL) /**< r- - Input value for GPIO5. */
#define PCR_PADIN_REG_PADIN_GPIO5_POS                                                               (16UL)
#define PCR_PADIN_REG_PADIN_GPIO4_MASK                                                              (0x00008000UL) /**< r- - Input value for GPIO4. */
#define PCR_PADIN_REG_PADIN_GPIO4_POS                                                               (15UL)
#define PCR_PADIN_REG_PADIN_GPIO3_MASK                                                              (0x00004000UL) /**< r- - Input value for GPIO3. */
#define PCR_PADIN_REG_PADIN_GPIO3_POS                                                               (14UL)
#define PCR_PADIN_REG_PADIN_GPIO2_MASK                                                              (0x00002000UL) /**< r- - Input value for GPIO2. */
#define PCR_PADIN_REG_PADIN_GPIO2_POS                                                               (13UL)
#define PCR_PADIN_REG_PADIN_GPIO1_MASK                                                              (0x00001000UL) /**< r- - Input value for GPIO1. */
#define PCR_PADIN_REG_PADIN_GPIO1_POS                                                               (12UL)
#define PCR_PADIN_REG_PADIN_DWL_REQ_MASK                                                            (0x00000800UL) /**< r- - Input value for  DWL_REQ. */
#define PCR_PADIN_REG_PADIN_DWL_REQ_POS                                                             (11UL)
#define PCR_PADIN_REG_PADIN_MISO_M_MASK                                                             (0x00000400UL) /**< r- - Input value for MISO_M. */
#define PCR_PADIN_REG_PADIN_MISO_M_POS                                                              (10UL)
#define PCR_PADIN_REG_PADIN_MOSI_M_MASK                                                             (0x00000200UL) /**< r- - Input value for MOSI_M. */
#define PCR_PADIN_REG_PADIN_MOSI_M_POS                                                              (9UL)
#define PCR_PADIN_REG_PADIN_SCLK_M_MASK                                                             (0x00000100UL) /**< r- - Input value for SCLK_M. */
#define PCR_PADIN_REG_PADIN_SCLK_M_POS                                                              (8UL)
#define PCR_PADIN_REG_PADIN_NSS_M_MASK                                                              (0x00000080UL) /**< r- - Input value for NSS_M. */
#define PCR_PADIN_REG_PADIN_NSS_M_POS                                                               (7UL)
#define PCR_PADIN_REG_PADIN_SDA_M_MASK                                                              (0x00000040UL) /**< r- - Input Value SDA_M. */
#define PCR_PADIN_REG_PADIN_SDA_M_POS                                                               (6UL)
#define PCR_PADIN_REG_PADIN_SCL_M_MASK                                                              (0x00000020UL) /**< r- - Input Value SCL_M. */
#define PCR_PADIN_REG_PADIN_SCL_M_POS                                                               (5UL)
#define PCR_PADIN_REG_PADIN_IRQ_MASK                                                                (0x00000010UL) /**< r- - input value for IRQ. */
#define PCR_PADIN_REG_PADIN_IRQ_POS                                                                 (4UL)
#define PCR_PADIN_REG_PADIN_ATX_D_MASK                                                              (0x00000008UL) /**< r- - Input Value ATX_D. */
#define PCR_PADIN_REG_PADIN_ATX_D_POS                                                               (3UL)
#define PCR_PADIN_REG_PADIN_ATX_C_MASK                                                              (0x00000004UL) /**< r- - Input Value ATX_C. */
#define PCR_PADIN_REG_PADIN_ATX_C_POS                                                               (2UL)
#define PCR_PADIN_REG_PADIN_ATX_B_MASK                                                              (0x00000002UL) /**< r- - Input Value ATX_B. */
#define PCR_PADIN_REG_PADIN_ATX_B_POS                                                               (1UL)
#define PCR_PADIN_REG_PADIN_ATX_A_MASK                                                              (0x00000001UL) /**< r- - Input Value ATX_A. */
#define PCR_PADIN_REG_PADIN_ATX_A_POS                                                               (0UL)
#define PCR_PADIN_REG__RESET_VALUE                                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Output value to be driven onto Output pads stored here
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADOUT_REG                                                                              (0x4002403CUL) /**< rw-*/
#define PCR_PADOUT_REG_PADOUT_SWPM_DWP_TX_MASK                                                      (0x10000000UL) /**< rw - Output value of SWPM DWP TX pad. */
#define PCR_PADOUT_REG_PADOUT_SWPM_DWP_TX_POS                                                       (28UL)
#define PCR_PADOUT_REG_PADOUT_SWPM_DWP_RX_MASK                                                      (0x08000000UL) /**< rw - Value to be overridden as input for SWPM DWP RX pad. */
#define PCR_PADOUT_REG_PADOUT_SWPM_DWP_RX_POS                                                       (27UL)
#define PCR_PADOUT_REG_PADOUT_CLK_AUX_MASK                                                          (0x04000000UL) /**< rw - Output value for CLK_AUX. */
#define PCR_PADOUT_REG_PADOUT_CLK_AUX_POS                                                           (26UL)
#define PCR_PADOUT_REG_PADOUT_IO_AUX_MASK                                                           (0x02000000UL) /**< rw - Output value for IO_AUX. */
#define PCR_PADOUT_REG_PADOUT_IO_AUX_POS                                                            (25UL)
#define PCR_PADOUT_REG_PADOUT_INT_AUX_MASK                                                          (0x01000000UL) /**< rw - Output value for INT_AUX. */
#define PCR_PADOUT_REG_PADOUT_INT_AUX_POS                                                           (24UL)
#define PCR_PADOUT_REG_PADOUT_GPIO12_MASK                                                           (0x00800000UL) /**< rw - Output value for GPIO12. */
#define PCR_PADOUT_REG_PADOUT_GPIO12_POS                                                            (23UL)
#define PCR_PADOUT_REG_PADOUT_GPIO11_MASK                                                           (0x00400000UL) /**< rw - Output value for GPIO11. */
#define PCR_PADOUT_REG_PADOUT_GPIO11_POS                                                            (22UL)
#define PCR_PADOUT_REG_PADOUT_GPIO10_MASK                                                           (0x00200000UL) /**< rw - Output value for GPIO10. */
#define PCR_PADOUT_REG_PADOUT_GPIO10_POS                                                            (21UL)
#define PCR_PADOUT_REG_PADOUT_GPIO9_MASK                                                            (0x00100000UL) /**< rw - Output value for GPIO9. */
#define PCR_PADOUT_REG_PADOUT_GPIO9_POS                                                             (20UL)
#define PCR_PADOUT_REG_PADOUT_GPIO8_MASK                                                            (0x00080000UL) /**< rw - Output value for GPIO8. */
#define PCR_PADOUT_REG_PADOUT_GPIO8_POS                                                             (19UL)
#define PCR_PADOUT_REG_PADOUT_GPIO7_MASK                                                            (0x00040000UL) /**< rw - Output value for GPIO7. */
#define PCR_PADOUT_REG_PADOUT_GPIO7_POS                                                             (18UL)
#define PCR_PADOUT_REG_PADOUT_GPIO6_MASK                                                            (0x00020000UL) /**< rw - Output value for GPIO6. */
#define PCR_PADOUT_REG_PADOUT_GPIO6_POS                                                             (17UL)
#define PCR_PADOUT_REG_PADOUT_GPIO5_MASK                                                            (0x00010000UL) /**< rw - Output value for GPIO5. */
#define PCR_PADOUT_REG_PADOUT_GPIO5_POS                                                             (16UL)
#define PCR_PADOUT_REG_PADOUT_GPIO4_MASK                                                            (0x00008000UL) /**< rw - Output value for GPIO4. */
#define PCR_PADOUT_REG_PADOUT_GPIO4_POS                                                             (15UL)
#define PCR_PADOUT_REG_PADOUT_GPIO3_MASK                                                            (0x00004000UL) /**< rw - Output value for GPIO3. */
#define PCR_PADOUT_REG_PADOUT_GPIO3_POS                                                             (14UL)
#define PCR_PADOUT_REG_PADOUT_GPIO2_MASK                                                            (0x00002000UL) /**< rw - Output value for GPIO2. */
#define PCR_PADOUT_REG_PADOUT_GPIO2_POS                                                             (13UL)
#define PCR_PADOUT_REG_PADOUT_GPIO1_MASK                                                            (0x00001000UL) /**< rw - Output value for GPIO1. */
#define PCR_PADOUT_REG_PADOUT_GPIO1_POS                                                             (12UL)
#define PCR_PADOUT_REG_PADOUT_DWL_REQ_MASK                                                          (0x00000800UL) /**< rw - Output value for  DWL_REQ. */
#define PCR_PADOUT_REG_PADOUT_DWL_REQ_POS                                                           (11UL)
#define PCR_PADOUT_REG_PADOUT_MISO_M_MASK                                                           (0x00000400UL) /**< rw - Output value for MISO_M. */
#define PCR_PADOUT_REG_PADOUT_MISO_M_POS                                                            (10UL)
#define PCR_PADOUT_REG_PADOUT_MOSI_M_MASK                                                           (0x00000200UL) /**< rw - Output value for MOSI_M. */
#define PCR_PADOUT_REG_PADOUT_MOSI_M_POS                                                            (9UL)
#define PCR_PADOUT_REG_PADOUT_SCLK_M_MASK                                                           (0x00000100UL) /**< rw - Output value for SCLK_M. */
#define PCR_PADOUT_REG_PADOUT_SCLK_M_POS                                                            (8UL)
#define PCR_PADOUT_REG_PADOUT_NSS_M_MASK                                                            (0x00000080UL) /**< rw - Output value for NSS_M. */
#define PCR_PADOUT_REG_PADOUT_NSS_M_POS                                                             (7UL)
#define PCR_PADOUT_REG_PADOUT_SDA_M_MASK                                                            (0x00000040UL) /**< rw - Output Value SDA_M. */
#define PCR_PADOUT_REG_PADOUT_SDA_M_POS                                                             (6UL)
#define PCR_PADOUT_REG_PADOUT_SCL_M_MASK                                                            (0x00000020UL) /**< rw - Output Value SCL_M. */
#define PCR_PADOUT_REG_PADOUT_SCL_M_POS                                                             (5UL)
#define PCR_PADOUT_REG_PADOUT_IRQ_MASK                                                              (0x00000010UL) /**< rw - Output value for IRQ. */
#define PCR_PADOUT_REG_PADOUT_IRQ_POS                                                               (4UL)
#define PCR_PADOUT_REG_PADOUT_ATX_D_MASK                                                            (0x00000008UL) /**< rw - Output Value ATX_D. */
#define PCR_PADOUT_REG_PADOUT_ATX_D_POS                                                             (3UL)
#define PCR_PADOUT_REG_PADOUT_ATX_C_MASK                                                            (0x00000004UL) /**< rw - Output Value ATX_C. */
#define PCR_PADOUT_REG_PADOUT_ATX_C_POS                                                             (2UL)
#define PCR_PADOUT_REG_PADOUT_ATX_B_MASK                                                            (0x00000002UL) /**< rw - Output Value ATX_B. */
#define PCR_PADOUT_REG_PADOUT_ATX_B_POS                                                             (1UL)
#define PCR_PADOUT_REG_PADOUT_ATX_A_MASK                                                            (0x00000001UL) /**< rw - Output Value ATX_A. */
#define PCR_PADOUT_REG_PADOUT_ATX_A_POS                                                             (0UL)
#define PCR_PADOUT_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Host if pad: i2c_scl/spi_nss/hsu_rx/usb_dp/smb_cl
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_ATX_A_REG                                                                           (0x40024040UL) /**< rw-*/
#define PCR_PAD_ATX_A_REG_ATX_A_SLEW_RATE_MASK                                                      (0x00000010UL) /**< rw - Select Driver Strength for ATX_A. */
#define PCR_PAD_ATX_A_REG_ATX_A_SLEW_RATE_POS                                                       (4UL)
#define PCR_PAD_ATX_A_REG_ATX_A_PUPD_MASK                                                           (0x0000000CUL) /**< rw - Enable PullUp/Down on ATX_A. */
#define PCR_PAD_ATX_A_REG_ATX_A_PUPD_POS                                                            (2UL)
#define PCR_PAD_ATX_A_REG_ATX_A_EN_OUT_MASK                                                         (0x00000002UL) /**< rw - Enables Output driver for ATX_A. */
#define PCR_PAD_ATX_A_REG_ATX_A_EN_OUT_POS                                                          (1UL)
#define PCR_PAD_ATX_A_REG_ATX_A_EN_IN_MASK                                                          (0x00000001UL) /**< rw - Enables Input Driver for ATX_A. */
#define PCR_PAD_ATX_A_REG_ATX_A_EN_IN_POS                                                           (0UL)
#define PCR_PAD_ATX_A_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Host if pad: i2c_sda/spi_mosi/hsu_tx/usb_dm/smb_da
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_ATX_B_REG                                                                           (0x40024044UL) /**< rw-*/
#define PCR_PAD_ATX_B_REG_ATX_B_SLEW_RATE_MASK                                                      (0x00000010UL) /**< rw - Select Driver Strength for ATX_B. */
#define PCR_PAD_ATX_B_REG_ATX_B_SLEW_RATE_POS                                                       (4UL)
#define PCR_PAD_ATX_B_REG_ATX_B_PUPD_MASK                                                           (0x0000000CUL) /**< rw - Enable PullUp/Down on ATX_B. */
#define PCR_PAD_ATX_B_REG_ATX_B_PUPD_POS                                                            (2UL)
#define PCR_PAD_ATX_B_REG_ATX_B_EN_OUT_MASK                                                         (0x00000002UL) /**< rw - Enables Output driver for ATX_B. */
#define PCR_PAD_ATX_B_REG_ATX_B_EN_OUT_POS                                                          (1UL)
#define PCR_PAD_ATX_B_REG_ATX_B_EN_IN_MASK                                                          (0x00000001UL) /**< rw - Enables Input Driver for ATX_B. */
#define PCR_PAD_ATX_B_REG_ATX_B_EN_IN_POS                                                           (0UL)
#define PCR_PAD_ATX_B_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Host if pad: i2c_adr0/spi_miso/hsu_rts_n/usb-N.A/smbalert
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_ATX_C_REG                                                                           (0x40024048UL) /**< rw-*/
#define PCR_PAD_ATX_C_REG_ATX_C_SLEW_RATE_MASK                                                      (0x00000010UL) /**< rw - Select Driver Strength for ATX_C. */
#define PCR_PAD_ATX_C_REG_ATX_C_SLEW_RATE_POS                                                       (4UL)
#define PCR_PAD_ATX_C_REG_ATX_C_PUPD_MASK                                                           (0x0000000CUL) /**< rw - Enable PullUp/Down on ATX_C. */
#define PCR_PAD_ATX_C_REG_ATX_C_PUPD_POS                                                            (2UL)
#define PCR_PAD_ATX_C_REG_ATX_C_EN_OUT_MASK                                                         (0x00000002UL) /**< rw - Enables Output driver for ATX_C. */
#define PCR_PAD_ATX_C_REG_ATX_C_EN_OUT_POS                                                          (1UL)
#define PCR_PAD_ATX_C_REG_ATX_C_EN_IN_MASK                                                          (0x00000001UL) /**< rw - Enables Input Driver for ATX_C. */
#define PCR_PAD_ATX_C_REG_ATX_C_EN_IN_POS                                                           (0UL)
#define PCR_PAD_ATX_C_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Host if pad: i2cadr1/spi_sck/hsu_cts_n/usb-N.A/smb-N.A
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_ATX_D_REG                                                                           (0x4002404CUL) /**< rw-*/
#define PCR_PAD_ATX_D_REG_ATX_D_SLEW_RATE_MASK                                                      (0x00000010UL) /**< rw - Select Driver Strength for ATX_D. */
#define PCR_PAD_ATX_D_REG_ATX_D_SLEW_RATE_POS                                                       (4UL)
#define PCR_PAD_ATX_D_REG_ATX_D_PUPD_MASK                                                           (0x0000000CUL) /**< rw - Enable PullUp/Down on ATX_D. */
#define PCR_PAD_ATX_D_REG_ATX_D_PUPD_POS                                                            (2UL)
#define PCR_PAD_ATX_D_REG_ATX_D_EN_OUT_MASK                                                         (0x00000002UL) /**< rw - Enables Output driver for ATX_D. */
#define PCR_PAD_ATX_D_REG_ATX_D_EN_OUT_POS                                                          (1UL)
#define PCR_PAD_ATX_D_REG_ATX_D_EN_IN_MASK                                                          (0x00000001UL) /**< rw - Enables Input Driver for ATX_D. */
#define PCR_PAD_ATX_D_REG_ATX_D_EN_IN_POS                                                           (0UL)
#define PCR_PAD_ATX_D_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name DWL_REQ pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADDWL_REQ_REG                                                                          (0x40024050UL) /**< rw-*/
#define PCR_PADDWL_REQ_REG_DWLREQ_SLEW_RATE_MASK                                                    (0x00000010UL) /**< rw - Select Driver Strength for DWLREQ. */
#define PCR_PADDWL_REQ_REG_DWLREQ_SLEW_RATE_POS                                                     (4UL)
#define PCR_PADDWL_REQ_REG_DWLREQ_PUPD_MASK                                                         (0x0000000CUL) /**< rw - Enable PullUp/Down on DWLREQ. */
#define PCR_PADDWL_REQ_REG_DWLREQ_PUPD_POS                                                          (2UL)
#define PCR_PADDWL_REQ_REG_DWLREQ_EN_OUT_MASK                                                       (0x00000002UL) /**< rw - Enables Output driver for DWLREQ. */
#define PCR_PADDWL_REQ_REG_DWLREQ_EN_OUT_POS                                                        (1UL)
#define PCR_PADDWL_REQ_REG_DWLREQ_EN_IN_MASK                                                        (0x00000001UL) /**< rw - Enables Input Driver for DWLREQ. */
#define PCR_PADDWL_REQ_REG_DWLREQ_EN_IN_POS                                                         (0UL)
#define PCR_PADDWL_REQ_REG__RESET_VALUE                                                             (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name INT_AUX pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_INT_AUX_REG                                                                         (0x40024054UL) /**< rw-*/
#define PCR_PAD_INT_AUX_REG_INT_AUX_ACTIVE_LOW_EN_MASK                                              (0x00000080UL) /**< rw - Configures INT_AUX to be interpretted as active low signal. */
#define PCR_PAD_INT_AUX_REG_INT_AUX_ACTIVE_LOW_EN_POS                                               (7UL)
#define PCR_PAD_INT_AUX_REG_INT_AUX_GPIOMODE_EN_MASK                                                (0x00000040UL) /**< rw - Puts the INT_AUX PAD in GPIO mode ( By default in I2C mode). */
#define PCR_PAD_INT_AUX_REG_INT_AUX_GPIOMODE_EN_POS                                                 (6UL)
#define PCR_PAD_INT_AUX_REG_INT_AUX_SW_ENABLE_MASK                                                  (0x00000020UL) /**< rw - Enabling software register control for INT_AUX. */
#define PCR_PAD_INT_AUX_REG_INT_AUX_SW_ENABLE_POS                                                   (5UL)
#define PCR_PAD_INT_AUX_REG_INT_AUX_SLEW_RATE_MASK                                                  (0x00000010UL) /**< rw - Select Driver Strength for INT_AUX. */
#define PCR_PAD_INT_AUX_REG_INT_AUX_SLEW_RATE_POS                                                   (4UL)
#define PCR_PAD_INT_AUX_REG_INT_AUX_PUPD_MASK                                                       (0x0000000CUL) /**< rw - Enable PullUp/Down on INT_AUX. */
#define PCR_PAD_INT_AUX_REG_INT_AUX_PUPD_POS                                                        (2UL)
#define PCR_PAD_INT_AUX_REG_INT_AUX_EN_OUT_MASK                                                     (0x00000002UL) /**< rw - Enables Output driver for INT_AUX. */
#define PCR_PAD_INT_AUX_REG_INT_AUX_EN_OUT_POS                                                      (1UL)
#define PCR_PAD_INT_AUX_REG_INT_AUX_EN_IN_MASK                                                      (0x00000001UL) /**< rw - Enables Input Driver for INT_AUX. */
#define PCR_PAD_INT_AUX_REG_INT_AUX_EN_IN_POS                                                       (0UL)
#define PCR_PAD_INT_AUX_REG__RESET_VALUE                                                            (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name IO_AUX pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_IO_AUX_REG                                                                          (0x40024058UL) /**< rw-*/
#define PCR_PAD_IO_AUX_REG_IO_AUX_GPIOMODE_EN_MASK                                                  (0x00000040UL) /**< rw - Puts the IO_AUX PAD in GPIO mode ( By default in I2C mode). */
#define PCR_PAD_IO_AUX_REG_IO_AUX_GPIOMODE_EN_POS                                                   (6UL)
#define PCR_PAD_IO_AUX_REG_IO_AUX_SW_ENABLE_MASK                                                    (0x00000020UL) /**< rw - Enabling software register control for IO_AUX. */
#define PCR_PAD_IO_AUX_REG_IO_AUX_SW_ENABLE_POS                                                     (5UL)
#define PCR_PAD_IO_AUX_REG_IO_AUX_SLEW_RATE_MASK                                                    (0x00000010UL) /**< rw - Select Driver Strength for IO_AUX. */
#define PCR_PAD_IO_AUX_REG_IO_AUX_SLEW_RATE_POS                                                     (4UL)
#define PCR_PAD_IO_AUX_REG_IO_AUX_PUPD_MASK                                                         (0x0000000CUL) /**< rw - Enable PullUp/Down on IO_AUX. */
#define PCR_PAD_IO_AUX_REG_IO_AUX_PUPD_POS                                                          (2UL)
#define PCR_PAD_IO_AUX_REG_IO_AUX_EN_OUT_MASK                                                       (0x00000002UL) /**< rw - Enables Output driver for IO_AUX. */
#define PCR_PAD_IO_AUX_REG_IO_AUX_EN_OUT_POS                                                        (1UL)
#define PCR_PAD_IO_AUX_REG_IO_AUX_EN_IN_MASK                                                        (0x00000001UL) /**< rw - Enables Input Driver for IO_AUX. */
#define PCR_PAD_IO_AUX_REG_IO_AUX_EN_IN_POS                                                         (0UL)
#define PCR_PAD_IO_AUX_REG__RESET_VALUE                                                             (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLK_AUX pad configuration
* <b>Reset value:</b> 0x00000002\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_CLK_AUX_REG                                                                         (0x4002405CUL) /**< rw-*/
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_GPIOMODE_EN_MASK                                                (0x00000040UL) /**< rw - Enabling CLK_AUX pad in GPIO mode ( By default in I2C mode). */
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_GPIOMODE_EN_POS                                                 (6UL)
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_SW_ENABLE_MASK                                                  (0x00000020UL) /**< rw - Enabling software register control for CLK_AUX. */
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_SW_ENABLE_POS                                                   (5UL)
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_SLEW_RATE_MASK                                                  (0x00000010UL) /**< rw - Select Driver Strength for CLK_AUX. */
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_SLEW_RATE_POS                                                   (4UL)
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_PUPD_MASK                                                       (0x0000000CUL) /**< rw - Enable PullUp/Down on CLK_AUX. */
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_PUPD_POS                                                        (2UL)
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_EN_OUT_MASK                                                     (0x00000002UL) /**< rw - Enables Output driver for CLK_AUX. */
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_EN_OUT_POS                                                      (1UL)
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_EN_IN_MASK                                                      (0x00000001UL) /**< rw - Enables Input Driver for CLK_AUX. */
#define PCR_PAD_CLK_AUX_REG_CLK_AUX_EN_IN_POS                                                       (0UL)
#define PCR_PAD_CLK_AUX_REG__RESET_VALUE                                                            (0x00000002UL) /**< value after reset */
/**
* @}
*/


/**
* \name IRQ pad configuration
* <b>Reset value:</b> 0x00000002\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADIRQ_REG                                                                              (0x40024060UL) /**< rw-*/
#define PCR_PADIRQ_REG_IRQ_SLEW_RATE_MASK                                                           (0x00000010UL) /**< rw - Select Driver Strength for IRQ. */
#define PCR_PADIRQ_REG_IRQ_SLEW_RATE_POS                                                            (4UL)
#define PCR_PADIRQ_REG_IRQ_PUPD_MASK                                                                (0x0000000CUL) /**< rw - Enable PullUp/Down on IRQ. */
#define PCR_PADIRQ_REG_IRQ_PUPD_POS                                                                 (2UL)
#define PCR_PADIRQ_REG_IRQ_EN_OUT_MASK                                                              (0x00000002UL) /**< rw - Enables Output driver for IRQ. */
#define PCR_PADIRQ_REG_IRQ_EN_OUT_POS                                                               (1UL)
#define PCR_PADIRQ_REG_IRQ_EN_IN_MASK                                                               (0x00000001UL) /**< rw - Enables Input Driver for IRQ. */
#define PCR_PADIRQ_REG_IRQ_EN_IN_POS                                                                (0UL)
#define PCR_PADIRQ_REG__RESET_VALUE                                                                 (0x00000002UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO1 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO1_REG                                                                            (0x40024064UL) /**< rw-*/
#define PCR_PADGPIO1_REG_GPIO1_TEST_EN_MASK                                                         (0x00000020UL) /**< rw - Indicating that the GPIO 1 pad is giving clock. */
#define PCR_PADGPIO1_REG_GPIO1_TEST_EN_POS                                                          (5UL)
#define PCR_PADGPIO1_REG_GPIO1_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO1. */
#define PCR_PADGPIO1_REG_GPIO1_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO1_REG_GPIO1_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO1. */
#define PCR_PADGPIO1_REG_GPIO1_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO1_REG_GPIO1_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO1. */
#define PCR_PADGPIO1_REG_GPIO1_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO1_REG_GPIO1_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO1. */
#define PCR_PADGPIO1_REG_GPIO1_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO1_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO2 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO2_REG                                                                            (0x40024068UL) /**< rw-*/
#define PCR_PADGPIO2_REG_GPIO2_TEST_EN_MASK                                                         (0x00000020UL) /**< rw - Indicating the GPIO2 pad is giving clock. */
#define PCR_PADGPIO2_REG_GPIO2_TEST_EN_POS                                                          (5UL)
#define PCR_PADGPIO2_REG_GPIO2_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO2. */
#define PCR_PADGPIO2_REG_GPIO2_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO2_REG_GPIO2_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO2. */
#define PCR_PADGPIO2_REG_GPIO2_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO2_REG_GPIO2_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO2. */
#define PCR_PADGPIO2_REG_GPIO2_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO2_REG_GPIO2_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO2. */
#define PCR_PADGPIO2_REG_GPIO2_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO2_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO3 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO3_REG                                                                            (0x4002406CUL) /**< rw-*/
#define PCR_PADGPIO3_REG_GPIO3_TEST_EN_MASK                                                         (0x00000020UL) /**< rw - Indicating the GPIO3 pad is used for testing of clock. */
#define PCR_PADGPIO3_REG_GPIO3_TEST_EN_POS                                                          (5UL)
#define PCR_PADGPIO3_REG_GPIO3_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO3. */
#define PCR_PADGPIO3_REG_GPIO3_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO3_REG_GPIO3_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO3. */
#define PCR_PADGPIO3_REG_GPIO3_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO3_REG_GPIO3_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO3. */
#define PCR_PADGPIO3_REG_GPIO3_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO3_REG_GPIO3_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO3. */
#define PCR_PADGPIO3_REG_GPIO3_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO3_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO4 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO4_REG                                                                            (0x40024070UL) /**< rw-*/
#define PCR_PADGPIO4_REG_GPIO4_TEST_EN_MASK                                                         (0x00000020UL) /**< rw - Indicating the GPIO4 pad is used for testing of clock. */
#define PCR_PADGPIO4_REG_GPIO4_TEST_EN_POS                                                          (5UL)
#define PCR_PADGPIO4_REG_GPIO4_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO4. */
#define PCR_PADGPIO4_REG_GPIO4_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO4_REG_GPIO4_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO4. */
#define PCR_PADGPIO4_REG_GPIO4_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO4_REG_GPIO4_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO4. */
#define PCR_PADGPIO4_REG_GPIO4_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO4_REG_GPIO4_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO4. */
#define PCR_PADGPIO4_REG_GPIO4_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO4_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO5 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO5_REG                                                                            (0x40024074UL) /**< rw-*/
#define PCR_PADGPIO5_REG_GPIO5_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO5. */
#define PCR_PADGPIO5_REG_GPIO5_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO5_REG_GPIO5_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO5. */
#define PCR_PADGPIO5_REG_GPIO5_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO5_REG_GPIO5_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO5. */
#define PCR_PADGPIO5_REG_GPIO5_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO5_REG_GPIO5_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO5. */
#define PCR_PADGPIO5_REG_GPIO5_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO5_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO6 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO6_REG                                                                            (0x40024078UL) /**< rw-*/
#define PCR_PADGPIO6_REG_GPIO6_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO6. */
#define PCR_PADGPIO6_REG_GPIO6_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO6_REG_GPIO6_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO6. */
#define PCR_PADGPIO6_REG_GPIO6_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO6_REG_GPIO6_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO6. */
#define PCR_PADGPIO6_REG_GPIO6_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO6_REG_GPIO6_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO6. */
#define PCR_PADGPIO6_REG_GPIO6_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO6_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO7 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO7_REG                                                                            (0x4002407CUL) /**< rw-*/
#define PCR_PADGPIO7_REG_GPIO7_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO7. */
#define PCR_PADGPIO7_REG_GPIO7_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO7_REG_GPIO7_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO7. */
#define PCR_PADGPIO7_REG_GPIO7_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO7_REG_GPIO7_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO7. */
#define PCR_PADGPIO7_REG_GPIO7_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO7_REG_GPIO7_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO7. */
#define PCR_PADGPIO7_REG_GPIO7_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO7_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO8 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO8_REG                                                                            (0x40024080UL) /**< rw-*/
#define PCR_PADGPIO8_REG_GPIO8_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO8. */
#define PCR_PADGPIO8_REG_GPIO8_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO8_REG_GPIO8_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO8. */
#define PCR_PADGPIO8_REG_GPIO8_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO8_REG_GPIO8_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO8. */
#define PCR_PADGPIO8_REG_GPIO8_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO8_REG_GPIO8_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO8. */
#define PCR_PADGPIO8_REG_GPIO8_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO8_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO9 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO9_REG                                                                            (0x40024084UL) /**< rw-*/
#define PCR_PADGPIO9_REG_GPIO9_SLEW_RATE_MASK                                                       (0x00000010UL) /**< rw - Select Driver Strength for GPIO9. */
#define PCR_PADGPIO9_REG_GPIO9_SLEW_RATE_POS                                                        (4UL)
#define PCR_PADGPIO9_REG_GPIO9_PUPD_MASK                                                            (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO9. */
#define PCR_PADGPIO9_REG_GPIO9_PUPD_POS                                                             (2UL)
#define PCR_PADGPIO9_REG_GPIO9_EN_OUT_MASK                                                          (0x00000002UL) /**< rw - Enables Output driver for GPIO9. */
#define PCR_PADGPIO9_REG_GPIO9_EN_OUT_POS                                                           (1UL)
#define PCR_PADGPIO9_REG_GPIO9_EN_IN_MASK                                                           (0x00000001UL) /**< rw - Enables Input Driver for GPIO9. */
#define PCR_PADGPIO9_REG_GPIO9_EN_IN_POS                                                            (0UL)
#define PCR_PADGPIO9_REG__RESET_VALUE                                                               (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO10 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO10_REG                                                                           (0x40024088UL) /**< rw-*/
#define PCR_PADGPIO10_REG_GPIO10_SLEW_RATE_MASK                                                     (0x00000010UL) /**< rw - Select Driver Strength for GPIO10. */
#define PCR_PADGPIO10_REG_GPIO10_SLEW_RATE_POS                                                      (4UL)
#define PCR_PADGPIO10_REG_GPIO10_PUPD_MASK                                                          (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO10. */
#define PCR_PADGPIO10_REG_GPIO10_PUPD_POS                                                           (2UL)
#define PCR_PADGPIO10_REG_GPIO10_EN_OUT_MASK                                                        (0x00000002UL) /**< rw - Enables Output driver for GPIO10. */
#define PCR_PADGPIO10_REG_GPIO10_EN_OUT_POS                                                         (1UL)
#define PCR_PADGPIO10_REG_GPIO10_EN_IN_MASK                                                         (0x00000001UL) /**< rw - Enables Input Driver for GPIO10. */
#define PCR_PADGPIO10_REG_GPIO10_EN_IN_POS                                                          (0UL)
#define PCR_PADGPIO10_REG__RESET_VALUE                                                              (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO11 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO11_REG                                                                           (0x4002408CUL) /**< rw-*/
#define PCR_PADGPIO11_REG_GPIO11_SLEW_RATE_MASK                                                     (0x00000010UL) /**< rw - Select Driver Strength for GPIO11. */
#define PCR_PADGPIO11_REG_GPIO11_SLEW_RATE_POS                                                      (4UL)
#define PCR_PADGPIO11_REG_GPIO11_PUPD_MASK                                                          (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO11. */
#define PCR_PADGPIO11_REG_GPIO11_PUPD_POS                                                           (2UL)
#define PCR_PADGPIO11_REG_GPIO11_EN_OUT_MASK                                                        (0x00000002UL) /**< rw - Enables Output driver for GPIO11. */
#define PCR_PADGPIO11_REG_GPIO11_EN_OUT_POS                                                         (1UL)
#define PCR_PADGPIO11_REG_GPIO11_EN_IN_MASK                                                         (0x00000001UL) /**< rw - Enables Input Driver for GPIO11. */
#define PCR_PADGPIO11_REG_GPIO11_EN_IN_POS                                                          (0UL)
#define PCR_PADGPIO11_REG__RESET_VALUE                                                              (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name GPIO12 pad configuration
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADGPIO12_REG                                                                           (0x40024090UL) /**< rw-*/
#define PCR_PADGPIO12_REG_GPIO12_SLEW_RATE_MASK                                                     (0x00000010UL) /**< rw - Select Driver Strength for GPIO12. */
#define PCR_PADGPIO12_REG_GPIO12_SLEW_RATE_POS                                                      (4UL)
#define PCR_PADGPIO12_REG_GPIO12_PUPD_MASK                                                          (0x0000000CUL) /**< rw - Enable PullUp/Down on GPIO12. */
#define PCR_PADGPIO12_REG_GPIO12_PUPD_POS                                                           (2UL)
#define PCR_PADGPIO12_REG_GPIO12_EN_OUT_MASK                                                        (0x00000002UL) /**< rw - Enables Output driver for GPIO12. */
#define PCR_PADGPIO12_REG_GPIO12_EN_OUT_POS                                                         (1UL)
#define PCR_PADGPIO12_REG_GPIO12_EN_IN_MASK                                                         (0x00000001UL) /**< rw - Enables Input Driver for GPIO12. */
#define PCR_PADGPIO12_REG_GPIO12_EN_IN_POS                                                          (0UL)
#define PCR_PADGPIO12_REG__RESET_VALUE                                                              (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name SWDIO pad slewrate configuration
* <b>Reset value:</b> 0x00000006\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADSWDIO_REG                                                                            (0x40024094UL) /**< rw-*/
#define PCR_PADSWDIO_REG_DFT_SWDIO_ENZI_EN_MASK                                                     (0x00000010UL) /**< rw - Enable DFT control of ENZI on SWDIO PAD. */
#define PCR_PADSWDIO_REG_DFT_SWDIO_ENZI_EN_POS                                                      (4UL)
#define PCR_PADSWDIO_REG_DFT_SWDIO_ENZI_VAL_MASK                                                    (0x00000008UL) /**< rw - DFT control value for ENZI on SWDIO PAD. */
#define PCR_PADSWDIO_REG_DFT_SWDIO_ENZI_VAL_POS                                                     (3UL)
#define PCR_PADSWDIO_REG_DFT_SWDIO_EPUD_MASK                                                        (0x00000006UL) /**< rw - DFT control for EPUD on SWDIO pad. */
#define PCR_PADSWDIO_REG_DFT_SWDIO_EPUD_POS                                                         (1UL)
#define PCR_PADSWDIO_REG_SWDIO_SLEW_RATE_MASK                                                       (0x00000001UL) /**< rw - Select Driver Strength for SWDIO. */
#define PCR_PADSWDIO_REG_SWDIO_SLEW_RATE_POS                                                        (0UL)
#define PCR_PADSWDIO_REG__RESET_VALUE                                                               (0x00000006UL) /**< value after reset */
/**
* @}
*/


/**
* \name SWDCLK pad DFT configuration
* <b>Reset value:</b> 0x00000006\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADSWDCLK_REG                                                                           (0x40024098UL) /**< rw-*/
#define PCR_PADSWDCLK_REG_DFT_SWDCLK_EPUD_MASK                                                      (0x00000006UL) /**< rw - DFT control for EPUD/EPD of SWDCLK PAD. */
#define PCR_PADSWDCLK_REG_DFT_SWDCLK_EPUD_POS                                                       (1UL)
#define PCR_PADSWDCLK_REG_DFT_SWDCLK_SLEW_RATE_MASK                                                 (0x00000001UL) /**< rw - DFT control for slew rate of SWDCLK PAD. */
#define PCR_PADSWDCLK_REG_DFT_SWDCLK_SLEW_RATE_POS                                                  (0UL)
#define PCR_PADSWDCLK_REG__RESET_VALUE                                                              (0x00000006UL) /**< value after reset */
/**
* @}
*/


/**
* \name SWDIO pad configuration
* <b>Reset value:</b> 0x0000020C\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADSWP_REG                                                                              (0x4002409CUL) /**< rw-*/
#define PCR_PADSWP_REG_DFT_SWPM_IO_MASK                                                             (0x00000800UL) /**< rw - DFT Signal for SWP UART. */
#define PCR_PADSWP_REG_DFT_SWPM_IO_POS                                                              (11UL)
#define PCR_PADSWP_REG_SWP_LATCH_MISMATCH_MASK                                                      (0x00000400UL) /**< r- - SWP Latch Mismatch for SWP. */
#define PCR_PADSWP_REG_SWP_LATCH_MISMATCH_POS                                                       (10UL)
#define PCR_PADSWP_REG_SWP_LATCH_CTRL_MASK                                                          (0x00000200UL) /**< rw - SWP Latch Control Signal for SWP. */
#define PCR_PADSWP_REG_SWP_LATCH_CTRL_POS                                                           (9UL)
#define PCR_PADSWP_REG_SWP_EN_PD_MASK                                                               (0x00000100UL) /**< rw - Enable PullDown. */
#define PCR_PADSWP_REG_SWP_EN_PD_POS                                                                (8UL)
#define PCR_PADSWP_REG_SWP_SEL_TESTMODE_MASK                                                        (0x000000C0UL) /**< rw - Test Mode Selection. */
#define PCR_PADSWP_REG_SWP_SEL_TESTMODE_POS                                                         (6UL)
#define PCR_PADSWP_REG_SWP_HIGH_Z_MASK                                                              (0x00000010UL) /**< rw - Activates high.z. */
#define PCR_PADSWP_REG_SWP_HIGH_Z_POS                                                               (4UL)
#define PCR_PADSWP_REG_SWP_POWER_DOWN_MASK                                                          (0x00000008UL) /**< rw - Power Down Pad. */
#define PCR_PADSWP_REG_SWP_POWER_DOWN_POS                                                           (3UL)
#define PCR_PADSWP_REG_SWP_CUR_THRES_MASK                                                           (0x00000006UL) /**< rw - Current Threshold Detection. */
#define PCR_PADSWP_REG_SWP_CUR_THRES_POS                                                            (1UL)
#define PCR_PADSWP_REG_SWP_EXT_SUSPEND_MASK                                                         (0x00000001UL) /**< rw - Enables Extended Suspend. */
#define PCR_PADSWP_REG_SWP_EXT_SUSPEND_POS                                                          (0UL)
#define PCR_PADSWP_REG__RESET_VALUE                                                                 (0x0000020CUL) /**< value after reset */
/**
* @}
*/


/**
* \name S2C pad configuration
* <b>Reset value:</b> 0x00000008\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PAD_SWPM_DWP_REG                                                                        (0x400240A0UL) /**< rw-*/
#define PCR_PAD_SWPM_DWP_REG_DWP_RX_LATCH_BYPASS_MASK                                               (0x00000040UL) /**< rw - Bypass the latch delaying DWP_RX information reaching PCR.. */
#define PCR_PAD_SWPM_DWP_REG_DWP_RX_LATCH_BYPASS_POS                                                (6UL)
#define PCR_PAD_SWPM_DWP_REG_MUX_DWP_SWP_MASK                                                       (0x00000020UL) /**< rw - MUX to select between 0 ... SWP 1 ... DWP. */
#define PCR_PAD_SWPM_DWP_REG_MUX_DWP_SWP_POS                                                        (5UL)
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_SW_ENABLE_MASK                                                (0x00000010UL) /**< rw - Enabling Formware control of SWPM_DWP pads. */
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_SW_ENABLE_POS                                                 (4UL)
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_TX_EN_OUT_MASK                                                (0x00000008UL) /**< rw - Enabling output for SWPM DWP Transmitter pad. */
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_TX_EN_OUT_POS                                                 (3UL)
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_TX_EPD_MASK                                                   (0x00000004UL) /**< rw - Enabling weak pull down for SWPM DWP Transmitter pad. */
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_TX_EPD_POS                                                    (2UL)
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_RX_EN_IN_MASK                                                 (0x00000002UL) /**< rw - Enabling reciever for SWPM DWP Receiver pad. */
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_RX_EN_IN_POS                                                  (1UL)
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_RX_EPD_MASK                                                   (0x00000001UL) /**< rw - Enabling weak pull down for SWPM DWP Receiver pad. */
#define PCR_PAD_SWPM_DWP_REG_SWPM_DWP_RX_EPD_POS                                                    (0UL)
#define PCR_PAD_SWPM_DWP_REG__RESET_VALUE                                                           (0x00000008UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C master pad configuration
* <b>Reset value:</b> 0x00000280\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_PADIICM_REG                                                                             (0x400240A4UL) /**< rw-*/
#define PCR_PADIICM_REG_IICM_ENABLE_MASK                                                            (0x00000800UL) /**< rw - Enabling I2C master, disabling this allows programming I2C master pads as GPIO if IICM_SW_ENABLE =1 ( Software control mode). */
#define PCR_PADIICM_REG_IICM_ENABLE_POS                                                             (11UL)
#define PCR_PADIICM_REG_IICM_SW_ENABLE_MASK                                                         (0x00000400UL) /**< rw - Enables register control of I2C pads. */
#define PCR_PADIICM_REG_IICM_SW_ENABLE_POS                                                          (10UL)
#define PCR_PADIICM_REG_SDA_EN_IN_MASK                                                              (0x00000200UL) /**< rw - Enables input driver for SDA. */
#define PCR_PADIICM_REG_SDA_EN_IN_POS                                                               (9UL)
#define PCR_PADIICM_REG_SDA_EN_OUT_MASK                                                             (0x00000100UL) /**< rw - Enables output driver for SDA. */
#define PCR_PADIICM_REG_SDA_EN_OUT_POS                                                              (8UL)
#define PCR_PADIICM_REG_SCL_EN_IN_MASK                                                              (0x00000080UL) /**< rw - Enables input driver for SCL. */
#define PCR_PADIICM_REG_SCL_EN_IN_POS                                                               (7UL)
#define PCR_PADIICM_REG_SCL_EN_OUT_MASK                                                             (0x00000040UL) /**< rw - Enables output driver for SCL. */
#define PCR_PADIICM_REG_SCL_EN_OUT_POS                                                              (6UL)
#define PCR_PADIICM_REG_SDA_PUPD_MASK                                                               (0x00000030UL) /**< rw - Enables pullup/down functionality on SDA. */
#define PCR_PADIICM_REG_SDA_PUPD_POS                                                                (4UL)
#define PCR_PADIICM_REG_SCL_PUPD_MASK                                                               (0x0000000CUL) /**< rw - Enables pullup/down functionality on SCL. */
#define PCR_PADIICM_REG_SCL_PUPD_POS                                                                (2UL)
#define PCR_PADIICM_REG_SDA_SLEW_MASK                                                               (0x00000002UL) /**< rw - SDA Slew Rate. */
#define PCR_PADIICM_REG_SDA_SLEW_POS                                                                (1UL)
#define PCR_PADIICM_REG_SCL_SLEW_MASK                                                               (0x00000001UL) /**< rw - SCL Slew Rate. */
#define PCR_PADIICM_REG_SCL_SLEW_POS                                                                (0UL)
#define PCR_PADIICM_REG__RESET_VALUE                                                                (0x00000280UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLIF standby GSN value selection
* <b>Reset value:</b> 0x0000000F\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_ANA_TX_STANDBY_REG                                                                      (0x400240A8UL) /**< rw-*/
#define PCR_ANA_TX_STANDBY_REG_TX_GSN_SRC_SEL_MASK                                                  (0x00000010UL) /**< rw - Source of GSN value (0...PCR, 1...CLIF). */
#define PCR_ANA_TX_STANDBY_REG_TX_GSN_SRC_SEL_POS                                                   (4UL)
#define PCR_ANA_TX_STANDBY_REG_TX_GSN_CW_SB_MASK                                                    (0x0000000FUL) /**< rw - GSN Value for Standby Mode. */
#define PCR_ANA_TX_STANDBY_REG_TX_GSN_CW_SB_POS                                                     (0UL)
#define PCR_ANA_TX_STANDBY_REG__RESET_VALUE                                                         (0x0000000FUL) /**< value after reset */
/**
* @}
*/


/**
* \name CLIF configuration related to powerdown
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_ANA_TXPROT_REG                                                                          (0x400240ACUL) /**< rw-*/
#define PCR_ANA_TXPROT_REG_RX_PROT_IDDQ_MASK                                                        (0x00000010UL) /**< rw - Set RX protection to power down for Iddq measurement. */
#define PCR_ANA_TXPROT_REG_RX_PROT_IDDQ_POS                                                         (4UL)
#define PCR_ANA_TXPROT_REG_TXPROT_ENABLE_AUTO_FREEZE_MASK                                           (0x00000008UL) /**< rw - Enable automatic freeze during typeB demodulation: typeB_det =1 -> freeze; else -> no freeze. */
#define PCR_ANA_TXPROT_REG_TXPROT_ENABLE_AUTO_FREEZE_POS                                            (3UL)
#define PCR_ANA_TXPROT_REG_TXPROT_LIM_FREEZE_MASK                                                   (0x00000004UL) /**< rw - freeze limiter impedance, active high. */
#define PCR_ANA_TXPROT_REG_TXPROT_LIM_FREEZE_POS                                                    (2UL)
#define PCR_ANA_TXPROT_REG_TXPROT_PD_VREF_MASK                                                      (0x00000002UL) /**< rw - power down refernce voltage generation for tx-prot. */
#define PCR_ANA_TXPROT_REG_TXPROT_PD_VREF_POS                                                       (1UL)
#define PCR_ANA_TXPROT_REG_TXPROT_ENABLE_MASK                                                       (0x00000001UL) /**< rw - enables tx protection. */
#define PCR_ANA_TXPROT_REG_TXPROT_ENABLE_POS                                                        (0UL)
#define PCR_ANA_TXPROT_REG__RESET_VALUE                                                             (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name Test bus selection
* <b>Reset value:</b> 0x00000041\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_TBSEL_REG                                                                               (0x400240B0UL) /**< rw-*/
#define PCR_TBSEL_REG_SPARE_CELL_MASK                                                               (0xFF800000UL) /**< rw - spare cells (not connected). */
#define PCR_TBSEL_REG_SPARE_CELL_POS                                                                (23UL)
#define PCR_TBSEL_REG_TBUS_USBPAD_DYN_EN_MASK                                                       (0x00400000UL) /**< rw - USB pad API,APM, dyn_en control. */
#define PCR_TBSEL_REG_TBUS_USBPAD_DYN_EN_POS                                                        (22UL)
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_BYPASS_LATCH_CURLIM_MASK                                     (0x00200000UL) /**< rw - when 1, the current limit latch that sets the current clamp is bypassed and the output of the current limiter directly controls the current clamp. */
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_BYPASS_LATCH_CURLIM_POS                                      (21UL)
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_DISABLE_CURLIM_MASK                                          (0x00100000UL) /**< rw - when 1, it disables current limit. */
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_DISABLE_CURLIM_POS                                           (20UL)
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_CURLIM_HIGHTH_EN_MASK                                        (0x00080000UL) /**< rw - enable to increase current limiter threshold to 100mA. */
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_CURLIM_HIGHTH_EN_POS                                         (19UL)
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_CURLIM_LOWTH_EN_MASK                                         (0x00040000UL) /**< rw - enable to decrease current limiter threshold to 75mA. */
#define PCR_TBSEL_REG_PVDDLDO_TESTMODE_CURLIM_LOWTH_EN_POS                                          (18UL)
#define PCR_TBSEL_REG_EN_TBUS_STBY_MASK                                                             (0x00020000UL) /**< rw - Enable Testbus 2 during standby mode. */
#define PCR_TBSEL_REG_EN_TBUS_STBY_POS                                                              (17UL)
#define PCR_TBSEL_REG_PAD_TB_SEL_MASK                                                               (0x0001F000UL) /**< rw - Testbus select for Pad control logic.. */
#define PCR_TBSEL_REG_PAD_TB_SEL_POS                                                                (12UL)
#define PCR_TBSEL_REG_TB2_SEL_MASK                                                                  (0x00000FC0UL) /**< rw - Testbus 2 Seleciton. */
#define PCR_TBSEL_REG_TB2_SEL_POS                                                                   (6UL)
#define PCR_TBSEL_REG_TB1_SEL_MASK                                                                  (0x0000003FUL) /**< rw - Testbus 1 Selection. */
#define PCR_TBSEL_REG_TB1_SEL_POS                                                                   (0UL)
#define PCR_TBSEL_REG__RESET_VALUE                                                                  (0x00000041UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM master pad configuration
* <b>Reset value:</b> 0x00000040\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_SPIM_REG                                                                                (0x400240B4UL) /**< rw-*/
#define PCR_SPIM_REG_SPIM_NSS1_EN_MASK                                                              (0x00080000UL) /**< rw - SPIM master second slave enabled : Slave 1, Slave0 enabled by default.. */
#define PCR_SPIM_REG_SPIM_NSS1_EN_POS                                                               (19UL)
#define PCR_SPIM_REG_SPIM_MOSI_EN_OUT_MASK                                                          (0x00040000UL) /**< rw - SW control for SPIM_MOSI ECS. */
#define PCR_SPIM_REG_SPIM_MOSI_EN_OUT_POS                                                           (18UL)
#define PCR_SPIM_REG_SPIM_MISO_EN_OUT_MASK                                                          (0x00020000UL) /**< rw - SW control for SPIM_MISO ECS. */
#define PCR_SPIM_REG_SPIM_MISO_EN_OUT_POS                                                           (17UL)
#define PCR_SPIM_REG_SPIM_NSS_EN_OUT_MASK                                                           (0x00010000UL) /**< rw - SW control for SPIM_NSS ECS. */
#define PCR_SPIM_REG_SPIM_NSS_EN_OUT_POS                                                            (16UL)
#define PCR_SPIM_REG_SPIM_SCK_EN_OUT_MASK                                                           (0x00008000UL) /**< rw - SW control for SPIM_SCK ECS. */
#define PCR_SPIM_REG_SPIM_SCK_EN_OUT_POS                                                            (15UL)
#define PCR_SPIM_REG_SPIM_MOSI_EN_IN_MASK                                                           (0x00004000UL) /**< rw - SW control for SPIM_MOSI ENZI ( ENZI = ~EN_IN). */
#define PCR_SPIM_REG_SPIM_MOSI_EN_IN_POS                                                            (14UL)
#define PCR_SPIM_REG_SPIM_MISO_EN_IN_MASK                                                           (0x00002000UL) /**< rw - SW control for SPIM_MISO ENZI ( ENZI = ~EN_IN). */
#define PCR_SPIM_REG_SPIM_MISO_EN_IN_POS                                                            (13UL)
#define PCR_SPIM_REG_SPIM_NSS_EN_IN_MASK                                                            (0x00001000UL) /**< rw - SW control for SPIM_NSS ENZI ( ENZI = ~EN_IN). */
#define PCR_SPIM_REG_SPIM_NSS_EN_IN_POS                                                             (12UL)
#define PCR_SPIM_REG_SPIM_SCK_EN_IN_MASK                                                            (0x00000800UL) /**< rw - SW control for SPIM_SCK ENZI ( ENZI = ~EN_IN). */
#define PCR_SPIM_REG_SPIM_SCK_EN_IN_POS                                                             (11UL)
#define PCR_SPIM_REG_SPIM_MISO_EPUD_MASK                                                            (0x00000600UL) /**< rw - SW control for SPIM_MISO EPUD/EPD. */
#define PCR_SPIM_REG_SPIM_MISO_EPUD_POS                                                             (9UL)
#define PCR_SPIM_REG_SPIM_MOSI_EPUD_MASK                                                            (0x00000180UL) /**< rw - SW control for SPIM_MOSI EPUD/EPD. */
#define PCR_SPIM_REG_SPIM_MOSI_EPUD_POS                                                             (7UL)
#define PCR_SPIM_REG_SPIM_NSS_EPUD_MASK                                                             (0x00000060UL) /**< rw - SW control for SPIM_NSS EPUD/EPD. */
#define PCR_SPIM_REG_SPIM_NSS_EPUD_POS                                                              (5UL)
#define PCR_SPIM_REG_SPIM_SCK_EPUD_MASK                                                             (0x00000018UL) /**< rw - SW control for SPIM_SCK EPUD/EPD. */
#define PCR_SPIM_REG_SPIM_SCK_EPUD_POS                                                              (3UL)
#define PCR_SPIM_REG_EHS_MASK                                                                       (0x00000004UL) /**< rw - Select dirver strength for SPIM pads. */
#define PCR_SPIM_REG_EHS_POS                                                                        (2UL)
#define PCR_SPIM_REG_SSUP_MASK                                                                      (0x00000002UL) /**< rw - Supply voltage for SPIM pads. */
#define PCR_SPIM_REG_SSUP_POS                                                                       (1UL)
#define PCR_SPIM_REG_SPIM_SW_ENABLE_MASK                                                            (0x00000001UL) /**< rw - Enables Pad configuration for SPI master. */
#define PCR_SPIM_REG_SPIM_SW_ENABLE_POS                                                             (0UL)
#define PCR_SPIM_REG__RESET_VALUE                                                                   (0x00000040UL) /**< value after reset */
/**
* @}
*/


/**
* \name CTIF presense detection pull-up
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_CTIF_REG                                                                                (0x400240B8UL) /**< rw-*/
#define PCR_CTIF_REG_CT_PRES_ENABLE_PUP_MASK                                                        (0x00000001UL) /**< r- - Value of ct_pres_enable_pup_i coming from CTIF is latched and is used to detect card activity during standby and suspend modes if CTIF is enabled as wakeup source. */
#define PCR_CTIF_REG_CT_PRES_ENABLE_PUP_POS                                                         (0UL)
#define PCR_CTIF_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Host interface Tx/RX divider value storage during standby
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_HOSTIF_SAVE1_REG                                                                        (0x400240BCUL) /**< rw-*/
#define PCR_HOSTIF_SAVE1_REG_HSU_TX_DIVIDER_MASK                                                    (0x00000400UL) /**< rw - TX Divider save (only save reg for stby). */
#define PCR_HOSTIF_SAVE1_REG_HSU_TX_DIVIDER_POS                                                     (10UL)
#define PCR_HOSTIF_SAVE1_REG_HSU_RX_DIVIDER_MASK                                                    (0x000003FFUL) /**< rw - RX Divider save (only save reg for stby). */
#define PCR_HOSTIF_SAVE1_REG_HSU_RX_DIVIDER_POS                                                     (0UL)
#define PCR_HOSTIF_SAVE1_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Host interface clock value storage during standby
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_HOSTIF_SAVE2_REG                                                                        (0x400240C0UL) /**< rw-*/
#define PCR_HOSTIF_SAVE2_REG_HSU_TX_CLK_CORRECT_MASK                                                (0x00FFF800UL) /**< rw - Clock correction for TX (only save reg for stby). */
#define PCR_HOSTIF_SAVE2_REG_HSU_TX_CLK_CORRECT_POS                                                 (11UL)
#define PCR_HOSTIF_SAVE2_REG_HSU_RX_CLK_CORRECT_MASK                                                (0x000007FFUL) /**< rw - Clock correction for RX (only save reg for stby). */
#define PCR_HOSTIF_SAVE2_REG_HSU_RX_CLK_CORRECT_POS                                                 (0UL)
#define PCR_HOSTIF_SAVE2_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TXLDO sequence management
* <b>Reset value:</b> 0x00000008\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_TXLDO_MON_REG                                                                           (0x400240C4UL) /**< rw-*/
#define PCR_TXLDO_MON_REG_WELL_MNGT_MASK                                                            (0x00000008UL) /**< rw - TXLDO well mngt. */
#define PCR_TXLDO_MON_REG_WELL_MNGT_POS                                                             (3UL)
#define PCR_TXLDO_MON_REG_SRC_5V_MONITOR_MASK                                                       (0x00000004UL) /**< rw - Input of the 5V monitor.. */
#define PCR_TXLDO_MON_REG_SRC_5V_MONITOR_POS                                                        (2UL)
#define PCR_TXLDO_MON_REG_THRES_5V_MONITOR_MASK                                                     (0x00000002UL) /**< rw - Comparator threshold selector. */
#define PCR_TXLDO_MON_REG_THRES_5V_MONITOR_POS                                                      (1UL)
#define PCR_TXLDO_MON_REG_EN_5V_MONITOR_MASK                                                        (0x00000001UL) /**< rw - Enable the 5V monitor on TVDD. */
#define PCR_TXLDO_MON_REG_EN_5V_MONITOR_POS                                                         (0UL)
#define PCR_TXLDO_MON_REG__RESET_VALUE                                                              (0x00000008UL) /**< value after reset */
/**
* @}
*/


/**
* \name General Purpose Register 3 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_BOOT2_REG                                                                               (0x400240C8UL) /**< rw-*/
#define PCR_BOOT2_REG_SPARE_CELL3_MASK                                                              (0xFF000000UL) /**< rw - Third set of spare cells.. */
#define PCR_BOOT2_REG_SPARE_CELL3_POS                                                               (24UL)
#define PCR_BOOT2_REG_BOOT_REASON_ACTIVE_HPD_MASK                                                   (0x00800000UL) /**< r- - Boot because of coming out of ACTIVE_HPD. */
#define PCR_BOOT2_REG_BOOT_REASON_ACTIVE_HPD_POS                                                    (23UL)
#define PCR_BOOT2_REG_BOOT_REASON_VBUS_LOW_MASK                                                     (0x00400000UL) /**< r- - Boot because of VBUS going low in suspend or standby.. */
#define PCR_BOOT2_REG_BOOT_REASON_VBUS_LOW_POS                                                      (22UL)
#define PCR_BOOT2_REG_SPARE_CELL2_MASK                                                              (0x003FFFFFUL) /**< rw - Second set of space cells. */
#define PCR_BOOT2_REG_SPARE_CELL2_POS                                                               (0UL)
#define PCR_BOOT2_REG__RESET_VALUE                                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General Purpose Register 3 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG3_REG                                                                              (0x400240CCUL) /**< rw-*/
#define PCR_GPREG3_REG_PCR_GPREG3_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG3_REG_PCR_GPREG3_POS                                                               (0UL)
#define PCR_GPREG3_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General Purpose Register 4 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG4_REG                                                                              (0x400240D0UL) /**< rw-*/
#define PCR_GPREG4_REG_PCR_GPREG4_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG4_REG_PCR_GPREG4_POS                                                               (0UL)
#define PCR_GPREG4_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General Purpose Register 5 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG5_REG                                                                              (0x400240D4UL) /**< rw-*/
#define PCR_GPREG5_REG_PCR_GPREG5_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG5_REG_PCR_GPREG5_POS                                                               (0UL)
#define PCR_GPREG5_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General Purpose Register 6 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG6_REG                                                                              (0x400240D8UL) /**< rw-*/
#define PCR_GPREG6_REG_PCR_GPREG6_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG6_REG_PCR_GPREG6_POS                                                               (0UL)
#define PCR_GPREG6_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name General Purpose Register 7 for SW
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPREG7_REG                                                                              (0x400240DCUL) /**< rw-*/
#define PCR_GPREG7_REG_PCR_GPREG7_MASK                                                              (0xFFFFFFFFUL) /**< rw -  General Purpose Register for SW. */
#define PCR_GPREG7_REG_PCR_GPREG7_POS                                                               (0UL)
#define PCR_GPREG7_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Register to program is gpio interrupts are active low level/ falling edge sensitive
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPIO_INT_ACTIVE_LOW_REG                                                                 (0x400240E0UL) /**< rw-*/
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO12_INTR_ACTIVE_LOW_MASK                                     (0x00000800UL) /**< rw - Indicates if GPIO12 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO12_INTR_ACTIVE_LOW_POS                                      (11UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO11_INTR_ACTIVE_LOW_MASK                                     (0x00000400UL) /**< rw - Indicates if GPIO11 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO11_INTR_ACTIVE_LOW_POS                                      (10UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO10_INTR_ACTIVE_LOW_MASK                                     (0x00000200UL) /**< rw - Indicates if GPIO10 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO10_INTR_ACTIVE_LOW_POS                                      (9UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO9_INTR_ACTIVE_LOW_MASK                                      (0x00000100UL) /**< rw - Indicates if GPIO9 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO9_INTR_ACTIVE_LOW_POS                                       (8UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO8_INTR_ACTIVE_LOW_MASK                                      (0x00000080UL) /**< rw - Indicates if GPIO8 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO8_INTR_ACTIVE_LOW_POS                                       (7UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO7_INTR_ACTIVE_LOW_MASK                                      (0x00000040UL) /**< rw - Indicates if GPIO7 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO7_INTR_ACTIVE_LOW_POS                                       (6UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO6_INTR_ACTIVE_LOW_MASK                                      (0x00000020UL) /**< rw - Indicates if GPIO6 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO6_INTR_ACTIVE_LOW_POS                                       (5UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO5_INTR_ACTIVE_LOW_MASK                                      (0x00000010UL) /**< rw - Indicates if GPIO5 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO5_INTR_ACTIVE_LOW_POS                                       (4UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO4_INTR_ACTIVE_LOW_MASK                                      (0x00000008UL) /**< rw - Indicates if GPIO4 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO4_INTR_ACTIVE_LOW_POS                                       (3UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO3_INTR_ACTIVE_LOW_MASK                                      (0x00000004UL) /**< rw - Indicates if GPIO3 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO3_INTR_ACTIVE_LOW_POS                                       (2UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO2_INTR_ACTIVE_LOW_MASK                                      (0x00000002UL) /**< rw - Indicates if GPIO2 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO2_INTR_ACTIVE_LOW_POS                                       (1UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO1_INTR_ACTIVE_LOW_MASK                                      (0x00000001UL) /**< rw - Indicates if GPIO1 interrupts are active low/falling edge. 0--> Active_high/Rising edge; 1--> Active_low/falling edge.. */
#define PCR_GPIO_INT_ACTIVE_LOW_REG_GPIO1_INTR_ACTIVE_LOW_POS                                       (0UL)
#define PCR_GPIO_INT_ACTIVE_LOW_REG__RESET_VALUE                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Register to program if gpio interrupts are level sensitive
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPIO_INT_LEVEL_SENSE_REG                                                                (0x400240E4UL) /**< rw-*/
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO12_INTR_LEVEL_SENSITIVE_MASK                               (0x00000800UL) /**< rw - Indicates if GPIO12 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO12_INTR_LEVEL_SENSITIVE_POS                                (11UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO11_INTR_LEVEL_SENSITIVE_MASK                               (0x00000400UL) /**< rw - Indicates if GPIO11 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO11_INTR_LEVEL_SENSITIVE_POS                                (10UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO10_INTR_LEVEL_SENSITIVE_MASK                               (0x00000200UL) /**< rw - Indicates if GPIO10 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO10_INTR_LEVEL_SENSITIVE_POS                                (9UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO9_INTR_LEVEL_SENSITIVE_MASK                                (0x00000100UL) /**< rw - Indicates if GPIO9 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO9_INTR_LEVEL_SENSITIVE_POS                                 (8UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO8_INTR_LEVEL_SENSITIVE_MASK                                (0x00000080UL) /**< rw - Indicates if GPIO8 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO8_INTR_LEVEL_SENSITIVE_POS                                 (7UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO7_INTR_LEVEL_SENSITIVE_MASK                                (0x00000040UL) /**< rw - Indicates if GPIO7 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO7_INTR_LEVEL_SENSITIVE_POS                                 (6UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO6_INTR_LEVEL_SENSITIVE_MASK                                (0x00000020UL) /**< rw - Indicates if GPIO6 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO6_INTR_LEVEL_SENSITIVE_POS                                 (5UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO5_INTR_LEVEL_SENSITIVE_MASK                                (0x00000010UL) /**< rw - Indicates if GPIO5 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO5_INTR_LEVEL_SENSITIVE_POS                                 (4UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO4_INTR_LEVEL_SENSITIVE_MASK                                (0x00000008UL) /**< rw - Indicates if GPIO4 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO4_INTR_LEVEL_SENSITIVE_POS                                 (3UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO3_INTR_LEVEL_SENSITIVE_MASK                                (0x00000004UL) /**< rw - Indicates if GPIO3 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO3_INTR_LEVEL_SENSITIVE_POS                                 (2UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO2_INTR_LEVEL_SENSITIVE_MASK                                (0x00000002UL) /**< rw - Indicates if GPIO2 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO2_INTR_LEVEL_SENSITIVE_POS                                 (1UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO1_INTR_LEVEL_SENSITIVE_MASK                                (0x00000001UL) /**< rw - Indicates if GPIO1 interrupts are level sensitive/edge sensitive. 0--> Edge sensitive; 1--> Level sensitive.. */
#define PCR_GPIO_INT_LEVEL_SENSE_REG_GPIO1_INTR_LEVEL_SENSITIVE_POS                                 (0UL)
#define PCR_GPIO_INT_LEVEL_SENSE_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Register to program if gpio interrupts are both edge sensitive
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG                                                            (0x400240E8UL) /**< rw-*/
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO12_INTR_BOTH_EDGE_SENSITIVE_MASK                       (0x00000800UL) /**< rw - Indicates if GPIO12 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO12_INTR_BOTH_EDGE_SENSITIVE_POS                        (11UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO11_INTR_BOTH_EDGE_SENSITIVE_MASK                       (0x00000400UL) /**< rw - Indicates if GPIO11 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO11_INTR_BOTH_EDGE_SENSITIVE_POS                        (10UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO10_INTR_BOTH_EDGE_SENSITIVE_MASK                       (0x00000200UL) /**< rw - Indicates if GPIO10 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO10_INTR_BOTH_EDGE_SENSITIVE_POS                        (9UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO9_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000100UL) /**< rw - Indicates if GPIO9 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO9_INTR_BOTH_EDGE_SENSITIVE_POS                         (8UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO8_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000080UL) /**< rw - Indicates if GPIO8 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO8_INTR_BOTH_EDGE_SENSITIVE_POS                         (7UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO7_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000040UL) /**< rw - Indicates if GPIO7 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO7_INTR_BOTH_EDGE_SENSITIVE_POS                         (6UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO6_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000020UL) /**< rw - Indicates if GPIO6 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO6_INTR_BOTH_EDGE_SENSITIVE_POS                         (5UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO5_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000010UL) /**< rw - Indicates if GPIO5 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO5_INTR_BOTH_EDGE_SENSITIVE_POS                         (4UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO4_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000008UL) /**< rw - Indicates if GPIO4 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO4_INTR_BOTH_EDGE_SENSITIVE_POS                         (3UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO3_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000004UL) /**< rw - Indicates if GPIO3 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO3_INTR_BOTH_EDGE_SENSITIVE_POS                         (2UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO2_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000002UL) /**< rw - Indicates if GPIO2 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO2_INTR_BOTH_EDGE_SENSITIVE_POS                         (1UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO1_INTR_BOTH_EDGE_SENSITIVE_MASK                        (0x00000001UL) /**< rw - Indicates if GPIO1 interrupts are both positive and negative edge triggered. 0--> Not both edge triggered; 1--> both positive edge and negative edge triggered.. */
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG_GPIO1_INTR_BOTH_EDGE_SENSITIVE_POS                         (0UL)
#define PCR_GPIO_INT_ACTIVE_BOH_EDGE_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Register to program the source for system clock
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_SELECT_SYSTEMCLOCK                                                                      (0x400240ECUL) /**< rw-*/
#define PCR_SELECT_SYSTEMCLOCK_SELECT_SYSTEMCLOCK_MASK                                              (0x00000007UL) /**< rw - Selects the clock source   :: 3'b001...clkHFO (20 MHz) 3'b010...clkUSBPLL/2 (24MHz) 3'b100...clkXtal (27.12MHz) Others…INVALID. */
#define PCR_SELECT_SYSTEMCLOCK_SELECT_SYSTEMCLOCK_POS                                               (0UL)
#define PCR_SELECT_SYSTEMCLOCK__RESET_VALUE                                                         (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name Register for configuring advanced RFLD detection FSM
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_ADV_RFLD_REG                                                                            (0x400240F0UL) /**< rw-*/
#define PCR_ADV_RFLD_REG_ADV_RFLD_IBIAS_TIME_MASK                                                   (0x01F00000UL) /**< rw - Ibias Bootup Time. */
#define PCR_ADV_RFLD_REG_ADV_RFLD_IBIAS_TIME_POS                                                    (20UL)
#define PCR_ADV_RFLD_REG_ADV_RFLD_BYPASS_FREQ_CHECK_MASK                                            (0x00080000UL) /**< rw - Bypass the digital frequency check in case of malfunction of frequency check. */
#define PCR_ADV_RFLD_REG_ADV_RFLD_BYPASS_FREQ_CHECK_POS                                             (19UL)
#define PCR_ADV_RFLD_REG_ADV_RFLD_BYPASS_MASK                                                       (0x00040000UL) /**< rw - Enable Bypass for PreAmplifier. To be set if normal RF Level detector is used for . */
#define PCR_ADV_RFLD_REG_ADV_RFLD_BYPASS_POS                                                        (18UL)
#define PCR_ADV_RFLD_REG_ADV_RFLD_PREAMP_GAIN_MASK                                                  (0x00030000UL) /**< rw - Gain Seting for PreAmplifier. */
#define PCR_ADV_RFLD_REG_ADV_RFLD_PREAMP_GAIN_POS                                                   (16UL)
#define PCR_ADV_RFLD_REG_ADV_RFLD_CLKREC_TIME_MASK                                                  (0x0000F000UL) /**< rw - Bootup Time for Clock Recovery. Defines time between switching on the Clock Recovery until Frequency Check is enabled. */
#define PCR_ADV_RFLD_REG_ADV_RFLD_CLKREC_TIME_POS                                                   (12UL)
#define PCR_ADV_RFLD_REG_ADV_RFLD_WAIT_TIME_MASK                                                    (0x00000F80UL) /**< rw - Bootup Time for Advanced RF Level Detector. Defines time between switching on the RF Level Detector until Field is checked.. */
#define PCR_ADV_RFLD_REG_ADV_RFLD_WAIT_TIME_POS                                                     (7UL)
#define PCR_ADV_RFLD_REG_ADV_RFLD_SLEEP_TIME_MASK                                                   (0x0000007FUL) /**< rw - Defines the Sleep Time (# LFO cycles) in the polling circuit where the Advanced RF Level Detector is switched off. . */
#define PCR_ADV_RFLD_REG_ADV_RFLD_SLEEP_TIME_POS                                                    (0UL)
#define PCR_ADV_RFLD_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Configuration bits for testing advanced RFLD detection FSM
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define PCR_ADV_RFLD_TEST_REG                                                                       (0x400240F4UL) /**< rw-*/
#define PCR_ADV_RFLD_TEST_REG_ADV_RFLD_TEST_BYPASS_FIELDDET_MASK                                    (0x00000004UL) /**< rw - Bypass the Field Detection during SLEEP TIME. */
#define PCR_ADV_RFLD_TEST_REG_ADV_RFLD_TEST_BYPASS_FIELDDET_POS                                     (2UL)
#define PCR_ADV_RFLD_TEST_REG_ADV_RFLD_TEST_PREAMP_ENABLE_MASK                                      (0x00000002UL) /**< rw - Enables the Advanced RF Level Detector during standby mode. The RF Level Detector as wakeup source must be enabled to use this feature. . */
#define PCR_ADV_RFLD_TEST_REG_ADV_RFLD_TEST_PREAMP_ENABLE_POS                                       (1UL)
#define PCR_ADV_RFLD_TEST_REG_ADV_RFLD_TEST_ENABLE_MASK                                             (0x00000001UL) /**< rw - Enable manual setting of Advanced RF Level Detection. . */
#define PCR_ADV_RFLD_TEST_REG_ADV_RFLD_TEST_ENABLE_POS                                              (0UL)
#define PCR_ADV_RFLD_TEST_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt clear enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PCR_INT_CLR_ENABLE_REG                                                                      (0x40027FD8UL) /**< -wm*/
#define PCR_INT_CLR_ENABLE_REG_GPIO12_PAD_HIGH_INT_CLR_ENABLE_MASK                                  (0x08000000UL) /**< -x - GPIO12 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO12_PAD_HIGH_INT_CLR_ENABLE_POS                                   (27UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO11_PAD_HIGH_INT_CLR_ENABLE_MASK                                  (0x04000000UL) /**< -x - GPIO11 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO11_PAD_HIGH_INT_CLR_ENABLE_POS                                   (26UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO10_PAD_HIGH_INT_CLR_ENABLE_MASK                                  (0x02000000UL) /**< -x - GPIO10 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO10_PAD_HIGH_INT_CLR_ENABLE_POS                                   (25UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO9_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x01000000UL) /**< -x - GPIO9 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO9_PAD_HIGH_INT_CLR_ENABLE_POS                                    (24UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO8_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00800000UL) /**< -x - GPIO8 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO8_PAD_HIGH_INT_CLR_ENABLE_POS                                    (23UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO7_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00400000UL) /**< -x - GPIO7 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO7_PAD_HIGH_INT_CLR_ENABLE_POS                                    (22UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO6_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00200000UL) /**< -x - GPIO6 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO6_PAD_HIGH_INT_CLR_ENABLE_POS                                    (21UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO5_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00100000UL) /**< -x - GPIO5 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO5_PAD_HIGH_INT_CLR_ENABLE_POS                                    (20UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO4_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00080000UL) /**< -x - GPIO4 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO4_PAD_HIGH_INT_CLR_ENABLE_POS                                    (19UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO3_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00040000UL) /**< -x - GPIO3 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO3_PAD_HIGH_INT_CLR_ENABLE_POS                                    (18UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO2_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00020000UL) /**< -x - GPIO2 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO2_PAD_HIGH_INT_CLR_ENABLE_POS                                    (17UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO1_PAD_HIGH_INT_CLR_ENABLE_MASK                                   (0x00010000UL) /**< -x - GPIO1 pad going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO1_PAD_HIGH_INT_CLR_ENABLE_POS                                    (16UL)
#define PCR_INT_CLR_ENABLE_REG_VBUSP_MON_HIGH_INT_CLR_ENABLE_MASK                                   (0x00000400UL) /**< -x - VBUSP monitor going high interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_VBUSP_MON_HIGH_INT_CLR_ENABLE_POS                                    (10UL)
#define PCR_INT_CLR_ENABLE_REG_CLIF_RFLD_ACT_INT_CLR_ENABLE_MASK                                    (0x00000200UL) /**< -x - Clif RF field activity observed interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_CLIF_RFLD_ACT_INT_CLR_ENABLE_POS                                     (9UL)
#define PCR_INT_CLR_ENABLE_REG_VBUS_MON_LOW_INT_CLR_ENABLE_MASK                                     (0x00000100UL) /**< -x - VBUS monitor going low interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_VBUS_MON_LOW_INT_CLR_ENABLE_POS                                      (8UL)
#define PCR_INT_CLR_ENABLE_REG_VBUSP_MON_LOW_INT_CLR_ENABLE_MASK                                    (0x00000080UL) /**< -x - VBUSP monitor going low interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_VBUSP_MON_LOW_INT_CLR_ENABLE_POS                                     (7UL)
#define PCR_INT_CLR_ENABLE_REG_PVDD_CURLIM_ACT_CLR_ENABLE_MASK                                      (0x00000040UL) /**< -x - PVDD current limiter active interrupt clear enable.. */
#define PCR_INT_CLR_ENABLE_REG_PVDD_CURLIM_ACT_CLR_ENABLE_POS                                       (6UL)
#define PCR_INT_CLR_ENABLE_REG_TEMPERROR1_INT_CLR_ENABLE_MASK                                       (0x00000020UL) /**< -x - Temperature error 1 interrupt  clear enable. */
#define PCR_INT_CLR_ENABLE_REG_TEMPERROR1_INT_CLR_ENABLE_POS                                        (5UL)
#define PCR_INT_CLR_ENABLE_REG_TEMPERROR0_INT_CLR_ENABLE_MASK                                       (0x00000010UL) /**< -x - Temperature error 0 interrupt  clear enable. */
#define PCR_INT_CLR_ENABLE_REG_TEMPERROR0_INT_CLR_ENABLE_POS                                        (4UL)
#define PCR_INT_CLR_ENABLE_REG_SUSPEND_DEFAULTED_INT_CLR_ENABLE_MASK                                (0x00000008UL) /**< -x - Suspend defaulted interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_SUSPEND_DEFAULTED_INT_CLR_ENABLE_POS                                 (3UL)
#define PCR_INT_CLR_ENABLE_REG_SUSPEND_INT_CLR_ENABLE_MASK                                          (0x00000002UL) /**< -x - Suspend interrupt clear enable.. */
#define PCR_INT_CLR_ENABLE_REG_SUSPEND_INT_CLR_ENABLE_POS                                           (1UL)
#define PCR_INT_CLR_ENABLE_REG_GPIO_INT_CLR_ENABLE_MASK                                             (0x00000001UL) /**< -x - GPIO interrupt clear enable. */
#define PCR_INT_CLR_ENABLE_REG_GPIO_INT_CLR_ENABLE_POS                                              (0UL)
#define PCR_INT_CLR_ENABLE_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt set enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PCR_INT_SET_ENABLE_REG                                                                      (0x40027FDCUL) /**< -wm*/
#define PCR_INT_SET_ENABLE_REG_GPIO12_PAD_HIGH_INT_SET_ENABLE_MASK                                  (0x08000000UL) /**< -x - GPIO12 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO12_PAD_HIGH_INT_SET_ENABLE_POS                                   (27UL)
#define PCR_INT_SET_ENABLE_REG_GPIO11_PAD_HIGH_INT_SET_ENABLE_MASK                                  (0x04000000UL) /**< -x - GPIO11 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO11_PAD_HIGH_INT_SET_ENABLE_POS                                   (26UL)
#define PCR_INT_SET_ENABLE_REG_GPIO10_PAD_HIGH_INT_SET_ENABLE_MASK                                  (0x02000000UL) /**< -x - GPIO10 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO10_PAD_HIGH_INT_SET_ENABLE_POS                                   (25UL)
#define PCR_INT_SET_ENABLE_REG_GPIO9_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x01000000UL) /**< -x - GPIO9 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO9_PAD_HIGH_INT_SET_ENABLE_POS                                    (24UL)
#define PCR_INT_SET_ENABLE_REG_GPIO8_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00800000UL) /**< -x - GPIO8 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO8_PAD_HIGH_INT_SET_ENABLE_POS                                    (23UL)
#define PCR_INT_SET_ENABLE_REG_GPIO7_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00400000UL) /**< -x - GPIO7 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO7_PAD_HIGH_INT_SET_ENABLE_POS                                    (22UL)
#define PCR_INT_SET_ENABLE_REG_GPIO6_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00200000UL) /**< -x - GPIO6 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO6_PAD_HIGH_INT_SET_ENABLE_POS                                    (21UL)
#define PCR_INT_SET_ENABLE_REG_GPIO5_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00100000UL) /**< -x - GPIO5 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO5_PAD_HIGH_INT_SET_ENABLE_POS                                    (20UL)
#define PCR_INT_SET_ENABLE_REG_GPIO4_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00080000UL) /**< -x - GPIO4 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO4_PAD_HIGH_INT_SET_ENABLE_POS                                    (19UL)
#define PCR_INT_SET_ENABLE_REG_GPIO3_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00040000UL) /**< -x - GPIO3 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO3_PAD_HIGH_INT_SET_ENABLE_POS                                    (18UL)
#define PCR_INT_SET_ENABLE_REG_GPIO2_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00020000UL) /**< -x - GPIO2 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO2_PAD_HIGH_INT_SET_ENABLE_POS                                    (17UL)
#define PCR_INT_SET_ENABLE_REG_GPIO1_PAD_HIGH_INT_SET_ENABLE_MASK                                   (0x00010000UL) /**< -x - GPIO1 pad going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO1_PAD_HIGH_INT_SET_ENABLE_POS                                    (16UL)
#define PCR_INT_SET_ENABLE_REG_VBUSP_MON_HIGH_INT_SET_ENABLE_MASK                                   (0x00000400UL) /**< -x - VBUSP monitor going high interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_VBUSP_MON_HIGH_INT_SET_ENABLE_POS                                    (10UL)
#define PCR_INT_SET_ENABLE_REG_CLIF_RFLD_ACT_INT_SET_ENABLE_MASK                                    (0x00000200UL) /**< -x - Clif RF field activity observed interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_CLIF_RFLD_ACT_INT_SET_ENABLE_POS                                     (9UL)
#define PCR_INT_SET_ENABLE_REG_VBUS_MON_LOW_INT_SET_ENABLE_MASK                                     (0x00000100UL) /**< -x - VBUS monitor going low interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_VBUS_MON_LOW_INT_SET_ENABLE_POS                                      (8UL)
#define PCR_INT_SET_ENABLE_REG_VBUSP_MON_LOW_INT_SET_ENABLE_MASK                                    (0x00000080UL) /**< -x - VBUSP monitor going low interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_VBUSP_MON_LOW_INT_SET_ENABLE_POS                                     (7UL)
#define PCR_INT_SET_ENABLE_REG_PVDD_CURLIM_ACT_SET_ENABLE_MASK                                      (0x00000040UL) /**< -x - PVDD current limiter active interrupt set enable.. */
#define PCR_INT_SET_ENABLE_REG_PVDD_CURLIM_ACT_SET_ENABLE_POS                                       (6UL)
#define PCR_INT_SET_ENABLE_REG_TEMPERROR1_INT_SET_ENABLE_MASK                                       (0x00000020UL) /**< -x - Temperature error 1 interrupt  set enable. */
#define PCR_INT_SET_ENABLE_REG_TEMPERROR1_INT_SET_ENABLE_POS                                        (5UL)
#define PCR_INT_SET_ENABLE_REG_TEMPERROR0_INT_SET_ENABLE_MASK                                       (0x00000010UL) /**< -x - Temperature error 0 interrupt  set enable. */
#define PCR_INT_SET_ENABLE_REG_TEMPERROR0_INT_SET_ENABLE_POS                                        (4UL)
#define PCR_INT_SET_ENABLE_REG_SUSPEND_DEFAULTED_INT_SET_ENABLE_MASK                                (0x00000008UL) /**< -x - Suspend defaulted interrupt set enable. */
#define PCR_INT_SET_ENABLE_REG_SUSPEND_DEFAULTED_INT_SET_ENABLE_POS                                 (3UL)
#define PCR_INT_SET_ENABLE_REG_SUSPEND_INT_SET_ENABLE_MASK                                          (0x00000002UL) /**< -x - Suspend interrupt set enable.. */
#define PCR_INT_SET_ENABLE_REG_SUSPEND_INT_SET_ENABLE_POS                                           (1UL)
#define PCR_INT_SET_ENABLE_REG_GPIO_INT_SET_ENABLE_MASK                                             (0x00000001UL) /**< -x - GPIO interrupt clear enable. */
#define PCR_INT_SET_ENABLE_REG_GPIO_INT_SET_ENABLE_POS                                              (0UL)
#define PCR_INT_SET_ENABLE_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define PCR_INT_STATUS_REG                                                                          (0x40027FE0UL) /**< r-m*/
#define PCR_INT_STATUS_REG_GPIO12_PAD_HIGH_INT_STATUS_MASK                                          (0x08000000UL) /**< r- - GPIO12 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO12_PAD_HIGH_INT_STATUS_POS                                           (27UL)
#define PCR_INT_STATUS_REG_GPIO11_PAD_HIGH_INT_STATUS_MASK                                          (0x04000000UL) /**< r- - GPIO11 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO11_PAD_HIGH_INT_STATUS_POS                                           (26UL)
#define PCR_INT_STATUS_REG_GPIO10_PAD_HIGH_INT_STATUS_MASK                                          (0x02000000UL) /**< r- - GPIO10 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO10_PAD_HIGH_INT_STATUS_POS                                           (25UL)
#define PCR_INT_STATUS_REG_GPIO9_PAD_HIGH_INT_STATUS_MASK                                           (0x01000000UL) /**< r- - GPIO9 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO9_PAD_HIGH_INT_STATUS_POS                                            (24UL)
#define PCR_INT_STATUS_REG_GPIO8_PAD_HIGH_INT_STATUS_MASK                                           (0x00800000UL) /**< r- - GPIO8 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO8_PAD_HIGH_INT_STATUS_POS                                            (23UL)
#define PCR_INT_STATUS_REG_GPIO7_PAD_HIGH_INT_STATUS_MASK                                           (0x00400000UL) /**< r- - GPIO7 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO7_PAD_HIGH_INT_STATUS_POS                                            (22UL)
#define PCR_INT_STATUS_REG_GPIO6_PAD_HIGH_INT_STATUS_MASK                                           (0x00200000UL) /**< r- - GPIO6 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO6_PAD_HIGH_INT_STATUS_POS                                            (21UL)
#define PCR_INT_STATUS_REG_GPIO5_PAD_HIGH_INT_STATUS_MASK                                           (0x00100000UL) /**< r- - GPIO5 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO5_PAD_HIGH_INT_STATUS_POS                                            (20UL)
#define PCR_INT_STATUS_REG_GPIO4_PAD_HIGH_INT_STATUS_MASK                                           (0x00080000UL) /**< r- - GPIO4 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO4_PAD_HIGH_INT_STATUS_POS                                            (19UL)
#define PCR_INT_STATUS_REG_GPIO3_PAD_HIGH_INT_STATUS_MASK                                           (0x00040000UL) /**< r- - GPIO3 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO3_PAD_HIGH_INT_STATUS_POS                                            (18UL)
#define PCR_INT_STATUS_REG_GPIO2_PAD_HIGH_INT_STATUS_MASK                                           (0x00020000UL) /**< r- - GPIO2 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO2_PAD_HIGH_INT_STATUS_POS                                            (17UL)
#define PCR_INT_STATUS_REG_GPIO1_PAD_HIGH_INT_STATUS_MASK                                           (0x00010000UL) /**< r- - GPIO1 pad going high interrupt status. */
#define PCR_INT_STATUS_REG_GPIO1_PAD_HIGH_INT_STATUS_POS                                            (16UL)
#define PCR_INT_STATUS_REG_VBUSP_MON_HIGH_INT_STATUS_MASK                                           (0x00000400UL) /**< r- - VBUSP monitor going high interrupt status. */
#define PCR_INT_STATUS_REG_VBUSP_MON_HIGH_INT_STATUS_POS                                            (10UL)
#define PCR_INT_STATUS_REG_CLIF_RFLD_ACT_INT_STATUS_MASK                                            (0x00000200UL) /**< r- - Clif RF field activity observed interrupt  status. */
#define PCR_INT_STATUS_REG_CLIF_RFLD_ACT_INT_STATUS_POS                                             (9UL)
#define PCR_INT_STATUS_REG_VBUS_MON_LOW_INT_STATUS_MASK                                             (0x00000100UL) /**< r- - VBUS monitor going low interrupt clear status. */
#define PCR_INT_STATUS_REG_VBUS_MON_LOW_INT_STATUS_POS                                              (8UL)
#define PCR_INT_STATUS_REG_VBUSP_MON_LOW_INT_STATUS_MASK                                            (0x00000080UL) /**< r- - VBUSP monitor going low interrupt clear status. */
#define PCR_INT_STATUS_REG_VBUSP_MON_LOW_INT_STATUS_POS                                             (7UL)
#define PCR_INT_STATUS_REG_PVDD_CURLIM_ACT_INT_STATUS_MASK                                          (0x00000040UL) /**< r- - PVDD current limiter active interrupt status. */
#define PCR_INT_STATUS_REG_PVDD_CURLIM_ACT_INT_STATUS_POS                                           (6UL)
#define PCR_INT_STATUS_REG_TEMPERROR1_INT_STATUS_MASK                                               (0x00000020UL) /**< r- - Temperature error 1 interrupt  status. */
#define PCR_INT_STATUS_REG_TEMPERROR1_INT_STATUS_POS                                                (5UL)
#define PCR_INT_STATUS_REG_TEMPERROR0_INT_STATUS_MASK                                               (0x00000010UL) /**< r- - Temperature error 0 interrupt  status. */
#define PCR_INT_STATUS_REG_TEMPERROR0_INT_STATUS_POS                                                (4UL)
#define PCR_INT_STATUS_REG_SUSPEND_DEFAULTED_INT_STATUS_MASK                                        (0x00000008UL) /**< r- - Suspend defaulted interrupt status. */
#define PCR_INT_STATUS_REG_SUSPEND_DEFAULTED_INT_STATUS_POS                                         (3UL)
#define PCR_INT_STATUS_REG_SUSPEND_INT_STATUS_MASK                                                  (0x00000002UL) /**< r- - Suspend interrupt status.. */
#define PCR_INT_STATUS_REG_SUSPEND_INT_STATUS_POS                                                   (1UL)
#define PCR_INT_STATUS_REG_GPIO_INT_STATUS_MASK                                                     (0x00000001UL) /**< r- - GPIO interrupt clear enable. */
#define PCR_INT_STATUS_REG_GPIO_INT_STATUS_POS                                                      (0UL)
#define PCR_INT_STATUS_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define PCR_INT_ENABLE_REG                                                                          (0x40027FE4UL) /**< r-m*/
#define PCR_INT_ENABLE_REG_GPIO12_PAD_HIGH_INT_ENABLE_MASK                                          (0x08000000UL) /**< r- - GPIO12 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO12_PAD_HIGH_INT_ENABLE_POS                                           (27UL)
#define PCR_INT_ENABLE_REG_GPIO11_PAD_HIGH_INT_ENABLE_MASK                                          (0x04000000UL) /**< r- - GPIO11 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO11_PAD_HIGH_INT_ENABLE_POS                                           (26UL)
#define PCR_INT_ENABLE_REG_GPIO10_PAD_HIGH_INT_ENABLE_MASK                                          (0x02000000UL) /**< r- - GPIO10 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO10_PAD_HIGH_INT_ENABLE_POS                                           (25UL)
#define PCR_INT_ENABLE_REG_GPIO9_PAD_HIGH_INT_ENABLE_MASK                                           (0x01000000UL) /**< r- - GPIO9 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO9_PAD_HIGH_INT_ENABLE_POS                                            (24UL)
#define PCR_INT_ENABLE_REG_GPIO8_PAD_HIGH_INT_ENABLE_MASK                                           (0x00800000UL) /**< r- - GPIO8 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO8_PAD_HIGH_INT_ENABLE_POS                                            (23UL)
#define PCR_INT_ENABLE_REG_GPIO7_PAD_HIGH_INT_ENABLE_MASK                                           (0x00400000UL) /**< r- - GPIO7 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO7_PAD_HIGH_INT_ENABLE_POS                                            (22UL)
#define PCR_INT_ENABLE_REG_GPIO6_PAD_HIGH_INT_ENABLE_MASK                                           (0x00200000UL) /**< r- - GPIO6 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO6_PAD_HIGH_INT_ENABLE_POS                                            (21UL)
#define PCR_INT_ENABLE_REG_GPIO5_PAD_HIGH_INT_ENABLE_MASK                                           (0x00100000UL) /**< r- - GPIO5 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO5_PAD_HIGH_INT_ENABLE_POS                                            (20UL)
#define PCR_INT_ENABLE_REG_GPIO4_PAD_HIGH_INT_ENABLE_MASK                                           (0x00080000UL) /**< r- - GPIO4 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO4_PAD_HIGH_INT_ENABLE_POS                                            (19UL)
#define PCR_INT_ENABLE_REG_GPIO3_PAD_HIGH_INT_ENABLE_MASK                                           (0x00040000UL) /**< r- - GPIO3 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO3_PAD_HIGH_INT_ENABLE_POS                                            (18UL)
#define PCR_INT_ENABLE_REG_GPIO2_PAD_HIGH_INT_ENABLE_MASK                                           (0x00020000UL) /**< r- - GPIO2 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO2_PAD_HIGH_INT_ENABLE_POS                                            (17UL)
#define PCR_INT_ENABLE_REG_GPIO1_PAD_HIGH_INT_ENABLE_MASK                                           (0x00010000UL) /**< r- - GPIO1 pad going high interrupt enable. */
#define PCR_INT_ENABLE_REG_GPIO1_PAD_HIGH_INT_ENABLE_POS                                            (16UL)
#define PCR_INT_ENABLE_REG_VBUSP_MON_HIGH_INT_ENABLE_MASK                                           (0x00000400UL) /**< r- - VBUSP monitor going high interrupt enable. */
#define PCR_INT_ENABLE_REG_VBUSP_MON_HIGH_INT_ENABLE_POS                                            (10UL)
#define PCR_INT_ENABLE_REG_CLIF_RFLD_ACT_INT_ENABLE_MASK                                            (0x00000200UL) /**< r- - Clif RF field activity observed interrupt enable. */
#define PCR_INT_ENABLE_REG_CLIF_RFLD_ACT_INT_ENABLE_POS                                             (9UL)
#define PCR_INT_ENABLE_REG_VBUS_MON_LOW_INT_ENABLE_MASK                                             (0x00000100UL) /**< r- - VBUS monitor going low interrupt enable. */
#define PCR_INT_ENABLE_REG_VBUS_MON_LOW_INT_ENABLE_POS                                              (8UL)
#define PCR_INT_ENABLE_REG_VBUSP_MON_LOW_INT_ENABLE_MASK                                            (0x00000080UL) /**< r- - VBUSP monitor going low interrupt enable. */
#define PCR_INT_ENABLE_REG_VBUSP_MON_LOW_INT_ENABLE_POS                                             (7UL)
#define PCR_INT_ENABLE_REG_PVDD_CURLIM_ACT_INT_ENABLE_MASK                                          (0x00000040UL) /**< r- - PVDD current limiter active interrupt enable. */
#define PCR_INT_ENABLE_REG_PVDD_CURLIM_ACT_INT_ENABLE_POS                                           (6UL)
#define PCR_INT_ENABLE_REG_TEMPERROR1_INT_ENABLE_MASK                                               (0x00000020UL) /**< r- - Temperature error 1 interrupt  enable. */
#define PCR_INT_ENABLE_REG_TEMPERROR1_INT_ENABLE_POS                                                (5UL)
#define PCR_INT_ENABLE_REG_TEMPERROR0_INT_ENABLE_MASK                                               (0x00000010UL) /**< r- - Temperature error 0 interrupt  enable. */
#define PCR_INT_ENABLE_REG_TEMPERROR0_INT_ENABLE_POS                                                (4UL)
#define PCR_INT_ENABLE_REG_SUSPEND_DEFAULTED_INT_ENABLE_MASK                                        (0x00000008UL) /**< r- - Suspend defaulted interrupt enable. */
#define PCR_INT_ENABLE_REG_SUSPEND_DEFAULTED_INT_ENABLE_POS                                         (3UL)
#define PCR_INT_ENABLE_REG_SUSPEND_INT_ENABLE_MASK                                                  (0x00000002UL) /**< r- - Suspend interrupt enable.. */
#define PCR_INT_ENABLE_REG_SUSPEND_INT_ENABLE_POS                                                   (1UL)
#define PCR_INT_ENABLE_REG_GPIO_INT_ENABLE_MASK                                                     (0x00000001UL) /**< r- - GPIO interrupt clear enable. */
#define PCR_INT_ENABLE_REG_GPIO_INT_ENABLE_POS                                                      (0UL)
#define PCR_INT_ENABLE_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt clear status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PCR_INT_CLR_STATUS_REG                                                                      (0x40027FE8UL) /**< -wm*/
#define PCR_INT_CLR_STATUS_REG_GPIO12_PAD_HIGH_INT_CLR_STATUS_MASK                                  (0x08000000UL) /**< -x - GPIO12 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO12_PAD_HIGH_INT_CLR_STATUS_POS                                   (27UL)
#define PCR_INT_CLR_STATUS_REG_GPIO11_PAD_HIGH_INT_CLR_STATUS_MASK                                  (0x04000000UL) /**< -x - GPIO11 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO11_PAD_HIGH_INT_CLR_STATUS_POS                                   (26UL)
#define PCR_INT_CLR_STATUS_REG_GPIO10_PAD_HIGH_INT_CLR_STATUS_MASK                                  (0x02000000UL) /**< -x - GPIO10 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO10_PAD_HIGH_INT_CLR_STATUS_POS                                   (25UL)
#define PCR_INT_CLR_STATUS_REG_GPIO9_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x01000000UL) /**< -x - GPIO9 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO9_PAD_HIGH_INT_CLR_STATUS_POS                                    (24UL)
#define PCR_INT_CLR_STATUS_REG_GPIO8_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00800000UL) /**< -x - GPIO8 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO8_PAD_HIGH_INT_CLR_STATUS_POS                                    (23UL)
#define PCR_INT_CLR_STATUS_REG_GPIO7_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00400000UL) /**< -x - GPIO7 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO7_PAD_HIGH_INT_CLR_STATUS_POS                                    (22UL)
#define PCR_INT_CLR_STATUS_REG_GPIO6_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00200000UL) /**< -x - GPIO6 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO6_PAD_HIGH_INT_CLR_STATUS_POS                                    (21UL)
#define PCR_INT_CLR_STATUS_REG_GPIO5_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00100000UL) /**< -x - GPIO5 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO5_PAD_HIGH_INT_CLR_STATUS_POS                                    (20UL)
#define PCR_INT_CLR_STATUS_REG_GPIO4_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00080000UL) /**< -x - GPIO4 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO4_PAD_HIGH_INT_CLR_STATUS_POS                                    (19UL)
#define PCR_INT_CLR_STATUS_REG_GPIO3_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00040000UL) /**< -x - GPIO3 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO3_PAD_HIGH_INT_CLR_STATUS_POS                                    (18UL)
#define PCR_INT_CLR_STATUS_REG_GPIO2_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00020000UL) /**< -x - GPIO2 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO2_PAD_HIGH_INT_CLR_STATUS_POS                                    (17UL)
#define PCR_INT_CLR_STATUS_REG_GPIO1_PAD_HIGH_INT_CLR_STATUS_MASK                                   (0x00010000UL) /**< -x - GPIO1 pad going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO1_PAD_HIGH_INT_CLR_STATUS_POS                                    (16UL)
#define PCR_INT_CLR_STATUS_REG_VBUSP_MON_HIGH_INT_CLR_STATUS_MASK                                   (0x00000400UL) /**< -x - VBUSP monitor going high interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_VBUSP_MON_HIGH_INT_CLR_STATUS_POS                                    (10UL)
#define PCR_INT_CLR_STATUS_REG_CLIF_RFLD_ACT_INT_CLR_STATUS_MASK                                    (0x00000200UL) /**< -x - Clif RF field activity observed interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_CLIF_RFLD_ACT_INT_CLR_STATUS_POS                                     (9UL)
#define PCR_INT_CLR_STATUS_REG_VBUS_MON_LOW_INT_CLR_STATUS_MASK                                     (0x00000100UL) /**< -x - VBUS monitor going low interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_VBUS_MON_LOW_INT_CLR_STATUS_POS                                      (8UL)
#define PCR_INT_CLR_STATUS_REG_VBUSP_MON_LOW_INT_CLR_STATUS_MASK                                    (0x00000080UL) /**< -x - VBUSP monitor going low interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_VBUSP_MON_LOW_INT_CLR_STATUS_POS                                     (7UL)
#define PCR_INT_CLR_STATUS_REG_PVDD_CURLIM_ACT_CLR_STATUS_MASK                                      (0x00000040UL) /**< -x - PVDD current limiter active interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_PVDD_CURLIM_ACT_CLR_STATUS_POS                                       (6UL)
#define PCR_INT_CLR_STATUS_REG_TEMPERROR1_INT_CLR_STATUS_MASK                                       (0x00000020UL) /**< -x - Temperature error 1 interrupt  clear status. */
#define PCR_INT_CLR_STATUS_REG_TEMPERROR1_INT_CLR_STATUS_POS                                        (5UL)
#define PCR_INT_CLR_STATUS_REG_TEMPERROR0_INT_CLR_STATUS_MASK                                       (0x00000010UL) /**< -x - Temperature error 0 interrupt  clear status. */
#define PCR_INT_CLR_STATUS_REG_TEMPERROR0_INT_CLR_STATUS_POS                                        (4UL)
#define PCR_INT_CLR_STATUS_REG_SUSPEND_DEFAULTED_INT_CLR_STATUS_MASK                                (0x00000008UL) /**< -x - Suspend defaulted interrupt clear status. */
#define PCR_INT_CLR_STATUS_REG_SUSPEND_DEFAULTED_INT_CLR_STATUS_POS                                 (3UL)
#define PCR_INT_CLR_STATUS_REG_SUSPEND_INT_CLR_STATUS_MASK                                          (0x00000002UL) /**< -x - Suspend interrupt clear status.. */
#define PCR_INT_CLR_STATUS_REG_SUSPEND_INT_CLR_STATUS_POS                                           (1UL)
#define PCR_INT_CLR_STATUS_REG_GPIO_INT_CLR_STATUS_MASK                                             (0x00000001UL) /**< -x - GPIO interrupt clear clear status. */
#define PCR_INT_CLR_STATUS_REG_GPIO_INT_CLR_STATUS_POS                                              (0UL)
#define PCR_INT_CLR_STATUS_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt set status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -wm
* @{
*/
#define PCR_INT_SET_STATUS_REG                                                                      (0x40027FECUL) /**< -wm*/
#define PCR_INT_SET_STATUS_REG_GPIO12_PAD_HIGH_INT_SET_STATUS_MASK                                  (0x08000000UL) /**< -x - GPIO12 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO12_PAD_HIGH_INT_SET_STATUS_POS                                   (27UL)
#define PCR_INT_SET_STATUS_REG_GPIO11_PAD_HIGH_INT_SET_STATUS_MASK                                  (0x04000000UL) /**< -x - GPIO11 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO11_PAD_HIGH_INT_SET_STATUS_POS                                   (26UL)
#define PCR_INT_SET_STATUS_REG_GPIO10_PAD_HIGH_INT_SET_STATUS_MASK                                  (0x02000000UL) /**< -x - GPIO10 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO10_PAD_HIGH_INT_SET_STATUS_POS                                   (25UL)
#define PCR_INT_SET_STATUS_REG_GPIO9_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x01000000UL) /**< -x - GPIO9 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO9_PAD_HIGH_INT_SET_STATUS_POS                                    (24UL)
#define PCR_INT_SET_STATUS_REG_GPIO8_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00800000UL) /**< -x - GPIO8 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO8_PAD_HIGH_INT_SET_STATUS_POS                                    (23UL)
#define PCR_INT_SET_STATUS_REG_GPIO7_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00400000UL) /**< -x - GPIO7 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO7_PAD_HIGH_INT_SET_STATUS_POS                                    (22UL)
#define PCR_INT_SET_STATUS_REG_GPIO6_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00200000UL) /**< -x - GPIO6 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO6_PAD_HIGH_INT_SET_STATUS_POS                                    (21UL)
#define PCR_INT_SET_STATUS_REG_GPIO5_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00100000UL) /**< -x - GPIO5 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO5_PAD_HIGH_INT_SET_STATUS_POS                                    (20UL)
#define PCR_INT_SET_STATUS_REG_GPIO4_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00080000UL) /**< -x - GPIO4 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO4_PAD_HIGH_INT_SET_STATUS_POS                                    (19UL)
#define PCR_INT_SET_STATUS_REG_GPIO3_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00040000UL) /**< -x - GPIO3 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO3_PAD_HIGH_INT_SET_STATUS_POS                                    (18UL)
#define PCR_INT_SET_STATUS_REG_GPIO2_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00020000UL) /**< -x - GPIO2 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO2_PAD_HIGH_INT_SET_STATUS_POS                                    (17UL)
#define PCR_INT_SET_STATUS_REG_GPIO1_PAD_HIGH_INT_SET_STATUS_MASK                                   (0x00010000UL) /**< -x - GPIO1 pad going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_GPIO1_PAD_HIGH_INT_SET_STATUS_POS                                    (16UL)
#define PCR_INT_SET_STATUS_REG_VBUSP_MON_HIGH_INT_SET_STATUS_MASK                                   (0x00000400UL) /**< -x - VBUSP monitor going high interrupt set status. */
#define PCR_INT_SET_STATUS_REG_VBUSP_MON_HIGH_INT_SET_STATUS_POS                                    (10UL)
#define PCR_INT_SET_STATUS_REG_CLIF_RFLD_ACT_INT_SET_STATUS_MASK                                    (0x00000200UL) /**< -x - Clif RF field activity observed interrupt set status. */
#define PCR_INT_SET_STATUS_REG_CLIF_RFLD_ACT_INT_SET_STATUS_POS                                     (9UL)
#define PCR_INT_SET_STATUS_REG_VBUS_MON_LOW_INT_SET_STATUS_MASK                                     (0x00000100UL) /**< -x - VBUS monitor going low interrupt set status. */
#define PCR_INT_SET_STATUS_REG_VBUS_MON_LOW_INT_SET_STATUS_POS                                      (8UL)
#define PCR_INT_SET_STATUS_REG_VBUSP_MON_LOW_INT_SET_STATUS_MASK                                    (0x00000080UL) /**< -x - VBUSP monitor going low interrupt set status. */
#define PCR_INT_SET_STATUS_REG_VBUSP_MON_LOW_INT_SET_STATUS_POS                                     (7UL)
#define PCR_INT_SET_STATUS_REG_PVDD_CURLIM_ACT_SET_STATUS_MASK                                      (0x00000040UL) /**< -x - PVDD current limiter active interrupt set status.. */
#define PCR_INT_SET_STATUS_REG_PVDD_CURLIM_ACT_SET_STATUS_POS                                       (6UL)
#define PCR_INT_SET_STATUS_REG_TEMPERROR1_INT_SET_STATUS_MASK                                       (0x00000020UL) /**< -x - Temperature error 1 interrupt  set status. */
#define PCR_INT_SET_STATUS_REG_TEMPERROR1_INT_SET_STATUS_POS                                        (5UL)
#define PCR_INT_SET_STATUS_REG_TEMPERROR0_INT_SET_STATUS_MASK                                       (0x00000010UL) /**< -x - Temperature error 0 interrupt  set status. */
#define PCR_INT_SET_STATUS_REG_TEMPERROR0_INT_SET_STATUS_POS                                        (4UL)
#define PCR_INT_SET_STATUS_REG_SUSPEND_DEFAULTED_INT_SET_STATUS_MASK                                (0x00000008UL) /**< -x - Suspend defaulted interrupt set status. */
#define PCR_INT_SET_STATUS_REG_SUSPEND_DEFAULTED_INT_SET_STATUS_POS                                 (3UL)
#define PCR_INT_SET_STATUS_REG_SUSPEND_INT_SET_STATUS_MASK                                          (0x00000002UL) /**< -x - Suspend interrupt set status.. */
#define PCR_INT_SET_STATUS_REG_SUSPEND_INT_SET_STATUS_POS                                           (1UL)
#define PCR_INT_SET_STATUS_REG_GPIO_INT_SET_STATUS_MASK                                             (0x00000001UL) /**< -x - GPIO interrupt clear clear status. */
#define PCR_INT_SET_STATUS_REG_GPIO_INT_SET_STATUS_POS                                              (0UL)
#define PCR_INT_SET_STATUS_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_PCR_H_INCLUDED */
