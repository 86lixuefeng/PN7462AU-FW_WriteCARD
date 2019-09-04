/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
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
 * ROM System Services interface file.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-07-11 13:30:09 +0530 (Mon, 11 Jul 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18565 $ (v04.13.00)
 */

#ifndef PHHALHWSYSSER_H
#define PHHALHWSYSSER_H

/** \addtogroup phhalSysSer
 *
 * ROM Interface (System Services) APIs
 *
 * The PN7462AU System Services are protected services that provide customers the following APIs
 *
 * -# \ref phSysSv_CLIF
 * -# \ref phSysSv_MISC
 * -# \ref phSysSv_PVDD
 * -# \ref phSysSv_SECROW
 * -# \ref phSysSv_TESTBus
 * -# \ref phSysSv_USBDL
 *
 * These APIs are implemented in the ROM Code with privileged r/w access rights to SECROW,
 * DFT block, protected registers of CLIF and NXP EE Section.
 *
 * Where applicable, the parameters passed should always be from RAM.
 *
 *  @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "ph_Status.h"
#include "phSysSv_Data.h"
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * @name To set or get the data from systemService.
 * @{
 */
typedef enum {
    /** \b 0x00 */ PH_SYSRV_SET_DATA, /**< Get data from the Code */
    /** \b 0x01 */ PH_SYSRV_GET_DATA, /**< Read data from the Code */
    /** @internal */ PH_SYSRV_GET_SET_U32  = 0x7FFFFFFF /* Promote enum to 32bit Number */
} phhalSysSer_SetGet_t;
/** @} */

/** System service call for disabling the DCDC */
#define PHSYSSV_CTIF_DCDC_DISABLE               0x06

/** @} */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/** @ingroup phSysSv_SECROW
 *
 * @{
 */

/**
* Secrow configuration structure.
*
*  - EnableCWP : 1 byte  ( only `1` is allowed)
*  - RSTN_PullDown : 1 byte ( `0`/`1` can be set)
*  - TVDD_Config3 : 1 byte (`0`/`1` can be set)
*  - Block_SWD : 1 byte ( only `1` is allowed)
*/
typedef PH_PACK_STRUCT_BEGIN struct phhalSysSer_Secrow_Config {
   uint8_t bEnableCWP[1];      /**< 1 byte  ( only '1' is allowed) OTP */
   uint8_t bRSTN_PullDown[1];  /**< 1 byte ( 0 /1 can be set) */
   uint8_t bTVDD_Config3[1];   /**< 1 byte (0 /1 can be set) */
   uint8_t bBlockSWD[1];       /**< 1 byte ( only '1' is allowed) OTP */
} PH_PACK_STRUCT_END phhalSysSer_Secrow_Config_t;  /**< typedef for \ref phhalSysSer_Secrow_Config */


/**
 * The SECROW configuration can be set or retrieved via phhalSysSer_OTP_SecrowConfig API.
 * @param eSetGet: Specifies Set/Get SECROW configuration. 0: To Set, 1: To Get configurations.
 * @param pConfig: pointer to RAM containing the phhalSysSer_Secrow_Config_t structure;
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 * Sample Usage:
 * @code
 *     phStatus_t status;
 *     phhalSysSer_Secrow_Config_t SecRow;
 *     phUser_MemSet(&SecRow , 0 , sizeof(SecRow));
 *     status = phhalSysSer_OTP_SecrowConfig (PH_SYSRV_GET_DATA, &SecRow);
 * @endcode
 *
 */
phStatus_t phhalSysSer_OTP_SecrowConfig (phhalSysSer_SetGet_t eSetGet, phhalSysSer_Secrow_Config_t *pConfig);

/**
 * phhalSysSer_OTP_SetSecrowLock performs the EEPROM secrow lock.
 * \note : This service locks the SECROW for any more modifications and is irreversible
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 */
phStatus_t phhalSysSer_OTP_SetSecrowLock(void);

/** @} */

/** @ingroup phSysSv_USBDL
 * @{
 */

/**
 * USB primary download configuration structure.
 */
typedef PH_PACK_STRUCT_BEGIN struct phhalSysSer_USB_Primary_Dnld_Config {
    uint8_t Primary_Dnld_Disable[1];        /**< 1 byte (Setting it disables primary download. ) */
    uint8_t CRP_Level[1];       /**< 1 byte (CRP) */
    uint8_t DRP_Level[1];       /**< 1 byte (DRP) */
} PH_PACK_STRUCT_END phhalSysSer_USB_Primary_Dnld_Config_t;  /**< typedef for \ref phhalSysSer_USB_Primary_Dnld_Config */

/**
 * USB configuration structure.
 */
typedef PH_PACK_STRUCT_BEGIN struct phhalSysSer_USBInfo
{
    uint8_t USBDesc_PID[2];                       /**< PN7462AU Product ID */
    uint8_t USBDesc_VID[2];                       /**< NXP Vendor ID */
    uint8_t MfrStr_Len[1];                        /**< Length of "NXP" string */
    uint8_t MfrString[16];                        /**< "NXP" */
    uint8_t ProductStr_Len[1];                    /**< Length of "PN7462AU" */
    uint8_t ProductString[16];                    /**< "PN7462AU" */
    uint8_t VolumeString[11];                     /**< Drive Name */
    /**
     * Self Powered or BusPowerd [7..0]
     *
     *  - Bit [0-4] Reserved. Should be 0
     *  - Bit 5 - Remote Wakeup
     *  - Bit 6 - 1 = Self powered, 0 = Bus Powered
     *  - Bit 7 - Reserved. Should be 1
     *  */
    uint8_t USBDesc_Attributes[1];
    /**
     * The current x2 (milli amps) consumed by the device
     * (e.g. 0x32 = 50 decimal = 100 milli amps )
     *  */
    uint8_t USBDesc_MaxCurr[1];

    uint8_t Reserved0[1];                        /**< Reserved byte0. */
    uint8_t Reserved1[1];                        /**< Reserved byte1. */
    uint8_t USB_Discharge_Time[1];               /**< Discharge Timeout */
    uint8_t USB_XTAL_HW_OK_Timeout[1];           /**< how much to wait for XTAL to be OK - HW. (Unit -milliseconds) */
    uint8_t USB_XTAL_SW_OK_Timeout[1];           /**< how much to wait for XTAL to be OK - SW (Unit - milliseconds) */

    /** Parameter for configuring input Clock Detector, must be read from EEPROM. [PT_SC2767] */
    uint8_t USB_PLL_DetectionWindowLen[1];
    /** Parameter for configuring input Clock Detector, must be read from EEPROM. [PT_SC2767] */
    uint8_t USB_PLL_ClkEdgesNum[1];
} PH_PACK_STRUCT_END phhalSysSer_USBInfo_t;     /**< typedef for \ref phhalSysSer_USBInfo */

/**
 * USB PVDD configuration structure.
 */
typedef PH_PACK_STRUCT_BEGIN struct phhalSysSer_USB_PVDD_Config {
    uint8_t PVDD_InTimeout[2];    /**< Pvdd in timeout. */
    uint8_t VBUS_MON2Timeout[2];  /**< Vbus monitor 2 timeout. */
    uint8_t PVDD_Source[1];       /**< Pvdd source. */
    uint8_t PVDD_Startup_Time[2]; /**< Pvdd startup time. */
} PH_PACK_STRUCT_END phhalSysSer_USB_PVDD_Config_t; /**< typedef for \ref phhalSysSer_USB_PVDD_Config */


/**
 * The USB configuration can be set or retrieved via this phhalSysSer_USB_Config API.
 * @param eSetGet: Specifies Set/Get USB configuration. 0: To Set, 1: To Get configurations.
 * @param pConfig: pointer to RAM containing the phhalSysSer_USBInfo_t structure;
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 *
 * Sample Usage:
 * @code
 *      phhalSysSer_USBInfo_t USB_Config;
 *      status = phhalSysSer_USB_Config(PH_SYSRV_GET_DATA, &USB_Config);
 *      PH_USER_ASSERT(PH_ERR_SUCCESS == status);
 *      USB_Config.USB_XTAL_HW_OK_Timeout[0] = 0x05;
 *      status = phhalSysSer_USB_Config(PH_SYSRV_SET_DATA, &USB_Config);
 * @endcode
 *
 */
phStatus_t phhalSysSer_USB_Config(phhalSysSer_SetGet_t eSetGet, phhalSysSer_USBInfo_t *pConfig);

/**
 * The Primary downlaod configuration can be set or retrieved via phhalSysSer_USB_PrimaryDnldConfig API.
 * This is system api that allows customer to set the Code Read Protection Level and Data Read Protection level.
 * The user can disable the primary download which is irreversibly and sets an EEPROM location.
 * During the ROM Boot, this location is checkedand the USB primary download is never entered
 * if the EEPROM indicates primary download is disabled. It is expected that this system api is called
 * only during production since the EEPROM location is not anti-teared.
 * @param eSetGet: Specifies Set/Get Primary downlaod configuration. 0: To Set, 1: To Get configurations.
 * @param pConfig: pointer to RAM containing the phhalSysSer_USB_Primary_Dnld_Config_t structure;
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 *
 * Sample Usage:
 * @code
 *      phhalSysSer_USB_Primary_Dnld_Config_t DnldConfig
 *      status = phhalSysSer_USB_PrimaryDnldConfig(PH_SYSRV_GET_DATA, &DnldConfig);
 *      PH_USER_ASSERT(PH_ERR_SUCCESS == status);
 *      DnldConfig.CRP_Level[0] = 1;
 *      status = phhalSysSer_USB_PrimaryDnldConfig(PH_SYSRV_SET_DATA, &DnldConfig);
 * @endcode
 *
 */
phStatus_t phhalSysSer_USB_PrimaryDnldConfig(phhalSysSer_SetGet_t eSetGet, phhalSysSer_USB_Primary_Dnld_Config_t *pConfig);

/** @} */

/** @ingroup phSysSv_PVDD
 * @{ */

/**
 * The user shall use phhalSysSer_USB_PVDD_Config API to program according to his HW configuration whether the
 * HW is supplied with external PVDD or the ROM Boot should turn on the internal LDO to generate the PVDD
 * @param eSetGet: Specifies Set/Get configuration. 0: To Set, 1: To Get configurations.
 * @param pConfig: pointer to RAM containing the phhalSysSer_USB_PVDD_Config_t structure;
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 * Sample Usage:
 * @code
 *   phStatus_t status;
 *   phhalSysSer_USB_PVDD_Config_t PVDD_Config;
 *   phUser_MemSet(&PVDD_Config, 0, sizeof(PVDD_Config));
 *   status = phhalSysSer_USB_PVDD_Config(PH_SYSRV_GET_DATA, &PVDD_Config);
 * @endcode
 */
phStatus_t phhalSysSer_USB_PVDD_Config(phhalSysSer_SetGet_t eSetGet, phhalSysSer_USB_PVDD_Config_t *pConfig);

/** @} */

/** @ingroup phSysSv_MISC
 * @{
 */

/**
 * phhalSysSer_SetCPU_Reset performs cpu reset only and directly boots from Flash.
 * It is not expected to return if it return then it is an error scenario.
 *
 * @return  Status code of type phStatus_t.
 */
phStatus_t phhalSysSer_SetCPU_Reset(void);

/**
 * phhalSysSer_SetFlashProgram performs a page write in Flash.
 * @param dwFlashPageNumber: Page flash number.
 * @param pdata: Pointer to the data in RAM.
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 * @warning This API is not expected to be used directly by the programmer/developer.
 * Programmer/developer is expected to use the APIs provided in @ref phhalFlash
 */
phStatus_t phhalSysSer_SetFlashProgram(uint32_t dwFlashPageNumber, void *pdata);

/**
 * phhalSysSer_GetROM_Version API returns the ROM Version.
 * @param eSetGet: Will be always Get.
 * @param pRomVersion: Pointer to RAM to store ROM version.
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 * Sample Usage:
 * @code
 *   uint32_t dwRomVersion=0;
 *   phStatus_t status;
 *   status = phhalSysSer_GetROM_Version(1, &dwRomVersion);
 * @endcode
 */
phStatus_t phhalSysSer_GetROM_Version(phhalSysSer_SetGet_t eSetGet, uint32_t *pRomVersion);

/**
 * phhalSysSer_GetROM_Version API returns the 16 byte Die-ID.
 * @param eSetGet: Will be always Get.
 * @param pDieID: Pointer to RAM to store Die-id.
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 * Sample Usage:
 * @code
 *    uint8_t pDieID[16];
 *    phStatus_t status ;
 *    status = phhalSysSer_GetDieID(1, &pDieID[0]);
 * @endcode
 */
phStatus_t phhalSysSer_GetDieID(phhalSysSer_SetGet_t eSetGet, uint8_t *pDieID);


/**
 * System service to update the Timeout values for EEPROM/FLASH write operations performed by System service Api's only.
 *
 * @param dwEEPTimeOut EEPROM write Timeout value in ticks.
 * @param pwFlashTimeOut Pointer to Flash write Timeout value in ticks.
 * @return Status of the API
 *
 * Sample Usage:
 * @code
 *   phStatus_t status;
 *   uint32_t dwPFTimeOut = 5000;
 *   uint32_t dwEEPTimeOut = 5000;
 *   status = phhalSysSer_SetFlashEEProgramTimeout(dwEEPTimeOut , &dwPFTimeOut);
 * @endcode
 */
phStatus_t phhalSysSer_SetFlashEEProgramTimeout(uint32_t dwEEPTimeOut, uint32_t *pwFlashTimeOut );

/**
 * phhalSysSer_SetSystemServiceExtension API if for Future Use.
 * @param dwCommand: For future use.
 * @param pCmdParameters: Pointer to command parameters.
 * @return Status of the API
 * \retval #PH_ERR_SUCCESS Success status if operation completed successfully else failure.
 *
 * @warning This API is not supposed to be used by developer/programmer directly.
 */
phStatus_t phhalSysSer_SetSystemServiceExtension(uint32_t dwCommand, void *pCmdParameters);

/** @} */

/** @ingroup phSysSv_CLIF
 * @{
 */

/**
 * This function will enable and configure the Card Mode Activation sub module.
 * @warning Do not attempt to change the Transmit Buffer while CMA is enabled.
 *
 * @param[in] dwCmd Defines the action to be performed on the CMA module \see phSysSv_Clif_CardMode_TypeA_Param_t
 * @param[in,out] pData Used to initialize, control and get state information for CMA module
 * \see phSysSv_Clif_CMTypeAParam_t
 *
 * @return Status of operation
 * @retval #PH_ERR_SUCCESS Operation Successful.
 * @maskedret #PH_ERR_USE_CONDITION Condition to use the function is not satisfied.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid address is been passed.
 *
 * @warning This API is not supposed to be used by developer/programmer directly.
 * These APIs are used by APIs of @ref RFHal
 */
phStatus_t phhalSysSer_CLIF_CardModeActivate(uint32_t dwCmd, phSysSv_Clif_CardMode_TypeA_Param_t *pData);

/**
 * This function will enable the MiFare Crypto sub module which is used for encrypted communication.
 * @warning Correct Sequence of the calls for this function with @em dwStep for each authentication cycle
 * needs to be performed
 * - #PH_SYSSV_CLIF_MFAUTH_LOADKEY
 * - #PH_SYSSV_CLIF_MFAUTH_PREPARE
 *
 * @note This function depends upon Rng for random number generation used in Mifare Authentication.
 *
 * @param[in] dwStep Step of Mifare Encryption Block configuration in the complete Mifare Authentication
 * algorithm \ref phSysSv_Clif_MF_Authenticate_Step_t.
 * @param[in,out] pData used to initialize the Mifare Crypto and return Authentication information.
 * \ref phSysSv_Clif_MF_Authenticate_Parameters_t.
 *
 * @return Status of operation
 * @retval #PH_ERR_SUCCESS Operation Successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Invalid paramter passed.
 * @maskedret #PH_ERR_INTERNAL_ERROR In case the Random number generation fails.
 *
 * @warning This API is not supposed to be used by developer/programmer directly.
 * These APIs are used by APIs of @ref RFHal
 */
phStatus_t phhalSysSer_CLIF_MF_Authenticate(phSysSv_Clif_MF_Authenticate_Step_t dwStep,
    phSysSv_Clif_MF_Authenticate_Parameters_t *pData);

/**
 * This function will set-up the Transmit and Receive Buffer.
 * @warning Card Mode Activate Module must not be active else the TX buffer initialization will fail.
 *
 * @warning The @em phSysSv_Clif_TxRxBuffer_Parameters_t.pRxBuffer Field for RX buffer needs to be DWORD aligned
 *
 * @warning DO NOT set up the Tx buffer while CLIF is in TRANSMITTING state,
 * and DO NOT set up the Rx Buffer while the Receiver is enabled.
 *
 * @param[in] pData Initialize either or both TX and RX Buffers. \ref phSysSv_Clif_TxRxBuffer_Parameters_t Type
 *
 * @return Status of operation
 * @retval #PH_ERR_SUCCESS Operation Successful.
 * @maskedret #PH_ERR_INVALID_PARAMETER Buffer passed are not in RAM.
 *
 * @warning This API is not supposed to be used by developer/programmer directly.
 * These APIs are used by APIs of @ref RFHal
 */
phStatus_t phhalSysSer_CLIF_Config_TxRxBuffer(phSysSv_Clif_TxRxBuffer_Parameters_t* pData);

/** @} */

/** @ingroup phSysSv_TESTBus
 * @{
 */


/**
 * Configure the CLIF analog test bus
 *
 * @param pData parameters for the configuration
 * @return API Status of the Configuration
 *
 * Sample Usage:
 * @code
 *      uint32_t dwClifAnalogTestBus[] = { 0x9B, 0x00201};
 *      status = phhalSysSer_CLIF_Config_AnalogTestBus(&dwClifAnalogTestBus[0]);
 * @endcode
 */
phStatus_t phhalSysSer_CLIF_Config_AnalogTestBus(uint32_t * pData);

/**
 * Configure the CLIF digital test bus
 *
 * @param pData parameters for the configuration
 * @return
 *
 * Sample Usage:
 * @code
 *
 *   uint8_t aTestBus1Config[] = {0x02, PH_EXMAIN_TESTBUS1_SELECT};
 *   uint8_t aTestBus2Config[] = {0x06, PH_EXMAIN_TESTBUS2_SELECT};
 *
 *   Enable the Digital Test Bus.
 *   wStatus = phhalSysSer_CLIF_Config_DigitalTestBus(&aTestBus1Config[0]);
 *   PH_USER_ASSERT(wStatus == PH_ERR_SUCCESS);
 *   wStatus = phhalSysSer_CLIF_Config_DigitalTestBus(&aTestBus2Config[0]);
 *   PH_USER_ASSERT(wStatus == PH_ERR_SUCCESS);
 *
 * @endcode
 */
phStatus_t phhalSysSer_CLIF_Config_DigitalTestBus(uint8_t * pData);

/**
 * LOCK the Analog Test Bus.
 *
 * Once locked, Analog Test Bus of CLIF can not be used.
 *
 * @param pDummy Pointer a dummy location in RAM.
 * @return API Status
 *
 * Sample Usage:
 * @code
 *     phStatus_t status;
 *     uint8_t aData;
 *     status = phhalSysSer_CLIF_Lock_AnalogTestBus(&aData);
 * @endcode
 */
phStatus_t phhalSysSer_CLIF_Lock_AnalogTestBus(void * pDummy);

/**
 * LOCK the Digital Test Bus.
 *
 * Once locked, Digital Test Bus of CLIF can not be used.
 *
 * @param pDummy Pointer a dummy location in RAM.
 * @return Sample Usage
 *
 * Sample Usage:
 * @code
 *     phStatus_t status;
 *     uint8_t aData;
 *     status = phhalSysSer_CLIF_Lock_DigitalTestBus(&aData);
 * @endcode
 */
phStatus_t phhalSysSer_CLIF_Lock_DigitalTestBus(void * pDummy);

/** @} */

#endif /* PHHALHWSYSSER_H */
