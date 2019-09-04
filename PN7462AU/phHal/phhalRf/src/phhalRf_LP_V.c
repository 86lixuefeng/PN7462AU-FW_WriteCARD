/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * @todo Constant values needed for LoadProtocol
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-01-09 15:23:38 +0530 (Sat, 09 Jan 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 17261 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"
#include "phhalRf_LP_V.h"
#include "phhalRf.h"
#include "PN7462AU/PN7462AU_clif.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

const uint8_t gkphhalRf_InitiatorTxNumEntries[] = PHHAL_RF_LP_INT_I_TX_CFG_OFFSETS;

const uint8_t gkphhalRf_InitiatorTxRegAddr[] = PHHAL_RF_LP_INT_I_TX_CLIF_REGADD_OFFSETS;




const uint8_t gkphhalRf_InitTxRegMaskIndex[] = PHHAL_RF_LP_INT_I_TX_MASK;


const uint32_t gkphhalRf_InitTxRegMaskValue[] = {
    (~( CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_TO_MAX_RM_MASK    |
        CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_AMPLITUDE_RM_MASK |
        CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_CW_RM_MASK |
        CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_MOD_RM_MASK))
};


const uint8_t gkphhalRf_InitiatorRxNumEntries[] = PHHAL_RF_LP_INT_I_RX_CFG_OFFSETS;

const uint8_t gkphhalRf_InitiatorRxRegAddr[] = PHHAL_RF_LP_INT_I_RX_CLIF_REGADD_OFFSETS;


const uint8_t gkphhalRf_TargetTxNumEntries[] = PHHAL_RF_LP_INT_T_TX_CFG_OFFSETS;

const uint8_t gkphhalRf_TargetTxRegAddr[] = PHHAL_RF_LP_INT_T_TX_CLIF_REGADD_OFFSETS;

const uint8_t gkphhalRf_TargetRxNumEntries[] = PHHAL_RF_LP_INT_T_RX_CFG_OFFSETS;

const uint8_t gkphhalRf_TargetRxRegAddr[] = PHHAL_RF_LP_INT_T_RX_CLIF_REGADD_OFFSETS;

/* #ifdef NXPBUILD__PHHAL_RF_APC */
/* Masking is been only enabled for APC. */
const uint8_t gkphhalRf_TarTxRegMaskIndex[] = PHHAL_RF_LP_INT_T_TX_MASK;

/**
 * Adding the mask for APC of  CLIF_ANA_TX_AMPLITUDE_REG and CLIF_TX_CONTROL_REG
 */
const uint32_t gkphhalRf_TarTxRegMaskValue[] = {
    (~( CLIF_ANA_TX_AMPLITUDE_REG_TX_RESIDUAL_CARRIER_MASK |
        CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_AMPLITUDE_ALM_CM_MASK)),
        (~(CLIF_TX_CONTROL_REG_TX_ALM_TYPE_SELECT_MASK))
};

/* #endif  NXPBUILD__PHHAL_RF_APC */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
