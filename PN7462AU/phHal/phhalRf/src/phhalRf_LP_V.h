/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
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
 * Helper for Load Protocol settings for CLIF.  This file works in
 * close co-ordination with values stored in EEPROM.
 *
 * This file contains values that are variable and depend on
 * Contents of the Excel Sheets containing CLIF Settings.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-11-21 14:48:37 +0530 (Mon, 21 Nov 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18753 $ (v04.13.00)
 */

#ifndef PHHALRF_LP_V_H
#define PHHALRF_LP_V_H

/* Generated on 2016-11-18 by clifcsv_to_xml.py script version 2016.01.21_00 */

/**
 * Number of registers to be loaded for T_Tx.
 */
#define PHHAL_RF_LP_INT_T_TX_CFG_OFFSETS { \
     9, /* T_TX_GTM            */ \
     9, /* T_TX_A_P            */ \
    12, /* T_TX_A_106_P        */ \
    15, /* T_TX_A_212          */ \
    18, /* T_TX_A_424          */ \
    21, /* T_TX_A_848          */ \
    26, /* T_TX_F_P            */ \
    26, /* T_TX_F_212_P        */ \
    26, /* T_TX_F_424_P        */ \
    29, /* T_TX_ACT            */ \
    38, /* T_TX_A_106_ACT      */ \
    46, /* T_TX_F_212_ACT      */ \
    54, /* T_TX_F_424_ACT      */ \
    55, /* T_TX_B              */ \
    55, /* T_TX_B_106          */ \
    55, /* T_TX_B_212          */ \
    55, /* T_TX_B_424          */ \
    55  /* T_TX_B_848          */ \
}

/**
 * Register address offset to be loaded for T_Tx.
 */
#define PHHAL_RF_LP_INT_T_TX_CLIF_REGADD_OFFSETS { \
    /* Mode: T_TX_GTM                                           */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x43, /* CLIF_ANA_PBF_CONTROL_REG             (@0x4000410C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x37, /* CLIF_TX_CONTROL_REG                  (@0x400040DC) */ \
    0x48, /* CLIF_ANA_CLK_MAN_REG                 (@0x40004120) */ \
    /* Mode: T_TX_A_P                                           */ \
    /* Mode: T_TX_A_106_P                                       */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    /* Mode: T_TX_A_212                                         */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    /* Mode: T_TX_A_424                                         */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    /* Mode: T_TX_A_848                                         */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    /* Mode: T_TX_F_P                                           */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x48, /* CLIF_ANA_CLK_MAN_REG                 (@0x40004120) */ \
    /* Mode: T_TX_F_212_P                                       */ \
    /* Mode: T_TX_F_424_P                                       */ \
    /* Mode: T_TX_ACT                                           */ \
    0x43, /* CLIF_ANA_PBF_CONTROL_REG             (@0x4000410C) */ \
    0x37, /* CLIF_TX_CONTROL_REG                  (@0x400040DC) */ \
    0x48, /* CLIF_ANA_CLK_MAN_REG                 (@0x40004120) */ \
    /* Mode: T_TX_A_106_ACT                                     */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: T_TX_F_212_ACT                                     */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: T_TX_F_424_ACT                                     */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: T_TX_B                                             */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    /* Mode: T_TX_B_106                                         */ \
    /* Mode: T_TX_B_212                                         */ \
    /* Mode: T_TX_B_424                                         */ \
    /* Mode: T_TX_B_848                                         */ \
}

/** Internal logic for masking */
#define PHHAL_RF_LP_INT_T_TX_MASK {     \
    /* Mode: T_TX_GTM */ \
    0, 0, 1, 0, 0, 0, 0, 2, 0,     \
    /* Mode: T_TX_A_P */ \
        \
    /* Mode: T_TX_A_106_P */ \
    0, 0, 0,     \
    /* Mode: T_TX_A_212 */ \
    0, 0, 0,     \
    /* Mode: T_TX_A_424 */ \
    0, 0, 0,     \
    /* Mode: T_TX_A_848 */ \
    0, 0, 0,     \
    /* Mode: T_TX_F_P */ \
    0, 1, 0, 0, 0,     \
    /* Mode: T_TX_F_212_P */ \
        \
    /* Mode: T_TX_F_424_P */ \
        \
    /* Mode: T_TX_ACT */ \
    0, 0, 0,     \
    /* Mode: T_TX_A_106_ACT */ \
    0, 0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: T_TX_F_212_ACT */ \
    0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: T_TX_F_424_ACT */ \
    0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: T_TX_B */ \
    1,     \
    /* Mode: T_TX_B_106 */ \
        \
    /* Mode: T_TX_B_212 */ \
        \
    /* Mode: T_TX_B_424 */ \
        \
    /* Mode: T_TX_B_848 */ \
    \
}

