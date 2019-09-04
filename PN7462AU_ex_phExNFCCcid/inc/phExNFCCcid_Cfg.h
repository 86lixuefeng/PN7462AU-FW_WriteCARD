/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * phExNFCCcid_Cfg.h : phExCcid application configuration file for specifying EMVCO/NFC compliance, Task and its
 *                  stack etc.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

#ifndef PHEXNFCCCID_CFG_H
#define PHEXNFCCCID_CFG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Log.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PHFL_LLCP_SNEP_ENABLE

#define PHFL_SYS_TASK_Q_LEN             2   /* Max Systen task requests in queue. */

#define PHFL_EVENT_CLIF_Q_LEN           2   /* Max Clif Task requests in Queue. */

/* Stack Guard to facilitate minor changes in phExCcid example application. */
#define PH_EXNFCCCID_CFG_RTOS_STACK_GUARD               0 //25

/* Minimum number of words (4 bytes) is required to print the status logs on console screen for different tasks. */
#if PHFL_LOG_ENABLE
#    define PH_EXNFCCCID_CFG_SYSTEM_LOG_STACK               105
#    define PH_EXNFCCCID_CFG_CLIF_LOG_STACK                 0
#    define PH_EXNFCCCID_CFG_CT_LOG_STACK                   75
#else
#    define PH_EXNFCCCID_CFG_SYSTEM_LOG_STACK               0
#    define PH_EXNFCCCID_CFG_CLIF_LOG_STACK                 0
#    define PH_EXNFCCCID_CFG_CT_LOG_STACK                   0
#endif /* PHFL_LOG_ENABLE */

/**
 *  Note actual allocation is x4 bytes
 */
#define PH_EXNFCCCID_CFG_RTOS_CLIF_TASK_STACK_SIZE    (340 + PH_EXNFCCCID_CFG_RTOS_STACK_GUARD + PH_EXNFCCCID_CFG_CLIF_LOG_STACK)
#define PH_EXNFCCCID_CFG_RTOS_CLIF_TASK_PRIORITY      5   /* Highest  */

#define PH_EXNFCCCID_CFG_RTOS_SYS_TASK_STACK_SIZE    (75 + PH_EXNFCCCID_CFG_RTOS_STACK_GUARD + PH_EXNFCCCID_CFG_SYSTEM_LOG_STACK)
#define PH_EXNFCCCID_CFG_RTOS_SYS_TASK_PRIORITY      4  /* Low */

#define PH_EXNFCCCID_CFG_SWD_DNLD_PIN                8
#define PH_EXNFCCCID_USETXLDO_EXTERNAL               0x0
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */


#endif /* PHEXNFCCCID_CFG_H */
