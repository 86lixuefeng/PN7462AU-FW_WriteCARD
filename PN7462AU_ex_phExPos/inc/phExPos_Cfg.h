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
 * phExPos_Cfg.h : phExPos application configuration file for specifying EMVCO/NFC compliance, Task and its
 *                  stack etc.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */

#ifndef PHEXPOS_CFG_H
#define PHEXPOS_CFG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Log.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PHFL_SYS_TASK_Q_LEN             2   /* Max Systen task requests in queue. */

#define PHFL_EVENT_CLIF_Q_LEN           2   /* Max Clif Task requests in Queue. */
#define PHFL_EVENT_CT_Q_LEN             2   /* Max CT Task requests in Queue. PT_SC3164 */

/* Stack Guard to facilitate minor changes in phExPos example application. */
#define PH_EXPOS_CFG_RTOS_STACK_GUARD                25

/* Minimum number of words (4 bytes) is required to print the status logs on console screen for different tasks. */
#if PHFL_LOG_ENABLE
#    define PH_EXPOS_CFG_SYSTEM_LOG_STACK               105
#    define PH_EXPOS_CFG_CLIF_LOG_STACK                 0
#    define PH_EXPOS_CFG_CT_LOG_STACK                   75
#else
#    define PH_EXPOS_CFG_SYSTEM_LOG_STACK               0
#    define PH_EXPOS_CFG_CLIF_LOG_STACK                 0
#    define PH_EXPOS_CFG_CT_LOG_STACK                   0
#endif /* PHFL_LOG_ENABLE */

/**
 *  Note actual allocation is x4 bytes
 */
#define PH_EXPOS_CFG_RTOS_CLIF_TASK_STACK_SIZE    (320 + PH_EXPOS_CFG_RTOS_STACK_GUARD + PH_EXPOS_CFG_CLIF_LOG_STACK+20)
#define PH_EXPOS_CFG_RTOS_CLIF_TASK_PRIORITY      4   /* Highest  */

/**
 *  Note actual allocation is x4 bytes
 */
#define PH_EXPOS_CFG_RTOS_CT_TASK_STACK_SIZE      (200 + PH_EXPOS_CFG_RTOS_STACK_GUARD + PH_EXPOS_CFG_CT_LOG_STACK)
#define PH_EXPOS_CFG_RTOS_CT_TASK_PRIORITY        5    /* Medium  */

/**
 *  Note actual allocation is x4 bytes
 */
#define PH_EXPOS_CFG_RTOS_SYS_TASK_STACK_SIZE    (75 + PH_EXPOS_CFG_RTOS_STACK_GUARD + PH_EXPOS_CFG_SYSTEM_LOG_STACK)
#define PH_EXPOS_CFG_RTOS_SYS_TASK_PRIORITY      3  /* Low */

#define PH_EXPOS_CFG_SWD_DNLD_PIN                    8

#define PH_EXPOS_CFG_USETXLDO_EXTERNAL               0x0

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */


#endif /* PHEXPOS_CFG_H */
