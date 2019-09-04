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
 * This file contains constants that shall never change.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-02-01 16:55:01 +0530 (Mon, 01 Feb 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 17658 $ (v04.13.00)
 */

#ifndef PHHALRF_LP_INT_H
#define PHHALRF_LP_INT_H

/* Generated on 2016-02-01 by clifcsv_to_xml.py script version 2016.01.21_00 */

/* =========================================================================== */
/* CLIF Target Mode - Transmit                                                 */
/* =========================================================================== */
/* PRQA S 0779 ++ */
/** Configuration offsets for CLIF Target Mode - Transmit.
 * @{ */
typedef enum phhalRf_LP_T_Tx {
    E_PHHAL_RF_LP_T_TX_GTM         =    (0),
    E_PHHAL_RF_LP_T_TX_A_P         =    (1),
    E_PHHAL_RF_LP_T_TX_A_106_P     =    (2),
    E_PHHAL_RF_LP_T_TX_A_212       =    (3),
    E_PHHAL_RF_LP_T_TX_A_424       =    (4),
    E_PHHAL_RF_LP_T_TX_A_848       =    (5),
    E_PHHAL_RF_LP_T_TX_F_P         =    (6),
    E_PHHAL_RF_LP_T_TX_F_212_P     =    (7),
    E_PHHAL_RF_LP_T_TX_F_424_P     =    (8),
    E_PHHAL_RF_LP_T_TX_ACT         =    (9),
    E_PHHAL_RF_LP_T_TX_A_106_ACT   =   (10),
    E_PHHAL_RF_LP_T_TX_F_212_ACT   =   (11),
    E_PHHAL_RF_LP_T_TX_F_424_ACT   =   (12),
    E_PHHAL_RF_LP_T_TX_B           =   (13),
    E_PHHAL_RF_LP_T_TX_B_106       =   (14),
    E_PHHAL_RF_LP_T_TX_B_212       =   (15),
    E_PHHAL_RF_LP_T_TX_B_424       =   (16),
    E_PHHAL_RF_LP_T_TX_B_848       =   (17),
    E_PHHAL_RF_LP_T_TX_KEEP_ASIS   =   (18),
    E_PHHAL_RF_LP_T_TX_UNKNOWN     =   (19),
    E_PHHAL_RF_LP_T_TX_MAX         =   (20)
} phhalRf_LP_T_Tx_t;
/** @} */
/* PRQA S 0779 -- */

/* =========================================================================== */
/* CLIF Target Mode - Receive                                                  */
/* =========================================================================== */
/* PRQA S 0779 ++ */
/** Configuration offsets for CLIF Target Mode - Receive.
 * @{ */
typedef enum phhalRf_LP_T_Rx {
    E_PHHAL_RF_LP_T_RX_GTM         =    (0),
    E_PHHAL_RF_LP_T_RX_A           =    (1),
    E_PHHAL_RF_LP_T_RX_A_106       =    (2),
    E_PHHAL_RF_LP_T_RX_A_212       =    (3),
    E_PHHAL_RF_LP_T_RX_A_424       =    (4),
    E_PHHAL_RF_LP_T_RX_A_848       =    (5),
    E_PHHAL_RF_LP_T_RX_F           =    (6),
    E_PHHAL_RF_LP_T_RX_F_212       =    (7),
    E_PHHAL_RF_LP_T_RX_F_424       =    (8),
    E_PHHAL_RF_LP_T_RX_ACT         =    (9),
    E_PHHAL_RF_LP_T_RX_A_106_ACT   =   (10),
    E_PHHAL_RF_LP_T_RX_F_212_ACT   =   (11),
    E_PHHAL_RF_LP_T_RX_F_424_ACT   =   (12),
    E_PHHAL_RF_LP_T_RX_B           =   (13),
    E_PHHAL_RF_LP_T_RX_B_106       =   (14),
    E_PHHAL_RF_LP_T_RX_B_212       =   (15),
    E_PHHAL_RF_LP_T_RX_B_424       =   (16),
    E_PHHAL_RF_LP_T_RX_B_848       =   (17),
    E_PHHAL_RF_LP_T_RX_KEEP_ASIS   =   (18),
    E_PHHAL_RF_LP_T_RX_UNKNOWN     =   (19),
    E_PHHAL_RF_LP_T_RX_MAX         =   (20)
} phhalRf_LP_T_Rx_t;
/** @} */
/* PRQA S 0779 -- */

/* =========================================================================== */
/* CLIF Initiator Mode - Transmit                                              */
/* =========================================================================== */
/* PRQA S 0779 ++ */
/** Configuration offsets for CLIF Initiator Mode - Transmit.
 * @{ */
typedef enum phhalRf_LP_I_Tx {
    E_PHHAL_RF_LP_I_TX             =    (0),
    E_PHHAL_RF_LP_I_TX_A           =    (1),
    E_PHHAL_RF_LP_I_TX_A_106       =    (2),
    E_PHHAL_RF_LP_I_TX_A_212       =    (3),
    E_PHHAL_RF_LP_I_TX_A_424       =    (4),
    E_PHHAL_RF_LP_I_TX_A_848       =    (5),
    E_PHHAL_RF_LP_I_TX_B           =    (6),
    E_PHHAL_RF_LP_I_TX_B_106       =    (7),
    E_PHHAL_RF_LP_I_TX_B_212       =    (8),
    E_PHHAL_RF_LP_I_TX_B_424       =    (9),
    E_PHHAL_RF_LP_I_TX_B_848       =   (10),
    E_PHHAL_RF_LP_I_TX_F           =   (11),
    E_PHHAL_RF_LP_I_TX_F_212       =   (12),
    E_PHHAL_RF_LP_I_TX_F_424       =   (13),
    E_PHHAL_RF_LP_I_TX_15693       =   (14),
    E_PHHAL_RF_LP_I_TX_15693_100   =   (15),
    E_PHHAL_RF_LP_I_TX_15693_10    =   (16),
    E_PHHAL_RF_LP_I_TX_EPCV2       =   (17),
    E_PHHAL_RF_LP_I_TX_EPCV2_TARI_9_44 =   (18),
    E_PHHAL_RF_LP_I_TX_EPCV2_TARI_18_88 =   (19),
    E_PHHAL_RF_LP_I_TX_ACT         =   (20),
    E_PHHAL_RF_LP_I_TX_ACT_106     =   (21),
    E_PHHAL_RF_LP_I_TX_ACT_212     =   (22),
    E_PHHAL_RF_LP_I_TX_ACT_424     =   (23),
    E_PHHAL_RF_LP_I_TX_BOOT        =   (24),
    E_PHHAL_RF_LP_I_TX_KEEP_ASIS   =   (25),
    E_PHHAL_RF_LP_I_TX_UNKNOWN     =   (26),
    E_PHHAL_RF_LP_I_TX_MAX         =   (27)
} phhalRf_LP_I_Tx_t;
/** @} */
/* PRQA S 0779 -- */

/* =========================================================================== */
/* CLIF Initiator Mode - Receive                                               */
/* =========================================================================== */
/* PRQA S 0779 ++ */
/** Configuration offsets for CLIF Initiator Mode - Receive.
 * @{ */
typedef enum phhalRf_LP_I_Rx {
    E_PHHAL_RF_LP_I_RX_PASS        =    (0),
    E_PHHAL_RF_LP_I_RX_A_P         =    (1),
    E_PHHAL_RF_LP_I_RX_A_106_P     =    (2),
    E_PHHAL_RF_LP_I_RX_A_212       =    (3),
    E_PHHAL_RF_LP_I_RX_A_424       =    (4),
    E_PHHAL_RF_LP_I_RX_A_848       =    (5),
    E_PHHAL_RF_LP_I_RX_B           =    (6),
    E_PHHAL_RF_LP_I_RX_B_106       =    (7),
    E_PHHAL_RF_LP_I_RX_B_212       =    (8),
    E_PHHAL_RF_LP_I_RX_B_424       =    (9),
    E_PHHAL_RF_LP_I_RX_B_848       =   (10),
    E_PHHAL_RF_LP_I_RX_F_P         =   (11),
    E_PHHAL_RF_LP_I_RX_F_212_P     =   (12),
    E_PHHAL_RF_LP_I_RX_F_424_P     =   (13),
    E_PHHAL_RF_LP_I_RX_15693       =   (14),
    E_PHHAL_RF_LP_I_RX_15693_26    =   (15),
    E_PHHAL_RF_LP_I_RX_15693_53    =   (16),
    E_PHHAL_RF_LP_I_RX_EPCV2       =   (17),
    E_PHHAL_RF_LP_I_RX_EPCV2_SC424_2MP =   (18),
    E_PHHAL_RF_LP_I_RX_EPCV2_SC424_4MP =   (19),
    E_PHHAL_RF_LP_I_RX_EPCV2_SC848_2MP =   (20),
    E_PHHAL_RF_LP_I_RX_EPCV2_SC848_4MP =   (21),
    E_PHHAL_RF_LP_I_RX_ACT         =   (22),
    E_PHHAL_RF_LP_I_RX_ACT_106     =   (23),
    E_PHHAL_RF_LP_I_RX_ACT_212     =   (24),
    E_PHHAL_RF_LP_I_RX_ACT_424     =   (25),
    E_PHHAL_RF_LP_I_RX_BOOT        =   (26),
    E_PHHAL_RF_LP_I_RX_KEEP_ASIS   =   (27),
    E_PHHAL_RF_LP_I_RX_UNKNOWN     =   (28),
    E_PHHAL_RF_LP_I_RX_MAX         =   (29)
} phhalRf_LP_I_Rx_t;
/** @} */
/* PRQA S 0779 -- */


#endif /* PHHALRF_LP_INT_H */
