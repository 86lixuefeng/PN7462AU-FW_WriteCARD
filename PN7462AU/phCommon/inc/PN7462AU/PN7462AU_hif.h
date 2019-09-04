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
#ifndef PN7462AU_IP_HIF_H_INCLUDED
#define PN7462AU_IP_HIF_H_INCLUDED

#include "PN7462AU/PN7462AU_hif_rorw.h"



/**
* \name Status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_STATUS_REG                                                                           (0x40020000UL) /**< r--*/
#define HOSTIF_STATUS_REG_TX_BUFFER_PREFETCH_OK_MASK                                                (0x00000020UL) /**< r- - 1 - Read prefetch of TX buffer completed. */
#define HOSTIF_STATUS_REG_TX_BUFFER_PREFETCH_OK_POS                                                 (5UL)
#define HOSTIF_STATUS_REG_TX_BUFFER_LOCK_MASK                                                       (0x00000010UL) /**< r- - 1 - TX buffer is currently in use by the HW.. */
#define HOSTIF_STATUS_REG_TX_BUFFER_LOCK_POS                                                        (4UL)
#define HOSTIF_STATUS_REG_RX3_BUFFER_LOCK_MASK                                                      (0x00000008UL) /**< r- - 1 - RX buffer 3 is currently in use by the HW.. */
#define HOSTIF_STATUS_REG_RX3_BUFFER_LOCK_POS                                                       (3UL)
#define HOSTIF_STATUS_REG_RX2_BUFFER_LOCK_MASK                                                      (0x00000004UL) /**< r- - 1 - RX buffer 2 is currently in use by the HW.. */
#define HOSTIF_STATUS_REG_RX2_BUFFER_LOCK_POS                                                       (2UL)
#define HOSTIF_STATUS_REG_RX1_BUFFER_LOCK_MASK                                                      (0x00000002UL) /**< r- - 1 - RX buffer 1 is currently in use by the HW.. */
#define HOSTIF_STATUS_REG_RX1_BUFFER_LOCK_POS                                                       (1UL)
#define HOSTIF_STATUS_REG_RX0_BUFFER_LOCK_MASK                                                      (0x00000001UL) /**< r- - 1 - RX buffer 0 is currently in use by the HW.. */
#define HOSTIF_STATUS_REG_RX0_BUFFER_LOCK_POS                                                       (0UL)
#define HOSTIF_STATUS_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Control
* <b>Reset value:</b> 0x0017FF80\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_CONTROL_REG                                                                          (0x40020004UL) /**< rw-*/
#define HOSTIF_CONTROL_REG_BUFFERS_SIZE_MASK                                                        (0x001FFF80UL) /**< rw - RAM aperture size. */
#define HOSTIF_CONTROL_REG_BUFFERS_SIZE_POS                                                         (7UL)
#define HOSTIF_CONTROL_REG_HIF_ENABLE_MASK                                                          (0x00000040UL) /**< rw - Enables the signal lines  MOSI, SCK, NSS, I2C_SDA, I2C_SCL) from the PCR block. */
#define HOSTIF_CONTROL_REG_HIF_ENABLE_POS                                                           (6UL)
#define HOSTIF_CONTROL_REG_LENGTH_MODE_MASK                                                         (0x00000020UL) /**< rw - 0 - Length in mem and in transfered over HDLL header 1 - Length in register. Not transfered in I2C/SPI. */
#define HOSTIF_CONTROL_REG_LENGTH_MODE_POS                                                          (5UL)
#define HOSTIF_CONTROL_REG_CRC_DISABLE_MASK                                                         (0x00000010UL) /**< rw - 0 - CRC active 1 - No CRC. */
#define HOSTIF_CONTROL_REG_CRC_DISABLE_POS                                                          (4UL)
#define HOSTIF_CONTROL_REG_NCI_MODE_MASK                                                            (0x00000008UL) /**< rw - 0 - NCI not used 1 - NCI mode active: CRC_DISABLE and LENGTH_MODE are used. */
#define HOSTIF_CONTROL_REG_NCI_MODE_POS                                                             (3UL)
#define HOSTIF_CONTROL_REG_SHORT_FRAME_LEN_MASK                                                     (0x00000004UL) /**< rw - Maximum length of a short frame (in bytes) 0 - 2 bytes 1 - 3 bytes. */
#define HOSTIF_CONTROL_REG_SHORT_FRAME_LEN_POS                                                      (2UL)
#define HOSTIF_CONTROL_REG_STORE_RX_ERROR_FRAMES_MASK                                               (0x00000002UL) /**< rw - 1- Store erroneous RX frames. RX<n>_DATA_READY is set by Host IF as if frame were received error-free. */
#define HOSTIF_CONTROL_REG_STORE_RX_ERROR_FRAMES_POS                                                (1UL)
#define HOSTIF_CONTROL_REG_BUFFER_FORMAT_MASK                                                       (0x00000001UL) /**< rw - 0 - HDLL Frames; 1 - Native format. */
#define HOSTIF_CONTROL_REG_BUFFER_FORMAT_POS                                                        (0UL)
#define HOSTIF_CONTROL_REG__RESET_VALUE                                                             (0x0017FF80UL) /**< value after reset */
/**
* @}
*/


/**
* \name Header Control
* <b>Reset value:</b> 0x00000428\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_HEADER_CONTROL_REG                                                                   (0x40020008UL) /**< rw-*/
#define HOSTIF_HEADER_CONTROL_REG_HEADER_SIZE_MASK                                                  (0x00000C00UL) /**< rw - Numbers of bytes -1 of the header. */
#define HOSTIF_HEADER_CONTROL_REG_HEADER_SIZE_POS                                                   (10UL)
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_MSB_BYTE_POS_MASK                                          (0x00000300UL) /**< rw - Byte position of Length MSB (in recepetion order). */
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_MSB_BYTE_POS_POS                                           (8UL)
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_MSB_BIT_POS_MASK                                           (0x000000E0UL) /**< rw - Bit position of Length MSB. */
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_MSB_BIT_POS_POS                                            (5UL)
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_LSB_BYTE_POS_MASK                                          (0x00000018UL) /**< rw - Byte position of Length LSB (in recepetion order). */
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_LSB_BYTE_POS_POS                                           (3UL)
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_LSB_BIT_POS_MASK                                           (0x00000007UL) /**< rw - Bit position of Length LSB. */
#define HOSTIF_HEADER_CONTROL_REG_LENGTH_LSB_BIT_POS_POS                                            (0UL)
#define HOSTIF_HEADER_CONTROL_REG__RESET_VALUE                                                      (0x00000428UL) /**< value after reset */
/**
* @}
*/


/**
* \name I2C control
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_I2C_CONTROL_REG                                                                      (0x4002000CUL) /**< rw-*/
#define HOSTIF_I2C_CONTROL_REG_I2C_REV_ID_MASK                                                      (0x000000E0UL) /**< rw - device_id_rev(2:0). */
#define HOSTIF_I2C_CONTROL_REG_I2C_REV_ID_POS                                                       (5UL)
#define HOSTIF_I2C_CONTROL_REG_I2C_DEVID_ENABLE_MASK                                                (0x00000010UL) /**< rw - 1 - Enable Device ID defined in the I2C standard. */
#define HOSTIF_I2C_CONTROL_REG_I2C_DEVID_ENABLE_POS                                                 (4UL)
#define HOSTIF_I2C_CONTROL_REG_I2C_RESET_ENABLE_MASK                                                (0x00000008UL) /**< rw - 1 - Enable Soft Reset sequence defined in the I2C standard. */
#define HOSTIF_I2C_CONTROL_REG_I2C_RESET_ENABLE_POS                                                 (3UL)
#define HOSTIF_I2C_CONTROL_REG_I2C_HS_ENABLE_MASK                                                   (0x00000004UL) /**< rw - 1 - Enable High-speed mode. */
#define HOSTIF_I2C_CONTROL_REG_I2C_HS_ENABLE_POS                                                    (2UL)
#define HOSTIF_I2C_CONTROL_REG_I2C_ADDR_MASK                                                        (0x00000003UL) /**< rw - Set two LSBs of the I2C address. */
#define HOSTIF_I2C_CONTROL_REG_I2C_ADDR_POS                                                         (0UL)
#define HOSTIF_I2C_CONTROL_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SPI control
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SPI_CONTROL_REG                                                                      (0x40020010UL) /**< rw-*/
#define HOSTIF_SPI_CONTROL_REG_SPI_CPHA_MASK                                                        (0x00000002UL) /**< rw - SPI clock phase. */
#define HOSTIF_SPI_CONTROL_REG_SPI_CPHA_POS                                                         (1UL)
#define HOSTIF_SPI_CONTROL_REG_SPI_CPOL_MASK                                                        (0x00000001UL) /**< rw - SPI clock polarity. */
#define HOSTIF_SPI_CONTROL_REG_SPI_CPOL_POS                                                         (0UL)
#define HOSTIF_SPI_CONTROL_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name HSU control Reg
* <b>Reset value:</b> 0x01410000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_HSU_CONTROL_REG                                                                      (0x40020014UL) /**< rw-*/
#define HOSTIF_HSU_CONTROL_REG_HSU_EOF_SIZE_MASK                                                    (0x0FE00000UL) /**< rw - EOF duration in bit number. */
#define HOSTIF_HSU_CONTROL_REG_HSU_EOF_SIZE_POS                                                     (21UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_STORE_BR_BYTE_MASK                                               (0x00100000UL) /**< rw - 1: the first 0 used to estimate baudrate is stored in memory. */
#define HOSTIF_HSU_CONTROL_REG_HSU_STORE_BR_BYTE_POS                                                (20UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_BR_ESTIMATOR_MODE_MASK                                           (0x000C0000UL) /**< rw - 00: inactive - 01: automatic - 10 active without clock setting. */
#define HOSTIF_HSU_CONTROL_REG_HSU_BR_ESTIMATOR_MODE_POS                                            (18UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_WAKEUP_BYTES_MASK                                                (0x00030000UL) /**< rw - Number of bytes lost during wakeup at RTS rising edge. */
#define HOSTIF_HSU_CONTROL_REG_HSU_WAKEUP_BYTES_POS                                                 (16UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_WAKEUP_STANDBY_MASK                                              (0x00008000UL) /**< rw - 1: simulate reception of bytes loast during wakeup from standby phase (number of bytes specified in HSU_WAKEUP_BYTES).. */
#define HOSTIF_HSU_CONTROL_REG_HSU_WAKEUP_STANDBY_POS                                               (15UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_STOPBIT_MASK                                                     (0x00004000UL) /**< rw - 0: 1 stopbit- 1: 2 stopbits. */
#define HOSTIF_HSU_CONTROL_REG_HSU_STOPBIT_POS                                                      (14UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_TX_DIVIDER_MASK                                                  (0x00002000UL) /**< rw - 0: Use HSU Rx Divider.  1: Use HSU_RX_DIVIDER+1. */
#define HOSTIF_HSU_CONTROL_REG_HSU_TX_DIVIDER_POS                                                   (13UL)
#define HOSTIF_HSU_CONTROL_REG_HSU_RX_DIVIDER_MASK                                                  (0x00001FFFUL) /**< rw - Clock Divider for Rx sampling. */
#define HOSTIF_HSU_CONTROL_REG_HSU_RX_DIVIDER_POS                                                   (0UL)
#define HOSTIF_HSU_CONTROL_REG__RESET_VALUE                                                         (0x01410000UL) /**< value after reset */
/**
* @}
*/


