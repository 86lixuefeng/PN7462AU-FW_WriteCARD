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
 * phExHif_Hw.h: phExHif hardware configurations, Api signatures and other declarations.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-09-18 14:24:22 +0530 (Fri, 18 Sep 2015) $
 * $Author: Karthik Ramachandra (nxp84320) $
 * $Revision: 15348 $ (v05.19.00)
 */

#ifndef PHEXHIF_HW_H
#define PHEXHIF_HW_H

/** \defgroup Example HIF application abstraction layer.
 *
 * \brief This module briefs all the prototypes of Example HIF application.
 *
 * This module implements supporting API's for Example HIF application.
 * This module is not designed for multi user/process environment.
 *  @{
 */

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "phpalCt.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * Ingress Ports
 * Gpio's 4 & 5 are used to select the Hif interface.
 *
 * GPIO5  GPIO4
 *   0      0         => Invalid
 *   0      1         => I2C
 *   1      0         => SPI
 *   1      1         => HSU
 */
#define PH_EXHIF_HW_GPIO_IN_PORT_L        4
#define PH_EXHIF_HW_GPIO_IN_PORT_H        5

/**
 * Egress Ports
 * Gpio's 6, 7 & 8 are used to select the communication channels.
 *
 * GPIO8  GPIO7  GPIO6
 *   0      0      0    => Loop back operation.
 *   0      0      1    => Send across I2CM.
 *   0      1      0    => Send across SPIM
 *   0      1      1    => Send across I2CM as well as SPIM.
 *   1      0      0    => EEPROM Write
 *   1      0      1    => Flash Write
 *   1      1      0    => CT Application
 *   1      1      1    => RFU
 */
#define PH_EXHIF_HW_GPIO_COMM_PORT_L        6
#define PH_EXHIF_HW_GPIO_COMM_PORT_M        7
#define PH_EXHIF_HW_GPIO_COMM_PORT_H        8

/*
 * Communication modes supported depending on Egress Ports.
 */
#define PH_EXHIF_HW_OUT_PORT_LOOPBACK           0
#define PH_EXHIF_HW_OUT_PORT_I2CM               1
#define PH_EXHIF_HW_OUT_PORT_SPIM               2
#define PH_EXHIF_HW_OUT_PORT_I2CM_SPIM          3
#define PH_EXHIF_HW_OUT_PORT_EEPROM             4
#define PH_EXHIF_HW_OUT_PORT_FLASH              5
#define PH_EXHIF_HW_OUT_PORT_CT                 6
#define PH_EXHIF_HW_OUT_PORT_PERFORMANCE        7

/* #warning "Define proper system clock, default it is set to 20 MHz !!!." */
#define PH_EXHIF_HW_SYSTEM_CORE_CLOCK       (20000000)

#define PH_EXHIF_HW_CRYSTAL_CLK             (27120000)
#define PH_EXHIF_HW_CRYSTAL_CLK_MHZ     ((uint32_t)PH_EXHIF_HW_CRYSTAL_CLK/1000000)

#define PH_EXHIF_HW_I2CM_TX_FIFO_THRES      0x02   /* Optimum Tx Threshold*/
#define PH_EXHIF_HW_I2CM_RX_FIFO_THRES      0x06   /* Optimum Rx Threshold*/
#define PH_EXHIF_HW_I2CM_TIMEOUT            0xFFFFFFFFUL
#define PH_EXHIF_HW_I2CM_RETRY_CNT          5

#define PH_EXHIF_HW_SPIM_TIMEOUT            0xFFFFFFFFUL
#define PH_EXHIF_HW_SPIM_SLAVE              0
#define PH_EXHIF_HW_SPIM_INIT_CRC           0 /* Flag to initialize internal CRC */
#define PH_EXHIF_HW_SPIM_APPEND_CRC         0 /* Flag to say appending of CRC is to be done for Tx bytes
                                              or not, if 1 then CRC is to be appended else not. */
#define PH_EXHIF_HW_SPIM_CRC_INIT           0 /* Transmit CRC initial value. */
#define PH_EXHIF_HW_SPIM_CRC_OFFSET         0 /* Number of bytes to be skipped for CRC computation. */
#define PH_EXHIF_HW_SPIM_MSB_FIRST          1 /* MSB or LSB selection flag. if true MSB transmitted first. */
#define PH_EXHIF_HW_SPIM_NSSPULSE           1 /* NSS pulse control. If false no pulse is generated between the bytes. */
#define PH_EXHIF_HW_SPIM_NSSPOL             0 /* NSS polarity control. If false the polarity is low. */

