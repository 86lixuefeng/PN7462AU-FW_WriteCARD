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
#ifndef PN7462AU_IP_I2CM_H_INCLUDED
#define PN7462AU_IP_I2CM_H_INCLUDED

#include "PN7462AU/PN7462AU_i2cm_rorw.h"



/**
* \name  I2C Master config register (RW)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define I2CMASTER_CONFIG_REG                                                                        (0x40030000UL) /**< rw-*/
#define I2CMASTER_CONFIG_REG_RESET_I2C_CORE_MASK                                                    (0x00000010UL) /**< -x - Set Only (Pulse) Register Bit Field to Reset the I2C Core Block.. */
#define I2CMASTER_CONFIG_REG_RESET_I2C_CORE_POS                                                     (4UL)
#define I2CMASTER_CONFIG_REG_RESET_REG_MASK                                                         (0x00000008UL) /**< -x - Set Only (Pulse) Register Bit Field to Reset Selected Registers of I2C Master. */
#define I2CMASTER_CONFIG_REG_RESET_REG_POS                                                          (3UL)
#define I2CMASTER_CONFIG_REG_FIFO_FLUSH_MASK                                                        (0x00000004UL) /**< -x - Set Only (Pulse) Register Bit Field to flush the FIFO irrespective of I2C Master Mode of operation. */
#define I2CMASTER_CONFIG_REG_FIFO_FLUSH_POS                                                         (2UL)
#define I2CMASTER_CONFIG_REG_SLV_ADDRESSING_MASK                                                    (0x00000002UL) /**< rw - I2C Slave 7/10 bit address selection. */
#define I2CMASTER_CONFIG_REG_SLV_ADDRESSING_POS                                                     (1UL)
#define I2CMASTER_CONFIG_REG_IP_MODE_MASK                                                           (0x00000001UL) /**< rw - I2C Master Mode selection. */
#define I2CMASTER_CONFIG_REG_IP_MODE_POS                                                            (0UL)
#define I2CMASTER_CONFIG_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master baudrate sellection register (RW)
* <b>Reset value:</b> 0x000000F4\n
* <b>Access:</b> rw-
* @{
*/
#define I2CMASTER_BAUDRATE_REG                                                                      (0x40030004UL) /**< rw-*/
#define I2CMASTER_BAUDRATE_REG_BAUDRATE_MASK                                                        (0x000000FFUL) /**< rw - BAUDRATE bit field is used for the I2C Serial Clock Frequency calculation.. */
#define I2CMASTER_BAUDRATE_REG_BAUDRATE_POS                                                         (0UL)
#define I2CMASTER_BAUDRATE_REG__RESET_VALUE                                                         (0x000000F4UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master SDA Hold Time Config register (RW)
* <b>Reset value:</b> 0x00000009\n
* <b>Access:</b> rw-
* @{
*/
#define I2CMASTER_SDA_HOLD_REG                                                                      (0x40030008UL) /**< rw-*/
#define I2CMASTER_SDA_HOLD_REG_SDA_HOLD_MASK                                                        (0x0000001FUL) /**< rw - SDA Hold time. */
#define I2CMASTER_SDA_HOLD_REG_SDA_HOLD_POS                                                         (0UL)
#define I2CMASTER_SDA_HOLD_REG__RESET_VALUE                                                         (0x00000009UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Slave address register (RW)
* <b>Reset value:</b> 0x0000002A\n
* <b>Access:</b> rw-
* @{
*/
#define I2CMASTER_I2C_ADDRESS_REG                                                                   (0x4003000CUL) /**< rw-*/
#define I2CMASTER_I2C_ADDRESS_REG_SLAVE_ADDRESS_MASK                                                (0x000003FFUL) /**< rw - I2C Slave 7 bit or 10 bit address. */
#define I2CMASTER_I2C_ADDRESS_REG_SLAVE_ADDRESS_POS                                                 (0UL)
#define I2CMASTER_I2C_ADDRESS_REG__RESET_VALUE                                                      (0x0000002AUL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master FIFO threshold register (RW)
* <b>Reset value:</b> 0x00000701\n
* <b>Access:</b> rw-
* @{
*/
#define I2CMASTER_FIFO_THRESHOLD_REG                                                                (0x40030010UL) /**< rw-*/
#define I2CMASTER_FIFO_THRESHOLD_REG_RXMODE_THRESHOLD_MASK                                          (0x00000700UL) /**< rw - FIFO Threshold level for Interrupt Request generation when I2C Master is configured for I2C Reception mode.. */
#define I2CMASTER_FIFO_THRESHOLD_REG_RXMODE_THRESHOLD_POS                                           (8UL)
#define I2CMASTER_FIFO_THRESHOLD_REG_TXMODE_THRESHOLD_MASK                                          (0x00000007UL) /**< rw - FIFO Threshold level for Interrupt Request generation when I2C Master is configured for I2C Transmission mode.. */
#define I2CMASTER_FIFO_THRESHOLD_REG_TXMODE_THRESHOLD_POS                                           (0UL)
#define I2CMASTER_FIFO_THRESHOLD_REG__RESET_VALUE                                                   (0x00000701UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Byte Count Config register (RW)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define I2CMASTER_BYTECOUNT_CONFIG_REG                                                              (0x40030014UL) /**< rw-*/
#define I2CMASTER_BYTECOUNT_CONFIG_REG_BYTE_COUNT_CONFIG_MASK                                       (0x000003FFUL) /**< rw - This register bit field is used to configure the number of byte to be Transmitted or Received. A maximum of 1023 byte can be transmitted or received in a frame.. */
#define I2CMASTER_BYTECOUNT_CONFIG_REG_BYTE_COUNT_CONFIG_POS                                        (0UL)
#define I2CMASTER_BYTECOUNT_CONFIG_REG__RESET_VALUE                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Byte Count Status register (R)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_BYTECOUNT_STATUS_REG                                                              (0x40030018UL) /**< r--*/
#define I2CMASTER_BYTECOUNT_STATUS_REG_BYTE_COUNT_STATUS_MASK                                       (0x000003FFUL) /**< r- - This register bit field is used to provide the status of number of byte currently Transmitted or Received. A maximum of 1023 byte can be transmitted or received in a frame.. */
#define I2CMASTER_BYTECOUNT_STATUS_REG_BYTE_COUNT_STATUS_POS                                        (0UL)
#define I2CMASTER_BYTECOUNT_STATUS_REG__RESET_VALUE                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Status register (R)
* <b>Reset value:</b> 0x0000002C\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_STATUS_REG                                                                        (0x4003001CUL) /**< r--*/
#define I2CMASTER_STATUS_REG_FIFO_LEVEL_MASK                                                        (0x00000F00UL) /**< r- - This register bit field is used to provide the status of number of byte currently Transmitted or Received. A maximum of 1023 byte can be transmitted or received in a frame.. */
#define I2CMASTER_STATUS_REG_FIFO_LEVEL_POS                                                         (8UL)
#define I2CMASTER_STATUS_REG_FIFO_EMPTY_STATUS_MASK                                                 (0x00000020UL) /**< r- - Indicates the FIFO empty condition irrespective of I2C Master Mode of operation. */
#define I2CMASTER_STATUS_REG_FIFO_EMPTY_STATUS_POS                                                  (5UL)
#define I2CMASTER_STATUS_REG_FIFO_FULL_STATUS_MASK                                                  (0x00000010UL) /**< r- - Indicates the FIFO full condition irrespective of I2C Master Mode of operation. */
#define I2CMASTER_STATUS_REG_FIFO_FULL_STATUS_POS                                                   (4UL)
#define I2CMASTER_STATUS_REG_SCL_MASK                                                               (0x00000008UL) /**< r- - Current Status of scl_a line. */
#define I2CMASTER_STATUS_REG_SCL_POS                                                                (3UL)
#define I2CMASTER_STATUS_REG_SDA_MASK                                                               (0x00000004UL) /**< r- - Current Status of sda_a line. */
#define I2CMASTER_STATUS_REG_SDA_POS                                                                (2UL)
#define I2CMASTER_STATUS_REG_I2C_MASTER_MODE_MASK                                                   (0x00000002UL) /**< r- - I2C Mode of operation. Status f rom the IP_MODE bit field of CONFIG_REG register. */
#define I2CMASTER_STATUS_REG_I2C_MASTER_MODE_POS                                                    (1UL)
#define I2CMASTER_STATUS_REG_I2C_BUS_ACTIVE_MASK                                                    (0x00000001UL) /**< r- - Indicates I2C Transmission or Reception is On-going. */
#define I2CMASTER_STATUS_REG_I2C_BUS_ACTIVE_POS                                                     (0UL)
#define I2CMASTER_STATUS_REG__RESET_VALUE                                                           (0x0000002CUL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Control register (W)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_CONTROL_REG                                                                       (0x40030020UL) /**< -w-*/
#define I2CMASTER_CONTROL_REG_I2C_ENABLE_MASK                                                       (0x00000001UL) /**< -w - Enable I2C Transmission or Reception. */
#define I2CMASTER_CONTROL_REG_I2C_ENABLE_POS                                                        (0UL)
#define I2CMASTER_CONTROL_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Testbus Selection register (RW)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define I2CMASTER_TBSEL_REG                                                                         (0x40030024UL) /**< rwm*/
#define I2CMASTER_TBSEL_REG_TESTBUS_SELECT_MASK                                                     (0x0000000FUL) /**< rw - Selects which set of test signals are routed to register TBVALUE_REG and output i2cm_testbus_obs[7:0]. */
#define I2CMASTER_TBSEL_REG_TESTBUS_SELECT_POS                                                      (0UL)
#define I2CMASTER_TBSEL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master Testbus Value register (R)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define I2CMASTER_TBVALUE_REG                                                                       (0x40030028UL) /**< r-m*/
#define I2CMASTER_TBVALUE_REG_TESTBUS_VALUE_MASK                                                    (0x000000FFUL) /**< r- - Copy of output testbus i2cm_testbus_obs[7:0]. */
#define I2CMASTER_TBVALUE_REG_TESTBUS_VALUE_POS                                                     (0UL)
#define I2CMASTER_TBVALUE_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master TX Data register (W)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_TXDATA1_REG                                                                       (0x40030040UL) /**< -w-*/
#define I2CMASTER_TXDATA1_REG_TX_DATA1_MASK                                                         (0xFFFFFFFFUL) /**< -x - Data to be transmitted. */
#define I2CMASTER_TXDATA1_REG_TX_DATA1_POS                                                          (0UL)
#define I2CMASTER_TXDATA1_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master TX Data register (W)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_TXDATA2_REG                                                                       (0x40030044UL) /**< -w-*/
#define I2CMASTER_TXDATA2_REG_TX_DATA2_MASK                                                         (0xFFFFFFFFUL) /**< -x - Data to be transmitted. */
#define I2CMASTER_TXDATA2_REG_TX_DATA2_POS                                                          (0UL)
#define I2CMASTER_TXDATA2_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master TX Data register (W)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_TXDATA3_REG                                                                       (0x40030048UL) /**< -w-*/
#define I2CMASTER_TXDATA3_REG_TX_DATA3_MASK                                                         (0xFFFFFFFFUL) /**< -x - Data to be transmitted. */
#define I2CMASTER_TXDATA3_REG_TX_DATA3_POS                                                          (0UL)
#define I2CMASTER_TXDATA3_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master TX Data register (W)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_TXDATA4_REG                                                                       (0x4003004CUL) /**< -w-*/
#define I2CMASTER_TXDATA4_REG_TX_DATA4_MASK                                                         (0xFFFFFFFFUL) /**< -x - Data to be transmitted. */
#define I2CMASTER_TXDATA4_REG_TX_DATA4_POS                                                          (0UL)
#define I2CMASTER_TXDATA4_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master RX Data register (R)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_RXDATA1_REG                                                                       (0x40030050UL) /**< r--*/
#define I2CMASTER_RXDATA1_REG_RX_DATA1_MASK                                                         (0xFFFFFFFFUL) /**< r- - Data received. */
#define I2CMASTER_RXDATA1_REG_RX_DATA1_POS                                                          (0UL)
#define I2CMASTER_RXDATA1_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master RX Data register (R)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_RXDATA2_REG                                                                       (0x40030054UL) /**< r--*/
#define I2CMASTER_RXDATA2_REG_RX_DATA2_MASK                                                         (0xFFFFFFFFUL) /**< r- - Data received. */
#define I2CMASTER_RXDATA2_REG_RX_DATA2_POS                                                          (0UL)
#define I2CMASTER_RXDATA2_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master RX Data register (R)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_RXDATA3_REG                                                                       (0x40030058UL) /**< r--*/
#define I2CMASTER_RXDATA3_REG_RX_DATA3_MASK                                                         (0xFFFFFFFFUL) /**< r- - Data received. */
#define I2CMASTER_RXDATA3_REG_RX_DATA3_POS                                                          (0UL)
#define I2CMASTER_RXDATA3_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name  I2C Master RX Data register (R)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_RXDATA4_REG                                                                       (0x4003005CUL) /**< r--*/
#define I2CMASTER_RXDATA4_REG_RX_DATA4_MASK                                                         (0xFFFFFFFFUL) /**< r- - Data received. */
#define I2CMASTER_RXDATA4_REG_RX_DATA4_POS                                                          (0UL)
#define I2CMASTER_RXDATA4_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C Master interrupt clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_INT_CLR_ENABLE_REG                                                                (0x40033FD8UL) /**< -w-*/
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_TX_FIFO_THRES_MASK                                  (0x00000800UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_TX_FIFO_THRES_POS                                   (11UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_RX_FIFO_THRES_MASK                                  (0x00000400UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_RX_FIFO_THRES_POS                                   (10UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_FIFO_EMPTY_MASK                                     (0x00000200UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_FIFO_EMPTY_POS                                      (9UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_FIFO_FULL_MASK                                      (0x00000100UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_FIFO_FULL_POS                                       (8UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_I2C_BUS_ERROR_MASK                                  (0x00000008UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_I2C_BUS_ERROR_POS                                   (3UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_NACK_MASK                                           (0x00000004UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_NACK_POS                                            (2UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_ARB_FAILURE_MASK                                    (0x00000002UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_ARB_FAILURE_POS                                     (1UL)
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_TRN_COMPLETED_MASK                                  (0x00000001UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_CLR_ENABLE_REG_CLR_ENABLE_TRN_COMPLETED_POS                                   (0UL)
#define I2CMASTER_INT_CLR_ENABLE_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C Master interrupt enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_INT_SET_ENABLE_REG                                                                (0x40033FDCUL) /**< -w-*/
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_TX_FIFO_THRES_MASK                                  (0x00000800UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_TX_FIFO_THRES_POS                                   (11UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_RX_FIFO_THRES_MASK                                  (0x00000400UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_RX_FIFO_THRES_POS                                   (10UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_FIFO_EMPTY_MASK                                     (0x00000200UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_FIFO_EMPTY_POS                                      (9UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_FIFO_FULL_MASK                                      (0x00000100UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_FIFO_FULL_POS                                       (8UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_I2C_BUS_ERROR_MASK                                  (0x00000008UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_I2C_BUS_ERROR_POS                                   (3UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_NACK_MASK                                           (0x00000004UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_NACK_POS                                            (2UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_ARB_FAILURE_MASK                                    (0x00000002UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_ARB_FAILURE_POS                                     (1UL)
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_TRN_COMPLETED_MASK                                  (0x00000001UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define I2CMASTER_INT_SET_ENABLE_REG_SET_ENABLE_TRN_COMPLETED_POS                                   (0UL)
#define I2CMASTER_INT_SET_ENABLE_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C Master interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_INT_STATUS_REG                                                                    (0x40033FE0UL) /**< r--*/
#define I2CMASTER_INT_STATUS_REG_TX_FIFO_THRES_MASK                                                 (0x00000800UL) /**< r- - Indicates that the FIFO threshold is reached while I2C transmission is on-going. */
#define I2CMASTER_INT_STATUS_REG_TX_FIFO_THRES_POS                                                  (11UL)
#define I2CMASTER_INT_STATUS_REG_RX_FIFO_THRES_MASK                                                 (0x00000400UL) /**< r- - Indicates that the FIFO threshold is reached while I2C reception is on-going. */
#define I2CMASTER_INT_STATUS_REG_RX_FIFO_THRES_POS                                                  (10UL)
#define I2CMASTER_INT_STATUS_REG_FIFO_EMPTY_MASK                                                    (0x00000200UL) /**< r- - Indicates that the FIFO empty condition is reached while I2C transmission is on-going. */
#define I2CMASTER_INT_STATUS_REG_FIFO_EMPTY_POS                                                     (9UL)
#define I2CMASTER_INT_STATUS_REG_FIFO_FULL_MASK                                                     (0x00000100UL) /**< r- - Indicates that the FIFO full condition is reached while I2C reception is on-going. */
#define I2CMASTER_INT_STATUS_REG_FIFO_FULL_POS                                                      (8UL)
#define I2CMASTER_INT_STATUS_REG_I2C_BUS_ERROR_MASK                                                 (0x00000008UL) /**< r- - Indicates an I2C bus error occurred. */
#define I2CMASTER_INT_STATUS_REG_I2C_BUS_ERROR_POS                                                  (3UL)
#define I2CMASTER_INT_STATUS_REG_NACK_MASK                                                          (0x00000004UL) /**< r- - Indicates an I2C slave didn’t acknowledge the I2C master request. */
#define I2CMASTER_INT_STATUS_REG_NACK_POS                                                           (2UL)
#define I2CMASTER_INT_STATUS_REG_ARB_FAILURE_MASK                                                   (0x00000002UL) /**< r- - Indicates an I2C master arbitration failure. */
#define I2CMASTER_INT_STATUS_REG_ARB_FAILURE_POS                                                    (1UL)
#define I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_MASK                                                 (0x00000001UL) /**< r- - Indicates an I2C master completed the I2C transmission or I2C reception. */
#define I2CMASTER_INT_STATUS_REG_TRN_COMPLETED_POS                                                  (0UL)
#define I2CMASTER_INT_STATUS_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C Master interrupt enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define I2CMASTER_INT_ENABLE_REG                                                                    (0x40033FE4UL) /**< r--*/
#define I2CMASTER_INT_ENABLE_REG_ENABLE_TX_FIFO_THRES_MASK                                          (0x00000800UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_TX_FIFO_THRES_POS                                           (11UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_RX_FIFO_THRES_MASK                                          (0x00000400UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_RX_FIFO_THRES_POS                                           (10UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_FIFO_EMPTY_MASK                                             (0x00000200UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_FIFO_EMPTY_POS                                              (9UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_FIFO_FULL_MASK                                              (0x00000100UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_FIFO_FULL_POS                                               (8UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_I2C_BUS_ERROR_MASK                                          (0x00000008UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_I2C_BUS_ERROR_POS                                           (3UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_NACK_MASK                                                   (0x00000004UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_NACK_POS                                                    (2UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_ARB_FAILURE_MASK                                            (0x00000002UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_ARB_FAILURE_POS                                             (1UL)
#define I2CMASTER_INT_ENABLE_REG_ENABLE_TRN_COMPLETED_MASK                                          (0x00000001UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define I2CMASTER_INT_ENABLE_REG_ENABLE_TRN_COMPLETED_POS                                           (0UL)
#define I2CMASTER_INT_ENABLE_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C Master interrupt status clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_INT_CLR_STATUS_REG                                                                (0x40033FE8UL) /**< -w-*/
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_TX_FIFO_THRES_MASK                                  (0x00000800UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_TX_FIFO_THRES_POS                                   (11UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_RX_FIFO_THRES_MASK                                  (0x00000400UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_RX_FIFO_THRES_POS                                   (10UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_FIFO_EMPTY_MASK                                     (0x00000200UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_FIFO_EMPTY_POS                                      (9UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_FIFO_FULL_MASK                                      (0x00000100UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_FIFO_FULL_POS                                       (8UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_I2C_BUS_ERROR_MASK                                  (0x00000008UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_I2C_BUS_ERROR_POS                                   (3UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_NACK_MASK                                           (0x00000004UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_NACK_POS                                            (2UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_ARB_FAILURE_MASK                                    (0x00000002UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_ARB_FAILURE_POS                                     (1UL)
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_TRN_COMPLETED_MASK                                  (0x00000001UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_CLR_STATUS_REG_CLR_STATUS_TRN_COMPLETED_POS                                   (0UL)
#define I2CMASTER_INT_CLR_STATUS_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C Master interrupt status set register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define I2CMASTER_INT_SET_STATUS_REG                                                                (0x40033FECUL) /**< -w-*/
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_TX_FIFO_THRES_MASK                                  (0x00000800UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_TX_FIFO_THRES_POS                                   (11UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_RX_FIFO_THRES_MASK                                  (0x00000400UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_RX_FIFO_THRES_POS                                   (10UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_FIFO_EMPTY_MASK                                     (0x00000200UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_FIFO_EMPTY_POS                                      (9UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_FIFO_FULL_MASK                                      (0x00000100UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_FIFO_FULL_POS                                       (8UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_I2C_BUS_ERROR_MASK                                  (0x00000008UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_I2C_BUS_ERROR_POS                                   (3UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_NACK_MASK                                           (0x00000004UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_NACK_POS                                            (2UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_ARB_FAILURE_MASK                                    (0x00000002UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_ARB_FAILURE_POS                                     (1UL)
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_TRN_COMPLETED_MASK                                  (0x00000001UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define I2CMASTER_INT_SET_STATUS_REG_SET_STATUS_TRN_COMPLETED_POS                                   (0UL)
#define I2CMASTER_INT_SET_STATUS_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_I2CM_H_INCLUDED */
