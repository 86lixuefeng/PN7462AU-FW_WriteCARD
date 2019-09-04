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
 * phExNFCCcid_Descriptors.h: CCID Descriptor Informations
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

#ifndef PHEXNFCCCID_DESCRIPTORS_H
#define PHEXNFCCCID_DESCRIPTORS_H

#include "mw_usbd.h"
#include "ph_Datatypes.h"
#include "mw_usbd_ccid.h"
#include "mw_usbd_desc.h"
#include "app_usbd_cfg.h"

/** @brief  Type define for the device configuration descriptor structure. This must be defined in the
 *          application code, as the configuration descriptor contains several sub-descriptors which
 *          vary between devices, and which describe the device's usage to the host.
 */
typedef PH_PACK_STRUCT_BEGIN struct {
    USB_CONFIGURATION_DESCRIPTOR  Config;
    USB_INTERFACE_DESCRIPTOR      Ccid_Interface;
    USB_SMARTCARD_DESCRIPTOR      Ccid_Descriptor;
    USB_ENDPOINT_DESCRIPTOR       Ccid_DataInEndpoint;
    USB_ENDPOINT_DESCRIPTOR       Ccid_DataOutEndpoint;
    USB_ENDPOINT_DESCRIPTOR       Ccid_InterruptInEndpoint;
    uint8_t                       Ccid_Termination;
}PH_PACK_STRUCT_END phExNFCCcid_Descriptors_USB_Descriptor_Configuration_t;


extern const USB_DEVICE_DESCRIPTOR           gphExNFCCcid_DeviceDescriptor;
extern const phExNFCCcid_Descriptors_USB_Descriptor_Configuration_t  gphExNFCCcid_FSConfigurationDescriptor;

#ifndef FULL_SPEED_ONLY
extern USB_DEVICE_QUALIFIER_DESCRIPTOR gphExNFCCcid_DeviceQualifierDescriptor;
#endif


#endif /* PHEXNFCCCID_DESCRIPTORS */