/**
 * Number of registers to be loaded for T_Rx.
 */
#define PHHAL_RF_LP_INT_T_RX_CFG_OFFSETS { \
    11, /* T_RX_GTM            */ \
    12, /* T_RX_A              */ \
    20, /* T_RX_A_106          */ \
    28, /* T_RX_A_212          */ \
    36, /* T_RX_A_424          */ \
    44, /* T_RX_A_848          */ \
    49, /* T_RX_F              */ \
    51, /* T_RX_F_212          */ \
    53, /* T_RX_F_424          */ \
    55, /* T_RX_ACT            */ \
    56, /* T_RX_A_106_ACT      */ \
    59, /* T_RX_F_212_ACT      */ \
    62, /* T_RX_F_424_ACT      */ \
    64, /* T_RX_B              */ \
    66, /* T_RX_B_106          */ \
    68, /* T_RX_B_212          */ \
    70, /* T_RX_B_424          */ \
    72  /* T_RX_B_848          */ \
}

/**
 * Register address offset to be loaded for T_Rx.
 */
#define PHHAL_RF_LP_INT_T_RX_CLIF_REGADD_OFFSETS { \
    /* Mode: T_RX_GTM                                           */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x47, /* CLIF_ANA_AGC_REG                     (@0x4000411C) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x35, /* CLIF_AGC_INPUT_REG                   (@0x400040D4) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_A                                             */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: T_RX_A_106                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_A_212                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_A_424                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_A_848                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_F                                             */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_F_212                                         */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    /* Mode: T_RX_F_424                                         */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    /* Mode: T_RX_ACT                                           */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    /* Mode: T_RX_A_106_ACT                                     */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    /* Mode: T_RX_F_212_ACT                                     */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    /* Mode: T_RX_F_424_ACT                                     */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    /* Mode: T_RX_B                                             */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    /* Mode: T_RX_B_106                                         */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    /* Mode: T_RX_B_212                                         */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    /* Mode: T_RX_B_424                                         */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    /* Mode: T_RX_B_848                                         */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F  /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
}

/**
 * Number of registers to be loaded for I_Tx.
 */
#define PHHAL_RF_LP_INT_I_TX_CFG_OFFSETS { \
     1, /* I_TX                */ \
     2, /* I_TX_A              */ \
    11, /* I_TX_A_106          */ \
    19, /* I_TX_A_212          */ \
    27, /* I_TX_A_424          */ \
    35, /* I_TX_A_848          */ \
    35, /* I_TX_B              */ \
    43, /* I_TX_B_106          */ \
    51, /* I_TX_B_212          */ \
    59, /* I_TX_B_424          */ \
    67, /* I_TX_B_848          */ \
    68, /* I_TX_F              */ \
    72, /* I_TX_F_212          */ \
    76, /* I_TX_F_424          */ \
    76, /* I_TX_15693          */ \
    84, /* I_TX_15693_100      */ \
    92, /* I_TX_15693_10       */ \
    92, /* I_TX_EPCV2          */ \
    100, /* I_TX_EPCV2_TARI_9_44*/ \
    108, /* I_TX_EPCV2_TARI_18_88*/ \
    109, /* I_TX_ACT            */ \
    119, /* I_TX_ACT_106        */ \
    128, /* I_TX_ACT_212        */ \
    137, /* I_TX_ACT_424        */ \
    152  /* I_TX_BOOT           */ \
}

/**
 * Register address offset to be loaded for I_Tx.
 */
