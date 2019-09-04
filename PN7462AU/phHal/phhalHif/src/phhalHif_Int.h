/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */
/** @file
 *
 * Header for Host Interface Core
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

#ifndef PHHALHIF_INT_H
#define PHHALHIF_INT_H

/**
 * \addtogroup phhalHif  HIF HAL
 * \brief This component implements the system Host Interface Hal functionalities for I2C Slave, Spi Slave and Hif Slave
 * @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_HIF
#include "ph_Status.h"
#include "ph_Reg.h"
#include "phhalHif.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
/**
 * This definition is used to select between different interface's like HSU/I2C/SPI
 */
#define PHHAL_HIF_SELECT_INTERFACE(bInterface)         PH_REG_SET_FIELD(PCR_SYS_REG,HIF_SELECTION,bInterface)

/**
 * This definition is used to identify the currently selected interface
 */
#define PHHAL_HIF_GET_INTERFACE()                      PH_REG_GET_FIELD(PCR_SYS_REG,HIF_SELECTION)


/**
 * This definition is enable the Hif so that it shall be possible to communicate using Hif
 */
#define PHHAL_HIF_ENABLE()                       PH_REG_SET_BIT(HOSTIF_CONTROL_REG, HIF_ENABLE)


/**
 * This definition is disable the Hif so that it will not be possible to communiacte via Hif
 */
#define PHHAL_HIF_DISABLE()                      PH_REG_CLEAR_BIT(HOSTIF_CONTROL_REG, HIF_ENABLE)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * This API initializes the I2C HW for features such as Reset Enable, HS Mode Switch Enable, Device ID Enable.
 * This API also configures the last 2 bits of the 7-bit Slave Address whose msbs are 0x28
 * ie: 00b = 0x28, 01b = 0x29, 10b = 0x2A, 11b = 0x2B
 * @param Configuration Byte containing bit fields that enable/disable the features and the Slave Address.
 * @return  None
 */
extern void phhalHif_I2c_Init( uint8_t );

/**
 * This Internal API initializes the CPOL and CPHA configuration of the SPI Slave.
 * This API will be called from phhalHif_Init if the selected host interface is
 * @param Configuration byte that holds 4 possible values of CPOL/CPHA.
 */
extern void phhalHif_Spi_Init( uint8_t );

/** This API will initialize HS UART HW with EOF Indicator, number of stop bits and baudrate.
 * The baud rate can be manual or automatic using baud rate estimator or semi automatic *
 * @param Pointer to config structure used to pass HSU parameters.
 */
extern void phhalHif_Hsu_Init( phhalHif_Hsu_t );

/**
 * This is HIf Isr Routine which will be executed whenever the Hif generates an interrupt
 *
 * This API is part the Vector Table and direclty called in the event of HIF/USB ISR.
 */
extern void HIF_IRQHandler( void );

/**
 * This is HIf USB Isr Routine which will be executed whenever the USB gets an interrupt
 */
extern void phhalHifUsb_Isr( void );

#endif /* NXPBUILD__PHHAL_HIF */

/** @} */
#endif /* PHHALHIF_INT_H */
