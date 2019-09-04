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
 * Prototype of NVIC module
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12425 $ (v04.13.00)
 * $Date: 2015-05-08 14:15:31 +0530 (Fri, 08 May 2015) $
 *
 * History:
 *
 */

#ifndef PHHAL_NVIC_H
#define PHHAL_NVIC_H

#include "PN7462AU/PN7462AU_regcore.h"

typedef enum
{
    PHHAL_NVIC_TIMER,           /* 0 */
    PHHAL_NVIC_RES1,           /* 1 */
    PHHAL_NVIC_CLIF,            /* 2 */
    PHHAL_NVIC_EECTRL,          /* 3 */
    PHHAL_NVIC_RES4,           /* 4 */
    PHHAL_NVIC_RES5,           /* 5 */
    PHHAL_NVIC_HIF,             /* 6 */
    PHHAL_NVIC_CTIF,            /* 7 */
    PHHAL_NVIC_BMA,             /* 8 */
    PHHAL_NVIC_PMU,             /* 9 */
    PHHAL_NVIC_SPI,             /* 10 */
    PHHAL_NVIC_I2C,             /* 11 */
    PHHAL_NVIC_PCR,             /* 12 */
    PHHAL_NVIC_GPIO_Common_Isr, /* 13 */
    PHHAL_NVIC_GPIO_01_Isr, /* 14 */
    PHHAL_NVIC_GPIO_02_Isr, /* 15 */
    PHHAL_NVIC_GPIO_03_Isr, /* 16 */
    PHHAL_NVIC_GPIO_04_Isr, /* 17 */
    PHHAL_NVIC_GPIO_05_Isr, /* 18 */
    PHHAL_NVIC_GPIO_06_Isr, /* 19 */
    PHHAL_NVIC_GPIO_07_Isr, /* 20 */
    PHHAL_NVIC_GPIO_08_Isr, /* 21 */
    PHHAL_NVIC_GPIO_09_Isr, /* 22 */
    PHHAL_NVIC_GPIO_10_Isr, /* 23 */
    PHHAL_NVIC_GPIO_11_Isr, /* 24 */
    PHHAL_NVIC_GPIO_12_Isr, /* 25 */
    PHHAL_NVIC_RES_26, /* 26 */
    PHHAL_NVIC_RES_27, /* 27 */
    PHHAL_NVIC_RES_28, /* 28 */
    PHHAL_NVIC_RES_29, /* 29 */
    PHHAL_NVIC_RES_30, /* 30 */
    PHHAL_NVIC_RES_31  /* 31 */
}phHal_Nvic_t;

#define PH_HAL_INTRPT_PRIO_REALTIME        (0x00U)
#define PH_HAL_INTRPT_PRIO_HIGH            (0x01U)
#define PH_HAL_INTRPT_PRIO_MED             (0x02U)
#define PH_HAL_INTRPT_PRIO_LOW             (0x03U)

#define PH_HAL_NVIC_TIMER_MASK    (1U << PHHAL_NVIC_TIMER)
#define PH_HAL_NVIC_CLIF_MASK     (1U << PHHAL_NVIC_CLIF)
#define PH_HAL_NVIC_EECTRL_MASK   (1U << PHHAL_NVIC_EECTRL)
#define PH_HAL_NVIC_SWPTX_MASK    (1U << PHHAL_NVIC_SWPTX)
#define PH_HAL_NVIC_SWPRX_MASK    (1U << PHHAL_NVIC_SWPRX)
#define PH_HAL_NVIC_HIF_MASK      (1U << PHHAL_NVIC_HIF)
#define PH_HAL_NVIC_CTIF_MASK     (1U << PHHAL_NVIC_CTIF)
#define PH_HAL_NVIC_BMA_MASK      (1U << PHHAL_NVIC_BMA)
#define PH_HAL_NVIC_PMU_MASK      (1U << PHHAL_NVIC_PMU)
#define PH_HAL_NVIC_SPI_MASK      (1U << PHHAL_NVIC_SPI)
#define PH_HAL_NVIC_I2C_MASK      (1U << PHHAL_NVIC_I2C)
#define PH_HAL_NVIC_PCR_MASK      (1U << PHHAL_NVIC_PCR)
#define PH_HAL_NVIC_NMI_MASK      (1U << PHHAL_NVIC_NMI)

#define PH_HAL_NVIC_DISABLE_ALL_INTERRUPTS() PH_REG_SET(NVIC_ICER_REG, 0xFFFFFFFFUL)

#define phHal_Nvic_EnableInterrupt(IrqMask)   PH_REG_SET(NVIC_ISER_REG, IrqMask)

#define phHal_Nvic_DisableInterrupt(IrqMask)   PH_REG_SET(NVIC_ICER_REG, IrqMask)

#define phHal_Nvic_GetInterruptEnabled(IrqMask)  ((PH_REG_GET(NVIC_ISER_REG)) & (IrqMask))

#define phHal_Nvic_ClearPendingInterrupt(IrqMask)   PH_REG_SET(NVIC_ICPR_REG, IrqMask)

#define phHal_Nvic_SetPendingInterrupt(IrqMask)   PH_REG_SET(NVIC_ISPR_REG, IrqMask)

#define phHal_Nvic_GetPendingInterrupt(IrqMask)    ((PH_REG_GET(NVIC_ISPR_REG)) & (IrqMask))

#define phHal_Nvic_ClearAllPendingInterrupt()    PH_REG_SET(NVIC_ICPR_REG, 0xFFFFFFFFUL);


/* Set interrupt priorities */

#define phHal_Nvic_GetPrioReg(num)      ( NVIC_IPR0_REG + (((num)/4) * 4) )
#define phHal_Nvic_GetNum(num)          ( (((num) - (((num)/4) * 4)) * 8) + 6 )


#define phHal_Nvic_SetInterruptPrio(num, prio)     *((uint32_t *)phHal_Nvic_GetPrioReg(num)) =                                                  \
                                                    ((*((uint32_t *)phHal_Nvic_GetPrioReg(num)) \
                                                     & (~(0x03U << phHal_Nvic_GetNum(num))))          \
                                                        | (((prio) << phHal_Nvic_GetNum(num))))


#define phHal_Nvic_SetSvCPrio(prio)         PH_REG_SET(SVC_SHPR2_REG, ((prio) << SVC_SHPR2_REG_PRIORITY_POS))
#define phHal_Nvic_SetPendSVPrio(prio)      PH_REG_SET_FIELDS_NS_SC(SVC_SHPR3_REG, SVC_SHPR3_REG_PENDSV_MASK, \
                                                    ((prio) << SVC_SHPR3_REG_PENDSV_POS))
#define phHal_Nvic_SetSysTick(prio)         PH_REG_SET_FIELDS_NS_SC(SVC_SHPR3_REG, SVC_SHPR3_REG_SYSTICK_MASK, \
                                                    ((prio) << SVC_SHPR3_REG_SYSTICK_POS))

#endif /* PHHAL_NVIC_H */
