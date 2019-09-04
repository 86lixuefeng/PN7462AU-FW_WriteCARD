/*******************************************************************************
 *
 *                Copyright (C), NXP Semiconductors
 *
 *                   (C) NXP Semiconductors
 *     All rights are reserved. Reproduction in whole or in part is
 *    prohibited without the written consent of the copyright owner.
 * NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                        arising from its use.
 *
 * *//** \file
 *
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14132 $ (v04.13.00)
 * $Date: 2015-07-27 09:47:10 +0530 (Mon, 27 Jul 2015) $
 *
 *******************************************************************************/
#ifndef PN7462AU_IP_USB_H_INCLUDED
#define PN7462AU_IP_USB_H_INCLUDED

#include "PN7462AU/PN7462AU_usb_rorw.h"



/**
* \name USB Device Command/Status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_COMMAND_STATUS                                                                       (0x40028000UL) /**< rw*/
#define USBDEV_COMMAND_STATUS_VBUSDEBOUNCED_MASK                                                    (0x10000000UL) /**< r- - This bit indicates if Vbus is detected or not. The bit raises immediately when Vbus becomes high. It drops to zero if Vbus is low for at least 3 ms.
If this bit is high and the DCon bit is set, the HW will enable the pull-up resistor to signal a connect.. */
#define USBDEV_COMMAND_STATUS_VBUSDEBOUNCED_POS                                                     (28UL)
#define USBDEV_COMMAND_STATUS_DRES_C_MASK                                                           (0x04000000UL) /**< rw - Device status - reset change
Firmware must write a one to reset the bit. */
#define USBDEV_COMMAND_STATUS_DRES_C_POS                                                            (26UL)
#define USBDEV_COMMAND_STATUS_DSUS_C_MASK                                                           (0x02000000UL) /**< rw - Device status - suspend change
Firmware must write a one to reset the bit. */
#define USBDEV_COMMAND_STATUS_DSUS_C_POS                                                            (25UL)
#define USBDEV_COMMAND_STATUS_DCON_C_MASK                                                           (0x01000000UL) /**< rw - Device status - connect change
Firmware must write a one to reset the bit. */
#define USBDEV_COMMAND_STATUS_DCON_C_POS                                                            (24UL)
#define USBDEV_COMMAND_STATUS_LPM_REWP_MASK                                                         (0x00100000UL) /**< r- - LPM Remote Wakeup Enabled by USB host. */
#define USBDEV_COMMAND_STATUS_LPM_REWP_POS                                                          (20UL)
#define USBDEV_COMMAND_STATUS_LPM_SUS_MASK                                                          (0x00080000UL) /**< rw - Device status - LPM Suspend. */
#define USBDEV_COMMAND_STATUS_LPM_SUS_POS                                                           (19UL)
#define USBDEV_COMMAND_STATUS_DSUS_MASK                                                             (0x00020000UL) /**< rw - Device status - suspend. */
#define USBDEV_COMMAND_STATUS_DSUS_POS                                                              (17UL)
#define USBDEV_COMMAND_STATUS_DCON_MASK                                                             (0x00010000UL) /**< rw - Device status - connect. */
#define USBDEV_COMMAND_STATUS_DCON_POS                                                              (16UL)
#define USBDEV_COMMAND_STATUS_INTONNAK_CI_MASK                                                      (0x00008000UL) /**< rw - Interrupt on NAK for control IN EP. */
#define USBDEV_COMMAND_STATUS_INTONNAK_CI_POS                                                       (15UL)
#define USBDEV_COMMAND_STATUS_INTONNAK_CO_MASK                                                      (0x00004000UL) /**< rw - Interrupt on NAK for control OUT EP. */
#define USBDEV_COMMAND_STATUS_INTONNAK_CO_POS                                                       (14UL)
#define USBDEV_COMMAND_STATUS_INTONNAK_AI_MASK                                                      (0x00002000UL) /**< rw - Interrupt on NAK for interrupt and bulk IN EP. */
#define USBDEV_COMMAND_STATUS_INTONNAK_AI_POS                                                       (13UL)
#define USBDEV_COMMAND_STATUS_INTONNAK_AO_MASK                                                      (0x00001000UL) /**< rw - Interrupt on NAK for interrupt and bulk OUT EP. */
#define USBDEV_COMMAND_STATUS_INTONNAK_AO_POS                                                       (12UL)
#define USBDEV_COMMAND_STATUS_LPM_SUP_MASK                                                          (0x00000800UL) /**< rw - LPM Supported. */
#define USBDEV_COMMAND_STATUS_LPM_SUP_POS                                                           (11UL)
#define USBDEV_COMMAND_STATUS_PLL_ON_MASK                                                           (0x00000200UL) /**< rw - Always PLL Clock on. */
#define USBDEV_COMMAND_STATUS_PLL_ON_POS                                                            (9UL)
#define USBDEV_COMMAND_STATUS_SETUP_MASK                                                            (0x00000100UL) /**< rw - SETUP token received
SW must clear this bit by writing a one.. */
#define USBDEV_COMMAND_STATUS_SETUP_POS                                                             (8UL)
#define USBDEV_COMMAND_STATUS_DEV_EN_MASK                                                           (0x00000080UL) /**< rw - USB device enable. */
#define USBDEV_COMMAND_STATUS_DEV_EN_POS                                                            (7UL)
#define USBDEV_COMMAND_STATUS_DEV_ADDR_MASK                                                         (0x0000007FUL) /**< rw - USB device address. */
#define USBDEV_COMMAND_STATUS_DEV_ADDR_POS                                                          (0UL)
#define USBDEV_COMMAND_STATUS__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Info
* <b>Reset value:</b> 0x00140000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_INFO                                                                                 (0x40028004UL) /**< rw*/
#define USBDEV_INFO_CHIP_ID_MASK                                                                    (0xFFFF0000UL) /**< r- - USB IP version number. */
#define USBDEV_INFO_CHIP_ID_POS                                                                     (16UL)
#define USBDEV_INFO_ERR_CODE_MASK                                                                   (0x00007800UL) /**< rw - The error code that last occurred. */
#define USBDEV_INFO_ERR_CODE_POS                                                                    (11UL)
#define USBDEV_INFO_FRAME_NR_MASK                                                                   (0x000007FFUL) /**< r- - Frame number. */
#define USBDEV_INFO_FRAME_NR_POS                                                                    (0UL)
#define USBDEV_INFO__RESET_VALUE                                                                    (0x00140000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB EP Command/Status List start address
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_EPLIST_START_ADDRESS                                                                 (0x40028008UL) /**< rw*/
#define USBDEV_EPLIST_START_ADDRESS_EP_LIST_MASK                                                    (0xFFFFFF00UL) /**< rw - These are the programmable bits for firmware to indicate the start address of the USB EP Command/Status List. */
#define USBDEV_EPLIST_START_ADDRESS_EP_LIST_POS                                                     (8UL)
#define USBDEV_EPLIST_START_ADDRESS__RESET_VALUE                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Data buffer start address
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_DATA_BUFFER_ADDRESS                                                                  (0x4002800CUL) /**< rw*/
#define USBDEV_DATA_BUFFER_ADDRESS_DA_BUF_MASK                                                      (0xFFC00000UL) /**< rw - These are the programmable bits for firmware to indicate the buffer pointer page where all endpoint data buffers are located.. */
#define USBDEV_DATA_BUFFER_ADDRESS_DA_BUF_POS                                                       (22UL)
#define USBDEV_DATA_BUFFER_ADDRESS__RESET_VALUE                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Link Power Management register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_LPM                                                                                  (0x40028010UL) /**< rw*/
#define USBDEV_LPM_LPM_DATA_PENDING_MASK                                                            (0x00000100UL) /**< rw - As long as this bit is set to one and LPM supported bit is set to one, HW will return a NYET handshake on every LPM token it receives.. */
#define USBDEV_LPM_LPM_DATA_PENDING_POS                                                             (8UL)
#define USBDEV_LPM_LPM_HIRD_SW_MASK                                                                 (0x000000F0UL) /**< rw - Host Initiated Resume Duration - SW
This is the time duration required by the USB device system to come out of LPM initiated suspend after receiving the host initiated LPM resume.. */
#define USBDEV_LPM_LPM_HIRD_SW_POS                                                                  (4UL)
#define USBDEV_LPM_LPM_HIRD_HW_MASK                                                                 (0x0000000FUL) /**< r- - Host Initiated Resume Duration - HW
This is the HIRD value from the last received LPM token. */
#define USBDEV_LPM_LPM_HIRD_HW_POS                                                                  (0UL)
#define USBDEV_LPM__RESET_VALUE                                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Endpoint skip
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_ENDPOINT_SKIP                                                                        (0x40028014UL) /**< rw*/
#define USBDEV_ENDPOINT_SKIP_SKIP_MASK                                                              (0x3FFFFFFFUL) /**< rw - Endpoint skip :
Writing 1b to one of these bits, will indicate to HW that it must deactivate the buffer assigned to this endpoint and return control back to firmware.. */
#define USBDEV_ENDPOINT_SKIP_SKIP_POS                                                               (0UL)
#define USBDEV_ENDPOINT_SKIP__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Endpoint Buffer in use
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_EP_BUFFER_INUSE                                                                      (0x40028018UL) /**< rw*/
#define USBDEV_EP_BUFFER_INUSE_BUF_MASK                                                             (0x3FFFFFFFUL) /**< rw - Buffer in use :
This register has one bit per physical endpoint.
0b : HW is accessing buffer 0
1b : HW is accessing buffer 1. */
#define USBDEV_EP_BUFFER_INUSE_BUF_POS                                                              (0UL)
#define USBDEV_EP_BUFFER_INUSE__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Endpoint Buffer Config
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_EP_BUFFER_CONFIG                                                                     (0x4002801CUL) /**< rw*/
#define USBDEV_EP_BUFFER_CONFIG_BUF_SB_MASK                                                         (0x3FFFFFFCUL) /**< rw - Buffer usage - This register has one bit per physical endpoint
0b : Single buffer
1b : Double buffer. */
#define USBDEV_EP_BUFFER_CONFIG_BUF_SB_POS                                                          (2UL)
#define USBDEV_EP_BUFFER_CONFIG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB interrupt status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_INTERRUPT_STATUS                                                                     (0x40028020UL) /**< rw*/
#define USBDEV_INTERRUPT_STATUS_DEV_INT_MASK                                                        (0x80000000UL) /**< rw - Device status interrupt. This bit is set by HW when one of the device status change bits is set.
Firmware can clear this bit by writing a one to it.. */
#define USBDEV_INTERRUPT_STATUS_DEV_INT_POS                                                         (31UL)
#define USBDEV_INTERRUPT_STATUS_FRAME_INT_MASK                                                      (0x40000000UL) /**< rw - Frame interrupt. This bit is set to one every millisecond when the VbusDebounced bit and the DCON bit are set. This bit can be used by software when handling the isochronous endpoints.
Firmware can clear this bit by writing a one to it.. */
#define USBDEV_INTERRUPT_STATUS_FRAME_INT_POS                                                       (30UL)
#define USBDEV_INTERRUPT_STATUS_EPX_INT_MASK                                                        (0x3FFFFFFFUL) /**< rw - Interrupt status register bit for the corresponding endpoint.
Firmware can clear this bit by writing a one to it.. */
#define USBDEV_INTERRUPT_STATUS_EPX_INT_POS                                                         (0UL)
#define USBDEV_INTERRUPT_STATUS__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB interrupt enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_INTERRUPT_ENABLE                                                                     (0x40028024UL) /**< rw*/
#define USBDEV_INTERRUPT_ENABLE_DEV_INT_EN_MASK                                                     (0x80000000UL) /**< rw - If this bit is set and the corresponding USB interrupt status bit is set a HW interrupt is generated on the interrupt line indicated by the corresponding USB interrupt routing bit.. */
#define USBDEV_INTERRUPT_ENABLE_DEV_INT_EN_POS                                                      (31UL)
#define USBDEV_INTERRUPT_ENABLE_FRAME_INT_EN_MASK                                                   (0x40000000UL) /**< rw - If this bit is set and the corresponding USB interrupt status bit is set a HW interrupt is generated on the interrupt line indicated by the corresponding USB interrupt routing bit.. */
#define USBDEV_INTERRUPT_ENABLE_FRAME_INT_EN_POS                                                    (30UL)
#define USBDEV_INTERRUPT_ENABLE_EPX_INT_EN_MASK                                                     (0x3FFFFFFFUL) /**< rw - If this bit is set and the corresponding USB interrupt status bit is set a HW interrupt is generated on the interrupt line indicated by the corresponding USB interrupt routing bit.. */
#define USBDEV_INTERRUPT_ENABLE_EPX_INT_EN_POS                                                      (0UL)
#define USBDEV_INTERRUPT_ENABLE__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB set interrupt status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_SET_INTERRUPT_STATUS                                                                 (0x40028028UL) /**< rw*/
#define USBDEV_SET_INTERRUPT_STATUS_DEV_INT_SET_MASK                                                (0x80000000UL) /**< rw - If firmware writes a one to one of these bits, the corresponding USB interrupt status bit is set.
When this register is read, the same value as the USB interrupt status register is returned.. */
#define USBDEV_SET_INTERRUPT_STATUS_DEV_INT_SET_POS                                                 (31UL)
#define USBDEV_SET_INTERRUPT_STATUS_FRAME_INT_SET_MASK                                              (0x40000000UL) /**< rw - If firmware writes a one to one of these bits, the corresponding USB interrupt status bit is set.
When this register is read, the same value as the USB interrupt status register is returned.. */
#define USBDEV_SET_INTERRUPT_STATUS_FRAME_INT_SET_POS                                               (30UL)
#define USBDEV_SET_INTERRUPT_STATUS_EPX_INT_SET_MASK                                                (0x3FFFFFFFUL) /**< rw - If firmware writes a one to one of these bits, the corresponding USB interrupt status bit is set.
When this register is read, the same value as the USB interrupt status register is returned.. */
#define USBDEV_SET_INTERRUPT_STATUS_EPX_INT_SET_POS                                                 (0UL)
#define USBDEV_SET_INTERRUPT_STATUS__RESET_VALUE                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB interrupt routing
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_INTERRUPT_ROUTING                                                                    (0x4002802CUL) /**< rw*/
#define USBDEV_INTERRUPT_ROUTING_DEV_INT_ROUTE_MASK                                                 (0x80000000UL) /**< rw - This bit determines to which HW interrupt line the interrupt is routed (0b = IRQ; 1b = FIQ). */
#define USBDEV_INTERRUPT_ROUTING_DEV_INT_ROUTE_POS                                                  (31UL)
#define USBDEV_INTERRUPT_ROUTING_FRAME_INT_ROUTE_MASK                                               (0x40000000UL) /**< rw - This bit determines to which HW interrupt line the interrupt is routed (0b = IRQ; 1b = FIQ). */
#define USBDEV_INTERRUPT_ROUTING_FRAME_INT_ROUTE_POS                                                (30UL)
#define USBDEV_INTERRUPT_ROUTING_EPX_INT_ROUTE_MASK                                                 (0x3FFFFFFFUL) /**< rw - This bit determines to which HW interrupt line the corresponding interrupt is routed (0b = IRQ; 1b = FIQ). */
#define USBDEV_INTERRUPT_ROUTING_EPX_INT_ROUTE_POS                                                  (0UL)
#define USBDEV_INTERRUPT_ROUTING__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Configuration
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define USBDEV_CONFIGURATION                                                                        (0x40028030UL) /**< r-*/
#define USBDEV_CONFIGURATION_TREG_MASK                                                              (0x00000080UL) /**< r- - Toggle register available. */
#define USBDEV_CONFIGURATION_TREG_POS                                                               (7UL)
#define USBDEV_CONFIGURATION_DB_MASK                                                                (0x00000040UL) /**< r- - EP Double buffer supported (derived from VHDL generic C_DOUBLE_BUFFER_SUPPORTED). */
#define USBDEV_CONFIGURATION_DB_POS                                                                 (6UL)
#define USBDEV_CONFIGURATION_SB_MASK                                                                (0x00000020UL) /**< r- - EP Single buffer supported (derived from VHDL generic C_SINGLE_BUFFER_SUPPORTED). */
#define USBDEV_CONFIGURATION_SB_POS                                                                 (5UL)
#define USBDEV_CONFIGURATION_PHYSEP_MASK                                                            (0x0000001FUL) /**< r- - Number of physical endpoints implemented in this design (excluding the default control endpoint 0)
E.g. a value of 2 indicates that the design contains the default control endpoint plus one IN endpoint (EP1 - IN) and one OUT endpoint (EP1 - OUT). */
#define USBDEV_CONFIGURATION_PHYSEP_POS                                                             (0UL)
#define USBDEV_CONFIGURATION__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Endpoint toggle
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define USBDEV_ENDPOINT_TOGGLE                                                                      (0x40028034UL) /**< r-*/
#define USBDEV_ENDPOINT_TOGGLE_TOGGLE_MASK                                                          (0x3FFFFFFFUL) /**< r- - Endpoint data toggle. */
#define USBDEV_ENDPOINT_TOGGLE_TOGGLE_POS                                                           (0UL)
#define USBDEV_ENDPOINT_TOGGLE__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Internal PLL
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_INTERNAL_PLL                                                                         (0x40028038UL) /**< rw*/
#define USBDEV_INTERNAL_PLL_SEL_EXT_CLK_MASK                                                        (0x00000001UL) /**< rw - Select external crystal clock. */
#define USBDEV_INTERNAL_PLL_SEL_EXT_CLK_POS                                                         (0UL)
#define USBDEV_INTERNAL_PLL__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name USB Test bus select register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define USBDEV_TBSEL                                                                                (0x4002803CUL) /**< rw*/
#define USBDEV_TBSEL_TESTBUS_VALUE_MASK                                                             (0x0000FF00UL) /**< r- - See testbus output sheet to see which signals are muxed to which bits depending on the value in the TESTBUS_SELECT field. */
#define USBDEV_TBSEL_TESTBUS_VALUE_POS                                                              (8UL)
#define USBDEV_TBSEL_TESTBUS_SELECT_MASK                                                            (0x00000007UL) /**< rw - Selects which set of test signals are routed to register field USBDEV_TBVALUE and output usbdev_testbus[7:0]. */
#define USBDEV_TBSEL_TESTBUS_SELECT_POS                                                             (0UL)
#define USBDEV_TBSEL__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_USB_H_INCLUDED */
