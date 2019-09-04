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
 * phExDoorAccess_Cfg.h : phExDoorAccess application configuration file for specifying EMVCO/NFC compliance, Task and its
 *                  stack etc.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */

#ifndef PHEXDOORACCESS_CFG_H
#define PHEXDOORACCESS_CFG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Log.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#ifdef PHFL_HALAPI_WITH_RTOS

#define PHFL_SYS_TASK_Q_LEN             2   /* Max Systen task requests in queue. */


#define PHFL_EVENT_CLIF_Q_LEN           2   /* Max Clif Task requests in Queue. */
//#define PHFL_EVENT_CT_Q_LEN             2   /* Max CT Task requests in Queue. */

/* Stack Guard to facilitate minor changes in phExDoorAccess example application. */
#define PH_EXDOORACCESS_CFG_RTOS_STACK_GUARD                25
#endif
/* Minimum number of words (4 bytes) is required to print the status logs on console screen for different tasks. */
#if PHFL_LOG_ENABLE
#    define PH_EXDOORACCESS_CFG_SYSTEM_LOG_STACK               105
#    define PH_EXDOORACCESS_CFG_CLIF_LOG_STACK                 0
//#    define PH_EXDOORACCESS_CFG_CT_LOG_STACK                   75
#else
#    define PH_EXDOORACCESS_CFG_SYSTEM_LOG_STACK               0
#    define PH_EXDOORACCESS_CFG_CLIF_LOG_STACK                 0
#    define PH_EXDOORACCESS_CFG_CT_LOG_STACK                   0
#endif /* PHFL_LOG_ENABLE */
#ifdef PHFL_HALAPI_WITH_RTOS
/**
 *  Note actual allocation is x4 bytes
 */

#define PH_EXDOORACCESS_CFG_RTOS_CLIF_TASK_STACK_SIZE    (280 + PH_EXDOORACCESS_CFG_RTOS_STACK_GUARD + PH_EXDOORACCESS_CFG_CLIF_LOG_STACK+20)
#define PH_EXDOORACCESS_CFG_RTOS_CLIF_TASK_PRIORITY      5  /* Highest  */

/**
 *  Note actual allocation is x4 bytes
 */

#define PH_EXDOORACCESS_CFG_RTOS_SYS_TASK_STACK_SIZE    (75 + PH_EXDOORACCESS_CFG_RTOS_STACK_GUARD + PH_EXDOORACCESS_CFG_SYSTEM_LOG_STACK)
#define PH_EXDOORACCESS_CFG_RTOS_SYS_TASK_PRIORITY      4  /* Low */
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
 * PH_EXDOORACCESS_CFG_SWD_DNLD_PIN is the GPIO Number that is used to sense such work around.
 * #PH_EXDOORACCESS_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE is the value expected to halt the boot.
 *
 */
#define PH_EXDOORACCESS_CFG_SWD_DNLD_PIN                    8

/* The value which will /NOT/ let boot up continue. But /HALT/ the system
 * so the SWD based download can run. @see PH_EXDOORACCESS_CFG_SWD_DNLD_PIN
 *
 * Do not define this value if you don't want wait on GPIO before entering
 * standby.
 *
 * Customer board v1.0 => 1
 * Customer board v1.1 => 0
 * Customer board v2.0 => 1
 * Customer board v2.1 => 1
 */
#define PH_EXDOORACCESS_CFG_SWD_DNLD_PIN_ASSERT_HALT_VALUE  1

#define PH_EXDOORACCESS_CFG_USETXLDO_EXTERNAL               0x0

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */


#endif /* PHEXDOORACCESS_CFG_H */
