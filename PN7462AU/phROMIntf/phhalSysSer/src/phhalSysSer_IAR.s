/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * phhalSysSer_IAR.s:  SystemServices entry for IAR Based environment
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-03-17 13:51:23 +0530 (Tue, 17 Mar 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 10353 $
 */


    MODULE  ?cisr
    SECTION .romintf:CODE




    PUBLIC phhalSysSer_OTP_SetSecrowLock
    THUMB

phSysSv_OTP_SetSecrowLock      EQU     0x000050C1

phhalSysSer_OTP_SetSecrowLock:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, #0x00; /* Set R3 to 0 */
    movs    r1, #0x01; /* Number for SysSv OTP_SetSecrowLock command. */

    CPSID   I
    BL phSysSv_OTP_SetSecrowLock
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_SetCPU_Reset
    THUMB

phSysSv_SetCPU_Reset      EQU     0x000050C1

phhalSysSer_SetCPU_Reset:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, #0x00; /* Set R3 to 0 */
    movs    r1, #0x02; /* Number for SysSv SetCPU_Reset command. */

    CPSID   I
    BL phSysSv_SetCPU_Reset
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_OTP_SecrowConfig
    THUMB

phSysSv_OTP_SecrowConfig      EQU     0x000050C1

phhalSysSer_OTP_SecrowConfig:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store eSetGet in r2 */
    movs    r3, r1; /* Store pConfig in r3 */
    movs    r1, #0x03; /* Number for SysSv OTP_SecrowConfig command. */

    CPSID   I
    BL phSysSv_OTP_SecrowConfig
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_USB_Config
    THUMB

phSysSv_USB_Config      EQU     0x000050C1

phhalSysSer_USB_Config:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store eSetGet in r2 */
    movs    r3, r1; /* Store pConfig in r3 */
    movs    r1, #0x04; /* Number for SysSv USB_Config command. */

    CPSID   I
    BL phSysSv_USB_Config
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_USB_PrimaryDnldConfig
    THUMB

phSysSv_USB_PrimaryDnldConfig      EQU     0x000050C1

phhalSysSer_USB_PrimaryDnldConfig:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store eSetGet in r2 */
    movs    r3, r1; /* Store pConfig in r3 */
    movs    r1, #0x05; /* Number for SysSv USB_PrimaryDnldConfig command. */

    CPSID   I
    BL phSysSv_USB_PrimaryDnldConfig
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_USB_PVDD_Config
    THUMB

phSysSv_USB_PVDD_Config      EQU     0x000050C1

phhalSysSer_USB_PVDD_Config:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store eSetGet in r2 */
    movs    r3, r1; /* Store pConfig in r3 */
    movs    r1, #0x06; /* Number for SysSv USB_PVDD_Config command. */

    CPSID   I
    BL phSysSv_USB_PVDD_Config
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_SetFlashEEProgramTimeout
    THUMB

phSysSv_SetFlashEEProgramTimeout      EQU     0x000050C1

phhalSysSer_SetFlashEEProgramTimeout:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store dwEEPTimeOut in r2 */
    movs    r3, r1; /* Store *pwFlashTimeOut in r3 */
    movs    r1, #0x07; /* Number for SysSv SetFlashEEProgramTimeout command. */

    CPSID   I
    BL phSysSv_SetFlashEEProgramTimeout
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_SetFlashProgram
    THUMB

phSysSv_SetFlashProgram      EQU     0x000050C1

phhalSysSer_SetFlashProgram:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store dwFlashPageNumber in r2 */
    movs    r3, r1; /* Store pdata in r3 */
    movs    r1, #0x08; /* Number for SysSv SetFlashProgram command. */

    CPSID   I
    BL phSysSv_SetFlashProgram
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_GetROM_Version
    THUMB

phSysSv_GetROM_Version      EQU     0x000050C1

phhalSysSer_GetROM_Version:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store eSetGet in r2 */
    movs    r3, r1; /* Store pRomVersion in r3 */
    movs    r1, #0x09; /* Number for SysSv GetROM_Version command. */

    CPSID   I
    BL phSysSv_GetROM_Version
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_GetDieID
    THUMB

phSysSv_GetDieID      EQU     0x000050C1

phhalSysSer_GetDieID:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store eSetGet in r2 */
    movs    r3, r1; /* Store pDieID in r3 */
    movs    r1, #0x0A; /* Number for SysSv GetDieID command. */

    CPSID   I
    BL phSysSv_GetDieID
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_SetSystemServiceExtension
    THUMB

phSysSv_SetSystemServiceExtension      EQU     0x000050C1

phhalSysSer_SetSystemServiceExtension:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store dwCommand in r2 */
    movs    r3, r1; /* Store pCmdParameters in r3 */
    movs    r1, #0x0B; /* Number for SysSv SetSystemServiceExtension command. */

    CPSID   I
    BL phSysSv_SetSystemServiceExtension
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_SetConfig
    THUMB

phSysSv_CLIF_SetConfig      EQU     0x000050C1

phhalSysSer_CLIF_SetConfig:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store dwConfig in r2 */
    movs    r3, r1; /* Store pdwValue in r3 */
    movs    r1, #0x0C; /* Number for SysSv CLIF_SetConfig command. */

    CPSID   I
    BL phSysSv_CLIF_SetConfig
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_CardModeActivate
    THUMB

phSysSv_CLIF_CardModeActivate      EQU     0x000050C1

phhalSysSer_CLIF_CardModeActivate:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store dwCmd in r2 */
    movs    r3, r1; /* Store pData in r3 */
    movs    r1, #0x0D; /* Number for SysSv CLIF_CardModeActivate command. */

    CPSID   I
    BL phSysSv_CLIF_CardModeActivate
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_MF_Authenticate
    THUMB

phSysSv_CLIF_MF_Authenticate      EQU     0x000050C1

phhalSysSer_CLIF_MF_Authenticate:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, r0; /* Store dwStep in r2 */
    movs    r3, r1; /* Store pData in r3 */
    movs    r1, #0x0E; /* Number for SysSv CLIF_MF_Authenticate command. */

    CPSID   I
    BL phSysSv_CLIF_MF_Authenticate
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_Config_TxRxBuffer
    THUMB

phSysSv_CLIF_Config_TxRxBuffer      EQU     0x000050C1

phhalSysSer_CLIF_Config_TxRxBuffer:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, r0; /* Store pData in r3 */
    movs    r1, #0x0F; /* Number for SysSv CLIF_Config_TxRxBuffer command. */

    CPSID   I
    BL phSysSv_CLIF_Config_TxRxBuffer
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_Config_AnalogTestBus
    THUMB

phSysSv_CLIF_Config_AnalogTestBus      EQU     0x000050C1

phhalSysSer_CLIF_Config_AnalogTestBus:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, r0; /* Store pData in r3 */
    movs    r1, #0x12; /* Number for SysSv CLIF_Config_AnalogTestBus command. */

    CPSID   I
    BL phSysSv_CLIF_Config_AnalogTestBus
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_Config_DigitalTestBus
    THUMB

phSysSv_CLIF_Config_DigitalTestBus      EQU     0x000050C1

phhalSysSer_CLIF_Config_DigitalTestBus:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, r0; /* Store pData in r3 */
    movs    r1, #0x13; /* Number for SysSv CLIF_Config_DigitalTestBus command. */

    CPSID   I
    BL phSysSv_CLIF_Config_DigitalTestBus
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_Lock_AnalogTestBus
    THUMB

phSysSv_CLIF_Lock_AnalogTestBus      EQU     0x000050C1

phhalSysSer_CLIF_Lock_AnalogTestBus:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, r0; /* Store pDummy in r3 */
    movs    r1, #0x14; /* Number for SysSv CLIF_Lock_AnalogTestBus command. */

    CPSID   I
    BL phSysSv_CLIF_Lock_AnalogTestBus
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}




    PUBLIC phhalSysSer_CLIF_Lock_DigitalTestBus
    THUMB

phSysSv_CLIF_Lock_DigitalTestBus      EQU     0x000050C1

phhalSysSer_CLIF_Lock_DigitalTestBus:
    PUSH    {lr}
    PUSH    {r1-r7}

    movs    r2, #0x00; /* Set R2 to 0 */
    movs    r3, r0; /* Store pDummy in r3 */
    movs    r1, #0x15; /* Number for SysSv CLIF_Lock_DigitalTestBus command. */

    CPSID   I
    BL phSysSv_CLIF_Lock_DigitalTestBus
    CPSIE   I
    ISB

    POP    {r1-r7}
    POP    {pc}



        END
