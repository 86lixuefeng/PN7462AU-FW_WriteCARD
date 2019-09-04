/***********************************************************************
 * $Id$
 *
 * Project: IP3511 USB device controller definitions
 *
 * Description:
 *     This file contains the structure definitions and manifest
 *     constants for the IP3511 component:
 *         Universal Serial Bus Device Controller
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

#ifndef __HW_USBD_IP3511_H
#define __HW_USBD_IP3511_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef volatile struct _EP_LIST {
    uint32_t  buf_ptr;
    uint32_t  buf_length;
} EP_LIST;

/* USB Module Register Structure */
typedef volatile struct {       /* USB Structure          */
    volatile uint32_t DEVCMDSTAT;   /* USB Device Command/Status register */
    volatile uint32_t INFO;         /* USB Info register      */
    volatile uint32_t EPLISTSTART;  /* USB EP Command/Status List start address */
    volatile uint32_t DATABUFSTART; /* USB Data buffer start address */
    volatile uint32_t LPM;          /* Link Power Management register */
    volatile uint32_t EPSKIP;       /* USB Endpoint skip      */
    volatile uint32_t EPINUSE;      /* USB Endpoint Buffer in use */
    volatile uint32_t EPBUFCFG;     /* USB Endpoint Buffer Configuration register */
    volatile uint32_t INTSTAT;      /* USB interrupt status register */
    volatile uint32_t INTEN;        /* USB interrupt enable register */
    volatile uint32_t INTSETSTAT;   /* USB set interrupt status register */
    volatile uint32_t INTROUTING;   /* USB interrupt routing register */
    volatile uint32_t RESERVED0[1]; /* HW Module Configuration information */
    volatile uint32_t EPTOGGLE;     /* USB Endpoint toggle register */
} USB_REGS_T;

#define EP_ZERO_BUF_MAX_BYTES (64 * 2)  /* EP 0 needs 2 buffers aligned at 64 byte boundary */
                                        /* IN and OUT share single buffer */

#define BUF_ACTIVE          (0x1U << 31)
#define EP_DISABLED         (0x1 << 30)
#define EP_STALL            (0x1 << 29)
#define EP_RESET            (0x1 << 28)
#define EP_ISO_TYPE         (0x1 << 26)

/* USB Device Command Status */
#define USB_EN              (0x1 << 7)  /* Device Enable */
#define USB_SETUP_RCVD      (0x1 << 8)  /* SETUP token received */
#define USB_PLL_ON          (0x1 << 9)  /* PLL is always ON */
#define USB_LPM             (0x1 << 11) /* LPM is supported */
#define USB_IntOnNAK_AO     (0x1 << 12) /* Device Interrupt on NAK BULK OUT */
#define USB_IntOnNAK_AI     (0x1 << 13) /* Device Interrupt on NAK BULK IN */
#define USB_IntOnNAK_CO     (0x1 << 14) /* Device Interrupt on NAK CTRL OUT */
#define USB_IntOnNAK_CI     (0x1 << 15) /* Device Interrupt on NAK CTRL IN */
#define USB_DCON            (0x1 << 16) /* Device connect */
#define USB_DSUS            (0x1 << 17) /* Device Suspend */
#define USB_LPM_SUS         (0x1 << 19) /* LPM suspend */
#define USB_REMOTE_WAKE     (0x1 << 20) /* LPM Remote Wakeup */
#define USB_DCON_C          (0x1 << 24) /* Device connection change */
#define USB_DSUS_C          (0x1 << 25) /* Device SUSPEND change */
#define USB_DRESET_C        (0x1 << 26) /* Device RESET */
#define USB_VBUS_DBOUNCE    (0x1 << 28) /* Device VBUS detect */

/* Device Interrupt Bit Definitions */
#define MAX_PHY_EP_INTS      (0x3FF)
#define NZ_EP_OUT_MASK      (0x555555554)
#define NZ_EP_IN_MASK       (0xAAAAAAAA8)
#define FRAME_INT           (0x1 << 30)
#define DEV_STAT_INT        (0x80000000)

/* Rx & Tx Packet Length Definitions */
#define PKT_LNGTH_MASK      0x000003FF

/* Error Status Register Definitions */
#define ERR_NOERROR             0x00
#define ERR_PID_ENCODE      0x01
#define ERR_UNKNOWN_PID     0x02
#define ERR_UNEXPECT_PKT    0x03
#define ERR_TCRC            0x04
#define ERR_DCRC            0x05
#define ERR_TIMEOUT         0x06
#define ERR_BABBIE          0x07
#define ERR_EOF_PKT         0x08
#define ERR_TX_RX_NAK           0x09
#define ERR_SENT_STALL      0x0A
#define ERR_BUF_OVERRUN     0x0B
#define ERR_SENT_EPT_PKT    0x0C
#define ERR_BIT_STUFF       0x0D
#define ERR_SYNC            0x0E
#define ERR_TOGGLE_BIT      0x0F

#ifdef __cplusplus
}
#endif

#endif /* __HW_USBD_IP3511_H */
