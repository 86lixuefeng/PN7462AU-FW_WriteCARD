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
 * phExNFCCcid_UsbUser.c: <This File provides the USB User Specific Initializations>
 *
 * Project: PN7462AU
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

/* Stack Related */
#include "mw_usbd_hw.h"
#include "mw_usbd_desc.h"
#include "mw_usbd_cciduser.h"

/* Application Related */
#include "app_usbd_cfg.h"
#include "phExNFCCcid_Descriptors.h"
#include "phExNFCCcid_Usb_If.h"
#include "phExNFCCcid_UsbCcid.h"

/* USB Core Buffer */
PH_ALIGN(256) volatile uint8_t gphExNFCCcid_Usb_CORE_Buffer[USBHAL_CORE_MEM_SIZE];
/* USB CCID Buffer */
PH_ALIGN(4) volatile  uint8_t gphExNFCCcid_Usb_CCID_buffer[USBHAL_CCID_MEM_SIZE];

USBD_HANDLE_T UsbHandle;

const uint8_t gphExNFCCcid_FSStringDescriptor[] =
{
    0x04,
    0x03,
    WBVAL(LANGUAGE_ID_ENG),

    0x08,
    0x03,
    'N',0,'X',0,'P',0,

    0x1C,
    0x03,
    'P',0,'N',0,'7',0,'4',0,'6',0,'2',0,'a',0,'u',0,' ',0,'C',0,'C',0,'I',0,'D',0,

    0x0A,
    0x03,
    '1',0,'.',0,'0',0,'0',0
};


PRAGMA_WEAK(USB_Interface_Event,UsbdRom_Event_Stub)
ErrorCode_t USB_Interface_Event (USBD_HANDLE_T hUsb) ATTR_WEAK ATTR_ALIAS(UsbdRom_Event_Stub);
ErrorCode_t UsbdRom_Event_Stub (USBD_HANDLE_T hUsb)
{
    return LPC_OK;
}

PRAGMA_WEAK(USB_Configure_Event,USB_Configure_Event_Stub)
ErrorCode_t USB_Configure_Event (USBD_HANDLE_T hUsb) ATTR_WEAK ATTR_ALIAS(USB_Configure_Event_Stub);
ErrorCode_t USB_Configure_Event_Stub (USBD_HANDLE_T hUsb)
{
    USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;

    gphExNFCCcid_sUsb_Bus_Status.bAddressed = pCtrl->config_value;
    return LPC_OK;
}

PRAGMA_WEAK(USB_Reset_Event,USB_Reset_Event_Stub)
ErrorCode_t USB_Reset_Event (USBD_HANDLE_T hUsb) ATTR_WEAK ATTR_ALIAS(USB_Reset_Event_Stub);
ErrorCode_t USB_Reset_Event_Stub (USBD_HANDLE_T hUsb)
{
    USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;

    pCtrl->device_status = 0;
    gphExNFCCcid_sUsb_Bus_Status.bAddressed = pCtrl->config_value;

    return LPC_OK;
}

#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
PRAGMA_WEAK(USB_Suspend_Event,USB_Suspend_Event_Stub)
ErrorCode_t USB_Suspend_Event (USBD_HANDLE_T hUsb) ATTR_WEAK ATTR_ALIAS(USB_Suspend_Event_Stub);
ErrorCode_t USB_Suspend_Event_Stub (USBD_HANDLE_T hUsb)
{
    if (gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable == 0)
    {
        gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable = 1;
    }

    return LPC_OK;
}


PRAGMA_WEAK(USB_Resume_Event,USB_Resume_Event_Stub)
ErrorCode_t USB_Resume_Event (USBD_HANDLE_T hUsb) ATTR_WEAK ATTR_ALIAS(USB_Resume_Event_Stub);
ErrorCode_t USB_Resume_Event_Stub (USBD_HANDLE_T hUsb)
{
    if (gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable == 1)
    {
        gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable = 0;
    }

    return LPC_OK;
}

PRAGMA_WEAK(USB_Power_Event,USB_Power_Event_Stub)
ErrorCode_t USB_Power_Event (USBD_HANDLE_T hUsb, uint32_t param1) ATTR_WEAK ATTR_ALIAS(USB_Power_Event_Stub);
ErrorCode_t USB_Power_Event_Stub (USBD_HANDLE_T hUsb, uint32_t param1)
{
    gphExNFCCcid_sUsb_Bus_Status.bAddressed = 0;
    return LPC_OK;
}

#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
PRAGMA_WEAK(USB_WakeUpCfg,USB_WakeUpCfg_Stub)
ErrorCode_t USB_WakeUpCfg (USBD_HANDLE_T hUsb, uint32_t param1) ATTR_WEAK ATTR_ALIAS(USB_WakeUpCfg_Stub);
ErrorCode_t USB_WakeUpCfg_Stub (USBD_HANDLE_T hUsb, uint32_t param1)
{
    /* Don't remove this WakeupCfg function - Current Consumption */
    hwUSB_WakeUpCfg(hUsb, param1);
    if (param1)
        gphExNFCCcid_sUsb_Bus_Status.bRemoteWakeupEnable = 1;
    else
        gphExNFCCcid_sUsb_Bus_Status.bRemoteWakeupEnable = 0;
    return LPC_OK;
}
#endif
#endif
/*
 * User Defined Interrupt IN End Point Handler
 */
