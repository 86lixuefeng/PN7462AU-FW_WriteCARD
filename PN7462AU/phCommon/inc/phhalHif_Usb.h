/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014,2015
 *
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */
/** @file
 *
 * Header for Host Interface Core
 *
 * $Author: Purnank H G (ing05193) $
 * $Revision: 15442 $ (v04.13.00)
 * $Date: 2015-09-23 17:59:43 +0530 (Wed, 23 Sep 2015) $
 *
 */

#ifndef PHHALHIF_USB_H
#define PHHALHIF_USB_H

/**
 * \addtogroup phhalHif_USB
 *
 * The USB (HAL) APIs
 *
 * @{
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_USB
#include "ph_Status.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/** Default USB PLL Detection window length. \see phExCcid_Usb_If_Init */
#define PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT (0x0D)
/** Default Clock Edges to wait. \see phExCcid_Usb_If_Init */
#define PHHAL_HIF_USB_PLL_CLKEDGESNUM_DEFAULT (0x80)
/** Default timeout to wait for USB PLL to be initialized.  \see phExCcid_Usb_If_Init */
#define PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT (1000)

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** \addtogroup grpErr_HalUSB
 *
 * Error codes for USB HAL
 *
 * @{
 */
/**
 * USB HAL Interface Error Code ENUMs
 */
typedef enum {
    PH_ERR_USB_PVDD_3V_NOK = (PH_ERR_CUSTOM_BEGIN+1),  /**< PVDD 3V is not Ok */
}phhalHif_Usb_ErrorCodes_t;
/** @} */

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */

/** This API initializes USB HAL  Context, enables NVIC Interrupt, and enables the HIF HW and starts USB BitClock
 *
 * @warning The test application / user application has to ensure that PVDD has started before invoking this API.
 *
 * @param[in] bNumberClkEdges Number of Clock edges expected in the PLL Clock detection Window.
 *             \see PHHAL_HIF_USB_PLL_CLKEDGESNUM_DEFAULT
 * @param[in] bClkDetectionWindowLen PLL Clock detection Window length.
 *            \see PHHAL_HIF_USB_PLL_DETECTIONWINDOWLEN_DEFAULT
 * @param[in] wTimeoutMicrosec Time out duration in Microseconds.
 *            \see PHHAL_HIF_USB_PLL_TIMEOUTMICROSEC_DEFAULT
 * @param[in] bUsbVbusPulldown If true enables the internal pull down resistance to pull down the USB_VBUS.
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS when the initialization is successfully done.
 * @maskedret #PH_ERR_USB_CLK_IN_ERROR Supply clock doesn't exist/less than 27.12MHz
 * @maskedret #PH_ERR_CLKIN_DETECTION_ERROR PLL Clock Input Detection procedure Timed out
 * @maskedret #PH_ERR_USB_PLL_LOCK_ERROR PLL Lock Timed out
 */
phStatus_t phhalHif_Usb_Init(uint8_t bNumberClkEdges, uint8_t bClkDetectionWindowLen, uint16_t wTimeoutMicrosec,
    uint8_t bUsbVbusPulldown);

/** This API de-initializes USB HAL  Context, disables NVIC Interrupt, and disables the HIF HW and disables USB BitClock
 * @retval    #PH_ERR_SUCCESS always
 */
phStatus_t phhalHif_Usb_DeInit();

/** This API configures the Power and Clock for USB module.
 *
 * @param[in] bNumberClkEdges Number of Clock edges expected in the PLL Clock detection Window.
 * @param[in] bClkDetectionWindowLen PLL Clock detection Window length.
 * @param[in] wTimeoutMicrosec Time out duration in Microseconds.
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS when the configuration is successfully done.
 * @maskedret #PH_ERR_USB_CLK_IN_ERROR Supply clock doesn't exist/less than 27.12MHz
 * @maskedret #PH_ERR_CLKIN_DETECTION_ERROR PLL Clock Input Detection procedure Timed out
 * @maskedret #PH_ERR_USB_PLL_LOCK_ERROR PLL Lock Timed out
 * @maskedret #PH_ERR_USB_PVDD_3V_NOK PVDD 3V not Available
 */
phStatus_t phhalHif_Usb_ConfigStart(uint8_t bNumberClkEdges,
                                   uint8_t bClkDetectionWindowLen,
                                   uint16_t wTimeoutMicrosec);

#endif /* NXPBUILD__PHHAL_USB */

/** @} */

#endif /* PHHALHIF_USB_H */
