/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *        All rights are reserved. Reproduction in whole or in part is
 *       prohibited without the written consent of the copyright owner.
 *   NXP reserves the right to make changes without notice at any time.
 *  NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 * particular purpose, or that the use will not infringe any third party patent,
 *  copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/**
 * \file VComInterface.h
 *
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18735 $ (v04.13.00)
 * $Date: 2016-10-19 16:12:50 +0530 (Wed, 19 Oct 2016) $
 *
 */


#ifndef VCOMINTERFACE_H_
#define VCOMINTERFACE_H_

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include "ph_Datatypes.h"


/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

phStatus_t VCom_Init(void);

uint32_t VCom_Read(uint8_t *pBuf, uint32_t buf_len);

uint32_t VCom_Write(uint8_t *pBuf, uint32_t buf_len);

#endif /* VCOMINTERFACE_H_ */
