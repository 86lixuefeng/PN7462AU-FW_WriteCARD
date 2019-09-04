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
 * phExCcid_Usb_If.h: USB Interface System specific Initialization
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
 */

#ifndef PHEXCCID_USB_IF_H
#define PHEXCCID_USB_IF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "PN7462AU/PN7462AU_usb.h"
#include "ph_Status.h"
#include "phhalPcr.h"
#include "app_usbd_cfg.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#ifdef __ICCARM__
#define _PPTOSTR_(x) #x
#define PRAGMA_WEAK(func,alias)  _Pragma(_PPTOSTR_(weak func=alias))
#define ATTR_WEAK
#define ATTR_ALIAS(Func)
#else
#define PRAGMA_WEAK(func,alias)
#define ATTR_WEAK                   __attribute__ ((weak))
#define ATTR_ALIAS(Func)            __attribute__ ((alias( #Func )))
#endif
#define USBHAL_CORE_MEM_SIZE                0x400 //0x500
#define USBHAL_CCID_MEM_SIZE                0x100

#define PN7462AU_USB_MAX_PACKET_SIZE        USB_FS_MAX_BULK_PACKET

#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
#ifndef PHFL_USB_MRA2
#define PHEXCCID_USB_IF_SUSPEND_POWER_REDUCTION_SETTINGS       (E_PD_GPIO1 | E_PD_GPIO2 | E_PD_GPIO3 | E_PD_GPIO4 | E_PD_GPIO5 |\
                                                                E_PD_GPIO6 | E_PD_GPIO7 | E_PD_ATXA | E_PD_ATXB | E_PD_ATXC |\
                                                                E_PD_ATXD | E_PD_INT_AUX | E_PD_IO_AUX | E_PD_CLK_AUX | E_PD_SPIM |\
                                                                E_PD_IICM | E_DISABLE_CT_CLK | E_PCR_ANA_TX_STANDBY_GSN | E_DISABLE_CLIF)
#endif
#endif

#define PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_TMP (0x0D)
#define PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP        (0x80)

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */
extern volatile uint8_t gphExCcid_Usb_CORE_Buffer[USBHAL_CORE_MEM_SIZE];
extern volatile uint8_t gphExCcid_Usb_CCID_buffer[USBHAL_CCID_MEM_SIZE];
extern USBD_HANDLE_T UsbHandle;

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phExCcid_Usb_If_Init(void);
phStatus_t phExCcid_Usb_If_TotalInit(void);

void phExCcid_Usbd_CORE_Init (void);
void phExCcid_Usbd_CCID_Init (void);
void phExCcid_Usbd_Connect_Enable(void);

#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
phStatus_t phExCcid_Usb_If_Suspend();
void phExCcid_Usb_If_Resume();
void phExCcid_Usbd_Remote_Wakeup();

#endif

#endif /* __HALUSB_H */
