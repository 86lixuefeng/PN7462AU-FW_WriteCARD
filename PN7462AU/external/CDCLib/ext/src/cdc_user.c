/***********************************************************************
* $Id$
*
* Project: USB device ROM Stack
*
* Description:
*     USB Communication Device Class User module.
*
***********************************************************************
*   Copyright(C) 2011, NXP Semiconductor
*   All rights reserved.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/
#include <string.h> /*for memcpy */
#include "mw_usbd.h"
#include "mw_usbd_core.h"
#include "mw_usbd_hw.h"
#include "usbd_cdc.h"
#include "usbd_cdcuser.h"

#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif

/*
 *  CDC Send Class Notification routine
 *  Parameters:     hCdc: Handle to the CDC Control Structure.
 *   								bNotification: Notification type
 *									data: Notification data
 *  Return Value:   ErrorCode_t type to indicate success or error condition.
 */

/*----------------------------------------------------------------------------
  Send the SERIAL_STATE notification as defined in usbcdc11.pdf, 6.3.5.
 *---------------------------------------------------------------------------*/
ErrorCode_t mwCDC_SendNotification (USBD_HANDLE_T hCdc, uint8_t bNotification, uint16_t data)
{
  USB_CDC_CTRL_T* pCdcCtrl = (USB_CDC_CTRL_T*)hCdc;

  uint16_t len = 8;

  pCdcCtrl->notice_buf[0] = 0xA1;                           /* bmRequestType */
  pCdcCtrl->notice_buf[1] = bNotification;
  pCdcCtrl->notice_buf[2] = 0x00;                           /* wValue */
  pCdcCtrl->notice_buf[3] = 0x00;
  pCdcCtrl->notice_buf[4] = 0x00;                           /* wIndex (Interface #, LSB first) */
  pCdcCtrl->notice_buf[5] = 0x00;
  pCdcCtrl->notice_buf[6] = 0x00;                           /* wLength (Data length = 2 bytes, LSB first) */
  pCdcCtrl->notice_buf[7] = 0x00;

  switch (bNotification) {
    case CDC_NOTIFICATION_NETWORK_CONNECTION:
      if ( data) {
        pCdcCtrl->notice_buf[2] = 1;
      }
      break;
    case CDC_RESPONSE_AVAILABLE:
      break;
    case CDC_NOTIFICATION_SERIAL_STATE:
      pCdcCtrl->notice_buf[6] = 0x02;                /* wLength (Data length = 2 bytes, LSB first) */
      pCdcCtrl->notice_buf[8] = (data >>  0) & 0xFF; /* UART State Bitmap (16bits, LSB first) */
      pCdcCtrl->notice_buf[9] = (data >>  8) & 0xFF;
      len = 10;
    default:
      return ERR_API_INVALID_PARAM2;
  }
  hwUSB_WriteEP (pCdcCtrl->pUsbCtrl, pCdcCtrl->epint_num,
    &pCdcCtrl->notice_buf[0], len);   /* send notification */

  return  LPC_OK;
}

/*
 *  CDC Get Request Callback routine
 *  Parameters:     hCdc: Handle to the CDC Control Structure.
 *   								pSetup: Pointer to setup packet received from host.
 *									pBuffer: Pointer to a pointer of data buffer containing response data.
 *									length: Amount of data to be sent back to host.
 *  Return Value:   ErrorCode_t type to indicate success or error condition.
 */

ErrorCode_t mwCDC_GetRequest( USBD_HANDLE_T hCdc, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* length)
{
  USB_CDC_CTRL_T* pCdcCtrl = (USB_CDC_CTRL_T*)hCdc;
  ErrorCode_t ret = ERR_USBD_UNHANDLED;

  switch (pSetup->bRequest) {
    case CDC_GET_ENCAPSULATED_RESPONSE:
      if (pCdcCtrl->GetEncpsResp) {
        ret = pCdcCtrl->GetEncpsResp( hCdc, pBuffer, length);
        break;
      }
      /* fall through */
    case CDC_GET_COMM_FEATURE:
      if (pCdcCtrl->GetCommFeature) {
        ret = pCdcCtrl->GetCommFeature( hCdc, pSetup->wValue.W, pBuffer, length);
        break;
      }
      else {
        memset (*pBuffer, 0, USB_MAX_PACKET0);
      }
      ret = LPC_OK;
      break;
    case CDC_GET_LINE_CODING:
      *pBuffer = (uint8_t*)&pCdcCtrl->line_coding;
      *length = 7;
      ret = LPC_OK;
      break;
  }
  return ret;
}

