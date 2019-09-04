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
 * phExCcid_UsbCcid.h: USB CCID Application Informations
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-08-26 14:16:23 +0530 (Fri, 26 Aug 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 3833 $ (v05.19.00)
 */

#ifndef PHEXCCID_USBCCID_H
#define PHEXCCID_USBCCID_H

#include "mw_usbd.h"
#include "phRtos.h"
#include "ph_Status.h"

/*
 * EndPoint Informations
 * Note: When Changing the End point Numbers change in the following file phExCcid_Descriptors.c
 */
#define PH_EXCCID_USBCCID_USB_INT_IN_EP                            0x82U
#define PH_EXCCID_USBCCID_USB_BULK_OUT_EP                          0x01U
#define PH_EXCCID_USBCCID_USB_BULK_IN_EP                           0x81U

/*
 * BULK OUT Messages from PC to RDR
 */
#define PH_EXCCID_USBCCID_PC_TO_RDR_ICCPOWERON                     0x62U
#define PH_EXCCID_USBCCID_PC_TO_RDR_ICCPOWEROFF                    0x63U
#define PH_EXCCID_USBCCID_PC_TO_RDR_GETSLOTSTATUS                  0x65U
#define PH_EXCCID_USBCCID_PC_TO_RDR_XFRBLOCK                       0x6FU
#define PH_EXCCID_USBCCID_PC_TO_RDR_GETPARAMETERS                  0x6CU
#define PH_EXCCID_USBCCID_PC_TO_RDR_RESETPARAMETERS                0x6DU
#define PH_EXCCID_USBCCID_PC_TO_RDR_SETPARAMETERS                  0x61U
#define PH_EXCCID_USBCCID_PC_TO_RDR_ESCAPE                         0x6BU
#define PH_EXCCID_USBCCID_PC_TO_RDR_ICCCLOCK                       0x6EU
#define PH_EXCCID_USBCCID_PC_TO_RDR_T0APDU                         0x6AU
#define PH_EXCCID_USBCCID_PC_TO_RDR_SECURE                         0x69U
#define PH_EXCCID_USBCCID_PC_TO_RDR_MECHANICAL                     0x71U
#define PH_EXCCID_USBCCID_PC_TO_RDR_ABORT                          0x72U
#define PH_EXCCID_USBCCID_PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY   0x73U

/*
 * BULK IN Messages from RDR to PC
 */
#define PH_EXCCID_USBCCID_RDR_TO_PC_DATABLOCK                      0x80U
#define PH_EXCCID_USBCCID_RDR_TO_PC_SLOTSTATUS                     0x81U
#define PH_EXCCID_USBCCID_RDR_TO_PC_PARAMETERS                     0x82U
#define PH_EXCCID_USBCCID_RDR_TO_PC_ESCAPE                         0x83U
#define PH_EXCCID_USBCCID_RDR_TO_PC_DATARATEANDCLOCKFREQUENCY      0x84U

/* INT IN Message From RDR to PC */
#define PH_EXCCID_USBCCID_CCID_INT_IN_SLOT_CHANGE_CMD              0x50U

/*
 * CCID Command Header
 */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MESSAGE_TYPE           0x00U    /**<  CCID Header - Byte 1  - Message Type      */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_1          0x01U    /**<  CCID Header - Byte 2  - Length 1 (LSB)    */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_2          0x02U    /**<  CCID Header - Byte 3  - Length 2          */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_3          0x03U    /**<  CCID Header - Byte 4  - Length 3          */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_LENGTH_BYTE_4          0x04U    /**<  CCID Header - Byte 5  - Length 4 (MSB)    */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_SLOT                   0x05U    /**<  CCID Header - Byte 6  - Slot Number       */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_SEQ                    0x06U    /**<  CCID Header - Byte 7  - Sequence          */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_1             0x07U    /**<  CCID Header - Byte 8  - Message Byte 1    */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_2             0x08U    /**<  CCID Header - Byte 9  - Message Byte 2    */
#define PH_EXCCID_USBCCID_PN7462AU_CCID_HEADER_MSG_BYTE_3             0x09U    /**<  CCID Header - Byte 10 - Message Byte 3    */

#define PH_EXCCID_USBCCID_PN7462AU_MAX_CCID_BUFFER_LEN                271

/*
 * CCID Errors
 */
#define PH_EXCCID_USBCCID_CCID_ERROR_SLOT_BUSY                     0xE0U    /**<  Error Code: Slot is busy                  */
#define PH_EXCCID_USBCCID_CCID_ERROR_SLOT_NOT_EXIST                0x05U    /**<  Error Code: Slot does not exit            */
#define PH_EXCCID_USBCCID_CCID_ERROR_SLOT_ICC_MUTE                 0xFEU    /**<  Error Code: ICC is mute                   */
#define PH_EXCCID_USBCCID_CCID_ERROR_SLOT_XFR_OVERRUN              0xFCU    /**<  Error Code: Buffer overrun                */
#define PH_EXCCID_USBCCID_CCID_ERROR_SLOT_CMD_NOT_SUPPORTED        0x00U    /**<  Error Code: Command not supported         */
#define PH_EXCCID_USBCCID_CCID_ERROR_SLOT_HW_ERROR                 0xFBU    /**<  Error Code: Hardware error                */

#define PH_EXCCID_USBCCID_CCID_MAX_BUFFER_SIZE                     261
#define PH_EXCCID_USBCCID_CCID_HEADER_LENGTH                       0x0AU

#define PH_EXCCID_USBCCID_CCID_COMMAND_NOT_SUPPORTED               0x40U

#define PH_EXCCID_USBCCID_CCID_CARD_PRESENT_IN_SLOT                0x01U
#define PH_EXCCID_USBCCID_CCID_CARD_NOT_PRESENT_IN_SLOT            0x00U

#define PH_EXCCID_USBCCID_MAX_ATR_LEN                              33
#define PH_EXCCID_USBCCID_CL_CHANNEL_NO                            0x01U
#define PH_EXCCID_USBCCID_CT_CHANNEL_NO                            0x02U
#define PH_EXCCID_USBCCID_CHANNEL_NONE                             0x00U

/* CL Event Notifications */
#define PH_EXCCID_USBCCID_CL_TRNSP_EX_CMD                          0x01U
#define PH_EXCCID_USBCCID_CL_DEACTIVATE_CARD_CMD                   0x02U
#define PH_EXCCID_USBCCID_CL_AUTH_CMD                              0x04U
#define PH_EXCCID_USBCCID_CL_READ_CARD_CMD                         0x08U
#define PH_EXCCID_USBCCID_CL_WRITE_CARD_CMD                        0x10U

/* CT Event Notifications */
#define PH_EXCCID_USBCCID_CT_TRNSP_EX_CMD                          0x01U
#define PH_EXCCID_USBCCID_CT_DEACTIVATE_CARD_CMD                   0x02U
#define PH_EXCCID_USBCCID_CT_ABORT_CMD                             0x04U

/* CL Card Types */
#define PH_EXCCID_USBCCID_CL_MIFARECLASSIC                         0x01U
#define PH_EXCCID_USBCCID_CL_MIFAREULTRALIGHT                      0x02U
#define PH_EXCCID_USBCCID_CL_TYPE_A_L4                             0x03U
#define PH_EXCCID_USBCCID_CL_FELICA                                0x04U
#define PH_EXCCID_USBCCID_CL_TYPE_B                                0x05U
#define PH_EXCCID_USBCCID_CL_TYPE_V                                0x06U
#define PH_EXCCID_USBCCID_CL_18000P3M3                             0x07U
#define PH_EXCCID_USBCCID_CL_TYPE_A_NS                             0x08U
#define PH_EXCCID_USBCCID_PN7462AU_MAX_KEY_LEN                        6

/*
 * APDU Headers
 */

#define PH_EXCCID_USBCCID_PN7462AU_APDU_CLASS                         0x00U                /**< APDU Class Offset */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_INS                           0x01U                /**< APDU Instruction Offset */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_P1                            0x02U                /**< APDU P1 Offset */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_P2                            0x03U                /**< APDU P2 Offset */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_LC                            0x04U                /**< APDU Lc Offset */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_LE                            4                    /**< APDU Le Offset */

#define PH_EXCCID_USBCCID_PN7462AU_APDU_PAYLOAD                       0                /**< APDU payload Offset */

#define PH_EXCCID_USBCCID_PN7462AU_APDU_CC_EXT_MANAGE_SESSION         0x00U
#define PH_EXCCID_USBCCID_PN7462AU_APDU_CC_EXT_TRANS_EXCHANGE         0x01U
#define PH_EXCCID_USBCCID_PN7462AU_APDU_CC_EXT_SWITCH_PROTOCOL        0x02U

/*
 * INS Commands Supported
 */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_CC_EXT_INS                    0xC2U
#define PH_EXCCID_USBCCID_PN7462AU_APDU_GET_DATA_INS                  0xCAU            /**< PCSC Extension: GetData  */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_LOAD_KEY_INS                  0x82U            /**< PCSC Extension: LoadKey  */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_G_AUTH_CMD_INS                0x86U            /**< PCSC Extension: Authenticate Command  */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_READ_BIN                      0xB0U            /**< PCSC Extension: Read Binary */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_UPDATE_BIN                    0xD6U            /**< PCSC Extension: Update Binary  */

/*
 * UserDefined INS Commands Supported
 */
#define PH_EXCCID_USBCCID_PN7462AU_APDU_GET_FW_VERS_INS               0xE1U
#define PH_EXCCID_USBCCID_PN7462AU_FW_UC_VERSION_LEN                  0x1CU

#define PH_EXCCID_USBCCID_PN7462AU_APDU_OPERATION_MODE_INS            0xF2U
#define PH_EXCCID_USBCCID_PN7462AU_OPERATION_MODE_LEN                 0x01U

#define PH_EXCCID_USBCCID_ESCAPE_BLUE_LED_ON                       0x11U
#define PH_EXCCID_USBCCID_ESCAPE_BLUE_LED_OFF                      0x10U

#define PH_EXCCID_USBCCID_ESCAPE_GREEN_LED_ON                      0x21U
#define PH_EXCCID_USBCCID_ESCAPE_GREEN_LED_OFF                     0x20U

#define PH_EXCCID_USBCCID_ESCAPE_YELLOW_LED_ON                     0x31U
#define PH_EXCCID_USBCCID_ESCAPE_YELLOW_LED_OFF                    0x30U

#define PH_EXCCID_USBCCID_ESCAPE_RED_LED_ON                        0x41U
#define PH_EXCCID_USBCCID_ESCAPE_RED_LED_OFF                       0x40U

#define PH_EXCCID_USBCCID_ESCAPE_ALL_LED_OFF                       0xA0U
#define PH_EXCCID_USBCCID_ESCAPE_ALL_LED_ON                        0xA1U

#define PH_EXCCID_USBCCID_ESCAPE_START_POLLING                     0xBBU
#define PH_EXCCID_USBCCID_ESCAPE_STOP_POLLING                      0xCCU

#define PH_EXCCID_USBCCID_PROTOCOL_T1             0x01U
#define PH_EXCCID_USBCCID_PROTOCOL_T0             0x00U
#define PH_EXCCID_USBCCID_ESCAPE_COMMAND_ENABLE   0x01U
#define PH_EXCCID_USBCCID_ESCAPE_COMMAND_DISABLE  0x00U
#define PH_EXCCID_USBCCID_PROTOCOL_T0_LEN         0x05U
#define PH_EXCCID_USBCCID_PROTOCOL_T1_LEN         0x07U

/* CCID PC_to_RDR_XfrBlock Message, Level Parameter Field Values */
#define PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_00    (0x00) /* Command APDU begins and ends with this command */
#define PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_01    (0x01) /* Command APDU begins with this command, and continue in the next PC_to_RDR_XfrBlock */
#define PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_02    (0x02) /* abData field continues a command APDU and ends the APDU command*/
#define PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_03    (0x03) /* abData field continues a command APDU and another block is to follow */
#define PH_EXCCID_CT_PC_RDR_XFRBLK_LEVELPARAM_LSB_10    (0x10) /* empty abData field, continuation of response APDU is expected in the next RDR_to_PC_DataBlock */

/*
 * BULK OUT PC->PN7462AU
 */
typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t  aRxBuff[PH_EXCCID_USBCCID_PN7462AU_MAX_CCID_BUFFER_LEN];
} PH_PACK_STRUCT_END phExCcid_Usb_BulkOut_Msg_t;

