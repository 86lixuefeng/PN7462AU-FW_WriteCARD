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
#ifndef PN7462AU_IP_TIMERS_H_INCLUDED
#define PN7462AU_IP_TIMERS_H_INCLUDED

#include "PN7462AU/PN7462AU_timers_rorw.h"



/**
* \name Control of Timer 0
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER0_CONTROL_REG                                                                   (0x4001C000UL) /**< rw-*/
#define TIMERS_TIMER0_CONTROL_REG_TIMER0_MODE_MASK                                                  (0x00000001UL) /**< rw - 0 - single shot 1 - free-running. */
#define TIMERS_TIMER0_CONTROL_REG_TIMER0_MODE_POS                                                   (0UL)
#define TIMERS_TIMER0_CONTROL_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timeout value of Timer0
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER0_TIMEOUT_REG                                                                   (0x4001C004UL) /**< rw-*/
#define TIMERS_TIMER0_TIMEOUT_REG_TIMER0_TIMEOUT_MASK                                               (0x00000FFFUL) /**< rw - Initial count value of Timer0. If set to 0, this feature is disabled.. */
#define TIMERS_TIMER0_TIMEOUT_REG_TIMER0_TIMEOUT_POS                                                (0UL)
#define TIMERS_TIMER0_TIMEOUT_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Current count value of Timer0
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_TIMER0_COUNT_REG                                                                     (0x4001C008UL) /**< r--*/
#define TIMERS_TIMER0_COUNT_REG_TIMER0_COUNT_MASK                                                   (0x00000FFFUL) /**< r- - Current count value of Timer0.. */
#define TIMERS_TIMER0_COUNT_REG_TIMER0_COUNT_POS                                                    (0UL)
#define TIMERS_TIMER0_COUNT_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Control of Timer 1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER1_CONTROL_REG                                                                   (0x4001C00CUL) /**< rw-*/
#define TIMERS_TIMER1_CONTROL_REG_ENABLE_TIMER0_TRIGGER_MASK                                        (0x00000002UL) /**< rw - 1- Timer1 will start counting automatically when Timer0 reaches its terminal count (assuming that field TIMER1_TIMEOUT is non-zero in register TIMERS_TIMER1_TIMEOUT). */
#define TIMERS_TIMER1_CONTROL_REG_ENABLE_TIMER0_TRIGGER_POS                                         (1UL)
#define TIMERS_TIMER1_CONTROL_REG_TIMER1_MODE_MASK                                                  (0x00000001UL) /**< rw - 0 - single shot 1 - free-running. */
#define TIMERS_TIMER1_CONTROL_REG_TIMER1_MODE_POS                                                   (0UL)
#define TIMERS_TIMER1_CONTROL_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timeout value of Timer1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER1_TIMEOUT_REG                                                                   (0x4001C010UL) /**< rw-*/
#define TIMERS_TIMER1_TIMEOUT_REG_TIMER1_TIMEOUT_MASK                                               (0x00000FFFUL) /**< rw - Initial count value of Timer1. If set to 0, this feature is disabled.. */
#define TIMERS_TIMER1_TIMEOUT_REG_TIMER1_TIMEOUT_POS                                                (0UL)
#define TIMERS_TIMER1_TIMEOUT_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Current count value of Timer1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_TIMER1_COUNT_REG                                                                     (0x4001C014UL) /**< r--*/
#define TIMERS_TIMER1_COUNT_REG_TIMER1_COUNT_MASK                                                   (0x00000FFFUL) /**< r- - Current count value of Timer1.. */
#define TIMERS_TIMER1_COUNT_REG_TIMER1_COUNT_POS                                                    (0UL)
#define TIMERS_TIMER1_COUNT_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Control of Timer 2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER2_CONTROL_REG                                                                   (0x4001C018UL) /**< rw-*/
#define TIMERS_TIMER2_CONTROL_REG_TIMER2_MODE_MASK                                                  (0x00000001UL) /**< rw - 0 - single shot 1 - free-running. */
#define TIMERS_TIMER2_CONTROL_REG_TIMER2_MODE_POS                                                   (0UL)
#define TIMERS_TIMER2_CONTROL_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timeout value of Timer2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER2_TIMEOUT_REG                                                                   (0x4001C01CUL) /**< rw-*/
#define TIMERS_TIMER2_TIMEOUT_REG_TIMER2_TIMEOUT_MASK                                               (0xFFFFFFFFUL) /**< rw - Initial count value of Timer2. If set to 0, this feature is disabled.. */
#define TIMERS_TIMER2_TIMEOUT_REG_TIMER2_TIMEOUT_POS                                                (0UL)
#define TIMERS_TIMER2_TIMEOUT_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Current count value of Timer2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_TIMER2_COUNT_REG                                                                     (0x4001C020UL) /**< r--*/
#define TIMERS_TIMER2_COUNT_REG_TIMER2_COUNT_MASK                                                   (0xFFFFFFFFUL) /**< r- - Current count value of Timer2.. */
#define TIMERS_TIMER2_COUNT_REG_TIMER2_COUNT_POS                                                    (0UL)
#define TIMERS_TIMER2_COUNT_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Control of Timer 3
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER3_CONTROL_REG                                                                   (0x4001C024UL) /**< rw-*/
#define TIMERS_TIMER3_CONTROL_REG_TIMER3_MODE_MASK                                                  (0x00000001UL) /**< rw - 0 - single shot 1 - free-running. */
#define TIMERS_TIMER3_CONTROL_REG_TIMER3_MODE_POS                                                   (0UL)
#define TIMERS_TIMER3_CONTROL_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timeout value of Timer3
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TIMER3_TIMEOUT_REG                                                                   (0x4001C028UL) /**< rw-*/
#define TIMERS_TIMER3_TIMEOUT_REG_TIMER3_TIMEOUT_MASK                                               (0xFFFFFFFFUL) /**< rw - Initial count value of Timer3. If set to 0, this feature is disabled.. */
#define TIMERS_TIMER3_TIMEOUT_REG_TIMER3_TIMEOUT_POS                                                (0UL)
#define TIMERS_TIMER3_TIMEOUT_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Current count value of Timer3
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_TIMER3_COUNT_REG                                                                     (0x4001C02CUL) /**< r--*/
#define TIMERS_TIMER3_COUNT_REG_TIMER3_COUNT_MASK                                                   (0xFFFFFFFFUL) /**< r- - Current count value of Timer3.. */
#define TIMERS_TIMER3_COUNT_REG_TIMER3_COUNT_POS                                                    (0UL)
#define TIMERS_TIMER3_COUNT_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Control of Watchdog Timer
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_WDOG_CONTROL_REG                                                                     (0x4001C030UL) /**< rw-*/
#define TIMERS_WDOG_CONTROL_REG_WDOG_KICK_MASK                                                      (0x00000001UL) /**< -w - 1 - re-initialise the Watchdog Timer to value WDOG_TIMEOUT 0 - no effect. */
#define TIMERS_WDOG_CONTROL_REG_WDOG_KICK_POS                                                       (0UL)
#define TIMERS_WDOG_CONTROL_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timeout value of Watchdog Timer
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_WDOG_TIMEOUT_REG                                                                     (0x4001C034UL) /**< rw-*/
#define TIMERS_WDOG_TIMEOUT_REG_WDOG_TIMEOUT_MASK                                                   (0x000003FFUL) /**< rw - Initial count value of Watchdog Timer. If set to 0, this feature is disabled.. */
#define TIMERS_WDOG_TIMEOUT_REG_WDOG_TIMEOUT_POS                                                    (0UL)
#define TIMERS_WDOG_TIMEOUT_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Count value of Watchdog Timer which triggers interrupt
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_WDOG_TRIGGER_INT_REG                                                                 (0x4001C038UL) /**< rw-*/
#define TIMERS_WDOG_TRIGGER_INT_REG_WDOG_INT_THRESHOLD_MASK                                         (0x000003FFUL) /**< rw - Value of count_reg which will trig a watchdog IT. */
#define TIMERS_WDOG_TRIGGER_INT_REG_WDOG_INT_THRESHOLD_POS                                          (0UL)
#define TIMERS_WDOG_TRIGGER_INT_REG__RESET_VALUE                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Current count value of Watchdog Timer
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_WDOG_COUNT_REG                                                                       (0x4001C03CUL) /**< r--*/
#define TIMERS_WDOG_COUNT_REG_WDOG_COUNT_MASK                                                       (0x000003FFUL) /**< r- - Current count value of Watchdog Timer.. */
#define TIMERS_WDOG_COUNT_REG_WDOG_COUNT_POS                                                        (0UL)
#define TIMERS_WDOG_COUNT_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Testbus select
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_TBSEL_REG                                                                            (0x4001C040UL) /**< rw-*/
#define TIMERS_TBSEL_REG_TESTBUS_SELECT_MASK                                                        (0x0000000FUL) /**< rw - Selects which set of test signals are routed to register TIMERS_TBVALUE_REG and output timer_testbus_obs[7:0]. */
#define TIMERS_TBSEL_REG_TESTBUS_SELECT_POS                                                         (0UL)
#define TIMERS_TBSEL_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Testbus output
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_TBVALUE_REG                                                                          (0x4001C044UL) /**< r--*/
#define TIMERS_TBVALUE_REG_TESTBUS_VALUE_MASK                                                       (0x000000FFUL) /**< r- - Selected set of test signals Also routed to output timer_testbus_obs[7:0]. */
#define TIMERS_TBVALUE_REG_TESTBUS_VALUE_POS                                                        (0UL)
#define TIMERS_TBVALUE_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timers spare register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define TIMERS_SPARE_REG                                                                            (0x4001C048UL) /**< rw-*/
#define TIMERS_SPARE_REG_SPARE7_MASK                                                                (0x00000080UL) /**< rw - spare7 read/write. */
#define TIMERS_SPARE_REG_SPARE7_POS                                                                 (7UL)
#define TIMERS_SPARE_REG_SPARE6_MASK                                                                (0x00000040UL) /**< rw - spare6 read/write. */
#define TIMERS_SPARE_REG_SPARE6_POS                                                                 (6UL)
#define TIMERS_SPARE_REG_SPARE5_MASK                                                                (0x00000020UL) /**< rw - spare5 read/write. */
#define TIMERS_SPARE_REG_SPARE5_POS                                                                 (5UL)
#define TIMERS_SPARE_REG_SPARE4_MASK                                                                (0x00000010UL) /**< rw - spare4 read/write. */
#define TIMERS_SPARE_REG_SPARE4_POS                                                                 (4UL)
#define TIMERS_SPARE_REG_SPARE3_MASK                                                                (0x00000008UL) /**< r- - spare3 read only. */
#define TIMERS_SPARE_REG_SPARE3_POS                                                                 (3UL)
#define TIMERS_SPARE_REG_SPARE2_MASK                                                                (0x00000004UL) /**< r- - spare2 read only. */
#define TIMERS_SPARE_REG_SPARE2_POS                                                                 (2UL)
#define TIMERS_SPARE_REG_SPARE1_MASK                                                                (0x00000002UL) /**< r- - spare1 read only. */
#define TIMERS_SPARE_REG_SPARE1_POS                                                                 (1UL)
#define TIMERS_SPARE_REG_SPARE0_MASK                                                                (0x00000001UL) /**< r- - spare0 read only. */
#define TIMERS_SPARE_REG_SPARE0_POS                                                                 (0UL)
#define TIMERS_SPARE_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Watchdog Interrupt status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_WDOG_INT_STATUS_REG                                                                  (0x4001FFCCUL) /**< r--*/
#define TIMERS_WDOG_INT_STATUS_REG_WDOG_TIMEOUT_STATUS_MASK                                         (0x00000001UL) /**< r- - Wdog timeout interrupt status. */
#define TIMERS_WDOG_INT_STATUS_REG_WDOG_TIMEOUT_STATUS_POS                                          (0UL)
#define TIMERS_WDOG_INT_STATUS_REG__RESET_VALUE                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Watchdog clear interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define TIMERS_WDOG_INT_CLR_STATUS_REG                                                              (0x4001FFD0UL) /**< -w-*/
#define TIMERS_WDOG_INT_CLR_STATUS_REG_WDOG_TIMEOUT_CLR_STATUS_MASK                                 (0x00000001UL) /**< -w - 1 - clear watchdog timeout interrupt 0 - no effect. */
#define TIMERS_WDOG_INT_CLR_STATUS_REG_WDOG_TIMEOUT_CLR_STATUS_POS                                  (0UL)
#define TIMERS_WDOG_INT_CLR_STATUS_REG__RESET_VALUE                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Watchdog set interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define TIMERS_WDOG_INT_SET_STATUS_REG                                                              (0x4001FFD4UL) /**< -w-*/
#define TIMERS_WDOG_INT_SET_STATUS_REG_WDOG_TIMEOUT_SET_STATUS_MASK                                 (0x00000001UL) /**< -w - 1 - set watchdog timeout interrupt 0 - no effect. */
#define TIMERS_WDOG_INT_SET_STATUS_REG_WDOG_TIMEOUT_SET_STATUS_POS                                  (0UL)
#define TIMERS_WDOG_INT_SET_STATUS_REG__RESET_VALUE                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timer clear interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define TIMERS_INT_CLR_ENABLE_REG                                                                   (0x4001FFD8UL) /**< -w-*/
#define TIMERS_INT_CLR_ENABLE_REG_TIMER3_TIMEOUT_CLR_ENABLE_MASK                                    (0x00000008UL) /**< -x - 1 - clear enable for Timer3 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_ENABLE_REG_TIMER3_TIMEOUT_CLR_ENABLE_POS                                     (3UL)
#define TIMERS_INT_CLR_ENABLE_REG_TIMER2_TIMEOUT_CLR_ENABLE_MASK                                    (0x00000004UL) /**< -x - 1 - clear enable for Timer2 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_ENABLE_REG_TIMER2_TIMEOUT_CLR_ENABLE_POS                                     (2UL)
#define TIMERS_INT_CLR_ENABLE_REG_TIMER1_TIMEOUT_CLR_ENABLE_MASK                                    (0x00000002UL) /**< -x - 1 - clear enable for Timer1 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_ENABLE_REG_TIMER1_TIMEOUT_CLR_ENABLE_POS                                     (1UL)
#define TIMERS_INT_CLR_ENABLE_REG_TIMER0_TIMEOUT_CLR_ENABLE_MASK                                    (0x00000001UL) /**< -x - 1 - clear enable for Timer0 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_ENABLE_REG_TIMER0_TIMEOUT_CLR_ENABLE_POS                                     (0UL)
#define TIMERS_INT_CLR_ENABLE_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timer set interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define TIMERS_INT_SET_ENABLE_REG                                                                   (0x4001FFDCUL) /**< -w-*/
#define TIMERS_INT_SET_ENABLE_REG_TIMER3_TIMEOUT_SET_ENABLE_MASK                                    (0x00000008UL) /**< -x - 1 - set enable for Timer3 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_ENABLE_REG_TIMER3_TIMEOUT_SET_ENABLE_POS                                     (3UL)
#define TIMERS_INT_SET_ENABLE_REG_TIMER2_TIMEOUT_SET_ENABLE_MASK                                    (0x00000004UL) /**< -x - 1 - set enable for Timer2 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_ENABLE_REG_TIMER2_TIMEOUT_SET_ENABLE_POS                                     (2UL)
#define TIMERS_INT_SET_ENABLE_REG_TIMER1_TIMEOUT_SET_ENABLE_MASK                                    (0x00000002UL) /**< -x - 1 - set enable for Timer1 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_ENABLE_REG_TIMER1_TIMEOUT_SET_ENABLE_POS                                     (1UL)
#define TIMERS_INT_SET_ENABLE_REG_TIMER0_TIMEOUT_SET_ENABLE_MASK                                    (0x00000001UL) /**< -x - 1 - set enable for Timer0 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_ENABLE_REG_TIMER0_TIMEOUT_SET_ENABLE_POS                                     (0UL)
#define TIMERS_INT_SET_ENABLE_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timer interrupt status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_INT_STATUS_REG                                                                       (0x4001FFE0UL) /**< r--*/
#define TIMERS_INT_STATUS_REG_TIMER3_TIMEOUT_STATUS_MASK                                            (0x00000008UL) /**< r- - Timer3 timeout interrupt status. */
#define TIMERS_INT_STATUS_REG_TIMER3_TIMEOUT_STATUS_POS                                             (3UL)
#define TIMERS_INT_STATUS_REG_TIMER2_TIMEOUT_STATUS_MASK                                            (0x00000004UL) /**< r- - Timer2 timeout interrupt status. */
#define TIMERS_INT_STATUS_REG_TIMER2_TIMEOUT_STATUS_POS                                             (2UL)
#define TIMERS_INT_STATUS_REG_TIMER1_TIMEOUT_STATUS_MASK                                            (0x00000002UL) /**< r- - Timer1 timeout interrupt status. */
#define TIMERS_INT_STATUS_REG_TIMER1_TIMEOUT_STATUS_POS                                             (1UL)
#define TIMERS_INT_STATUS_REG_TIMER0_TIMEOUT_STATUS_MASK                                            (0x00000001UL) /**< r- - Timer0 timeout interrupt status. */
#define TIMERS_INT_STATUS_REG_TIMER0_TIMEOUT_STATUS_POS                                             (0UL)
#define TIMERS_INT_STATUS_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timer interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define TIMERS_INT_ENABLE_REG                                                                       (0x4001FFE4UL) /**< r--*/
#define TIMERS_INT_ENABLE_REG_TIMER3_TIMEOUT_ENABLE_MASK                                            (0x00000008UL) /**< r- - Timer3 timeout interrupt enable. */
#define TIMERS_INT_ENABLE_REG_TIMER3_TIMEOUT_ENABLE_POS                                             (3UL)
#define TIMERS_INT_ENABLE_REG_TIMER2_TIMEOUT_ENABLE_MASK                                            (0x00000004UL) /**< r- - Timer2 timeout interrupt enable. */
#define TIMERS_INT_ENABLE_REG_TIMER2_TIMEOUT_ENABLE_POS                                             (2UL)
#define TIMERS_INT_ENABLE_REG_TIMER1_TIMEOUT_ENABLE_MASK                                            (0x00000002UL) /**< r- - Timer1 timeout interrupt enable. */
#define TIMERS_INT_ENABLE_REG_TIMER1_TIMEOUT_ENABLE_POS                                             (1UL)
#define TIMERS_INT_ENABLE_REG_TIMER0_TIMEOUT_ENABLE_MASK                                            (0x00000001UL) /**< r- - Timer0 timeout interrupt enable. */
#define TIMERS_INT_ENABLE_REG_TIMER0_TIMEOUT_ENABLE_POS                                             (0UL)
#define TIMERS_INT_ENABLE_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timer clear interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define TIMERS_INT_CLR_STATUS_REG                                                                   (0x4001FFE8UL) /**< -w-*/
#define TIMERS_INT_CLR_STATUS_REG_TIMER3_TIMEOUT_CLR_STATUS_MASK                                    (0x00000008UL) /**< -x - 1 - clear Timer3 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_STATUS_REG_TIMER3_TIMEOUT_CLR_STATUS_POS                                     (3UL)
#define TIMERS_INT_CLR_STATUS_REG_TIMER2_TIMEOUT_CLR_STATUS_MASK                                    (0x00000004UL) /**< -x - 1 - clear Timer2 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_STATUS_REG_TIMER2_TIMEOUT_CLR_STATUS_POS                                     (2UL)
#define TIMERS_INT_CLR_STATUS_REG_TIMER1_TIMEOUT_CLR_STATUS_MASK                                    (0x00000002UL) /**< -x - 1 - clear Timer1 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_STATUS_REG_TIMER1_TIMEOUT_CLR_STATUS_POS                                     (1UL)
#define TIMERS_INT_CLR_STATUS_REG_TIMER0_TIMEOUT_CLR_STATUS_MASK                                    (0x00000001UL) /**< -x - 1 - clear Timer0 timeout interrupt 0 - no effect. */
#define TIMERS_INT_CLR_STATUS_REG_TIMER0_TIMEOUT_CLR_STATUS_POS                                     (0UL)
#define TIMERS_INT_CLR_STATUS_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Timer set interrupt enable
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define TIMERS_INT_SET_STATUS_REG                                                                   (0x4001FFECUL) /**< -w-*/
#define TIMERS_INT_SET_STATUS_REG_TIMER3_TIMEOUT_SET_STATUS_MASK                                    (0x00000008UL) /**< -x - 1 - set Timer3 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_STATUS_REG_TIMER3_TIMEOUT_SET_STATUS_POS                                     (3UL)
#define TIMERS_INT_SET_STATUS_REG_TIMER2_TIMEOUT_SET_STATUS_MASK                                    (0x00000004UL) /**< -x - 1 - set Timer2 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_STATUS_REG_TIMER2_TIMEOUT_SET_STATUS_POS                                     (2UL)
#define TIMERS_INT_SET_STATUS_REG_TIMER1_TIMEOUT_SET_STATUS_MASK                                    (0x00000002UL) /**< -x - 1 - set Timer1 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_STATUS_REG_TIMER1_TIMEOUT_SET_STATUS_POS                                     (1UL)
#define TIMERS_INT_SET_STATUS_REG_TIMER0_TIMEOUT_SET_STATUS_MASK                                    (0x00000001UL) /**< -x - 1 - set Timer0 timeout interrupt 0 - no effect. */
#define TIMERS_INT_SET_STATUS_REG_TIMER0_TIMEOUT_SET_STATUS_POS                                     (0UL)
#define TIMERS_INT_SET_STATUS_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_TIMERS_H_INCLUDED */
