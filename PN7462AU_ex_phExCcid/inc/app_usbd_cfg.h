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
 * USB Stack specific Compile time Build Configuration
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-05 15:52:30 +0530 (Tue, 05 Jul 2016) $
 * $Author: nxp86397 $
 * $Revision: 3650 $ (v05.19.00)
 */

#ifndef __APP_USBD_CFG_H
#define __APP_USBD_CFG_H

#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"
#include "mw_usbd.h"

/*
 * MACROS DEFINITIONS
 */

#define PN7462AU_USB_BASE                      0x40028000UL

#define USB_STRING_LEN(UnicodeChars)        (2 + ((UnicodeChars) << 1))
#define LANGUAGE_ID_ENG                     0x0409

#define USB_MAX_IF_NUM                      1         /*!< Max interface number used for building USBDL_Lib. DON'T CHANGE. */
#define USB_MAX_EP_NUM                      3         /*!< Max number of EP used for building USBD_Lib. DON'T CHANGE. */
#define USB_MAX_PACKET0                     64        /*!< Max EP0 packet size used for building USBD_Lib. DON'T CHANGE. */
#define USB_FS_MAX_BULK_PACKET              64        /*!< MAXP for FS bulk EPs used for building USBD_Lib. DON'T CHANGE. */
#define USB_HS_MAX_BULK_PACKET              64        /*!< MAXP for HS bulk EPs used for building USBD_Lib. DON'T CHANGE. */

#define FULL_SPEED_ONLY

#endif