/*
 * BULK IN PN7462AU->PC
 */
typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t  aTxBuff[PH_EXCCID_USBCCID_PN7462AU_MAX_CCID_BUFFER_LEN];
} PH_PACK_STRUCT_END phExCcid_Usb_BulkIn_Msg_t;


/* CCID INTERRUPT IN Packet Information */
typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t   bMsgType;
    uint8_t   bStatus;
} PH_PACK_STRUCT_END phExCcid_Usb_IntIn_Msg_t;

/*
 * USB BULK OUT Length Information
 */
typedef PH_PACK_STRUCT_BEGIN struct {
    volatile uint32_t dwLength;
    volatile uint32_t dwIndex;
} PH_PACK_STRUCT_END phExCcid_Usb_Comm_Out_t;

/*
 * USB BULK IN Length Information
 */
typedef PH_PACK_STRUCT_BEGIN struct {
    volatile uint32_t dwLength;
    volatile uint32_t dwWritenLength;
} PH_PACK_STRUCT_END phExCcid_Usb_Comm_In_t;

typedef PH_PACK_STRUCT_BEGIN struct {
    volatile uint8_t bIsBusy;
    volatile uint8_t bProcess_Pend;
} PH_PACK_STRUCT_END phExCcid_Usb_Comm_Status_t;

typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t          bSlotType;
    uint8_t          bProtocolType;
    uint8_t          bCLSlotType;
    uint8_t          bCardPresent;
    uint8_t          bAtrValid;
    uint8_t          bAtrSize;
    uint8_t          aAtr[33];
} PH_PACK_STRUCT_END phExCcid_Slot_Info_t;

typedef PH_PACK_STRUCT_BEGIN struct {
    phRtos_EventHandle_t xCL_Events;
    phRtos_EventHandle_t xCT_Events;
} PH_PACK_STRUCT_END phExCcid_Event_Info_t;

typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t bAuth;
    uint8_t bKeyType;
    uint8_t bMFCKeyLength;
    uint8_t aMFCKey[PH_EXCCID_USBCCID_PN7462AU_MAX_KEY_LEN];
} PH_PACK_STRUCT_END phExCcid_MiFare_Info_t;

typedef PH_PACK_STRUCT_BEGIN struct {
    uint8_t bSuspendEnable;
    uint8_t bAddressed;
    uint8_t bRemoteWakeupEnable;
    uint8_t bCardEnabled;
} PH_PACK_STRUCT_END phExCcid_Usb_Bus_Status_t;

typedef PH_PACK_STRUCT_BEGIN struct {
    volatile uint8_t bOperationMode;
} PH_PACK_STRUCT_END phExCcid_Usb_Operation_Mode_t;