#define PHHAL_RF_LP_INT_I_TX_CLIF_REGADD_OFFSETS { \
    /* Mode: I_TX                                               */ \
    0x43, /* CLIF_ANA_PBF_CONTROL_REG             (@0x4000410C) */ \
    /* Mode: I_TX_A                                             */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    /* Mode: I_TX_A_106                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: I_TX_A_212                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    /* Mode: I_TX_A_424                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    /* Mode: I_TX_A_848                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    /* Mode: I_TX_B                                             */ \
    /* Mode: I_TX_B_106                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    /* Mode: I_TX_B_212                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    /* Mode: I_TX_B_424                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    /* Mode: I_TX_B_848                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    /* Mode: I_TX_F                                             */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    /* Mode: I_TX_F_212                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    /* Mode: I_TX_F_424                                         */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    /* Mode: I_TX_15693                                         */ \
    /* Mode: I_TX_15693_100                                     */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    /* Mode: I_TX_15693_10                                      */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    /* Mode: I_TX_EPCV2                                         */ \
    /* Mode: I_TX_EPCV2_TARI_9_44                               */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    0x11, /* CLIF_TX_SYMBOL1_DEF_REG              (@0x40004044) */ \
    /* Mode: I_TX_EPCV2_TARI_18_88                              */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    0x11, /* CLIF_TX_SYMBOL1_DEF_REG              (@0x40004044) */ \
    /* Mode: I_TX_ACT                                           */ \
    0x43, /* CLIF_ANA_PBF_CONTROL_REG             (@0x4000410C) */ \
    /* Mode: I_TX_ACT_106                                       */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: I_TX_ACT_212                                       */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: I_TX_ACT_424                                       */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    /* Mode: I_TX_BOOT                                          */ \
    0x03, /* CLIF_TRANSCEIVE_CONTROL_REG          (@0x4000400C) */ \
    0x43, /* CLIF_ANA_PBF_CONTROL_REG             (@0x4000410C) */ \
    0x42, /* CLIF_ANA_TX_AMPLITUDE_REG            (@0x40004108) */ \
    0x41, /* CLIF_ANA_TX_CLK_CONTROL_REG          (@0x40004104) */ \
    0x16, /* CLIF_TX_UNDERSHOOT_CONFIG_REG        (@0x40004058) */ \
    0x15, /* CLIF_TX_OVERSHOOT_CONFIG_REG         (@0x40004054) */ \
    0x4A, /* CLIF_ANA_TX_SHAPE_CONTROL_REG        (@0x40004128) */ \
    0x0D, /* CLIF_TX_DATA_MOD_REG                 (@0x40004034) */ \
    0x14, /* CLIF_TX_SYMBOL23_MOD_REG             (@0x40004050) */ \
    0x0F, /* CLIF_TX_SYMBOL_CONFIG_REG            (@0x4000403C) */ \
    0x48, /* CLIF_ANA_CLK_MAN_REG                 (@0x40004120) */ \
    0x13, /* CLIF_TX_SYMBOL01_MOD_REG             (@0x4000404C) */ \
    0x10, /* CLIF_TX_SYMBOL0_DEF_REG              (@0x40004040) */ \
    0x11, /* CLIF_TX_SYMBOL1_DEF_REG              (@0x40004044) */ \
    0x3F  /* CLIF_TEST_CONTROL_REG                (@0x400040FC) */ \
}

/** Internal logic for masking */
#define PHHAL_RF_LP_INT_I_TX_MASK {     \
    /* Mode: I_TX */ \
    0,     \
    /* Mode: I_TX_A */ \
    0,     \
    /* Mode: I_TX_A_106 */ \
    1, 0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_A_212 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_A_424 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_A_848 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_B */ \
        \
    /* Mode: I_TX_B_106 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_B_212 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_B_424 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_B_848 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_F */ \
    0,     \
    /* Mode: I_TX_F_212 */ \
    1, 0, 0, 0,     \
    /* Mode: I_TX_F_424 */ \
    1, 0, 0, 0,     \
    /* Mode: I_TX_15693 */ \
        \
    /* Mode: I_TX_15693_100 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_15693_10 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_EPCV2 */ \
        \
    /* Mode: I_TX_EPCV2_TARI_9_44 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_EPCV2_TARI_18_88 */ \
    1, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_ACT */ \
    0,     \
    /* Mode: I_TX_ACT_106 */ \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_ACT_212 */ \
    0, 0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_ACT_424 */ \
    0, 0, 0, 0, 0, 0, 0, 0, 0,     \
    /* Mode: I_TX_BOOT */ \
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
}

/**
 * Number of registers to be loaded for I_Rx.
 */
