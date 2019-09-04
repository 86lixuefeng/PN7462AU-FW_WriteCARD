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
 * phExPos_SPIHIF.h:  SPI Api signatures and other declarations.
 *
 *
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 15:33:37 +0530 (Thu, 15 Sep 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18648 $ (v05.19.00)
 */

#ifndef PHEXPOS_SPIHIF_H
#define PHEXPOS_SPIHIF_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ph_Datatypes.h>
#include <ph_Status.h>


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
void phExPos_SPIHIFPosConfigSPIHIF(void);
phStatus_t phExPos_SPIHIFSPI_Init(void);
void phExPos_SPIHIFSPISlaveErrorCallBack(uint32_t dwIntStatus, void *fptr);
void phExPos_SPIHIFSPIRxCallBack(uint32_t dwIntStatus, void *fptr);
void phExPos_SPIHIFSPITxCallback(uint32_t dwIntStatus, void *fptr);
void phExPos_SPIHIFGPIOLineIdle(void);
void phExPos_SPIHIFGPIOLineBusy(void);
void phExPos_SPIHIFStubSendAPDUResponse(uint8_t cmd);
void phExPos_SPIHIFSPIPostEventCLTask(uint32_t dwEvent);
void phExPos_SPIHIFSPIPostEventCTTask(uint32_t dwEvent);
uint8_t * phExPos_SPIHIFGet_Output_Payload_Buffer(void);

#endif /* PHEXPOS_SPIHIF_H */

