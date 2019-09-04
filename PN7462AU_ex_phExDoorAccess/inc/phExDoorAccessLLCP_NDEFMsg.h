/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
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
 * phExLLCP_NDEFMsg.h: phExLLCP application NDEF messages.
 *
 * Project:  PN7462AU
 *
 * $Date:
 * $Author:
 * $Revision:
 */

#ifndef PHEXLLCP_NDEFMSG_H
#define PHEXLLCP_NDEFMSG_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#define PHFL_EXLLCP_MSG_NXPCOM 1
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
#if PHFL_EXLLCP_MSG_NXPCOM
#   define PH_EXLLCP_NDEFMSG {                              \
        0xC1,                   /* NDEF Record Structure */ \
        0x01,                   /* TYPE LENGTH */           \
        0x00, 0x00, 0x00, 7 + 1, /* PAYLOAD LENGTH */       \
        0x55,                   /* TYPE TEXT */             \
        0x01,                   /* ID UTF8 */               \
        'n','x','p','.','c','o','m'                         \
        }
#endif


/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

#endif /* PHEXLLCP_NDEFMSG_H */
