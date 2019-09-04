/*
 * usbd_cdc_str.h
 *
 *  Created on: Oct 12, 2015
 *      Author: nxp69678
 */

#ifndef USBD_CDC_STR_H_
#define USBD_CDC_STR_H_
#include "mw_usbd.h"
#include "mw_usbd_hw.h"
#include "usbd_cdcuser.h"

/** USB Standard Interface Association Descriptor */
PRE_PACK struct POST_PACK _USB_IAD_DESCRIPTOR
{
  uint8_t  bLength; /**< Size of this descriptor in bytes*/
  uint8_t  bDescriptorType; /**< INTERFACE ASSOCIATION Descriptor Type*/
  uint8_t  bFirstInterface; /**< Interface number of the first interface that is
                            associated with this function.*/
  uint8_t  bInterfaceCount; /**< Number of contiguous interfaces that are
                            associated with this function. */
  uint8_t  bFunctionClass; /**< Class code (assigned by USB-IF). \n
                            A value of zero is not allowed in this descriptor.
                            If this field is FFH, the function class is vendorspecific.
                            All other values are reserved for assignment by
                            the USB-IF.*/
  uint8_t  bFunctionSubClass; /**< Subclass code (assigned by USB-IF). \n
                            If the bFunctionClass field is not set to FFH all
                            values are reserved for assignment by the USBIF.*/
  uint8_t  bFunctionProtocol; /**< Protocol code (assigned by the USB). \n
                                These codes are qualified by the values of the
                                bFunctionClass and bFunctionSubClass fields.*/
  uint8_t  iFunction; /**< Index of string descriptor describing this function.*/
} ;
/** USB Standard Interface Association Descriptor */
typedef struct _USB_IAD_DESCRIPTOR USB_IAD_DESCRIPTOR;
#define USB_INTERFACE_ASSOC_DESC_SIZE   (sizeof(USB_IAD_DESCRIPTOR))

#endif /* USBD_CDC_STR_H_ */
