/*
 * @brief Vitual communication port example
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
#include "phhalHif_Usb.h"
#include <stdio.h>
#include <string.h>
#include "usbd_cdc_str.h"
#include "mw_usbd_hw.h"
#include "app_usbd_cfg.h"
#include "cdc_vcom.h"
#include "mw_usbd.h"
#include "usbd_cdcuser.h"
#include "phhalNvic.h"
#include "phUser.h"

#ifdef NXPBUILD__PHHAL_USB

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* USB Core Buffer */
PH_ALIGN(256)
volatile uint8_t gpUsb_CORE_Buffer[256 * (USB_MAX_EP_NUM+2)] PH_NOINIT;

PH_NOINIT_ALIGNED
static USBD_HANDLE_T g_hUsb;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initialize pin and clocks for Hif-USB */
static void usb_pin_clk_init(void)
{
    uint16_t wStatus;

    wStatus = phhalHif_Usb_Init(PHHAL_HIF_USB_PLL_CLKEDGESNUM_DEFAULT, PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
            PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT, true);
    PH_USER_ASSERT(wStatus == PH_ERR_SUCCESS);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
    USB_COMMON_DESCRIPTOR *pD;
    USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
    uint32_t next_desc_adr;

    pD = (USB_COMMON_DESCRIPTOR *) pDesc;
    next_desc_adr = (uint32_t) pDesc;

    while (pD->bLength) {
        /* is it interface descriptor */
        if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

            pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
            /* did we find the right interface descriptor */
            if (pIntfDesc->bInterfaceClass == intfClass) {
                break;
            }
        }
        pIntfDesc = 0;
        next_desc_adr = (uint32_t) pD + pD->bLength;
        pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
    }

    return pIntfDesc;
}

/**
 * @brief    main routine for HW and SW initialization of the VCOM
 * @return    Function should not exit.
 */
phStatus_t VCom_Init(void)
{
    USBD_API_INIT_PARAM_T usb_param;
    USB_CORE_DESCS_T desc;
    ErrorCode_t ret = LPC_OK;

    /* enable clocks and pinmux */
    usb_pin_clk_init();

    memset((void *)&gpUsb_CORE_Buffer[0], 0x00, sizeof(gpUsb_CORE_Buffer));

    /* initialize call back structures */
    memset((void *) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
    usb_param.usb_reg_base = PN7462AU_USB_BASE;
    usb_param.max_num_ep = USB_MAX_EP_NUM;
    usb_param.mem_base = (uint32_t)gpUsb_CORE_Buffer;
    usb_param.mem_size = sizeof(gpUsb_CORE_Buffer);

    /* Set the USB descriptors */
    desc.device_desc = (uint8_t *) &USB_DeviceDescriptor[0];
    desc.string_desc = (uint8_t *) &USB_StringDescriptor[0];
    /* Note, to pass USBCV test full-speed only devices should have both
       descriptor arrays point to same location and device_qualifier set to 0.
     */
    desc.high_speed_desc = (uint8_t *) &USB_FsConfigDescriptor[0];
    desc.full_speed_desc = (uint8_t *) &USB_FsConfigDescriptor[0];
    desc.device_qualifier = 0;

    /* USB Initialization */
    ret = hwUSB_Init(&g_hUsb, &desc, &usb_param);
    if (ret == LPC_OK)
    {
        /* Init VCOM interface */
        ret = vcom_init(g_hUsb, &desc, &usb_param);
        if (ret == LPC_OK)
        {
            /*  enable USB interrrupts */
            phHal_Nvic_EnableInterrupt(PHHAL_NVIC_HIF);
            /* now connect */
            hwUSB_Connect(g_hUsb, 1);
        }
        else
        {
            return PH_ERR_FAILED;
        }
    }
    else
    {
        return PH_ERR_FAILED;
    }

    return PH_ERR_SUCCESS;

}

uint32_t VCom_Read(uint8_t *pBuf, uint32_t buf_len)
{
    return vcom_bread(pBuf, buf_len);
}

#define CHUNK_SIZE 63

uint32_t VCom_Write(uint8_t *pBuf, uint32_t buf_len)
{
    int ret = 0;
    int writeOffset = 0;
    int idwLen = buf_len;
    extern VCOM_DATA_T g_vCOM;
    while ( idwLen > CHUNK_SIZE ) {
        ret += vcom_write(pBuf + writeOffset, CHUNK_SIZE);
        while (0 != ( g_vCOM.tx_flags & VCOM_TX_BUSY));
        writeOffset += CHUNK_SIZE;
        idwLen -= CHUNK_SIZE;
    }
    if (idwLen > 0) {
        ret += vcom_write(pBuf + writeOffset, idwLen);
    }
    return ret;
}

void HIF_IRQHandler(void)
{
    hwUSB_ISR(g_hUsb);
}

#endif /* NXPBUILD__PHHAL_USB */
