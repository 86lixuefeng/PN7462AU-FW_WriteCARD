/*
 * @brief Configuration file needed for USB ROM stack based applications.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "ph_Datatypes.h"
#include "error.h"


#ifndef _APP_USB_CFG_H_
#define _APP_USB_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/** @ingroup EXAMPLES_USBDLIB_11XX_CDC
 * @{
 */

#define PN7462AU_USB_BASE                      0x40028000UL

/* Manifest constants used by USBD LIB stack. These values SHOULD NOT BE CHANGED
   for advance features which require usage of USB_CORE_CTRL_T structure.
   Since these are the values used for compiling USB stack.
 */
#define USB_MAX_IF_NUM          1       /*!< Max interface number used for building USBDL_Lib. Please understand the system while setting/changing this parameter. */
#define USB_MAX_EP_NUM          3       /*!< Max number of EP used for building USBD_Lib. Please understand the system while setting/changing this parameter. */
#define USB_MAX_PACKET0         64      /*!< Max EP0 packet size used for building USBD_Lib. Please understand the system while setting/changing this parameter. */
#define USB_FS_MAX_BULK_PACKET  64      /*!< MAXP for FS bulk EPs used for building USBD_Lib. Please understand the system while setting/changing this parameter. */
#define USB_HS_MAX_BULK_PACKET  64      /*!< MAXP for HS bulk EPs used for building USBD_Lib. Please understand the system while setting/changing this parameter. */
#define USB_DFU_XFER_SIZE       64      /*!< Max DFU transfer size used for building USBD_Lib. Please understand the system while setting/changing this parameter. */

/* Manifest constants defining interface numbers and endpoints used by a
   particular interface in this application.
 */
#define USB_CDC_CIF_NUM         0
#define USB_CDC_DIF_NUM         1
#define USB_CDC_IN_EP           0x81
#define USB_CDC_OUT_EP          0x01
#define USB_CDC_INT_EP          0x82

/* USB descriptor arrays defined *_desc.c file */
extern const uint8_t USB_DeviceDescriptor[];
extern uint8_t USB_FsConfigDescriptor[];
extern const uint8_t USB_StringDescriptor[];
extern const uint8_t USB_DeviceQualifier[];

/**
 * @brief	Find the address of interface descriptor for given class type.
 * @param	pDesc		: Pointer to configuration descriptor in which the desired class
 *			interface descriptor to be found.
 * @param	intfClass	: Interface class type to be searched.
 * @return	If found returns the address of requested interface else returns NULL.
 */
extern USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass);

#define FULL_SPEED_ONLY
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _APP_USB_CFG_H_ */