#define PHHAL_RF_LP_INT_I_RX_CFG_OFFSETS { \
     2, /* I_RX_PASS           */ \
     2, /* I_RX_A_P            */ \
     7, /* I_RX_A_106_P        */ \
    12, /* I_RX_A_212          */ \
    17, /* I_RX_A_424          */ \
    22, /* I_RX_A_848          */ \
    25, /* I_RX_B              */ \
    27, /* I_RX_B_106          */ \
    29, /* I_RX_B_212          */ \
    31, /* I_RX_B_424          */ \
    33, /* I_RX_B_848          */ \
    36, /* I_RX_F_P            */ \
    38, /* I_RX_F_212_P        */ \
    40, /* I_RX_F_424_P        */ \
    43, /* I_RX_15693          */ \
    45, /* I_RX_15693_26       */ \
    47, /* I_RX_15693_53       */ \
    49, /* I_RX_EPCV2          */ \
    51, /* I_RX_EPCV2_SC424_2MP*/ \
    53, /* I_RX_EPCV2_SC424_4MP*/ \
    55, /* I_RX_EPCV2_SC848_2MP*/ \
    57, /* I_RX_EPCV2_SC848_4MP*/ \
    63, /* I_RX_ACT            */ \
    68, /* I_RX_ACT_106        */ \
    75, /* I_RX_ACT_212        */ \
    82, /* I_RX_ACT_424        */ \
    96  /* I_RX_BOOT           */ \
}

/**
 * Register address offset to be loaded for I_Rx.
 */
#define PHHAL_RF_LP_INT_I_RX_CLIF_REGADD_OFFSETS { \
    /* Mode: I_RX_PASS                                          */ \
    0x47, /* CLIF_ANA_AGC_REG                     (@0x4000411C) */ \
    0x35, /* CLIF_AGC_INPUT_REG                   (@0x400040D4) */ \
    /* Mode: I_RX_A_P                                           */ \
    /* Mode: I_RX_A_106_P                                       */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_A_212                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_A_424                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_A_848                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_B                                             */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_B_106                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_B_212                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_B_424                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_B_848                                         */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_F_P                                           */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_F_212_P                                       */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_F_424_P                                       */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_15693                                         */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_15693_26                                      */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_15693_53                                      */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_EPCV2                                         */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    /* Mode: I_RX_EPCV2_SC424_2MP                               */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_EPCV2_SC424_4MP                               */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_EPCV2_SC848_2MP                               */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_EPCV2_SC848_4MP                               */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    /* Mode: I_RX_ACT                                           */ \
    0x47, /* CLIF_ANA_AGC_REG                     (@0x4000411C) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x35, /* CLIF_AGC_INPUT_REG                   (@0x400040D4) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x85, /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
    0x48, /* CLIF_ANA_CLK_MAN_REG                 (@0x40004120) */ \
    /* Mode: I_RX_ACT_106                                       */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_ACT_212                                       */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_ACT_424                                       */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    /* Mode: I_RX_BOOT                                          */ \
    0x44, /* CLIF_ANA_RX_REG                      (@0x40004110) */ \
    0x47, /* CLIF_ANA_AGC_REG                     (@0x4000411C) */ \
    0x2D, /* CLIF_SIGPRO_RM_CONFIG1_REG           (@0x400040B4) */ \
    0x34, /* CLIF_AGC_CONFIG1_REG                 (@0x400040D0) */ \
    0x33, /* CLIF_AGC_CONFIG0_REG                 (@0x400040CC) */ \
    0x35, /* CLIF_AGC_INPUT_REG                   (@0x400040D4) */ \
    0x45, /* CLIF_ANA_CM_CONFIG_REG               (@0x40004114) */ \
    0x7F, /* CLIF_ANA_TEST_REG                    (@0x400041FC) */ \
    0x40, /* CLIF_ANA_NFCLD_REG                   (@0x40004100) */ \
    0x2E, /* CLIF_SIGPRO_CM_CONFIG_REG            (@0x400040B8) */ \
    0x30, /* CLIF_SIGPRO_ADCBCM_THRESHOLD_REG     (@0x400040C0) */ \
    0x2F, /* CLIF_SIGPRO_ADCBCM_CONFIG_REG        (@0x400040BC) */ \
    0x17, /* CLIF_RX_CONFIG_REG                   (@0x4000405C) */ \
    0x85  /* CLIF_BBA_CONTROL_REG                 (@0x40004214) */ \
}


#endif /* PHHALRF_LP_V_H */