extern phExCcid_Usb_BulkOut_Msg_t     gphExCcid_sUsb_BulkOutMsg;
extern phExCcid_Usb_BulkIn_Msg_t      gphExCcid_sUsb_BulkInMsg;
extern phExCcid_Usb_IntIn_Msg_t       gphExCcid_sUsb_IntInMsg;
extern phExCcid_Usb_Comm_Out_t        gphExCcid_sUsb_Comm_Out;
extern phExCcid_Usb_Comm_In_t         gphExCcid_sUsb_Comm_In;
extern phExCcid_Usb_Comm_Status_t     gphExCcid_sUsb_Status;
extern phExCcid_Slot_Info_t           gphExCcid_sUsb_SlotInfo;
extern phExCcid_MiFare_Info_t         gphExCcid_sUsb_MifareInfo;
extern phExCcid_Usb_Bus_Status_t      gphExCcid_sUsb_Bus_Status;
extern phExCcid_Usb_Operation_Mode_t  gphExCcid_sUsb_Operation_Mode;
extern phExCcid_Event_Info_t          gphExCcid_sUsb_EventInfo;
/*
 * USB Interrupt Handler Functions
 */
void phExCcid_UsbCcid_Usb_Bulk_Out(void);
void phExCcid_UsbCcid_Usb_Bulk_In(void);
void phExCcid_UsbCcid_Usb_IntIn(void);