/*
 *  CDC Set Request Callback routine
 *  Parameters:     hCdc: Handle to the CDC Control Structure.
 *   								pSetup: Pointer to setup packet received from host.
 *									pBuffer: Pointer to a pointer of data buffer containing request data.
 *									length: Amount of data copied to destination buffer.
 *  Return Value:   ErrorCode_t type to indicate success or error condition.
 */

ErrorCode_t mwCDC_SetRequest( USBD_HANDLE_T hCdc, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length)
{
  USB_CDC_CTRL_T* pCdcCtrl = (USB_CDC_CTRL_T*)hCdc;
  ErrorCode_t ret = LPC_OK;

  switch (pSetup->bRequest) {
    case CDC_SEND_ENCAPSULATED_COMMAND:
      if ((length != 0 ) && (pCdcCtrl->SendEncpsCmd != 0)) {
        ret = pCdcCtrl->SendEncpsCmd( hCdc, *pBuffer, length);
      }
      break;
    case CDC_SET_COMM_FEATURE:
      if ((length != 0 ) && (pCdcCtrl->SetCommFeature != 0)) {
        ret = pCdcCtrl->SetCommFeature( hCdc, pSetup->wValue.W, *pBuffer, length);
      }
      break;
    case CDC_CLEAR_COMM_FEATURE:
      if (pCdcCtrl->ClrCommFeature) {
        ret = pCdcCtrl->ClrCommFeature( hCdc, pSetup->wValue.W);
      }
      break;
    case CDC_SET_CONTROL_LINE_STATE:
      if (pCdcCtrl->SetCtrlLineState) {
        ret = pCdcCtrl->SetCtrlLineState( hCdc, pSetup->wValue.W);
      }
      break;
    case CDC_SEND_BREAK:
      if (pCdcCtrl->SendBreak) {
        ret = pCdcCtrl->SendBreak( hCdc, pSetup->wValue.W);
      }
      break;
    case CDC_SET_LINE_CODING:

      if (length != 0) {
        /* line coding data should have been copied to the data member */
        if (pCdcCtrl->SetLineCode) {
          ret = pCdcCtrl->SetLineCode( hCdc, &pCdcCtrl->line_coding);
          break;
        }
      }
      else {
        /* copy line coding data directly into the structure */
        *pBuffer = (uint8_t*)&pCdcCtrl->line_coding;
      }
      break;
    default:
      ret = ERR_USBD_UNHANDLED;
      break;
  }
  return ret;
}

/*
 *  Default CDC Class Handler
 *  Parameters:     hUsb: Handle to the USB device stack.
 *   								data: Pointer to the data which will be passed when callback function is called by the stack.
 *									event:  Type of endpoint event.
 *  Return Value:   ErrorCode_t type to indicate success or error condition.
 */

