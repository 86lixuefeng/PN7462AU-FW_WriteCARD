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
 * phExNFCCcid_Usb_If.c: <This File provides the USB System Specific Informations>
 *
 * Project: PN7426AU
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phhalNvic.h"
#include "phUser.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_regcore.h"
#include "PN7462AU/PN7462AU_usb.h"
#include "PN7462AU/PN7462AU_clkgen.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "phExNFCCcid_LED.h"
#include "phhalTimer.h"
#include "phhalPcr.h"
#include "phhalGpio.h"
#include "phhalHif_Usb.h"
#include "app_usbd_cfg.h"
#include "phExNFCCcid_Usb_If.h"
#include "phExNFCCcid_UsbCcid.h"
#include "phCfg_EE.h"
#include "phhalPmu.h"

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
extern phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define SET_HIF_USB_LOW_PRIORITY  PH_REG_SET_FIELDS_NS( NVIC_IPR1_REG, (0XC0<<16), (0xC0<<16))

//#define PHEXNFCCCID_USB_SELF_POWERED_MODE
#define PHEXNFCCCID_USB_BUS_POWERED_MODE_ENABLE  0x00
#define PHEXNFCCCID_USB_SELF_POWERED_MODE_ENABLE 0x01
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phExNFCCcid_Usb_If_TotalInit(void)
{
    phStatus_t bStatus = PH_ERR_INTERNAL_ERROR;

    SET_HIF_USB_LOW_PRIORITY;

    /* Initialize the USB IP */
    bStatus = phExNFCCcid_Usb_If_Init();

    /* Continue only if the USB is initialized successfully */
    if (bStatus == PH_ERR_SUCCESS)
    {
        /* Initialize the CCID Class */
        phExNFCCcid_Usbd_CCID_Init ();

        /* Connect the USB Cable */
        phExNFCCcid_Usbd_Connect_Enable();
    }
    return bStatus;
}

phStatus_t phExNFCCcid_Usb_If_Init()
{
    phStatus_t bStatus = PH_ERR_INTERNAL_ERROR;

#ifdef PHEXNFCCCID_USB_SELF_POWERED_MODE
    bStatus = phhalHif_Usb_Init(PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP,
                                PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
                                PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT,
								PHEXNFCCCID_USB_SELF_POWERED_MODE_ENABLE);
#else
    bStatus = phhalHif_Usb_Init(PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP,
                                PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
                                PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT,
								PHEXNFCCCID_USB_BUS_POWERED_MODE_ENABLE);
#endif

    if (bStatus == PH_ERR_SUCCESS)
    {
        /* Call the USB HW-Core Initialization */
        phExNFCCcid_Usbd_CORE_Init ();
    }

    return bStatus;
}

#if (PH_EXNFCCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
phStatus_t phExNFCCcid_Usb_If_Suspend()
{
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason = {0};
    phhalPcr_WakeUpConfig_t      sWakeUpConfig;
    phStatus_t eStatus = PH_ERR_SUCCESS;

    sWakeUpConfig.wWakeUpTimerVal  = 2000;
    sWakeUpConfig.bEnableHIFWakeup = 1;
    sWakeUpConfig.bSmBusAddrResp   = 0x0;
    sWakeUpConfig.bI2CAddr         = 0x28;
#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
    sWakeUpConfig.bWakeUpTimer     = 1;
#else
    sWakeUpConfig.bWakeUpTimer     = 0;
#endif
    sWakeUpConfig.bWakeUpRfLdt     = 0;
    sWakeUpConfig.bWakeUpTemp      = 0;
    sWakeUpConfig.bWakeUpPvddLim   = 0;
    sWakeUpConfig.bWakeUpCtPr      = 0;
    sWakeUpConfig.bWakeUpIntAux    = 0;
    sWakeUpConfig.bWakeUpTvddMon   = 0;
    sWakeUpConfig.bWakeUpGpio      = 0;
    sWakeUpConfig.blWakeupVbusLow  = 0;

    if (gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable == 1)
    {
        /* Stop the LED Timer */
        phhalTimer_Stop(pLedTimer);

        /* Switch OFF all LED's */
        phExNFCCcid_All_LED_Off();

        eStatus = phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                                   &sWakeUpConfig, &sStandbyPrevReason);

        if (eStatus == PH_ERR_SUCCESS)
        {
            return eStatus;
        }
        else
        {
            phExNFCCcid_All_LED_Off();
            phExNFCCcid_All_LED_On();
            return eStatus;
        }
    }
    else
    {
        /* Dont do anything */
        return eStatus;
    }
}

void phExNFCCcid_Usb_If_Resume()
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    if (gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable == 1)
    {
        gphExNFCCcid_sUsb_Bus_Status.bSuspendEnable = 0;

        eStatus = phhalHif_Usb_ConfigStart(PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP,
                                           PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
                                           PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT);

        if (eStatus == PH_ERR_SUCCESS)
        {
            phExNFCCcid_All_LED_Off();
            phExNFCCcid_LED_Status(GREEN_LED, LED_ON);
#if (PH_EXNFCCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
            if ((gphExNFCCcid_sUsb_Bus_Status.bRemoteWakeupEnable == 1))
            {
                phExNFCCcid_Usbd_Remote_Wakeup();
                phExNFCCcid_LED_Status(YELLOW_LED, LED_ON);
            }
#endif
        }
        else
        {
            phExNFCCcid_All_LED_Off();
            phExNFCCcid_LED_Status(BLUE_LED, LED_ON);
        }
    }
    else
    {
        return;
    }
}
#endif