/*
 * Card Notification Functions
 */
void phExCcid_UsbCcid_CardInserted  (void);
void phExCcid_UsbCcid_CardRemoved   (void);

/*
 * CCID Command-Response Functions
 */
void phExCcid_UsbCcid_Icc_Power_On  (void);
void phExCcid_UsbCcid_GetSlotStatus (void);
void phExCcid_UsbCcid_Icc_Power_Off (void);
void phExCcid_UsbCcid_XfrBlock      (void);
void phExCcid_UsbCcid_GetParameters (void);
void phExCcid_UsbCcid_SetParameters (void);
void phExCcid_UsbCcid_Escape        (void);
void phExCcid_UsbCcid_Icc_Clock     (void);

void phExCcid_UsbCcid_Send_Frame (uint8_t bMsgType, uint8_t bByte1, uint8_t bByte2, uint8_t bByte3);
void phExCcid_UsbCcid_Set_Output_Payload_Length(uint32_t dwPayloadLength);
uint32_t phExCcid_UsbCcid_Get_Input_Payload_Length(void);
uint8_t phExCcid_UsbCcid_CCID_InHdr_Get_Byte(uint8_t bByte);
uint8_t * phExCcid_UsbCcid_Get_Output_Payload_Buffer(void);
uint8_t * phExCcid_UsbCcid_Get_Input_Payload_Buffer(void);
phStatus_t phExCcid_UsbCcid_PCSC_Send_APDU(uint8_t bSW1, uint8_t bSW2, uint16_t wPayloadLength);
phStatus_t phExCcid_UsbCcid_PCSC_SendEscape_APDU(uint8_t bSW1, uint8_t bSW2, uint16_t wPayloadLength);

uint32_t phExCcid_UsbCcid_Get_ATR();
void phExCcid_UsbCcid_ATR_FromAts(uint8_t *pAts);
void phExCcid_UsbCcid_ATR_TypeBL4(uint8_t *pAtqb, uint8_t bMbli);
void phExCcid_UsbCcid_ATR_Felica_Mifare_ICode(uint8_t bSak);
uint32_t phExCcid_UsbCcid_Usb_Send(uint8_t bEP, uint8_t * what, uint32_t how_much);

void phExCcid_UsbCcid_Escape_Function(uint8_t bEscape);
void phExCcid_UsbCcid_Operation_Mode(void);

void phExCcid_UsbCcid_Decode_APDU(void);
void phExCcid_UsbCcid_UsbPostEventCLTask(uint32_t dwEvent);
void phExCcid_UsbCcid_UsbPostEventCTTask(uint32_t dwEvent);

#endif /* PHEXCCID_USBCCID_H */