ErrorCode_t mwCDC_ep0_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
  USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
  USB_CDC_CTRL_T* pCdcCtrl = (USB_CDC_CTRL_T*) data;
  ErrorCode_t ret = ERR_USBD_UNHANDLED;

  switch (event) {
    case USB_EVT_SETUP:
      if ((pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_CLASS) &&
          (pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE) &&
          ((pCtrl->SetupPacket.wIndex.WB.L == pCdcCtrl->cif_num)  ||       /* IF number correct? */
           (pCtrl->SetupPacket.wIndex.WB.L == pCdcCtrl->dif_num)) ) {

        /* assign default EP0 buffer for data transfer */
        pCtrl->EP0Data.pData = pCtrl->EP0Buf;                      /* data to be received/sent */
        /* check if it is set request */
        if ( pCtrl->SetupPacket.bmRequestType.BM.Dir == REQUEST_HOST_TO_DEVICE) {
          /* ask user where to recieve the data associated with request */
          ret = pCdcCtrl->CIC_SetRequest(pCdcCtrl, &pCtrl->SetupPacket,
                &pCtrl->EP0Data.pData, 0);
          if ( (ret == LPC_OK) && (pCtrl->SetupPacket.wLength == 0)) {
						mwUSB_StatusInStage(pCtrl);   /* send status handshake */
          }
        }
        else {
          /* allow user to copy data to EP0Buf or change the pointer to his own buffer */
          ret = pCdcCtrl->CIC_GetRequest(pCdcCtrl, &pCtrl->SetupPacket,
            &pCtrl->EP0Data.pData, &pCtrl->EP0Data.Count);
          if (ret == LPC_OK) {
            mwUSB_DataInStage(pCtrl);                                 /* send requested data */
          }
        }
      }
      break;
    case USB_EVT_OUT:
      if ((pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_CLASS) &&
          (pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE) &&
          ((pCtrl->SetupPacket.wIndex.WB.L == pCdcCtrl->cif_num)  ||       /* IF number correct? */
           (pCtrl->SetupPacket.wIndex.WB.L == pCdcCtrl->dif_num)) ) {

        pCtrl->EP0Data.pData -= pCtrl->SetupPacket.wLength;
        ret = pCdcCtrl->CIC_SetRequest(pCdcCtrl, &pCtrl->SetupPacket, &pCtrl->EP0Data.pData,
           pCtrl->SetupPacket.wLength);
        if ( ret == LPC_OK) {

          mwUSB_StatusInStage(pCtrl);                         /* send Acknowledge */
        }
      }
      break;
    default:
      break;
  }
  return ret;
}

/**
 * @brief   Get memory required by CDC class.
 * @param [in/out] param parameter structure used for initialisation.
 * @retval  Length required for CDC data structure and buffers.
 *
 * Example Usage:
 * @code
 *    mem_req = mwCDC_GetMemSize(param);
 * @endcode
 */
uint32_t mwCDC_GetMemSize(USBD_CDC_INIT_PARAM_T* param)
{
  uint32_t req_len = 0;

  /* calculate required length */
  req_len += sizeof(USB_CDC_CTRL_T);   /* memory for MSC controller structure */
  req_len += 4; /* for alignment overhead */
  req_len &= ~0x3;

  return req_len;
}

/*
 *  MSC function initialization routine
 *  Parameters:     hUsb: Handle to the USB device stack.
 *   								param: Structure containing CDC function driver module
 *						      initialization parameters.
 *									phCDC: Handle to CDC Control Structure
 *  Return Value:   ErrorCode_t type to indicate success or error condition.
 */

