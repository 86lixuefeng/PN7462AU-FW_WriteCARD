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
 * phExVCom_Cfg.h : phExVCom application configuration file for specifying Task and its
 *                  stack etc.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-10-20 10:18:10 +0530 (Thu, 20 Oct 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18736 $ (v05.19.00)
 */

#ifndef PHEXVCOM_CFG_H
#define PHEXVCOM_CFG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Log.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#ifdef PHFL_HALAPI_WITH_RTOS

#define PHFL_SYS_TASK_Q_LEN             2   /* Max System task requests in queue. */
#define PHFL_EVENT_CLIF_Q_LEN           4   /* Max Clif Task requests in Queue. */


/* Stack Guard to facilitate minor changes in phExVCom example application. */
#define PH_EXVCOM_CFG_RTOS_STACK_GUARD                25
#endif
/* Minimum number of words (4 bytes/word) required to print the status logs on console screen for different tasks. */
#if PHFL_LOG_ENABLE
#    define PH_EXVCOM_CFG_SYSTEM_LOG_STACK               105
#    define PH_EXVCOM_CFG_CLIF_LOG_STACK                 0
#else
#    define PH_EXVCOM_CFG_SYSTEM_LOG_STACK               0
#    define PH_EXVCOM_CFG_CLIF_LOG_STACK                 0
#endif /* PHFL_LOG_ENABLE */
#ifdef PHFL_HALAPI_WITH_RTOS
/**
 *  Note actual allocation is x4 bytes
 */

#define PH_EXVCOM_CFG_RTOS_CLIF_TASK_STACK_SIZE    (370 + PH_EXVCOM_CFG_RTOS_STACK_GUARD + PH_EXVCOM_CFG_CLIF_LOG_STACK+20)
#define PH_EXVCOM_CFG_RTOS_CLIF_TASK_PRIORITY      4   /* Medium  */

/**
 *  Note actual allocation is x4 bytes
 */

#define PH_EXVCOM_CFG_RTOS_SYS_TASK_STACK_SIZE    (75 + PH_EXVCOM_CFG_RTOS_STACK_GUARD + PH_EXVCOM_CFG_SYSTEM_LOG_STACK)
#define PH_EXVCOM_CFG_RTOS_SYS_TASK_PRIORITY      3  /* Low */
#endif

/** Enable/Assist SWD Based download _WITH_ Standby
 *
 * When the IC goes to standby/sleep mode/low power mode, SWD Connection is lost.
 *
 * If the IC enters standby, say, immediately after boot up, SWD download won't work.
 * under such circumstances, the user is expected to pull high/low (based on the board
 * configuration) a compile time configurable GPIO.
 *
 * During FW boot up, if a compile time configured /ASSERTED/ value is sensed on the
 * the GPIO, FW Boot up is halted, and thus the Connection over SWD Lines is retained.
 *
 * PH_EXVCOM_CFG_SWD_DNLD_PIN is the GPIO Number that is used to sense such work around.
 * #PH_EXVCOM_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE is the value expected to halt the boot.
 *
 */
#define PH_EXVCOM_CFG_SWD_DNLD_PIN                    8

/* The value which will /NOT/ let boot up continue. But /HALT/ the system
 * so the SWD based download can run. @see PH_EXVCOM_CFG_SWD_DNLD_PIN
 *
 * Do not define this value if you don't want wait on GPIO before entering
 * standby.
 *
 * Customer board v1.0 => 1
 * Customer board v1.1 => 0
 * Customer board v2.0 => 1
 * Customer board v2.1 => 1
 */
#define PH_EXVCOM_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE  1

#define PH_EXVCOM_USETXLDO_EXTERNAL               0x0
#define PH_EXVCOM_TXLDO_MONITOR_ENABLE            0x1
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */


#endif /* PHEXVCOM_CFG_H */