/**
 * Returns the Gpio status.
 * bGpioNumber : Gpio number which ranges form 1 to 12.
 */
#define PH_EXHIF_HW_GET_GPIO_VAL(bGpioNumber)    PH_REG_TEST_BITN_SC(PCR_PADIN_REG,                    \
                                                  (PCR_PADIN_REG_PADIN_DWL_REQ_POS + (bGpioNumber) ))

/* Performance Captures*/
#define PH_EXHIF_HW_CLEAR_GPIO(bGpioNumber)      PH_REG_CLEAR_BITN(PCR_PADOUT_REG,     \
                                                    (PCR_PADOUT_REG_PADOUT_DWL_REQ_POS + (bGpioNumber)))

#define PH_EXHIF_HW_SET_GPIO(bGpioNumber)        PH_REG_SET_BITN(PCR_PADOUT_REG,   \
                                                (PCR_PADOUT_REG_PADOUT_DWL_REQ_POS + (bGpioNumber)))

/**
 * Configure the Maximum buffer size to be used in Application
 */
#define PH_EXHIF_HW_BUFFER_SIZE                 256 /* 256*4*4, 1024 bytes. */

#define PH_EXHIF_HW_SYS_CLK_MHZ                 ((uint32_t)PH_EXHIF_SYSTEM_CORE_CLOCK/1000000)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/** Global buffer used for HIF data reception */
extern uint32_t gphExHif_Hw_RxBuffer[PH_EXHIF_HW_BUFFER_SIZE];

/** Handle for the events */
extern phRtos_EventHandle_t gphExHif_Hw_HifEvents;

/** Variable to check for CT initialization */
extern uint8_t gphExHif_Hw_Ct_Init;

/** Structure to store card params. */
extern phpalCt_DataParams_t gphExHif_Hw_DataParams;

/** CT ATR buffer */
extern uint8_t gphExHif_Hw_AtrBuffer[32];
/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
/**
 * This API Initializes the Ingress port(i.e Host interface) depending on the GPIO4 and GPIO5 value set by Host.
 *
 * @return Ingress port
 *
 * @retval #return values Read from GPIO's
 */
uint8_t phExHif_Hw_InPort_Init(void);

/**
 * This API Initializes the Egress port depending on the GPIO6, GPIO7 and GPIO8 value set by Host.
 *
 * @return Egress port
 *
 * @retval #return values Read from GPIO's
 *
 */
uint8_t phExHif_Hw_OutPort_Init(void);

/**
 * This API DeInitializes the Egress port depending on the GPIO6, GPIO7 and GPIO8 value set by Host.
 *
 * @param[in] bCommChannel Defines the Egress Port selected for communication.
 *
 * @retval #PH_ERR_SUCCESS If DeInitialization is completed successfully.
 */
uint8_t phExHif_Hw_DeInit(uint8_t bCommChannel);

/**
 * This API tranmits the data over the selected Egress port.
 *
 * @param[in] bCommChannel Defines the Egress Port selected for communication.
 * @param[in] bHifInterface Defines the Ingress Port selected for communication.
 * @param[in] wLength Defines the length of data to be transmitted over the Egress Port.
 * @param[in] pdwData     Defines the pointer to the Tx buffer
 * @return Status of the Operation
 *
 * @retval #PH_ERR_SUCCESS If transmission is completed successfully.
 * @retval #PH_ERR_ERROR - In case of TRANSMIT failure
 */
phStatus_t phExHif_Hw_Transmit(uint8_t bCommChannel, uint8_t bHifInterface,
        uint16_t wLength, uint32_t *pdwData);

/**
 * This API Received the data over the selected Ingress Port
 *
 * @param[in] pwLength     Defines the pointer to the length of the received buffer.
 * @return    Status of the Operation
 *
 * @retval #PH_ERR_SUCCESS If transmission is completed successfully.
 * @retval #PH_ERR_ERROR - In case of TRANSMIT failure
 */
phStatus_t phExHif_Hw_HostInterface_Receive(uint16_t* pwLength);

#endif /* PHEXHIF_HW_H */