/**
* \name HSU sample Reg
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_HSU_SAMPLE_REG                                                                       (0x40020018UL) /**< rw-*/
#define HOSTIF_HSU_SAMPLE_REG_HSU_TX_CLK_CORRECT_MASK                                               (0x003FF800UL) /**< rw - Used to correct Clock division: if TX_CLK_CORRECT[i]=1, then duration of bit i will be extended by 1 clock cycle. */
#define HOSTIF_HSU_SAMPLE_REG_HSU_TX_CLK_CORRECT_POS                                                (11UL)
#define HOSTIF_HSU_SAMPLE_REG_HSU_RX_CLK_CORRECT_MASK                                               (0x000007FFUL) /**< rw - Used to correct Clock division: if RX_CLK_CORRECT[i]=1, then duration of bit i will be extended by 1 clock cycle. */
#define HOSTIF_HSU_SAMPLE_REG_HSU_RX_CLK_CORRECT_POS                                                (0UL)
#define HOSTIF_HSU_SAMPLE_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name HSU estimated clock dividers
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_HSU_EST_CLOCK_DIVIDER_REG                                                            (0x4002001CUL) /**< r--*/
#define HOSTIF_HSU_EST_CLOCK_DIVIDER_REG_HSU_EST_TX_DIVIDER_MASK                                    (0x00002000UL) /**< r- - Estimated value for HSU_TX_DIVIDER. */
#define HOSTIF_HSU_EST_CLOCK_DIVIDER_REG_HSU_EST_TX_DIVIDER_POS                                     (13UL)
#define HOSTIF_HSU_EST_CLOCK_DIVIDER_REG_HSU_EST_RX_DIVIDER_MASK                                    (0x00001FFFUL) /**< r- - Estimated value for HSU_RX_DIVIDER. */
#define HOSTIF_HSU_EST_CLOCK_DIVIDER_REG_HSU_EST_RX_DIVIDER_POS                                     (0UL)
#define HOSTIF_HSU_EST_CLOCK_DIVIDER_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name HSU estimated clock correct parameters
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_HSU_EST_CLOCK_CORRECT_REG                                                            (0x40020020UL) /**< r--*/
#define HOSTIF_HSU_EST_CLOCK_CORRECT_REG_HSU_EST_TX_CLK_CORRECT_MASK                                (0x003FF800UL) /**< r- - Estimated value for HSU_TX_CLK_CORRECT. */
#define HOSTIF_HSU_EST_CLOCK_CORRECT_REG_HSU_EST_TX_CLK_CORRECT_POS                                 (11UL)
#define HOSTIF_HSU_EST_CLOCK_CORRECT_REG_HSU_EST_RX_CLK_CORRECT_MASK                                (0x000007FFUL) /**< r- - Estimated value for HSU_RX_CLK_CORRECT. */
#define HOSTIF_HSU_EST_CLOCK_CORRECT_REG_HSU_EST_RX_CLK_CORRECT_POS                                 (0UL)
#define HOSTIF_HSU_EST_CLOCK_CORRECT_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SMBUS control
* <b>Reset value:</b> 0x00061000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SMBUS_CONTROL_REG                                                                    (0x40020024UL) /**< rw-*/
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_RESET_COUNT_MASK                                             (0x000FE000UL) /**< rw - Tune the duration of snbus_reset detection. 0 to disable. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_RESET_COUNT_POS                                              (13UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_PEC_ENABLE_MASK                                              (0x00001000UL) /**< rw - Enables PEC management at HW level. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_PEC_ENABLE_POS                                               (12UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_CANCEL_ALERT_MASK                                            (0x00000800UL) /**< -x - Cancel current SMBALERT# process. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_CANCEL_ALERT_POS                                             (11UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_ALERT_EN_MASK                                                (0x00000400UL) /**< -x - Start SMBUS SMBALERT# process. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_ALERT_EN_POS                                                 (10UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_ARP_EN_MASK                                                  (0x00000200UL) /**< rw - Enable SMBUS ARP. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_ARP_EN_POS                                                   (9UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_AV_MASK                                                      (0x00000100UL) /**< rw - SMBUS AV flag. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_AV_POS                                                       (8UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_AR_MASK                                                      (0x00000080UL) /**< rw - SMBUS AR flag. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_AR_POS                                                       (7UL)
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_ADDR_MASK                                                    (0x0000007FUL) /**< rw - SMBUS device address. */
#define HOSTIF_SMBUS_CONTROL_REG_SMBUS_ADDR_POS                                                     (0UL)
#define HOSTIF_SMBUS_CONTROL_REG__RESET_VALUE                                                       (0x00061000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SMBUS Udid Part 0
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SMBUS_UDID_0_REG                                                                     (0x40020028UL) /**< rw-*/
#define HOSTIF_SMBUS_UDID_0_REG_UDID0_MASK                                                          (0xFFFFFFFFUL) /**< rw - Udid(31:0). */
#define HOSTIF_SMBUS_UDID_0_REG_UDID0_POS                                                           (0UL)
#define HOSTIF_SMBUS_UDID_0_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SMBUS Udid Part 1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SMBUS_UDID_1_REG                                                                     (0x4002002CUL) /**< rw-*/
#define HOSTIF_SMBUS_UDID_1_REG_UDID1_MASK                                                          (0xFFFFFFFFUL) /**< rw - Udid(63:32). */
#define HOSTIF_SMBUS_UDID_1_REG_UDID1_POS                                                           (0UL)
#define HOSTIF_SMBUS_UDID_1_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SMBUS Udid Part 2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SMBUS_UDID_2_REG                                                                     (0x40020030UL) /**< rw-*/
#define HOSTIF_SMBUS_UDID_2_REG_UDID2_MASK                                                          (0xFFFFFFFFUL) /**< rw - Udid(95:64). */
#define HOSTIF_SMBUS_UDID_2_REG_UDID2_POS                                                           (0UL)
#define HOSTIF_SMBUS_UDID_2_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SMBUS Udid Part 3
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SMBUS_UDID_3_REG                                                                     (0x40020034UL) /**< rw-*/
#define HOSTIF_SMBUS_UDID_3_REG_UDID3_MASK                                                          (0xFFFFFFFFUL) /**< rw - Udid(127:96). */
#define HOSTIF_SMBUS_UDID_3_REG_UDID3_POS                                                           (0UL)
#define HOSTIF_SMBUS_UDID_3_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX buffer 0 configuration
* <b>Reset value:</b> 0x08000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_BUFFER_RX0_CFG_REG                                                                   (0x40020038UL) /**< rw-*/
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_BUFFER_DISABLE_MASK                                           (0x10000000UL) /**< rw - 1 - buffer is disabled. */
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_BUFFER_DISABLE_POS                                            (28UL)
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_HEADER_OFFSET_MASK                                            (0x0C000000UL) /**< rw - number of padding bytes (MSbyte) which should be added to the first word written to RX buffer in order that the payload is always word-aligned. */
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_HEADER_OFFSET_POS                                             (26UL)
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_SHORT_FRAME_BUFFER_MASK                                       (0x02000000UL) /**< rw - 1 - buffer assigned to short frames 0 - buffer assigned to non-short frames. */
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_SHORT_FRAME_BUFFER_POS                                        (25UL)
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_MAX_SIZE_MASK                                                 (0x01FFC000UL) /**< rw - Maximum size (bytes) of RX buffer 0. */
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_MAX_SIZE_POS                                                  (14UL)
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_START_ADDR_MASK                                               (0x00003FFFUL) /**< rw - Start address of RX buffer 0. */
#define HOSTIF_BUFFER_RX0_CFG_REG_RX0_START_ADDR_POS                                                (0UL)
#define HOSTIF_BUFFER_RX0_CFG_REG__RESET_VALUE                                                      (0x08000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX buffer 1 configuration
* <b>Reset value:</b> 0x08000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_BUFFER_RX1_CFG_REG                                                                   (0x4002003CUL) /**< rw-*/
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_BUFFER_DISABLE_MASK                                           (0x10000000UL) /**< rw - 1 - buffer is disabled. */
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_BUFFER_DISABLE_POS                                            (28UL)
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_HEADER_OFFSET_MASK                                            (0x0C000000UL) /**< rw - number of padding bytes (MSbyte) which should be added to the first word written to RX buffer in order that the payload is always word-aligned. */
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_HEADER_OFFSET_POS                                             (26UL)
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_SHORT_FRAME_BUFFER_MASK                                       (0x02000000UL) /**< rw - 1 - buffer assigned to short frames 0 - buffer assigned to non-short frames. */
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_SHORT_FRAME_BUFFER_POS                                        (25UL)
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_MAX_SIZE_MASK                                                 (0x01FFC000UL) /**< rw - Maximum size (bytes) of RX buffer 1. */
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_MAX_SIZE_POS                                                  (14UL)
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_START_ADDR_MASK                                               (0x00003FFFUL) /**< rw - Start address of RX buffer 1. */
#define HOSTIF_BUFFER_RX1_CFG_REG_RX1_START_ADDR_POS                                                (0UL)
#define HOSTIF_BUFFER_RX1_CFG_REG__RESET_VALUE                                                      (0x08000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX buffer 2 configuration
* <b>Reset value:</b> 0x08000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_BUFFER_RX2_CFG_REG                                                                   (0x40020040UL) /**< rw-*/
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_BUFFER_DISABLE_MASK                                           (0x10000000UL) /**< rw - 1 - buffer is disabled. */
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_BUFFER_DISABLE_POS                                            (28UL)
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_HEADER_OFFSET_MASK                                            (0x0C000000UL) /**< rw - number of padding bytes (MSbyte) which should be added to the first word written to RX buffer in order that the payload is always word-aligned. */
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_HEADER_OFFSET_POS                                             (26UL)
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_SHORT_FRAME_BUFFER_MASK                                       (0x02000000UL) /**< rw - 1 - buffer assigned to short frames 0 - buffer assigned to non-short frames. */
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_SHORT_FRAME_BUFFER_POS                                        (25UL)
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_MAX_SIZE_MASK                                                 (0x01FFC000UL) /**< rw - Maximum size (bytes) of RX buffer 2. */
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_MAX_SIZE_POS                                                  (14UL)
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_START_ADDR_MASK                                               (0x00003FFFUL) /**< rw - Start address of RX buffer 2. */
#define HOSTIF_BUFFER_RX2_CFG_REG_RX2_START_ADDR_POS                                                (0UL)
#define HOSTIF_BUFFER_RX2_CFG_REG__RESET_VALUE                                                      (0x08000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX buffer 3 configuration
* <b>Reset value:</b> 0x08000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_BUFFER_RX3_CFG_REG                                                                   (0x40020044UL) /**< rw-*/
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_BUFFER_DISABLE_MASK                                           (0x10000000UL) /**< rw - 1 - buffer is disabled. */
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_BUFFER_DISABLE_POS                                            (28UL)
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_HEADER_OFFSET_MASK                                            (0x0C000000UL) /**< rw - number of padding bytes (MSbyte) which should be added to the first word written to RX buffer in order that the payload is always word-aligned. */
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_HEADER_OFFSET_POS                                             (26UL)
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_SHORT_FRAME_BUFFER_MASK                                       (0x02000000UL) /**< rw - 1 - buffer assigned to short frames 0 - buffer assigned to non-short frames. */
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_SHORT_FRAME_BUFFER_POS                                        (25UL)
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_MAX_SIZE_MASK                                                 (0x01FFC000UL) /**< rw - Maximum size (bytes) of RX buffer 3. */
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_MAX_SIZE_POS                                                  (14UL)
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_START_ADDR_MASK                                               (0x00003FFFUL) /**< rw - Start address of RX buffer 3. */
#define HOSTIF_BUFFER_RX3_CFG_REG_RX3_START_ADDR_POS                                                (0UL)
#define HOSTIF_BUFFER_RX3_CFG_REG__RESET_VALUE                                                      (0x08000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TX buffer configuration
* <b>Reset value:</b> 0x00008000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_BUFFER_TX_CFG_REG                                                                    (0x40020048UL) /**< rw-*/
#define HOSTIF_BUFFER_TX_CFG_REG_TX_EMPTY_PAYLOAD_ENABLE_MASK                                       (0x00010000UL) /**< rw - 1 - only send the first two bytes containing the header from the first word (second word in Native mode) in the TX buffer. Used to send empty payload packets.. */
#define HOSTIF_BUFFER_TX_CFG_REG_TX_EMPTY_PAYLOAD_ENABLE_POS                                        (16UL)
#define HOSTIF_BUFFER_TX_CFG_REG_TX_HEADER_OFFSET_MASK                                              (0x0000C000UL) /**< rw - Number of bytes to skip in first word of frame (second word in Native mode). Used when sending frames directly from the CLIF RX buffer.. */
#define HOSTIF_BUFFER_TX_CFG_REG_TX_HEADER_OFFSET_POS                                               (14UL)
#define HOSTIF_BUFFER_TX_CFG_REG_TX_START_ADDR_MASK                                                 (0x00003FFFUL) /**< rw - Start address of TX buffer. */
#define HOSTIF_BUFFER_TX_CFG_REG_TX_START_ADDR_POS                                                  (0UL)
#define HOSTIF_BUFFER_TX_CFG_REG__RESET_VALUE                                                       (0x00008000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX0 length
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_BUFFER_RX0_LEN_REG                                                                   (0x4002004CUL) /**< r--*/
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_SMBUS_ERROR_MASK                                              (0x00002000UL) /**< r- - 1 - SMBUS received extra data. */
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_SMBUS_ERROR_POS                                               (13UL)
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_PEC_OK_MASK                                                   (0x00001000UL) /**< r- - 1 - Last byte received matched computed SMBUS PEC. */
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_PEC_OK_POS                                                    (12UL)
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_PEC_RECEIVED_MASK                                             (0x00000800UL) /**< r- - 1 - Last byte received was a PEC. */
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_PEC_RECEIVED_POS                                              (11UL)
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_LENGTH_MASK                                                   (0x000007FFUL) /**< r- - RX0 buffer packet length. */
#define HOSTIF_BUFFER_RX0_LEN_REG_RX0_LENGTH_POS                                                    (0UL)
#define HOSTIF_BUFFER_RX0_LEN_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX1 length
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_BUFFER_RX1_LEN_REG                                                                   (0x40020050UL) /**< r--*/
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_SMBUS_ERROR_MASK                                              (0x00002000UL) /**< r- - 1 - SMBUS received extra data. */
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_SMBUS_ERROR_POS                                               (13UL)
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_PEC_OK_MASK                                                   (0x00001000UL) /**< r- - 1 - Last byte received matched computed SMBUS PEC. */
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_PEC_OK_POS                                                    (12UL)
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_PEC_RECEIVED_MASK                                             (0x00000800UL) /**< r- - 1 - Last byte received was a PEC. */
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_PEC_RECEIVED_POS                                              (11UL)
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_LENGTH_MASK                                                   (0x000007FFUL) /**< r- - RX1 buffer packet length. */
#define HOSTIF_BUFFER_RX1_LEN_REG_RX1_LENGTH_POS                                                    (0UL)
#define HOSTIF_BUFFER_RX1_LEN_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX2 length
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_BUFFER_RX2_LEN_REG                                                                   (0x40020054UL) /**< r--*/
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_SMBUS_ERROR_MASK                                              (0x00002000UL) /**< r- - 1 - SMBUS received extra data. */
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_SMBUS_ERROR_POS                                               (13UL)
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_PEC_OK_MASK                                                   (0x00001000UL) /**< r- - 1 - Last byte received matched computed SMBUS PEC. */
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_PEC_OK_POS                                                    (12UL)
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_PEC_RECEIVED_MASK                                             (0x00000800UL) /**< r- - 1 - Last byte received was a PEC. */
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_PEC_RECEIVED_POS                                              (11UL)
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_LENGTH_MASK                                                   (0x000007FFUL) /**< r- - RX2 buffer packet length. */
#define HOSTIF_BUFFER_RX2_LEN_REG_RX2_LENGTH_POS                                                    (0UL)
#define HOSTIF_BUFFER_RX2_LEN_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RX3 length
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_BUFFER_RX3_LEN_REG                                                                   (0x40020058UL) /**< r--*/
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_SMBUS_ERROR_MASK                                              (0x00002000UL) /**< r- - 1 - SMBUS received extra data. */
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_SMBUS_ERROR_POS                                               (13UL)
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_PEC_OK_MASK                                                   (0x00001000UL) /**< r- - 1 - Last byte received matched computed SMBUS PEC. */
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_PEC_OK_POS                                                    (12UL)
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_PEC_RECEIVED_MASK                                             (0x00000800UL) /**< r- - 1 - Last byte received was a PEC. */
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_PEC_RECEIVED_POS                                              (11UL)
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_LENGTH_MASK                                                   (0x000007FFUL) /**< r- - RX3 buffer packet length. */
#define HOSTIF_BUFFER_RX3_LEN_REG_RX3_LENGTH_POS                                                    (0UL)
#define HOSTIF_BUFFER_RX3_LEN_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Configuration of TX length
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_BUFFER_TX_LEN_REG                                                                    (0x4002005CUL) /**< rw-*/
#define HOSTIF_BUFFER_TX_LEN_REG_TX_LENGTH_MASK                                                     (0x000007FFUL) /**< rw - TX buffer packet length. */
#define HOSTIF_BUFFER_TX_LEN_REG_TX_LENGTH_POS                                                      (0UL)
#define HOSTIF_BUFFER_TX_LEN_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Inter-character timeout
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_TIC_TIMEOUT_REG                                                                      (0x40020060UL) /**< rw-*/
#define HOSTIF_TIC_TIMEOUT_REG_TX_TIMEOUT_VALUE_MASK                                                (0x0000FFFFUL) /**< rw - Inter-character timeout in 5us steps (max delay 200ms). If set to 0, this feature is disabled. */
#define HOSTIF_TIC_TIMEOUT_REG_TX_TIMEOUT_VALUE_POS                                                 (0UL)
#define HOSTIF_TIC_TIMEOUT_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Waterlevel
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_WATERLEVEL_REG                                                                       (0x40020064UL) /**< rw-*/
#define HOSTIF_WATERLEVEL_REG_WATERLEVEL_MASK                                                       (0x000007FFUL) /**< rw - Number of bytes received in incoming frame before triggering an interrupt (pre-empting EOR). If set to 0, this feature is disabled.. */
#define HOSTIF_WATERLEVEL_REG_WATERLEVEL_POS                                                        (0UL)
#define HOSTIF_WATERLEVEL_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Set buffer ready register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define HOSTIF_SET_DATA_READY_REG                                                                   (0x40020068UL) /**< -w-*/
#define HOSTIF_SET_DATA_READY_REG_SET_TX_DATA_READY_MASK                                            (0x00000010UL) /**< -x - Set Tx_data_ready bit. */
#define HOSTIF_SET_DATA_READY_REG_SET_TX_DATA_READY_POS                                             (4UL)
#define HOSTIF_SET_DATA_READY_REG_SET_RX3_DATA_READY_MASK                                           (0x00000008UL) /**< -x - Set Rx3_data_ready bit. */
#define HOSTIF_SET_DATA_READY_REG_SET_RX3_DATA_READY_POS                                            (3UL)
#define HOSTIF_SET_DATA_READY_REG_SET_RX2_DATA_READY_MASK                                           (0x00000004UL) /**< -x - Set Rx2_data_ready bit. */
#define HOSTIF_SET_DATA_READY_REG_SET_RX2_DATA_READY_POS                                            (2UL)
#define HOSTIF_SET_DATA_READY_REG_SET_RX1_DATA_READY_MASK                                           (0x00000002UL) /**< -x - Set Rx1_data_ready bit. */
#define HOSTIF_SET_DATA_READY_REG_SET_RX1_DATA_READY_POS                                            (1UL)
#define HOSTIF_SET_DATA_READY_REG_SET_RX0_DATA_READY_MASK                                           (0x00000001UL) /**< -x - Set Rx0_data_ready bit. */
#define HOSTIF_SET_DATA_READY_REG_SET_RX0_DATA_READY_POS                                            (0UL)
#define HOSTIF_SET_DATA_READY_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Clr buffer ready register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define HOSTIF_CLR_DATA_READY_REG                                                                   (0x4002006CUL) /**< -w-*/
#define HOSTIF_CLR_DATA_READY_REG_CLR_TX_DATA_READY_MASK                                            (0x00000010UL) /**< -x - Clear Tx_data_ready bit. */
#define HOSTIF_CLR_DATA_READY_REG_CLR_TX_DATA_READY_POS                                             (4UL)
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX3_DATA_READY_MASK                                           (0x00000008UL) /**< -x - Clear Rx3_data_ready bit. */
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX3_DATA_READY_POS                                            (3UL)
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX2_DATA_READY_MASK                                           (0x00000004UL) /**< -x - Clear Rx2_data_ready bit. */
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX2_DATA_READY_POS                                            (2UL)
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX1_DATA_READY_MASK                                           (0x00000002UL) /**< -x - Clear Rx1_data_ready bit. */
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX1_DATA_READY_POS                                            (1UL)
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX0_DATA_READY_MASK                                           (0x00000001UL) /**< -x - Clear Rx0_data_ready bit. */
#define HOSTIF_CLR_DATA_READY_REG_CLR_RX0_DATA_READY_POS                                            (0UL)
#define HOSTIF_CLR_DATA_READY_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Buffer status
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_DATA_READY_STATUS_REG                                                                (0x40020070UL) /**< r--*/
#define HOSTIF_DATA_READY_STATUS_REG_TX_DATA_READY_MASK                                             (0x00000010UL) /**< r- - Frame valid bit for TX buffer. 1 - buffer loaded with frame by FW to be sent to Host 0 - frame successfully sent to Host . */
#define HOSTIF_DATA_READY_STATUS_REG_TX_DATA_READY_POS                                              (4UL)
#define HOSTIF_DATA_READY_STATUS_REG_RX3_DATA_READY_MASK                                            (0x00000008UL) /**< r- - Frame valid bit for RX buffer 3.  1 - buffer contains an error-free frame received from Host  0 - frame has been processed by FW and buffer is free to receive a new frame.. */
#define HOSTIF_DATA_READY_STATUS_REG_RX3_DATA_READY_POS                                             (3UL)
#define HOSTIF_DATA_READY_STATUS_REG_RX2_DATA_READY_MASK                                            (0x00000004UL) /**< r- - Frame valid bit for RX buffer 2.  1 - buffer contains an error-free frame received from Host  0 - frame has been processed by FW and buffer is free to receive a new frame.. */
#define HOSTIF_DATA_READY_STATUS_REG_RX2_DATA_READY_POS                                             (2UL)
#define HOSTIF_DATA_READY_STATUS_REG_RX1_DATA_READY_MASK                                            (0x00000002UL) /**< r- - Frame valid bit for RX buffer 1.  1 - buffer contains an error-free frame received from Host 0 - frame has been processed by FW and buffer is free to receive a new frame.. */
#define HOSTIF_DATA_READY_STATUS_REG_RX1_DATA_READY_POS                                             (1UL)
#define HOSTIF_DATA_READY_STATUS_REG_RX0_DATA_READY_MASK                                            (0x00000001UL) /**< r- - Frame valid bit for RX buffer 0.  1 - buffer contains an error-free frame received from Host 0 - frame has been processed by FW and buffer is free to receive a new frame.. */
#define HOSTIF_DATA_READY_STATUS_REG_RX0_DATA_READY_POS                                             (0UL)
#define HOSTIF_DATA_READY_STATUS_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Debug received data
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_DBG_RX_REG                                                                           (0x40020074UL) /**< r--*/
#define HOSTIF_DBG_RX_REG_RX_REG_MASK                                                               (0xFFFFFFFFUL) /**< r- - Value of received bytes. */
#define HOSTIF_DBG_RX_REG_RX_REG_POS                                                                (0UL)
#define HOSTIF_DBG_RX_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Debug received address
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_DBG_RX_ADDR_REG                                                                      (0x40020078UL) /**< r--*/
#define HOSTIF_DBG_RX_ADDR_REG_WR_PTR_MASK                                                          (0x0000C000UL) /**< r- - Pointer of next data to write in DBG_RX_REG. */
#define HOSTIF_DBG_RX_ADDR_REG_WR_PTR_POS                                                           (14UL)
#define HOSTIF_DBG_RX_ADDR_REG_WR_ADDR_MASK                                                         (0x00003FFFUL) /**< r- - Next write address. */
#define HOSTIF_DBG_RX_ADDR_REG_WR_ADDR_POS                                                          (0UL)
#define HOSTIF_DBG_RX_ADDR_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Testbus select
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define HOSTIF_TBSEL_REG                                                                            (0x4002007CUL) /**< rwm*/
#define HOSTIF_TBSEL_REG_TESTBUS_SELECT_MASK                                                        (0x0000001FUL) /**< rw - Selects which set of test signals are routed to register HOSTIF_TBVALUE_REG and output hostif_testbus_obs[7:0]. */
#define HOSTIF_TBSEL_REG_TESTBUS_SELECT_POS                                                         (0UL)
#define HOSTIF_TBSEL_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Testbus output
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define HOSTIF_TBVALUE_REG                                                                          (0x40020080UL) /**< r-m*/
#define HOSTIF_TBVALUE_REG_TESTBUS_VALUE_MASK                                                       (0x000000FFUL) /**< r- - Selected set of test signals Also routed to output hostif_testbus_obs[7:0]. */
#define HOSTIF_TBVALUE_REG_TESTBUS_VALUE_POS                                                        (0UL)
#define HOSTIF_TBVALUE_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Spare register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define HOSTIF_SPARE_REG                                                                            (0x40020084UL) /**< rw-*/
#define HOSTIF_SPARE_REG_SPARE7_MASK                                                                (0x00000080UL) /**< rw - spare7 read/write. */
#define HOSTIF_SPARE_REG_SPARE7_POS                                                                 (7UL)
#define HOSTIF_SPARE_REG_SPARE6_MASK                                                                (0x00000040UL) /**< rw - spare6 read/write. */
#define HOSTIF_SPARE_REG_SPARE6_POS                                                                 (6UL)
#define HOSTIF_SPARE_REG_SPARE5_MASK                                                                (0x00000020UL) /**< rw - spare5 read/write. */
#define HOSTIF_SPARE_REG_SPARE5_POS                                                                 (5UL)
#define HOSTIF_SPARE_REG_SPARE4_MASK                                                                (0x00000010UL) /**< rw - spare4 read/write. */
#define HOSTIF_SPARE_REG_SPARE4_POS                                                                 (4UL)
#define HOSTIF_SPARE_REG_SPARE3_MASK                                                                (0x00000008UL) /**< r- - spare3 read only. */
#define HOSTIF_SPARE_REG_SPARE3_POS                                                                 (3UL)
#define HOSTIF_SPARE_REG_SPARE2_MASK                                                                (0x00000004UL) /**< r- - spare2 read only. */
#define HOSTIF_SPARE_REG_SPARE2_POS                                                                 (2UL)
#define HOSTIF_SPARE_REG_SPARE1_MASK                                                                (0x00000002UL) /**< r- - spare1 read only. */
#define HOSTIF_SPARE_REG_SPARE1_POS                                                                 (1UL)
#define HOSTIF_SPARE_REG_SPARE0_MASK                                                                (0x00000001UL) /**< r- - spare0 read only. */
#define HOSTIF_SPARE_REG_SPARE0_POS                                                                 (0UL)
#define HOSTIF_SPARE_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt clear enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define HOSTIF_INT_CLR_ENABLE_REG                                                                   (0x40023FD8UL) /**< -w-*/
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_SMBALERT_DONE_CLR_ENABLE_MASK                               (0x40000000UL) /**< -x - Clear enable for Smbus SMBALERT finished interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_SMBALERT_DONE_CLR_ENABLE_POS                                (30UL)
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_ARP_ASSIGN_CLR_ENABLE_MASK                                  (0x20000000UL) /**< -x - Clear enable for ARP assign address interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_ARP_ASSIGN_CLR_ENABLE_POS                                   (29UL)
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_ARP_RESET_CLR_ENABLE_MASK                                   (0x10000000UL) /**< -x - Clear enable for ARP Reset interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_ARP_RESET_CLR_ENABLE_POS                                    (28UL)
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_ARP_PREPARE_CLR_ENABLE_MASK                                 (0x08000000UL) /**< -x - Clear enable for ARP Prepare interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_SMBUS_ARP_PREPARE_CLR_ENABLE_POS                                  (27UL)
#define HOSTIF_INT_CLR_ENABLE_REG_HSU_RX_FER_CLR_ENABLE_MASK                                        (0x04000000UL) /**< -x - Clear enable for Rx framing error interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_HSU_RX_FER_CLR_ENABLE_POS                                         (26UL)
#define HOSTIF_INT_CLR_ENABLE_REG_BUFFER_CFG_CHANGED_ERROR_CLR_ENABLE_MASK                          (0x02000000UL) /**< -x - Clear enable for buffer_cfg_changed interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_BUFFER_CFG_CHANGED_ERROR_CLR_ENABLE_POS                           (25UL)
#define HOSTIF_INT_CLR_ENABLE_REG_AHB_WR_SLOW_CLR_ENABLE_MASK                                       (0x01000000UL) /**< -x - Clear enable for AHB wr slow interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_AHB_WR_SLOW_CLR_ENABLE_POS                                        (24UL)
#define HOSTIF_INT_CLR_ENABLE_REG_AHB_RD_SLOW_CLR_ENABLE_MASK                                       (0x00800000UL) /**< -x - Clear enable for AHB rd slow interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_AHB_RD_SLOW_CLR_ENABLE_POS                                        (23UL)
#define HOSTIF_INT_CLR_ENABLE_REG_AHB_ERROR_CLR_ENABLE_MASK                                         (0x00400000UL) /**< -x - Clear enable for AHB error interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_AHB_ERROR_CLR_ENABLE_POS                                          (22UL)
#define HOSTIF_INT_CLR_ENABLE_REG_WATERLEVEL_REACHED_CLR_ENABLE_MASK                                (0x00200000UL) /**< -x - Clear enable for waterlevel reached interrupt 0 - no effect. */
#define HOSTIF_INT_CLR_ENABLE_REG_WATERLEVEL_REACHED_CLR_ENABLE_POS                                 (21UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_BUFFER_OVERFLOW_CLR_ENABLE_MASK                               (0x00100000UL) /**< -x - Clear enable for max buffer size interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_BUFFER_OVERFLOW_CLR_ENABLE_POS                                (20UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_BUFFER_OVERFLOW_CLR_ENABLE_MASK                               (0x00080000UL) /**< -x - Clear enable for max buffer size interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_BUFFER_OVERFLOW_CLR_ENABLE_POS                                (19UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_BUFFER_OVERFLOW_CLR_ENABLE_MASK                               (0x00040000UL) /**< -x - Clear enable for max buffer size interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_BUFFER_OVERFLOW_CLR_ENABLE_POS                                (18UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_BUFFER_OVERFLOW_CLR_ENABLE_MASK                               (0x00020000UL) /**< -x - Clear enable for max buffer size interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_BUFFER_OVERFLOW_CLR_ENABLE_POS                                (17UL)
#define HOSTIF_INT_CLR_ENABLE_REG_CRC_NOK_CLR_ENABLE_MASK                                           (0x00010000UL) /**< -x - Clear enable for data-link Layer CRC error interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_CRC_NOK_CLR_ENABLE_POS                                            (16UL)
#define HOSTIF_INT_CLR_ENABLE_REG_TX_TIMEOUT_CLR_ENABLE_MASK                                        (0x00008000UL) /**< -x - Clear enable for inter-character timeout (TIC) exceeded on transmitted frame interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_TX_TIMEOUT_CLR_ENABLE_POS                                         (15UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_FRAME_OVERFLOW_CLR_ENABLE_MASK                                (0x00004000UL) /**< -x - Clear enable for frame overflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_FRAME_OVERFLOW_CLR_ENABLE_POS                                 (14UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_FRAME_OVERFLOW_CLR_ENABLE_MASK                                (0x00002000UL) /**< -x - Clear enable for frame overflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_FRAME_OVERFLOW_CLR_ENABLE_POS                                 (13UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_FRAME_OVERFLOW_CLR_ENABLE_MASK                                (0x00001000UL) /**< -x - Clear enable for frame overflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_FRAME_OVERFLOW_CLR_ENABLE_POS                                 (12UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_FRAME_OVERFLOW_CLR_ENABLE_MASK                                (0x00000800UL) /**< -x - Clear enable for frame overflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_FRAME_OVERFLOW_CLR_ENABLE_POS                                 (11UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_FRAME_UNDERFLOW_CLR_ENABLE_MASK                               (0x00000400UL) /**< -x - Clear enable for frame underflow interrupt for RX buffer in 3. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_FRAME_UNDERFLOW_CLR_ENABLE_POS                                (10UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_FRAME_UNDERFLOW_CLR_ENABLE_MASK                               (0x00000200UL) /**< -x - Clear enable for frame underflow interrupt for RX buffer in 2. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_FRAME_UNDERFLOW_CLR_ENABLE_POS                                (9UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_FRAME_UNDERFLOW_CLR_ENABLE_MASK                               (0x00000100UL) /**< -x - Clear enable for frame underflow interrupt for RX buffer in 1. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_FRAME_UNDERFLOW_CLR_ENABLE_POS                                (8UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_FRAME_UNDERFLOW_CLR_ENABLE_MASK                               (0x00000080UL) /**< -x - Clear enable for frame underflow interrupt for RX buffer in 0. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_FRAME_UNDERFLOW_CLR_ENABLE_POS                                (7UL)
#define HOSTIF_INT_CLR_ENABLE_REG_TX_FRAME_NOT_AVAILABLE_CLR_ENABLE_MASK                            (0x00000040UL) /**< -x - Clear enable for TX frame not available interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_TX_FRAME_NOT_AVAILABLE_CLR_ENABLE_POS                             (6UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX_BUFFER_NOT_AVAILABLE_CLR_ENABLE_MASK                           (0x00000020UL) /**< -x - Clear enable for no receive buffers available interrupt. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX_BUFFER_NOT_AVAILABLE_CLR_ENABLE_POS                            (5UL)
#define HOSTIF_INT_CLR_ENABLE_REG_EOT_CLR_ENABLE_MASK                                               (0x00000010UL) /**< -x - Clear enable for EOT interrupt 0. */
#define HOSTIF_INT_CLR_ENABLE_REG_EOT_CLR_ENABLE_POS                                                (4UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_EOR_CLR_ENABLE_MASK                                           (0x00000008UL) /**< -x - Clear enable for End of Reception in buffer 3. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX3_EOR_CLR_ENABLE_POS                                            (3UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_EOR_CLR_ENABLE_MASK                                           (0x00000004UL) /**< -x - Clear enable for End of Reception in buffer 2. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX2_EOR_CLR_ENABLE_POS                                            (2UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_EOR_CLR_ENABLE_MASK                                           (0x00000002UL) /**< -x - Clear enable for End of Reception in buffer 1. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX1_EOR_CLR_ENABLE_POS                                            (1UL)
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_EOR_CLR_ENABLE_MASK                                           (0x00000001UL) /**< -x - Clear enable for End of Reception in buffer 0. */
#define HOSTIF_INT_CLR_ENABLE_REG_RX0_EOR_CLR_ENABLE_POS                                            (0UL)
#define HOSTIF_INT_CLR_ENABLE_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt set enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define HOSTIF_INT_SET_ENABLE_REG                                                                   (0x40023FDCUL) /**< -w-*/
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_SMBALERT_DONE_SET_ENABLE_MASK                               (0x40000000UL) /**< -x - Set enable for Smbus SMBALERT finished interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_SMBALERT_DONE_SET_ENABLE_POS                                (30UL)
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_ARP_ASSIGN_SET_ENABLE_MASK                                  (0x20000000UL) /**< -x - Set enable for ARP assign address interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_ARP_ASSIGN_SET_ENABLE_POS                                   (29UL)
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_ARP_RESET_SET_ENABLE_MASK                                   (0x10000000UL) /**< -x - Set enable for ARP Reset interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_ARP_RESET_SET_ENABLE_POS                                    (28UL)
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_ARP_PREPARE_SET_ENABLE_MASK                                 (0x08000000UL) /**< -x - Set enable for ARP Prepare interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_SMBUS_ARP_PREPARE_SET_ENABLE_POS                                  (27UL)
#define HOSTIF_INT_SET_ENABLE_REG_HSU_RX_FER_SET_ENABLE_MASK                                        (0x04000000UL) /**< -x - Set enable for Rx framing error interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_HSU_RX_FER_SET_ENABLE_POS                                         (26UL)
#define HOSTIF_INT_SET_ENABLE_REG_BUFFER_CFG_CHANGED_ERROR_SET_ENABLE_MASK                          (0x02000000UL) /**< -x - Set enable for buffer_cfg_changed interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_BUFFER_CFG_CHANGED_ERROR_SET_ENABLE_POS                           (25UL)
#define HOSTIF_INT_SET_ENABLE_REG_AHB_WR_SLOW_SET_ENABLE_MASK                                       (0x01000000UL) /**< -x - Set enable for AHB wr slow interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_AHB_WR_SLOW_SET_ENABLE_POS                                        (24UL)
#define HOSTIF_INT_SET_ENABLE_REG_AHB_RD_SLOW_SET_ENABLE_MASK                                       (0x00800000UL) /**< -x - Set enable for AHB rd slow interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_AHB_RD_SLOW_SET_ENABLE_POS                                        (23UL)
#define HOSTIF_INT_SET_ENABLE_REG_AHB_ERROR_SET_ENABLE_MASK                                         (0x00400000UL) /**< -x - Set enable for AHB error interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_AHB_ERROR_SET_ENABLE_POS                                          (22UL)
#define HOSTIF_INT_SET_ENABLE_REG_WATERLEVEL_REACHED_SET_ENABLE_MASK                                (0x00200000UL) /**< -x - Set enable for waterlevel reached interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_WATERLEVEL_REACHED_SET_ENABLE_POS                                 (21UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX3_BUFFER_OVERFLOW_SET_ENABLE_MASK                               (0x00100000UL) /**< -x - Set enable for max buffer size interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_SET_ENABLE_REG_RX3_BUFFER_OVERFLOW_SET_ENABLE_POS                                (20UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX2_BUFFER_OVERFLOW_SET_ENABLE_MASK                               (0x00080000UL) /**< -x - Set enable for max buffer size interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_SET_ENABLE_REG_RX2_BUFFER_OVERFLOW_SET_ENABLE_POS                                (19UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX1_BUFFER_OVERFLOW_SET_ENABLE_MASK                               (0x00040000UL) /**< -x - Set enable for max buffer size interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_SET_ENABLE_REG_RX1_BUFFER_OVERFLOW_SET_ENABLE_POS                                (18UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX0_BUFFER_OVERFLOW_SET_ENABLE_MASK                               (0x00020000UL) /**< -x - Set enable for max buffer size interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_SET_ENABLE_REG_RX0_BUFFER_OVERFLOW_SET_ENABLE_POS                                (17UL)
#define HOSTIF_INT_SET_ENABLE_REG_CRC_NOK_SET_ENABLE_MASK                                           (0x00010000UL) /**< -x - Set enable for data-link Layer CRC error interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_CRC_NOK_SET_ENABLE_POS                                            (16UL)
#define HOSTIF_INT_SET_ENABLE_REG_TX_TIMEOUT_SET_ENABLE_MASK                                        (0x00008000UL) /**< -x - Set enable for inter-character timeout (TIC) exceeded on transmitted frame interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_TX_TIMEOUT_SET_ENABLE_POS                                         (15UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX3_FRAME_OVERFLOW_SET_ENABLE_MASK                                (0x00004000UL) /**< -x - Set enable for frame overflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_SET_ENABLE_REG_RX3_FRAME_OVERFLOW_SET_ENABLE_POS                                 (14UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX2_FRAME_OVERFLOW_SET_ENABLE_MASK                                (0x00002000UL) /**< -x - Set enable for frame overflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_SET_ENABLE_REG_RX2_FRAME_OVERFLOW_SET_ENABLE_POS                                 (13UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX1_FRAME_OVERFLOW_SET_ENABLE_MASK                                (0x00001000UL) /**< -x - Set enable for frame overflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_SET_ENABLE_REG_RX1_FRAME_OVERFLOW_SET_ENABLE_POS                                 (12UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX0_FRAME_OVERFLOW_SET_ENABLE_MASK                                (0x00000800UL) /**< -x - Set enable for frame overflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_SET_ENABLE_REG_RX0_FRAME_OVERFLOW_SET_ENABLE_POS                                 (11UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX3_FRAME_UNDERFLOW_SET_ENABLE_MASK                               (0x00000400UL) /**< -x - Set enable for frame underflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_SET_ENABLE_REG_RX3_FRAME_UNDERFLOW_SET_ENABLE_POS                                (10UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX2_FRAME_UNDERFLOW_SET_ENABLE_MASK                               (0x00000200UL) /**< -x - Set enable for frame underflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_SET_ENABLE_REG_RX2_FRAME_UNDERFLOW_SET_ENABLE_POS                                (9UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX1_FRAME_UNDERFLOW_SET_ENABLE_MASK                               (0x00000100UL) /**< -x - Set enable for frame underflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_SET_ENABLE_REG_RX1_FRAME_UNDERFLOW_SET_ENABLE_POS                                (8UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX0_FRAME_UNDERFLOW_SET_ENABLE_MASK                               (0x00000080UL) /**< -x - Set enable for frame underflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_SET_ENABLE_REG_RX0_FRAME_UNDERFLOW_SET_ENABLE_POS                                (7UL)
#define HOSTIF_INT_SET_ENABLE_REG_TX_FRAME_NOT_AVAILABLE_SET_ENABLE_MASK                            (0x00000040UL) /**< -x - Set enable for TX frame not available interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_TX_FRAME_NOT_AVAILABLE_SET_ENABLE_POS                             (6UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX_BUFFER_NOT_AVAILABLE_SET_ENABLE_MASK                           (0x00000020UL) /**< -x - Set enable for no receive buffers available interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_RX_BUFFER_NOT_AVAILABLE_SET_ENABLE_POS                            (5UL)
#define HOSTIF_INT_SET_ENABLE_REG_EOT_SET_ENABLE_MASK                                               (0x00000010UL) /**< -x - Set enable for EOT interrupt. */
#define HOSTIF_INT_SET_ENABLE_REG_EOT_SET_ENABLE_POS                                                (4UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX3_EOR_SET_ENABLE_MASK                                           (0x00000008UL) /**< -x - Set enable for End of Reception in buffer 3. */
#define HOSTIF_INT_SET_ENABLE_REG_RX3_EOR_SET_ENABLE_POS                                            (3UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX2_EOR_SET_ENABLE_MASK                                           (0x00000004UL) /**< -x - Set enable for End of Reception in buffer 2. */
#define HOSTIF_INT_SET_ENABLE_REG_RX2_EOR_SET_ENABLE_POS                                            (2UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX1_EOR_SET_ENABLE_MASK                                           (0x00000002UL) /**< -x - Set enable for End of Reception in buffer 1. */
#define HOSTIF_INT_SET_ENABLE_REG_RX1_EOR_SET_ENABLE_POS                                            (1UL)
#define HOSTIF_INT_SET_ENABLE_REG_RX0_EOR_SET_ENABLE_MASK                                           (0x00000001UL) /**< -x - Set enable for End of Reception in buffer 0. */
#define HOSTIF_INT_SET_ENABLE_REG_RX0_EOR_SET_ENABLE_POS                                            (0UL)
#define HOSTIF_INT_SET_ENABLE_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_INT_STATUS_REG                                                                       (0x40023FE0UL) /**< r--*/
#define HOSTIF_INT_STATUS_REG_SMBUS_SMBALERT_DONE_STATUS_MASK                                       (0x40000000UL) /**< r- - Smbus SMBALERT finished interrupt. */
#define HOSTIF_INT_STATUS_REG_SMBUS_SMBALERT_DONE_STATUS_POS                                        (30UL)
#define HOSTIF_INT_STATUS_REG_SMBUS_ARP_ASSIGN_STATUS_MASK                                          (0x20000000UL) /**< r- - Smbus ARP Assign address command received interrupt. */
#define HOSTIF_INT_STATUS_REG_SMBUS_ARP_ASSIGN_STATUS_POS                                           (29UL)
#define HOSTIF_INT_STATUS_REG_SMBUS_ARP_RESET_STATUS_MASK                                           (0x10000000UL) /**< r- - Smbus ARP Reset command received interrupt. */
#define HOSTIF_INT_STATUS_REG_SMBUS_ARP_RESET_STATUS_POS                                            (28UL)
#define HOSTIF_INT_STATUS_REG_SMBUS_ARP_PREPARE_STATUS_MASK                                         (0x08000000UL) /**< r- - Smbus ARP Prepare command received interrupt. */
#define HOSTIF_INT_STATUS_REG_SMBUS_ARP_PREPARE_STATUS_POS                                          (27UL)
#define HOSTIF_INT_STATUS_REG_HSU_RX_FER_STATUS_MASK                                                (0x04000000UL) /**< r- - Rx framing error interrupt. */
#define HOSTIF_INT_STATUS_REG_HSU_RX_FER_STATUS_POS                                                 (26UL)
#define HOSTIF_INT_STATUS_REG_BUFFER_CFG_CHANGED_ERROR_STATUS_MASK                                  (0x02000000UL) /**< r- - Buffer_cfg_changed interrupt. */
#define HOSTIF_INT_STATUS_REG_BUFFER_CFG_CHANGED_ERROR_STATUS_POS                                   (25UL)
#define HOSTIF_INT_STATUS_REG_AHB_WR_SLOW_STATUS_MASK                                               (0x01000000UL) /**< r- - AHB wr slow interrupt. */
#define HOSTIF_INT_STATUS_REG_AHB_WR_SLOW_STATUS_POS                                                (24UL)
#define HOSTIF_INT_STATUS_REG_AHB_RD_SLOW_STATUS_MASK                                               (0x00800000UL) /**< r- - AHB rd slow interrupt. */
#define HOSTIF_INT_STATUS_REG_AHB_RD_SLOW_STATUS_POS                                                (23UL)
#define HOSTIF_INT_STATUS_REG_AHB_ERROR_STATUS_MASK                                                 (0x00400000UL) /**< r- - AHB error interrupt. */
#define HOSTIF_INT_STATUS_REG_AHB_ERROR_STATUS_POS                                                  (22UL)
#define HOSTIF_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_MASK                                        (0x00200000UL) /**< r- - waterlevel reached interrupt status. */
#define HOSTIF_INT_STATUS_REG_WATERLEVEL_REACHED_STATUS_POS                                         (21UL)
#define HOSTIF_INT_STATUS_REG_RX3_BUFFER_OVERFLOW_STATUS_MASK                                       (0x00100000UL) /**< r- - Max buffer size interrupt status for RX buffer in buffer 3. */
#define HOSTIF_INT_STATUS_REG_RX3_BUFFER_OVERFLOW_STATUS_POS                                        (20UL)
#define HOSTIF_INT_STATUS_REG_RX2_BUFFER_OVERFLOW_STATUS_MASK                                       (0x00080000UL) /**< r- - Max buffer size interrupt status for RX buffer in buffer 2. */
#define HOSTIF_INT_STATUS_REG_RX2_BUFFER_OVERFLOW_STATUS_POS                                        (19UL)
#define HOSTIF_INT_STATUS_REG_RX1_BUFFER_OVERFLOW_STATUS_MASK                                       (0x00040000UL) /**< r- - Max buffer size interrupt status for RX buffer in buffer 1. */
#define HOSTIF_INT_STATUS_REG_RX1_BUFFER_OVERFLOW_STATUS_POS                                        (18UL)
#define HOSTIF_INT_STATUS_REG_RX0_BUFFER_OVERFLOW_STATUS_MASK                                       (0x00020000UL) /**< r- - Max buffer size interrupt status for RX buffer in buffer 0 . */
#define HOSTIF_INT_STATUS_REG_RX0_BUFFER_OVERFLOW_STATUS_POS                                        (17UL)
#define HOSTIF_INT_STATUS_REG_CRC_NOK_STATUS_MASK                                                   (0x00010000UL) /**< r- - Data-link Layer CRC error interrupt status. */
#define HOSTIF_INT_STATUS_REG_CRC_NOK_STATUS_POS                                                    (16UL)
#define HOSTIF_INT_STATUS_REG_TX_TIMEOUT_STATUS_MASK                                                (0x00008000UL) /**< r- - Inter-character timeout (TIC) exceeded on transmitted frame interrupt status. */
#define HOSTIF_INT_STATUS_REG_TX_TIMEOUT_STATUS_POS                                                 (15UL)
#define HOSTIF_INT_STATUS_REG_RX3_FRAME_OVERFLOW_STATUS_MASK                                        (0x00004000UL) /**< r- - Frame overflow interrupt status for RX buffer in buffer 3. */
#define HOSTIF_INT_STATUS_REG_RX3_FRAME_OVERFLOW_STATUS_POS                                         (14UL)
#define HOSTIF_INT_STATUS_REG_RX2_FRAME_OVERFLOW_STATUS_MASK                                        (0x00002000UL) /**< r- - Frame overflow interrupt status for RX buffer in buffer 2. */
#define HOSTIF_INT_STATUS_REG_RX2_FRAME_OVERFLOW_STATUS_POS                                         (13UL)
#define HOSTIF_INT_STATUS_REG_RX1_FRAME_OVERFLOW_STATUS_MASK                                        (0x00001000UL) /**< r- - Frame overflow interrupt status for RX buffer in buffer 1. */
#define HOSTIF_INT_STATUS_REG_RX1_FRAME_OVERFLOW_STATUS_POS                                         (12UL)
#define HOSTIF_INT_STATUS_REG_RX0_FRAME_OVERFLOW_STATUS_MASK                                        (0x00000800UL) /**< r- - Frame overflow interrupt status for RX buffer in buffer 0. */
#define HOSTIF_INT_STATUS_REG_RX0_FRAME_OVERFLOW_STATUS_POS                                         (11UL)
#define HOSTIF_INT_STATUS_REG_RX3_FRAME_UNDERFLOW_STATUS_MASK                                       (0x00000400UL) /**< r- - Frame underflow interrupt status for RX buffer in buffer 3. */
#define HOSTIF_INT_STATUS_REG_RX3_FRAME_UNDERFLOW_STATUS_POS                                        (10UL)
#define HOSTIF_INT_STATUS_REG_RX2_FRAME_UNDERFLOW_STATUS_MASK                                       (0x00000200UL) /**< r- - Frame underflow interrupt status for RX buffer in buffer 2. */
#define HOSTIF_INT_STATUS_REG_RX2_FRAME_UNDERFLOW_STATUS_POS                                        (9UL)
#define HOSTIF_INT_STATUS_REG_RX1_FRAME_UNDERFLOW_STATUS_MASK                                       (0x00000100UL) /**< r- - Frame underflow interrupt status for RX buffer in buffer 1. */
#define HOSTIF_INT_STATUS_REG_RX1_FRAME_UNDERFLOW_STATUS_POS                                        (8UL)
#define HOSTIF_INT_STATUS_REG_RX0_FRAME_UNDERFLOW_STATUS_MASK                                       (0x00000080UL) /**< r- - Frame underflow interrupt status for RX buffer in buffer 0. */
#define HOSTIF_INT_STATUS_REG_RX0_FRAME_UNDERFLOW_STATUS_POS                                        (7UL)
#define HOSTIF_INT_STATUS_REG_TX_FRAME_NOT_AVAILABLE_STATUS_MASK                                    (0x00000040UL) /**< r- - TX frame not available interrupt status. */
#define HOSTIF_INT_STATUS_REG_TX_FRAME_NOT_AVAILABLE_STATUS_POS                                     (6UL)
#define HOSTIF_INT_STATUS_REG_RX_BUFFER_NOT_AVAILABLE_STATUS_MASK                                   (0x00000020UL) /**< r- - No receive buffers available interrupt status. */
#define HOSTIF_INT_STATUS_REG_RX_BUFFER_NOT_AVAILABLE_STATUS_POS                                    (5UL)
#define HOSTIF_INT_STATUS_REG_EOT_STATUS_MASK                                                       (0x00000010UL) /**< r- - EOT interrupt status. */
#define HOSTIF_INT_STATUS_REG_EOT_STATUS_POS                                                        (4UL)
#define HOSTIF_INT_STATUS_REG_RX3_EOR_STATUS_MASK                                                   (0x00000008UL) /**< r- - End of Reception in buffer 3 interrupt status. */
#define HOSTIF_INT_STATUS_REG_RX3_EOR_STATUS_POS                                                    (3UL)
#define HOSTIF_INT_STATUS_REG_RX2_EOR_STATUS_MASK                                                   (0x00000004UL) /**< r- - End of Reception in buffer 2 interrupt status. */
#define HOSTIF_INT_STATUS_REG_RX2_EOR_STATUS_POS                                                    (2UL)
#define HOSTIF_INT_STATUS_REG_RX1_EOR_STATUS_MASK                                                   (0x00000002UL) /**< r- - End of Reception in buffer 1 interrupt status. */
#define HOSTIF_INT_STATUS_REG_RX1_EOR_STATUS_POS                                                    (1UL)
#define HOSTIF_INT_STATUS_REG_RX0_EOR_STATUS_MASK                                                   (0x00000001UL) /**< r- - End of Reception in buffer 0 interrupt status. */
#define HOSTIF_INT_STATUS_REG_RX0_EOR_STATUS_POS                                                    (0UL)
#define HOSTIF_INT_STATUS_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt enable register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define HOSTIF_INT_ENABLE_REG                                                                       (0x40023FE4UL) /**< r--*/
#define HOSTIF_INT_ENABLE_REG_SMBUS_SMBALERT_DONE_ENABLE_MASK                                       (0x40000000UL) /**< r- - Smbus SMBALERT finished interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_SMBUS_SMBALERT_DONE_ENABLE_POS                                        (30UL)
#define HOSTIF_INT_ENABLE_REG_SMBUS_ARP_ASSIGN_ENABLE_MASK                                          (0x20000000UL) /**< r- - Smbus ARP Assign address command received interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_SMBUS_ARP_ASSIGN_ENABLE_POS                                           (29UL)
#define HOSTIF_INT_ENABLE_REG_SMBUS_ARP_RESET_ENABLE_MASK                                           (0x10000000UL) /**< r- - Smbus ARP Reset command received interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_SMBUS_ARP_RESET_ENABLE_POS                                            (28UL)
#define HOSTIF_INT_ENABLE_REG_SMBUS_ARP_PREPARE_ENABLE_MASK                                         (0x08000000UL) /**< r- - Smbus ARP Prepare command received interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_SMBUS_ARP_PREPARE_ENABLE_POS                                          (27UL)
#define HOSTIF_INT_ENABLE_REG_HSU_RX_FER_ENABLE_MASK                                                (0x04000000UL) /**< r- - Rx framing error interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_HSU_RX_FER_ENABLE_POS                                                 (26UL)
#define HOSTIF_INT_ENABLE_REG_BUFFER_CFG_CHANGED_ERROR_ENABLE_MASK                                  (0x02000000UL) /**< r- - Buffer_cfg_changed interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_BUFFER_CFG_CHANGED_ERROR_ENABLE_POS                                   (25UL)
#define HOSTIF_INT_ENABLE_REG_AHB_WR_SLOW_ENABLE_MASK                                               (0x01000000UL) /**< r- - AHB wr slow interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_AHB_WR_SLOW_ENABLE_POS                                                (24UL)
#define HOSTIF_INT_ENABLE_REG_AHB_RD_SLOW_ENABLE_MASK                                               (0x00800000UL) /**< r- - AHB rd slow interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_AHB_RD_SLOW_ENABLE_POS                                                (23UL)
#define HOSTIF_INT_ENABLE_REG_AHB_ERROR_ENABLE_MASK                                                 (0x00400000UL) /**< r- - AHB error interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_AHB_ERROR_ENABLE_POS                                                  (22UL)
#define HOSTIF_INT_ENABLE_REG_WATERLEVEL_REACHED_ENABLE_MASK                                        (0x00200000UL) /**< r- - waterlevel reached interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_WATERLEVEL_REACHED_ENABLE_POS                                         (21UL)
#define HOSTIF_INT_ENABLE_REG_RX3_BUFFER_OVERFLOW_ENABLE_MASK                                       (0x00100000UL) /**< r- - Max buffer size interrupt enable for RX buffer in buffer 3. */
#define HOSTIF_INT_ENABLE_REG_RX3_BUFFER_OVERFLOW_ENABLE_POS                                        (20UL)
#define HOSTIF_INT_ENABLE_REG_RX2_BUFFER_OVERFLOW_ENABLE_MASK                                       (0x00080000UL) /**< r- - Max buffer size interrupt enable for RX buffer in buffer 2. */
#define HOSTIF_INT_ENABLE_REG_RX2_BUFFER_OVERFLOW_ENABLE_POS                                        (19UL)
#define HOSTIF_INT_ENABLE_REG_RX1_BUFFER_OVERFLOW_ENABLE_MASK                                       (0x00040000UL) /**< r- - Max buffer size interrupt enable for RX buffer in buffer 1. */
#define HOSTIF_INT_ENABLE_REG_RX1_BUFFER_OVERFLOW_ENABLE_POS                                        (18UL)
#define HOSTIF_INT_ENABLE_REG_RX0_BUFFER_OVERFLOW_ENABLE_MASK                                       (0x00020000UL) /**< r- - Max buffer size interrupt enable for RX buffer in buffer 0. */
#define HOSTIF_INT_ENABLE_REG_RX0_BUFFER_OVERFLOW_ENABLE_POS                                        (17UL)
#define HOSTIF_INT_ENABLE_REG_CRC_NOK_ENABLE_MASK                                                   (0x00010000UL) /**< r- - Data-link Layer CRC error interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_CRC_NOK_ENABLE_POS                                                    (16UL)
#define HOSTIF_INT_ENABLE_REG_TX_TIMEOUT_ENABLE_MASK                                                (0x00008000UL) /**< r- - Inter-character timeout (TIC) exceeded on transmitted frame interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_TX_TIMEOUT_ENABLE_POS                                                 (15UL)
#define HOSTIF_INT_ENABLE_REG_RX3_FRAME_OVERFLOW_ENABLE_MASK                                        (0x00004000UL) /**< r- - Frame overflow interrupt enable for RX buffer in buffer 3. */
#define HOSTIF_INT_ENABLE_REG_RX3_FRAME_OVERFLOW_ENABLE_POS                                         (14UL)
#define HOSTIF_INT_ENABLE_REG_RX2_FRAME_OVERFLOW_ENABLE_MASK                                        (0x00002000UL) /**< r- - Frame overflow interrupt enable for RX buffer in buffer 2. */
#define HOSTIF_INT_ENABLE_REG_RX2_FRAME_OVERFLOW_ENABLE_POS                                         (13UL)
#define HOSTIF_INT_ENABLE_REG_RX1_FRAME_OVERFLOW_ENABLE_MASK                                        (0x00001000UL) /**< r- - Frame overflow interrupt enable for RX buffer in buffer 1. */
#define HOSTIF_INT_ENABLE_REG_RX1_FRAME_OVERFLOW_ENABLE_POS                                         (12UL)
#define HOSTIF_INT_ENABLE_REG_RX0_FRAME_OVERFLOW_ENABLE_MASK                                        (0x00000800UL) /**< r- - Frame overflow interrupt enable for RX buffer in buffer 0. */
#define HOSTIF_INT_ENABLE_REG_RX0_FRAME_OVERFLOW_ENABLE_POS                                         (11UL)
#define HOSTIF_INT_ENABLE_REG_RX3_FRAME_UNDERFLOW_ENABLE_MASK                                       (0x00000400UL) /**< r- - Frame underflow interrupt enable for RX buffer in buffer 3. */
#define HOSTIF_INT_ENABLE_REG_RX3_FRAME_UNDERFLOW_ENABLE_POS                                        (10UL)
#define HOSTIF_INT_ENABLE_REG_RX2_FRAME_UNDERFLOW_ENABLE_MASK                                       (0x00000200UL) /**< r- - Frame underflow interrupt enable for RX buffer in buffer 2. */
#define HOSTIF_INT_ENABLE_REG_RX2_FRAME_UNDERFLOW_ENABLE_POS                                        (9UL)
#define HOSTIF_INT_ENABLE_REG_RX1_FRAME_UNDERFLOW_ENABLE_MASK                                       (0x00000100UL) /**< r- - Frame underflow interrupt enable for RX buffer in buffer 1. */
#define HOSTIF_INT_ENABLE_REG_RX1_FRAME_UNDERFLOW_ENABLE_POS                                        (8UL)
#define HOSTIF_INT_ENABLE_REG_RX0_FRAME_UNDERFLOW_ENABLE_MASK                                       (0x00000080UL) /**< r- - Frame underflow interrupt enable for RX buffer in buffer 0. */
#define HOSTIF_INT_ENABLE_REG_RX0_FRAME_UNDERFLOW_ENABLE_POS                                        (7UL)
#define HOSTIF_INT_ENABLE_REG_TX_FRAME_NOT_AVAILABLE_ENABLE_MASK                                    (0x00000040UL) /**< r- - TX frame not available interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_TX_FRAME_NOT_AVAILABLE_ENABLE_POS                                     (6UL)
#define HOSTIF_INT_ENABLE_REG_RX_BUFFER_NOT_AVAILABLE_ENABLE_MASK                                   (0x00000020UL) /**< r- - No receive buffers available interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_RX_BUFFER_NOT_AVAILABLE_ENABLE_POS                                    (5UL)
#define HOSTIF_INT_ENABLE_REG_EOT_ENABLE_MASK                                                       (0x00000010UL) /**< r- - EOT interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_EOT_ENABLE_POS                                                        (4UL)
#define HOSTIF_INT_ENABLE_REG_RX3_EOR_ENABLE_MASK                                                   (0x00000008UL) /**< r- - End of Reception in buffer 3 interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_RX3_EOR_ENABLE_POS                                                    (3UL)
#define HOSTIF_INT_ENABLE_REG_RX2_EOR_ENABLE_MASK                                                   (0x00000004UL) /**< r- - End of Reception in buffer 2 interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_RX2_EOR_ENABLE_POS                                                    (2UL)
#define HOSTIF_INT_ENABLE_REG_RX1_EOR_ENABLE_MASK                                                   (0x00000002UL) /**< r- - End of Reception in buffer 1 interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_RX1_EOR_ENABLE_POS                                                    (1UL)
#define HOSTIF_INT_ENABLE_REG_RX0_EOR_ENABLE_MASK                                                   (0x00000001UL) /**< r- - End of Reception in buffer 0 interrupt enable. */
#define HOSTIF_INT_ENABLE_REG_RX0_EOR_ENABLE_POS                                                    (0UL)
#define HOSTIF_INT_ENABLE_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt clear status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define HOSTIF_INT_CLR_STATUS_REG                                                                   (0x40023FE8UL) /**< -w-*/
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_SMBALERT_DONE_CLR_STATUS_MASK                               (0x40000000UL) /**< -x - Clear Smbus SMBALERT finished interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_SMBALERT_DONE_CLR_STATUS_POS                                (30UL)
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_ARP_ASSIGN_CLR_STATUS_MASK                                  (0x20000000UL) /**< -x - Clear Smbus ARP Assign address command received interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_ARP_ASSIGN_CLR_STATUS_POS                                   (29UL)
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_ARP_RESET_CLR_STATUS_MASK                                   (0x10000000UL) /**< -x - Clear Smbus ARP Reset command received interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_ARP_RESET_CLR_STATUS_POS                                    (28UL)
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_ARP_PREPARE_CLR_STATUS_MASK                                 (0x08000000UL) /**< -x - Clear Smbus ARP Prepare command received interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_SMBUS_ARP_PREPARE_CLR_STATUS_POS                                  (27UL)
#define HOSTIF_INT_CLR_STATUS_REG_HSU_RX_FER_CLR_STATUS_MASK                                        (0x04000000UL) /**< -x - Clear Rx framing error interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_HSU_RX_FER_CLR_STATUS_POS                                         (26UL)
#define HOSTIF_INT_CLR_STATUS_REG_BUFFER_CFG_CHANGED_ERROR_CLR_STATUS_MASK                          (0x02000000UL) /**< -x - Clear Buffer_cfg_changed interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_BUFFER_CFG_CHANGED_ERROR_CLR_STATUS_POS                           (25UL)
#define HOSTIF_INT_CLR_STATUS_REG_AHB_WR_SLOW_CLR_STATUS_MASK                                       (0x01000000UL) /**< -x - Clear AHB wr slow interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_AHB_WR_SLOW_CLR_STATUS_POS                                        (24UL)
#define HOSTIF_INT_CLR_STATUS_REG_AHB_RD_SLOW_CLR_STATUS_MASK                                       (0x00800000UL) /**< -x - Clear AHB rd slow interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_AHB_RD_SLOW_CLR_STATUS_POS                                        (23UL)
#define HOSTIF_INT_CLR_STATUS_REG_AHB_ERROR_CLR_STATUS_MASK                                         (0x00400000UL) /**< -x - Clear AHB error interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_AHB_ERROR_CLR_STATUS_POS                                          (22UL)
#define HOSTIF_INT_CLR_STATUS_REG_WATERLEVEL_REACHED_CLR_STATUS_MASK                                (0x00200000UL) /**< -x - Clear waterlevel reached interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_WATERLEVEL_REACHED_CLR_STATUS_POS                                 (21UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX3_BUFFER_OVERFLOW_CLR_STATUS_MASK                               (0x00100000UL) /**< -x - Clear max buffer size interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_CLR_STATUS_REG_RX3_BUFFER_OVERFLOW_CLR_STATUS_POS                                (20UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX2_BUFFER_OVERFLOW_CLR_STATUS_MASK                               (0x00080000UL) /**< -x - Clear max buffer size interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_CLR_STATUS_REG_RX2_BUFFER_OVERFLOW_CLR_STATUS_POS                                (19UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX1_BUFFER_OVERFLOW_CLR_STATUS_MASK                               (0x00040000UL) /**< -x - Clear max buffer size interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_CLR_STATUS_REG_RX1_BUFFER_OVERFLOW_CLR_STATUS_POS                                (18UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX0_BUFFER_OVERFLOW_CLR_STATUS_MASK                               (0x00020000UL) /**< -x - Clear max buffer size interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_CLR_STATUS_REG_RX0_BUFFER_OVERFLOW_CLR_STATUS_POS                                (17UL)
#define HOSTIF_INT_CLR_STATUS_REG_CRC_NOK_CLR_STATUS_MASK                                           (0x00010000UL) /**< -x - Clear data-link Layer CRC error interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_CRC_NOK_CLR_STATUS_POS                                            (16UL)
#define HOSTIF_INT_CLR_STATUS_REG_TX_TIMEOUT_CLR_STATUS_MASK                                        (0x00008000UL) /**< -x - Clear inter-character timeout (TIC) exceeded on transmitted frame interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_TX_TIMEOUT_CLR_STATUS_POS                                         (15UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX3_FRAME_OVERFLOW_CLR_STATUS_MASK                                (0x00004000UL) /**< -x - Clear frame overflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_CLR_STATUS_REG_RX3_FRAME_OVERFLOW_CLR_STATUS_POS                                 (14UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX2_FRAME_OVERFLOW_CLR_STATUS_MASK                                (0x00002000UL) /**< -x - Clear frame overflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_CLR_STATUS_REG_RX2_FRAME_OVERFLOW_CLR_STATUS_POS                                 (13UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX1_FRAME_OVERFLOW_CLR_STATUS_MASK                                (0x00001000UL) /**< -x - Clear frame overflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_CLR_STATUS_REG_RX1_FRAME_OVERFLOW_CLR_STATUS_POS                                 (12UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX0_FRAME_OVERFLOW_CLR_STATUS_MASK                                (0x00000800UL) /**< -x - Clear frame overflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_CLR_STATUS_REG_RX0_FRAME_OVERFLOW_CLR_STATUS_POS                                 (11UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX3_FRAME_UNDERFLOW_CLR_STATUS_MASK                               (0x00000400UL) /**< -x - Clear frame underflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_CLR_STATUS_REG_RX3_FRAME_UNDERFLOW_CLR_STATUS_POS                                (10UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX2_FRAME_UNDERFLOW_CLR_STATUS_MASK                               (0x00000200UL) /**< -x - Clear frame underflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_CLR_STATUS_REG_RX2_FRAME_UNDERFLOW_CLR_STATUS_POS                                (9UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX1_FRAME_UNDERFLOW_CLR_STATUS_MASK                               (0x00000100UL) /**< -x - Clear frame underflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_CLR_STATUS_REG_RX1_FRAME_UNDERFLOW_CLR_STATUS_POS                                (8UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX0_FRAME_UNDERFLOW_CLR_STATUS_MASK                               (0x00000080UL) /**< -x - Clear frame underflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_CLR_STATUS_REG_RX0_FRAME_UNDERFLOW_CLR_STATUS_POS                                (7UL)
#define HOSTIF_INT_CLR_STATUS_REG_TX_FRAME_NOT_AVAILABLE_CLR_STATUS_MASK                            (0x00000040UL) /**< -x - Clear TX frame not available interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_TX_FRAME_NOT_AVAILABLE_CLR_STATUS_POS                             (6UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX_BUFFER_NOT_AVAILABLE_CLR_STATUS_MASK                           (0x00000020UL) /**< -x - Clear no receive buffers available interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_RX_BUFFER_NOT_AVAILABLE_CLR_STATUS_POS                            (5UL)
#define HOSTIF_INT_CLR_STATUS_REG_EOT_CLR_STATUS_MASK                                               (0x00000010UL) /**< -x - Clear EOT interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_EOT_CLR_STATUS_POS                                                (4UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX3_EOR_CLR_STATUS_MASK                                           (0x00000008UL) /**< -x - Clear End of Reception in buffer 3 interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_RX3_EOR_CLR_STATUS_POS                                            (3UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX2_EOR_CLR_STATUS_MASK                                           (0x00000004UL) /**< -x - Clear End of Reception in buffer 2 interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_RX2_EOR_CLR_STATUS_POS                                            (2UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX1_EOR_CLR_STATUS_MASK                                           (0x00000002UL) /**< -x - Clear End of Reception in buffer 1 interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_RX1_EOR_CLR_STATUS_POS                                            (1UL)
#define HOSTIF_INT_CLR_STATUS_REG_RX0_EOR_CLR_STATUS_MASK                                           (0x00000001UL) /**< -x - Clear End of Reception in buffer 0 interrupt. */
#define HOSTIF_INT_CLR_STATUS_REG_RX0_EOR_CLR_STATUS_POS                                            (0UL)
#define HOSTIF_INT_CLR_STATUS_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt set status register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define HOSTIF_INT_SET_STATUS_REG                                                                   (0x40023FECUL) /**< -w-*/
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_SMBALERT_DONE_SET_STATUS_MASK                               (0x40000000UL) /**< -x - Set Clear Smbus SMBALERT finished interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_SMBALERT_DONE_SET_STATUS_POS                                (30UL)
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_ARP_ASSIGN_SET_STATUS_MASK                                  (0x20000000UL) /**< -x - Set Smbus ARP Assign address command received interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_ARP_ASSIGN_SET_STATUS_POS                                   (29UL)
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_ARP_RESET_SET_STATUS_MASK                                   (0x10000000UL) /**< -x - Set Smbus ARP Reset command received interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_ARP_RESET_SET_STATUS_POS                                    (28UL)
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_ARP_PREPARE_SET_STATUS_MASK                                 (0x08000000UL) /**< -x - Set Smbus ARP Prepare command received interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_SMBUS_ARP_PREPARE_SET_STATUS_POS                                  (27UL)
#define HOSTIF_INT_SET_STATUS_REG_HSU_RX_FER_SET_STATUS_MASK                                        (0x04000000UL) /**< -x - Set Rx framing error interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_HSU_RX_FER_SET_STATUS_POS                                         (26UL)
#define HOSTIF_INT_SET_STATUS_REG_BUFFER_CFG_CHANGED_ERROR_SET_STATUS_MASK                          (0x02000000UL) /**< -x - Set Buffer_cfg_changed interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_BUFFER_CFG_CHANGED_ERROR_SET_STATUS_POS                           (25UL)
#define HOSTIF_INT_SET_STATUS_REG_AHB_WR_SLOW_SET_STATUS_MASK                                       (0x01000000UL) /**< -x - Set AHB wr slow interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_AHB_WR_SLOW_SET_STATUS_POS                                        (24UL)
#define HOSTIF_INT_SET_STATUS_REG_AHB_RD_SLOW_SET_STATUS_MASK                                       (0x00800000UL) /**< -x - Set AHB rd slow interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_AHB_RD_SLOW_SET_STATUS_POS                                        (23UL)
#define HOSTIF_INT_SET_STATUS_REG_AHB_ERROR_SET_STATUS_MASK                                         (0x00400000UL) /**< -x - Set AHB error interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_AHB_ERROR_SET_STATUS_POS                                          (22UL)
#define HOSTIF_INT_SET_STATUS_REG_WATERLEVEL_REACHED_SET_STATUS_MASK                                (0x00200000UL) /**< -x - Set waterlevel reached interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_WATERLEVEL_REACHED_SET_STATUS_POS                                 (21UL)
#define HOSTIF_INT_SET_STATUS_REG_RX3_BUFFER_OVERFLOW_SET_STATUS_MASK                               (0x00100000UL) /**< -x - Set max buffer size interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_SET_STATUS_REG_RX3_BUFFER_OVERFLOW_SET_STATUS_POS                                (20UL)
#define HOSTIF_INT_SET_STATUS_REG_RX2_BUFFER_OVERFLOW_SET_STATUS_MASK                               (0x00080000UL) /**< -x - Set max buffer size interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_SET_STATUS_REG_RX2_BUFFER_OVERFLOW_SET_STATUS_POS                                (19UL)
#define HOSTIF_INT_SET_STATUS_REG_RX1_BUFFER_OVERFLOW_SET_STATUS_MASK                               (0x00040000UL) /**< -x - Set max buffer size interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_SET_STATUS_REG_RX1_BUFFER_OVERFLOW_SET_STATUS_POS                                (18UL)
#define HOSTIF_INT_SET_STATUS_REG_RX0_BUFFER_OVERFLOW_SET_STATUS_MASK                               (0x00020000UL) /**< -x - Set max buffer size interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_SET_STATUS_REG_RX0_BUFFER_OVERFLOW_SET_STATUS_POS                                (17UL)
#define HOSTIF_INT_SET_STATUS_REG_CRC_NOK_SET_STATUS_MASK                                           (0x00010000UL) /**< -x - Set data-link Layer CRC error interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_CRC_NOK_SET_STATUS_POS                                            (16UL)
#define HOSTIF_INT_SET_STATUS_REG_TX_TIMEOUT_SET_STATUS_MASK                                        (0x00008000UL) /**< -x - Set inter-character timeout (TIC) exceeded on transmitted frame interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_TX_TIMEOUT_SET_STATUS_POS                                         (15UL)
#define HOSTIF_INT_SET_STATUS_REG_RX3_FRAME_OVERFLOW_SET_STATUS_MASK                                (0x00004000UL) /**< -x - Set frame overflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_SET_STATUS_REG_RX3_FRAME_OVERFLOW_SET_STATUS_POS                                 (14UL)
#define HOSTIF_INT_SET_STATUS_REG_RX2_FRAME_OVERFLOW_SET_STATUS_MASK                                (0x00002000UL) /**< -x - Set frame overflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_SET_STATUS_REG_RX2_FRAME_OVERFLOW_SET_STATUS_POS                                 (13UL)
#define HOSTIF_INT_SET_STATUS_REG_RX1_FRAME_OVERFLOW_SET_STATUS_MASK                                (0x00001000UL) /**< -x - Set frame overflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_SET_STATUS_REG_RX1_FRAME_OVERFLOW_SET_STATUS_POS                                 (12UL)
#define HOSTIF_INT_SET_STATUS_REG_RX0_FRAME_OVERFLOW_SET_STATUS_MASK                                (0x00000800UL) /**< -x - Set frame overflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_SET_STATUS_REG_RX0_FRAME_OVERFLOW_SET_STATUS_POS                                 (11UL)
#define HOSTIF_INT_SET_STATUS_REG_RX3_FRAME_UNDERFLOW_SET_STATUS_MASK                               (0x00000400UL) /**< -x - Set frame underflow interrupt for RX buffer in buffer 3. */
#define HOSTIF_INT_SET_STATUS_REG_RX3_FRAME_UNDERFLOW_SET_STATUS_POS                                (10UL)
#define HOSTIF_INT_SET_STATUS_REG_RX2_FRAME_UNDERFLOW_SET_STATUS_MASK                               (0x00000200UL) /**< -x - Set frame underflow interrupt for RX buffer in buffer 2. */
#define HOSTIF_INT_SET_STATUS_REG_RX2_FRAME_UNDERFLOW_SET_STATUS_POS                                (9UL)
#define HOSTIF_INT_SET_STATUS_REG_RX1_FRAME_UNDERFLOW_SET_STATUS_MASK                               (0x00000100UL) /**< -x - Set frame underflow interrupt for RX buffer in buffer 1. */
#define HOSTIF_INT_SET_STATUS_REG_RX1_FRAME_UNDERFLOW_SET_STATUS_POS                                (8UL)
#define HOSTIF_INT_SET_STATUS_REG_RX0_FRAME_UNDERFLOW_SET_STATUS_MASK                               (0x00000080UL) /**< -x - Set frame underflow interrupt for RX buffer in buffer 0. */
#define HOSTIF_INT_SET_STATUS_REG_RX0_FRAME_UNDERFLOW_SET_STATUS_POS                                (7UL)
#define HOSTIF_INT_SET_STATUS_REG_TX_FRAME_NOT_AVAILABLE_SET_STATUS_MASK                            (0x00000040UL) /**< -x - Set TX frame not available interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_TX_FRAME_NOT_AVAILABLE_SET_STATUS_POS                             (6UL)
#define HOSTIF_INT_SET_STATUS_REG_RX_BUFFER_NOT_AVAILABLE_SET_STATUS_MASK                           (0x00000020UL) /**< -x - Set no receive buffers available interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_RX_BUFFER_NOT_AVAILABLE_SET_STATUS_POS                            (5UL)
#define HOSTIF_INT_SET_STATUS_REG_EOT_SET_STATUS_MASK                                               (0x00000010UL) /**< -x - Set EOT interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_EOT_SET_STATUS_POS                                                (4UL)
#define HOSTIF_INT_SET_STATUS_REG_RX3_EOR_SET_STATUS_MASK                                           (0x00000008UL) /**< -x - Set End of Reception in buffer 3 interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_RX3_EOR_SET_STATUS_POS                                            (3UL)
#define HOSTIF_INT_SET_STATUS_REG_RX2_EOR_SET_STATUS_MASK                                           (0x00000004UL) /**< -x - Set End of Reception in buffer 2 interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_RX2_EOR_SET_STATUS_POS                                            (2UL)
#define HOSTIF_INT_SET_STATUS_REG_RX1_EOR_SET_STATUS_MASK                                           (0x00000002UL) /**< -x - Set End of Reception in buffer 1 interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_RX1_EOR_SET_STATUS_POS                                            (1UL)
#define HOSTIF_INT_SET_STATUS_REG_RX0_EOR_SET_STATUS_MASK                                           (0x00000001UL) /**< -x - Set End of Reception in buffer 0 interrupt. */
#define HOSTIF_INT_SET_STATUS_REG_RX0_EOR_SET_STATUS_POS                                            (0UL)
#define HOSTIF_INT_SET_STATUS_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_HIF_H_INCLUDED */
