/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Type definitions for Reader Library Framework.
* $Author: Purnank H G (ing05193) $
* $Revision: 8933 $ (v04.13.00)
* $Date: 2015-01-27 14:57:13 +0530 (Tue, 27 Jan 2015) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PH_TYPEDEFS_H
#define PH_TYPEDEFS_H

#ifndef __QMORE__
#   include <string.h> /* Required for memcpy invocation by NxpRLib */
#endif

#ifndef PH_DATATYPES_H
#   include <ph_Datatypes.h>
#endif


#ifndef __handle_defined
#define __handle_defined
/**
*\brief Unsigned handle
*/
typedef uint32_t ph_NfcHandle;
#endif
/*@}*/

#endif /* PH_TYPEDEFS_H */