ErrorCode_t mwCDC_init(USBD_HANDLE_T hUsb, USBD_CDC_INIT_PARAM_T* param, USBD_HANDLE_T* phCDC)
{
  ErrorCode_t ret = LPC_OK;
  uint32_t new_addr, i;
  USB_CDC_CTRL_T* pCdcCtrl;
  USB_ENDPOINT_DESCRIPTOR* pEpDesc;
  USB_INTERFACE_DESCRIPTOR* pIntfDesc = (USB_INTERFACE_DESCRIPTOR*)param->cif_intf_desc;

  /* check for memory alignment */
  if ((param->mem_base &  0x3) &&
    (param->mem_size < sizeof(USB_CDC_CTRL_T)))
    return ERR_USBD_BAD_MEM_BUF;

  /* allocate memory for the control data structure */
  pCdcCtrl = (USB_CDC_CTRL_T*)param->mem_base;
  param->mem_base += sizeof(USB_CDC_CTRL_T);
  param->mem_size -= sizeof(USB_CDC_CTRL_T);
  /* align to 4 byte boundary */
  param->mem_base += 0x03;
  param->mem_base &= ~0x03;
  param->mem_size &= ~0x03;

  /* Init control structures with passed params */
  memset((void*)pCdcCtrl, 0, sizeof(USB_CDC_CTRL_T));
  pCdcCtrl->pUsbCtrl = (USB_CORE_CTRL_T*)hUsb;
  /* copy default handlers */
  pCdcCtrl->CIC_GetRequest = mwCDC_GetRequest;
  pCdcCtrl->CIC_SetRequest = mwCDC_SetRequest;
  /* copy user defined call backs functions*/
  pCdcCtrl->SendEncpsCmd = param->SendEncpsCmd;
  pCdcCtrl->GetEncpsResp = param->GetEncpsResp;
  pCdcCtrl->SetCommFeature = param->SetCommFeature;
  pCdcCtrl->GetCommFeature = param->GetCommFeature;
  pCdcCtrl->ClrCommFeature = param->ClrCommFeature;
  pCdcCtrl->SetCtrlLineState = param->SetCtrlLineState;
  pCdcCtrl->SendBreak = param->SendBreak;
  pCdcCtrl->SetLineCode = param->SetLineCode;


  /* parse the control interface descriptor */
  if ((pIntfDesc->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) &&
    (pIntfDesc->bInterfaceClass == CDC_COMMUNICATION_INTERFACE_CLASS) &&
    (pIntfDesc->bNumEndpoints < 2) ) {

    /* store interface number */
    pCdcCtrl->cif_num = pIntfDesc->bInterfaceNumber;
    if (pIntfDesc->bNumEndpoints == 1) {
      new_addr = (uint32_t)pIntfDesc + pIntfDesc->bLength;
      /* move to next descriptor */
      do {
        pEpDesc = (USB_ENDPOINT_DESCRIPTOR*)new_addr;
        new_addr = (uint32_t)pEpDesc + pEpDesc->bLength;

        /* parse endpoint descriptor */
        if ((pEpDesc->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE) &&
           (pEpDesc->bmAttributes == USB_ENDPOINT_TYPE_INTERRUPT)) {

          /* store INTERRUPT IN endpoint */
          pCdcCtrl->epint_num = pEpDesc->bEndpointAddress;
          break;
        }
      } while(pEpDesc->bDescriptorType != USB_INTERFACE_DESCRIPTOR_TYPE);

      if (pCdcCtrl->epint_num == 0)
        return ERR_USBD_BAD_EP_DESC;
    }
  }
  else
    return ERR_USBD_BAD_INTF_DESC;

  /* parse the data interface descriptor */
  pIntfDesc = (USB_INTERFACE_DESCRIPTOR*)param->dif_intf_desc;
  if ((pIntfDesc->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) &&
    (pIntfDesc->bInterfaceClass == CDC_DATA_INTERFACE_CLASS) &&
    (pIntfDesc->bInterfaceSubClass == 0x00) ) {

    /* store interface number */
    pCdcCtrl->dif_num = pIntfDesc->bInterfaceNumber;
    new_addr = (uint32_t)pIntfDesc + pIntfDesc->bLength;
    /* move to next descriptor */
    for (i = 0; i < pIntfDesc->bNumEndpoints; i++) {
      pEpDesc = (USB_ENDPOINT_DESCRIPTOR*)new_addr;
      new_addr = (uint32_t)pEpDesc + pEpDesc->bLength;

      /* parse endpoint descriptor */
      if ((pEpDesc->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE) &&
         (pEpDesc->bmAttributes == USB_ENDPOINT_TYPE_BULK)) {

        if (pEpDesc->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) {
          /* store BULK IN endpoint */
          pCdcCtrl->epin_num = pEpDesc->bEndpointAddress;
        }
        else {
          /* store BULK OUT endpoint */
          pCdcCtrl->epout_num = pEpDesc->bEndpointAddress;
        }
      }
    }
  }
  else
    return ERR_USBD_BAD_INTF_DESC;

  if ( (pCdcCtrl->epin_num == 0) || (pCdcCtrl->epout_num == 0) || (ret != LPC_OK) )
    return ERR_USBD_BAD_EP_DESC;

  /* register ep0 handler */
  /* check if user wants his own handler */
  if (param->CDC_Ep0_Hdlr == 0) {
    ret = mwUSB_RegisterClassHandler(hUsb, mwCDC_ep0_hdlr, pCdcCtrl);
  }
  else {
    ret = mwUSB_RegisterClassHandler(hUsb, param->CDC_Ep0_Hdlr, pCdcCtrl);
	param->CDC_Ep0_Hdlr = mwCDC_ep0_hdlr;
  }
  /* return the handle */
  *phCDC = (USBD_HANDLE_T)pCdcCtrl;

  return ret;
}
