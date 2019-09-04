/*
 * NfcrdlibEx4_MIFAREClassic.h
 *
 *  Created on: May 19, 2016
 *      Author: nxa05442
 */

#ifndef INTFS_NFCRDLIBEX4_MIFARECLASSIC_H_
#define INTFS_NFCRDLIBEX4_MIFARECLASSIC_H_
#include <ph_Status.h>

#ifdef PH_OSAL_FREERTOS

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define MIFARECLASSIC_TASK_STACK              (1200/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define MIFARECLASSIC_TASK_STACK              (1200/4)
#else /* __PN74XXXX__*/
#define MIFARECLASSIC_TASK_STACK              1200
#endif /* __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

#define MIFARECLASSIC_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define MIFARECLASSIC_TASK_STACK                0x20000
#define MIFARECLASSIC_TASK_PRIO                 0
#endif /* PH_OSAL_LINUX */


#endif /* INTFS_NFCRDLIBEX4_MIFARECLASSIC_H_ */
