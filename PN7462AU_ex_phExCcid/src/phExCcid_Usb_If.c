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
 * phExCcid_Usb_If.c: <This File provides the USB System Specific Informations>
 *
 * Project: PN7426AU
 * $Date: 2015-05-08 12:19:15 +0530 (Fri, 08 May 2015) $
 * $Author: nxp86397 $
 * $Revision: 1150 $ (v05.19.00)
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
#include "phExCcid_LED.h"
#include "phhalTimer.h"
#include "phhalPcr.h"
#include "phhalGpio.h"
#include "phhalHif_Usb.h"
#include "app_usbd_cfg.h"
#include "phExCcid_Usb_If.h"
#include "phExCcid_UsbCcid.h"
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

//#define PHEXCCID_USB_SELF_POWERED_MODE
#define PHEXCCID_USB_BUS_POWERED_MODE_ENABLE  0x00
#define PHEXCCID_USB_SELF_POWERED_MODE_ENABLE 0x01
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phExCcid_Usb_If_TotalInit(void)
{
    phStatus_t bStatus = PH_ERR_INTERNAL_ERROR;

    //SET_HIF_USB_LOW_PRIORITY;

    /* Initialize the USB IP */
    bStatus = phExCcid_Usb_If_Init();

    /* Continue only if the USB is initialized successfully */
    if (bStatus == PH_ERR_SUCCESS)
    {
        /* Initialize the CCID Class */
        phExCcid_Usbd_CCID_Init ();

        /* Connect the USB Cable */
        phExCcid_Usbd_Connect_Enable();

        //phhalPcr_ConfigPuPd (6,0,1);
        //phhalPcr_ConfigInput(6,1,1,0,0,1,0);


    }
    return bStatus;
}

phStatus_t phExCcid_Usb_If_Init()
{
    phStatus_t bStatus = PH_ERR_INTERNAL_ERROR;

#ifdef PHEXCCID_USB_SELF_POWERED_MODE
    bStatus = phhalHif_Usb_Init(PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP,
                                PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
                                PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT,
								PHEXCCID_USB_SELF_POWERED_MODE_ENABLE);
#else
    bStatus = phhalHif_Usb_Init(PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP,
                                PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
                                PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT,
								PHEXCCID_USB_BUS_POWERED_MODE_ENABLE);
#endif

    if (bStatus == PH_ERR_SUCCESS)
    {
        /* Call the USB HW-Core Initialization */
        phExCcid_Usbd_CORE_Init ();
    }

    return bStatus;
}

#if (PH_EXCCID_USB_IF_USB_SUSPEND_RESUME_FTR == 1)
phStatus_t phExCcid_Usb_If_Suspend()
{
    phhalPcr_StandbyPrevReason_t sStandbyPrevReason = {0};
    phhalPcr_WakeUpConfig_t      sWakeUpConfig;
    phStatus_t eStatus = PH_ERR_SUCCESS;

    sWakeUpConfig.wWakeUpTimerVal  = 2000;
    sWakeUpConfig.bEnableHIFWakeup = 1;
    sWakeUpConfig.bSmBusAddrResp   = 0x0;
    sWakeUpConfig.bI2CAddr         = 0x28;
    sWakeUpConfig.bWakeUpTimer     = 0;
    sWakeUpConfig.bWakeUpRfLdt     = 0;
    sWakeUpConfig.bWakeUpTemp      = 0;
    sWakeUpConfig.bWakeUpPvddLim   = 0;
#if (PH_EXCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
    sWakeUpConfig.bWakeUpCtPr      = 1;
#else
    sWakeUpConfig.bWakeUpCtPr      = 0;
#endif
    sWakeUpConfig.bWakeUpIntAux    = 0;
    sWakeUpConfig.bWakeUpTvddMon   = 0;
    sWakeUpConfig.bWakeUpGpio      = 0;
    sWakeUpConfig.blWakeupVbusLow  = 0;

    if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
    {
        /* Stop the LED Timer */
        phhalTimer_Stop(pLedTimer);

        /* Switch OFF all LED's */
        phExCcid_All_LED_Off();

        eStatus = phhalPcr_EnterLowPowerMode((phhalPcr_RfLdtParams_t *)gpkphCfg_EE_HW_RfLdtParams,
                                   &sWakeUpConfig, &sStandbyPrevReason);

        if (eStatus == PH_ERR_SUCCESS)
        {
            return eStatus;
        }
        else
        {
            phExCcid_All_LED_Off();
            phExCcid_All_LED_On();
            return eStatus;
        }
    }
    else
    {
        /* Dont do anything */
        return eStatus;
    }
}

void phExCcid_Usb_If_Resume()
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    if (gphExCcid_sUsb_Bus_Status.bSuspendEnable == 1)
    {
        gphExCcid_sUsb_Bus_Status.bSuspendEnable = 0;

        eStatus = phhalHif_Usb_ConfigStart(PHHAL_HIF_USB_PLL_CLKEDGESNUM_TMP,
                                           PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT,
                                           PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT);

        if (eStatus == PH_ERR_SUCCESS)
        {
            phExCcid_All_LED_Off();
            phExCcid_LED_Status(GREEN_LED, LED_ON);
#if (PH_EXCCID_USB_IF_USB_REMOTE_WAKEUP_FTR == 1)
            if ((gphExCcid_sUsb_Bus_Status.bRemoteWakeupEnable == 1))
            {
                phExCcid_Usbd_Remote_Wakeup();
                phExCcid_LED_Status(YELLOW_LED, LED_ON);
            }
#endif
        }
        else
        {
            phExCcid_All_LED_Off();
            phExCcid_LED_Status(BLUE_LED, LED_ON);
        }
    }
    else
    {
        return;
    }
}
#endif

