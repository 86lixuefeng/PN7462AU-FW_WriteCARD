/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
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
 * phExPos_Common.h:  <The purpose and scope of this file>
 *
 * Project:  PN7462AU
 *
 */

#ifndef PHEXPOS_COMMON_H
#define PHEXPOS_COMMON_H

#include <string.h>

#include <phRtos.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <event_groups.h>
#include <ph_Status.h>


#define PH_EXPOS_COMMON_POS_STUB_SUPP

#define PH_EXPOS_COMMON_POS_BASE_PRIORITY    0

#define PH_EXPOS_COMMON_HEADER_SIZE	4

#define PH_EXPOS_COMMON_CL_SLOT_TAG_TYPE                                    0x01U
#define PH_EXPOS_COMMON_CT_SLOT_TAG_TYPE                                    0x02U
#define PH_EXPOS_COMMON_SAM_TAG_TYPE                                        0x03U
#define PH_EXPOS_COMMON_EXTND_TAG_TYPE                                      0x04U

#define PH_EXPOS_COMMON_CHANNEL_NONE                                        0x00U
#define PH_EXPOS_COMMON_CL_CHANNEL_NO                                       0x01U
#define PH_EXPOS_COMMON_CT_CHANNEL_NO                                       0x02U

#define PH_EXPOS_COMMON_PH_EXMAIN_CFG_POLL_TIMER_TIMEOUT_MS                 300 /* 300 milli secs. */

#define PH_EXPOS_COMMON_CL_SLOT                                 0x00000001U
#define PH_EXPOS_COMMON_CT_SLOT                                 0x00000002U
#define PH_EXPOS_COMMON_SAM1_SLOT                               0x00000003U
#define PH_EXPOS_COMMON_SAM2_SLOT                               0x00000004U
#define PH_EXPOS_COMMON_SAM3_SLOT                               0x00000005U
#define PH_EXPOS_COMMON_SAM4_SLOT                               0x00000006U
#define PH_EXPOS_COMMON_SAM5_SLOT                               0x00000007U

#define PH_EXPOS_COMMON_TRANS_EXCHANGE_CMD                      0x000000FFU
#define PH_EXPOS_COMMON_HOST_TX_BUF_LEN                         64
#define PH_EXPOS_COMMON_HOST_RX_BUF_LEN                         64
#define PH_EXPOS_COMMON_MAX_ATR_LEN                             32

#define PH_EXPOS_COMMON_POS_GPIO_LINE                           0x05U

#define PH_EXPOS_COMMON_POS_BLUE_LED                            0x09U
#define PH_EXPOS_COMMON_POS_GREEN_LED                           0x0AU
#define PH_EXPOS_COMMON_POS_YELLOW_LED                          0x0BU
#define PH_EXPOS_COMMON_POS_RED_LED                             0x0CU

#define PH_EXPOS_COMMON_CNTRL_CMD_GET_CONFIG                    0x00U
#define PH_EXPOS_COMMON_CNTRL_CMD_SET_CONFIG                    0x01U
#define PH_EXPOS_COMMON_CNTRL_CMD_GET_STATUS                    0x02U
#define PH_EXPOS_COMMON_CNTRL_CMD_GET_ATR                       0x03U
#define PH_EXPOS_COMMON_CNTRL_CMD_DEACTIVATE_CARD               0x04U
#define PH_EXPOS_COMMON_CNTRL_CMD_CHK_CARD_PRESENCE             0x05U
#define PH_EXPOS_COMMON_CNTRL_CMD_SWITCH_SLOT                   0x06U
#define PH_EXPOS_COMMON_CNTRL_CMD_EN_POLLING                    0x07U
#define PH_EXPOS_COMMON_CNTRL_CMD_DIS_POLLING                   0x08U
#define PH_EXPOS_COMMON_CNTRL_CMD_CONFIG_POLLING                0x09U
#define PH_EXPOS_COMMON_CNTRL_CMD_ABORT_TRANSFER                0x0AU
#define PH_EXPOS_COMMON_CNTRL_CMD_SW_RESET                      0x0BU
#define PH_EXPOS_COMMON_CNTRL_CMD_GET_UID                       0x0CU


