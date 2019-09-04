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
 * Implementation of Host Interface - I2C Slave
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 12:36:10 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16977 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_USB
#include "phhalPcr.h"
#include "phhalHif_Usb.h"
#include "phhalClkGen.h"
#include "phhalNvic.h"
#include "phUser.h"
#include "PN7462AU/PN7462AU_hif.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

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

phStatus_t phhalHif_Usb_Init(uint8_t bNumberClkEdges, uint8_t bClkDetectionWindowLen, uint16_t wTimeoutMicrosec,
    uint8_t bUsbVbusPulldown)
{
    phStatus_t status;

    status = phhalHif_Usb_ConfigStart(bNumberClkEdges, bClkDetectionWindowLen, wTimeoutMicrosec);

    if(status ==  PH_ERR_SUCCESS)
    {
        PHHAL_PCR_SELECT_HIF(PHHAL_PCR_HIF_USB);

        /* PULLDOWN shall be enabled in self power mode and disabled in Hif-USB bus power mode. */
        if(bUsbVbusPulldown)
        {
            PH_REG_SET_BIT(PCR_CTRL_REG, USB_VBUS_PULLDOWN);
        }
        else
        {
            PH_REG_CLEAR_BIT(PCR_CTRL_REG, USB_VBUS_PULLDOWN);
        }

        /* Enable the Host Interface Input */
        PH_REG_SET_BIT(HOSTIF_CONTROL_REG, HIF_ENABLE);

        phHal_Nvic_EnableInterrupt( 1<<PHHAL_NVIC_HIF );
    }

    return status;

}

phStatus_t phhalHif_Usb_DeInit()
{
    phhalClkGen_UsbClockStop();
    PHHAL_PCR_SELECT_HIF(0);
    PH_REG_CLEAR_BIT(HOSTIF_CONTROL_REG, HIF_ENABLE);
    phHal_Nvic_DisableInterrupt( 1<<PHHAL_NVIC_HIF );

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHif_Usb_ConfigStart(uint8_t bNumberClkEdges,
                                   uint8_t bClkDetectionWindowLen,
                                   uint16_t wTimeoutMicrosec)
{
     phStatus_t status = PH_ERR_SUCCESS;

     do
     {
         if(!PHHAL_PCR_CHECKPVDD_3V_OK())
         {
             status = PH_ERR(USB_PVDD_3V_NOK,USB);
             break;
         }

         /* Enable the HIF IP Clock */
         PH_REG_SET_BIT( PCR_CLK_CFG_REG,CLOCK_HOSTIF_ENABLE);

         if(status == PH_ERR_SUCCESS)
         {
             status = phhalClkGen_UsbClockStart(bNumberClkEdges, bClkDetectionWindowLen, wTimeoutMicrosec);
         }
     }while(0);

     return status;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHHAL_USB */