ErrorCode_t CCID_Interrupt_In_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    switch (event)
    {
    case USB_EVT_IN:
    case USB_EVT_IN_NAK:
         break;
        //phExNFCCcid_UsbCcid_Usb_IntIn();
    }
    return LPC_OK;
}


/*
 *  User Defined Bulk Out EndPoint Handler
 */
ErrorCode_t CCID_Bulk_Out_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    switch (event)
    {
    case USB_EVT_OUT_NAK:
         break;
    case USB_EVT_OUT:
         phExNFCCcid_UsbCcid_Usb_Bulk_Out ();
         break;
    default:
        break;
    }
    return LPC_OK;
}



/*
 * User Defined Bulk IN EndPoint Handler
 */
ErrorCode_t CCID_Bulk_In_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    switch (event)
    {
    case USB_EVT_IN:
         phExNFCCcid_UsbCcid_Usb_Bulk_In();
         break;
    case USB_EVT_IN_NAK:
    default:
        break;
    }

    return LPC_OK;
}

/*
 * USB CCID Class Initialization
 */
void phExNFCCcid_Usbd_CCID_Init(void)
{
    USBD_CCID_INIT_PARAM_T  ccid_param =
    {
        .intf_desc           = (uint8_t*)&(gphExNFCCcid_FSConfigurationDescriptor.Ccid_Interface),
        .mem_base            = (uint32_t)gphExNFCCcid_Usb_CCID_buffer,
        .mem_size            = USBHAL_CCID_MEM_SIZE,
        .CCID_EpBulkOut_Hdlr = &CCID_Bulk_Out_hdlr,
        .CCID_EpBulkIn_Hdlr  = &CCID_Bulk_In_hdlr,
        .CCID_EpIntIn_Hdlr   = &CCID_Interrupt_In_hdlr
    };
    mwCCID_init(UsbHandle, &ccid_param);
}

/*
 * USB Core Initialization
 */
void phExNFCCcid_Usbd_CORE_Init(void)
{
    ErrorCode_t ret;

    USBD_API_INIT_PARAM_T usb_param =
    {
        .usb_reg_base        = PN7462AU_USB_BASE,
        .max_num_ep          = (USB_MAX_EP_NUM),
        .mem_base            = (uint32_t) gphExNFCCcid_Usb_CORE_Buffer,
        .mem_size            = USBHAL_CORE_MEM_SIZE,
        .USB_Configure_Event = USB_Configure_Event,
        .USB_Reset_Event     = USB_Reset_Event
#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
        ,
        .USB_Power_Event     = USB_Power_Event,
        .USB_Suspend_Event   = USB_Suspend_Event,
        .USB_Resume_Event    = USB_Resume_Event
#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
        ,
        .USB_WakeUpCfg       = USB_WakeUpCfg
#endif
#endif
    };

    USB_CORE_DESCS_T DeviceDes = {NULL};

    /* add custom Interface Event */
    if(USB_Interface_Event != UsbdRom_Event_Stub)
    {
        usb_param.USB_Interface_Event = USB_Interface_Event;
    }

#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
    if(USB_Suspend_Event != USB_Suspend_Event_Stub)
    {
        usb_param.USB_Suspend_Event = USB_Suspend_Event;
    }

    if(USB_Resume_Event != USB_Resume_Event_Stub)
    {
        usb_param.USB_Resume_Event = USB_Resume_Event;
    }
    if (USB_Power_Event != USB_Power_Event_Stub)
    {
        usb_param.USB_Power_Event = USB_Power_Event;
    }
#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
    if (USB_WakeUpCfg != USB_WakeUpCfg_Stub)
    {
        usb_param.USB_WakeUpCfg = USB_WakeUpCfg;
    }
#endif
#endif
    DeviceDes.device_desc      = (uint8_t*)&gphExNFCCcid_DeviceDescriptor;
    DeviceDes.high_speed_desc  = (uint8_t*)&gphExNFCCcid_FSConfigurationDescriptor;
    DeviceDes.full_speed_desc  = (uint8_t*)&gphExNFCCcid_FSConfigurationDescriptor;
    DeviceDes.string_desc      = (uint8_t*)gphExNFCCcid_FSStringDescriptor;
#ifndef FULL_SPEED_ONLY
    DeviceDes.device_qualifier = (uint8_t*)&gphExNFCCcid_DeviceQualifierDescriptor;
#else
    DeviceDes.device_qualifier = (uint8_t*)0;
#endif

    ret = hwUSB_Init(&UsbHandle, &DeviceDes, &usb_param);

    /* Failed initialization */
    while(ret != LPC_OK);
}

void HIF_IRQHandler(void)
{
    hwUSB_ISR(UsbHandle);
}

void phExNFCCcid_Usbd_Connect_Enable(void)
{
    hwUSB_Connect(UsbHandle, 1);
}

#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
void phExNFCCcid_Usbd_Remote_Wakeup()
{
    if (gphExNFCCcid_sUsb_Bus_Status.bRemoteWakeupEnable == 1)
    {
        hwUSB_WakeUpCfg(UsbHandle, 0);
        hwUSB_WakeUp(UsbHandle);
        hwUSB_WakeUpCfg(UsbHandle, 1);
    }
    else
    {
        hwUSB_WakeUpCfg(UsbHandle, 1);
    }
}
#endif
