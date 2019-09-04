/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014-2015
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
 * phhalSysSer_gcc.c:  SystemServices entry for GCC Based environment
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-11-18 15:23:43 +0530 (Fri, 18 Nov 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18752 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phhalSysSer.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#ifdef __GNUC__

#if __GNUC__ == 4 && __GNUC_MINOR__ == 8 /*  __GNUC_PATCHLEVEL__ 3 */
/* OK. we have tested and developed magical system services against GCC 4.8.3 */
#	define VALIDATED_GCC_VERSION
#endif
#if __GNUC__ == 4 && __GNUC_MINOR__ == 9 /*  __GNUC_PATCHLEVEL__ 3 */
/* OK. we have also tested and developed magical system services against GCC 4.9.3 */
#	define VALIDATED_GCC_VERSION
#endif
#if __GNUC__ == 5 && __GNUC_MINOR__ == 2 /*  __GNUC_PATCHLEVEL__ 1 */
/* OK. we have also tested and developed magical system services against GCC 5.2.1 */
#	define VALIDATED_GCC_VERSION
#endif
#if __GNUC__ == 5 && __GNUC_MINOR__ == 4 /*  __GNUC_PATCHLEVEL__ 1 */
/* OK. we have also tested and developed magical system services against GCC 5.4.1 */
#	define VALIDATED_GCC_VERSION
#endif
#ifndef VALIDATED_GCC_VERSION
#   warning "System services have not been validated with this version of GCC. Check your GCC Version with 'arm-none-eabi-gcc.exe --version'"
/* Please check the sanity of PN7462AU_ex_phExSystemServices against this version of GCC */
#endif

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/*
 *Function Name     : phhalSysSer_OTP_SetSecrowLock
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_OTP_SetSecrowLock(
    void)
{
    __asm(
        ".EQU phSysSv_OTP_SetSecrowLock   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, #0x00;" /* Set R3 to 0 */
        "MOVS    r1, #0x01;" /* Number for SysSv OTP_SetSecrowLock command. */
        "CPSID   I;"
        "BL phSysSv_OTP_SetSecrowLock;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_SetCPU_Reset
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_SetCPU_Reset(
    void)
{
    __asm(
        ".EQU phSysSv_SetCPU_Reset   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, #0x00;" /* Set R3 to 0 */
        "MOVS    r1, #0x02;" /* Number for SysSv SetCPU_Reset command. */
        "CPSID   I;"
        "BL phSysSv_SetCPU_Reset;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_OTP_SecrowConfig
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_OTP_SecrowConfig(
    phhalSysSer_SetGet_t eSetGet, phhalSysSer_Secrow_Config_t * pConfig)
{
    (void) eSetGet;
    (void) pConfig;

    __asm(
        ".EQU phSysSv_OTP_SecrowConfig   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store eSetGet in r2 */
        "MOVS    r3, r1;" /* Store pConfig in r3 */
        "MOVS    r1, #0x03;" /* Number for SysSv OTP_SecrowConfig command. */
        "CPSID   I;"
        "BL phSysSv_OTP_SecrowConfig;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_USB_Config
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_USB_Config(
    phhalSysSer_SetGet_t eSetGet, phhalSysSer_USBInfo_t * pConfig)
{
    (void) eSetGet;
    (void) pConfig;

    __asm(
        ".EQU phSysSv_USB_Config   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store eSetGet in r2 */
        "MOVS    r3, r1;" /* Store pConfig in r3 */
        "MOVS    r1, #0x04;" /* Number for SysSv USB_Config command. */
        "CPSID   I;"
        "BL phSysSv_USB_Config;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_USB_PrimaryDnldConfig
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_USB_PrimaryDnldConfig(
    phhalSysSer_SetGet_t eSetGet, phhalSysSer_USB_Primary_Dnld_Config_t * pConfig)
{
    (void) eSetGet;
    (void) pConfig;

    __asm(
        ".EQU phSysSv_USB_PrimaryDnldConfig   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store eSetGet in r2 */
        "MOVS    r3, r1;" /* Store pConfig in r3 */
        "MOVS    r1, #0x05;" /* Number for SysSv USB_PrimaryDnldConfig command. */
        "CPSID   I;"
        "BL phSysSv_USB_PrimaryDnldConfig;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_USB_PVDD_Config
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_USB_PVDD_Config(
    phhalSysSer_SetGet_t eSetGet, phhalSysSer_USB_PVDD_Config_t * pConfig)
{
    (void) eSetGet;
    (void) pConfig;

    __asm(
        ".EQU phSysSv_USB_PVDD_Config   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store eSetGet in r2 */
        "MOVS    r3, r1;" /* Store pConfig in r3 */
        "MOVS    r1, #0x06;" /* Number for SysSv USB_PVDD_Config command. */
        "CPSID   I;"
        "BL phSysSv_USB_PVDD_Config;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_SetFlashEEProgramTimeout
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_SetFlashEEProgramTimeout(
    uint32_t dwEEPTimeOut, uint32_t *pwFlashTimeOut)
{
    (void) dwEEPTimeOut;
    (void) *pwFlashTimeOut;

    __asm(
        ".EQU phSysSv_SetFlashEEProgramTimeout   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store dwEEPTimeOut in r2 */
        "MOVS    r3, r1;" /* Store *pwFlashTimeOut in r3 */
        "MOVS    r1, #0x07;" /* Number for SysSv SetFlashEEProgramTimeout command. */
        "CPSID   I;"
        "BL phSysSv_SetFlashEEProgramTimeout;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_SetFlashProgram
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_SetFlashProgram(
    uint32_t dwFlashPageNumber, void * pdata)
{
    (void) dwFlashPageNumber;
    (void) pdata;

    __asm(
        ".EQU phSysSv_SetFlashProgram   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store dwFlashPageNumber in r2 */
        "MOVS    r3, r1;" /* Store pdata in r3 */
        "MOVS    r1, #0x08;" /* Number for SysSv SetFlashProgram command. */
        "CPSID   I;"
        "BL phSysSv_SetFlashProgram;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_GetROM_Version
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_GetROM_Version(
    phhalSysSer_SetGet_t eSetGet, uint32_t * pRomVersion)
{
    (void) eSetGet;
    (void) pRomVersion;

    __asm(
        ".EQU phSysSv_GetROM_Version   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store eSetGet in r2 */
        "MOVS    r3, r1;" /* Store pRomVersion in r3 */
        "MOVS    r1, #0x09;" /* Number for SysSv GetROM_Version command. */
        "CPSID   I;"
        "BL phSysSv_GetROM_Version;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_GetDieID
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_GetDieID(
    phhalSysSer_SetGet_t eSetGet, uint8_t * pDieID)
{
    (void) eSetGet;
    (void) pDieID;

    __asm(
        ".EQU phSysSv_GetDieID   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store eSetGet in r2 */
        "MOVS    r3, r1;" /* Store pDieID in r3 */
        "MOVS    r1, #0x0A;" /* Number for SysSv GetDieID command. */
        "CPSID   I;"
        "BL phSysSv_GetDieID;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_SetSystemServiceExtension
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_SetSystemServiceExtension(
    uint32_t dwCommand, void * pCmdParameters)
{
    (void) dwCommand;
    (void) pCmdParameters;

    __asm(
        ".EQU phSysSv_SetSystemServiceExtension   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store dwCommand in r2 */
        "MOVS    r3, r1;" /* Store pCmdParameters in r3 */
        "MOVS    r1, #0x0B;" /* Number for SysSv SetSystemServiceExtension command. */
        "CPSID   I;"
        "BL phSysSv_SetSystemServiceExtension;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_SetConfig
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_SetConfig(
    uint32_t dwConfig,uint32_t * pdwValue)
{
    (void) dwConfig;
    (void) pdwValue;

    __asm(
        ".EQU phSysSv_CLIF_SetConfig   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store dwConfig in r2 */
        "MOVS    r3, r1;" /* Store pdwValue in r3 */
        "MOVS    r1, #0x0C;" /* Number for SysSv CLIF_SetConfig command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_SetConfig;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_CardModeActivate
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_CardModeActivate(
    uint32_t dwCmd,phSysSv_Clif_CardMode_TypeA_Param_t * pData)
{
    (void) dwCmd;
    (void) pData;

    __asm(
        ".EQU phSysSv_CLIF_CardModeActivate   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store dwCmd in r2 */
        "MOVS    r3, r1;" /* Store pData in r3 */
        "MOVS    r1, #0x0D;" /* Number for SysSv CLIF_CardModeActivate command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_CardModeActivate;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_MF_Authenticate
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_MF_Authenticate(
    phSysSv_Clif_MF_Authenticate_Step_t dwStep,phSysSv_Clif_MF_Authenticate_Parameters_t * pData)
{
    (void) dwStep;
    (void) pData;

    __asm(
        ".EQU phSysSv_CLIF_MF_Authenticate   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, r0;" /* Store dwStep in r2 */
        "MOVS    r3, r1;" /* Store pData in r3 */
        "MOVS    r1, #0x0E;" /* Number for SysSv CLIF_MF_Authenticate command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_MF_Authenticate;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_Config_TxRxBuffer
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_Config_TxRxBuffer(
    phSysSv_Clif_TxRxBuffer_Parameters_t * pData)
{
    (void) pData;

    __asm(
        ".EQU phSysSv_CLIF_Config_TxRxBuffer   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, r0;" /* Store pData in r3 */
        "MOVS    r1, #0x0F;" /* Number for SysSv CLIF_Config_TxRxBuffer command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_Config_TxRxBuffer;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_Config_AnalogTestBus
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_Config_AnalogTestBus(
    uint32_t * pData)
{
    (void) pData;

    __asm(
        ".EQU phSysSv_CLIF_Config_AnalogTestBus   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, r0;" /* Store pData in r3 */
        "MOVS    r1, #0x12;" /* Number for SysSv CLIF_Config_AnalogTestBus command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_Config_AnalogTestBus;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_Config_DigitalTestBus
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_Config_DigitalTestBus(
    uint8_t * pData)
{
    (void) pData;

    __asm(
        ".EQU phSysSv_CLIF_Config_DigitalTestBus   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, r0;" /* Store pData in r3 */
        "MOVS    r1, #0x13;" /* Number for SysSv CLIF_Config_DigitalTestBus command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_Config_DigitalTestBus;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_Lock_AnalogTestBus
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_Lock_AnalogTestBus(
    void * pDummy)
{
    (void) pDummy;

    __asm(
        ".EQU phSysSv_CLIF_Lock_AnalogTestBus   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, r0;" /* Store pDummy in r3 */
        "MOVS    r1, #0x14;" /* Number for SysSv CLIF_Lock_AnalogTestBus command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_Lock_AnalogTestBus;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}
/*
 *Function Name     : phhalSysSer_CLIF_Lock_DigitalTestBus
 *Description       :
 *
 *Input Parameters  :
 *Output Parameters :
 *
 *Note:             :
 */
phStatus_t __attribute__((naked)) phhalSysSer_CLIF_Lock_DigitalTestBus(
    void * pDummy)
{
    (void) pDummy;

    __asm(
        ".EQU phSysSv_CLIF_Lock_DigitalTestBus   ,  0x000050C1;"

        "PUSH    {lr};"
        "PUSH    {r1-r7};"

        "MOVS    r2, #0x00;" /* Set R2 to 0 */
        "MOVS    r3, r0;" /* Store pDummy in r3 */
        "MOVS    r1, #0x15;" /* Number for SysSv CLIF_Lock_DigitalTestBus command. */
        "CPSID   I;"
        "BL phSysSv_CLIF_Lock_DigitalTestBus;"
        "CPSIE   I;"
        "ISB;"
        "POP    {r1-r7};"
        "POP    {pc};"
    );
}

#endif /* __GNUC__ */


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
