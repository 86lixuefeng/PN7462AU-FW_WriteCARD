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
 * mw_usbd_cciduser.c: <CCID Class Custom User Module>
 *
 * Project: PN7426AU
 * $Date: 2015-08-26 11:34:37 +0530 (Wed, 26 Aug 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14749 $ (v04.13.00)
 */
#include <string.h> /*for memcpy */

#include "mw_usbd.h"
#include "mw_usbd_core.h"
#include "mw_usbd_hw.h"
#include "mw_usbd_cciduser.h"

#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif


void mwCCID_SetStallEP (USB_CCID_CTRL_T* pCcidCtrl, uint32_t EPNum)
{
    /* set EP halt status according stall status */
    hwUSB_SetStallEP(pCcidCtrl->pUsbCtrl, EPNum);
    pCcidCtrl->pUsbCtrl->ep_halt  |=  (EPNum & 0x80) ? ((1 << 16) << (EPNum & 0x0F)) : (1 << EPNum);
}

ErrorCode_t mwCCID_ep0_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
  USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
  USB_CCID_CTRL_T* pCcidCtrl = (USB_CCID_CTRL_T*) data;
  ErrorCode_t ret = ERR_USBD_UNHANDLED;

  switch (event)
  {
    case USB_EVT_SETUP:
       if ((pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_CLASS) &&
          (pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE) &&
          (pCtrl->SetupPacket.wIndex.WB.L == pCcidCtrl->if_num) )
       {
             switch (pCtrl->SetupPacket.bRequest)
             {
                case CCID_USB_CONTROL_REQUEST_ABORT:
                    mwUSB_StatusInStage(pCtrl);
                    return LPC_OK;
                break;
                case CCID_USB_CONTROL_REQUEST_GET_CLOCK_FREQUENCIES:
                case CCID_USB_CONTROL_REQUEST_GET_DATA_RATES:
                     return LPC_OK;
                 default:
                     break;
              }
       }
       else
       {
           /* MISRA */
       }
    default:
       break;
   }
   return ret;

}

uint32_t mwCCID_GetMemSize(USBD_CCID_INIT_PARAM_T* param)
{
  uint32_t req_len = 0;

  /* calculate required length */
  req_len += sizeof(USB_CCID_CTRL_T);
  req_len += 4; /* for alignment overhead */
  req_len &= ~0x3;

  return req_len;
}

