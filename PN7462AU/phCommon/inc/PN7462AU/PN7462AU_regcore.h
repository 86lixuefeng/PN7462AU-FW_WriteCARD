/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */
/** \file
 * ARM Core register and bit definitions.
 * $Author: Purnank H G (ing05193) $
 * $Revision: 11786 $ (v04.13.00)
 * $Date: 2015-04-23 17:14:00 +0530 (Thu, 23 Apr 2015) $
 *
 */

#ifndef PN7462AU_IP_REGCORE_H_INCLUDED
#define PN7462AU_IP_REGCORE_H_INCLUDED

#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_regcore_rorw.h"

/**
* \name SysTick Control and Status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define SYST_CSR_REG                                                       (0xE000E010UL) /**< rw*/
#define SYST_CSR_REG_COUNTFLAG_MASK                                        (0x00010000UL) /**< r - Counter Flag. */
#define SYST_CSR_REG_COUNTFLAG_POS                                         (16UL)
#define SYST_CSR_REG_CLKSOURCE_MASK                                        (0x00000004UL) /**< rw -Clock source. */
#define SYST_CSR_REG_CLKSOURCE_POS                                         (2UL)
#define SYST_CSR_REG_TICKINT_MASK                                          (0x00000002UL) /**< rw  Systick Exception Enable. */
#define SYST_CSR_REG_TICKINT_POS                                           (1UL)
#define SYST_CSR_REG_ENABLE_MASK                                           (0x00000001UL) /**< rw - Systick counter Enable. */
#define SYST_CSR_REG_ENABLE_POS                                            (0UL)

/**
* \name SysTick Reload value register
* <b>Reset value:</b> Unknown \n
* <b>Access:</b> rw
* @{
*/
#define SYST_RVR_REG                                                       (0xE000E014UL) /**< rw*/

/**
* \name SysTick Current value register
* <b>Reset value:</b> Unknown \n
* <b>Access:</b> rw
* @{
*/
#define SYST_CVR_REG                                                       (0xE000E018UL) /**< rw*/
#define SYST_CVR_REG_VALUE_MASK                                            (0x00FFFFFFUL) /**< rW*/

/**
* \name SysTick Calibration value register
* <b>Reset value:</b> IMP DEF \n
* <b>Access:</b> r
* @{
*/
#define SYST_CALIB_REG                                                     (0xE000E01CUL) /**< r*/
#define SYST_CALIB_REG_NOREF_MASK                                          (0x80000000UL) /**< r - Reference clock. */
#define SYST_CALIB_REG_NOREF_POS                                           (31UL)
#define SYST_CALIB_REG_SKEW_MASK                                           (0x40000000UL) /**< r - Clock skew. */
#define SYST_CALIB_REG_SKEW_POS                                            (30UL)
#define SYST_CALIB_REG_TENMS_MASK                                          (0x00FFFFFFUL) /**< r - 10ms optional reload value. */
#define SYST_CALIB_REG_TENMS_POS                                           (0UL)



/**
* \name NVIC ISER REG Interrupt set enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_ISER_REG                   (0xE000E100UL) /**< rw*/

/**
* @}
*/

/**
* \name NVIC ICER REG Interrupt clear enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_ICER_REG                   (0xE000E180UL) /**< rw*/

/**
* @}
*/


/**
* \name NVIC ICER REG Interrupt set pending register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_ISPR_REG                   (0xE000E200UL) /**< rw*/

/**
* @}
*/

/**
* \name NVIC ICER REG Interrupt clear pending register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_ICPR_REG                   (0xE000E280UL) /**< rw*/

/**
* @}
*/


/**
* \name NVIC ICER REG Interrupt 3-0 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR0_REG                   (0xE000E400UL) /**< rw*/

/**
* @}
*/


/**
* \name NVIC ICER REG Interrupt 7-4 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR1_REG                   (0xE000E404UL) /**< rw*/

/**
* @}
*/

/**
* \name NVIC ICER REG Interrupt 11-8 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR2_REG                   (0xE000E408UL) /**< rw*/

/**
* @}
*/

/**
* \name NVIC ICER REG Interrupt 15-12 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR3_REG                   (0xE000E40CUL) /**< rw*/

/**
* @}
*/

/**
* \name NVIC ICER REG Interrupt 19-16 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR4_REG                   (0xE000E410UL) /**< rw*/

/**
* @}
*/


/**
* \name NVIC ICER REG Interrupt 23-20 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR5_REG                   (0xE000E414UL) /**< rw*/

/**
* @}
*/


/**
* \name NVIC ICER REG Interrupt 27-24 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR6_REG                   (0xE000E418UL) /**< rw*/

/**
* @}
*/

/**
* \name NVIC ICER REG Interrupt 31-28 priority register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define NVIC_IPR7_REG                   (0xE000E41CUL) /**< rw*/

/**
* @}
*/




/**
* \name System Handler Priority Register 2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define SVC_SHPR2_REG                            (0xE000ED1CUL) /**< rw*/
#define SVC_SHPR2_REG_PRIORITY_MASK              (0xFF000000UL) /**< rw */
#define SVC_SHPR2_REG_PRIORITY_POS               (30UL)

/**
* @}
*/



/**
* \name System Handler Priority Register 3
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define SVC_SHPR3_REG                            (0xE000ED20UL) /**< rw*/
#define SVC_SHPR3_REG_PENDSV_MASK                (0x00C00000UL) /**< rw */
#define SVC_SHPR3_REG_PENDSV_POS                 (22UL)
#define SVC_SHPR3_REG_SYSTICK_MASK               (0xC0000000UL) /**< rw */
#define SVC_SHPR3_REG_SYSTICK_POS                (30UL)

/**
* @}
*/


#endif /* PN7462AU_IP_REGCORE_H_INCLUDED */
