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
 * mw_usbd_cciduser.h : CCID Class User Specific Parameters
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-04 17:19:56 +0530 (Fri, 04 Sep 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15087 $ (v04.13.00)
 */
#ifndef __CCIDUSER_H__
#define __CCIDUSER_H__

#include "error.h"
#include "mw_usbd.h"
#include "mw_usbd_core.h"
#include "mw_usbd_ccid.h"
#include "app_usbd_cfg.h"

/** \file
 *  \brief  CCID Class API Structures and function prototypes.
 *
 *  Definition of functions for CCID driver.
 *
 */

/** \brief CCID Class function driver initialization parameter data structure.
 *  \ingroup USBD_CCID
 *
 *  \details  This data structure is used to pass initialization parameters to the
 *  CCID class function driver's init function.
 *
 */
typedef struct USBD_CCID_INIT_PARAM
{
   /* memory allocation params */
   uint32_t  mem_base;  /**< Base memory location from where the stack can allocate
                        data and buffers. \note The memory address set in this field
                        should be accessible by USB DMA controller. Also this value
                        should be aligned on 4 byte boundary.
                        */
   uint32_t  mem_size;  /**< The size of memory buffer which stack can use.
                        \note The \em mem_size should be greater than the size returned
                        by USBD_CCID_API::GetMemSize() routine. */

   /** Pointer to the interface descriptor within the descriptor
   * array (\em full_speed_desc) passed to Init() through \ref USB_CORE_DESCS_T
   * structure.
   */
   uint8_t*  intf_desc;

   /* User defined functions */

   /**
   *  Optional user override-able function to replace the default CCID class handler.
   *
   *  The application software could override the default EP0 class handler with their
   *  own by providing the handler function address as this data member of the parameter
   *  structure. Application which like the default handler should set this data member
   *  to zero before calling the USBD_CCID_API::Init().
   *  \n
   *  \note
   *
   *  \param[in] hUsb Handle to the USB device stack.
   *  \param[in] data Pointer to the data which will be passed when callback function is called by the stack.
   *  \param[in] event  Type of endpoint event. See \ref USBD_EVENT_T for more details.
   *  \return The call back should returns \ref ErrorCode_t type to indicate success or error condition.
   *          \retval LPC_OK On Success.
   *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
   *          \retval ERR_USBD_xxx  For Other error conditions.
   *
   */
   ErrorCode_t (*CCID_Ep0_Hdlr) (USBD_HANDLE_T hUsb, void* data, uint32_t event);

   /**
   *  Call back function for the Interrupt-IN Endpoint handler.
   *
   *  The application software should provide this handler with their own by
   *  providing the handler function address as this data member of the parameter
   *  structure.
   *  \n
   *  \note
   *
   *  \param[in] hUsb Handle to the USB device stack.
   *  \param[in] data Pointer to the data which will be passed when callback function is called by the stack.
   *  \param[in] event  Type of endpoint event. See \ref USBD_EVENT_T for more details.
   *  \return The call back should returns \ref ErrorCode_t type to indicate success or error condition.
   *          \retval LPC_OK On Success.
   *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
   *          \retval ERR_USBD_xxx  For Other error conditions.
   *
   */
   ErrorCode_t (*CCID_EpIntIn_Hdlr) (USBD_HANDLE_T hUsb, void* data, uint32_t event);

   /**
   *  Call back function for the Bulk-IN Endpoint handler.
   *
   *  The application software should provide this handler with their own by
   *  providing the handler function address as this data member of the parameter
   *  structure.
   *  \n
   *  \note
   *
   *  \param[in] hUsb Handle to the USB device stack.
   *  \param[in] data Pointer to the data which will be passed when callback function is called by the stack.
   *  \param[in] event  Type of endpoint event. See \ref USBD_EVENT_T for more details.
   *  \return The call back should returns \ref ErrorCode_t type to indicate success or error condition.
   *          \retval LPC_OK On Success.
   *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
   *          \retval ERR_USBD_xxx  For Other error conditions.
   *
   */
   ErrorCode_t (*CCID_EpBulkIn_Hdlr) (USBD_HANDLE_T hUsb, void* data, uint32_t event);

   /**
   *  Call back function for the Bulk-OUT Endpoint handler.
   *
   *  The application software should provide this handler with their own by
   *  providing the handler function address as this data member of the parameter
   *  structure.
   *  \n
   *  \note
   *
   *  \param[in] hUsb Handle to the USB device stack.
   *  \param[in] data Pointer to the data which will be passed when callback function is called by the stack.
   *  \param[in] event  Type of endpoint event. See \ref USBD_EVENT_T for more details.
   *  \return The call back should returns \ref ErrorCode_t type to indicate success or error condition.
   *          \retval LPC_OK On Success.
   *          \retval ERR_USBD_UNHANDLED  Event is not handled hence pass the event to next in line.
   *          \retval ERR_USBD_xxx  For Other error conditions.
   *
   */
   ErrorCode_t (*CCID_EpBulkOut_Hdlr) (USBD_HANDLE_T hUsb, void* data, uint32_t event);

} USBD_CCID_INIT_PARAM_T;

/** \brief CCID class API functions structure.
 *  \ingroup USBD_CCID
 *
 *  This module exposes functions which interact directly with USB device controller hardware.
 *
 */
typedef struct USBD_CCID_API
{
    /** \fn uint32_t GetMemSize(USBD_CCID_INIT_PARAM_T* param)
     *  Function to determine the memory required by the CCID Function driver module.
     *
     *  This function is called by application layer before calling mwCCID_init(), to allocate memory used
     *  by CCID function driver module. The application should allocate the memory which is accessible by USB
     *  controller/DMA controller.
     *  \note Some memory areas are not accessible by all bus masters.
     *
     *  \param[in] param Structure containing CCID function driver module initialization parameters.
     *  \return Returns the required memory size in bytes.
     */
    uint32_t (*GetMemSize)(USBD_CCID_INIT_PARAM_T* param);

    /** \fn ErrorCode_t init(USBD_HANDLE_T hUsb, USBD_CCID_INIT_PARAM_T* param)
     *  Function to initialize CCID function driver module.
     *
     *  This function is called by application layer to initialize CCID function driver module.
     *
     *  \param[in] hUsb Handle to the USB device stack.
     *  \param[in, out] param Structure containing CCID function driver module initialization parameters.
     *  \return Returns \ref ErrorCode_t type to indicate success or error condition.
     *          \retval LPC_OK On success
     *          \retval ERR_USBD_BAD_MEM_BUF  Memory buffer passed is not 4-byte
     *                  aligned or smaller than required.
     *          \retval ERR_USBD_BAD_INTF_DESC  Wrong interface descriptor is passed.
     *          \retval ERR_USBD_BAD_EP_DESC  Wrong endpoint descriptor is passed.
     */
    ErrorCode_t (*init)(USBD_HANDLE_T hUsb, USBD_CCID_INIT_PARAM_T* param);

} USBD_CCID_API_T;

/*-----------------------------------------------------------------------------
 *  Private functions & structures prototypes
 *-----------------------------------------------------------------------------*/
/** @cond  ADVANCED_API */

typedef struct _CCID_CTRL_T
{
   USB_CORE_CTRL_T*  pUsbCtrl;
   uint8_t*          smart_desc;
   uint32_t          BulkLen;               /* Bulk In/Out Length */
   uint8_t           BulkStage;             /* Bulk Stage         */
   uint8_t           if_num;                /* Interface Number   */
   uint8_t           epin_num;              /* Bulk IN endpoint number */
   uint8_t           epout_num;             /* Bulk OUT endpoint number */
   uint8_t           epint_num;             /* Interrupt IN endpoint number */
   uint32_t          MemOK;                 /* Memory OK */

} USB_CCID_CTRL_T;

/** @cond DIRECT_API */
extern uint32_t mwCCID_GetMemSize(USBD_CCID_INIT_PARAM_T* param);
extern ErrorCode_t mwCCID_init(USBD_HANDLE_T hUsb, USBD_CCID_INIT_PARAM_T* param);
/** @endcond */

/** @endcond */
#endif  /* __CCIDUSER_H__ */
