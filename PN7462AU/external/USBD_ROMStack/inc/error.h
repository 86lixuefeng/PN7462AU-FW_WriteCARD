/*
%
% Copyright 2012 NXP Semiconductors,
% 411 E Plumeria Dr San Jose CA USA
% All rights are reserved. Reproduction in whole or in part is prohibited
% without the prior written consent of the copyright owner.
%
*/

/*
+++ IDENTIFICATION

PACKAGE           :
COMPONENT         :
INTERFACE         :
TEMPLATE VERSION  : 3
*/
#ifndef __LPC_ERROR_H__
#define __LPC_ERROR_H__

/** \file
 *  \brief Error code returned by USB drivers/library functions.
 *  \ingroup Common
 *
 *  This file contains unified error codes to be used across driver,
 *  middleware, applications, hal and demo software.
 *
 */

/** Error code returned by USB drivers/library functions
*
*  Error codes are a 32-bit value with :
*      - The 16 MSB contains the peripheral code number
*      - The 16 LSB contains an error code number associated to that peripheral
*
*/

typedef enum
{
  /**\b 0x00000000*/ LPC_OK=0, /**< enum value returned on Success */
  /**\b 0xFFFFFFFF*/ ERR_FAILED = -1, /**< enum value returned on general failure */
  /**\b 0xFFFFFFFE*/ ERR_TIME_OUT = -2, /**< enum value returned on general timeout */
  /**\b 0xFFFFFFFD*/ ERR_BUSY = -3, /**< enum value returned when resource is busy */

   /* API related errors */
  ERR_API_BASE = 0x00010000,
  /**\b 0x00010001*/ ERR_API_INVALID_PARAMS = ERR_API_BASE + 1, /**< Invalid parameters*/
  /**\b 0x00010002*/ ERR_API_INVALID_PARAM1, /**< PARAM1 is invalid */
  /**\b 0x00010003*/ ERR_API_INVALID_PARAM2, /**< PARAM2 is invalid */
  /**\b 0x00010004*/ ERR_API_INVALID_PARAM3, /**< PARAM3 is invalid */
  /**\b 0x00010005*/ ERR_API_MOD_INIT, /**< API is called before module init */

  /* USB device stack related errors */
  ERR_USBD_BASE = 0x00040000,
  /**\b 0x00040001*/ ERR_USBD_INVALID_REQ = ERR_USBD_BASE + 1, /**< invalid request */
  /**\b 0x00040002*/ ERR_USBD_UNHANDLED, /**< Callback did not process the event */
  /**\b 0x00040003*/ ERR_USBD_STALL,     /**< Stall the endpoint on which the call back is called */
  /**\b 0x00040004*/ ERR_USBD_SEND_ZLP,  /**< Send ZLP packet on the endpoint on which the call back is called */
  /**\b 0x00040005*/ ERR_USBD_SEND_DATA, /**< Send data packet on the endpoint on which the call back is called */
  /**\b 0x00040006*/ ERR_USBD_BAD_DESC,  /**< Bad descriptor*/
  /**\b 0x00040007*/ ERR_USBD_BAD_CFG_DESC,/**< Bad config descriptor*/
  /**\b 0x00040008*/ ERR_USBD_BAD_INTF_DESC,/**< Bad interface descriptor*/
  /**\b 0x00040009*/ ERR_USBD_BAD_EP_DESC,/**< Bad endpoint descriptor*/
  /**\b 0x0004000a*/ ERR_USBD_BAD_MEM_BUF, /**< Bad alignment of buffer passed. */
  /**\b 0x0004000b*/ ERR_USBD_TOO_MANY_CLASS_HDLR /**< Too many class handlers. */

} ErrorCode_t;


#ifndef offsetof
#   define offsetof(s,m)   (int)&(((s *)0)->m)
#endif
#define COMPILE_TIME_ASSERT(pred)    switch(0){case 0:case pred:;}

//define common callback function type
typedef void  (*CALLBK_T) (unsigned int res0, unsigned int res1 );
#endif /* __LPC_ERROR_H__ */