ErrorCode_t mwCCID_init(USBD_HANDLE_T hUsb, USBD_CCID_INIT_PARAM_T* param)
{
   uint32_t new_addr, i, ep_indx;
   ErrorCode_t ret = LPC_OK;
   USB_CCID_CTRL_T* pCcidCtrl;
   USB_ENDPOINT_DESCRIPTOR* pEpDesc;
   USB_SMARTCARD_DESCRIPTOR* pSmartDesc;
   USB_INTERFACE_DESCRIPTOR* pIntfDesc = (USB_INTERFACE_DESCRIPTOR*)param->intf_desc;

   /* check for memory alignment */
   if ((param->mem_base &  0x3) &&
       (param->mem_size < sizeof(USB_CCID_CTRL_T)))
      return ERR_USBD_BAD_MEM_BUF;

   /* allocate memory for the control data structure */
   pCcidCtrl = (USB_CCID_CTRL_T*)param->mem_base;
   param->mem_base += sizeof(USB_CCID_CTRL_T);
   param->mem_size -= sizeof(USB_CCID_CTRL_T);

   /* align to 4 byte boundary */
   while (param->mem_base & 0x03)
   {
      param->mem_base++;
      param->mem_size--;
   }

   /* Init Control Structure with passed params */
   memset((void*)pCcidCtrl, 0, sizeof(USB_CCID_CTRL_T));
   pCcidCtrl->pUsbCtrl = (USB_CORE_CTRL_T*)hUsb;

   /* parse the interface descriptor */
   if ((pIntfDesc->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) &&
       (pIntfDesc->bInterfaceClass == USB_DEVICE_CLASS_CCID) &&
       (pIntfDesc->bNumEndpoints >= 2))
   {
      /* store interface number */
      pCcidCtrl->if_num = pIntfDesc->bInterfaceNumber;

      new_addr = (uint32_t)pIntfDesc + pIntfDesc->bLength;

      /* Now we have the Smart Card Descriptor */
      pSmartDesc = (USB_SMARTCARD_DESCRIPTOR*)new_addr;

      if (pSmartDesc->bDescriptorType != USB_SMARTCARD_DESCRIPTOR_TYPE)
         return ERR_USBD_BAD_DESC;

      /* Store the SmartCard Descriptor Address */
      pCcidCtrl->smart_desc = (uint8_t*)new_addr;

      /* move to next descriptor */
      new_addr += pSmartDesc->bLength;

      /* move to next descriptor */
      for (i = 0; i < pIntfDesc->bNumEndpoints; i++)
      {
         pEpDesc = (USB_ENDPOINT_DESCRIPTOR*)new_addr;
         new_addr = (uint32_t)pEpDesc + pEpDesc->bLength;

         /* parse endpoint descriptor */
         if ((pEpDesc->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE) &&
             (pEpDesc->bmAttributes    == USB_ENDPOINT_TYPE_BULK))
         {

            if (pEpDesc->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK)
            {
               /* store BULK IN endpoint */
               pCcidCtrl->epin_num = pEpDesc->bEndpointAddress;
               ep_indx = ((pCcidCtrl->epin_num & 0x0F) << 1) + 1;
               /* register endpoint interrupt handler */
               if (param->CCID_EpBulkIn_Hdlr != 0)
               {
                  ret = mwUSB_RegisterEpHandler (hUsb, ep_indx, param->CCID_EpBulkIn_Hdlr, pCcidCtrl);
               }
            }
            else
            {
               /* store BULK OUT endpoint */
               pCcidCtrl->epout_num = pEpDesc->bEndpointAddress;
               ep_indx = ((pCcidCtrl->epout_num & 0x0F) << 1);
               /* register endpoint interrupt handler */
               if (param->CCID_EpBulkOut_Hdlr != 0)
               {
                  ret = mwUSB_RegisterEpHandler (hUsb, ep_indx, param->CCID_EpBulkOut_Hdlr, pCcidCtrl);
               }
            }
         }
         else if((pEpDesc->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE) &&
                 (pEpDesc->bmAttributes    == USB_ENDPOINT_TYPE_INTERRUPT))
         {
            if (pEpDesc->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK)
            {
               /* store INTERRUPT IN endpoint */
               pCcidCtrl->epint_num = pEpDesc->bEndpointAddress;
               ep_indx = ((pCcidCtrl->epint_num & 0x0F) << 1) + 1;
               /* register endpoint interrupt handler if provided*/
               if (param->CCID_EpIntIn_Hdlr != 0)
               {
                   ret = mwUSB_RegisterEpHandler (hUsb, ep_indx, param->CCID_EpIntIn_Hdlr, pCcidCtrl);
               }
            }
         }
      }

   }
   else
       return ERR_USBD_BAD_INTF_DESC;

  if ( (pCcidCtrl->epin_num == 0) || (pCcidCtrl->epout_num == 0) || (pCcidCtrl->epint_num == 0) || (ret != LPC_OK) )
    return ERR_USBD_BAD_EP_DESC;

  /* register ep0 handler */
  /* check if user wants his own handler */
  if (param->CCID_Ep0_Hdlr == 0) {
    ret = mwUSB_RegisterClassHandler(hUsb, mwCCID_ep0_hdlr, pCcidCtrl);
  }
  else {
    ret = mwUSB_RegisterClassHandler(hUsb, param->CCID_Ep0_Hdlr, pCcidCtrl);
    param->CCID_Ep0_Hdlr = mwCCID_ep0_hdlr;
  }

  return ret;
}
