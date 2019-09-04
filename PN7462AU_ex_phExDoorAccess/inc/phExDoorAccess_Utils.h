#ifndef PHEXDOORACCESSNONEXPORTCONTROLUTILS_H
#define PHEXDOORACCESSNONEXPORTCONTROLUTILS_H

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include <ph_Datatypes.h>
#include "phhalHif.h"
#include <ph_Status.h>
#include "ph_Reg.h"
#include "phUser.h"
#include "phhalGpio.h"

#include "ph_Registers.h"
#include "PN7462AU/PN7462AU_timers.h"

#define PH_EXDOORACCESS_UTILS_HSU_BUFFER_SIZE 200

extern volatile uint32_t gphExDoorAccess_Utils_CounterValue;
extern volatile float gphExDoorAccess_Utils_Timeinms;
extern phhalHif_Config_t gphExDoorAccess_Utils_HsuHifConfig;
extern uint8_t gphExDoorAccess_Utils_SysHsuTxBuffer[PH_EXDOORACCESS_UTILS_HSU_BUFFER_SIZE];
extern uint16_t str_index;
static const char hex_numbers[] = "0123456789ABCDEF";

void phExDoorAccess_Utils_HsuHifConfig(void);
void phExDoorAccess_Utils_HsuHifReceiveCallBack(uint32_t dwIntStatus);
void phExDoorAccess_Utils_HsuHifTransmitCallBack(uint32_t dwIntStatus);

void phExDoorAccess_Utils_HsuHifErrorCallBack(uint32_t dwIntStatus);
void phExDoorAccess_Utils_Hsu_Print(uint8_t *inBuf);
void phExDoorAccess_Utils_Fill_Tx_Buffer(char inString[]);
void phExDoorAccess_Utils_FillUID_Tx_Buffer(uint8_t *inUIDString,uint8_t UID_Length);

void phExDoorAccess_Utils_Config_GPIO(void);
void phExDoorAccess_Utils_Glow_Error_LEDS(void);
void phExDoorAccess_Utils_Glow_Success_LEDS(void);
void phExDoorAccess_Utils_Glow_Authetication_Success_LED(void);
void phExDoorAccess_Utils_Glow_Authetication_Failure_LED(void);
void phExDoorAccess_Utils_Timer1_Enable(void);
void phExDoorAccess_Utils_Timer1_Start(void);
void phExDoorAccess_Utils_Get_Time(char *str);

#endif
