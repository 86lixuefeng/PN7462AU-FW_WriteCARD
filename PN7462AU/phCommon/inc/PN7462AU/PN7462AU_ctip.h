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
#ifndef PN7462AU_IP_CTIP_H_INCLUDED
#define PN7462AU_IP_CTIP_H_INCLUDED

#include "PN7462AU/PN7462AU_ctip_rorw.h"



/**
* \name Slot Select Register
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define CT_SSR_REG                                                                                  (0x40014000UL) /**< rw-*/
#define CT_SSR_REG_PRES_CON_NO_MASK                                                                 (0x00000010UL) /**< rw - PRESN Connector Normally Open. */
#define CT_SSR_REG_PRES_CON_NO_POS                                                                  (4UL)
#define CT_SSR_REG_PRES_PUP_EN_MASK                                                                 (0x00000008UL) /**< rw - PRESN internal Pull Up Enable. */
#define CT_SSR_REG_PRES_PUP_EN_POS                                                                  (3UL)
#define CT_SSR_REG_CLKAUXEN_MASK                                                                    (0x00000004UL) /**< rw - CLKAUX clock enable signal. */
#define CT_SSR_REG_CLKAUXEN_POS                                                                     (2UL)
#define CT_SSR_REG_IOAUXEN_MASK                                                                     (0x00000002UL) /**< rw - Select the second (auxiliary) slot. */
#define CT_SSR_REG_IOAUXEN_POS                                                                      (1UL)
#define CT_SSR_REG_NOT_SOFTRESET_MASK                                                               (0x00000001UL) /**< rw - Software reset. */
#define CT_SSR_REG_NOT_SOFTRESET_POS                                                                (0UL)
#define CT_SSR_REG__RESET_VALUE                                                                     (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name Programmable Divider Register Least Significant Byte slot x (PDR LSB register of main slot accessed when IOAUXen = 0, else PDR LSB register of auxiliary slot accessed)
* <b>Reset value:</b> 0x00000074\n
* <b>Access:</b> rw-
* @{
*/
#define CT_PDRx_LSB_REG                                                                             (0x40014004UL) /**< rw-*/
#define CT_PDRx_LSB_REG_PD7_PD0_MASK                                                                (0x000000FFUL) /**< rw - Programmable Divider Least Significant Byte. */
#define CT_PDRx_LSB_REG_PD7_PD0_POS                                                                 (0UL)
#define CT_PDRx_LSB_REG__RESET_VALUE                                                                (0x00000074UL) /**< value after reset */
/**
* @}
*/


/**
* \name Programmable Divider Register Most Significant Byte slot x (PDR MSB register of main slot accessed when IOAUXen = 0, else PDR MSB register of auxiliary slot accessed)
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define CT_PDRx_MSB_REG                                                                             (0x40014008UL) /**< rw-*/
#define CT_PDRx_MSB_REG_PD15_PD8_MASK                                                               (0x000000FFUL) /**< rw - Programmable Divider Most Significant Byte. */
#define CT_PDRx_MSB_REG_PD15_PD8_POS                                                                (0UL)
#define CT_PDRx_MSB_REG__RESET_VALUE                                                                (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name FIFO Control Register
* <b>Reset value:</b> 0x00000001\n
* <b>Access:</b> rw-
* @{
*/
#define CT_FCR_REG                                                                                  (0x4001400CUL) /**< rw-*/
#define CT_FCR_REG_PEC2_PEC0_MASK                                                                   (0x000000E0UL) /**< rw - Parity Error counter. */
#define CT_FCR_REG_PEC2_PEC0_POS                                                                    (5UL)
#define CT_FCR_REG_FTC4_FTC0_MASK                                                                   (0x0000001FUL) /**< rw - FIFO Threshold Configuration. */
#define CT_FCR_REG_FTC4_FTC0_POS                                                                    (0UL)
#define CT_FCR_REG__RESET_VALUE                                                                     (0x00000001UL) /**< value after reset */
/**
* @}
*/


/**
* \name Guard Time Register slot x (GTR register of main slot accessed when IOAUXen = 0, else GTR register of auxiliary slot accessed)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_GTRx_REG                                                                                 (0x40014010UL) /**< rw-*/
#define CT_GTRx_REG_GT7_GT0_MASK                                                                    (0x000000FFUL) /**< rw - Guard time. */
#define CT_GTRx_REG_GT7_GT0_POS                                                                     (0UL)
#define CT_GTRx_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Configuration Register 1 slot x (UCR1 register of main slot accessed when IOAUXen = 0, else UCR1 register of auxiliary slot accessed)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_UCR1x_REG                                                                                (0x40014014UL) /**< rw-*/
#define CT_UCR1x_REG_FIP_MASK                                                                       (0x00000020UL) /**< rw - Force Inverse Parity. */
#define CT_UCR1x_REG_FIP_POS                                                                        (5UL)
#define CT_UCR1x_REG_FC_MASK                                                                        (0x00000010UL) /**< rw - TBD. */
#define CT_UCR1x_REG_FC_POS                                                                         (4UL)
#define CT_UCR1x_REG_PROT_MASK                                                                      (0x00000008UL) /**< rw - PROTocol. */
#define CT_UCR1x_REG_PROT_POS                                                                       (3UL)
#define CT_UCR1x_REG_T_R_MASK                                                                       (0x00000004UL) /**< rw - Transmit_Receive. */
#define CT_UCR1x_REG_T_R_POS                                                                        (2UL)
#define CT_UCR1x_REG_LCT_MASK                                                                       (0x00000002UL) /**< rw - Last Character to Transmit. */
#define CT_UCR1x_REG_LCT_POS                                                                        (1UL)
#define CT_UCR1x_REG_CONV_MASK                                                                      (0x00000001UL) /**< rw - CONVention. */
#define CT_UCR1x_REG_CONV_POS                                                                       (0UL)
#define CT_UCR1x_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Configuration Register 2 slot x (UCR2 register of main slot accessed when IOAUXen = 0, else UCR2 register of auxiliary slot accessed)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_UCR2x_REG                                                                                (0x40014018UL) /**< rw-*/
#define CT_UCR2x_REG_WRDACC_MASK                                                                    (0x00000080UL) /**< rw - FIFO WoRD ACCess. */
#define CT_UCR2x_REG_WRDACC_POS                                                                     (7UL)
#define CT_UCR2x_REG_FIFO_FLUSH_MASK                                                                (0x00000040UL) /**< rw - FIFO flush. */
#define CT_UCR2x_REG_FIFO_FLUSH_POS                                                                 (6UL)
#define CT_UCR2x_REG_DISINTAUX_MASK                                                                 (0x00000020UL) /**< rw - DISable INTAUX. */
#define CT_UCR2x_REG_DISINTAUX_POS                                                                  (5UL)
#define CT_UCR2x_REG_DISATRCOUNTER_MASK                                                             (0x00000010UL) /**< rw - DISable ATR counter. */
#define CT_UCR2x_REG_DISATRCOUNTER_POS                                                              (4UL)
#define CT_UCR2x_REG_DISPE_MASK                                                                     (0x00000008UL) /**< rw - DISable Parity Error interrupt bit. */
#define CT_UCR2x_REG_DISPE_POS                                                                      (3UL)
#define CT_UCR2x_REG_DISFT_MASK                                                                     (0x00000004UL) /**< rw - DISable Fifo Threshold interrupt bit. */
#define CT_UCR2x_REG_DISFT_POS                                                                      (2UL)
#define CT_UCR2x_REG_MAN_BGT_MASK                                                                   (0x00000002UL) /**< rw - MANual BGT. */
#define CT_UCR2x_REG_MAN_BGT_POS                                                                    (1UL)
#define CT_UCR2x_REG_NOT_AUTOCONV_MASK                                                              (0x00000001UL) /**< rw - AUTOmatically detected CONVention. */
#define CT_UCR2x_REG_NOT_AUTOCONV_POS                                                               (0UL)
#define CT_UCR2x_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Clock Configuration Register slot x (CCR register of main slot accessed when IOAUXen = 0, else CCR register of auxiliary slot accessed)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_CCRx_REG                                                                                 (0x4001401CUL) /**< rw-*/
#define CT_CCRx_REG_SHL_MASK                                                                        (0x00000020UL) /**< rw - Stop HIGH or LOW. */
#define CT_CCRx_REG_SHL_POS                                                                         (5UL)
#define CT_CCRx_REG_CST_MASK                                                                        (0x00000010UL) /**< rw - Clock Stop (RFU for auxiliary slot). */
#define CT_CCRx_REG_CST_POS                                                                         (4UL)
#define CT_CCRx_REG_SAN_MASK                                                                        (0x00000008UL) /**< rw - Synchronous Asynchronous Card. */
#define CT_CCRx_REG_SAN_POS                                                                         (3UL)
#define CT_CCRx_REG_ACC2_ACC0_MASK                                                                  (0x00000007UL) /**< rw - Asynchronous Card Clock. */
#define CT_CCRx_REG_ACC2_ACC0_POS                                                                   (0UL)
#define CT_CCRx_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Power Control Register
* <b>Reset value:</b> 0x000000C0\n
* <b>Access:</b> rw-
* @{
*/
#define CT_PCR_REG                                                                                  (0x40014020UL) /**< rw-*/
#define CT_PCR_REG_C8_MASK                                                                          (0x00000080UL) /**< rw - Contact 8. */
#define CT_PCR_REG_C8_POS                                                                           (7UL)
#define CT_PCR_REG_C4_MASK                                                                          (0x00000040UL) /**< rw - Contact 4. */
#define CT_PCR_REG_C4_POS                                                                           (6UL)
#define CT_PCR_REG_RSTIN_MASK                                                                       (0x00000010UL) /**< rw - Reset bit. */
#define CT_PCR_REG_RSTIN_POS                                                                        (4UL)
#define CT_PCR_REG_VCCSEL1_VCCSEL0_MASK                                                             (0x0000000CUL) /**< rw - Vcc selection. */
#define CT_PCR_REG_VCCSEL1_VCCSEL0_POS                                                              (2UL)
#define CT_PCR_REG_WARM_MASK                                                                        (0x00000002UL) /**< rw - WARM. */
#define CT_PCR_REG_WARM_POS                                                                         (1UL)
#define CT_PCR_REG_START_MASK                                                                       (0x00000001UL) /**< rw - START. */
#define CT_PCR_REG_START_POS                                                                        (0UL)
#define CT_PCR_REG__RESET_VALUE                                                                     (0x000000C0UL) /**< value after reset */
/**
* @}
*/


/**
* \name Early answer CounteR
* <b>Reset value:</b> 0x000000AA\n
* <b>Access:</b> rw-
* @{
*/
#define CT_ECR_REG                                                                                  (0x40014024UL) /**< rw-*/
#define CT_ECR_REG_EC7_EC0_MASK                                                                     (0x000000FFUL) /**< rw - Early answer CounteR. */
#define CT_ECR_REG_EC7_EC0_POS                                                                      (0UL)
#define CT_ECR_REG__RESET_VALUE                                                                     (0x000000AAUL) /**< value after reset */
/**
* @}
*/


/**
* \name Mute card CounteR RST Low register Least Significant Byte
* <b>Reset value:</b> 0x00000074\n
* <b>Access:</b> rw-
* @{
*/
#define CT_MCLR_LSB_REG                                                                             (0x40014028UL) /**< rw-*/
#define CT_MCLR_LSB_REG_MCL7_MCL0_MASK                                                              (0x000000FFUL) /**< rw - Mute card CounteR RST Low Least Significant Byte. */
#define CT_MCLR_LSB_REG_MCL7_MCL0_POS                                                               (0UL)
#define CT_MCLR_LSB_REG__RESET_VALUE                                                                (0x00000074UL) /**< value after reset */
/**
* @}
*/


/**
* \name Mute card CounteR RST Low register Most Significant Byte
* <b>Reset value:</b> 0x000000A4\n
* <b>Access:</b> rw-
* @{
*/
#define CT_MCLR_MSB_REG                                                                             (0x4001402CUL) /**< rw-*/
#define CT_MCLR_MSB_REG_MCL15_MCL7_MASK                                                             (0x000000FFUL) /**< rw - Mute card CounteR RST Low Most Significant Byte. */
#define CT_MCLR_MSB_REG_MCL15_MCL7_POS                                                              (0UL)
#define CT_MCLR_MSB_REG__RESET_VALUE                                                                (0x000000A4UL) /**< value after reset */
/**
* @}
*/


/**
* \name Mute card CounteR RST HIGH register Least Significant Byte
* <b>Reset value:</b> 0x00000074\n
* <b>Access:</b> rw-
* @{
*/
#define CT_MCHR_LSB_REG                                                                             (0x40014030UL) /**< rw-*/
#define CT_MCHR_LSB_REG_MCH7_MCH0_MASK                                                              (0x000000FFUL) /**< rw - Mute card CounteR RST HIGH Least Significant Byte. */
#define CT_MCHR_LSB_REG_MCH7_MCH0_POS                                                               (0UL)
#define CT_MCHR_LSB_REG__RESET_VALUE                                                                (0x00000074UL) /**< value after reset */
/**
* @}
*/


/**
* \name Mute card CounteR RST HIGH register Most Significant Byte
* <b>Reset value:</b> 0x000000A4\n
* <b>Access:</b> rw-
* @{
*/
#define CT_MCHR_MSB_REG                                                                             (0x40014034UL) /**< rw-*/
#define CT_MCHR_MSB_REG_MCH15_MCH7_MASK                                                             (0x000000FFUL) /**< rw - Mute card CounteR RST HIGH Most Significant Byte. */
#define CT_MCHR_MSB_REG_MCH15_MCH7_POS                                                              (0UL)
#define CT_MCHR_MSB_REG__RESET_VALUE                                                                (0x000000A4UL) /**< value after reset */
/**
* @}
*/


/**
* \name Slew Rate configuration Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_SRR_REG                                                                                  (0x40014038UL) /**< rw-*/
#define CT_SRR_REG_VCC_RISE_SEL1_VCC_RISE_SEL0_MASK                                                 (0x00000030UL) /**< rw - Vcc rise time selection. */
#define CT_SRR_REG_VCC_RISE_SEL1_VCC_RISE_SEL0_POS                                                  (4UL)
#define CT_SRR_REG_CLK_SR1_CLK_SR0_MASK                                                             (0x0000000CUL) /**< rw - CLK slew rate. */
#define CT_SRR_REG_CLK_SR1_CLK_SR0_POS                                                              (2UL)
#define CT_SRR_REG_IO_SR1_IO_SR0_MASK                                                               (0x00000003UL) /**< rw - IO slew rate. */
#define CT_SRR_REG_IO_SR1_IO_SR0_POS                                                                (0UL)
#define CT_SRR_REG__RESET_VALUE                                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Transmit & Receive Register (FIFO access registers, URR used when reading, UTR used when writing, 4 contiguous addresses to support LDM/STM ARM instructions)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_UTR_URR_REG_adr1                                                                         (0x4001403CUL) /**< rw-*/
#define CT_UTR_URR_REG_adr1_UTRURR31_UTRURR0_MASK                                                   (0xFFFFFFFFUL) /**< rw - Uart Transmit & Receive Register. */
#define CT_UTR_URR_REG_adr1_UTRURR31_UTRURR0_POS                                                    (0UL)
#define CT_UTR_URR_REG_adr1__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Transmit & Receive Register (FIFO access registers, URR used when reading, UTR used when writing, 4 contiguous addresses to support LDM/STM ARM instructions)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_UTR_URR_REG_adr2                                                                         (0x40014040UL) /**< rw-*/
#define CT_UTR_URR_REG_adr2_UTRURR31_UTRURR0_MASK                                                   (0xFFFFFFFFUL) /**< rw - Uart Transmit & Receive Register. */
#define CT_UTR_URR_REG_adr2_UTRURR31_UTRURR0_POS                                                    (0UL)
#define CT_UTR_URR_REG_adr2__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Transmit & Receive Register (FIFO access registers, URR used when reading, UTR used when writing, 4 contiguous addresses to support LDM/STM ARM instructions)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_UTR_URR_REG_adr3                                                                         (0x40014044UL) /**< rw-*/
#define CT_UTR_URR_REG_adr3_UTRURR31_UTRURR0_MASK                                                   (0xFFFFFFFFUL) /**< rw - Uart Transmit & Receive Register. */
#define CT_UTR_URR_REG_adr3_UTRURR31_UTRURR0_POS                                                    (0UL)
#define CT_UTR_URR_REG_adr3__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Transmit & Receive Register (FIFO access registers, URR used when reading, UTR used when writing, 4 contiguous addresses to support LDM/STM ARM instructions)
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_UTR_URR_REG_adr4                                                                         (0x40014048UL) /**< rw-*/
#define CT_UTR_URR_REG_adr4_UTRURR31_UTRURR0_MASK                                                   (0xFFFFFFFFUL) /**< rw - Uart Transmit & Receive Register. */
#define CT_UTR_URR_REG_adr4_UTRURR31_UTRURR0_POS                                                    (0UL)
#define CT_UTR_URR_REG_adr4__RESET_VALUE                                                            (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Time-Out Register 1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define CT_TOR1_REG                                                                                 (0x4001404CUL) /**< -w-*/
#define CT_TOR1_REG_TOL7_TOL0_MASK                                                                  (0x000000FFUL) /**< w - Time-Out Latched. */
#define CT_TOR1_REG_TOL7_TOL0_POS                                                                   (0UL)
#define CT_TOR1_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Time-Out Register 2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define CT_TOR2_REG                                                                                 (0x40014050UL) /**< -w-*/
#define CT_TOR2_REG_TOL15_TOL8_MASK                                                                 (0x000000FFUL) /**< w - Time-Out Latched. */
#define CT_TOR2_REG_TOL15_TOL8_POS                                                                  (0UL)
#define CT_TOR2_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Time-Out Register 3
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> -w-
* @{
*/
#define CT_TOR3_REG                                                                                 (0x40014054UL) /**< -w-*/
#define CT_TOR3_REG_TOL23_TOL16_MASK                                                                (0x000000FFUL) /**< w - Time-Out Latched. */
#define CT_TOR3_REG_TOL23_TOL16_POS                                                                 (0UL)
#define CT_TOR3_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Time-Out Configuration
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rw-
* @{
*/
#define CT_TOC_REG                                                                                  (0x40014058UL) /**< rw-*/
#define CT_TOC_REG_TOC7_TOC0_MASK                                                                   (0x000000FFUL) /**< rw - Time-Out Configuration. */
#define CT_TOC_REG_TOC7_TOC0_POS                                                                    (0UL)
#define CT_TOC_REG__RESET_VALUE                                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name FIFO Status Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define CT_FSR_REG                                                                                  (0x4001405CUL) /**< r--*/
#define CT_FSR_REG_FFL5_FFL0_MASK                                                                   (0x0000003FUL) /**< r - FIFO Fulfilment Level. */
#define CT_FSR_REG_FFL5_FFL0_POS                                                                    (0UL)
#define CT_FSR_REG__RESET_VALUE                                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name Mixed Status Register
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define CT_MSR_REG                                                                                  (0x40014060UL) /**< r--*/
#define CT_MSR_REG_INTAUX_MASK                                                                      (0x00000004UL) /**< r - Auxiliary interrupt. */
#define CT_MSR_REG_INTAUX_POS                                                                       (2UL)
#define CT_MSR_REG_BGT_MASK                                                                         (0x00000002UL) /**< r - Block Guard Time. */
#define CT_MSR_REG_BGT_POS                                                                          (1UL)
#define CT_MSR_REG_PRES_MASK                                                                        (0x00000001UL) /**< r - PRESence. */
#define CT_MSR_REG_PRES_POS                                                                         (0UL)
#define CT_MSR_REG__RESET_VALUE                                                                     (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Status Register 1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define CT_USR1_REG                                                                                 (0x40014064UL) /**< r--*/
#define CT_USR1_REG_MUTE_MASK                                                                       (0x00000020UL) /**< r - Card mute. */
#define CT_USR1_REG_MUTE_POS                                                                        (5UL)
#define CT_USR1_REG_EARLY_MASK                                                                      (0x00000010UL) /**< r - Early answer. */
#define CT_USR1_REG_EARLY_POS                                                                       (4UL)
#define CT_USR1_REG_PE_MASK                                                                         (0x00000008UL) /**< r - Parity Error. */
#define CT_USR1_REG_PE_POS                                                                          (3UL)
#define CT_USR1_REG_OVR_MASK                                                                        (0x00000004UL) /**< r - OverRun. */
#define CT_USR1_REG_OVR_POS                                                                         (2UL)
#define CT_USR1_REG_FER_MASK                                                                        (0x00000002UL) /**< r - Framing Error. */
#define CT_USR1_REG_FER_POS                                                                         (1UL)
#define CT_USR1_REG_FT_MASK                                                                         (0x00000001UL) /**< r - Fifo Threshold. */
#define CT_USR1_REG_FT_POS                                                                          (0UL)
#define CT_USR1_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name UART Status Register 2
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r--
* @{
*/
#define CT_USR2_REG                                                                                 (0x40014068UL) /**< r--*/
#define CT_USR2_REG_TO3_MASK                                                                        (0x00000080UL) /**< r - Time Out counter3. */
#define CT_USR2_REG_TO3_POS                                                                         (7UL)
#define CT_USR2_REG_TO2_MASK                                                                        (0x00000040UL) /**< r - Time Out counter2. */
#define CT_USR2_REG_TO2_POS                                                                         (6UL)
#define CT_USR2_REG_TO1_MASK                                                                        (0x00000020UL) /**< r - Time Out counter1. */
#define CT_USR2_REG_TO1_POS                                                                         (5UL)
#define CT_USR2_REG_WRDACCERR_MASK                                                                  (0x00000010UL) /**< r - WoRD ACCess ERRor. */
#define CT_USR2_REG_WRDACCERR_POS                                                                   (4UL)
#define CT_USR2_REG_INTAUXL_MASK                                                                    (0x00000008UL) /**< r - AUXiliary INTerrupt Latched. */
#define CT_USR2_REG_INTAUXL_POS                                                                     (3UL)
#define CT_USR2_REG_PROTL_MASK                                                                      (0x00000004UL) /**< r - PROTection Latched. */
#define CT_USR2_REG_PROTL_POS                                                                       (2UL)
#define CT_USR2_REG_PRESL_MASK                                                                      (0x00000002UL) /**< r - PRESence Latched. */
#define CT_USR2_REG_PRESL_POS                                                                       (1UL)
#define CT_USR2_REG_PTL_MASK                                                                        (0x00000001UL) /**< r - Protection Temperature Latched. */
#define CT_USR2_REG_PTL_POS                                                                         (0UL)
#define CT_USR2_REG__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TestBus Selection
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define CT_TBSEL_REG                                                                                (0x4001406CUL) /**< rwm*/
#define CT_TBSEL_REG_TBSEL_MASK                                                                     (0x0000000FUL) /**< rw - TestBus SELection. */
#define CT_TBSEL_REG_TBSEL_POS                                                                      (0UL)
#define CT_TBSEL_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TestBus VALue
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> r-m
* @{
*/
#define CT_TBVAL_REG                                                                                (0x40014070UL) /**< r-m*/
#define CT_TBVAL_REG_TBVAL_MASK                                                                     (0x000000FFUL) /**< r - TestBus VALue. */
#define CT_TBVAL_REG_TBVAL_POS                                                                      (0UL)
#define CT_TBVAL_REG__RESET_VALUE                                                                   (0x00000000UL) /**< value after reset */
/**
* @}
*/


/**
* \name TeST Register 1
* <b>Reset value:</b> 0x00000000\n
* <b>Access:</b> rwm
* @{
*/
#define CT_TST1_REF                                                                                 (0x40014074UL) /**< rwm*/
#define CT_TST1_REF_INHIB_MASK                                                                      (0x00000008UL) /**< rw - Inhibit fault for the sequencer. */
#define CT_TST1_REF_INHIB_POS                                                                       (3UL)
#define CT_TST1_REF_MASK_DCDC_READY_MASK                                                            (0x00000004UL) /**< rw - Mask DCDC_ready signal. */
#define CT_TST1_REF_MASK_DCDC_READY_POS                                                             (2UL)
#define CT_TST1_REF_DISPTEMP_MASK                                                                   (0x00000002UL) /**< rw - DISable Protection TEMPerature. */
#define CT_TST1_REF_DISPTEMP_POS                                                                    (1UL)
#define CT_TST1_REF_DISDEB_MASK                                                                     (0x00000001UL) /**< rw - DISable presence DEBounce feature. */
#define CT_TST1_REF_DISDEB_POS                                                                      (0UL)
#define CT_TST1_REF__RESET_VALUE                                                                    (0x00000000UL) /**< value after reset */
/**
* @}
*/


#endif /* PN7462AU_IP_CTIP_H_INCLUDED */
