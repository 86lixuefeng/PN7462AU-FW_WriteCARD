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
#ifndef PN7462AU_IP_SPIM_H_INCLUDED
#define PN7462AU_IP_SPIM_H_INCLUDED

#include "PN7462AU/PN7462AU_spim_rorw.h"



/**
* \name  SPIM Status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define SPIM_STATUS_REG                                                                             (0x40034000UL) /**< r--*/
#define SPIM_STATUS_REG_TX_ONGOING_MASK                                                             (0x00000002UL) /**< r- - 1 - TX buffer is currently in use by the HW.. */
#define SPIM_STATUS_REG_TX_ONGOING_POS                                                              (1UL)
#define SPIM_STATUS_REG_RX_ONGOING_MASK                                                             (0x00000001UL) /**< r- - 1 - RX buffer is currently in use by the HW.. */
#define SPIM_STATUS_REG_RX_ONGOING_POS                                                              (0UL)
#define SPIM_STATUS_REG__RESET_VALUE                                                                (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Configuration of SPI Master
* <b>Reset value:</b> 0x00000002\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_CONFIG_REG                                                                             (0x40034004UL) /**< rw-*/
#define SPIM_CONFIG_REG_BAUDRATE_MASK                                                               (0x00070000UL) /**< rw - SCK frequency selction. */
#define SPIM_CONFIG_REG_BAUDRATE_POS                                                                (16UL)
#define SPIM_CONFIG_REG_SLAVE_SELECT_MASK                                                           (0x00000100UL) /**< rw - If set, Slave 1 is selected.  If cleared, Slave 0 is selected.. */
#define SPIM_CONFIG_REG_SLAVE_SELECT_POS                                                            (8UL)
#define SPIM_CONFIG_REG_MSB_FIRST_MASK                                                              (0x00000080UL) /**< rw - If set, MS bit of a byte is transmitted & received first. */
#define SPIM_CONFIG_REG_MSB_FIRST_POS                                                               (7UL)
#define SPIM_CONFIG_REG_CPHA_MASK                                                                   (0x00000040UL) /**< rw - 1 = Sampling of data occurs at even edges (2,4,6,...,16) of the SCK clock                                                                          0 = Sampling of data occurs at odd edges (1,3,5,...,15) of the SCK clock. */
#define SPIM_CONFIG_REG_CPHA_POS                                                                    (6UL)
#define SPIM_CONFIG_REG_CPOL_MASK                                                                   (0x00000020UL) /**< rw - 1 = Active-low clocks selected. In idle state SCK is high.        0 = Active-high clocks selected. In idle state SCK is low           . */
#define SPIM_CONFIG_REG_CPOL_POS                                                                    (5UL)
#define SPIM_CONFIG_REG_NSS_VAL_MASK                                                                (0x00000010UL) /**< rw - Value to ouput to nss if NSS_CTRL=1. */
#define SPIM_CONFIG_REG_NSS_VAL_POS                                                                 (4UL)
#define SPIM_CONFIG_REG_NSS_CTRL_MASK                                                               (0x00000008UL) /**< rw - 1: override NSS value with NSS_VAL. */
#define SPIM_CONFIG_REG_NSS_CTRL_POS                                                                (3UL)
#define SPIM_CONFIG_REG_NSS_PULSE_MASK                                                              (0x00000002UL) /**< rw - 1: a pulse on NSS is generated between 2 bytes. */
#define SPIM_CONFIG_REG_NSS_PULSE_POS                                                               (1UL)
#define SPIM_CONFIG_REG_NSS_POLARITY_MASK                                                           (0x00000001UL) /**< rw - 0: NSS active low. */
#define SPIM_CONFIG_REG_NSS_POLARITY_POS                                                            (0UL)
#define SPIM_CONFIG_REG__RESET_VALUE                                                                (0x00000002UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM RX/TX Control
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define SPIM_CONTROL_REG                                                                            (0x40034008UL) /**< -w-*/
#define SPIM_CONTROL_REG_TX_SET_CRC_MASK                                                            (0x00000008UL) /**< -x - Flag to init internal CRC to TX_CRC_INIT. Automatically returns to 0. */
#define SPIM_CONTROL_REG_TX_SET_CRC_POS                                                             (3UL)
#define SPIM_CONTROL_REG_TX_START_MASK                                                              (0x00000004UL) /**< -x - 1: Start Tx. Automatically returns to 0. */
#define SPIM_CONTROL_REG_TX_START_POS                                                               (2UL)
#define SPIM_CONTROL_REG_RX_SET_CRC_MASK                                                            (0x00000002UL) /**< -x - Flag to init internal CRC to RX_CRC_INIT. Automatically returns to 0. */
#define SPIM_CONTROL_REG_RX_SET_CRC_POS                                                             (1UL)
#define SPIM_CONTROL_REG_RX_START_MASK                                                              (0x00000001UL) /**< -x - 1: Start Rx. Automatically returns to 0. */
#define SPIM_CONTROL_REG_RX_START_POS                                                               (0UL)
#define SPIM_CONTROL_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Configuration of RX buffer
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_RX_BUFFER_REG                                                                          (0x4003400CUL) /**< rw-*/
#define SPIM_RX_BUFFER_REG_RX_LENGTH_MASK                                                           (0x007FC000UL) /**< rw - Size of Rx transfer. */
#define SPIM_RX_BUFFER_REG_RX_LENGTH_POS                                                            (14UL)
#define SPIM_RX_BUFFER_REG_RX_START_ADDR_MASK                                                       (0x00003FFFUL) /**< rw - Byte start address of RX buffer . */
#define SPIM_RX_BUFFER_REG_RX_START_ADDR_POS                                                        (0UL)
#define SPIM_RX_BUFFER_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Configuration of RX CRC
* <b>Reset value:</b> 0x000001FF\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_RX_BUFFER_CRC_REG                                                                      (0x40034010UL) /**< rw-*/
#define SPIM_RX_BUFFER_CRC_REG_RX_CRC_PAYLOAD_OFFSET_MASK                                           (0x0001FF00UL) /**< rw - Number of bytes to skip for CRC computation. */
#define SPIM_RX_BUFFER_CRC_REG_RX_CRC_PAYLOAD_OFFSET_POS                                            (8UL)
#define SPIM_RX_BUFFER_CRC_REG_RX_CRC_INIT_MASK                                                     (0x000000FFUL) /**< rw - RX CRC init value. Only used if RX_SET_CRC is set. */
#define SPIM_RX_BUFFER_CRC_REG_RX_CRC_INIT_POS                                                      (0UL)
#define SPIM_RX_BUFFER_CRC_REG__RESET_VALUE                                                         (0x000001FFUL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Configuration of TX buffer
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_TX_BUFFER_REG                                                                          (0x40034014UL) /**< rw-*/
#define SPIM_TX_BUFFER_REG_TX_LENGTH_MASK                                                           (0x007FC000UL) /**< rw - Size of Tx transfer. */
#define SPIM_TX_BUFFER_REG_TX_LENGTH_POS                                                            (14UL)
#define SPIM_TX_BUFFER_REG_TX_START_ADDR_MASK                                                       (0x00003FFFUL) /**< rw - Byte start address of TX buffer. */
#define SPIM_TX_BUFFER_REG_TX_START_ADDR_POS                                                        (0UL)
#define SPIM_TX_BUFFER_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Configuration of TX CRC
* <b>Reset value:</b> 0x000201FF\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_TX_BUFFER_CRC_REG                                                                      (0x40034018UL) /**< rw-*/
#define SPIM_TX_BUFFER_CRC_REG_TX_APPEND_CRC_MASK                                                   (0x00020000UL) /**< rw - 1: CRC is sent after  transmission of TX_LENGTH bytes. */
#define SPIM_TX_BUFFER_CRC_REG_TX_APPEND_CRC_POS                                                    (17UL)
#define SPIM_TX_BUFFER_CRC_REG_TX_CRC_PAYLOAD_OFFSET_MASK                                           (0x0001FF00UL) /**< rw - Number of bytes to skip for CRC computation. */
#define SPIM_TX_BUFFER_CRC_REG_TX_CRC_PAYLOAD_OFFSET_POS                                            (8UL)
#define SPIM_TX_BUFFER_CRC_REG_TX_CRC_INIT_MASK                                                     (0x000000FFUL) /**< rw - TX CRC init value. Only used if TX_SET_CRC is set. */
#define SPIM_TX_BUFFER_CRC_REG_TX_CRC_INIT_POS                                                      (0UL)
#define SPIM_TX_BUFFER_CRC_REG__RESET_VALUE                                                         (0x000201FFUL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Configuration of TX buffer
* <b>Reset value:</b> 0x0000FFFF\n
* <b>Access:</b> r--
* @{
*/
#define SPIM_CRC_STATUS_REG                                                                         (0x4003401CUL) /**< r--*/
#define SPIM_CRC_STATUS_REG_CRC_TX_VAL_MASK                                                         (0x0000FF00UL) /**< r- - Value of internal TX CRC. */
#define SPIM_CRC_STATUS_REG_CRC_TX_VAL_POS                                                          (8UL)
#define SPIM_CRC_STATUS_REG_CRC_RX_VAL_MASK                                                         (0x000000FFUL) /**< r- - Value of internal RX CRC. */
#define SPIM_CRC_STATUS_REG_CRC_RX_VAL_POS                                                          (0UL)
#define SPIM_CRC_STATUS_REG__RESET_VALUE                                                            (0x0000FFFFUL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Waterlevel
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_WATERLEVEL_REG                                                                         (0x40034020UL) /**< rw-*/
#define SPIM_WATERLEVEL_REG_WATERLEVEL_MASK                                                         (0x000001FFUL) /**< rw - Number of bytes received in incoming frame before triggering an interrupt (pre-empting EOR). If set to 0, this feature is disabled.. */
#define SPIM_WATERLEVEL_REG_WATERLEVEL_POS                                                          (0UL)
#define SPIM_WATERLEVEL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Testbus select
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_TBSEL_REG                                                                              (0x40034024UL) /**< rw-*/
#define SPIM_TBSEL_REG_TESTBUS_SELECT_MASK                                                          (0x0000000FUL) /**< rw - Selects which set of test signals are routed to register SPIM_TBVALUE_REG and output SPIM_testbus_obs[7:0]. */
#define SPIM_TBSEL_REG_TESTBUS_SELECT_POS                                                           (0UL)
#define SPIM_TBSEL_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Testbus output
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define SPIM_TBVALUE_REG                                                                            (0x40034028UL) /**< r--*/
#define SPIM_TBVALUE_REG_TESTBUS_VALUE_MASK                                                         (0x000000FFUL) /**< r- - Selected set of test signals Also routed to output SPIM_testbus_obs[7:0]. */
#define SPIM_TBVALUE_REG_TESTBUS_VALUE_POS                                                          (0UL)
#define SPIM_TBVALUE_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM Buffer Mapping Register
* <b>Reset value:</b> 0x00002FFF\n
* <b>Access:</b> rw-
* @{
*/
#define SPIM_BUFFER_MAPPING_REG                                                                     (0x4003402CUL) /**< rw-*/
#define SPIM_BUFFER_MAPPING_REG_REGION_START_ADDR_MASK                                              (0x3FFF0000UL) /**< rw - Start address of buffer region in system RAM. */
#define SPIM_BUFFER_MAPPING_REG_REGION_START_ADDR_POS                                               (16UL)
#define SPIM_BUFFER_MAPPING_REG_REGION_SIZE_MASK                                                    (0x00003FFFUL) /**< rw - Size of buffer region in system RAM. */
#define SPIM_BUFFER_MAPPING_REG_REGION_SIZE_POS                                                     (0UL)
#define SPIM_BUFFER_MAPPING_REG__RESET_VALUE                                                        (0x00002FFFUL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM interrupt clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define SPIM_INT_CLR_ENABLE_REG                                                                     (0x40037FD8UL) /**< -w-*/
#define SPIM_INT_CLR_ENABLE_REG_AHB_ADDR_ERROR_CLR_ENABLE_MASK                                      (0x00000200UL) /**< -x - clear enable for AHB address overflow interrupt. */
#define SPIM_INT_CLR_ENABLE_REG_AHB_ADDR_ERROR_CLR_ENABLE_POS                                       (9UL)
#define SPIM_INT_CLR_ENABLE_REG_AHB_ERROR_CLR_ENABLE_MASK                                           (0x00000100UL) /**< -x - clear enable for AHB Slave error interrupt. */
#define SPIM_INT_CLR_ENABLE_REG_AHB_ERROR_CLR_ENABLE_POS                                            (8UL)
#define SPIM_INT_CLR_ENABLE_REG_WATERLEVEL_REACHED_CLR_ENABLE_MASK                                  (0x00000004UL) /**< -x - Clear enable for waterlevel reached interrupt 0 - no effect. */
#define SPIM_INT_CLR_ENABLE_REG_WATERLEVEL_REACHED_CLR_ENABLE_POS                                   (2UL)
#define SPIM_INT_CLR_ENABLE_REG_EOT_CLR_ENABLE_MASK                                                 (0x00000002UL) /**< -x - Clear enable for EOT interrupt. */
#define SPIM_INT_CLR_ENABLE_REG_EOT_CLR_ENABLE_POS                                                  (1UL)
#define SPIM_INT_CLR_ENABLE_REG_EOR_CLR_ENABLE_MASK                                                 (0x00000001UL) /**< -x - Clear enable for EOR interrupt. */
#define SPIM_INT_CLR_ENABLE_REG_EOR_CLR_ENABLE_POS                                                  (0UL)
#define SPIM_INT_CLR_ENABLE_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM interrupt set register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define SPIM_INT_SET_ENABLE_REG                                                                     (0x40037FDCUL) /**< -w-*/
#define SPIM_INT_SET_ENABLE_REG_AHB_ADDR_ERROR_SET_ENABLE_MASK                                      (0x00000200UL) /**< -x - set enable for AHB address overflow interrupt. */
#define SPIM_INT_SET_ENABLE_REG_AHB_ADDR_ERROR_SET_ENABLE_POS                                       (9UL)
#define SPIM_INT_SET_ENABLE_REG_AHB_ERROR_SET_ENABLE_MASK                                           (0x00000100UL) /**< -x - set enable for AHB Slave Error interrupt. */
#define SPIM_INT_SET_ENABLE_REG_AHB_ERROR_SET_ENABLE_POS                                            (8UL)
#define SPIM_INT_SET_ENABLE_REG_WATERLEVEL_REACHED_SET_ENABLE_MASK                                  (0x00000004UL) /**< -x - Set enable for waterlevel reached interrupt. */
#define SPIM_INT_SET_ENABLE_REG_WATERLEVEL_REACHED_SET_ENABLE_POS                                   (2UL)
#define SPIM_INT_SET_ENABLE_REG_EOT_SET_ENABLE_MASK                                                 (0x00000002UL) /**< -x - Set enable for EOT interrupt. */
#define SPIM_INT_SET_ENABLE_REG_EOT_SET_ENABLE_POS                                                  (1UL)
#define SPIM_INT_SET_ENABLE_REG_EOR_SET_ENABLE_MASK                                                 (0x00000001UL) /**< -x - Set enable for EOR interrupt. */
#define SPIM_INT_SET_ENABLE_REG_EOR_SET_ENABLE_POS                                                  (0UL)
#define SPIM_INT_SET_ENABLE_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM interrupt clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define SPIM_INT_STATUS_REG                                                                         (0x40037FE0UL) /**< r--*/
#define SPIM_INT_STATUS_REG_AHB_ADDR_ERROR_STATUS_MASK                                              (0x00000200UL) /**< r- - AHB address overflow interrupt status. */
#define SPIM_INT_STATUS_REG_AHB_ADDR_ERROR_STATUS_POS                                               (9UL)
#define SPIM_INT_STATUS_REG_AHB_ERROR_STATUS_MASK                                                   (0x00000100UL) /**< r- - AHB Slave Error interrupt status. */
#define SPIM_INT_STATUS_REG_AHB_ERROR_STATUS_POS                                                    (8UL)
#define SPIM_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_MASK                                          (0x00000004UL) /**< r- - Waterlevel reached interrupt status. */
#define SPIM_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_POS                                           (2UL)
#define SPIM_INT_STATUS_REG_EOT_STATUS_MASK                                                         (0x00000002UL) /**< r- - EOT interrupt status. */
#define SPIM_INT_STATUS_REG_EOT_STATUS_POS                                                          (1UL)
#define SPIM_INT_STATUS_REG_EOR_STATUS_MASK                                                         (0x00000001UL) /**< r- - EOR interrupt status. */
#define SPIM_INT_STATUS_REG_EOR_STATUS_POS                                                          (0UL)
#define SPIM_INT_STATUS_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM interrupt clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define SPIM_INT_ENABLE_REG                                                                         (0x40037FE4UL) /**< r--*/
#define SPIM_INT_ENABLE_REG_AHB_ADDR_ERROR_ENABLE_MASK                                              (0x00000200UL) /**< r- - AHB address overflow interrupt enable. */
#define SPIM_INT_ENABLE_REG_AHB_ADDR_ERROR_ENABLE_POS                                               (9UL)
#define SPIM_INT_ENABLE_REG_AHB_ERROR_ENABLE_MASK                                                   (0x00000100UL) /**< r- - AHB Slave Error interrupt enable. */
#define SPIM_INT_ENABLE_REG_AHB_ERROR_ENABLE_POS                                                    (8UL)
#define SPIM_INT_ENABLE_REG_WATERLEVEL_REACHED_ENABLE_MASK                                          (0x00000004UL) /**< r- - Waterlevel reached interrupt enable. */
#define SPIM_INT_ENABLE_REG_WATERLEVEL_REACHED_ENABLE_POS                                           (2UL)
#define SPIM_INT_ENABLE_REG_EOT_ENABLE_MASK                                                         (0x00000002UL) /**< r- - EOT interrupt enable. */
#define SPIM_INT_ENABLE_REG_EOT_ENABLE_POS                                                          (1UL)
#define SPIM_INT_ENABLE_REG_EOR_ENABLE_MASK                                                         (0x00000001UL) /**< r- - EOR interrupt enable. */
#define SPIM_INT_ENABLE_REG_EOR_ENABLE_POS                                                          (0UL)
#define SPIM_INT_ENABLE_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM interrupt clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define SPIM_INT_CLR_STATUS_REG                                                                     (0x40037FE8UL) /**< -w-*/
#define SPIM_INT_CLR_STATUS_REG_AHB_ADDR_ERROR_CLR_STATUS_MASK                                      (0x00000200UL) /**< -x - clear AHB address overflow interrupt. */
#define SPIM_INT_CLR_STATUS_REG_AHB_ADDR_ERROR_CLR_STATUS_POS                                       (9UL)
#define SPIM_INT_CLR_STATUS_REG_AHB_ERROR_CLR_STATUS_MASK                                           (0x00000100UL) /**< -x - clear AHB Slave Error interrupt. */
#define SPIM_INT_CLR_STATUS_REG_AHB_ERROR_CLR_STATUS_POS                                            (8UL)
#define SPIM_INT_CLR_STATUS_REG_WATERLEVEL_REACHED_CLR_STATUS_MASK                                  (0x00000004UL) /**< -x - Clear waterlevel reached interrupt. */
#define SPIM_INT_CLR_STATUS_REG_WATERLEVEL_REACHED_CLR_STATUS_POS                                   (2UL)
#define SPIM_INT_CLR_STATUS_REG_EOT_CLR_STATUS_MASK                                                 (0x00000002UL) /**< -x - Clear EOT interrupt. */
#define SPIM_INT_CLR_STATUS_REG_EOT_CLR_STATUS_POS                                                  (1UL)
#define SPIM_INT_CLR_STATUS_REG_EOR_CLR_STATUS_MASK                                                 (0x00000001UL) /**< -x - Clear EOR interrupt. */
#define SPIM_INT_CLR_STATUS_REG_EOR_CLR_STATUS_POS                                                  (0UL)
#define SPIM_INT_CLR_STATUS_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPIM interrupt clear register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define SPIM_INT_SET_STATUS_REG                                                                     (0x40037FECUL) /**< -w-*/
#define SPIM_INT_SET_STATUS_REG_AHB_ADDR_ERROR_SET_STATUS_MASK                                      (0x00000200UL) /**< -x - set AHB address overflow interrupt. */
#define SPIM_INT_SET_STATUS_REG_AHB_ADDR_ERROR_SET_STATUS_POS                                       (9UL)
#define SPIM_INT_SET_STATUS_REG_AHB_ERROR_SET_STATUS_MASK                                           (0x00000100UL) /**< -x - set AHB Slave Error interrupt. */
#define SPIM_INT_SET_STATUS_REG_AHB_ERROR_SET_STATUS_POS                                            (8UL)
#define SPIM_INT_SET_STATUS_REG_WATERLEVEL_REACHED_SET_STATUS_MASK                                  (0x00000004UL) /**< -x - Set waterlevel reached interrupt. */
#define SPIM_INT_SET_STATUS_REG_WATERLEVEL_REACHED_SET_STATUS_POS                                   (2UL)
#define SPIM_INT_SET_STATUS_REG_EOT_SET_STATUS_MASK                                                 (0x00000002UL) /**< -x - Set EOT interrupt. */
#define SPIM_INT_SET_STATUS_REG_EOT_SET_STATUS_POS                                                  (1UL)
#define SPIM_INT_SET_STATUS_REG_EOR_SET_STATUS_MASK                                                 (0x00000001UL) /**< -x - Set EOR interrupt. */
#define SPIM_INT_SET_STATUS_REG_EOR_SET_STATUS_POS                                                  (0UL)
#define SPIM_INT_SET_STATUS_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_SPIM_H_INCLUDED */
