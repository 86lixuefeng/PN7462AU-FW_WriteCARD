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
 * mw_usbd_ccid.h : CCID Descriptor Information
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-04 17:19:56 +0530 (Fri, 04 Sep 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15087 $ (v04.13.00)
 */
#ifndef __CCID_H__
#define __CCID_H__

#include "mw_usbd.h"

/** \file
 *  \brief  CCID Class Descriptors.
 *
 *  Definition of Smart Class Descriptors and the control commands.
 *
 */

 /* CCID Class Control Request */
#define CCID_USB_CONTROL_REQUEST_ABORT                      0x01
#define CCID_USB_CONTROL_REQUEST_GET_CLOCK_FREQUENCIES      0x02
#define CCID_USB_CONTROL_REQUEST_GET_DATA_RATES             0x03

#define USB_DEVICE_CLASS_CCID                               0x0B
/* Smart Card Class Specific Device Descriptor*/
#define USB_SMARTCARD_DESCRIPTOR_TYPE                       0x21

/** Smart Card Descriptor
 *
 *  DWG_Smart-Card_CCID_Rev110.pdf
 *  Table 5.1-1 Smart Card Device Class Descriptors
 *
 */
typedef PRE_PACK struct _USB_SMARTCARD_DESCRIPTOR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdCCID;
    uint8_t  bMaxSlotIndex;
    uint8_t  bVoltageSupport;
    uint32_t dwProtocols;
    uint32_t dwDefaultClock;
    uint32_t dwMaximumClock;
    uint8_t  bNumClockSupported;
    uint32_t dwDataRate;
    uint32_t dwMaxDataRate;
    uint8_t  bNumDataRatesSupported;
    uint32_t dwMaxIFSD;
    uint32_t dwSynchProtocols;
    uint32_t dwMechanical;
    uint32_t dwFeatures;
    uint32_t dwMaxCCIDMessageLength;
    uint8_t  bClassGetResponse;
    uint8_t  bClassEnvelope;
    uint16_t wLcdLayout;
    uint8_t  bPinSupport;
    uint8_t  bMaxCCIDBusySlots;
} POST_PACK USB_SMARTCARD_DESCRIPTOR;

#endif  /* __CCID_H__ */
