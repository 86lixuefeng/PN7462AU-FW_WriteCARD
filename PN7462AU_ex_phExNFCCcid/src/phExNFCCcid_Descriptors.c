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
 * phExNFCCcid_Descriptors.c: <This File provides the USB Descriptor Informations>
 *
 * Project: PN640
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phExNFCCcid_Descriptors.h"

const USB_DEVICE_DESCRIPTOR  gphExNFCCcid_DeviceDescriptor =
{
    .bLength               = sizeof(USB_DEVICE_DESCRIPTOR),
    .bDescriptorType       = 0x1,        /* USB_DEVICE_DESCRIPTOR_TYPE                */
    .bcdUSB                = 0x0200,     /* USB Specification Release Number          */
    .bDeviceClass          = 0x00,       /* Device Class                              */
    .bDeviceSubClass       = 0x00,       /* Device SubClass                           */
    .bDeviceProtocol       = 0x00,       /* Device Protocol                           */
    .bMaxPacketSize0       = 0x40,       /* Endpoint 0 Max Packet Size                */
    .idVendor              = 0x1FC9,     /* Vendor ID                                 */
    .idProduct             = 0x0117,     /* Product ID                                */
    .bcdDevice             = 0x0101,     /* Device Release Number                     */
    .iManufacturer         = 0x01,       /* Index of String Descriptor - Manufacturer */
    .iProduct              = 0x02,       /* Index of String Descriptor - Product      */
    .iSerialNumber         = 0x03,       /* Index of String Descriptor - Serial No    */
    .bNumConfigurations    = 0x01        /* Number of Configurations                  */
};

const phExNFCCcid_Descriptors_USB_Descriptor_Configuration_t gphExNFCCcid_FSConfigurationDescriptor =
{
    /* Configuration Descriptor */
    .Config =
    {
        .bLength             = sizeof(USB_CONFIGURATION_DESCRIPTOR),                               /* Length of Descriptor     */
        .bDescriptorType     = 0x2,                                                                /* Descriptor Type          */
        .wTotalLength        = sizeof(phExNFCCcid_Descriptors_USB_Descriptor_Configuration_t) - 1,    /* Total Length             */
        .bNumInterfaces      = 1,                                                                  /* Number of Interfaces     */
        .bConfigurationValue = 1,                                                                  /* Configuration Value      */
        .iConfiguration      = 0,                                                                  /* Index of String Descriptor - Configuration */
#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
        .bmAttributes        = 0xA0,                                                               /* Configuration Characteristics              */
#else
        .bmAttributes        = 0x80,                                                               /* Configuration Characteristics              */
#endif
#else
		.bmAttributes        = 0x80,
#endif
        .bMaxPower           = 0xFA                                                                /* Maximum Power Consumption                  */
    },
    /* Interface Descriptor */
    .Ccid_Interface =
    {
        .bLength             = sizeof(USB_INTERFACE_DESCRIPTOR),                                   /* Length of Descriptor      */
        .bDescriptorType     = 0x04,                                                               /* Interface Descriptor Type */
        .bInterfaceNumber    = 0x00,                                                               /* Interface Number          */
        .bAlternateSetting   = 0x00,                                                               /* Alternate Settings        */
        .bNumEndpoints       = 0x03,                                                               /* Total Number of Endpoints */
        .bInterfaceClass     = 0x0B,                                                               /* Smart Card Class          */
        .bInterfaceSubClass  = 0x00,                                                               /* Interface Sub Class       */
        .bInterfaceProtocol  = 0x00,                                                               /* Interface Protocol        */
        .iInterface          = 0x00                                                                /* Index of String Descriptor - Interface      */
    },
    .Ccid_Descriptor =
    {
        .bLength                = sizeof(USB_SMARTCARD_DESCRIPTOR),                                /* Length of Descriptor      */
        .bDescriptorType        = 0x21,                                                            /* CCID Descriptor Type      */
        .bcdCCID                = 0x0110,                                                          /* CCID Specification Number */
        .bMaxSlotIndex          = 0,                                                               /* Maximum Slot Index        */
        .bVoltageSupport        = 0x7,                                                             /* Voltage Support           */
        .dwProtocols            = 0x03,                                                            /* Protocols Support         */
        .dwDefaultClock         = 0xE65,                                                           /* Default Clock             */
        .dwMaximumClock         = 0x37F0,                                                          /* Maximum Clock             */
        .bNumClockSupported     = 0x00,                                                            /* Num of Clock Supported    */
        .dwDataRate             = 0x26B5,                                                          /* Data Rate                 */
        .dwMaxDataRate          = 0xCF080,                                                         /* Maximum Data Rate         */
        .bNumDataRatesSupported = 0x00,                                                            /* Number of Data Rates Sup  */
        .dwMaxIFSD              = 0xFE,                                                            /* Maximum IFSD              */
        .dwSynchProtocols       = 0x0,                                                             /* Synch Protocols           */
        .dwMechanical           = 0x0,                                                             /* Mechanical Features       */
        .dwFeatures             = 0x204BE,                                                         /* Features Supported        */
        .dwMaxCCIDMessageLength = 0x10F,                                                           /* CCID Msg Length           */
        .bClassGetResponse      = 0x00,                                                            /* Get Response              */
        .bClassEnvelope         = 0x00,                                                            /* Class Envelope            */
        .wLcdLayout             = 0x0,                                                             /* LCD Support               */
        .bPinSupport            = 0,                                                               /* PIN Support               */
        .bMaxCCIDBusySlots      = 1                                                                /* Busy CCID Slots           */
    },
    /* Bulk IN End Point Descriptor */
    .Ccid_DataInEndpoint =
    {
        .bLength             = sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /* Length of the Descriptor    */
        .bDescriptorType     = 0x05,                                                               /* Endpoint Descriptor Type    */
        .bEndpointAddress    = 0x81,                                                               /* Bulk IN Endpoint Address    */
        .bmAttributes        = 0x02,                                                               /* Attributes                  */
        .wMaxPacketSize      = 0x40,                                                               /* Maximum Packet Size         */
        .bInterval           = 0x00                                                                /* Interval Period             */
    },
    /* Bulk OUT End Point Descriptor */
    .Ccid_DataOutEndpoint =
    {
        .bLength             = sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /* Length of the Descriptor    */
        .bDescriptorType     = 0x05,                                                               /* Endpoint Descriptor Type    */
        .bEndpointAddress    = 0x01,                                                               /* Bulk Out Endpoint Address   */
        .bmAttributes        = 0x02,                                                               /* Attributes                  */
        .wMaxPacketSize      = 0x40,                                                               /* Maximum Packet Size         */
        .bInterval           = 0x00                                                                /* Interval Period             */
    },
    .Ccid_InterruptInEndpoint =
    {
        .bLength             = sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /* Length of Descriptor       */
        .bDescriptorType     = 0x05,                                                               /* Endpoint Descriptor Type   */
        .bEndpointAddress    = 0x82,                                                               /* Interrupt Endpoint Address */
        .bmAttributes        = 0x03,                                                               /* Attributes                 */
        .wMaxPacketSize      = 0x40,                                                               /* Maximum Packet Size        */
        .bInterval           = 0x04                                                                /* Interval Period            */
     },
    .Ccid_Termination = 0x00

};



#ifndef FULL_SPEED_ONLY
USB_DEVICE_QUALIFIER_DESCRIPTOR gphExNFCCcid_DeviceQualifierDescriptor =
{
    .bLength            = sizeof(USB_DEVICE_QUALIFIER_DESCRIPTOR),
    .bDescriptorType    = 0x06,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = 0x40,
    .bNumConfigurations = 1
};
#endif
