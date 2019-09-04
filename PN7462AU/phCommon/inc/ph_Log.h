/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * ph_Log.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-10-28 13:26:49 +0530 (Wed, 28 Oct 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 16050 $ (v04.13.00)
 */


#ifndef PH_LOG_H_
#define PH_LOG_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#ifdef __CODE_RED
#   ifndef PHFL_LOG_ENABLE
#       define PHFL_LOG_ENABLE 1
#   endif /* PHFL_LOG_ENABLE */
#endif

#ifdef __ICCARM__
#   ifndef PHFL_LOG_ENABLE
#       define PHFL_LOG_ENABLE 1
#   endif /* PHFL_LOG_ENABLE */
#endif


#ifdef PHFL_ENABLE_STANDBY
#       undef PHFL_LOG_ENABLE
#endif

#ifdef NDEBUG
#   undef PHFL_LOG_ENABLE
#endif

#ifndef PHFL_LOG_ENABLE
#   define PHFL_LOG_ENABLE 0
#endif

#ifndef PHFL_LOG_VERBOSE
#   define PHFL_LOG_VERBOSE 0
#endif


#if PHFL_LOG_ENABLE
#   if PHFL_LOG_VERBOSE
        /** Log text */
#       define LOG_TXT(TXT)     printf("%s:\t%d:\t%s",__FUNCTION__,__LINE__,(TXT))
        /** Log 32bit integer value */
#       define LOG_U32(TXT,U32) printf("%s:\t%d:\t%s=%d\n",__FUNCTION__,__LINE__,(TXT),(U32))
        /** Log 32bit hex value */
#       define LOG_X32(TXT,X32) printf("%s:\t%d:\t%s=0x%X\n",__FUNCTION__,__LINE__,(TXT),(X32))

#   else                        /* PHFL_LOG_VERBOSE */
#       define LOG_TXT(TXT)     printf((const char *)(TXT))
#       define LOG_U32(TXT,U32) printf("\t%s=%d\n",(const char *)(TXT),(U32))
#       define LOG_X32(TXT,X32) printf("\t%s=0x%X\n",(const char *)(TXT),(X32))
#   endif  /* PHFL_LOG_VERBOSE */

#   define     LOG_STATUS(status, TXT)   if(status == PH_ERR_SUCCESS){ printf("%s : PASS\n", TXT); } \
                                            else{ printf("%s : FAILURE(0x%04X)\n", TXT,status); }
#   define     LOG_ON_FAILURE(status, TXT)   \
        if(status != PH_ERR_SUCCESS){ printf("%s : FAILURE(0x%04X)\n", TXT,status); }

    /**
     *
     * @param[in] AU8 Array of uint8_t values
     * @param[in] LU8 Length of LU8
     */
#   define LOG_AU8(TXT,AU8,LU8) do {            \
                LOG_TXT(TXT);                   \
                phUser_Log_AU8((uint8_t * )(AU8),(LU8));    \
    } while (0)

#else                           /* PHFL_LOG_ENABLE */
#    define LOG_TXT(TXT)
#                               define LOG_U32(TXT,U32)
#    define LOG_X32(TXT,U32)
#                               define LOG_AU8(TXT,AU8,LU8)
#    define LOG_STATUS(status, TXT)
#    define LOG_ON_FAILURE(status, TXT)
#endif

#   define LOG_TXT_E(TXT)       LOG_TXT(TXT)
/* Logging for Debug Mode */
#if PHFL_LOG_VERBOSE
#   define LOG_TXT_D(TXT)       LOG_TXT(TXT)
#   define LOG_AU8_D(TXT,AU8,LU8)  LOG_AU8(TXT,AU8,LU8)
#   define LOG_X32_D(TXT,U32)  LOG_X32(TXT,U32)
#else
#   define LOG_TXT_D(TXT)
#   define LOG_AU8_D(TXT,AU8,LU8)
#   define LOG_X32_D(TXT,U32)
#endif


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
#if PHFL_LOG_ENABLE
    void  phUser_Log_AU8(uint8_t * inBuf, const uint32_t inBufLen);
#endif /* PHFL_LOG_ENABLE */

#endif /* PH_LOG_H_ */
