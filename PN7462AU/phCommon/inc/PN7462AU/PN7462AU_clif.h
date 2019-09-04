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
#ifndef PN7462AU_IP_CLIF_H_INCLUDED
#define PN7462AU_IP_CLIF_H_INCLUDED

#include "PN7462AU/PN7462AU_clif_rorw.h"



/**
* \name Main control register for CLIF digital
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_CONTROL_REG                                                                            (0x40004000UL) /**< rw*/
#define CLIF_CONTROL_REG_START_SEND_MASK                                                            (0x00000008UL) /**< rw - Set to logic 1. the data transmission is started. Note: This bit is only valid in combination with the Transceive command Note: If TxWait is set to a value other than zero the TxWait period configured must be expired as well that the transmission starts Note: As soon as the transmission started this bit is cleared by HW. . */
#define CLIF_CONTROL_REG_START_SEND_POS                                                             (3UL)
#define CLIF_CONTROL_REG_COMMAND_MASK                                                               (0x00000007UL) /**< rw - This registers hold the command bits 0* IDLE/StopCom Command. stops all ongoing communication and set the CLIF to IDLE mode. reset value 1 Transmit command. starts a transmission immediately 2 Receive command. enables the receiver. After end of reception the bits are clear and IDLE 3 Transceive command. initiates a transceive cycle. Note: Depending on the value of the Initiator bit a transmission is started or the receiver is enabled Note: The transceive command does not finish automatically. It stays in the transceive cycle until stopped via the IDLE/StopCom command 4 KeepCommand command. This command does not change the content of the command register and might be used in case other bits in the register are to be changed 5 LoopBack command. This command is for test purposes only. It starts a transmission and at the same enables the receiver. 6 - 7 Reserved: Do not use this settings  1. */
#define CLIF_CONTROL_REG_COMMAND_POS                                                                (0UL)
#define CLIF_CONTROL_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLIF digital clock control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_CLOCK_CONTROL_REG                                                                      (0x40004004UL) /**< rw*/
#define CLIF_CLOCK_CONTROL_REG_FORCE_TEMP_CLK_ON_RFOFF_MASK                                         (0x00000020UL) /**< rw - If set to 1 upon RFOFF event the clock is always switched to temporary-clock no matter if pll-clock is available.. */
#define CLIF_CLOCK_CONTROL_REG_FORCE_TEMP_CLK_ON_RFOFF_POS                                          (5UL)
#define CLIF_CLOCK_CONTROL_REG_FORCE_PLL_CLOCK_ON_TXACTIVE_MASK                                     (0x00000010UL) /**< rw - If set to 1 clock is automatically switched to DPLL clock for transmission. */
#define CLIF_CLOCK_CONTROL_REG_FORCE_PLL_CLOCK_ON_TXACTIVE_POS                                      (4UL)
#define CLIF_CLOCK_CONTROL_REG_RELEASE_TEMP_CLOCK_MASK                                              (0x00000004UL) /**< -x - Setting this register bit to 1 will release the temporary clock and switch back to functional mode. Afterwards it is cleared by HW. Note: In normal operation this bit must not be set.. */
#define CLIF_CLOCK_CONTROL_REG_RELEASE_TEMP_CLOCK_POS                                               (2UL)
#define CLIF_CLOCK_CONTROL_REG_CLIF_CLOCK_SELECT_MASK                                               (0x00000003UL) /**< rw - This register controlls the source of the CLIF clock (13.56MHz contactless frequency clock) 0* Automatic clock selection. The source of the clock is automatically chosen depending on the RF-Field status. If an external RF-Field is detected the RF-clock is chosen otherwise the PLL-clock (indipendent if it is available) 1 Force RF-clock. RF-clock is chosen as source. not automatic switching is performed. 2 Force PLL-clock. PLL-clock is chosen as source. not automatic switching is performed. 3 Reserved. Do not use this setting. */
#define CLIF_CLOCK_CONTROL_REG_CLIF_CLOCK_SELECT_POS                                                (0UL)
#define CLIF_CLOCK_CONTROL_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLIF digital status register
* <b>Reset value:</b> 0x10000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_STATUS_REG                                                                             (0x40004008UL) /**< r-*/
#define CLIF_STATUS_REG_DPLL_ENABLE_MASK                                                            (0x40000000UL) /**< r- - This bit indicates that the DPLL Controler has enabled the DPLL (RF on RF frequency ok PLL locked). */
#define CLIF_STATUS_REG_DPLL_ENABLE_POS                                                             (30UL)
#define CLIF_STATUS_REG_AGC_RFOFF_DET_MASK                                                          (0x20000000UL) /**< r- - This bit indicates than the AGC has detected the external RF-Field was switched off while transmitting in SL-ALM mode. Note: Only valid if the detection mode is enabled with the register bit-field AGC_RF_DETECT_SEL. */
#define CLIF_STATUS_REG_AGC_RFOFF_DET_POS                                                           (29UL)
#define CLIF_STATUS_REG_CRC_OK_MASK                                                                 (0x10000000UL) /**< r- - This bit indicates the status of the actual CRC calculation. If 1 the CRC is correct. meaning the CRC register has the value 0 or the residue value if inverted CRC is used. Note: This flag should only by evaluated at the end of a communication . */
#define CLIF_STATUS_REG_CRC_OK_POS                                                                  (28UL)
#define CLIF_STATUS_REG_SC_DETECTED_MASK                                                            (0x08000000UL) /**< r- - Status signal indicating that a subcarrier is detected . */
#define CLIF_STATUS_REG_SC_DETECTED_POS                                                             (27UL)
#define CLIF_STATUS_REG_SOF_DETECTED_MASK                                                           (0x04000000UL) /**< r- - Status signal indicating that a SOF has been detected . */
#define CLIF_STATUS_REG_SOF_DETECTED_POS                                                            (26UL)
#define CLIF_STATUS_REG_TX_RF_STATUS_MASK                                                           (0x02000000UL) /**< r- - If set to 1 this bit indicates that the drivers are turned on. meaning an RF-Field is created by the device itself. . */
#define CLIF_STATUS_REG_TX_RF_STATUS_POS                                                            (25UL)
#define CLIF_STATUS_REG_RF_DET_STATUS_MASK                                                          (0x01000000UL) /**< r- - If set to 1 this bit indicates that an external RF-Field is detected by the rf level detectors (after digital filtering) . */
#define CLIF_STATUS_REG_RF_DET_STATUS_POS                                                           (24UL)
#define CLIF_STATUS_REG_CLOCK_ERROR_MASK                                                            (0x00008000UL) /**< r- - If set to 1 CLIF is operating on the temporary clock.. */
#define CLIF_STATUS_REG_CLOCK_ERROR_POS                                                             (15UL)
#define CLIF_STATUS_REG_BMA_TRANSFER_ONGOING_MASK                                                   (0x00004000UL) /**< r- - Status signal from BufferManager to indicate that a transfer is actually ongoing.. */
#define CLIF_STATUS_REG_BMA_TRANSFER_ONGOING_POS                                                    (14UL)
#define CLIF_STATUS_REG_TX_READ_ERROR_MASK                                                          (0x00002000UL) /**< r- - This error flag is set to 1 if for an ongoing transmission data is not copied from RAM in time (BMA encountered read error) and therefor the transmission is aborted            Note: This case should not happen in normal operation. */
#define CLIF_STATUS_REG_TX_READ_ERROR_POS                                                           (13UL)
#define CLIF_STATUS_REG_TX_LATENCY_ERROR_MASK                                                       (0x00001000UL) /**< r- - This error flag is set to 1. if for an ongoing transmission data is not availabe in time (BMA latency to big) and therefor the transmission is aborted. Note: This case should not happen in normal operation . */
#define CLIF_STATUS_REG_TX_LATENCY_ERROR_POS                                                        (12UL)
#define CLIF_STATUS_REG_TX_NO_DATA_ERROR_MASK                                                       (0x00000800UL) /**< r- - This error flag is set to 1. in case a transmission is started but no data is available (register NumBytesToSend == 0). . */
#define CLIF_STATUS_REG_TX_NO_DATA_ERROR_POS                                                        (11UL)
#define CLIF_STATUS_REG_RF_ACTIVE_ERROR_CAUSE_MASK                                                  (0x00000700UL) /**< r- - This status flag indicates the cause of an NFC-Active error. Note: This bits are only valid when the RF_ACTIVE_ERROR_IRQ is raised and will be cleared as soon as the bit TX_RF_ENABLE is set to 1. 0* No Error. reset value 1 External field was detected on within TIDT timing 2 External field was detected on within TADT timing 3 No external field was detected within TADT timings 4 Peer did switch off RFField without but no Rx event was raised (no data received) 5 - 7 Reserved . */
#define CLIF_STATUS_REG_RF_ACTIVE_ERROR_CAUSE_POS                                                   (8UL)
#define CLIF_STATUS_REG_RX_ENABLE_MASK                                                              (0x00000020UL) /**< r- - This bit indicates if the RxDecoder is enalbed. If 1 the RxDecoder was enabled by the Transceive Unit and is now ready for data reception . */
#define CLIF_STATUS_REG_RX_ENABLE_POS                                                               (5UL)
#define CLIF_STATUS_REG_TX_ACTIVE_MASK                                                              (0x00000010UL) /**< r- - This bit indicates activity of the TxEncoder. If 1 a transmission is ongoing otherwise the TxEncoder is in idle state.. */
#define CLIF_STATUS_REG_TX_ACTIVE_POS                                                               (4UL)
#define CLIF_STATUS_REG_RX_ACTIVE_MASK                                                              (0x00000008UL) /**< r- - This bit indicates activity of the RxDecoder. If 1 a data reception is ongoing. otherwise the RxDecoder is in idle state. . */
#define CLIF_STATUS_REG_RX_ACTIVE_POS                                                               (3UL)
#define CLIF_STATUS_REG_TRANSCEIVE_STATE_MASK                                                       (0x00000007UL) /**< r- - This registers hold the command bits 0* IDLE state 1 WaitTransmit state 2 Transmitting state 3 WaitReceive state 4 WaitForData state 5 Receiving state 6 LoopBack state 7 reserved. */
#define CLIF_STATUS_REG_TRANSCEIVE_STATE_POS                                                        (0UL)
#define CLIF_STATUS_REG__RESET_VALUE                                                                (0x10000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Transceive control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TRANSCEIVE_CONTROL_REG                                                                 (0x4000400CUL) /**< rw*/
#define CLIF_TRANSCEIVE_CONTROL_REG_STATE_TRIGGER_SELECT_MASK                                       (0x3F000000UL) /**< rw - Register to select the state to trigger the STATE_CHANGE_IRQ flag. Each bit of the bit field enables one state - several states are possible. Note: If all bits are 0 no IRQ is triggered. xxxxx1 IDLE state enabled to trigger IRQ xxxx1x WaitTransmit state enabled to trigger IRQ  xxx1xx Transmitting state enabled to trigger IRQ xx1xxx WaitReceive state enabled to trigger IRQ x1xxxx WaitForData state enabled to trigger IRQ 1xxxxx Receiving state enabled to trigger IRQ . */
#define CLIF_TRANSCEIVE_CONTROL_REG_STATE_TRIGGER_SELECT_POS                                        (24UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_WAIT_RFON_ENABLE_MASK                                        (0x00020000UL) /**< rw - If set to 1 the TxWait guard time will be started when the own RF-Field is switched on. Note: In default configuration the TxWait guard time is started at the end of reception. This feature is intended to be used for NFC-Active communication . */
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_WAIT_RFON_ENABLE_POS                                         (17UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_RX_WAIT_RFON_ENABLE_MASK                                        (0x00010000UL) /**< rw - If set to 1 the RxWait guard time will be started when the own RF-Field is switched off and an external RF-Field was detected. Note: In default configuration the RxWait guard time is started at the end of transmission. This feature is intended to be used for NFC-Active communication . */
#define CLIF_TRANSCEIVE_CONTROL_REG_RX_WAIT_RFON_ENABLE_POS                                         (16UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_BITPHASE_MASK                                                (0x0000FF00UL) /**< rw - Defines the number of 13.56MHz cycles used for adjustment of tx_wait to meet the FDT. . */
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_BITPHASE_POS                                                 (8UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_MILLER_SYNC_ENABLE_MASK                                      (0x00000040UL) /**< rw - If set to 1 guard time is synchronized to miller envelope for transmission. Used for AGC.. */
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_MILLER_SYNC_ENABLE_POS                                       (6UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_SKIP_SEND_ENABLE_MASK                                        (0x00000020UL) /**< rw - If set to 1 not transmission is started after tx_wait is expired and START_SEND was set Note: The bit is cleared by HW when the WaitReceive state is entered.. */
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_SKIP_SEND_ENABLE_POS                                         (5UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_PICC2_TIMESLOT_ENABLE_MASK                                   (0x00000010UL) /**< rw - If set to 1 two PICC responses are transmitted in different timeslots. Note: Only valid when command Transceive is active. . */
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_PICC2_TIMESLOT_ENABLE_POS                                    (4UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_FRAMESTEP_ENABLE_MASK                                        (0x00000008UL) /**< rw - If set to 1. at every start of transmission. each byte of data is sent in a seperate frame. SOF and EOF is appended to the data byte according to the framing settings. After one byte is transmitted. the TxEncoder waits for a new start trigger to continue with the next byte. . */
#define CLIF_TRANSCEIVE_CONTROL_REG_TX_FRAMESTEP_ENABLE_POS                                         (3UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_RX_MULTIPLE_ENABLE_MASK                                         (0x00000004UL) /**< rw - If this bit is set to 1. the receiver is re-activated after the end of a reception. A statusbyte is written to the RAM containing all relevant status information of the frame. Note: Data in RAM is word aligned therefore empty bytes of a data Word in RAM are padded with 0x00 bytes. SW has to calculate the correct address for the following frame. . */
#define CLIF_TRANSCEIVE_CONTROL_REG_RX_MULTIPLE_ENABLE_POS                                          (2UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_MILLER_SYNC_ENABLE_MASK                                         (0x00000002UL) /**< rw - If set to one miller pulse synchronisation is enabled. Note: TX_BITPHASE is used to adjust the prescaler of the guard timer. Note: This bit is set by HW when TypeA protocol is detected in automatic mode detection. . */
#define CLIF_TRANSCEIVE_CONTROL_REG_MILLER_SYNC_ENABLE_POS                                          (1UL)
#define CLIF_TRANSCEIVE_CONTROL_REG_INITIATOR_MASK                                                  (0x00000001UL) /**< rw - Set to 1. the CLIF is configured for initiator mode. Depending on this setting the behavior of the transceive command is different. */
#define CLIF_TRANSCEIVE_CONTROL_REG_INITIATOR_POS                                                   (0UL)
#define CLIF_TRANSCEIVE_CONTROL_REG__RESET_VALUE                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Transceive TxWait register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_WAIT_REG                                                                            (0x40004010UL) /**< rw*/
#define CLIF_TX_WAIT_REG_TX_WAIT_VALUE_MASK                                                         (0x0FFFFF00UL) /**< rw - Defines the tx_wait timer reload value. Note: If set to 00000h the tx_wait guard time is disabled Note: This bit is set by HW a protocol is detected in automatic mode detection. */
#define CLIF_TX_WAIT_REG_TX_WAIT_VALUE_POS                                                          (8UL)
#define CLIF_TX_WAIT_REG_TX_WAIT_PRESCALER_MASK                                                     (0x000000FFUL) /**< rw - Defines the prescaler reload value for the tx_wait timer. Note: This bit is set by HW a protocol is  detected in automatic mode detection. */
#define CLIF_TX_WAIT_REG_TX_WAIT_PRESCALER_POS                                                      (0UL)
#define CLIF_TX_WAIT_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Transceive TimeSlot register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_TIMESLOT_SEL_REG                                                                    (0x40004014UL) /**< rw*/
#define CLIF_TX_TIMESLOT_SEL_REG_TIMESLOT_SEL1_MASK                                                 (0x000000F0UL) /**< rw - Defines the number of timeslots (tx_wait / 2) to extend the tx_wait period after for transmission of the first frame selected by TIMESLOT_SEL0. See Transceive Spec for details. Note: SW must take care to stay within the maximum allowed number of timeslots. . */
#define CLIF_TX_TIMESLOT_SEL_REG_TIMESLOT_SEL1_POS                                                  (4UL)
#define CLIF_TX_TIMESLOT_SEL_REG_TIMESLOT_SEL0_MASK                                                 (0x0000000FUL) /**< rw - Defines the number of timeslots (tx_wait / 2) to extend the tx_wait period.. */
#define CLIF_TX_TIMESLOT_SEL_REG_TIMESLOT_SEL0_POS                                                  (0UL)
#define CLIF_TX_TIMESLOT_SEL_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Transceive RxWait register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_RX_WAIT_REG                                                                            (0x40004018UL) /**< rw*/
#define CLIF_RX_WAIT_REG_RX_WAIT_VALUE_MASK                                                         (0x0FFFFF00UL) /**< rw - Defines the rx_wait timer reload value. Note: If set to 00000h the rx_wait guard time is disabled . */
#define CLIF_RX_WAIT_REG_RX_WAIT_VALUE_POS                                                          (8UL)
#define CLIF_RX_WAIT_REG_RX_WAIT_PRESCALER_MASK                                                     (0x000000FFUL) /**< rw - Defines the prescaler reload value for the rx_wait timer.. */
#define CLIF_RX_WAIT_REG_RX_WAIT_PRESCALER_POS                                                      (0UL)
#define CLIF_RX_WAIT_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name BufferManager TxBuffer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_BUFFER_REG                                                                          (0x4000401CUL) /**< rw*/
#define CLIF_TX_BUFFER_REG_TX_HEADER_BYTE_OFFSET_MASK                                               (0x00030000UL) /**< rw - Defines the number of bytes to be skipped of the 1st data word transmitted . */
#define CLIF_TX_BUFFER_REG_TX_HEADER_BYTE_OFFSET_POS                                                (16UL)
#define CLIF_TX_BUFFER_REG_TX_BUFFER_START_ADDRESS_MASK                                             (0x000007FFUL) /**< rw - Defines the relative buffer (word) address of the Tx buffer. Note: Changing the buffer address while the transmitter is active will trigger a BMA error.. */
#define CLIF_TX_BUFFER_REG_TX_BUFFER_START_ADDRESS_POS                                              (0UL)
#define CLIF_TX_BUFFER_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name BufferManager RxBuffer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_RX_BUFFER_REG                                                                          (0x40004020UL) /**< rw*/
#define CLIF_RX_BUFFER_REG_RX_BUFFER_LENGTH_MASK                                                    (0x007F0000UL) /**< rw - Defines the length of the Rx buffer (in data words). */
#define CLIF_RX_BUFFER_REG_RX_BUFFER_LENGTH_POS                                                     (16UL)
#define CLIF_RX_BUFFER_REG_RX_BUFFER_START_ADDRESS_MASK                                             (0x000007FFUL) /**< rw - Defines the relative buffer (word) address of the Rx buffer. Note: Changing the buffer address while the receiver is enabled will trigger a BMA error.. */
#define CLIF_RX_BUFFER_REG_RX_BUFFER_START_ADDRESS_POS                                              (0UL)
#define CLIF_RX_BUFFER_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name BufferManager TxWaterlevel register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_WATERLEVEL_REG                                                                      (0x40004024UL) /**< rw*/
#define CLIF_TX_WATERLEVEL_REG_TX_WATERLEVEL_MASK                                                   (0x000000FFUL) /**< rw - Defines a warning level to indicate that TX_WATERLEVEL number of words were already transmitted in the actual frame. When this level is reached the corresponding IRQ is set. Note: 0 disables the waterlevel Note: In case a header byte offset is set the waterlevel refers to word fetched from RAM. */
#define CLIF_TX_WATERLEVEL_REG_TX_WATERLEVEL_POS                                                    (0UL)
#define CLIF_TX_WATERLEVEL_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name BufferManager TxWaterlevel register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_RX_WATERLEVEL_REG                                                                      (0x40004028UL) /**< rw*/
#define CLIF_RX_WATERLEVEL_REG_RX_WATERLEVEL_MASK                                                   (0x000000FFUL) /**< rw - Defines a warning level to indicate that RX_WATERLEVEL number of words were already received in the actual frame. When this level is reached the corresponding IRQ is set. Note: 0 disables the waterlevel. */
#define CLIF_RX_WATERLEVEL_REG_RX_WATERLEVEL_POS                                                    (0UL)
#define CLIF_RX_WATERLEVEL_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RFControl control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_RF_CONTROL_REG                                                                         (0x4000402CUL) /**< rw*/
#define CLIF_RF_CONTROL_REG_TX_RF_ENABLE_MASK                                                       (0x00000010UL) /**< rw - Set to 1. turning on the driver in reader or active mode is requested. Note: According to the setting of InitialRFOn. AutoRFOn and CAOn the driver is turned after a defined time and depended on the presence of an external RF field. Note: In case of an RFActiveError this bit is cleared by HW . */
#define CLIF_RF_CONTROL_REG_TX_RF_ENABLE_POS                                                        (4UL)
#define CLIF_RF_CONTROL_REG_TX_COLL_AV_ENABLE_MASK                                                  (0x00000008UL) /**< rw - Set to 1.enables automatic collision avoidance. See ISO18092 for more details. . */
#define CLIF_RF_CONTROL_REG_TX_COLL_AV_ENABLE_POS                                                   (3UL)
#define CLIF_RF_CONTROL_REG_TX_INITIAL_RFON_MASK                                                    (0x00000004UL) /**< rw - Set to 1. the drivers are automatically turned on when no external field is present. In the case an external field is detected. the turning on the drivers is delay until the external field vanishes. Note: The driver on procedure must be triggered by setting TX_RF_ENABLE. Note: This bit is reset to 0 as soon as the drivers turned on. . */
#define CLIF_RF_CONTROL_REG_TX_INITIAL_RFON_POS                                                     (2UL)
#define CLIF_RF_CONTROL_REG_TX_AUTO_RFON_MASK                                                       (0x00000002UL) /**< rw - Set to 1. the drivers are automatically turned on after a before present external field vanished. Note: The driver on procedure must be triggered by setting TX_RF_ENABLE. . */
#define CLIF_RF_CONTROL_REG_TX_AUTO_RFON_POS                                                        (1UL)
#define CLIF_RF_CONTROL_REG_TX_AUTO_RFOFF_MASK                                                      (0x00000001UL) /**< rw - Set to 1. the drivers are automatically turned off after data is transmitted.. */
#define CLIF_RF_CONTROL_REG_TX_AUTO_RFOFF_POS                                                       (0UL)
#define CLIF_RF_CONTROL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_DATA_CONFIG_REG                                                                     (0x40004030UL) /**< rw*/
#define CLIF_TX_DATA_CONFIG_REG_TX_PICC2_NUM_BYTES_2_SEND_MASK                                      (0x01FF0000UL) /**< rw - Defines the number of bytes to be transmit in case a second PICC response is enabled. The maximum number of bytes is 260 (0x104). If the Transmission Of Data Is Enabled (TX_DATA_ENABLE) and TX_PICC2_NUM_BYTES_2_SEND is zero then a NO_DATA_ERROR occurs.. */
#define CLIF_TX_DATA_CONFIG_REG_TX_PICC2_NUM_BYTES_2_SEND_POS                                       (16UL)
#define CLIF_TX_DATA_CONFIG_REG_TX_LAST_BITS_MASK                                                   (0x00007000UL) /**< rw - Defines how many bits of the last data byte to be sent. If set to 000b all bits of the last data byte are sent. Note: Bits are skipped at the end of the byte . */
#define CLIF_TX_DATA_CONFIG_REG_TX_LAST_BITS_POS                                                    (12UL)
#define CLIF_TX_DATA_CONFIG_REG_TX_FIRST_BITS_MASK                                                  (0x00000E00UL) /**< rw - Defines how many bits of the first data byte to be sent. If set to 000b all bits of the last data byte are sent. Note: Bits are skipped at the beginning of the byte . */
#define CLIF_TX_DATA_CONFIG_REG_TX_FIRST_BITS_POS                                                   (9UL)
#define CLIF_TX_DATA_CONFIG_REG_TX_NUM_BYTES_2_SEND_MASK                                            (0x000001FFUL) /**< rw - Defines the number of bytes to be transmit. The maximum number of bytes is 260 (0x104). If the Transmission Of Data Is Enabled (TX_DATA_ENABLE) and TX_NUM_BYTES_2_SEND is zero. then a NO_DATA_ERROR occurs.. */
#define CLIF_TX_DATA_CONFIG_REG_TX_NUM_BYTES_2_SEND_POS                                             (0UL)
#define CLIF_TX_DATA_CONFIG_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_DATA_MOD_REG                                                                        (0x40004034UL) /**< rw*/
#define CLIF_TX_DATA_MOD_REG_TX_DATA_MOD_WIDTH_MASK                                                 (0x00FF0000UL) /**< rw - Specifies the length of a pulse for sending data with miller pulse modulation enabled. The length is given by the number of carrier clocks + 1.. */
#define CLIF_TX_DATA_MOD_REG_TX_DATA_MOD_WIDTH_POS                                                  (16UL)
#define CLIF_TX_DATA_MOD_REG_TX_DATA_MILLER_ENABLE_MASK                                             (0x00000100UL) /**< rw - If set to 1 pulse modulation is applied according to modified miller coding. */
#define CLIF_TX_DATA_MOD_REG_TX_DATA_MILLER_ENABLE_POS                                              (8UL)
#define CLIF_TX_DATA_MOD_REG_TX_DATA_INV_ENV_MASK                                                   (0x00000080UL) /**< rw - If set to 1 the output envelope is inverted. */
#define CLIF_TX_DATA_MOD_REG_TX_DATA_INV_ENV_POS                                                    (7UL)
#define CLIF_TX_DATA_MOD_REG_TX_DATA_ENV_TYPE_MASK                                                  (0x00000070UL) /**< rw - Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple coding 111b RFU. */
#define CLIF_TX_DATA_MOD_REG_TX_DATA_ENV_TYPE_POS                                                   (4UL)
#define CLIF_TX_DATA_MOD_REG_TX_DATA_SC_FREQ_MASK                                                   (0x00000008UL) /**< rw - Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz. */
#define CLIF_TX_DATA_MOD_REG_TX_DATA_SC_FREQ_POS                                                    (3UL)
#define CLIF_TX_DATA_MOD_REG_TX_DATA_BIT_FREQ_MASK                                                  (0x00000007UL) /**< rw - Specifies the frequency of the bit-stream.  000b -> 1.695 MHz. 001b -> Reserved. 010b -> 26 kHz. 011b -> 53 kHz. 100b -> 106 kHz. 101b -> 212 kHz. 110b -> 424 kHz. 111b -> 848 kHz.. */
#define CLIF_TX_DATA_MOD_REG_TX_DATA_BIT_FREQ_POS                                                   (0UL)
#define CLIF_TX_DATA_MOD_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000200\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_FRAME_CONFIG_REG                                                                    (0x40004038UL) /**< rw*/
#define CLIF_TX_FRAME_CONFIG_REG_TX_DATA_CODE_TYPE_MASK                                             (0x00070000UL) /**< rw - Specifies the type of encoding of data to be used 000b No special code 001b 1 out of 4 code [I-Code SLI] 010b 1 out of 256 code [I-Code SLI] 011b Pulse interval encoding (PIE) [I-Code EPC-V2] 100b 2bit tuple code (intended only for test purpose) 101-111b Reserved . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_DATA_CODE_TYPE_POS                                              (16UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_STOPBIT_TYPE_MASK                                               (0x0000E000UL) /**< rw - Enables the stop bit (logic "1") and extra guard time (logic "1"). The value 0 disables transmission of stop-bits. 000b no stop-bit. no EGT 001b stop-bit. no EGT 010b stop-bit + 1 EGT 011b stop-bit + 2 EGT 100b stop-bit + 3 EGT 101b stop-bit + 4 EGT 110b stop-bit + 5 EGT 111b stop-bit + 6 EGT . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_STOPBIT_TYPE_POS                                                (13UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_STARTBIT_ENABLE_MASK                                            (0x00001000UL) /**< rw - If set to 1. a start-bit (logic "0") will be send . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_STARTBIT_ENABLE_POS                                             (12UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_MSB_FIRST_MASK                                                  (0x00000800UL) /**< rw - If set to 1. data bytes are interpreted MSB first for data transmission . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_MSB_FIRST_POS                                                   (11UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_LAST_INV_ENABLE_MASK                                     (0x00000400UL) /**< rw - If set to 1. the parity bit of last sent data byte is inverted . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_LAST_INV_ENABLE_POS                                      (10UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_TYPE_MASK                                                (0x00000200UL) /**< rw - Defines the type of the parity bit 0 Even Parity is calculated 1 Odd parity is calculated       . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_TYPE_POS                                                 (9UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_ENABLE_MASK                                              (0x00000100UL) /**< rw - If set to 1. a parity bit is calculated and appended to each byte transmitted. If the Transmission Of Data Is Enabled and TX_NUM_BYTES_2_SEND is zero. then a NO_DATA_ERROR occurs. . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_PARITY_ENABLE_POS                                               (8UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_DATA_ENABLE_MASK                                                (0x00000010UL) /**< rw - If set to 1. transmission of data is enabled otherwise only symbols are tranmitted. . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_DATA_ENABLE_POS                                                 (4UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_STOP_SYMBOL_MASK                                                (0x0000000CUL) /**< rw - Defines which pattern symbol is sent as frame stop-symbol 00b No symbol is sent 01b Symbol1 is sent 10b Symbol2 is sent 11b Symbol3 is sent . */
#define CLIF_TX_FRAME_CONFIG_REG_TX_STOP_SYMBOL_POS                                                 (2UL)
#define CLIF_TX_FRAME_CONFIG_REG_TX_START_SYMBOL_MASK                                               (0x00000003UL) /**< rw - Defines which symbol pattern is sent as frame start-symbol 00b No symbol pattern is sent 01b Symbol0 is sent 10b Symbol1 is sent 11b Symbol2 is sent. */
#define CLIF_TX_FRAME_CONFIG_REG_TX_START_SYMBOL_POS                                                (0UL)
#define CLIF_TX_FRAME_CONFIG_REG__RESET_VALUE                                                       (0x00000200UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_SYMBOL_CONFIG_REG                                                                   (0x4000403CUL) /**< rw*/
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_LEN_MASK                                         (0x78000000UL) /**< rw - Specifies the number of bits issued for symbol 1 burst. The 3 bits encode a range from 8 to 256 bit length: 0000b 8 bit 0001b 12 bit 0010b 16 bit 0011b 24 bit 0100b 32 bit 0101b 40 bit 0110b 48 bit 0111b 64 bit 1000b 80 bit 1001b 96 bit 1010b 112 bit 1011b 128 bit 1100b 160 bit 1101b 192 bit 1110b 224 bit 1111b 256 bit. */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_LEN_POS                                          (27UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_TYPE_MASK                                        (0x04000000UL) /**< rw - Specifies the type of the burst of Symbol1 (logical zero / logical one) . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_TYPE_POS                                         (26UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_ONLY_MASK                                        (0x02000000UL) /**< rw - If set to 1. Symbol1 consists only of a burst and no symbol pattern . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_ONLY_POS                                         (25UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_ENABLE_MASK                                      (0x01000000UL) /**< rw - If set to 1. the burst of Symbol0 of the length defined in bit field SYMBOL1_BURST_LEN is enabled         . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_BURST_ENABLE_POS                                       (24UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_LEN_MASK                                         (0x00780000UL) /**< rw - Specifies the number of bits issued for symbol 0 burst. The 3 bits encode a range from 8 to 256 bit length: 0000b 8 bit 0001b 12 bit 0010b 16 bit 0011b 24 bit 0100b 32 bit 0101b 40 bit 0110b 48 bit 0111b 64 bit 1000b 80 bit 1001b 96 bit 1010b 112 bit 1011b 128 bit 1100b 160 bit 1101b 192 bit 1110b 224 bit 1111b 256 bit. */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_LEN_POS                                          (19UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_TYPE_MASK                                        (0x00040000UL) /**< rw - Specifies the type of the burst of Symbol0 (logical zero / logical one) . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_TYPE_POS                                         (18UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_ONLY_MASK                                        (0x00020000UL) /**< rw - If set to 1. Symbol0 consists only of a burst and no symbol pattern . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_ONLY_POS                                         (17UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_ENABLE_MASK                                      (0x00010000UL) /**< rw - If set to 1. the burst of Symbol0 of the length defined in bit field SYMBOL0_BURST_LEN is enabled . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_BURST_ENABLE_POS                                       (16UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL3_LEN_MASK                                               (0x0000E000UL) /**< rw - Specifies the number of valid bits of the symbol definition of Symbol3. The range is from 1 bit (value 0000) to 8 bit (value 111) . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL3_LEN_POS                                                (13UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL2_LEN_MASK                                               (0x00001C00UL) /**< rw - Specifies the number of valid bits of the symbol definition of Symbol2. The range is from 1 bit (value 0000) to 8bit (value 111) . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL2_LEN_POS                                                (10UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_LEN_MASK                                               (0x000003E0UL) /**< rw - Specifies the number of valid bits of the symbol definition of Symbol1. The range is from 1 bit (value 0000) to 28 bit (value 11011) . */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL1_LEN_POS                                                (5UL)
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_LEN_MASK                                               (0x0000001FUL) /**< rw - Specifies the number of valid bits of the symbol definition of Symbol0. The range is from 1 bit (value 0000) to 28 bit (value 11011). */
#define CLIF_TX_SYMBOL_CONFIG_REG_TX_SYMBOL0_LEN_POS                                                (0UL)
#define CLIF_TX_SYMBOL_CONFIG_REG__RESET_VALUE                                                      (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_SYMBOL0_DEF_REG                                                                     (0x40004040UL) /**< rw*/
#define CLIF_TX_SYMBOL0_DEF_REG_TX_SYMBOL0_DEF_MASK                                                 (0x0FFFFFFFUL) /**< rw - Pattern definition for Symbol0. */
#define CLIF_TX_SYMBOL0_DEF_REG_TX_SYMBOL0_DEF_POS                                                  (0UL)
#define CLIF_TX_SYMBOL0_DEF_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_SYMBOL1_DEF_REG                                                                     (0x40004044UL) /**< rw*/
#define CLIF_TX_SYMBOL1_DEF_REG_TX_SYMBOL1_DEF_MASK                                                 (0x0FFFFFFFUL) /**< rw - Pattern definition for Symbol1 . */
#define CLIF_TX_SYMBOL1_DEF_REG_TX_SYMBOL1_DEF_POS                                                  (0UL)
#define CLIF_TX_SYMBOL1_DEF_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_SYMBOL23_DEF_REG                                                                    (0x40004048UL) /**< rw*/
#define CLIF_TX_SYMBOL23_DEF_REG_TX_SYMBOL3_DEF_MASK                                                (0x00FF0000UL) /**< rw - Pattern definition for Symbol3 . */
#define CLIF_TX_SYMBOL23_DEF_REG_TX_SYMBOL3_DEF_POS                                                 (16UL)
#define CLIF_TX_SYMBOL23_DEF_REG_TX_SYMBOL2_DEF_MASK                                                (0x000000FFUL) /**< rw - Pattern definition for Symbol2. */
#define CLIF_TX_SYMBOL23_DEF_REG_TX_SYMBOL2_DEF_POS                                                 (0UL)
#define CLIF_TX_SYMBOL23_DEF_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_SYMBOL01_MOD_REG                                                                    (0x4000404CUL) /**< rw*/
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_MODWIDTH_MASK                                               (0x00FF0000UL) /**< rw - Specifies the length of a pulse for sending data of symbol 0/1. The length is given by the number of carrier clocks + 1. . */
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_MODWIDTH_POS                                                (16UL)
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_MILLER_ENABLE_MASK                                          (0x00000100UL) /**< rw - If set to 1. pulse modulation is applied according to modified miller coding. . */
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_MILLER_ENABLE_POS                                           (8UL)
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_INV_ENV_MASK                                                (0x00000080UL) /**< rw - If set to 1. the output envelope is inverted. . */
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_INV_ENV_POS                                                 (7UL)
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_ENV_TYPE_MASK                                               (0x00000070UL) /**< rw - Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple 111b RFU. */
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_ENV_TYPE_POS                                                (4UL)
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_SC_FREQ_MASK                                                (0x00000008UL) /**< rw - Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz . */
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_SC_FREQ_POS                                                 (3UL)
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_BIT_FREQ_MASK                                               (0x00000007UL) /**< rw - Specifies the frequency of the bit-stream.  000b -> 1.695 MHz. 001b -> Reserved. 010b -> 26 kHz. 011b -> 53 kHz. 100b -> 106 kHz. 101b -> 212 kHz. 110b -> 424 kHz. 111b -> 848 kHz.. */
#define CLIF_TX_SYMBOL01_MOD_REG_TX_S01_BIT_FREQ_POS                                                (0UL)
#define CLIF_TX_SYMBOL01_MOD_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_SYMBOL23_MOD_REG                                                                    (0x40004050UL) /**< rw*/
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_MODWIDTH_MASK                                               (0x00FF0000UL) /**< rw - Specifies the length of a pulse for sending data of symbol 2/3. The length is given by the number of carrier clocks + 1.. */
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_MODWIDTH_POS                                                (16UL)
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_MILLER_ENABLE_MASK                                          (0x00000100UL) /**< rw - If set to 1 pulse modulation is applied according to modified miller coding.. */
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_MILLER_ENABLE_POS                                           (8UL)
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_INV_ENV_MASK                                                (0x00000080UL) /**< rw - If set to 1 the output envelope is inverted.. */
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_INV_ENV_POS                                                 (7UL)
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_ENV_TYPE_MASK                                               (0x00000070UL) /**< rw - Specifies the type of envelope used for transmission of data packets. The selected envelope type is applied to the pseudo bit stream. 000b Direct output 001b Manchester code 010b Manchster code with subcarrier 011b BPSK 100b RZ (pulse of half bit length at beginning of second half of bit) 101b RZ (pulse of half bit length at beginning of bit) 110b Manchester tupple 111b RFU. */
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_ENV_TYPE_POS                                                (4UL)
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_SC_FREQ_MASK                                                (0x00000008UL) /**< rw - Specifies the frequency of the subcarrier. 0 424 kHz 1 848 kHz. */
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_SC_FREQ_POS                                                 (3UL)
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_BIT_FREQ_MASK                                               (0x00000007UL) /**< rw - Specifies the frequency of the bit-stream.  000b -> 1.695 MHz. 001b -> Reserved. 010b -> 26 kHz. 011b -> 53 kHz. 100b -> 106 kHz. 101b -> 212 kHz. 110b -> 424 kHz. 111b -> 848 kHz.. */
#define CLIF_TX_SYMBOL23_MOD_REG_TX_S23_BIT_FREQ_POS                                                (0UL)
#define CLIF_TX_SYMBOL23_MOD_REG__RESET_VALUE                                                       (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder overshot prevention config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_OVERSHOOT_CONFIG_REG                                                                (0x40004054UL) /**< rw*/
#define CLIF_TX_OVERSHOOT_CONFIG_REG_TX_OVERSHOOT_PATTERN_MASK                                      (0xFFFF0000UL) /**< rw - Overshoot pattern which is transmitted after each rising egde. . */
#define CLIF_TX_OVERSHOOT_CONFIG_REG_TX_OVERSHOOT_PATTERN_POS                                       (16UL)
#define CLIF_TX_OVERSHOOT_CONFIG_REG_TX_OVERSHOOT_PATTERN_LEN_MASK                                  (0x0000001EUL) /**< rw - Defines length of the overshoot prevention pattern (value +1). The pattern is applied starting from the LSB of the defined pattern. all other bits are ignored. . */
#define CLIF_TX_OVERSHOOT_CONFIG_REG_TX_OVERSHOOT_PATTERN_LEN_POS                                   (1UL)
#define CLIF_TX_OVERSHOOT_CONFIG_REG_TX_OVERSHOOT_PROT_ENABLE_MASK                                  (0x00000001UL) /**< rw - If set to 1. the overshoot protection is enabled.. */
#define CLIF_TX_OVERSHOOT_CONFIG_REG_TX_OVERSHOOT_PROT_ENABLE_POS                                   (0UL)
#define CLIF_TX_OVERSHOOT_CONFIG_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TxEncoder undershot prevention config register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_UNDERSHOOT_CONFIG_REG                                                               (0x40004058UL) /**< rw*/
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PATTERN_MASK                                    (0xFFFF0000UL) /**< rw - Undershoot pattern which is transmitted after each falling egde. . */
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PATTERN_POS                                     (16UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PROT_LAST_SC_ENABLE_MASK                        (0x00000020UL) /**< rw - This mode activates the undershoot prevention circuit only for the last sub-carrier cycle for card-mode transmission - Note: The bit TX_UNDERSHOOT_PROT_ENABLE must not be set in this mode. . */
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PROT_LAST_SC_ENABLE_POS                         (5UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PATTERN_LEN_MASK                                (0x0000001EUL) /**< rw - Defines length of the undershoot prevention pattern (value +1). The pattern is applied starting from the LSB of the defined pattern. all other bits are ignored. . */
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PATTERN_LEN_POS                                 (1UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PROT_ENABLE_MASK                                (0x00000001UL) /**< rw - If set to 1. the undershoot protection is enabled. */
#define CLIF_TX_UNDERSHOOT_CONFIG_REG_TX_UNDERSHOOT_PROT_ENABLE_POS                                 (0UL)
#define CLIF_TX_UNDERSHOOT_CONFIG_REG__RESET_VALUE                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name RxDecoder registers
* <b>Reset value:</b> 0x00000002\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_RX_CONFIG_REG                                                                          (0x4000405CUL) /**< rw*/
#define CLIF_RX_CONFIG_REG_RX_PARITY_EMD_ON_SOVER_MASK                                              (0x80000000UL) /**< rw - if set decision if EMD due to parity error is taken at sover (depending on frame length). */
#define CLIF_RX_CONFIG_REG_RX_PARITY_EMD_ON_SOVER_POS                                               (31UL)
#define CLIF_RX_CONFIG_REG_RX_MISSING_PARITY_IS_EMD_MASK                                            (0x40000000UL) /**< rw - if set a missing parity bit in the 4th byte is treated as EMD (EMD options 3/4). */
#define CLIF_RX_CONFIG_REG_RX_MISSING_PARITY_IS_EMD_POS                                             (30UL)
#define CLIF_RX_CONFIG_REG_RX_ADVANCED_EMD_ENABLE_MASK                                              (0x20000000UL) /**< rw - if set new EMD options are enabled. */
#define CLIF_RX_CONFIG_REG_RX_ADVANCED_EMD_ENABLE_POS                                               (29UL)
#define CLIF_RX_CONFIG_REG_RX_PARITY_ERROR_IS_EMD_MASK                                              (0x10000000UL) /**< rw - If set to 1 a parity error in the 3rd/4th byte (depending on RX_EMD_SUP setting) is interpreted as an EMD error. Otherwise it is interpreted as a parity error.. */
#define CLIF_RX_CONFIG_REG_RX_PARITY_ERROR_IS_EMD_POS                                               (28UL)
#define CLIF_RX_CONFIG_REG_RX_EMD_SUP_MASK                                                          (0x0E000000UL) /**< rw - Defines EMD suppression mechanism 000 Off 001 EMD suppression according to ISO14443 010 EMD suppression according to NFC Forum (with respect to the first 3 characters) 011 EMD suppression according to NFC Forum (with respect to the first 4characters) 100 EMD suppression according to NFC Forum (with respect to the first 4characters all valid frames <4 bytes are ignored) 101-111 RFU. */
#define CLIF_RX_CONFIG_REG_RX_EMD_SUP_POS                                                           (25UL)
#define CLIF_RX_CONFIG_REG_RX_COLL_IS_DATA_ERROR_MASK                                               (0x01000000UL) /**< rw - If set to 1. a collision is treated as a data integrity error (especially for ISO14443-4) . */
#define CLIF_RX_CONFIG_REG_RX_COLL_IS_DATA_ERROR_POS                                                (24UL)
#define CLIF_RX_CONFIG_REG_VALUES_AFTER_COLLISION_MASK                                              (0x00800000UL) /**< rw - This bit defined the value of bits received after a collision occured. 0* All received bits after a collision will be cleared. 1 All received bits after a collision keep their value. . */
#define CLIF_RX_CONFIG_REG_VALUES_AFTER_COLLISION_POS                                               (23UL)
#define CLIF_RX_CONFIG_REG_RX_CRC_ALLOW_BITS_MASK                                                   (0x00400000UL) /**< rw - Set to 1. a frame with less than one byte length is writtten to ram. when the CRC is enabled otherwise it is discarted. . */
#define CLIF_RX_CONFIG_REG_RX_CRC_ALLOW_BITS_POS                                                    (22UL)
#define CLIF_RX_CONFIG_REG_RX_FORCE_CRC_WRITE_MASK                                                  (0x00200000UL) /**< rw - Set to 1. the received CRC byte(s) are written to ram. In normal operation (if this bit is set to 0) CRC bytes are only checked and removed from the data stream. . */
#define CLIF_RX_CONFIG_REG_RX_FORCE_CRC_WRITE_POS                                                   (21UL)
#define CLIF_RX_CONFIG_REG_RX_PARITY_ERR_8BITS_ENABLE_MASK                                          (0x00080000UL) /**< rw - if set parity error is detected when only 8 bits in last byte received. */
#define CLIF_RX_CONFIG_REG_RX_PARITY_ERR_8BITS_ENABLE_POS                                           (19UL)
#define CLIF_RX_CONFIG_REG_RX_BIT_ALIGN_MASK                                                        (0x00070000UL) /**< rw - RxAlign defines the bit position within the byte for the first bit received. Further received bits are stored at the following bit positions.    . */
#define CLIF_RX_CONFIG_REG_RX_BIT_ALIGN_POS                                                         (16UL)
#define CLIF_RX_CONFIG_REG_RX_EOF_SYMBOL_MASK                                                       (0x0000FF00UL) /**< rw - This value defines the pattern of the EOF symbol. Note: All bits are clear upon enabling of the receiver is the automatic mode detection is enabled. If in such a case an ISO14443A communication is detected bit0 is set by the mod-detector. . */
#define CLIF_RX_CONFIG_REG_RX_EOF_SYMBOL_POS                                                        (8UL)
#define CLIF_RX_CONFIG_REG_RX_EOF_TYPE_MASK                                                         (0x00000040UL) /**< rw - Defines the EOF handling 0 EOF as defined in the register field RX_EOF_SYMBOL is expected. 1 EOF as defined in ISO14443B is expected. . */
#define CLIF_RX_CONFIG_REG_RX_EOF_TYPE_POS                                                          (6UL)
#define CLIF_RX_CONFIG_REG_RX_STOP_CONDITION_MASK                                                   (0x00000030UL) /**< rw - This bit field defines the condition to stop a reception. Note: This bit is set by the mod-detector if automatic mode detection is enabled and the corresponding communication is detected. 00b Reception is stopped only if end of data communication is detected by the signal processing 01b Reception is stopped upon reception of the EOF pattern selected by the register field RX_EOF_TYPE and RX_EOF_SYMBOL Note: This value is set by the mod-detector if a ISO14443A or ISO14443B communication is detected 10b Reception is stopped on detection of an incorrect parity bit. 11b Recption is stopped if the number of received bytes reaches the defined frame length (which extracted from the first data-byte received). Note: This value is set by the mod-detector if a Felica communication is detected . */
#define CLIF_RX_CONFIG_REG_RX_STOP_CONDITION_POS                                                    (4UL)
#define CLIF_RX_CONFIG_REG_RX_MSB_FIRST_MASK                                                        (0x00000008UL) /**< rw - If set to 1. data bytes are interpreted MSB first for reception . */
#define CLIF_RX_CONFIG_REG_RX_MSB_FIRST_POS                                                         (3UL)
#define CLIF_RX_CONFIG_REG_RX_STOPBIT_ENABLE_MASK                                                   (0x00000004UL) /**< rw - Set to 1. a stop-bit is expected and will be checked and extracted from data stream. Note: The stop-bit is not configurable and fixed to a logic 1. Note: This bit is set by the mod-detector if automatic mode detection is enabled and ISO14443B communication is detected.. */
#define CLIF_RX_CONFIG_REG_RX_STOPBIT_ENABLE_POS                                                    (2UL)
#define CLIF_RX_CONFIG_REG_RX_PARITY_TYPE_MASK                                                      (0x00000002UL) /**< rw - Defines which type of the parity-bit is used Note: This bit is set by the mod-detector if automatic mode detection is enabled and ISO14443A communication is detected. 0 Even parity calculation is used 1 Odd parity calculation is used . */
#define CLIF_RX_CONFIG_REG_RX_PARITY_TYPE_POS                                                       (1UL)
#define CLIF_RX_CONFIG_REG_RX_PARITY_ENABLE_MASK                                                    (0x00000001UL) /**< rw - If set to 1. a parity-bit for each byte is expected. will be extracted from data stream and checked for correctness. In case the parity-bit is incorrect. the RX_DATA_INTEGRITY_ERROR flag is set. Nevertheless the reception is continued. Note: This bit is set by the mod-detector if automatic mode detection is enabled and ISO14443A communication is detected.. */
#define CLIF_RX_CONFIG_REG_RX_PARITY_ENABLE_POS                                                     (0UL)
#define CLIF_RX_CONFIG_REG__RESET_VALUE                                                             (0x00000002UL) /**< value after reset */
/**
* @}
*/


/**
* \name RxDecoder registers
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_RX_STATUS_REG                                                                          (0x40004060UL) /**< r-*/
#define CLIF_RX_STATUS_REG_RX_COLL_POS_MASK                                                         (0x7F000000UL) /**< r- - These bits show the bit position of the first detected collision in a received frame (only databits are interpreted). Note: These bits shall only be interpreted in passive communication mode at 106 kbit/s or ISO14443A /MIFARE® reader / writer or ICode SLI reader / writer mode if bit CollPosValid is set to 1. Note: If RX_ALIGN is set to a value different to 0.this value is included in the RX_COLL_POS. . */
#define CLIF_RX_STATUS_REG_RX_COLL_POS_POS                                                          (24UL)
#define CLIF_RX_STATUS_REG_RX_WRITE_ERROR_MASK                                                      (0x00800000UL) /**< r- - This error flag is set to 1. if for an ongoing reception data is not copied to RAM in time (BMA encountered write error) and therefor the reception is aborted Note: This case should not happen in normal operation . */
#define CLIF_RX_STATUS_REG_RX_WRITE_ERROR_POS                                                       (23UL)
#define CLIF_RX_STATUS_REG_RX_LATENCY_ERROR_MASK                                                    (0x00400000UL) /**< r- - This error flag is set to 1. if for an ongoing reception data is not copied to RAM in time (BMA latency to big) and therefor the reception is aborted Note: This case should not happen in normal operation . */
#define CLIF_RX_STATUS_REG_RX_LATENCY_ERROR_POS                                                     (22UL)
#define CLIF_RX_STATUS_REG_RX_BUFFER_OVFL_ERROR_MASK                                                (0x00200000UL) /**< r- - This flag is set to 1 when the number of received bytes exceeds the size of the RxBuffer . */
#define CLIF_RX_STATUS_REG_RX_BUFFER_OVFL_ERROR_POS                                                 (21UL)
#define CLIF_RX_STATUS_REG_RX_CL_ERROR_MASK                                                         (0x00100000UL) /**< r- - General contactless error. is a combination of data integrity. protocol and collision error . */
#define CLIF_RX_STATUS_REG_RX_CL_ERROR_POS                                                          (20UL)
#define CLIF_RX_STATUS_REG_RX_LEN_ERROR_MASK                                                        (0x00080000UL) /**< r- - This flag is set to 1. if RxMultiple is activate and the LEN byte indicates more than 28 bytes        . */
#define CLIF_RX_STATUS_REG_RX_LEN_ERROR_POS                                                         (19UL)
#define CLIF_RX_STATUS_REG_RX_COLLISION_DETECTED_MASK                                               (0x00040000UL) /**< r- - This flag is set to 1. when a collision has accured. The position of the first collision is shown in the register RX_COLLPOS. */
#define CLIF_RX_STATUS_REG_RX_COLLISION_DETECTED_POS                                                (18UL)
#define CLIF_RX_STATUS_REG_RX_PROTOCOL_ERROR_MASK                                                   (0x00020000UL) /**< r- - This flag is set to 1. when a protocol error has accured. A protocol error can be a wrong stopbit. a missing or wrong ISO14443B EOF or SOF or a wrong number of received data bytes. Note: When a protocol error is detected. data reception is stopped. Note: The flag is automatically cleared at start of next reception. . */
#define CLIF_RX_STATUS_REG_RX_PROTOCOL_ERROR_POS                                                    (17UL)
#define CLIF_RX_STATUS_REG_RX_DATA_INTEGRITY_ERROR_MASK                                             (0x00010000UL) /**< r- - This flag is set to 1. if a data integrity error has been detected. Possible caused can be a wrong parity or a wrong CRC. Note: On a data integrity error the reception is continued Note: The flag is automatically cleared at start of next reception. Note: If a reversed parity bit is a stop criteria. the flag is not set to 1 in case of a wrong parity. . */
#define CLIF_RX_STATUS_REG_RX_DATA_INTEGRITY_ERROR_POS                                              (16UL)
#define CLIF_RX_STATUS_REG_RX_NUM_LAST_BITS_MASK                                                    (0x0000E000UL) /**< r- - Defines the number of valid bits of the last data byte received in bit-oriented communications. If zero the whole byte is valid. . */
#define CLIF_RX_STATUS_REG_RX_NUM_LAST_BITS_POS                                                     (13UL)
#define CLIF_RX_STATUS_REG_RX_NUM_FRAMES_RECEIVED_MASK                                              (0x00001E00UL) /**< r- - Indicates the number of frames received. The value is updated when the RxIRQ is raised. Note: This bit field is only valid when the RxMultiple is active (bit RX_MULTIPLE_ENABLE set) . */
#define CLIF_RX_STATUS_REG_RX_NUM_FRAMES_RECEIVED_POS                                               (9UL)
#define CLIF_RX_STATUS_REG_RX_NUM_BYTES_RECEIVED_MASK                                               (0x000001FFUL) /**< r- - the number of bytes received. The value is valid when the RxIRQ is raised until the receiver is enabled again.. */
#define CLIF_RX_STATUS_REG_RX_NUM_BYTES_RECEIVED_POS                                                (0UL)
#define CLIF_RX_STATUS_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CMA registers
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_CMA_REG                                                                                (0x40004064UL) /**< rw*/
#define CLIF_CMA_REG_CMA_NUM_CL_MASK                                                                (0x00001800UL) /**< rw - 3 Defines the number of cascade levels (length of UID) 0 4 byte UID (number of CLs = 1) 1 7 byte UID (number of CLs = 2) 2 10 byte UID (number of CLs = 3) 3 RFU. */
#define CLIF_CMA_REG_CMA_NUM_CL_POS                                                                 (11UL)
#define CLIF_CMA_REG_CMA_STATE_CHANGE_MASK                                                          (0x00000400UL) /**< rw - 3 Set CMA FSM to IDLE or HALT state 0 Remain in current state 1 SET to IDLE state 2 SET to HALT state 3 RFU. */
#define CLIF_CMA_REG_CMA_STATE_CHANGE_POS                                                           (10UL)
#define CLIF_CMA_REG_CMA_STATE_CHANGE_EXEC_MASK                                                     (0x00000200UL) /**< rw - if set to 1 the value of CMA_STATE_CHANGE has been modified. After the CMA module has taken over the new value the register bit is reset to 0. */
#define CLIF_CMA_REG_CMA_STATE_CHANGE_EXEC_POS                                                      (9UL)
#define CLIF_CMA_REG_BYPASS_MODE_ENABLE_MASK                                                        (0x00000100UL) /**< rw - 1 If set to 1 bypass option for CL2 is enabled . */
#define CLIF_CMA_REG_BYPASS_MODE_ENABLE_POS                                                         (8UL)
#define CLIF_CMA_REG_CMA_VALID_CMDA_RECEIVED_MASK                                                   (0x00000040UL) /**< r- - Indicates that CMA received at least one valid TypeA command in IDLE state. */
#define CLIF_CMA_REG_CMA_VALID_CMDA_RECEIVED_POS                                                    (6UL)
#define CLIF_CMA_REG_CMA_STATE_MASK                                                                 (0x0000003CUL) /**< r- - F Indicates the state of the CMA FSM 0 MODE_DET 1 IDLE(SENSE) 2 READY1(RESOLUTION1) 3 READY2(RESOLUTION2) 4 READY3(RESOLUTION3) 5 ACTIVE(SELECTED) 6 HALT(SLEEP) 7 READY1*(RESOLUTION1*) 8 READY2*(RESOLUTION2*) 9 READY3*(RESOLUTION3*) A ACTIVE*(SELECTED*) B-F RFU. */
#define CLIF_CMA_REG_CMA_STATE_POS                                                                  (2UL)
#define CLIF_CMA_REG_CMA_ACTIVE_MASK                                                                (0x00000002UL) /**< r- - 1 If set to 1 the CMA module is active (controls configuration settings) . */
#define CLIF_CMA_REG_CMA_ACTIVE_POS                                                                 (1UL)
#define CLIF_CMA_REG_CMA_ENABLE_MASK                                                                (0x00000001UL) /**< rw - 1 If set to 1 the Card Mode Activation is enabled. */
#define CLIF_CMA_REG_CMA_ENABLE_POS                                                                 (0UL)
#define CLIF_CMA_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name NFC-WI register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_NFCWI_REG                                                                              (0x40004068UL) /**< rw*/
#define CLIF_NFCWI_REG_SE_PICC_ALIGN_ENABLE_MASK                                                    (0x00008000UL) /**< rw - If set to 1 SE response and internal PICC response is aligned. Note: activate only if SE and internal PICC are used together!. */
#define CLIF_NFCWI_REG_SE_PICC_ALIGN_ENABLE_POS                                                     (15UL)
#define CLIF_NFCWI_REG_SIGIN_ACT_VALUE_MASK                                                         (0x00004000UL) /**< r- - Current value of sigIn_act signal. */
#define CLIF_NFCWI_REG_SIGIN_ACT_VALUE_POS                                                          (14UL)
#define CLIF_NFCWI_REG_SIGIN_VALUE_MASK                                                             (0x00002000UL) /**< r- - Current value at the SigIn pad (ZIA). */
#define CLIF_NFCWI_REG_SIGIN_VALUE_POS                                                              (13UL)
#define CLIF_NFCWI_REG_WIRED_MODE_SEL_MASK                                                          (0x00001000UL) /**< rw - 1 Must be set to 1 if the NFC-WI wired mode should be used . */
#define CLIF_NFCWI_REG_WIRED_MODE_SEL_POS                                                           (12UL)
#define CLIF_NFCWI_REG_NFCWI_RING_ENABLE_MASK                                                       (0x00000800UL) /**< rw - 1 If set to 1. the ring detection mechanism is enabled        . */
#define CLIF_NFCWI_REG_NFCWI_RING_ENABLE_POS                                                        (11UL)
#define CLIF_NFCWI_REG_SIGIN_ACT_IRQ_POS_MASK                                                       (0x00000600UL) /**< rw - Defines at which position of sigin_act the SIGIN_ACT_IRQ is set 00b* SIGIN_ACT_IRQ is set at the rising edge of sigin_act 01b SIGIN_ACT_IRQ is set at the falling edge of sigin_act 10b SIGIN_ACT_IRQ is set at both edges of sigin_act 11b RFU. */
#define CLIF_NFCWI_REG_SIGIN_ACT_IRQ_POS_POS                                                        (9UL)
#define CLIF_NFCWI_REG_SIGIN_DELAY_ENABLE_MASK                                                      (0x00000100UL) /**< rw - 1 If set to 1. the delay circuit for the SigIn signal is avtivated and the delayed signal is used as the internal SigIn signal. Note: This feature is intented to be used only for ISO14443A communication @ 106kbit/s via NFC-WI Note: CLTimer0 has to be used to set up the bit-grid for the response . */
#define CLIF_NFCWI_REG_SIGIN_DELAY_ENABLE_POS                                                       (8UL)
#define CLIF_NFCWI_REG_SIGIN_INV_MASK                                                               (0x00000080UL) /**< rw - 1 If set to 1. the polarity of the SigIn signal is reversed before it is processed internally . */
#define CLIF_NFCWI_REG_SIGIN_INV_POS                                                                (7UL)
#define CLIF_NFCWI_REG_SIGOUT_INV_MASK                                                              (0x00000040UL) /**< rw - 1 If set to 1. the polarity of the signal on SigOut is reversed . */
#define CLIF_NFCWI_REG_SIGOUT_INV_POS                                                               (6UL)
#define CLIF_NFCWI_REG_SIGOUT_CLOCK_SEL_MASK                                                        (0x00000030UL) /**< rw - 3 Defines how the 13.56MHz clock is combined with the SigOut signal 00* Plain signal on SigOut (no combination with clock) 01 13.56MHz clock is XORed with the SigOut signal (ISO14443B. Felica) 10 13.56MHz clock is ANDed with the SigOut signal (ISO14443A @106kbit/s) 11 RFU. */
#define CLIF_NFCWI_REG_SIGOUT_CLOCK_SEL_POS                                                         (4UL)
#define CLIF_NFCWI_REG_SIGOUT_SEL_MASK                                                              (0x0000000FUL) /**< rw - F Defines which signal is put on SigOut 0000* Low 0001 High 0010 TxEnvelope 0011 miller envelope from analog frontend 0100 manchester envelope from analog frontend 0101 nrz envelope from analog frontend 0110 ADCBCM envelope 0111 selected testbus bit 1000-1111 RFU. */
#define CLIF_NFCWI_REG_SIGOUT_SEL_POS                                                               (0UL)
#define CLIF_NFCWI_REG__RESET_VALUE                                                                 (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CRC registers
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_CRC_RX_CONFIG_REG                                                                      (0x4000406CUL) /**< rw*/
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_PRESET_VALUE_MASK                                             (0xFFFF0000UL) /**< rw - Arbitrary preset value for the Rx-Decoder CRC calculation. . */
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_PRESET_VALUE_POS                                              (16UL)
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_PRESET_SEL_MASK                                               (0x00000038UL) /**< rw - Preset value of the CRC register for the Rx-Decoder. For a CRC calculation using 5bits only the LSByte is used. 000b* 0000h reset value Note that this configuration is set by the Mode detector for FeliCa. 001b  6363h Note that this configuration is set by the Mode detector for ISO14443 type A. 010b  A671h 011b FFFFh Note that this configuration is set by the Mode detector for ISO14443 type B. 100b 0012h 101b E012h 110b RFU 111b Use arbitrary preset value RX_CRC_PRESET_VALUE. */
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_PRESET_SEL_POS                                                (3UL)
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_TYPE_MASK                                                     (0x00000004UL) /**< rw - Controls the type of CRC calulation for the Rx-Decoder 0* 16bit CRC calculation. reset value 1 5bit CRC calculation . */
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_TYPE_POS                                                      (2UL)
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_INV_MASK                                                      (0x00000002UL) /**< rw - Controls the comparison of the CRC checksum for the Rx-Decoder 0* Not inverted CRC value: 0000h reset value Note that this nit is cleared by the Mode detector for ISO14443 type A and FeliCa. 1 Inverted CRC value: F0B8h Note that this bit is set by the Mode detector for ISO14443 type B.. */
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_INV_POS                                                       (1UL)
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_ENABLE_MASK                                                   (0x00000001UL) /**< rw - If set. the Rx-Decoder will check the CRC for correctness. Note that this bit is set by the Mode Detector. when ISO14443 type B. or FeliCa (212 or 424kBd) is detected.. */
#define CLIF_CRC_RX_CONFIG_REG_RX_CRC_ENABLE_POS                                                    (0UL)
#define CLIF_CRC_RX_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CRC registers
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_CRC_TX_CONFIG_REG                                                                      (0x40004070UL) /**< rw*/
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_PRESET_VALUE_MASK                                             (0xFFFF0000UL) /**< rw - Arbitrary preset value for the Tx-Encoder CRC calculation. . */
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_PRESET_VALUE_POS                                              (16UL)
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_BYTE2_ENABLE_MASK                                             (0x00000040UL) /**< rw - If set. the CRC is calculated from the 2nd byte onwards (intended for HID). Note that this option is used in the Tx-Encoder.           . */
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_BYTE2_ENABLE_POS                                              (6UL)
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_PRESET_SEL_MASK                                               (0x00000038UL) /**< rw - Preset value of the CRC register for the Tx-Encoder. For a CRC calculation using 5bits. only the LSByte is used. 000b* 0000h. reset value 001b  6363h 010b A671h 011b  FFFFh 100b 0012h 101b E012h 110b RFU 111b Use arbitrary preset value TX_CRC_PRESET_VALUE. */
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_PRESET_SEL_POS                                                (3UL)
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_TYPE_MASK                                                     (0x00000004UL) /**< rw - Controls the type of CRC calulation for the Tx-Encoder 0* 16bit CRC calculation. reset value 1 5bit CRC calculation . */
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_TYPE_POS                                                      (2UL)
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_INV_MASK                                                      (0x00000002UL) /**< rw - Controls the sending of an inverted CRC value by the Tx-Encoder 0* Not inverted CRC checksum. reset value 1 Inverted CRC checksum . */
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_INV_POS                                                       (1UL)
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_ENABLE_MASK                                                   (0x00000001UL) /**< rw - If set to one. the Tx-Encoder will compute and transmit a CRC.. */
#define CLIF_CRC_TX_CONFIG_REG_TX_CRC_ENABLE_POS                                                    (0UL)
#define CLIF_CRC_TX_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CRC registers
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_CRC_DATA_REG                                                                           (0x40004074UL) /**< r-*/
#define CLIF_CRC_DATA_REG_CRC_DATA_MASK                                                             (0x0000FFFFUL) /**< r- - CRC register value . */
#define CLIF_CRC_DATA_REG_CRC_DATA_POS                                                              (0UL)
#define CLIF_CRC_DATA_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER0_CONFIG_REG                                                                      (0x40004078UL) /**< rw*/
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_T3_EXPIRED_MASK                                           (0x80000000UL) /**< rw - T0_STOP_EVENT: If set the timer T0 is stopped when timer T3 raises its expiration flag.. */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_T3_EXPIRED_POS                                            (31UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RX_STARTED_MASK                                           (0x40000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when a data reception begins (1st bit is received). . */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RX_STARTED_POS                                            (30UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_TX_STARTED_MASK                                           (0x20000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when a data transmission begins. . */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_TX_STARTED_POS                                            (29UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_ON_EXT_MASK                                            (0x10000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when the external RF field is detected.. */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_ON_EXT_POS                                             (28UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_OFF_EXT_MASK                                           (0x08000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when the external RF field vanishes. . */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_OFF_EXT_POS                                            (27UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_ON_INT_MASK                                            (0x04000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when the internal RF field is turned on. . */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_ON_INT_POS                                             (26UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_OFF_INT_MASK                                           (0x02000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when the internal RF field is turned off. . */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_RF_OFF_INT_POS                                            (25UL)
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_SIGIN_ACT_MASK                                            (0x01000000UL) /**< rw - T0_STOP_EVENT: If set. the timer T0 is stopped when an activity on SigIn is detected. . */
#define CLIF_TIMER0_CONFIG_REG_T0_STOP_ON_SIGIN_ACT_POS                                             (24UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_T3_RUNNING_MASK                                          (0x00040000UL) /**< rw - T0_START_EVENT: If set the timer T0 is started when the timer T3 starts running.. */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_T3_RUNNING_POS                                           (18UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RX_STARTED_MASK                                          (0x00020000UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when a data reception begins (1st bit is received). . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RX_STARTED_POS                                           (17UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RX_ENDED_MASK                                            (0x00010000UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when a data reception ends. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RX_ENDED_POS                                             (16UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_TX_STARTED_MASK                                          (0x00008000UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when a data transmission begins. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_TX_STARTED_POS                                           (15UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_TX_ENDED_MASK                                            (0x00004000UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when a data transmission ends. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_TX_ENDED_POS                                             (14UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_ON_EXT_MASK                                           (0x00002000UL) /**< rw - T0_START_EVENT: If set. the timer T0 is startedwhen the external RF field is detected. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_ON_EXT_POS                                            (13UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_OFF_EXT_MASK                                          (0x00001000UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when the external RF field is not detected any more. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_OFF_EXT_POS                                           (12UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_ON_INT_MASK                                           (0x00000800UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when an internal RF field is turned on. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_ON_INT_POS                                            (11UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_OFF_INT_MASK                                          (0x00000400UL) /**< rw - T0_START_EVENT:If set. the timer T0 is started when an internal RF field is turned off. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_RF_OFF_INT_POS                                           (10UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_SIGIN_ACT_MASK                                           (0x00000200UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started when an activity on SigIn is detected. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_ON_SIGIN_ACT_POS                                            (9UL)
#define CLIF_TIMER0_CONFIG_REG_T0_START_NOW_MASK                                                    (0x00000100UL) /**< rw - T0_START_EVENT: If set. the timer T0 is started immediatly. . */
#define CLIF_TIMER0_CONFIG_REG_T0_START_NOW_POS                                                     (8UL)
#define CLIF_TIMER0_CONFIG_REG_T0_PRESCALE_SEL_MASK                                                 (0x00000038UL) /**< rw - Controls frequency/period of the timer T0 when the prescaler is activated in T0_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter   . */
#define CLIF_TIMER0_CONFIG_REG_T0_PRESCALE_SEL_POS                                                  (3UL)
#define CLIF_TIMER0_CONFIG_REG_T0_MODE_SEL_MASK                                                     (0x00000004UL) /**< rw - Configuration of the timer T0 clock. 0b* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1b Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T0_PRESCALE_SEL).. */
#define CLIF_TIMER0_CONFIG_REG_T0_MODE_SEL_POS                                                      (2UL)
#define CLIF_TIMER0_CONFIG_REG_T0_RELOAD_ENABLE_MASK                                                (0x00000002UL) /**< rw - If set to 0.the timer T0 will stop on expiration. 0* After expiration the timer T0 will stop counting. i.e.. remain zero. reset value. 1 After expiration the timer T0 will reload its preset value and continue counting down. . */
#define CLIF_TIMER0_CONFIG_REG_T0_RELOAD_ENABLE_POS                                                 (1UL)
#define CLIF_TIMER0_CONFIG_REG_T0_ENABLE_MASK                                                       (0x00000001UL) /**< rw - Enables the timer T0. */
#define CLIF_TIMER0_CONFIG_REG_T0_ENABLE_POS                                                        (0UL)
#define CLIF_TIMER0_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER1_CONFIG_REG                                                                      (0x4000407CUL) /**< rw*/
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_T3_EXPIRED_MASK                                           (0x80000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when timer T3 raises its expiration flag. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_T3_EXPIRED_POS                                            (31UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RX_STARTED_MASK                                           (0x40000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when a data reception begins (1st bit is received). . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RX_STARTED_POS                                            (30UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_TX_STARTED_MASK                                           (0x20000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when a data transmission begins. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_TX_STARTED_POS                                            (29UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_ON_EXT_MASK                                            (0x10000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when the external RF field is detected. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_ON_EXT_POS                                             (28UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_OFF_EXT_MASK                                           (0x08000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when the external RF field vanishes. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_OFF_EXT_POS                                            (27UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_ON_INT_MASK                                            (0x04000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when the internal RF field is turned on. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_ON_INT_POS                                             (26UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_OFF_INT_MASK                                           (0x02000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when the internal RF field is turned off. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_RF_OFF_INT_POS                                            (25UL)
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_SIGIN_ACT_MASK                                            (0x01000000UL) /**< rw - T1_STOP_EVENT: If set. the timer T1 is stopped when an activity on SigIn is detected. . */
#define CLIF_TIMER1_CONFIG_REG_T1_STOP_ON_SIGIN_ACT_POS                                             (24UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_T3_RUNNING_MASK                                          (0x00040000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when timer T3 has started. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_T3_RUNNING_POS                                           (18UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RX_STARTED_MASK                                          (0x00020000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when a data reception begins (1st bit is received). . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RX_STARTED_POS                                           (17UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RX_ENDED_MASK                                            (0x00010000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when a data reception ends. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RX_ENDED_POS                                             (16UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_TX_STARTED_MASK                                          (0x00008000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when a data transmission begins.   . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_TX_STARTED_POS                                           (15UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_TX_ENDED_MASK                                            (0x00004000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when a data transmission ends. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_TX_ENDED_POS                                             (14UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_ON_EXT_MASK                                           (0x00002000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is startedwhen the external RF field is detected. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_ON_EXT_POS                                            (13UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_OFF_EXT_MASK                                          (0x00001000UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when the external RF field is not detected any more. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_OFF_EXT_POS                                           (12UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_ON_INT_MASK                                           (0x00000800UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when an internal RF field is turned on. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_ON_INT_POS                                            (11UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_OFF_INT_MASK                                          (0x00000400UL) /**< rw - T1_START_EVENT:If set. the timer T1 is started when an internal RF field is turned off. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_RF_OFF_INT_POS                                           (10UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_SIGIN_ACT_MASK                                           (0x00000200UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started when an activity on SigIn is detected. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_ON_SIGIN_ACT_POS                                            (9UL)
#define CLIF_TIMER1_CONFIG_REG_T1_START_NOW_MASK                                                    (0x00000100UL) /**< rw - T1_START_EVENT: If set. the timer T1 is started immediatly. . */
#define CLIF_TIMER1_CONFIG_REG_T1_START_NOW_POS                                                     (8UL)
#define CLIF_TIMER1_CONFIG_REG_T1_PRESCALE_SEL_MASK                                                 (0x00000038UL) /**< rw - Controls frequency/period of the timer T1 when the prescaler is activated by T1_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter . */
#define CLIF_TIMER1_CONFIG_REG_T1_PRESCALE_SEL_POS                                                  (3UL)
#define CLIF_TIMER1_CONFIG_REG_T1_MODE_SEL_MASK                                                     (0x00000004UL) /**< rw - If set. the timer T1 is started the prescaler for the timer T1 is enabled. 0* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1 Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T1_PRESCALE_SEL). . */
#define CLIF_TIMER1_CONFIG_REG_T1_MODE_SEL_POS                                                      (2UL)
#define CLIF_TIMER1_CONFIG_REG_T1_RELOAD_ENABLE_MASK                                                (0x00000002UL) /**< rw - If set to 0.the timer T1 will stop on expiration. 0* After expiration the timer T1 will stop counting. i.e.. remain zero. reset value. 1 After expiration the timer T1 will reload its preset value and continue counting down. . */
#define CLIF_TIMER1_CONFIG_REG_T1_RELOAD_ENABLE_POS                                                 (1UL)
#define CLIF_TIMER1_CONFIG_REG_T1_ENABLE_MASK                                                       (0x00000001UL) /**< rw - Enables the timer T1. */
#define CLIF_TIMER1_CONFIG_REG_T1_ENABLE_POS                                                        (0UL)
#define CLIF_TIMER1_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER2_CONFIG_REG                                                                      (0x40004080UL) /**< rw*/
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_T3_EXPIRED_MASK                                           (0x80000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when timer T3 raises its expiration flag. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_T3_EXPIRED_POS                                            (31UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RX_STARTED_MASK                                           (0x40000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when a data reception begins (1st bit is received). . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RX_STARTED_POS                                            (30UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_TX_STARTED_MASK                                           (0x20000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when a data transmission begins. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_TX_STARTED_POS                                            (29UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_ON_EXT_MASK                                            (0x10000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when the external RF field is detected. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_ON_EXT_POS                                             (28UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_OFF_EXT_MASK                                           (0x08000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when the external RF field vanishes. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_OFF_EXT_POS                                            (27UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_ON_INT_MASK                                            (0x04000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when the internal RF field is turned on. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_ON_INT_POS                                             (26UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_OFF_INT_MASK                                           (0x02000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when the internal RF field is turned off. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_RF_OFF_INT_POS                                            (25UL)
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_SIGIN_ACT_MASK                                            (0x01000000UL) /**< rw - T2_STOP_EVENT: If set. the timer T2 is stopped when an activity on SigIn is detected. . */
#define CLIF_TIMER2_CONFIG_REG_T2_STOP_ON_SIGIN_ACT_POS                                             (24UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_T3_RUNNING_MASK                                          (0x00040000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when timer T3 has started. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_T3_RUNNING_POS                                           (18UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RX_STARTED_MASK                                          (0x00020000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when a data reception begins (1st bit is received). . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RX_STARTED_POS                                           (17UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RX_ENDED_MASK                                            (0x00010000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when a data reception ends. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RX_ENDED_POS                                             (16UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_TX_STARTED_MASK                                          (0x00008000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when a data transmission begins. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_TX_STARTED_POS                                           (15UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_TX_ENDED_MASK                                            (0x00004000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when a data transmission ends. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_TX_ENDED_POS                                             (14UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_ON_EXT_MASK                                           (0x00002000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when the external RF field is detected. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_ON_EXT_POS                                            (13UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_OFF_EXT_MASK                                          (0x00001000UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when the external RF field is not detected any more. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_OFF_EXT_POS                                           (12UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_ON_INT_MASK                                           (0x00000800UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when an internal RF field is turned on. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_ON_INT_POS                                            (11UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_OFF_INT_MASK                                          (0x00000400UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when an internal RF field is turned off. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_RF_OFF_INT_POS                                           (10UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_SIGIN_ACT_MASK                                           (0x00000200UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started when an activity on SigIn is detected. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_ON_SIGIN_ACT_POS                                            (9UL)
#define CLIF_TIMER2_CONFIG_REG_T2_START_NOW_MASK                                                    (0x00000100UL) /**< rw - T2_START_EVENT: If set. the timer T2 is started immediatly. . */
#define CLIF_TIMER2_CONFIG_REG_T2_START_NOW_POS                                                     (8UL)
#define CLIF_TIMER2_CONFIG_REG_T2_PRESCALE_SEL_MASK                                                 (0x00000038UL) /**< rw - Controls frequency/period of the timer T2 when the prescaler is activated by T2_MODE_SEL. 000b* 6.78MHz counter 001b 3.39MHz counter 010b 1.70MHz counter 011b 848kHz counter 100b 424kHz counter 101b 212kHz counter 110b 106kHz counter 111b 53kHz counter . */
#define CLIF_TIMER2_CONFIG_REG_T2_PRESCALE_SEL_POS                                                  (3UL)
#define CLIF_TIMER2_CONFIG_REG_T2_MODE_SEL_MASK                                                     (0x00000004UL) /**< rw - If set. the timer T2 is started the prescaler for the timer T2 is enabled. 0* Prescaler is disabled: the timer frequency matches CLIF clock frequency (13.56MHz). 1 Prescaler is enabled: the timer operates on the prescaler signal frequency (chosen by T2_PRESCALE_SEL). . */
#define CLIF_TIMER2_CONFIG_REG_T2_MODE_SEL_POS                                                      (2UL)
#define CLIF_TIMER2_CONFIG_REG_T2_RELOAD_ENABLE_MASK                                                (0x00000002UL) /**< rw - If set to 0.the timer T2 will stop on expiration. 0* After expiration the timer T2 will stop counting. i.e.. remain zero. reset value. 1 After expiration the timer T2 will reload its preset value and continue counting down. . */
#define CLIF_TIMER2_CONFIG_REG_T2_RELOAD_ENABLE_POS                                                 (1UL)
#define CLIF_TIMER2_CONFIG_REG_T2_ENABLE_MASK                                                       (0x00000001UL) /**< rw - Enables the timer T2 . */
#define CLIF_TIMER2_CONFIG_REG_T2_ENABLE_POS                                                        (0UL)
#define CLIF_TIMER2_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER0_RELOAD_REG                                                                      (0x40004084UL) /**< rw*/
#define CLIF_TIMER0_RELOAD_REG_T0_RELOAD_VALUE_MASK                                                 (0x000FFFFFUL) /**< rw - Reload value of the timer T0. . */
#define CLIF_TIMER0_RELOAD_REG_T0_RELOAD_VALUE_POS                                                  (0UL)
#define CLIF_TIMER0_RELOAD_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER1_RELOAD_REG                                                                      (0x40004088UL) /**< rw*/
#define CLIF_TIMER1_RELOAD_REG_T1_RELOAD_VALUE_MASK                                                 (0x000FFFFFUL) /**< rw - Reload value of the timer T1.. */
#define CLIF_TIMER1_RELOAD_REG_T1_RELOAD_VALUE_POS                                                  (0UL)
#define CLIF_TIMER1_RELOAD_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER2_RELOAD_REG                                                                      (0x4000408CUL) /**< rw*/
#define CLIF_TIMER2_RELOAD_REG_T2_RELOAD_VALUE_MASK                                                 (0x000FFFFFUL) /**< rw - Reload value of the timer T2. . */
#define CLIF_TIMER2_RELOAD_REG_T2_RELOAD_VALUE_POS                                                  (0UL)
#define CLIF_TIMER2_RELOAD_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_TIMER0_OUTPUT_REG                                                                      (0x40004090UL) /**< r-*/
#define CLIF_TIMER0_OUTPUT_REG_T0_RUNNING_MASK                                                      (0x01000000UL) /**< r- - Indicates that timer T0 is running (busy) . */
#define CLIF_TIMER0_OUTPUT_REG_T0_RUNNING_POS                                                       (24UL)
#define CLIF_TIMER0_OUTPUT_REG_T0_VALUE_MASK                                                        (0x000FFFFFUL) /**< r- - Value of 20bit counter in timer T0 . */
#define CLIF_TIMER0_OUTPUT_REG_T0_VALUE_POS                                                         (0UL)
#define CLIF_TIMER0_OUTPUT_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_TIMER1_OUTPUT_REG                                                                      (0x40004094UL) /**< r-*/
#define CLIF_TIMER1_OUTPUT_REG_T1_RUNNING_MASK                                                      (0x01000000UL) /**< r- - Indicates that timer T1 is running (busy) . */
#define CLIF_TIMER1_OUTPUT_REG_T1_RUNNING_POS                                                       (24UL)
#define CLIF_TIMER1_OUTPUT_REG_T1_VALUE_MASK                                                        (0x000FFFFFUL) /**< r- - Value of 20bit counter in timer T1 . */
#define CLIF_TIMER1_OUTPUT_REG_T1_VALUE_POS                                                         (0UL)
#define CLIF_TIMER1_OUTPUT_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_TIMER2_OUTPUT_REG                                                                      (0x40004098UL) /**< r-*/
#define CLIF_TIMER2_OUTPUT_REG_T2_RUNNING_MASK                                                      (0x01000000UL) /**< r- - Indicates that timer T2 is running (busy) . */
#define CLIF_TIMER2_OUTPUT_REG_T2_RUNNING_POS                                                       (24UL)
#define CLIF_TIMER2_OUTPUT_REG_T2_VALUE_MASK                                                        (0x000FFFFFUL) /**< r- - Value of 20bit counter in timer T2 . */
#define CLIF_TIMER2_OUTPUT_REG_T2_VALUE_POS                                                         (0UL)
#define CLIF_TIMER2_OUTPUT_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name CLTimer register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TIMER3_CONFIG_REG                                                                      (0x4000409CUL) /**< rw*/
#define CLIF_TIMER3_CONFIG_REG_T3_RUNNING_MASK                                                      (0x80000000UL) /**< r- - Indicates that timer T3 is running (busy). */
#define CLIF_TIMER3_CONFIG_REG_T3_RUNNING_POS                                                       (31UL)
#define CLIF_TIMER3_CONFIG_REG_T3_START_VALUE_MASK                                                  (0x0FFFFF00UL) /**< rw - Start value of timer T3. */
#define CLIF_TIMER3_CONFIG_REG_T3_START_VALUE_POS                                                   (8UL)
#define CLIF_TIMER3_CONFIG_REG_T3_START_NOW_MASK                                                    (0x00000004UL) /**< rw - Start event for T3. */
#define CLIF_TIMER3_CONFIG_REG_T3_START_NOW_POS                                                     (2UL)
#define CLIF_TIMER3_CONFIG_REG_T3_CLOCK_SEL_MASK                                                    (0x00000002UL) /**< rw - Select the timer clock frequency    0 HFO clock   1 LFO clock. */
#define CLIF_TIMER3_CONFIG_REG_T3_CLOCK_SEL_POS                                                     (1UL)
#define CLIF_TIMER3_CONFIG_REG_T3_ENABLE_MASK                                                       (0x00000001UL) /**< rw - Enables timer T3. */
#define CLIF_TIMER3_CONFIG_REG_T3_ENABLE_POS                                                        (0UL)
#define CLIF_TIMER3_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name MFC-Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_MFC_CONFIG_REG                                                                         (0x400040A0UL) /**< rw*/
#define CLIF_MFC_CONFIG_REG_MFC_CRYPTO_ON_MASK                                                      (0x00000100UL) /**< rw - If set to 1 the mfc-crypto bit is generated for end-/de-cryption. */
#define CLIF_MFC_CONFIG_REG_MFC_CRYPTO_ON_POS                                                       (8UL)
#define CLIF_MFC_CONFIG_REG_MFC_XOR_TYPE_MASK                                                       (0x000000C0UL) /**< rw - Selects the input bit to the crypto register 00* crypto output bit (for mode ENCRYPT/DECRYPT) 01 crypto output bit XOR Rx-Decoder/Tx-Encoder bit (for mode SHIFT_XOR_32) 10 reserved 11 crypto output bit XOR Rx-Decoder/Tx-Encoder bit XOR MFC_DATA (current bit) (for mode SHIFT_XOR) . */
#define CLIF_MFC_CONFIG_REG_MFC_XOR_TYPE_POS                                                        (6UL)
#define CLIF_MFC_CONFIG_REG_MFC_LOAD_TYPE_MASK                                                      (0x00000030UL) /**< rw - Selects the input bit to the crypto register. 00* Resereved (MFC_LOAD_EXEC will be ignored) 01 Loads the upper 16bit of CLIF_MFC_INPUT_REG .MFC_DATA[31:0] to the PRNG register 10 Loads the 32bits of CLIF_MFC_INPUT_REG .MFC_DATA[31:0] to the lower part of the 48bit MFC register (31:0). 11 Loads the lower 16bits from CLIF_MFC_INPUT_REG .MFC_DATA[15:0] to the upper part of the 48bit MFC register (47:32). . */
#define CLIF_MFC_CONFIG_REG_MFC_LOAD_TYPE_POS                                                       (4UL)
#define CLIF_MFC_CONFIG_REG_MFC_LOAD_EXEC_MASK                                                      (0x00000008UL) /**< rw - If set. the load from the CLIF_MFC_INPUT_REG .MFC_DATA to the target defined in MFC_LOAD_TYPE is performed. . */
#define CLIF_MFC_CONFIG_REG_MFC_LOAD_EXEC_POS                                                       (3UL)
#define CLIF_MFC_CONFIG_REG_MFC_CRYPTO_TYPE_MASK                                                    (0x00000004UL) /**< rw - Chooses the MiFare crypto type. 0 MiFare crypto type A 1 MiFare crypto type B . */
#define CLIF_MFC_CONFIG_REG_MFC_CRYPTO_TYPE_POS                                                     (2UL)
#define CLIF_MFC_CONFIG_REG_MFC_ENABLE_MASK                                                         (0x00000002UL) /**< rw - Enables the encryption/decryption for transmission/reception. . */
#define CLIF_MFC_CONFIG_REG_MFC_ENABLE_POS                                                          (1UL)
#define CLIF_MFC_CONFIG_REG_MFC_BUSY_MASK                                                           (0x00000001UL) /**< r- - Status bit of the MFC that indicates that the MFC is busy. Changing the configuration is not allowed when the MFC is active. */
#define CLIF_MFC_CONFIG_REG_MFC_BUSY_POS                                                            (0UL)
#define CLIF_MFC_CONFIG_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name MFC-Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_MFC_PRNG_REG                                                                           (0x400040A4UL) /**< r-*/
#define CLIF_MFC_PRNG_REG_PRNG_VALUE_MASK                                                           (0x0000FFFFUL) /**< r- - Output byte of the internal PRNG register. Note that any read access will tirgger the generation of a new random number.. */
#define CLIF_MFC_PRNG_REG_PRNG_VALUE_POS                                                            (0UL)
#define CLIF_MFC_PRNG_REG__RESET_VALUE                                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name MFC-Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_MFC_INPUT_REG                                                                          (0x400040A8UL) /**< rw*/
#define CLIF_MFC_INPUT_REG_MFC_DATA_MASK                                                            (0xFFFFFFFFUL) /**< rw - 32bit data to be passed to the MFC. . */
#define CLIF_MFC_INPUT_REG_MFC_DATA_POS                                                             (0UL)
#define CLIF_MFC_INPUT_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SIGPRO_CONFIG_REG                                                                      (0x400040ACUL) /**< rw*/
#define CLIF_SIGPRO_CONFIG_REG_DIG_CLOCK_FAIL_DETECTION_ENABLE_MASK                                 (0x00100000UL) /**< rw - If set digital clock detection is used instead of analog. */
#define CLIF_SIGPRO_CONFIG_REG_DIG_CLOCK_FAIL_DETECTION_ENABLE_POS                                  (20UL)
#define CLIF_SIGPRO_CONFIG_REG_NRZ_SOURCE_EXT_MASK                                                  (0x00080000UL) /**< rw - If set the NRZ decoder uses the external modulation signal for decoding. Note: This setting (if not 0) overrides other input settings. */
#define CLIF_SIGPRO_CONFIG_REG_NRZ_SOURCE_EXT_POS                                                   (19UL)
#define CLIF_SIGPRO_CONFIG_REG_MANCHESTER_SOURCE_EXT_MASK                                           (0x00040000UL) /**< rw - If set the manchester decoder uses the external modulation signal for decoding. Note: This setting (if not 0) overrides other input settings. */
#define CLIF_SIGPRO_CONFIG_REG_MANCHESTER_SOURCE_EXT_POS                                            (18UL)
#define CLIF_SIGPRO_CONFIG_REG_MILLER_SOURCE_EXT_MASK                                               (0x00030000UL) /**< rw - Defines if miller decoder uses the external modulation signal for decoding. Note: This setting (if not 0) overrides other input settings. 0: not used 1: plain envelope from external device 2: 100% envelope generated on 13.56MHz clock domain 3: 100% envelope generated on HFO clock domain. */
#define CLIF_SIGPRO_CONFIG_REG_MILLER_SOURCE_EXT_POS                                                (16UL)
#define CLIF_SIGPRO_CONFIG_REG_SIGPRO_RM_FOR_CM_F424_ENABLE_MASK                                    (0x00000800UL) /**< rw - If set to 1 the reader mode signal processing chain is used for Felica 424 demodulation Note: FW must configure the  registers CLIF_SIGPRO_RM_CONFIG0_REG and CLIF_SIGPRO_RM_CONFIG1_REG correctly. */
#define CLIF_SIGPRO_CONFIG_REG_SIGPRO_RM_FOR_CM_F424_ENABLE_POS                                     (11UL)
#define CLIF_SIGPRO_CONFIG_REG_USE_CMPLUS_MILLER_MASK                                               (0x00000200UL) /**< rw - If set the Miller decoder uses the CM plus (Manchester demodulator) as preferred analog demodulator. Note that if USE_CMPLUS_MILLER and USE_ADCBCM_MILLER are set then the ADCBCM is used as input to the Miller decoder.. */
#define CLIF_SIGPRO_CONFIG_REG_USE_CMPLUS_MILLER_POS                                                (9UL)
#define CLIF_SIGPRO_CONFIG_REG_USE_ADCBCM_NRZ_MASK                                                  (0x00000100UL) /**< rw - If set. the ADC based Card-Mode output is used as input to the NRZ decoder. . */
#define CLIF_SIGPRO_CONFIG_REG_USE_ADCBCM_NRZ_POS                                                   (8UL)
#define CLIF_SIGPRO_CONFIG_REG_USE_ADCBCM_MANCHESTER_MASK                                           (0x00000080UL) /**< rw - If set. the ADC based Card-Mode output is used as input to the Manchester decoder. . */
#define CLIF_SIGPRO_CONFIG_REG_USE_ADCBCM_MANCHESTER_POS                                            (7UL)
#define CLIF_SIGPRO_CONFIG_REG_USE_ADCBCM_MILLER_MASK                                               (0x00000040UL) /**< rw - If set. the ADC based Card-Mode output is used as input to the Miller decoder. . */
#define CLIF_SIGPRO_CONFIG_REG_USE_ADCBCM_MILLER_POS                                                (6UL)
#define CLIF_SIGPRO_CONFIG_REG_SIGPRO_IN_SEL_MASK                                                   (0x00000030UL) /**< rw - Defines the source of SigPro input signals. Note: Analogue means for reader mode the ADC signals and for card mode the signal from the appropriate analog demodulator or the ADC based card mode. 00* TxEnvelope for loopback mode 01 Analog Modulator / ADCBCM 010 SigIn 011 Analog Modulator / ADCBCM + SigIn 100 TxEnvelope for loopback mode. */
#define CLIF_SIGPRO_CONFIG_REG_SIGPRO_IN_SEL_POS                                                    (4UL)
#define CLIF_SIGPRO_CONFIG_REG_TARGET_MASK                                                          (0x00000008UL) /**< rw - Defines that the receiver is in target mode. Note: Do not change this bit when the Rx-Decoder is enabled. . */
#define CLIF_SIGPRO_CONFIG_REG_TARGET_POS                                                           (3UL)
#define CLIF_SIGPRO_CONFIG_REG_BAUDRATE_MASK                                                        (0x00000007UL) /**< rw - Defines the baudrate of the receiving signal. The MSB is only relevent for reader mode. Note: These bits are set by the mode-detector if automatic mode detection is enabled and the communication mode is detected. 000* RFU 001 RFU 010 26kBd (only for reader mode) 011 53kBd (only for reader mode) 100 106kBd Note that this configuration is set by the Mode detector for ISO14443 type A and B. 101 212kBd Note that this configuration is set by the Mode detector for FeliCa 212kBd. 110 424kBd Note that this configuration is set by the Mode detector for FeliCa 424kBd. 111 848kBd. */
#define CLIF_SIGPRO_CONFIG_REG_BAUDRATE_POS                                                         (0UL)
#define CLIF_SIGPRO_CONFIG_REG__RESET_VALUE                                                         (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SIGPRO_RM_CONFIG0_REG                                                                  (0x400040B0UL) /**< rw*/
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_VAL_MASK                                                    (0xFFFF0000UL) /**< rw - Defines the Sync Pattern. which is expected to be sent as preamble before the actual data. . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_VAL_POS                                                     (16UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_LEN_MASK                                                    (0x0000F000UL) /**< rw - Defines how many Bits of Sync_Val are valid. Example: 0 .. 1 Bit. . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_LEN_POS                                                     (12UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_NEGEDGE_MASK                                                (0x00000800UL) /**< rw - Defines a SOF with no min or max in correlation. The bitgrid will be defined by the neg edge (EPC. UID). . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_NEGEDGE_POS                                                 (11UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_LAST_SYNC_HALF_MASK                                              (0x00000400UL) /**< rw - The last Bit of the Synccode has only half of the length compared to all other bits (EPC V2). . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_LAST_SYNC_HALF_POS                                               (10UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_TYPE_MASK                                                   (0x00000300UL) /**< rw - Set to 0 all 16 bits of SyncVal are interpreted as bits. Set to 1 a nipple of bits is interpreted as one bit in following way:{data. coll} data=zero or one. coll=1 means a collision on this bit. Note: if Coll=1 the vale of data is ignored.  Set to 2 the synchronisation is done at every startbit of each byte (TypeB). */
#define CLIF_SIGPRO_RM_CONFIG0_REG_SYNC_TYPE_POS                                                    (8UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_EGT_CHECK_MASK                                                   (0x00000040UL) /**< rw - Set to 1. enables the EGT check functionality. An invalid EGT length causes an protocol error . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_EGT_CHECK_POS                                                    (6UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_SPEED_MASK                                                  (0x00000030UL) /**< rw - Defines the number of clocks used for one correlation. 00* ISO14443 01 ICODE 53kBaud. FeliCa 424 kBaud 10 ICODE 26kBaud. FeliCa 212 kBaud 11 RFU. */
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_SPEED_POS                                                   (4UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_FREQ_MASK                                                   (0x0000000CUL) /**< rw - Sets the correlation frequency 00:212 kHz 01:424 kHz 10:848 kHz 11:848 kHz . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_FREQ_POS                                                    (2UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_LENGTH_MASK                                                 (0x00000002UL) /**< rw - Reserved . */
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_LENGTH_POS                                                  (1UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_INV_MASK                                                    (0x00000001UL) /**< rw - Defines a logical one: 0:subcarrier / no subcarrier 1:no subcarrier / subcarrier. */
#define CLIF_SIGPRO_RM_CONFIG0_REG_CORR_INV_POS                                                     (0UL)
#define CLIF_SIGPRO_RM_CONFIG0_REG__RESET_VALUE                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x000C0000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SIGPRO_RM_CONFIG1_REG                                                                  (0x400040B4UL) /**< rw*/
#define CLIF_SIGPRO_RM_CONFIG1_REG_DYNAMIC_BPSK_TH_ENABLE_MASK                                      (0x01000000UL) /**< rw - If set to 1 the threshold for BPSK demodulation is dynamically adapted while reception. Note: This setting is only valid  in BSPK modes - otherwise it is ignored. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_DYNAMIC_BPSK_TH_ENABLE_POS                                       (24UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_BPSK_IQ_MODE_MASK                                                (0x00E00000UL) /**< rw - 000*-111 Defines strategy of signal processing regarding I and Q channel  000* Both channels (I and Q) are used for signal processing   001 Use only I channel   010 Use only Q channel   011 RFU   100 Use the strongest channel   101 Use the first channel   110-111 RFU . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_BPSK_IQ_MODE_POS                                                 (21UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_BPSK_FILT6_MASK                                                  (0x00100000UL) /**< rw - This bit changes the Spike filter for the Phaseshift detection from order 5 to order 6 (default = order 6).. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_BPSK_FILT6_POS                                                   (20UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_RESYNC_EQ_ON_MASK                                                (0x00080000UL) /**< rw - Resynchronisaton during the SOF for an equal correlation value is done (default = activated).. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_RESYNC_EQ_ON_POS                                                 (19UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_CORR_RESET_ON_MASK                                               (0x00040000UL) /**< rw - The correclator is reset at a reset (default = activated).. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_CORR_RESET_ON_POS                                                (18UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_VALID_FILT_OFF_MASK                                              (0x00020000UL) /**< rw - Disables a special filter in BPSK mode. If set to 0 the correlation of 0110 is filtered with the correlation of 1110 and 0111. Otherwise the demodulation is done using the correlation with 0110. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_VALID_FILT_OFF_POS                                               (17UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_DATA_BEFORE_MIN_MASK                                             (0x00010000UL) /**< rw - Data is received even before the first minimum at the SOF (default: = deactivated).. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_DATA_BEFORE_MIN_POS                                              (16UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_MIN_LEVEL_MASK                                                   (0x0000F000UL) /**< rw - Defines the MinLevel of the reception Note: The MinLevel should be higher than the noise level in the system Note: Used for BPSK and Manchester with Subcarrier communication types as MinLevel! . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_MIN_LEVEL_POS                                                    (12UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_MIN_LEVEL_P_MASK                                                 (0x00000F00UL) /**< rw - Defines the minlevel for the phaseshift detector unit . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_MIN_LEVEL_P_POS                                                  (8UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_USE_SMALL_EVAL_MASK                                              (0x00000080UL) /**< rw - Defines the length of the eval periode for the correlator for Manchester subcarrier communication types. . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_USE_SMALL_EVAL_POS                                               (7UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_COLL_LEVEL_MASK                                                  (0x00000060UL) /**< rw - Defines how strong a signal must be to be interpreded as a collision for Manchester subcarrier communication types. 00* >12.5% 01 >25% 10 >50% No Collison . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_COLL_LEVEL_POS                                                   (5UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_PRE_FILTER_MASK                                                  (0x00000010UL) /**< rw - If set to 1 four samples are combined to one data. (average) . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_PRE_FILTER_POS                                                   (4UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_RECT_FILTER_MASK                                                 (0x00000008UL) /**< rw - If set to one. the ADC-values are changed to a more rectangular waveshape. . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_RECT_FILTER_POS                                                  (3UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_SYNC_HIGH_MASK                                                   (0x00000004UL) /**< rw - Defines if the bitgrid is fixed at maximum (1) or at a minimum(0) value of the correlation. . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_SYNC_HIGH_POS                                                    (2UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_FSK_MASK                                                         (0x00000002UL) /**< rw - If set to 1. the demodulation scheme is FSK. . */
#define CLIF_SIGPRO_RM_CONFIG1_REG_FSK_POS                                                          (1UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG_BPSK_MASK                                                        (0x00000001UL) /**< rw - If set to 1 the demodulation scheme is BPSK.. */
#define CLIF_SIGPRO_RM_CONFIG1_REG_BPSK_POS                                                         (0UL)
#define CLIF_SIGPRO_RM_CONFIG1_REG__RESET_VALUE                                                     (0x000C0000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x00000004\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SIGPRO_CM_CONFIG_REG                                                                   (0x400040B8UL) /**< rw*/
#define CLIF_SIGPRO_CM_CONFIG_REG_MANCHESTER_BITDETECT_ENABLE_MASK                                  (0x00004000UL) /**< rw - Enables the new method to for baudrate detection.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_MANCHESTER_BITDETECT_ENABLE_POS                                   (14UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_MANCHESTER_LOCK_ENABLE_MASK                                       (0x00002000UL) /**< rw - Enables the new method to freeze the subcarrier signal after the preamble.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_MANCHESTER_LOCK_ENABLE_POS                                        (13UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_USE_RFCOUNTER_MASK                                     (0x00001000UL) /**< rw - If set the Adv. Miller decoder uses the RF counter directly for decoding. This feature is intended for 848kBd to avoid additional errors due to synchronization.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_USE_RFCOUNTER_POS                                      (12UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_FILTER_ENABLE_MASK                                     (0x00000800UL) /**< rw - If set the Miller input signal is filtered with a 3-out-of-5 median filter (on HFO clock) before decoding. Note: This feature is not available in combination with ADV_MILLER_USE_RFCOUNTER.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_FILTER_ENABLE_POS                                      (11UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_NRZ_DEC_ENABLE_MASK                                               (0x00000400UL) /**< rw - If set the NRZ decoder is activated for ISO14443 type B PICC reception. Note that the NRZ decoder must be activated in general target mode to be able to detect ISO14443 type B.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_NRZ_DEC_ENABLE_POS                                                (10UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_MANCHESTER_DEC_ENABLE_MASK                                        (0x00000200UL) /**< rw - If set the Manchester decoder is activated for FeliCa PICC reception. Note that the Manchester decoder must be activated in general target mode to be able to detect FeliCa.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_MANCHESTER_DEC_ENABLE_POS                                         (9UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_MILLER_DEC_ENABLE_MASK                                            (0x00000100UL) /**< rw - If set the Miller decoder is activated for ISO14443 type A PICC reception. Note that the Miller decoder must be activated in general target mode to be able to detect ISO14443 type A.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_MILLER_DEC_ENABLE_POS                                             (8UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_CALIB_OFF_MASK                                         (0x00000080UL) /**< rw - If set the calibration of the advanced Miller decoder is deactivated and the nominal threshold values are used.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_CALIB_OFF_POS                                          (7UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_EDGE_SEL_MASK                                          (0x00000040UL) /**< rw - Select the edge for decoding Note: This feature is not available in combination with ADV_MILLER_USE_RFCOUNTER. 0* decoder counts from falling to falling edge: 1 decoder counts from rising to rising edge. . */
#define CLIF_SIGPRO_CM_CONFIG_REG_ADV_MILLER_EDGE_SEL_POS                                           (6UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_CM_EGT_CHECK_MASK                                                 (0x00000020UL) /**< rw - If set the EGT check for ISO14443 type B card-mode is activated. An invalid EGT length causes an protocol error.. */
#define CLIF_SIGPRO_CM_CONFIG_REG_CM_EGT_CHECK_POS                                                  (5UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_TAU_MASK                                                          (0x00000018UL) /**< rw - Defines the timing constant of the internal PLL as a negative power of two. . */
#define CLIF_SIGPRO_CM_CONFIG_REG_TAU_POS                                                           (3UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_MODE_DETECTOR_OFF_MASK                                            (0x00000004UL) /**< rw - If set to 1. the mode detector is deactivated . */
#define CLIF_SIGPRO_CM_CONFIG_REG_MODE_DETECTOR_OFF_POS                                             (2UL)
#define CLIF_SIGPRO_CM_CONFIG_REG_RX_FRAMING_MASK                                                   (0x00000003UL) /**< rw - Defines the framing in card mode. Note that these bits are set by the mod-detector if automatic mode detection is enabled and the communication mode is detected. 00* ISO14443A / Mifare 01 ISO18092 (NFC - with Syncbyte 0xF0) 10 FeliCa 11 ISO14443B. */
#define CLIF_SIGPRO_CM_CONFIG_REG_RX_FRAMING_POS                                                    (0UL)
#define CLIF_SIGPRO_CM_CONFIG_REG__RESET_VALUE                                                      (0x00000004UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x01000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG                                                               (0x400040BCUL) /**< rw*/
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_SYNC_MODE_SEL_MASK                                            (0xC0000000UL) /**< rw - Chooses the threshold for interpolation of the zero-crossing detection. 0h 0.25. 1h 0.5. 2h-3h 0.75. */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_SYNC_MODE_SEL_POS                                             (30UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_EPSILON_TH_MASK                                               (0x3F000000UL) /**< rw - Threshold for the epsilon environment in the zero-crossing detection block. Note that with value 1h the block is disabled and the value 0h is forbidden. . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_EPSILON_TH_POS                                                (24UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_WATCH_DOG_VAL_MASK                                            (0x00FF0000UL) /**< rw - Reset value for the Watch-Dog Counter.After this number of 848kHz clock cycles where no edges was detected the Watch Dog counter resets the edge-locking the output envelope and the dynamic threshold.. */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_WATCH_DOG_VAL_POS                                             (16UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_BIT_DETECT_DYN_MODE_MASK                                      (0x0000C000UL) /**< rw - Defines the dynamic bit detector adaptation. For dynamic threshold adaptation the minimum threshold is defined in CLIF_SIGPRO_ADCBCM_THRESHOLD_REG   00 Static bit detection   01 Dynamic edge detection 25% from last decision   10 Dynamic edge detection 50% from last decision   11 Dynamic edge detection 75% from last decision. */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_BIT_DETECT_DYN_MODE_POS                                       (14UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_EDGE_DETECT_DYN_MODE_MASK                                     (0x00003000UL) /**< rw - Defines the dynamic edge detector adaptation. For dynamic threshold adaptation the minimum threshold is defined in CLIF_SIGPRO_ADCBCM_THRESHOLD_REG   00 Static edge detection   01 Dynamic edge detection 25% from last decision   10 Dynamic edge detection 50% from last decision   11 Dynamic edge detection 75% from last decision. */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_EDGE_DETECT_DYN_MODE_POS                                      (12UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_WATCH_DOG_MODE_MASK                                           (0x00000C00UL) /**< rw - Selects mode of the watch-dog timer.  00 Watch-dog timer disabled   01 Watch dog timer with edge-detection as input (as for PN547/548)  10 Watch dog timer with valid-signal as input  11 Watch dog timer with valid-signal as input. */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_WATCH_DOG_MODE_POS                                            (10UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_SYNC_MODE_ENABLE_MASK                                         (0x00000200UL) /**< rw - If set. the sync mode is activated. The sync-mode is used to reduce the jitter in the zero-crossing detection (usage depends on the analog HW configuration). . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_SYNC_MODE_ENABLE_POS                                          (9UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_EDGE_DETECT_TAP_SEL_MASK                                      (0x000001E0UL) /**< rw - Selects the number of taps of the edge-detector filter.  0000* Edge detector filter with 2 taps  0001 Edge detector filter with 4 taps  0010 Edge detector filter with 6 taps  0011 Edge detector filter with 8 taps  0100 Edge detector filter with 10 taps  0101 Edge detector filter with 12 taps  0110 Edge detector filter with 14 taps  0111 Edge detector filter with 16 taps  1000 Edge detector filter with 18 taps  1001 Edge detector filter with 20 taps  1010 Edge detector filter with 22 taps  1011 Edge detector filter with 24 taps  1100 Edge detector filter with 26 taps  1101 Edge detector filter with 28 taps  1110 Edge detector filter with 30 taps  1111 Edge detector filter with 32 taps. */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_EDGE_DETECT_TAP_SEL_POS                                       (5UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_848_MASK                                        (0x00000010UL) /**< rw - Enables the 848kBd envelope detection of the ADCBCM. . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_848_POS                                         (4UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_424_MASK                                        (0x00000008UL) /**< rw - Enables the 424kBd envelope detection of the ADCBCM. At start up for mode-detection this mode has to be enabled. . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_424_POS                                         (3UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_212_MASK                                        (0x00000004UL) /**< rw - Enables the 212kBd envelope detection of the ADCBCM. At start up for mode-detection this mode has to be enabled. . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_212_POS                                         (2UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_106_MASK                                        (0x00000002UL) /**< rw - Enables the 106kBd envelope detection of the ADCBCM. At start up for mode-detection this mode has to be enabled. . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_106_POS                                         (1UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_MASK                                            (0x00000001UL) /**< rw - Enables the ADCBCM. Note that at least one mode (106. 212. 424. or 848kBd) has to enabled additionally. . */
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG_ADCBCM_ENABLE_POS                                             (0UL)
#define CLIF_SIGPRO_ADCBCM_CONFIG_REG__RESET_VALUE                                                  (0x01000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SIGPRO_ADCBCM_THRESHOLD_REG                                                            (0x400040C0UL) /**< rw*/
#define CLIF_SIGPRO_ADCBCM_THRESHOLD_REG_EDGE_DETECT_TH_MASK                                        (0x1FFF0000UL) /**< rw - Threshold for the edge decision block of the ADCBCM. . */
#define CLIF_SIGPRO_ADCBCM_THRESHOLD_REG_EDGE_DETECT_TH_POS                                         (16UL)
#define CLIF_SIGPRO_ADCBCM_THRESHOLD_REG_BIT_DETECT_TH_MASK                                         (0x00001FFFUL) /**< rw - Threshold for the bit decision block of the ADCBCM.. */
#define CLIF_SIGPRO_ADCBCM_THRESHOLD_REG_BIT_DETECT_TH_POS                                          (0UL)
#define CLIF_SIGPRO_ADCBCM_THRESHOLD_REG__RESET_VALUE                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_SIGPRO_ADCBCM_BITENERGY0_REG                                                           (0x400040C4UL) /**< r-*/
#define CLIF_SIGPRO_ADCBCM_BITENERGY0_REG_BIT_ENERGY_212_MASK                                       (0x1FFE0000UL) /**< r- - Value of the bit-energy block for 212Bd Note that the actual bit-energy is twice the register value. Therefore the bits CLIF_SIGPRO_ADCBCM_BITENERGY0_REG[28:16] may be accessed (left shift).. */
#define CLIF_SIGPRO_ADCBCM_BITENERGY0_REG_BIT_ENERGY_212_POS                                        (17UL)
#define CLIF_SIGPRO_ADCBCM_BITENERGY0_REG_BIT_ENERGY_106_MASK                                       (0x00001FFFUL) /**< r- - Value of the bit-energy block for 106kBd. */
#define CLIF_SIGPRO_ADCBCM_BITENERGY0_REG_BIT_ENERGY_106_POS                                        (0UL)
#define CLIF_SIGPRO_ADCBCM_BITENERGY0_REG__RESET_VALUE                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name SigPro register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_SIGPRO_ADCBCM_BITENERGY1_REG                                                           (0x400040C8UL) /**< r-*/
#define CLIF_SIGPRO_ADCBCM_BITENERGY1_REG_BIT_ENERGY_848_MASK                                       (0x1FF80000UL) /**< r- - Value of the bit-energy block for 848Bd Note that the actual bit-energy is eight times the register value. Therefore the bits CLIF_SIGPRO_ADCBCM_BITENERGY0_REG[28:16] may be accessed (3x left shift).. */
#define CLIF_SIGPRO_ADCBCM_BITENERGY1_REG_BIT_ENERGY_848_POS                                        (19UL)
#define CLIF_SIGPRO_ADCBCM_BITENERGY1_REG_BIT_ENERGY_424_MASK                                       (0x00001FFCUL) /**< r- - Value of the bit-energy block for 424kBd Note that the actual bit-energy is four times the register value. Therefore the bits CLIF_SIGPRO_ADCBCM_BITENERGY0_REG[12:0] may be accessed (2x left-shift).. */
#define CLIF_SIGPRO_ADCBCM_BITENERGY1_REG_BIT_ENERGY_424_POS                                        (2UL)
#define CLIF_SIGPRO_ADCBCM_BITENERGY1_REG__RESET_VALUE                                              (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name AGC register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_AGC_CONFIG0_REG                                                                        (0x400040CCUL) /**< rw*/
#define CLIF_AGC_CONFIG0_REG_AGC_TESTMODE_ENABLE_MASK                                               (0x80000000UL) /**< rw - If this bit is set then the AGC value is increased/decreased by signals form the testbus. Note that this bit is shall never be set by FW and is intended only for test-purposes.. */
#define CLIF_AGC_CONFIG0_REG_AGC_TESTMODE_ENABLE_POS                                                (31UL)
#define CLIF_AGC_CONFIG0_REG_MILLER_BG_FREEZE_VALUE_MASK                                            (0x7F000000UL) /**< rw - This bit is primarily intended to achieve AGC operation in RM for ISO14443 type A 106kBd (AGC_FREEZE_ON_MILLER_BG). However this field is also re-used for the back-up modes AGC_FREEZE_ON_RX_WAIT and AGC_FAST_MODE_RX_WAIT.. */
#define CLIF_AGC_CONFIG0_REG_MILLER_BG_FREEZE_VALUE_POS                                             (24UL)
#define CLIF_AGC_CONFIG0_REG_AGC_THRESHOLD_MASK                                                     (0x00FF8000UL) /**< rw - Defines the threshold value for the AGC. Value must not be larger than AGC_TIME_CONSTANT>>1 - 1.. */
#define CLIF_AGC_CONFIG0_REG_AGC_THRESHOLD_POS                                                      (15UL)
#define CLIF_AGC_CONFIG0_REG_AGC_TIME_CONSTANT_MASK                                                 (0x00007FE0UL) /**< rw - Time constant for the AGC update. An AGC period is given by (AGC_TIME_CONSTANT+1 ) * 13.56MHz. */
#define CLIF_AGC_CONFIG0_REG_AGC_TIME_CONSTANT_POS                                                  (5UL)
#define CLIF_AGC_CONFIG0_REG_AGC_SYNC_COMP_DISABLE_MASK                                             (0x00000010UL) /**< rw - If set the synchronization register at the input of the digital controller to synchronize agc_comp_ai is by-passed.. */
#define CLIF_AGC_CONFIG0_REG_AGC_SYNC_COMP_DISABLE_POS                                              (4UL)
#define CLIF_AGC_CONFIG0_REG_AGC_INPUT_SEL_MASK                                                     (0x00000008UL) /**< rw - Selects the AGC value to be loaded into the AGC and the scource for manual mode: 0* CLIF_AGC_INPUT_REG.AGC_CM_VALUE 1 CLIF_AGC_INPUT_REG.AGC_RM_VALUE. */
#define CLIF_AGC_CONFIG0_REG_AGC_INPUT_SEL_POS                                                      (3UL)
#define CLIF_AGC_CONFIG0_REG_AGC_LOAD_MASK                                                          (0x00000004UL) /**< -x - If set. one AGC control value is loaded from CLIF_AGC_VALUE_REG .into the internal AGC register (depending on AGC_INPUT_SEL). . */
#define CLIF_AGC_CONFIG0_REG_AGC_LOAD_POS                                                           (2UL)
#define CLIF_AGC_CONFIG0_REG_AGC_MODE_SEL_MASK                                                      (0x00000002UL) /**< rw - Selects the operation mode of the AGC: 0* Rx-Divider is controlled by the register CLIF_AGC_INPUT_REG.AGC_CM_VALUE or CLIF_AGC_INPUT_REG.AGC_RM_VALUE (dependent on AGC_INPUT_SEL) 1 Rx-Divider value is controlled by the AGC.. */
#define CLIF_AGC_CONFIG0_REG_AGC_MODE_SEL_POS                                                       (1UL)
#define CLIF_AGC_CONFIG0_REG_AGC_MODE_ENABLE_MASK                                                   (0x00000001UL) /**< rw - If set the AGC is enabled. If not set the Rx-Divider is controlled by either the internal AGC register or a register value (dependent on AGC_MODE_SEL).. */
#define CLIF_AGC_CONFIG0_REG_AGC_MODE_ENABLE_POS                                                    (0UL)
#define CLIF_AGC_CONFIG0_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name AGC register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_AGC_CONFIG1_REG                                                                        (0x400040D0UL) /**< rw*/
#define CLIF_AGC_CONFIG1_REG_AGC_RF_DETECT_SEL_MASK                                                 (0x60000000UL) /**< rw - Selects/enables the RF-off detection during transmission (intended for SL-ALM). If enabled the AGC reference voltage is set to its minimum (0x00). */
#define CLIF_AGC_CONFIG1_REG_AGC_RF_DETECT_SEL_POS                                                  (29UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_TX_SWITCH_VALUE_MASK                                        (0x10000000UL) /**< rw - If set the internal AGC is frozen during transmission and the output value is switched with the tx-envelope between the CM value (env=0) (CLIF_AGC_INPUT_REG.AGC_CM_VALUE) and the RM value (env=1) (CLIF_AGC_INPUT_REG.AGC_RM_VALUE).. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_TX_SWITCH_VALUE_POS                                         (28UL)
#define CLIF_AGC_CONFIG1_REG_AGC_MAX_TXRFOFF_ENABLE_MASK                                            (0x08000000UL) /**< rw - If set the AGC is forced to maximum value for a given time period after the devices RF-Field was switched off.. */
#define CLIF_AGC_CONFIG1_REG_AGC_MAX_TXRFOFF_ENABLE_POS                                             (27UL)
#define CLIF_AGC_CONFIG1_REG_AGC_MASK_DURATION_MASK                                                 (0x07E00000UL) /**< rw - Number of RF cycles the AGC active signal is set after each AGC update. In RM the signal masks out the preamble detection in the SigPro RM. In CM the signal masks out the data signals in the SigPro during Fast-Mode. In case AGC_MASK_DURATION is set to zero the agc_active_o signal is not set.. */
#define CLIF_AGC_CONFIG1_REG_AGC_MASK_DURATION_POS                                                  (21UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_RESET_ON_RF_OFF_MASK                                        (0x00100000UL) /**< rw - If set the AGC is reset to zero when an external field vanishes and frozen (as long as no external field is present).. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_RESET_ON_RF_OFF_POS                                         (20UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_MILLER_BG_MASK                                           (0x00080000UL) /**< rw - If set the AGC is frozen when the bit-grid value is below the value in MILLER_BG_FREEZE_VALUE.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_MILLER_BG_POS                                            (19UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_SOF_DETECTED_MASK                                        (0x00040000UL) /**< rw - If rm_startbit_detected_i is set (SOF detected by the SigPro) the AGC is frozen.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_SOF_DETECTED_POS                                         (18UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_DPRESENT_MASK                                            (0x00020000UL) /**< rw - If rm_dpresent_i is set (preamble detected by the SigPro) the AGC is frozen.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_DPRESENT_POS                                             (17UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_DPRESENT_NEXT_MASK                                       (0x00010000UL) /**< rw - If rm_dpresent_next_i is set (preamble detected by the SigPro) the AGC is frozen.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_DPRESENT_NEXT_POS                                        (16UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_RXWAIT_OFF_MASK                                          (0x00008000UL) /**< rw - If rx_wait_i is not set the AGC is frozen.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FREEZE_ON_RXWAIT_OFF_POS                                           (15UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_DURATION_MASK                                            (0x00007FE0UL) /**< rw - Duration of the Fast-Mode in multiples of RF clock cycles (clk13).. */
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_DURATION_POS                                             (5UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_RFON_EXT_MASK                                            (0x00000010UL) /**< rw - If set the AGC enters fast mode (HW sets AGC_FAST_MODE_ENABLE) when an external RF field is detected.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_RFON_EXT_POS                                             (4UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_TX_OVER_MASK                                             (0x00000008UL) /**< rw - If set to one the AGC fast-mode (i.e. time-constant 1 and step size given by AGC_FAST_MODE_STEP) is enabled for AGC_FAST_MODE_DURATION clk13 cycles after transmission. */
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_TX_OVER_POS                                              (3UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_STEP_MASK                                                (0x00000006UL) /**< rw - Step size of the AGC in fast-mode: 0* Step by one 1 Step by two 2 Step by three 3 Step by four. */
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_STEP_POS                                                 (1UL)
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_ENABLE_MASK                                              (0x00000001UL) /**< rw - If set to one the AGC fast-mode (i.e. time-constant 1 and step size given by AGC_FAST_MODE_STEP) is enabled for AGC_FAST_MODE_DURATION clk13 cycles.. */
#define CLIF_AGC_CONFIG1_REG_AGC_FAST_MODE_ENABLE_POS                                               (0UL)
#define CLIF_AGC_CONFIG1_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name AGC register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_AGC_INPUT_REG                                                                          (0x400040D4UL) /**< rw*/
#define CLIF_AGC_INPUT_REG_AGC_RM_VALUE_MASK                                                        (0x03FF0000UL) /**< rw - Static AGC value used for reader mode . */
#define CLIF_AGC_INPUT_REG_AGC_RM_VALUE_POS                                                         (16UL)
#define CLIF_AGC_INPUT_REG_AGC_CM_VALUE_MASK                                                        (0x000003FFUL) /**< rw - Static AGC value used for card mode. */
#define CLIF_AGC_INPUT_REG_AGC_CM_VALUE_POS                                                         (0UL)
#define CLIF_AGC_INPUT_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name AGC register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_RSSI_REG                                                                               (0x400040D8UL) /**< r-*/
#define CLIF_RSSI_REG_AGC_VALUE_MASK                                                                (0x0000FFC0UL) /**< r- - RSSI: Current value of the AGC. */
#define CLIF_RSSI_REG_AGC_VALUE_POS                                                                 (6UL)
#define CLIF_RSSI_REG_ADC_DATA_Q_MASK                                                               (0x0000003FUL) /**< r- - RSSI: Q-channel ADC value (for RSSI DC coupling shall be used). */
#define CLIF_RSSI_REG_ADC_DATA_Q_POS                                                                (0UL)
#define CLIF_RSSI_REG__RESET_VALUE                                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TX Control Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TX_CONTROL_REG                                                                         (0x400040DCUL) /**< rw*/
#define CLIF_TX_CONTROL_REG_TX_BOOSTER_CLOCK_SELECT_MASK                                            (0x00020000UL) /**< rw - Selects the output clock for the booster. If set PLL clock is selected otherwise standard clk13 is used. */
#define CLIF_TX_CONTROL_REG_TX_BOOSTER_CLOCK_SELECT_POS                                             (17UL)
#define CLIF_TX_CONTROL_REG_TX_BOOSTER_CLOCK_ENABLE_MASK                                            (0x00010000UL) /**< rw - If set a 13.56MHz clock signal is propagated to the pad RF_CLK_OUT. */
#define CLIF_TX_CONTROL_REG_TX_BOOSTER_CLOCK_ENABLE_POS                                             (16UL)
#define CLIF_TX_CONTROL_REG_TX_ALM_LD_MASK_VALUE_MASK                                               (0x00006000UL) /**< rw - Defines to period of masking after the end of transmission (to allow the level detection to settle). The period is defined in HFO/2 cycles (20MHz nominal) - 200 cycles (10us nominal) - 1 320 cycles (16us nominal) - 2 440 cycles (22us nominal) - 3 560 cycles (28us nominal) . */
#define CLIF_TX_CONTROL_REG_TX_ALM_LD_MASK_VALUE_POS                                                (13UL)
#define CLIF_TX_CONTROL_REG_TX_ALM_LD_MASK_ENABLE_MASK                                              (0x00001000UL) /**< rw - If set rf-off detection is masked out while an ongoing transmission (i.e. rf-detection is forced to field present). */
#define CLIF_TX_CONTROL_REG_TX_ALM_LD_MASK_ENABLE_POS                                               (12UL)
#define CLIF_TX_CONTROL_REG_TX_ENV_SYNC_ENABLE_MASK                                                 (0x00000400UL) /**< rw - Is set to 1 synchronization of the signal tx_envelope to 27.12MHz clock is enabled (moved from Spare-Reg in PN548-B0) . */
#define CLIF_TX_CONTROL_REG_TX_ENV_SYNC_ENABLE_POS                                                  (10UL)
#define CLIF_TX_CONTROL_REG_TX_ACTIVE_RF_SYNC_ENABLE_MASK                                           (0x00000200UL) /**< rw - Is set to 1 synchronization of the signal tx_active_rf to 27.12MHz clock is enabled (moved from Spare-Reg in PN548-B0). */
#define CLIF_TX_CONTROL_REG_TX_ACTIVE_RF_SYNC_ENABLE_POS                                            (9UL)
#define CLIF_TX_CONTROL_REG_TX_CM_GSN_MOD_ON_TXACTIVE_MASK                                          (0x00000100UL) /**< rw - If set to 1 CM GSN value is switched with tx_active instead of envelope. */
#define CLIF_TX_CONTROL_REG_TX_CM_GSN_MOD_ON_TXACTIVE_POS                                           (8UL)
#define CLIF_TX_CONTROL_REG_TX_ALM_BPSK_ENABLE_MASK                                                 (0x00000040UL) /**< rw - If set to 1 modulation for ALM is done with BPSK scheme on carrier.. */
#define CLIF_TX_CONTROL_REG_TX_ALM_BPSK_ENABLE_POS                                                  (6UL)
#define CLIF_TX_CONTROL_REG_TX_ALM_TYPE_SELECT_MASK                                                 (0x00000020UL) /**< rw - Defines which ALM type is used. If set to 1 Dual-Loop-ALM is used default (0) is Single-Loop-ALM.. */
#define CLIF_TX_CONTROL_REG_TX_ALM_TYPE_SELECT_POS                                                  (5UL)
#define CLIF_TX_CONTROL_REG_TX_ALM_DRIVER_SELECT_MASK                                               (0x00000010UL) /**< rw - Selects the driver to be used for ALM. Note: This bit is only evaluated when both TX_ALM_ENABLE and TX_ALM_TYPE_SELECT are 1. */
#define CLIF_TX_CONTROL_REG_TX_ALM_DRIVER_SELECT_POS                                                (4UL)
#define CLIF_TX_CONTROL_REG_TX_ALM_ENABLE_MASK                                                      (0x00000008UL) /**< rw - If set to 1 ALM is used for transmission in card mode. */
#define CLIF_TX_CONTROL_REG_TX_ALM_ENABLE_POS                                                       (3UL)
#define CLIF_TX_CONTROL_REG_TX_INVERT_MASK                                                          (0x00000004UL) /**< rw - If this bit is set to 1 the resulting signal is inverted. */
#define CLIF_TX_CONTROL_REG_TX_INVERT_POS                                                           (2UL)
#define CLIF_TX_CONTROL_REG_TX_SOURCE_SELECT_MASK                                                   (0x00000003UL) /**< rw - Select input for the transnmitter driver 0 TxEnvelope 1 SigIn 2 TxEnvelope + SigIn 3 IDLE. */
#define CLIF_TX_CONTROL_REG_TX_SOURCE_SELECT_POS                                                    (0UL)
#define CLIF_TX_CONTROL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TBControl
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TBCONTROL_REG                                                                          (0x400040E0UL) /**< rw*/
#define CLIF_TBCONTROL_REG_CLIF_TB_CLOCK_SELECT_MASK                                                (0x30000000UL) /**< rw - Defines to clock to be connected to a testbus. 0 -> disables clock connection. 0 -> No clock selected. 1 -> clk13. 2 -> clk_clif_hfo. 3 -> clk_smu_system. */
#define CLIF_TBCONTROL_REG_CLIF_TB_CLOCK_SELECT_POS                                                 (28UL)
#define CLIF_TBCONTROL_REG_CLIF_TB_CLOCK_BIT_SELECT_MASK                                            (0x0F000000UL) /**< rw - Defines the bit to which the selected clock is connected. 0 - 7 for TB0 8 - F for TB1. */
#define CLIF_TBCONTROL_REG_CLIF_TB_CLOCK_BIT_SELECT_POS                                             (24UL)
#define CLIF_TBCONTROL_REG_CLIF_TB1_TO_TB0_MASK                                                     (0x00100000UL) /**< rw - Connects the selected TB1 to the TB0 signals. */
#define CLIF_TBCONTROL_REG_CLIF_TB1_TO_TB0_POS                                                      (20UL)
#define CLIF_TBCONTROL_REG_CLIF_TB_BIT_SELECT_MASK                                                  (0x000F0000UL) /**< rw - Selects one bit from TB0 or TB1 to be switched to SigOut pin. */
#define CLIF_TBCONTROL_REG_CLIF_TB_BIT_SELECT_POS                                                   (16UL)
#define CLIF_TBCONTROL_REG_CLIF_TB1_SELECT_MASK                                                     (0x0000FF00UL) /**< rw - Selects the CLIF TB to be connected to TB1. */
#define CLIF_TBCONTROL_REG_CLIF_TB1_SELECT_POS                                                      (8UL)
#define CLIF_TBCONTROL_REG_CLIF_TB0_SELECT_MASK                                                     (0x000000FFUL) /**< rw - Selects the CLIF TB to be connected to TB0. */
#define CLIF_TBCONTROL_REG_CLIF_TB0_SELECT_POS                                                      (0UL)
#define CLIF_TBCONTROL_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TBControl
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_DACCONTROL_REG                                                                         (0x400040E4UL) /**< rw*/
#define CLIF_DACCONTROL_REG_CLIF_DAC_VALUE_MASK                                                     (0x003F0000UL) /**< rw - Defines an input value for the DAC0 & DAC1. */
#define CLIF_DACCONTROL_REG_CLIF_DAC_VALUE_POS                                                      (16UL)
#define CLIF_DACCONTROL_REG_CLIF_DAC1_SOURCE_SELECT_MASK                                            (0x00003F00UL) /**< rw - Selects the source for the DAC1. */
#define CLIF_DACCONTROL_REG_CLIF_DAC1_SOURCE_SELECT_POS                                             (8UL)
#define CLIF_DACCONTROL_REG_CLIF_DAC0_SOURCE_SELECT_MASK                                            (0x0000003FUL) /**< rw - Selects the source for the DAC0. */
#define CLIF_DACCONTROL_REG_CLIF_DAC0_SOURCE_SELECT_POS                                             (0UL)
#define CLIF_DACCONTROL_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TBControl
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TBVALUE_REG                                                                            (0x400040E8UL) /**< rw*/
#define CLIF_TBVALUE_REG_CLIF_TB1_VALUE_MASK                                                        (0x00FF0000UL) /**< rw - Defines an output value for TB1 - source must be selected via CLIF_TB1_SELECT.. */
#define CLIF_TBVALUE_REG_CLIF_TB1_VALUE_POS                                                         (16UL)
#define CLIF_TBVALUE_REG_TB1_VALUE_MASK                                                             (0x0000FF00UL) /**< r- - Actual value of testbus1. */
#define CLIF_TBVALUE_REG_TB1_VALUE_POS                                                              (8UL)
#define CLIF_TBVALUE_REG_TB0_VALUE_MASK                                                             (0x000000FFUL) /**< r- - Actual value of testbus0. */
#define CLIF_TBVALUE_REG_TB0_VALUE_POS                                                              (0UL)
#define CLIF_TBVALUE_REG__RESET_VALUE                                                               (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name BufferManager Rx data register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_RX_DATA_BUFFER_REG                                                                     (0x400040ECUL) /**< r-*/
#define CLIF_RX_DATA_BUFFER_REG_RX_DATA_BUFFER_MASK                                                 (0xFFFFFFFFUL) /**< r- - Data buffer for APB based BufferManager data transfer.. */
#define CLIF_RX_DATA_BUFFER_REG_RX_DATA_BUFFER_POS                                                  (0UL)
#define CLIF_RX_DATA_BUFFER_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Test option register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_TEST_CONTROL_REG                                                                       (0x400040FCUL) /**< rw*/
#define CLIF_TEST_CONTROL_REG_CLIF_SOFT_RESET_MASK                                                  (0x80000000UL) /**< rw - Applies a reset to the complete CLIF digital (except the registers). This is only a test feature and must not be used in normal operation! . */
#define CLIF_TEST_CONTROL_REG_CLIF_SOFT_RESET_POS                                                   (31UL)
#define CLIF_TEST_CONTROL_REG_CLIF_TESTBIT_1_MASK                                                   (0x40000000UL) /**< rw - Reserved testbit. */
#define CLIF_TEST_CONTROL_REG_CLIF_TESTBIT_1_POS                                                    (30UL)
#define CLIF_TEST_CONTROL_REG_AGC_HP_AUTO_ENABLE_MASK                                               (0x20000000UL) /**< rw - AGC Testoption: If set the AGC automatically switches the analog high-pass filter. To be precise when the AGC is active the high-pass filter with the shortest impulse response is selected (CLIF_ANA_RX_REG. RX_HPCF = 11).. */
#define CLIF_TEST_CONTROL_REG_AGC_HP_AUTO_ENABLE_POS                                                (29UL)
#define CLIF_TEST_CONTROL_REG_NRZ_INPUT_FILTER_BYPASS_MASK                                          (0x04000000UL) /**< rw - If set to 1 the NRZ input filter is switched off (NFCC7275). */
#define CLIF_TEST_CONTROL_REG_NRZ_INPUT_FILTER_BYPASS_POS                                           (26UL)
#define CLIF_TEST_CONTROL_REG_RXDEC_NFCC5699_BYPASS_MASK                                            (0x02000000UL) /**< rw - If set to 1 NFCC5699 fix is switched off. */
#define CLIF_TEST_CONTROL_REG_RXDEC_NFCC5699_BYPASS_POS                                             (25UL)
#define CLIF_TEST_CONTROL_REG_TX_BOOSTER_DRIVER_ON_MASK                                             (0x01000000UL) /**< rw - If set to 1 the drivers are enabled concurrently with the booster!. */
#define CLIF_TEST_CONTROL_REG_TX_BOOSTER_DRIVER_ON_POS                                              (24UL)
#define CLIF_TEST_CONTROL_REG_DEFAULT_PLL_MODE_ENABLE_MASK                                          (0x00800000UL) /**< rw - By setting this bit the default mode for switching to PLL clock is used. Note: FORCE_PLL_CLOCK_ON_TXACTIVE is not working with this setting!. */
#define CLIF_TEST_CONTROL_REG_DEFAULT_PLL_MODE_ENABLE_POS                                           (23UL)
#define CLIF_TEST_CONTROL_REG_TADT_EXTEND_SELECT_MASK                                               (0x00600000UL) /**< rw - By setting a value != 0 the minimum time for TADT can be increased. */
#define CLIF_TEST_CONTROL_REG_TADT_EXTEND_SELECT_POS                                                (21UL)
#define CLIF_TEST_CONTROL_REG_PRBS_TYPE_MASK                                                        (0x00100000UL) /**< rw - Defines the PRBS type. If set to 1 PRBS15 is selected default value 0 selects PRBS9. */
#define CLIF_TEST_CONTROL_REG_PRBS_TYPE_POS                                                         (20UL)
#define CLIF_TEST_CONTROL_REG_LONG_LD_MASK_ENABLE_MASK                                              (0x00080000UL) /**< rw - If set to 1 the masking time for the NFC-LD after TxRFOff is longer.. */
#define CLIF_TEST_CONTROL_REG_LONG_LD_MASK_ENABLE_POS                                               (19UL)
#define CLIF_TEST_CONTROL_REG_TIDT_SKIP_ENABLE_MASK                                                 (0x00040000UL) /**< rw - If set to 1 the TIDT reload value is reduced to 1 resulting in a nearly immediate switch on of the RF-Field.. */
#define CLIF_TEST_CONTROL_REG_TIDT_SKIP_ENABLE_POS                                                  (18UL)
#define CLIF_TEST_CONTROL_REG_TYPEB_TX_WAIT_PRESELECT_MASK                                          (0x00020000UL) /**< rw - If set to 1 a dedicated value for tx_wait could be pre-configured in the CLIF_TX_WAIT_REG register to be set when TypeB is detected. */
#define CLIF_TEST_CONTROL_REG_TYPEB_TX_WAIT_PRESELECT_POS                                           (17UL)
#define CLIF_TEST_CONTROL_REG_DIRECT_DATA_ACCESS_ENABLE_MASK                                        (0x00010000UL) /**< rw - If set to one the transfer between the BufferManager and the RAM is done via the ABP with IRQs instead the BMA.. */
#define CLIF_TEST_CONTROL_REG_DIRECT_DATA_ACCESS_ENABLE_POS                                         (16UL)
#define CLIF_TEST_CONTROL_REG_NRZ_SOF_DETECT_TESTBIT_MASK                                           (0x00008000UL) /**< rw - If set to 1 an alternate version (as used on PN547) is used for NRZ SOF detection. Per default the advance detection mechanism is used. */
#define CLIF_TEST_CONTROL_REG_NRZ_SOF_DETECT_TESTBIT_POS                                            (15UL)
#define CLIF_TEST_CONTROL_REG_MILLER_CLOCKFAIL_IGNORED_MASK                                         (0x00004000UL) /**< rw - If set to 1 clockfail is not used for miller decoding. */
#define CLIF_TEST_CONTROL_REG_MILLER_CLOCKFAIL_IGNORED_POS                                          (14UL)
#define CLIF_TEST_CONTROL_REG_MODE_DETECTOR_SEL_MASK                                                (0x00002000UL) /**< rw - Chooses the mode detector type:: 0* -> Enhanced Type B SOF detection enabled (to avoid that FeliCa 212kBd is interpreted as Type B) : 1 -> Regular Type B SOF detection (from PN544). */
#define CLIF_TEST_CONTROL_REG_MODE_DETECTOR_SEL_POS                                                 (13UL)
#define CLIF_TEST_CONTROL_REG_MILLER_DECODER_SEL_MASK                                               (0x00001000UL) /**< rw - Chooses the Miller decoder 0* Advanced Miller decoder (HFO clock based) 1 RF clock based Miller decoder. */
#define CLIF_TEST_CONTROL_REG_MILLER_DECODER_SEL_POS                                                (12UL)
#define CLIF_TEST_CONTROL_REG_TADT_SKIP_CHECK_ENABLE_MASK                                           (0x00000800UL) /**< rw - If set to 1 the timing check for TADT is skipped . */
#define CLIF_TEST_CONTROL_REG_TADT_SKIP_CHECK_ENABLE_POS                                            (11UL)
#define CLIF_TEST_CONTROL_REG_RF_FREQ_OK_OVERRIDE_ENABLE_MASK                                       (0x00000400UL) /**< rw - If set to 1 the frequency check of the RF-Clock is overridden. . */
#define CLIF_TEST_CONTROL_REG_RF_FREQ_OK_OVERRIDE_ENABLE_POS                                        (10UL)
#define CLIF_TEST_CONTROL_REG_RFDET_SOURCE_SEL_MASK                                                 (0x00000300UL) /**< rw - Select the source for RF-Field detection. 0* -> NFC-Level detector indication signal is used. 1 -> RF-Level detector indication signal is used 2. -> NFC- and RF-Level detector indication signal is used 3. -> Override - RF-Field detected is emulated. */
#define CLIF_TEST_CONTROL_REG_RFDET_SOURCE_SEL_POS                                                  (8UL)
#define CLIF_TEST_CONTROL_REG_EXTERN_PRO_MASK                                                       (0x00000080UL) /**< rw - SigPro: This is a test option for the signal processing. If set to 1 signal processing can be done externaly.. */
#define CLIF_TEST_CONTROL_REG_EXTERN_PRO_POS                                                        (7UL)
#define CLIF_TEST_CONTROL_REG_SIG_STARTBIT_MASK                                                     (0x00000040UL) /**< rw - This is a test option for the signal processing. This bit shall never be changed by software. Its needed when the signal processing is done off-chip. . */
#define CLIF_TEST_CONTROL_REG_SIG_STARTBIT_POS                                                      (6UL)
#define CLIF_TEST_CONTROL_REG_SIG_I_MASK                                                            (0x00000020UL) /**< rw - SigPro: This is a test option for the signal processing. This bit shall never be changed by software. Defines if the ADC data channel I is transferred to the external processing.. */
#define CLIF_TEST_CONTROL_REG_SIG_I_POS                                                             (5UL)
#define CLIF_TEST_CONTROL_REG_SIG_Q_MASK                                                            (0x00000010UL) /**< rw - SigPro: This is a test option for the signal processing. This bit shall never be changed by software. Defines if the DC data channel Q is transferred to the external processing.. */
#define CLIF_TEST_CONTROL_REG_SIG_Q_POS                                                             (4UL)
#define CLIF_TEST_CONTROL_REG_RESET_RF_ON_OFF_MASK                                                  (0x00000004UL) /**< rw - SigPro: This is a test option for the signal processing. This bit shall never be changed by software. The card demodulator is not reset in case of the abscence of the RF field. */
#define CLIF_TEST_CONTROL_REG_RESET_RF_ON_OFF_POS                                                   (2UL)
#define CLIF_TEST_CONTROL_REG_STOP_CLOCK_MASK                                                       (0x00000002UL) /**< rw - SigPro: This is a test option for the signal processing. This bit shall never be changed by software.. */
#define CLIF_TEST_CONTROL_REG_STOP_CLOCK_POS                                                        (1UL)
#define CLIF_TEST_CONTROL_REG_TX_ENV_TB_INPUT_ENABLE_MASK                                           (0x00000001UL) /**< rw - If set to 1 the tx_envelope used for driver is connected to testbus input bit 0. */
#define CLIF_TEST_CONTROL_REG_TX_ENV_TB_INPUT_ENABLE_POS                                            (0UL)
#define CLIF_TEST_CONTROL_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name NFC Level Detector Control Register
* <b>Reset value:</b> 0x00000104\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_NFCLD_REG                                                                          (0x40004100UL) /**< rw*/
#define CLIF_ANA_NFCLD_REG_CM_PD_NFC_DET_MASK                                                       (0x00000100UL) /**< rw - Power Down NFC level detector. */
#define CLIF_ANA_NFCLD_REG_CM_PD_NFC_DET_POS                                                        (8UL)
#define CLIF_ANA_NFCLD_REG_CM_RFL_NFC_MASK                                                          (0x0000003FUL) /**< rw - Programming of detection level. */
#define CLIF_ANA_NFCLD_REG_CM_RFL_NFC_POS                                                           (0UL)
#define CLIF_ANA_NFCLD_REG__RESET_VALUE                                                             (0x00000104UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog TX Clock control register
* <b>Reset value:</b> 0x00000083\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_TX_CLK_CONTROL_REG                                                                 (0x40004104UL) /**< rw*/
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX_CLK_MODE_OVUN_PREV_MASK                                      (0x00000700UL) /**< rw - Defines the TX clockmode for the period the overshoot/undershoot prevention is active. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX_CLK_MODE_OVUN_PREV_POS                                       (8UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX2_INV_RM_MASK                                                 (0x00000080UL) /**< rw - If 1 -> TX output is inverted (clk_13m56_n is used). 0 -> clk_13m56 is used. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX2_INV_RM_POS                                                  (7UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX2_INV_CM_MASK                                                 (0x00000040UL) /**< rw - If 1 -> TX output is inverted (clk_13m56_n is used). 0 -> clk_13m56 is used. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX2_INV_CM_POS                                                  (6UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX1_INV_RM_MASK                                                 (0x00000020UL) /**< rw - If 1 -> TX output is inverted (clk_13m56_n is used). 0 -> clk_13m56 is used. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX1_INV_RM_POS                                                  (5UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX1_INV_CM_MASK                                                 (0x00000010UL) /**< rw - If 1 -> TX output is inverted (clk_13m56_n is used). 0 -> clk_13m56 is used. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX1_INV_CM_POS                                                  (4UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX_CLK_MODE_RM_MASK                                             (0x0000000EUL) /**< rw - TX clockmode. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX_CLK_MODE_RM_POS                                              (1UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX_PD_CLK_GEN_MASK                                              (0x00000001UL) /**< rw - TX clock generation disabled and thus also envelope synchronization to clk is disabled.. */
#define CLIF_ANA_TX_CLK_CONTROL_REG_TX_PD_CLK_GEN_POS                                               (0UL)
#define CLIF_ANA_TX_CLK_CONTROL_REG__RESET_VALUE                                                    (0x00000083UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog TX amplitude control register
* <b>Reset value:</b> 0x00000003\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_TX_AMPLITUDE_REG                                                                   (0x40004108UL) /**< rw*/
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_CW_RM_MASK                                                 (0xF0000000UL) /**< rw - gsn setting at continuous wave in reader mode. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_CW_RM_POS                                                  (28UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_CW_CM_MASK                                                 (0x0F000000UL) /**< rw - gsn setting at continuous wave in card mode. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_CW_CM_POS                                                  (24UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_MOD_RM_MASK                                                (0x00F00000UL) /**< rw - gsn setting at continuous wave in reader mode. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_MOD_RM_POS                                                 (20UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_MOD_CM_MASK                                                (0x000F0000UL) /**< rw - gsn setting at continuous wave in card mode. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_GSN_MOD_CM_POS                                                 (16UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_ASKMODE_SELECT_MASK                                            (0x00004000UL) /**< rw - Selects the output voltage mode of the transmitter. Note: This bitfield is new on PN549 default value is 1! 0: Setting used for ALM mode. Tx output voltage is independent of the envelope and hence the TX output amplitude can be adjusted by the bitfield TX_RESIDUAL_CARRIER. 1: Setting used for standard modes (reader and active mode). */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_ASKMODE_SELECT_POS                                             (14UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_AMPLITUDE_RM_MASK                                           (0x00003000UL) /**< rw - set amplitude of unmodulated carrier at reader mode. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_AMPLITUDE_RM_POS                                            (12UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_AMPLITUDE_ALM_CM_MASK                                       (0x00000300UL) /**< rw - set amplitude of unmodulated carrier at card mode. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_AMPLITUDE_ALM_CM_POS                                        (8UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_RESIDUAL_CARRIER_MASK                                          (0x000000F8UL) /**< rw - set residual carrier (0=100% 1F = 0%). */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_RESIDUAL_CARRIER_POS                                           (3UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_TO_MAX_RM_MASK                                              (0x00000004UL) /**< rw - TX HI output is the maximum voltage obtainable from charge pump (RM setting). if set to 1 -> TX_CW_AMPLITUDE_RM is overruled.. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_TO_MAX_RM_POS                                               (2UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_TO_MAX_ALM_CM_MASK                                          (0x00000002UL) /**< rw - TX HI output is the maximum voltage obtainable from charge pump (CM setting). if set to 1 -> TX_CW_AMPLITUDE_CM is overruled.. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_CW_TO_MAX_ALM_CM_POS                                           (1UL)
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_PD_MASK                                                        (0x00000001UL) /**< rw - charge pumps clamped to tvdd. */
#define CLIF_ANA_TX_AMPLITUDE_REG_TX_PD_POS                                                         (0UL)
#define CLIF_ANA_TX_AMPLITUDE_REG__RESET_VALUE                                                      (0x00000003UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog PbF control register
* <b>Reset value:</b> 0x000000A0\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_PBF_CONTROL_REG                                                                    (0x4000410CUL) /**< rw*/
#define CLIF_ANA_PBF_CONTROL_REG_PBF_INVERT_ON_TXACTIVE_MASK                                        (0x00000400UL) /**< rw - If set to 1 the PbF short interface in card mode is inverted when tx_active is asserted (i.e. while transmission). Note: this bit is only valid in card mode. Note: if it BF_ALM_AUTO_SWITCH_ENABLE is set this setting is ignored. */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_INVERT_ON_TXACTIVE_POS                                         (10UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_ALM_AUTO_SWITCH_ENABLE_MASK                                    (0x00000200UL) /**< rw - If set to 1 the PBF settting for ALM is switch automatically by HW. By default for ALM the pbf_short and pbf_mod uses the same settings as for PLM.. */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_ALM_AUTO_SWITCH_ENABLE_POS                                     (9UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_ALM_FW_RESET_MASK                                              (0x00000100UL) /**< -x - If set to 1 the PBF setting for ALM is reset to its initial receive configuration. */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_ALM_FW_RESET_POS                                               (8UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_SHORT_SELECT_RM_MASK                                           (0x00000080UL) /**< rw - Selects the control of the PbF modulation interface in reader mode. */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_SHORT_SELECT_RM_POS                                            (7UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_PD_REG_MASK                                                    (0x00000040UL) /**< rw - Power down of Regulator. */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_PD_REG_POS                                                     (6UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_EN_LIMVDHF_MASK                                                (0x00000020UL) /**< rw - Enable the VHDF limiter. */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_EN_LIMVDHF_POS                                                 (5UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_SHORT_SELECT_MASK                                              (0x0000000CUL) /**< rw - Selects the control of the PbF short interface in cardmode for PLM. in reader mode and ALM the analog control signals are switched by digital logic.   00b Constant 0 (PbF open)    01b Constant 1 (PbF short)   10b TxEnvelope used (idle = 1 modulation = 0)   11b Inverted TxEnvelope used (idle = 0 modulation = 1). */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_SHORT_SELECT_POS                                               (2UL)
#define CLIF_ANA_PBF_CONTROL_REG_PBF_MOD_SELECT_MASK                                                (0x00000003UL) /**< rw - Selects the control of the PbF modulation interface in cardmode for PLM. in reader mode and ALM the analog control signals are switched by digital logic.   00b Constant 0 (No modulation on PbF mod )   01b Constant 1 (modulation on PbF mod)   10b TxEnvelope used (idle = 1 modulation = 0)   11b Inverted TxEnvelope used (idle = 0 modulation = 1). */
#define CLIF_ANA_PBF_CONTROL_REG_PBF_MOD_SELECT_POS                                                 (0UL)
#define CLIF_ANA_PBF_CONTROL_REG__RESET_VALUE                                                       (0x000000A0UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog RM receiver control register
* <b>Reset value:</b> 0x0003D820\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_RX_REG                                                                             (0x40004110UL) /**< rw*/
#define CLIF_ANA_RX_REG_RX_SINGLE_MASK                                                              (0x00040000UL) /**< rw - Single rx Input Pin Mode. 0-> Fully differential. 1-> Quasi differential. */
#define CLIF_ANA_RX_REG_RX_SINGLE_POS                                                               (18UL)
#define CLIF_ANA_RX_REG_RX_PD_TEST_CLK_MASK                                                         (0x00020000UL) /**< rw - Power Down Mixer test clock output. */
#define CLIF_ANA_RX_REG_RX_PD_TEST_CLK_POS                                                          (17UL)
#define CLIF_ANA_RX_REG_RX_PD_MIXER_MASK                                                            (0x00010000UL) /**< rw - Power Down Reader Mixer. */
#define CLIF_ANA_RX_REG_RX_PD_MIXER_POS                                                             (16UL)
#define CLIF_ANA_RX_REG_RX_PD_ADC_Q_MASK                                                            (0x00008000UL) /**< rw - Power Down ADC Q-channel. */
#define CLIF_ANA_RX_REG_RX_PD_ADC_Q_POS                                                             (15UL)
#define CLIF_ANA_RX_REG_RX_PD_ADC_I_MASK                                                            (0x00004000UL) /**< rw - Power Down ADC I-channel. */
#define CLIF_ANA_RX_REG_RX_PD_ADC_I_POS                                                             (14UL)
#define CLIF_ANA_RX_REG_RX_PD_BBA_BIAS_MASK                                                         (0x00002000UL) /**< rw - Power Down Baseband Amplifier Bias Block. */
#define CLIF_ANA_RX_REG_RX_PD_BBA_BIAS_POS                                                          (13UL)
#define CLIF_ANA_RX_REG_RX_PD_BBA_Q_MASK                                                            (0x00001000UL) /**< rw - Power Down Baseband Amplifier Q-Channel. */
#define CLIF_ANA_RX_REG_RX_PD_BBA_Q_POS                                                             (12UL)
#define CLIF_ANA_RX_REG_RX_PD_BBA_I_MASK                                                            (0x00000800UL) /**< rw - Power Down Baseband Amplifier I-Channel. */
#define CLIF_ANA_RX_REG_RX_PD_BBA_I_POS                                                             (11UL)
#define CLIF_ANA_RX_REG_RX_PD_VMID_MASK                                                             (0x00000400UL) /**< rw - Power Down Vmid generation. */
#define CLIF_ANA_RX_REG_RX_PD_VMID_POS                                                              (10UL)
#define CLIF_ANA_RX_REG_RX_VMID_RSEL_MASK                                                           (0x00000300UL) /**< rw - Selection of resistance between VDD and VSS: 00->1.4k. 01->2.8k 10->5.7k 11->11k. */
#define CLIF_ANA_RX_REG_RX_VMID_RSEL_POS                                                            (8UL)
#define CLIF_ANA_RX_REG_RX_SHORT_MIX2BBA_ST2_MASK                                                   (0x00000080UL) /**< rw - Short Mixer to BBA Stage 2. I only. */
#define CLIF_ANA_RX_REG_RX_SHORT_MIX2BBA_ST2_POS                                                    (7UL)
#define CLIF_ANA_RX_REG_RX_SHORT_MIX2ADC_MASK                                                       (0x00000040UL) /**< rw - Short Mixer to ADC I and Q. */
#define CLIF_ANA_RX_REG_RX_SHORT_MIX2ADC_POS                                                        (6UL)
#define CLIF_ANA_RX_REG_RX_OC_ENABLE_MASK                                                           (0x00000020UL) /**< rw - Offset Compensation BBA. */
#define CLIF_ANA_RX_REG_RX_OC_ENABLE_POS                                                            (5UL)
#define CLIF_ANA_RX_REG_RX_HP_LOWF_MASK                                                             (0x00000010UL) /**< rw - Lower Corner Frequency of BBA to 2kHz. */
#define CLIF_ANA_RX_REG_RX_HP_LOWF_POS                                                              (4UL)
#define CLIF_ANA_RX_REG_RX_HPCF_MASK                                                                (0x0000000CUL) /**< rw - Lower Corner Frequency: 00->45kHz 01->85kHz 10->150kHz 11->250kHz. */
#define CLIF_ANA_RX_REG_RX_HPCF_POS                                                                 (2UL)
#define CLIF_ANA_RX_REG_RX_GAIN_MASK                                                                (0x00000003UL) /**< rw - Gain Adjustment BBA: 00->33dB 01->40dB 10->50dB 11->57dB. */
#define CLIF_ANA_RX_REG_RX_GAIN_POS                                                                 (0UL)
#define CLIF_ANA_RX_REG__RESET_VALUE                                                                (0x0003D820UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog CM control register
* <b>Reset value:</b> 0x0001C080\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_CM_CONFIG_REG                                                                      (0x40004114UL) /**< rw*/
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_RSSI_MASK                                                      (0x00010000UL) /**< rw - Power Down for the RSSI subsystem (AGC based). */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_RSSI_POS                                                       (16UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_MILLER_MASK                                                    (0x00008000UL) /**< rw - Power Down CM reference based miller demodulator. */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_MILLER_POS                                                     (15UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_MILLER_EN_LOCK_MASK                                               (0x00004000UL) /**< rw - CM miller reference lock functionality when set to 1 the feedback loop locks the reference level during a Miller pulse. */
#define CLIF_ANA_CM_CONFIG_REG_CM_MILLER_EN_LOCK_POS                                                (14UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_MILLER_SENS_MASK                                                  (0x00003000UL) /**< rw - Configuration bits for reference level of Miller demodulator. */
#define CLIF_ANA_CM_CONFIG_REG_CM_MILLER_SENS_POS                                                   (12UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_MILLER_TAU_MASK                                                   (0x00000C00UL) /**< rw - Configuration bits for the time constant of the reference generation in Miller demodulator. */
#define CLIF_ANA_CM_CONFIG_REG_CM_MILLER_TAU_POS                                                    (10UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_MIXER_I_MASK                                                   (0x00000200UL) /**< rw - Power down CM mixer i-channel. */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_MIXER_I_POS                                                    (9UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_MIXER_Q_MASK                                                   (0x00000100UL) /**< rw - Power down CM mixer q-channel. */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_MIXER_Q_POS                                                    (8UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_MIXER_ENABLE_PULLDOWN_MASK                                        (0x00000080UL) /**< rw - Enable Pulldown at Mixer Output. */
#define CLIF_ANA_CM_CONFIG_REG_CM_MIXER_ENABLE_PULLDOWN_POS                                         (7UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_ATTEN_ALWAYS_ON_MASK                                              (0x00000040UL) /**< rw - Switch on attenuator disregarding mixer I channel output (for AGC usage in Reader Mode). */
#define CLIF_ANA_CM_CONFIG_REG_CM_ATTEN_ALWAYS_ON_POS                                               (6UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_CLKREC_MASK                                                    (0x00000020UL) /**< rw - Power Down CM clock recovery. */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_CLKREC_POS                                                     (5UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_DLL_MASK                                                       (0x00000010UL) /**< rw - Power Down CM delay lock loop (DLL). */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_DLL_POS                                                        (4UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_DLL_CP_MASK                                                    (0x00000008UL) /**< rw - Power Down CM charge pump (inside DLL). */
#define CLIF_ANA_CM_CONFIG_REG_CM_PD_DLL_CP_POS                                                     (3UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_CLK_REC_DIFF_MASK                                                 (0x00000004UL) /**< rw - Configure Clock recovery for differential input (RFU). */
#define CLIF_ANA_CM_CONFIG_REG_CM_CLK_REC_DIFF_POS                                                  (2UL)
#define CLIF_ANA_CM_CONFIG_REG_CM_DLL_COARSE_MASK                                                   (0x00000003UL) /**< rw - Configuration bits for dll current (default setting=00). */
#define CLIF_ANA_CM_CONFIG_REG_CM_DLL_COARSE_POS                                                    (0UL)
#define CLIF_ANA_CM_CONFIG_REG__RESET_VALUE                                                         (0x0001C080UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog CM plus control register
* <b>Reset value:</b> 0x00000200\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_CMPLUS_B_REG                                                                       (0x40004118UL) /**< rw*/
#define CLIF_ANA_CMPLUS_B_REG_CM_PD_CM_PLUS_MASK                                                    (0x00000200UL) /**< rw - Power Down CM+ for typeA higher baudrate (424&848k) and FeliCa communication. */
#define CLIF_ANA_CMPLUS_B_REG_CM_PD_CM_PLUS_POS                                                     (9UL)
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_SW_RESET_MASK                                                 (0x00000080UL) /**< rw - Reset CM+ comparator (by SW). */
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_SW_RESET_POS                                                  (7UL)
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_MASK_RESET_MASK                                               (0x00000040UL) /**< rw - Disable automatic reset for CM+ comparator. */
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_MASK_RESET_POS                                                (6UL)
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_BBA2COMP_MASK                                                 (0x00000020UL) /**< rw - Connect BBA output to CM+ comparator. */
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_BBA2COMP_POS                                                  (5UL)
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_DYN_HYST_MASK                                                 (0x00000010UL) /**< rw - CM+ comparator when set to 1 the hysteresis and offset of CM+ comparator is multiplied by factor of 3. */
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_DYN_HYST_POS                                                  (4UL)
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_HYST_MASK                                                     (0x0000000CUL) /**< rw - Configuration bits for CM+ comp hysteresis 00->27mV 01->51mV 10->76mV 11->101mV. */
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_HYST_POS                                                      (2UL)
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_OFFSET_MASK                                                   (0x00000003UL) /**< rw - Configuration bits for CM+ comparator offset 00->0mV 01->36mV 10->72mV 11->110mV. */
#define CLIF_ANA_CMPLUS_B_REG_CM_PLUS_OFFSET_POS                                                    (0UL)
#define CLIF_ANA_CMPLUS_B_REG__RESET_VALUE                                                          (0x00000200UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog AGC control register
* <b>Reset value:</b> 0x00000004\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_AGC_REG                                                                            (0x4000411CUL) /**< rw*/
#define CLIF_ANA_AGC_REG_AGC_TEST_CLOCK_SEL_MASK                                                    (0x00000008UL) /**< rw - Select the PLL clock for testing the comparator. */
#define CLIF_ANA_AGC_REG_AGC_TEST_CLOCK_SEL_POS                                                     (3UL)
#define CLIF_ANA_AGC_REG_AGC_PD_MASK                                                                (0x00000004UL) /**< rw - AGC power down. */
#define CLIF_ANA_AGC_REG_AGC_PD_POS                                                                 (2UL)
#define CLIF_ANA_AGC_REG_AGC_VREF_SEL_MASK                                                          (0x00000003UL) /**< rw - Select the comparison reference voltage. */
#define CLIF_ANA_AGC_REG_AGC_VREF_SEL_POS                                                           (0UL)
#define CLIF_ANA_AGC_REG__RESET_VALUE                                                               (0x00000004UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog clock management control Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_CLK_MAN_REG                                                                        (0x40004120UL) /**< rw*/
#define CLIF_ANA_CLK_MAN_REG_CLOCK_TX_SEL_SLALM_MASK                                                (0x00000040UL) /**< rw - Selects DPLL. */
#define CLIF_ANA_CLK_MAN_REG_CLOCK_TX_SEL_SLALM_POS                                                 (6UL)
#define CLIF_ANA_CLK_MAN_REG_CLOCK_PD_RF_27M12_MASK                                                 (0x00000020UL) /**< rw - Power down RF clock generation. */
#define CLIF_ANA_CLK_MAN_REG_CLOCK_PD_RF_27M12_POS                                                  (5UL)
#define CLIF_ANA_CLK_MAN_REG_CLOCK_ADC_SEL_RF_MASK                                                  (0x00000010UL) /**< rw - Select RF clock for ADC from DLL or PLL. */
#define CLIF_ANA_CLK_MAN_REG_CLOCK_ADC_SEL_RF_POS                                                   (4UL)
#define CLIF_ANA_CLK_MAN_REG_CLOCK_TX_SEL_RF_MASK                                                   (0x00000008UL) /**< rw - Select DLL clock for TX (instead of PLL clock). */
#define CLIF_ANA_CLK_MAN_REG_CLOCK_TX_SEL_RF_POS                                                    (3UL)
#define CLIF_ANA_CLK_MAN_REG_CLOCK_CONFIG_DLL_ALM_MASK                                              (0x00000007UL) /**< rw - Select DLL clock phase. */
#define CLIF_ANA_CLK_MAN_REG_CLOCK_CONFIG_DLL_ALM_POS                                               (0UL)
#define CLIF_ANA_CLK_MAN_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog Testbus control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_TESTBUS_SELECT_REG                                                                 (0x40004124UL) /**< rw*/
#define CLIF_ANA_TESTBUS_SELECT_REG_TX_TB1_SELECT_MASK                                              (0x70000000UL) /**< rw - TX test bus 1 signal select -> see Table: CLIF Analog Test Bus Select 0 -> Idle. 1 -> test_tx_v_fb_aux_cw. 2 -> test_tx_v_fb_cw. 3 -> test_tx_sig_2. 4 -> test_tx_vref_mod. 5 -> test_tx_v_fb_mod. 6 -> test_tx_v_fb_aux_mod. */
#define CLIF_ANA_TESTBUS_SELECT_REG_TX_TB1_SELECT_POS                                               (28UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_TX_TB0_SELECT_MASK                                              (0x07000000UL) /**< rw - TX test bus 0 signal select -> see Table: CLIF Analog Test Bus Select 0 -> Idle. 1 -> test_tx_v_fb_aux_mod. 2 -> test_tx_v_fb_mod. 3 -> test_tx_sig_1. 4 -> test_tx_vref_cw. 5 -> test_tx_v_fb_cw. 6 -> test_tx_v_fb_aux_cw. */
#define CLIF_ANA_TESTBUS_SELECT_REG_TX_TB0_SELECT_POS                                               (24UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_ATBN_SELECT_MASK                                                (0x00F00000UL) /**< rw - Analog testbus rxn signal select 0 -> Idle. 1 -> atbn_select_bba_out_i_n. 2 -> atbn_select_bba_out_q_n. 3 -> atbn_select_bba_stage1_i_n. 4 -> atbn_select_bba_stage1_q_n. 5 -> atbn_select_mix_out_i_n. 6 -> atbn_select_mix_out_q_n. */
#define CLIF_ANA_TESTBUS_SELECT_REG_ATBN_SELECT_POS                                                 (20UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_ATBP_SELECT_MASK                                                (0x000F0000UL) /**< rw - Analog testbus rxp signal select 0 -> Idle. 1 -> atbp_select_bba_out_i_p. 2 -> atbp_select_bba_out_q_p. 3 -> atbp_select_bba_stage1_i_p. 4 -> atbp_select_bba_stage1_q_p. 5 -> atbp_select_mix_out_i_p. 6 -> atbp_select_mix_out_q_p. */
#define CLIF_ANA_TESTBUS_SELECT_REG_ATBP_SELECT_POS                                                 (16UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB3_SELECT_MASK                                                (0x0000F000UL) /**< rw - Analog testbus 3 signal select 0 -> Idle. 1 -> atb3_cm_demod_q_n. 2 -> atb3_cm_miller_vn_slow. 3 -> atb3_cm_typeb_vn_slow. 4 -> atb3_cm_plus_comp_in_n. 5 -> atb3_cm_clkreg_i_in. 6 -> atb3_cm_dll_i_in. */
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB3_SELECT_POS                                                 (12UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB2_SELECT_MASK                                                (0x00000F00UL) /**< rw - Analog testbus 2 signal select 0 -> Idle. 1 -> atb2_cm_demod_q_p. 2 -> atb2_cm_miller_vp_slow. 3 -> atb2_cm_typeb_vp_slow. 4 -> atb2_cm_plus_comp_in_p. 5 -> atb2_cm_clkreg_i_out. 6 -> atb2_cm_dll_i_out. 7 -> atb2_rfld_vref. 8 -> atb2_agc_vref. */
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB2_SELECT_POS                                                 (8UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB1_SELECT_MASK                                                (0x000000F0UL) /**< rw - Analog testbus 1 signal select 0 -> Idle. 1 -> atb1_select_mix_out_i_p. 2 -> atb1_select_mix_out_i_n. 3 -> atb1_select_mix_out_q_p. 4 -> atb1_select_mix_out_q_n. 5 -> atb1_select_bba_stage1_i_p. 6 -> atb1_select_bba_stage1_i_n. 7 -> atb1_select_bba_stage1_q_p. 8 -> atb1_select_bba_stage1_q_n. 9 -> atb1_select_bba_out_i_p. 10 -> atb1_select_bba_out_i_n. 11 -> atb1_select_bba_out_q_p. 12 -> atb1_select_bba_out_q_n. 13 -> atb1_select_dac2. 14 -> atb1_cm_dll_vctr_n. 15 -> atb1_rfld_vcref. */
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB1_SELECT_POS                                                 (4UL)
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB0_SELECT_MASK                                                (0x0000000FUL) /**< rw - Analog testbus 0 signal select -> see Table: CLIF Analog Testbus Select @ sheet TSTBUS 0 -> Idle. 1 -> atb0_select_mix_out_i_p. 2 -> atb0_select_mix_out_i_n. 3 -> atb0_select_mix_out_q_p. 4 -> atb0_select_mix_out_q_n. 5 -> atb0_select_bba_stage1_i_p. 6 -> atb0_select_bba_stage1_i_n. 7 -> atb0_select_bba_stage1_q_p. 8 -> atb0_select_bba_stage1_q_n. 9 -> atb0_select_bba_out_i_p. 10 -> atb0_select_bba_out_i_n. 11 -> atb0_select_bba_out_q_p. 12 -> atb0_select_bba_out_q_n. 13 -> atb0_select_dac1. 14 -> atb0_cm_dll_vctr_p. 15 -> atb0_rfld_vcrx. */
#define CLIF_ANA_TESTBUS_SELECT_REG_ATB0_SELECT_POS                                                 (0UL)
#define CLIF_ANA_TESTBUS_SELECT_REG__RESET_VALUE                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog TX shaping control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_TX_SHAPE_CONTROL_REG                                                               (0x40004128UL) /**< rw*/
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_RESIDUAL_CARRIER_OV_PREV_MASK                              (0x1F000000UL) /**< rw - Defines the value for the residual carrier for the period the overshoot prevention pattern is active.. */
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_RESIDUAL_CARRIER_OV_PREV_POS                               (24UL)
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_BYPASS_SC_SHAPING_MASK                                 (0x00020000UL) /**< rw - Bypasses switched capacitor shaping of the Transmitter Signal. */
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_BYPASS_SC_SHAPING_POS                                  (17UL)
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_SLEW_SHUNTREG_MASK                                     (0x00000F00UL) /**< rw - Set slew rate for shunt regulator. */
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_SLEW_SHUNTREG_POS                                      (8UL)
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_TAU_MOD_FALLING_MASK                                   (0x000000F0UL) /**< rw - Transmitter TAU setting for falling edge of modulation shape. In AnalogControl module the output signal is switched with the tx_envelope. Only valid is TX_SET_SINGLE_CP_MODE is set. */
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_TAU_MOD_FALLING_POS                                    (4UL)
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_TAU_MOD_RISING_MASK                                    (0x0000000FUL) /**< rw - Transmitter TAU setting for rising edge of modulation shape. In AnalogControl module the output signal is switched with the tx_envelope. Only valid is TX_SET_SINGLE_CP_MODE is set. */
#define CLIF_ANA_TX_SHAPE_CONTROL_REG_TX_SET_TAU_MOD_RISING_POS                                     (0UL)
#define CLIF_ANA_TX_SHAPE_CONTROL_REG__RESET_VALUE                                                  (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog Testbus control register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_TESTBUS_SELECT2_REG                                                                (0x4000412CUL) /**< rw*/
#define CLIF_ANA_TESTBUS_SELECT2_REG_TX_TB2_SELECT_MASK                                             (0x00000007UL) /**< rw - TX test bus 2 signal select -> see Table: CLIF Analog TEst Bus Select. */
#define CLIF_ANA_TESTBUS_SELECT2_REG_TX_TB2_SELECT_POS                                              (0UL)
#define CLIF_ANA_TESTBUS_SELECT2_REG__RESET_VALUE                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Analog test control register
* <b>Reset value:</b> 0x0050004A\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_ANA_TEST_REG                                                                           (0x400041FCUL) /**< rw*/
#define CLIF_ANA_TEST_REG_TX_TESTMODE_ENABLE_MASK                                                   (0x04000000UL) /**< rw - If set to 1 the Tx output is forced to Z and enables Tx Testsignals to be propagated over TX pads. */
#define CLIF_ANA_TEST_REG_TX_TESTMODE_ENABLE_POS                                                    (26UL)
#define CLIF_ANA_TEST_REG_TXPROT_TYPE_B_DEMOD_DYN_ENABLE_MASK                                       (0x02000000UL) /**< rw - Input mux activation for txprot_type_b_demod signal. */
#define CLIF_ANA_TEST_REG_TXPROT_TYPE_B_DEMOD_DYN_ENABLE_POS                                        (25UL)
#define CLIF_ANA_TEST_REG_TX_PD_CLK_GEN_OVERRIDE_MASK                                               (0x01000000UL) /**< rw - If set to 1it is possible to override the PD of the Tx clock gen (register CLIF_ANA_TX_CLK_CONTROL_REG). */
#define CLIF_ANA_TEST_REG_TX_PD_CLK_GEN_OVERRIDE_POS                                                (24UL)
#define CLIF_ANA_TEST_REG_PBF_PD_CM_OVERRIDE_MASK                                                   (0x00800000UL) /**< rw - If set to 1it is possible to override the power-down setting for the PbF interface (register CLIF_ANA_PBF_CONTROL_REG). */
#define CLIF_ANA_TEST_REG_PBF_PD_CM_OVERRIDE_POS                                                    (23UL)
#define CLIF_ANA_TEST_REG_TX_SET_ILOAD_MASK                                                         (0x00780000UL) /**< rw - Settings for set_iload and corresponding assumed load current in the output path. */
#define CLIF_ANA_TEST_REG_TX_SET_ILOAD_POS                                                          (19UL)
#define CLIF_ANA_TEST_REG_TX_CLOCK_DUTY_SET_N_MASK                                                  (0x0007E000UL) /**< rw - With this setting the duty cycle of the inverted TX 13.56MHz clock can be adjusted. */
#define CLIF_ANA_TEST_REG_TX_CLOCK_DUTY_SET_N_POS                                                   (13UL)
#define CLIF_ANA_TEST_REG_TX_CLOCK_DUTY_SET_P_MASK                                                  (0x00001F80UL) /**< rw - With this setting the duty cycle of the TX 13.56MHz clock can be adjusted. */
#define CLIF_ANA_TEST_REG_TX_CLOCK_DUTY_SET_P_POS                                                   (7UL)
#define CLIF_ANA_TEST_REG_TX_CLOCK_DUTY_SET_PD_MASK                                                 (0x00000040UL) /**< rw - If set to 1 the duty cycle regulation is disabled. */
#define CLIF_ANA_TEST_REG_TX_CLOCK_DUTY_SET_PD_POS                                                  (6UL)
#define CLIF_ANA_TEST_REG_TX_SET_IBOOST_MASK                                                        (0x0000001EUL) /**< rw - boost setting for charge pump. */
#define CLIF_ANA_TEST_REG_TX_SET_IBOOST_POS                                                         (1UL)
#define CLIF_ANA_TEST_REG_TX_CW_AMP_REF2TVDD_MASK                                                   (0x00000001UL) /**< rw - If set to 1 the reference of the unmodulated carrier is defined relative to TVDDNote: Do not change in normal operation. */
#define CLIF_ANA_TEST_REG_TX_CW_AMP_REF2TVDD_POS                                                    (0UL)
#define CLIF_ANA_TEST_REG__RESET_VALUE                                                              (0x0050004AUL) /**< value after reset */
/**
* @}
*/


/**
* \name RFU Register
* <b>Reset value:</b> 0xFF000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_SPARE_REG                                                                              (0x40004200UL) /**< rw*/
#define CLIF_SPARE_REG_SPARE_RW_MASK                                                                (0xFFFF0000UL) /**< rw - Read-Write spare registers. */
#define CLIF_SPARE_REG_SPARE_RW_POS                                                                 (16UL)
#define CLIF_SPARE_REG_SPARE_RO_MASK                                                                (0x0000FFFFUL) /**< r- - Read-only spare registers. */
#define CLIF_SPARE_REG_SPARE_RO_POS                                                                 (0UL)
#define CLIF_SPARE_REG__RESET_VALUE                                                                 (0xFF000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name DPLL Control Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_DPLL_CONTROL_REG                                                                       (0x40004204UL) /**< rw*/
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_TX_ENV_MASK                                            (0x00000300UL) /**< rw - Freeze when activly modulating (env for ALM == 1). 'b00 = disabled 'b01 = unfreeze as early as possible after modulation 'b10 = unfreeze with additional medium delay after modulation (additional 3 cycles) 'b11 = do not unfreeze during subcarrier modulation (except phase  shift). */
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_TX_ENV_POS                                             (8UL)
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_FW_MASK                                                (0x00000080UL) /**< rw - Freeze initiated by FW intended to change the gear shift settings. */
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_FW_POS                                                 (7UL)
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_ADCBCM_ACTIVE_MASK                                     (0x00000040UL) /**< rw - Freezes the DPLL frequency already when the ADC based card-mode detects an envelope. */
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_ADCBCM_ACTIVE_POS                                      (6UL)
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_RX_MASK                                                (0x00000020UL) /**< rw - Freezes the DPLL frequency in RECEIVE state (as soon as the start of reception is detected). */
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_RX_POS                                                 (5UL)
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_WAITDATA_MASK                                          (0x00000010UL) /**< rw - Freezes the DPLL frequency in WAIT_FOR_DATA state. */
#define CLIF_DPLL_CONTROL_REG_DPLL_FREEZE_ON_WAITDATA_POS                                           (4UL)
#define CLIF_DPLL_CONTROL_REG_DPLL_RESTART_GEAR_TXWAIT_MASK                                         (0x00000004UL) /**< rw - Command to restart the gear shifting at the beginning of Tx-Wait from gear zero. */
#define CLIF_DPLL_CONTROL_REG_DPLL_RESTART_GEAR_TXWAIT_POS                                          (2UL)
#define CLIF_DPLL_CONTROL_REG_DPLL_RESTART_GEAR_MASK                                                (0x00000002UL) /**< rw - Command to restart the gear shifting from gear zero with initial settings from CLIF_DPLL_INIT_REG. */
#define CLIF_DPLL_CONTROL_REG_DPLL_RESTART_GEAR_POS                                                 (1UL)
#define CLIF_DPLL_CONTROL_REG_SINGLE_LOOP_ALM_ENABLE_MASK                                           (0x00000001UL) /**< rw - Enables the DPLL controller. */
#define CLIF_DPLL_CONTROL_REG_SINGLE_LOOP_ALM_ENABLE_POS                                            (0UL)
#define CLIF_DPLL_CONTROL_REG__RESET_VALUE                                                          (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name DPLL Configuration Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_DPLL_INIT_REG                                                                          (0x40004208UL) /**< rw*/
#define CLIF_DPLL_INIT_REG_DPLL_CLOCK_CONFIG_ALM_SIGN_MASK                                          (0x08000000UL) /**< rw - Defines the sign of the fine phase-changes (positive or negative).. */
#define CLIF_DPLL_INIT_REG_DPLL_CLOCK_CONFIG_ALM_SIGN_POS                                           (27UL)
#define CLIF_DPLL_INIT_REG_DPLL_CLOCK_CONFIG_ALM_MASK                                               (0x07000000UL) /**< rw - Is the fine tuning of the phase configuration done on top of the coarse setting CLIF_ANA_CLK_MAN_REG.CLOCK_CONFIG_DLL_ALM. */
#define CLIF_DPLL_INIT_REG_DPLL_CLOCK_CONFIG_ALM_POS                                                (24UL)
#define CLIF_DPLL_INIT_REG_DPLL_WAKEUP_TIME_MASK                                                    (0x00300000UL) /**< rw - Duration of the Wakeup-Mode. */
#define CLIF_DPLL_INIT_REG_DPLL_WAKEUP_TIME_POS                                                     (20UL)
#define CLIF_DPLL_INIT_REG_DPLL_WAKEUP_INT_ENABLE_MASK                                              (0x00040000UL) /**< rw - Enables the integral path of the loop-filter even in WAKEUP mode. */
#define CLIF_DPLL_INIT_REG_DPLL_WAKEUP_INT_ENABLE_POS                                               (18UL)
#define CLIF_DPLL_INIT_REG_DPLL_WAKEUP_LF_SEL_MASK                                                  (0x00030000UL) /**< rw - Proportional parameter of loop filter for WAKEUP mode. */
#define CLIF_DPLL_INIT_REG_DPLL_WAKEUP_LF_SEL_POS                                                   (16UL)
#define CLIF_DPLL_INIT_REG_DPLL_TIME_INIT_SEL_MASK                                                  (0x00003000UL) /**< rw - Initial gear duration. */
#define CLIF_DPLL_INIT_REG_DPLL_TIME_INIT_SEL_POS                                                   (12UL)
#define CLIF_DPLL_INIT_REG_DPLL_INT_INIT_SEL_MASK                                                   (0x00000700UL) /**< rw - Initial gear integral parameter. */
#define CLIF_DPLL_INIT_REG_DPLL_INT_INIT_SEL_POS                                                    (8UL)
#define CLIF_DPLL_INIT_REG_DPLL_PROP_INIT_SEL_MASK                                                  (0x00000070UL) /**< rw - Initial gear proportional parameter. */
#define CLIF_DPLL_INIT_REG_DPLL_PROP_INIT_SEL_POS                                                   (4UL)
#define CLIF_DPLL_INIT_REG_DPLL_NUM_GEARS_SEL_MASK                                                  (0x00000003UL) /**< rw - Defines the number of gears. */
#define CLIF_DPLL_INIT_REG_DPLL_NUM_GEARS_SEL_POS                                                   (0UL)
#define CLIF_DPLL_INIT_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name DPLL Configuration Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_DPLL_GEAR_REG                                                                          (0x4000420CUL) /**< rw*/
#define CLIF_DPLL_GEAR_REG_DPLL_TIME_UPDATE_SEL_MASK                                                (0x007E0000UL) /**< rw - Update of the gear duration. */
#define CLIF_DPLL_GEAR_REG_DPLL_TIME_UPDATE_SEL_POS                                                 (17UL)
#define CLIF_DPLL_GEAR_REG_DPLL_INT_UPDATE_SEL_MASK                                                 (0x00007F00UL) /**< rw - Update of the integral coefficient. */
#define CLIF_DPLL_GEAR_REG_DPLL_INT_UPDATE_SEL_POS                                                  (8UL)
#define CLIF_DPLL_GEAR_REG_DPLL_PROP_UPDATE_SEL_MASK                                                (0x0000007FUL) /**< rw - Update of the proportional coefficient. */
#define CLIF_DPLL_GEAR_REG_DPLL_PROP_UPDATE_SEL_POS                                                 (0UL)
#define CLIF_DPLL_GEAR_REG__RESET_VALUE                                                             (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name DPLL Configuration Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_DPLL_INIT_FREQ_REG                                                                     (0x40004210UL) /**< rw*/
#define CLIF_DPLL_INIT_FREQ_REG_DPLL_SATURATION_ENABLE_MASK                                         (0x80000000UL) /**< rw - If set the integrator is saturated to DPLL_CTRL +/- 0.0546875. */
#define CLIF_DPLL_INIT_FREQ_REG_DPLL_SATURATION_ENABLE_POS                                          (31UL)
#define CLIF_DPLL_INIT_FREQ_REG_DPLL_CTRL_MASK                                                      (0x0000FFFFUL) /**< rw - The initial DPLL integrator value i.e. the nominal post-divider value. Note: This value is loaded into the DPLL in the first cycle the DPLL controller enables the DPLL (refer to CLIF_DPLL_INIT_REG.SINGLE_LOOP_ALM_ENABLE). Note: It is not allowed to change this value during DPLL operation because the limits (convergence range) is derived from it. */
#define CLIF_DPLL_INIT_FREQ_REG_DPLL_CTRL_POS                                                       (0UL)
#define CLIF_DPLL_INIT_FREQ_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name BBA Control Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_BBA_CONTROL_REG                                                                        (0x40004214UL) /**< rw*/
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_FREEZE_MODE_MASK                                           (0x00000600UL) /**< rw - Selects signal to freeze the BBA switching. 0: Freeze signal based on edge detection (slow). 1: Freeze signal based on input filter (fast) using a sensitive threshold. 2: Freeze signal based on input filter (fast)using an insensitive threshold. 3: Freeze signal based on input filter (fast) using a dynamic (RSSI-based) threshold. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_FREEZE_MODE_POS                                            (9UL)
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_WATCH_DOG_ENABLE_MASK                                      (0x00000100UL) /**< rw - If set a watch-dog timer resets the modulation detection signal to avoid that the BBA gets stuck in the wrong state. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_WATCH_DOG_ENABLE_POS                                       (8UL)
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST2_DOWN_MASK                                            (0x00000080UL) /**< rw - Defines the width of the second hysteresis. 0: BBA_CTRL_HYST2_UP+2. 1: BBA_CTRL_HYST2_UP+3. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST2_DOWN_POS                                             (7UL)
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST2_UP_MASK                                              (0x00000060UL) /**< rw - Enables the block and defines the initial value of the second hysteresis. Disables (only one hysteresis is used). BBA_CTRL_HYST1_DOWN + 2. BBA_CTRL_HYST1_DOWN + 3. BBA_CTRL_HYST1_DOWN + 4. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST2_UP_POS                                               (5UL)
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST1_DOWN_MASK                                            (0x00000010UL) /**< rw - Defines the width of the first hysteresis.  0: BBA_CTRL_HYST1_UP+2. 1: BBA_CTRL_HYST1_UP+3. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST1_DOWN_POS                                             (4UL)
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST1_UP_MASK                                              (0x0000000EUL) /**< rw - Defines the initial value for the hysteresis. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_HYST1_UP_POS                                               (1UL)
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_ENABLE_MASK                                                (0x00000001UL) /**< rw - If set the BBA-Control block is enabled otherwise the gain is statically controlled by the register bit field CLIF_ANA_RX_REG.RX_GAIN. */
#define CLIF_BBA_CONTROL_REG_CM_BBA_CTRL_ENABLE_POS                                                 (0UL)
#define CLIF_BBA_CONTROL_REG__RESET_VALUE                                                           (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Register to configure/read RSSI latch
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw
* @{
*/
#define CLIF_RSSI_LATCH_REG                                                                         (0x40004218UL) /**< rw*/
#define CLIF_RSSI_LATCH_REG_TRIGGER_TX_ENABLE_MASK                                                  (0x08000000UL) /**< rw - Enable to set TX_IRQ as RSSI trigger condition. */
#define CLIF_RSSI_LATCH_REG_TRIGGER_TX_ENABLE_POS                                                   (27UL)
#define CLIF_RSSI_LATCH_REG_TRIGGER_RX_ENABLE_MASK                                                  (0x04000000UL) /**< rw - Enable to set RX_IRQ as RSSI trigger condition. */
#define CLIF_RSSI_LATCH_REG_TRIGGER_RX_ENABLE_POS                                                   (26UL)
#define CLIF_RSSI_LATCH_REG_TRIGGER_CMA_ENABLE_MASK                                                 (0x02000000UL) /**< rw - Enable to set CARD_ACTIVATED_IRQ as RSSI trigger condition. */
#define CLIF_RSSI_LATCH_REG_TRIGGER_CMA_ENABLE_POS                                                  (25UL)
#define CLIF_RSSI_LATCH_REG_TRIGGER_RFON_ENABLE_MASK                                                (0x01000000UL) /**< rw - Enable to set the level detector as RSSI trigger condition. */
#define CLIF_RSSI_LATCH_REG_TRIGGER_RFON_ENABLE_POS                                                 (24UL)
#define CLIF_RSSI_LATCH_REG_RSSI_LATCH_EVENT_MASK                                                   (0x00070000UL) /**< r- - Status indicating the trigger source of last latch event. No valid trigger occurred RSSI value invalid. RSSI-Latching was triggered by RFON-Event. RSSI-Latching was triggered by CMA-Event. RSSI-Latching was triggered by RX-Event. RSSI-Latching was triggered by TX-Event. */
#define CLIF_RSSI_LATCH_REG_RSSI_LATCH_EVENT_POS                                                    (16UL)
#define CLIF_RSSI_LATCH_REG_AGC_VALUE_LATCHED_MASK                                                  (0x0000FFC0UL) /**< r- - Value of the AGC store at last trigger. */
#define CLIF_RSSI_LATCH_REG_AGC_VALUE_LATCHED_POS                                                   (6UL)
#define CLIF_RSSI_LATCH_REG_ADC_DATA_Q_LATCHED_MASK                                                 (0x0000003FUL) /**< r- - Value of the ADC-Q channel store at last trigger. */
#define CLIF_RSSI_LATCH_REG_ADC_DATA_Q_LATCHED_POS                                                  (0UL)
#define CLIF_RSSI_LATCH_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w
* @{
*/
#define CLIF_INT_CLR_ENABLE_REG                                                                     (0x40007FD8UL) /**< -w*/
#define CLIF_INT_CLR_ENABLE_REG_AGC_RFOFF_DET_IRQ_CLR_ENABLE_MASK                                   (0x20000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_AGC_RFOFF_DET_IRQ_CLR_ENABLE_POS                                    (29UL)
#define CLIF_INT_CLR_ENABLE_REG_TX_DATA_REQ_IRQ_CLR_ENABLE_MASK                                     (0x10000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TX_DATA_REQ_IRQ_CLR_ENABLE_POS                                      (28UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_DATA_AV_IRQ_CLR_ENABLE_MASK                                      (0x08000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RX_DATA_AV_IRQ_CLR_ENABLE_POS                                       (27UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_BUFFER_OVERFLOW_IRQ_CLR_ENABLE_MASK                              (0x04000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RX_BUFFER_OVERFLOW_IRQ_CLR_ENABLE_POS                               (26UL)
#define CLIF_INT_CLR_ENABLE_REG_TX_WATERLEVEL_IRQ_CLR_ENABLE_MASK                                   (0x02000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TX_WATERLEVEL_IRQ_CLR_ENABLE_POS                                    (25UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_WATERLEVEL_IRQ_CLR_ENABLE_MASK                                   (0x01000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RX_WATERLEVEL_IRQ_CLR_ENABLE_POS                                    (24UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_SC_DET_IRQ_CLR_ENABLE_MASK                                       (0x00400000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RX_SC_DET_IRQ_CLR_ENABLE_POS                                        (22UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_SOF_DET_IRQ_CLR_ENABLE_MASK                                      (0x00200000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RX_SOF_DET_IRQ_CLR_ENABLE_POS                                       (21UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_EMD_IRQ_CLR_ENABLE_MASK                                          (0x00100000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RX_EMD_IRQ_CLR_ENABLE_POS                                           (20UL)
#define CLIF_INT_CLR_ENABLE_REG_TIMER3_IRQ_CLR_ENABLE_MASK                                          (0x00080000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TIMER3_IRQ_CLR_ENABLE_POS                                           (19UL)
#define CLIF_INT_CLR_ENABLE_REG_TIMER2_IRQ_CLR_ENABLE_MASK                                          (0x00040000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TIMER2_IRQ_CLR_ENABLE_POS                                           (18UL)
#define CLIF_INT_CLR_ENABLE_REG_TIMER1_IRQ_CLR_ENABLE_MASK                                          (0x00020000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TIMER1_IRQ_CLR_ENABLE_POS                                           (17UL)
#define CLIF_INT_CLR_ENABLE_REG_TIMER0_IRQ_CLR_ENABLE_MASK                                          (0x00010000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TIMER0_IRQ_CLR_ENABLE_POS                                           (16UL)
#define CLIF_INT_CLR_ENABLE_REG_CLOCK_ERROR_IRQ_CLR_ENABLE_MASK                                     (0x00008000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_CLOCK_ERROR_IRQ_CLR_ENABLE_POS                                      (15UL)
#define CLIF_INT_CLR_ENABLE_REG_NFCWI_RING_IRQ_CLR_ENABLE_MASK                                      (0x00004000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_NFCWI_RING_IRQ_CLR_ENABLE_POS                                       (14UL)
#define CLIF_INT_CLR_ENABLE_REG_SIGIN_ACT_IRQ_CLR_ENABLE_MASK                                       (0x00002000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_SIGIN_ACT_IRQ_CLR_ENABLE_POS                                        (13UL)
#define CLIF_INT_CLR_ENABLE_REG_RF_ACTIVE_ERROR_IRQ_CLR_ENABLE_MASK                                 (0x00001000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RF_ACTIVE_ERROR_IRQ_CLR_ENABLE_POS                                  (12UL)
#define CLIF_INT_CLR_ENABLE_REG_TX_RFON_IRQ_CLR_ENABLE_MASK                                         (0x00000800UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TX_RFON_IRQ_CLR_ENABLE_POS                                          (11UL)
#define CLIF_INT_CLR_ENABLE_REG_TX_RFOFF_IRQ_CLR_ENABLE_MASK                                        (0x00000400UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TX_RFOFF_IRQ_CLR_ENABLE_POS                                         (10UL)
#define CLIF_INT_CLR_ENABLE_REG_RFON_DET_IRQ_CLR_ENABLE_MASK                                        (0x00000200UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RFON_DET_IRQ_CLR_ENABLE_POS                                         (9UL)
#define CLIF_INT_CLR_ENABLE_REG_RFOFF_DET_IRQ_CLR_ENABLE_MASK                                       (0x00000100UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_RFOFF_DET_IRQ_CLR_ENABLE_POS                                        (8UL)
#define CLIF_INT_CLR_ENABLE_REG_STATE_CHANGE_IRQ_CLR_ENABLE_MASK                                    (0x00000020UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_STATE_CHANGE_IRQ_CLR_ENABLE_POS                                     (5UL)
#define CLIF_INT_CLR_ENABLE_REG_CARD_ACTIVATED_IRQ_CLR_ENABLE_MASK                                  (0x00000010UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_CARD_ACTIVATED_IRQ_CLR_ENABLE_POS                                   (4UL)
#define CLIF_INT_CLR_ENABLE_REG_MODE_DETECTED_IRQ_CLR_ENABLE_MASK                                   (0x00000008UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag           . */
#define CLIF_INT_CLR_ENABLE_REG_MODE_DETECTED_IRQ_CLR_ENABLE_POS                                    (3UL)
#define CLIF_INT_CLR_ENABLE_REG_IDLE_IRQ_CLR_ENABLE_MASK                                            (0x00000004UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_IDLE_IRQ_CLR_ENABLE_POS                                             (2UL)
#define CLIF_INT_CLR_ENABLE_REG_TX_IRQ_CLR_ENABLE_MASK                                              (0x00000002UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag . */
#define CLIF_INT_CLR_ENABLE_REG_TX_IRQ_CLR_ENABLE_POS                                               (1UL)
#define CLIF_INT_CLR_ENABLE_REG_RX_IRQ_CLR_ENABLE_MASK                                              (0x00000001UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ ENABLE flag. */
#define CLIF_INT_CLR_ENABLE_REG_RX_IRQ_CLR_ENABLE_POS                                               (0UL)
#define CLIF_INT_CLR_ENABLE_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w
* @{
*/
#define CLIF_INT_SET_ENABLE_REG                                                                     (0x40007FDCUL) /**< -w*/
#define CLIF_INT_SET_ENABLE_REG_AGC_RFOFF_DET_IRQ_SET_ENABLE_MASK                                   (0x20000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_AGC_RFOFF_DET_IRQ_SET_ENABLE_POS                                    (29UL)
#define CLIF_INT_SET_ENABLE_REG_TX_DATA_REQ_IRQ_SET_ENABLE_MASK                                     (0x10000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TX_DATA_REQ_IRQ_SET_ENABLE_POS                                      (28UL)
#define CLIF_INT_SET_ENABLE_REG_RX_DATA_AV_IRQ_SET_ENABLE_MASK                                      (0x08000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RX_DATA_AV_IRQ_SET_ENABLE_POS                                       (27UL)
#define CLIF_INT_SET_ENABLE_REG_RX_BUFFER_OVERFLOW_IRQ_SET_ENABLE_MASK                              (0x04000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RX_BUFFER_OVERFLOW_IRQ_SET_ENABLE_POS                               (26UL)
#define CLIF_INT_SET_ENABLE_REG_TX_WATERLEVEL_IRQ_SET_ENABLE_MASK                                   (0x02000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TX_WATERLEVEL_IRQ_SET_ENABLE_POS                                    (25UL)
#define CLIF_INT_SET_ENABLE_REG_RX_WATERLEVEL_IRQ_SET_ENABLE_MASK                                   (0x01000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RX_WATERLEVEL_IRQ_SET_ENABLE_POS                                    (24UL)
#define CLIF_INT_SET_ENABLE_REG_RX_SC_DET_IRQ_SET_ENABLE_MASK                                       (0x00400000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RX_SC_DET_IRQ_SET_ENABLE_POS                                        (22UL)
#define CLIF_INT_SET_ENABLE_REG_RX_SOF_DET_IRQ_SET_ENABLE_MASK                                      (0x00200000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag        . */
#define CLIF_INT_SET_ENABLE_REG_RX_SOF_DET_IRQ_SET_ENABLE_POS                                       (21UL)
#define CLIF_INT_SET_ENABLE_REG_RX_EMD_IRQ_SET_ENABLE_MASK                                          (0x00100000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RX_EMD_IRQ_SET_ENABLE_POS                                           (20UL)
#define CLIF_INT_SET_ENABLE_REG_TIMER3_IRQ_SET_ENABLE_MASK                                          (0x00080000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TIMER3_IRQ_SET_ENABLE_POS                                           (19UL)
#define CLIF_INT_SET_ENABLE_REG_TIMER2_IRQ_SET_ENABLE_MASK                                          (0x00040000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TIMER2_IRQ_SET_ENABLE_POS                                           (18UL)
#define CLIF_INT_SET_ENABLE_REG_TIMER1_IRQ_SET_ENABLE_MASK                                          (0x00020000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TIMER1_IRQ_SET_ENABLE_POS                                           (17UL)
#define CLIF_INT_SET_ENABLE_REG_TIMER0_IRQ_SET_ENABLE_MASK                                          (0x00010000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TIMER0_IRQ_SET_ENABLE_POS                                           (16UL)
#define CLIF_INT_SET_ENABLE_REG_CLOCK_ERROR_IRQ_SET_ENABLE_MASK                                     (0x00008000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_CLOCK_ERROR_IRQ_SET_ENABLE_POS                                      (15UL)
#define CLIF_INT_SET_ENABLE_REG_NFCWI_RING_IRQ_SET_ENABLE_MASK                                      (0x00004000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_NFCWI_RING_IRQ_SET_ENABLE_POS                                       (14UL)
#define CLIF_INT_SET_ENABLE_REG_SIGIN_ACT_IRQ_SET_ENABLE_MASK                                       (0x00002000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_SIGIN_ACT_IRQ_SET_ENABLE_POS                                        (13UL)
#define CLIF_INT_SET_ENABLE_REG_RF_ACTIVE_ERROR_IRQ_SET_ENABLE_MASK                                 (0x00001000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RF_ACTIVE_ERROR_IRQ_SET_ENABLE_POS                                  (12UL)
#define CLIF_INT_SET_ENABLE_REG_TX_RFON_IRQ_SET_ENABLE_MASK                                         (0x00000800UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TX_RFON_IRQ_SET_ENABLE_POS                                          (11UL)
#define CLIF_INT_SET_ENABLE_REG_TX_RFOFF_IRQ_SET_ENABLE_MASK                                        (0x00000400UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TX_RFOFF_IRQ_SET_ENABLE_POS                                         (10UL)
#define CLIF_INT_SET_ENABLE_REG_RFON_DET_IRQ_SET_ENABLE_MASK                                        (0x00000200UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RFON_DET_IRQ_SET_ENABLE_POS                                         (9UL)
#define CLIF_INT_SET_ENABLE_REG_RFOFF_DET_IRQ_SET_ENABLE_MASK                                       (0x00000100UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_RFOFF_DET_IRQ_SET_ENABLE_POS                                        (8UL)
#define CLIF_INT_SET_ENABLE_REG_STATE_CHANGE_IRQ_SET_ENABLE_MASK                                    (0x00000020UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_STATE_CHANGE_IRQ_SET_ENABLE_POS                                     (5UL)
#define CLIF_INT_SET_ENABLE_REG_CARD_ACTIVATED_IRQ_SET_ENABLE_MASK                                  (0x00000010UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_CARD_ACTIVATED_IRQ_SET_ENABLE_POS                                   (4UL)
#define CLIF_INT_SET_ENABLE_REG_MODE_DETECTED_IRQ_SET_ENABLE_MASK                                   (0x00000008UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_MODE_DETECTED_IRQ_SET_ENABLE_POS                                    (3UL)
#define CLIF_INT_SET_ENABLE_REG_IDLE_IRQ_SET_ENABLE_MASK                                            (0x00000004UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_IDLE_IRQ_SET_ENABLE_POS                                             (2UL)
#define CLIF_INT_SET_ENABLE_REG_TX_IRQ_SET_ENABLE_MASK                                              (0x00000002UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag . */
#define CLIF_INT_SET_ENABLE_REG_TX_IRQ_SET_ENABLE_POS                                               (1UL)
#define CLIF_INT_SET_ENABLE_REG_RX_IRQ_SET_ENABLE_MASK                                              (0x00000001UL) /**< -x - Writing 1 to this register does set the corresponding IRQ ENABLE flag. */
#define CLIF_INT_SET_ENABLE_REG_RX_IRQ_SET_ENABLE_POS                                               (0UL)
#define CLIF_INT_SET_ENABLE_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_INT_STATUS_REG                                                                         (0x40007FE0UL) /**< r-*/
#define CLIF_INT_STATUS_REG_AGC_RFOFF_DET_IRQ_MASK                                                  (0x20000000UL) /**< r- - Set to 1 by HW when the AGC has detected the external RF-Field was switched off while transmitting in SL-ALM mode. Note: Only valid if the detection mode is enabled with the register bit-field AGC_RF_DETECT_SEL. */
#define CLIF_INT_STATUS_REG_AGC_RFOFF_DET_IRQ_POS                                                   (29UL)
#define CLIF_INT_STATUS_REG_TX_DATA_REQ_IRQ_MASK                                                    (0x10000000UL) /**< r- - Set to 1 by HW when the BufferManager requests data for transmission from RAM. Note: Only valid if the bit DIRECT_DATA_ACCESS_ENABLE is set to 1. */
#define CLIF_INT_STATUS_REG_TX_DATA_REQ_IRQ_POS                                                     (28UL)
#define CLIF_INT_STATUS_REG_RX_DATA_AV_IRQ_MASK                                                     (0x08000000UL) /**< r- - Set to 1 by HW when the BufferManager holds received data from reception to be written to RAM. Note: Only valid if the bit DIRECT_DATA_ACCESS_ENABLE is set to 1. */
#define CLIF_INT_STATUS_REG_RX_DATA_AV_IRQ_POS                                                      (27UL)
#define CLIF_INT_STATUS_REG_RX_BUFFER_OVERFLOW_IRQ_MASK                                             (0x04000000UL) /**< r- - Set to 1 by HW when the number of bytes received exceds the size of the RX buffer. Note: Reception is stoped in that case. Note: If RX_MULTIPLE is set to 1 this an this IRQ is raised when the sum of all frames exceed the RX buffer size . */
#define CLIF_INT_STATUS_REG_RX_BUFFER_OVERFLOW_IRQ_POS                                              (26UL)
#define CLIF_INT_STATUS_REG_TX_WATERLEVEL_IRQ_MASK                                                  (0x02000000UL) /**< r- - Set to 1 by HW when the number of bytes transmitted is equal to the TX_WATERLEVEL . */
#define CLIF_INT_STATUS_REG_TX_WATERLEVEL_IRQ_POS                                                   (25UL)
#define CLIF_INT_STATUS_REG_RX_WATERLEVEL_IRQ_MASK                                                  (0x01000000UL) /**< r- - Set to 1 by HW when the number of bytes received is equal to the RX_WATERLEVEL . */
#define CLIF_INT_STATUS_REG_RX_WATERLEVEL_IRQ_POS                                                   (24UL)
#define CLIF_INT_STATUS_REG_RX_SC_DET_IRQ_MASK                                                      (0x00400000UL) /**< r- - Set to 1 by HW when in reader mode a subcarrier is detected . */
#define CLIF_INT_STATUS_REG_RX_SC_DET_IRQ_POS                                                       (22UL)
#define CLIF_INT_STATUS_REG_RX_SOF_DET_IRQ_MASK                                                     (0x00200000UL) /**< r- - Set to 1 by HW when in reader mode an SOF is detected . */
#define CLIF_INT_STATUS_REG_RX_SOF_DET_IRQ_POS                                                      (21UL)
#define CLIF_INT_STATUS_REG_RX_EMD_IRQ_MASK                                                         (0x00100000UL) /**< r- - Set to 1 by HW when an EMD event is detected . */
#define CLIF_INT_STATUS_REG_RX_EMD_IRQ_POS                                                          (20UL)
#define CLIF_INT_STATUS_REG_TIMER3_IRQ_MASK                                                         (0x00080000UL) /**< r- - Set to 1 by HW when the Timer3 is expired. . */
#define CLIF_INT_STATUS_REG_TIMER3_IRQ_POS                                                          (19UL)
#define CLIF_INT_STATUS_REG_TIMER2_IRQ_MASK                                                         (0x00040000UL) /**< r- - Set to 1 by HW when the Timer2 is expired. . */
#define CLIF_INT_STATUS_REG_TIMER2_IRQ_POS                                                          (18UL)
#define CLIF_INT_STATUS_REG_TIMER1_IRQ_MASK                                                         (0x00020000UL) /**< r- - Set to 1 by HW when the Timer1 is expired. . */
#define CLIF_INT_STATUS_REG_TIMER1_IRQ_POS                                                          (17UL)
#define CLIF_INT_STATUS_REG_TIMER0_IRQ_MASK                                                         (0x00010000UL) /**< r- - Set to 1 by HW when the Timer0 is expired.        . */
#define CLIF_INT_STATUS_REG_TIMER0_IRQ_POS                                                          (16UL)
#define CLIF_INT_STATUS_REG_CLOCK_ERROR_IRQ_MASK                                                    (0x00008000UL) /**< r- - Set to 1 by HW when RF-Field vanished (and consequently the RF-Clock is not present) and the clock of the system PLL is not available. */
#define CLIF_INT_STATUS_REG_CLOCK_ERROR_IRQ_POS                                                     (15UL)
#define CLIF_INT_STATUS_REG_NFCWI_RING_IRQ_MASK                                                     (0x00004000UL) /**< r- - Set to 1 by HW when a ring indicator was detected on the SigIn pin. . */
#define CLIF_INT_STATUS_REG_NFCWI_RING_IRQ_POS                                                      (14UL)
#define CLIF_INT_STATUS_REG_SIGIN_ACT_IRQ_MASK                                                      (0x00002000UL) /**< r- - Set to 1 by HW when activity on the SigIn pin is detected. . */
#define CLIF_INT_STATUS_REG_SIGIN_ACT_IRQ_POS                                                       (13UL)
#define CLIF_INT_STATUS_REG_RF_ACTIVE_ERROR_IRQ_MASK                                                (0x00001000UL) /**< r- - Set to 1 by HW when an RF error case occurred . */
#define CLIF_INT_STATUS_REG_RF_ACTIVE_ERROR_IRQ_POS                                                 (12UL)
#define CLIF_INT_STATUS_REG_TX_RFON_IRQ_MASK                                                        (0x00000800UL) /**< r- - Set to 1 by HW when the internally generated RF-field was switched on. . */
#define CLIF_INT_STATUS_REG_TX_RFON_IRQ_POS                                                         (11UL)
#define CLIF_INT_STATUS_REG_TX_RFOFF_IRQ_MASK                                                       (0x00000400UL) /**< r- - Set to 1 by HW when the internally generated RF-field was switched off. . */
#define CLIF_INT_STATUS_REG_TX_RFOFF_IRQ_POS                                                        (10UL)
#define CLIF_INT_STATUS_REG_RFON_DET_IRQ_MASK                                                       (0x00000200UL) /**< r- - Set to 1 by HW when an external RF-field is detected. . */
#define CLIF_INT_STATUS_REG_RFON_DET_IRQ_POS                                                        (9UL)
#define CLIF_INT_STATUS_REG_RFOFF_DET_IRQ_MASK                                                      (0x00000100UL) /**< r- - Set to 1 by HW when an until then preset external RF-field is switched off. . */
#define CLIF_INT_STATUS_REG_RFOFF_DET_IRQ_POS                                                       (8UL)
#define CLIF_INT_STATUS_REG_STATE_CHANGE_IRQ_MASK                                                   (0x00000020UL) /**< r- - Set to 1 by HW when a transceive state is entered selected in the register field STATE_TRIGGER_SELECT . */
#define CLIF_INT_STATUS_REG_STATE_CHANGE_IRQ_POS                                                    (5UL)
#define CLIF_INT_STATUS_REG_CARD_ACTIVATED_IRQ_MASK                                                 (0x00000010UL) /**< r- - Set to 1 by HW when TypeA card mode activation FSM reached the ACTIVATED or ACTIVATE_S state . */
#define CLIF_INT_STATUS_REG_CARD_ACTIVATED_IRQ_POS                                                  (4UL)
#define CLIF_INT_STATUS_REG_MODE_DETECTED_IRQ_MASK                                                  (0x00000008UL) /**< r- - Set to 1 by HW when the card mode has been detected by the ModeDetector Note: While the TypeA activation FSM is active no IRQ is issued any more . */
#define CLIF_INT_STATUS_REG_MODE_DETECTED_IRQ_POS                                                   (3UL)
#define CLIF_INT_STATUS_REG_IDLE_IRQ_MASK                                                           (0x00000004UL) /**< r- - Set to 1 by HW when the IDLE state is entered . */
#define CLIF_INT_STATUS_REG_IDLE_IRQ_POS                                                            (2UL)
#define CLIF_INT_STATUS_REG_TX_IRQ_MASK                                                             (0x00000002UL) /**< r- - Set to 1 by HW when an ongoing transmission is finished. . */
#define CLIF_INT_STATUS_REG_TX_IRQ_POS                                                              (1UL)
#define CLIF_INT_STATUS_REG_RX_IRQ_MASK                                                             (0x00000001UL) /**< r- - Set to 1 by HW when an ongoing reception is finished. */
#define CLIF_INT_STATUS_REG_RX_IRQ_POS                                                              (0UL)
#define CLIF_INT_STATUS_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-
* @{
*/
#define CLIF_INT_ENABLE_REG                                                                         (0x40007FE4UL) /**< r-*/
#define CLIF_INT_ENABLE_REG_AGC_RFOFF_DET_IRQ_ENABLE_MASK                                           (0x20000000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_AGC_RFOFF_DET_IRQ_ENABLE_POS                                            (29UL)
#define CLIF_INT_ENABLE_REG_TX_DATA_REQ_IRQ_ENABLE_MASK                                             (0x10000000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TX_DATA_REQ_IRQ_ENABLE_POS                                              (28UL)
#define CLIF_INT_ENABLE_REG_RX_DATA_AV_IRQ_ENABLE_MASK                                              (0x08000000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RX_DATA_AV_IRQ_ENABLE_POS                                               (27UL)
#define CLIF_INT_ENABLE_REG_RX_BUFFER_OVERFLOW_IRQ_ENABLE_MASK                                      (0x04000000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RX_BUFFER_OVERFLOW_IRQ_ENABLE_POS                                       (26UL)
#define CLIF_INT_ENABLE_REG_TX_WATERLEVEL_IRQ_ENABLE_MASK                                           (0x02000000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TX_WATERLEVEL_IRQ_ENABLE_POS                                            (25UL)
#define CLIF_INT_ENABLE_REG_RX_WATERLEVEL_IRQ_ENABLE_MASK                                           (0x01000000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RX_WATERLEVEL_IRQ_ENABLE_POS                                            (24UL)
#define CLIF_INT_ENABLE_REG_RX_SC_DET_IRQ_ENABLE_MASK                                               (0x00400000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RX_SC_DET_IRQ_ENABLE_POS                                                (22UL)
#define CLIF_INT_ENABLE_REG_RX_SOF_DET_IRQ_ENABLE_MASK                                              (0x00200000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RX_SOF_DET_IRQ_ENABLE_POS                                               (21UL)
#define CLIF_INT_ENABLE_REG_RX_EMD_IRQ_ENABLE_MASK                                                  (0x00100000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RX_EMD_IRQ_ENABLE_POS                                                   (20UL)
#define CLIF_INT_ENABLE_REG_TIMER3_IRQ_ENABLE_MASK                                                  (0x00080000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TIMER3_IRQ_ENABLE_POS                                                   (19UL)
#define CLIF_INT_ENABLE_REG_TIMER2_IRQ_ENABLE_MASK                                                  (0x00040000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TIMER2_IRQ_ENABLE_POS                                                   (18UL)
#define CLIF_INT_ENABLE_REG_TIMER1_IRQ_ENABLE_MASK                                                  (0x00020000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TIMER1_IRQ_ENABLE_POS                                                   (17UL)
#define CLIF_INT_ENABLE_REG_TIMER0_IRQ_ENABLE_MASK                                                  (0x00010000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TIMER0_IRQ_ENABLE_POS                                                   (16UL)
#define CLIF_INT_ENABLE_REG_CLOCK_ERROR_IRQ_ENABLE_MASK                                             (0x00008000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_CLOCK_ERROR_IRQ_ENABLE_POS                                              (15UL)
#define CLIF_INT_ENABLE_REG_NFCWI_RING_IRQ_ENABLE_MASK                                              (0x00004000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_NFCWI_RING_IRQ_ENABLE_POS                                               (14UL)
#define CLIF_INT_ENABLE_REG_SIGIN_ACT_IRQ_ENABLE_MASK                                               (0x00002000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_SIGIN_ACT_IRQ_ENABLE_POS                                                (13UL)
#define CLIF_INT_ENABLE_REG_RF_ACTIVE_ERROR_IRQ_ENABLE_MASK                                         (0x00001000UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RF_ACTIVE_ERROR_IRQ_ENABLE_POS                                          (12UL)
#define CLIF_INT_ENABLE_REG_TX_RFON_IRQ_ENABLE_MASK                                                 (0x00000800UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TX_RFON_IRQ_ENABLE_POS                                                  (11UL)
#define CLIF_INT_ENABLE_REG_TX_RFOFF_IRQ_ENABLE_MASK                                                (0x00000400UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller        . */
#define CLIF_INT_ENABLE_REG_TX_RFOFF_IRQ_ENABLE_POS                                                 (10UL)
#define CLIF_INT_ENABLE_REG_RFON_DET_IRQ_ENABLE_MASK                                                (0x00000200UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RFON_DET_IRQ_ENABLE_POS                                                 (9UL)
#define CLIF_INT_ENABLE_REG_RFOFF_DET_IRQ_ENABLE_MASK                                               (0x00000100UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_RFOFF_DET_IRQ_ENABLE_POS                                                (8UL)
#define CLIF_INT_ENABLE_REG_STATE_CHANGE_IRQ_ENABLE_MASK                                            (0x00000020UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_STATE_CHANGE_IRQ_ENABLE_POS                                             (5UL)
#define CLIF_INT_ENABLE_REG_CARD_ACTIVATED_IRQ_ENABLE_MASK                                          (0x00000010UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_CARD_ACTIVATED_IRQ_ENABLE_POS                                           (4UL)
#define CLIF_INT_ENABLE_REG_MODE_DETECTED_IRQ_ENABLE_MASK                                           (0x00000008UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_MODE_DETECTED_IRQ_ENABLE_POS                                            (3UL)
#define CLIF_INT_ENABLE_REG_IDLE_IRQ_ENABLE_MASK                                                    (0x00000004UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_IDLE_IRQ_ENABLE_POS                                                     (2UL)
#define CLIF_INT_ENABLE_REG_TX_IRQ_ENABLE_MASK                                                      (0x00000002UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller . */
#define CLIF_INT_ENABLE_REG_TX_IRQ_ENABLE_POS                                                       (1UL)
#define CLIF_INT_ENABLE_REG_RX_IRQ_ENABLE_MASK                                                      (0x00000001UL) /**< r- - If this bit is 1 the corresponding IRQ can propergate to the CPUs IRQ controller. */
#define CLIF_INT_ENABLE_REG_RX_IRQ_ENABLE_POS                                                       (0UL)
#define CLIF_INT_ENABLE_REG__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w
* @{
*/
#define CLIF_INT_CLR_STATUS_REG                                                                     (0x40007FE8UL) /**< -w*/
#define CLIF_INT_CLR_STATUS_REG_AGC_RFOFF_DET_IRQ_CLR_STATUS_MASK                                   (0x20000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_AGC_RFOFF_DET_IRQ_CLR_STATUS_POS                                    (29UL)
#define CLIF_INT_CLR_STATUS_REG_TX_DATA_REQ_IRQ_CLR_STATUS_MASK                                     (0x10000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TX_DATA_REQ_IRQ_CLR_STATUS_POS                                      (28UL)
#define CLIF_INT_CLR_STATUS_REG_RX_DATA_AV_IRQ_CLR_STATUS_MASK                                      (0x08000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RX_DATA_AV_IRQ_CLR_STATUS_POS                                       (27UL)
#define CLIF_INT_CLR_STATUS_REG_RX_BUFFER_OVERFLOW_IRQ_CLR_STATUS_MASK                              (0x04000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RX_BUFFER_OVERFLOW_IRQ_CLR_STATUS_POS                               (26UL)
#define CLIF_INT_CLR_STATUS_REG_TX_WATERLEVEL_IRQ_CLR_STATUS_MASK                                   (0x02000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TX_WATERLEVEL_IRQ_CLR_STATUS_POS                                    (25UL)
#define CLIF_INT_CLR_STATUS_REG_RX_WATERLEVEL_IRQ_CLR_STATUS_MASK                                   (0x01000000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RX_WATERLEVEL_IRQ_CLR_STATUS_POS                                    (24UL)
#define CLIF_INT_CLR_STATUS_REG_RX_SC_DET_IRQ_CLR_STATUS_MASK                                       (0x00400000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RX_SC_DET_IRQ_CLR_STATUS_POS                                        (22UL)
#define CLIF_INT_CLR_STATUS_REG_RX_SOF_DET_IRQ_CLR_STATUS_MASK                                      (0x00200000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RX_SOF_DET_IRQ_CLR_STATUS_POS                                       (21UL)
#define CLIF_INT_CLR_STATUS_REG_RX_EMD_IRQ_CLR_STATUS_MASK                                          (0x00100000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RX_EMD_IRQ_CLR_STATUS_POS                                           (20UL)
#define CLIF_INT_CLR_STATUS_REG_TIMER3_IRQ_CLR_STATUS_MASK                                          (0x00080000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TIMER3_IRQ_CLR_STATUS_POS                                           (19UL)
#define CLIF_INT_CLR_STATUS_REG_TIMER2_IRQ_CLR_STATUS_MASK                                          (0x00040000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TIMER2_IRQ_CLR_STATUS_POS                                           (18UL)
#define CLIF_INT_CLR_STATUS_REG_TIMER1_IRQ_CLR_STATUS_MASK                                          (0x00020000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TIMER1_IRQ_CLR_STATUS_POS                                           (17UL)
#define CLIF_INT_CLR_STATUS_REG_TIMER0_IRQ_CLR_STATUS_MASK                                          (0x00010000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TIMER0_IRQ_CLR_STATUS_POS                                           (16UL)
#define CLIF_INT_CLR_STATUS_REG_CLOCK_ERROR_IRQ_CLR_STATUS_MASK                                     (0x00008000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_CLOCK_ERROR_IRQ_CLR_STATUS_POS                                      (15UL)
#define CLIF_INT_CLR_STATUS_REG_NFCWI_RING_IRQ_CLR_STATUS_MASK                                      (0x00004000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_NFCWI_RING_IRQ_CLR_STATUS_POS                                       (14UL)
#define CLIF_INT_CLR_STATUS_REG_SIGIN_ACT_IRQ_CLR_STATUS_MASK                                       (0x00002000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_SIGIN_ACT_IRQ_CLR_STATUS_POS                                        (13UL)
#define CLIF_INT_CLR_STATUS_REG_RF_ACTIVE_ERROR_IRQ_CLR_STATUS_MASK                                 (0x00001000UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RF_ACTIVE_ERROR_IRQ_CLR_STATUS_POS                                  (12UL)
#define CLIF_INT_CLR_STATUS_REG_TX_RFON_IRQ_CLR_STATUS_MASK                                         (0x00000800UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TX_RFON_IRQ_CLR_STATUS_POS                                          (11UL)
#define CLIF_INT_CLR_STATUS_REG_TX_RFOFF_IRQ_CLR_STATUS_MASK                                        (0x00000400UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TX_RFOFF_IRQ_CLR_STATUS_POS                                         (10UL)
#define CLIF_INT_CLR_STATUS_REG_RFON_DET_IRQ_CLR_STATUS_MASK                                        (0x00000200UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RFON_DET_IRQ_CLR_STATUS_POS                                         (9UL)
#define CLIF_INT_CLR_STATUS_REG_RFOFF_DET_IRQ_CLR_STATUS_MASK                                       (0x00000100UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_RFOFF_DET_IRQ_CLR_STATUS_POS                                        (8UL)
#define CLIF_INT_CLR_STATUS_REG_STATE_CHANGE_IRQ_CLR_STATUS_MASK                                    (0x00000020UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_STATE_CHANGE_IRQ_CLR_STATUS_POS                                     (5UL)
#define CLIF_INT_CLR_STATUS_REG_CARD_ACTIVATED_IRQ_CLR_STATUS_MASK                                  (0x00000010UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_CARD_ACTIVATED_IRQ_CLR_STATUS_POS                                   (4UL)
#define CLIF_INT_CLR_STATUS_REG_MODE_DETECTED_IRQ_CLR_STATUS_MASK                                   (0x00000008UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag        . */
#define CLIF_INT_CLR_STATUS_REG_MODE_DETECTED_IRQ_CLR_STATUS_POS                                    (3UL)
#define CLIF_INT_CLR_STATUS_REG_IDLE_IRQ_CLR_STATUS_MASK                                            (0x00000004UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_IDLE_IRQ_CLR_STATUS_POS                                             (2UL)
#define CLIF_INT_CLR_STATUS_REG_TX_IRQ_CLR_STATUS_MASK                                              (0x00000002UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag . */
#define CLIF_INT_CLR_STATUS_REG_TX_IRQ_CLR_STATUS_POS                                               (1UL)
#define CLIF_INT_CLR_STATUS_REG_RX_IRQ_CLR_STATUS_MASK                                              (0x00000001UL) /**< -x - Writing 1 to this register does clear the corresponding IRQ STATUS flag. */
#define CLIF_INT_CLR_STATUS_REG_RX_IRQ_CLR_STATUS_POS                                               (0UL)
#define CLIF_INT_CLR_STATUS_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Interrupt register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w
* @{
*/
#define CLIF_INT_SET_STATUS_REG                                                                     (0x40007FECUL) /**< -w*/
#define CLIF_INT_SET_STATUS_REG_AGC_RFOFF_DET_IRQ_SET_STATUS_MASK                                   (0x20000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_AGC_RFOFF_DET_IRQ_SET_STATUS_POS                                    (29UL)
#define CLIF_INT_SET_STATUS_REG_TX_DATA_REQ_IRQ_SET_STATUS_MASK                                     (0x10000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TX_DATA_REQ_IRQ_SET_STATUS_POS                                      (28UL)
#define CLIF_INT_SET_STATUS_REG_RX_DATA_AV_IRQ_SET_STATUS_MASK                                      (0x08000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_DATA_AV_IRQ_SET_STATUS_POS                                       (27UL)
#define CLIF_INT_SET_STATUS_REG_RX_BUFFER_OVERFLOW_IRQ_SET_STATUS_MASK                              (0x04000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_BUFFER_OVERFLOW_IRQ_SET_STATUS_POS                               (26UL)
#define CLIF_INT_SET_STATUS_REG_TX_WATERLEVEL_IRQ_SET_STATUS_MASK                                   (0x02000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TX_WATERLEVEL_IRQ_SET_STATUS_POS                                    (25UL)
#define CLIF_INT_SET_STATUS_REG_RX_WATERLEVEL_IRQ_SET_STATUS_MASK                                   (0x01000000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_WATERLEVEL_IRQ_SET_STATUS_POS                                    (24UL)
#define CLIF_INT_SET_STATUS_REG_RX_SC_DET_IRQ_SET_STATUS_MASK                                       (0x00400000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_SC_DET_IRQ_SET_STATUS_POS                                        (22UL)
#define CLIF_INT_SET_STATUS_REG_RX_SOF_DET_IRQ_SET_STATUS_MASK                                      (0x00200000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_SOF_DET_IRQ_SET_STATUS_POS                                       (21UL)
#define CLIF_INT_SET_STATUS_REG_RX_EMD_IRQ_SET_STATUS_MASK                                          (0x00100000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_EMD_IRQ_SET_STATUS_POS                                           (20UL)
#define CLIF_INT_SET_STATUS_REG_TIMER3_IRQ_SET_STATUS_MASK                                          (0x00080000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag. */
#define CLIF_INT_SET_STATUS_REG_TIMER3_IRQ_SET_STATUS_POS                                           (19UL)
#define CLIF_INT_SET_STATUS_REG_TIMER2_IRQ_SET_STATUS_MASK                                          (0x00040000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TIMER2_IRQ_SET_STATUS_POS                                           (18UL)
#define CLIF_INT_SET_STATUS_REG_TIMER1_IRQ_SET_STATUS_MASK                                          (0x00020000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TIMER1_IRQ_SET_STATUS_POS                                           (17UL)
#define CLIF_INT_SET_STATUS_REG_TIMER0_IRQ_SET_STATUS_MASK                                          (0x00010000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TIMER0_IRQ_SET_STATUS_POS                                           (16UL)
#define CLIF_INT_SET_STATUS_REG_CLOCK_ERROR_IRQ_SET_STATUS_MASK                                     (0x00008000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_CLOCK_ERROR_IRQ_SET_STATUS_POS                                      (15UL)
#define CLIF_INT_SET_STATUS_REG_NFCWI_RING_IRQ_SET_STATUS_MASK                                      (0x00004000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_NFCWI_RING_IRQ_SET_STATUS_POS                                       (14UL)
#define CLIF_INT_SET_STATUS_REG_SIGIN_ACT_IRQ_SET_STATUS_MASK                                       (0x00002000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_SIGIN_ACT_IRQ_SET_STATUS_POS                                        (13UL)
#define CLIF_INT_SET_STATUS_REG_RF_ACTIVE_ERROR_IRQ_SET_STATUS_MASK                                 (0x00001000UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RF_ACTIVE_ERROR_IRQ_SET_STATUS_POS                                  (12UL)
#define CLIF_INT_SET_STATUS_REG_TX_RFON_IRQ_SET_STATUS_MASK                                         (0x00000800UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TX_RFON_IRQ_SET_STATUS_POS                                          (11UL)
#define CLIF_INT_SET_STATUS_REG_TX_RFOFF_IRQ_SET_STATUS_MASK                                        (0x00000400UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TX_RFOFF_IRQ_SET_STATUS_POS                                         (10UL)
#define CLIF_INT_SET_STATUS_REG_RFON_DET_IRQ_SET_STATUS_MASK                                        (0x00000200UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RFON_DET_IRQ_SET_STATUS_POS                                         (9UL)
#define CLIF_INT_SET_STATUS_REG_RFOFF_DET_IRQ_SET_STATUS_MASK                                       (0x00000100UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RFOFF_DET_IRQ_SET_STATUS_POS                                        (8UL)
#define CLIF_INT_SET_STATUS_REG_STATE_CHANGE_IRQ_SET_STATUS_MASK                                    (0x00000020UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_STATE_CHANGE_IRQ_SET_STATUS_POS                                     (5UL)
#define CLIF_INT_SET_STATUS_REG_CARD_ACTIVATED_IRQ_SET_STATUS_MASK                                  (0x00000010UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_CARD_ACTIVATED_IRQ_SET_STATUS_POS                                   (4UL)
#define CLIF_INT_SET_STATUS_REG_MODE_DETECTED_IRQ_SET_STATUS_MASK                                   (0x00000008UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_MODE_DETECTED_IRQ_SET_STATUS_POS                                    (3UL)
#define CLIF_INT_SET_STATUS_REG_IDLE_IRQ_SET_STATUS_MASK                                            (0x00000004UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_IDLE_IRQ_SET_STATUS_POS                                             (2UL)
#define CLIF_INT_SET_STATUS_REG_TX_IRQ_SET_STATUS_MASK                                              (0x00000002UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_TX_IRQ_SET_STATUS_POS                                               (1UL)
#define CLIF_INT_SET_STATUS_REG_RX_IRQ_SET_STATUS_MASK                                              (0x00000001UL) /**< -x - Writing 1 to this register does set the corresponding IRQ STATUS flag . */
#define CLIF_INT_SET_STATUS_REG_RX_IRQ_SET_STATUS_POS                                               (0UL)
#define CLIF_INT_SET_STATUS_REG__RESET_VALUE                                                        (0x00000000UL) /**< value after reset */
/**
* @}
*/

#endif /* PN7462AU_IP_CLIF_H_INCLUDED */