#define PH_EXPOS_COMMON_CL_TRNSP_EX_CMD                         0x01U
#define PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD                  0x02U
#define PH_EXPOS_COMMON_CL_ABORT_CMD                            0x04U


#define PH_EXPOS_COMMON_CT_TRNSP_EX_CMD                         0x01U
#define PH_EXPOS_COMMON_CT_DEACTIVATE_CARD_CMD                  0x02U
#define PH_EXPOS_COMMON_CT_ABORT_CMD                            0x04U

#define PH_EXPOS_COMMON_CMD_SUCCESS_CODE                        0x9000U
#define PH_EXPOS_COMMON_ERR_UNKNOWN_REASON                      0x6300U
#define PH_EXPOS_COMMON_ERR_APDU_LEN_WRONG                      0x6500U
#define PH_EXPOS_COMMON_ERR_CLASS_BYTE_WRONG                    0x6501U
#define PH_EXPOS_COMMON_ERR_INVALID_PARAM                       0x6502U
#define PH_EXPOS_COMMON_ERR_LE_INVALID                          0x6503U
#define PH_EXPOS_COMMON_ERR_INS_INVALID                         0x6504U
#define PH_EXPOS_COMMON_ERR_UNSUPPORTED_CMD                     0x6600U
#define PH_EXPOS_COMMON_ERR_WRONG_SEQ                           0x6700U
#define PH_EXPOS_COMMON_STATUS_SUCCESS                                       0x00

#define PH_EXPOS_COMMON_MIFARECLASSIC_1K_SAK      					0x08U
#define PH_EXPOS_COMMON_MIFARECLASSIC_4K_SAK      					0x18U
#define PH_EXPOS_COMMON_MIFAREULTRALIGHT_SAK      					0x00U

#define PH_EXPOS_COMMON_CLA_BYTE                                0x04U
#define PH_EXPOS_COMMON_INS_BYTE                                0x05U
#define PH_EXPOS_COMMON_P1_BYTE                                 0x06U



typedef enum phExPos_Pos_Async_Events
{
	E_STS_NO_EVENT,
	E_BANKING_CARD_INSERTION_EVENT_TYPEA,
	E_BANKING_CARD_INSERTION_EVENT_TYPEB,
	E_BANKING_CARD_INSERTION_EVENT_CT,
	E_NONBANKING_CARD_INSERTION_EVENT_TYPEA,
	E_NONBANKING_CARD_INSERTION_EVENT_TYPEB,
	E_CARD_REMOVAL_EVENT,
	E_SAM_INSERTION_EVENT,
	E_SAM_REMOVAL_EVENT,
	E_RF_COM_ERR_EVENT,
	E_SYS_EXCEPTION_EVENT

}phExPos_Pos_Async_Events_t;

typedef struct phExPos_APDU_Rsp
{
	uint16_t      wHeader;  // contains length field
	uint16_t      wStausCode;
	uint8_t       *pData;
}phExPos_APDU_Rsp_t;

/**
* \brief Stores all reader global settings and variables
*/
typedef struct phExPos_Posfw_Data
{

	struct
	{
		uint8_t config_info1;
		uint8_t config_info2;
	} PosConfig;

	phExPos_Pos_Async_Events_t eCurrEvent;
	uint8_t  bActiveSlot;
	uint8_t  aAtr[PH_EXPOS_COMMON_MAX_ATR_LEN];
	uint8_t  bAtrValid;
	uint8_t  bAtrSize;
	uint8_t Trans_complete;
	uint8_t Uid[15];
	uint8_t Uid_Length;

	phRtos_EventHandle_t xCT_Events;
    phRtos_EventHandle_t xCL_Events;

    phRtos_MutexHandle_t xCT_CL_Mutex;

	uint8_t bTxDataValid;
	uint8_t bRxDataValid;
	uint8_t bEnPollStartup;
	uint32_t aTxBuff[PH_EXPOS_COMMON_HOST_TX_BUF_LEN];
	uint32_t aRxBuff[PH_EXPOS_COMMON_HOST_RX_BUF_LEN];

	uint8_t *bpTxBuf;
	uint8_t *bpRxBuf;

} phExPos_Posfw_Data_t;

#endif /* POS_FW_H_ */
