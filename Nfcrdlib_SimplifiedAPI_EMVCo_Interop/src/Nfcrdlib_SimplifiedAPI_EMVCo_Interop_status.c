/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Example Source file for EMVCo Interoperability test status.
* This file performs initialization of GPOI's and LED indication for EMVCo Interoperability loopback test status.
* Please refer Readme.txt file for Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Jaganmohan Reddy (nxp95127) $
* $Revision: 1 $ (v05.21.00)
* $Date: 2017-11-08 11:30:00 +0530 (Wed, 08 Nov 2017) $
*
*/
#include "phApp_Init.h"
#ifndef NXPBUILD__PHHAL_HW_PN7462AU
#include "BoardSelection.h"
#else
#include "phhalGpio.h"
#include "ph_Reg.h"
#endif

#if defined(PHDRIVER_PIPN5180_BOARD) || defined(PHDRIVER_PIRC663_BOARD)
#include <time.h>
#include <stdlib.h>
#endif

#include "Nfcrdlib_SimplifiedAPI_EMVCo_Interop.h"
#include "Nfcrdlib_SimplifiedAPI_EMVCo_Interop_status.h"

/***********************************************************************************************
 * \brief   This function performs initialization of GPIO's to indicate Interoperability test status.
 * \param   none
 * \return  status  Returns the function status
 **********************************************************************************************/
phStatus_t Emvco_Interop_Init(void)
{
#if !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD)

#ifndef NXPBUILD__PHHAL_HW_PN7462AU
    phStatus_t statusTmp;
    phDriver_Pin_Config_t pinCfg;

    pinCfg.bPullSelect = PH_DRIVER_PULL_DOWN;
    pinCfg.bOutputLogic = EMVCO_INTEROP_LED_OFF;
    PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_PinConfig(PHDRIVER_PIN_RLED, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));

    pinCfg.bPullSelect = PH_DRIVER_PULL_DOWN;
    pinCfg.bOutputLogic = EMVCO_INTEROP_LED_OFF;
    PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_PinConfig(PHDRIVER_PIN_GLED, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));
#else
    phhalPcr_ConfigOutput(GREEN_LED_PN7462AU, true, false);
    phhalPcr_ConfigOutput(RED_LED_PN7462AU, true, false);
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#else
    system("echo none | sudo tee /sys/class/leds/led0/trigger >/dev/null");
    system("echo 0 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
#endif /* !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD) */

    return PH_ERR_SUCCESS;
}

/***********************************************************************************************
 * \brief   This function performs LED indication for Interoperability test status.
 *          CLEV6630B 2.0, PNEV5180B v2.0, PN7462AU v2.1 and FRDM-K82F Board:
 *              Success: Green LED On, 500 milli sec delay and LED Off.
 *              Fail   : RED LED On, 500 milli sec delay and LED Off.
 *          Raspberry Pi:
 *              Success: Green LED On, 2 sec delay and LED Off.
 *              Fail   : Green LED Toggles every 250 milli sec for 2 sec and LED Off.
 * \param   Error_Status EMVCO_INTEROP_ERR_SUCCESS or EMVCO_INTEROP_ERR_FAIL
 * \return  none
 *
 * Note: - CLEV6630B 2.0 or PNEV5180B v2.0 Customer Evaluation Board LED indications:
 *         Red LED - LD201 and Green LED - LD203
 *       - PN7462AU v2.1 Customer Evaluation Board LED indications:
 *         Red LED - LED8 and Green LED - LED9
 *       - FRDM-K82F Board LED indications:
 *         Red LED - D3 RGB and Green LED - D3 RGB
 *       - Raspberry Pi:
 *         ACT LED
 **********************************************************************************************/
void Emvco_Interop_Status(uint8_t Error_Status)
{
#if !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD)

    if(Error_Status != EMVCO_INTEROP_ERR_FAIL)
    {
        /* Success - Green LED On, 500 milli sec delay and LED Off */
        EMVCO_INTEROP_STAT_GLED_ON();
        CHECK_NFCLIB_STATUS(phhalHw_Wait(pHal, PHHAL_HW_TIME_MILLISECONDS, 500U));
        EMVCO_INTEROP_STAT_GLED_OFF();
    }
    else
    {
        /* Fail - Red LED On, 500 milli sec delay and LED Off  */
        EMVCO_INTEROP_STAT_RLED_ON();
        CHECK_NFCLIB_STATUS(phhalHw_Wait(pHal, PHHAL_HW_TIME_MILLISECONDS, 500U));
        EMVCO_INTEROP_STAT_RLED_OFF();
    }

#else
    struct timespec sTimeSpecReq;
    struct timespec sTimeSpecRem;
    uint8_t bCnt;

    if(Error_Status != EMVCO_INTEROP_ERR_FAIL)
    {
        /* SUCCESS: ON for 2 sec. */
        system("echo 1 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
        sTimeSpecReq.tv_sec = PHDRIVER_LED_SUCCESS_DELAY;
        sTimeSpecReq.tv_nsec = 0;
        nanosleep(&sTimeSpecReq, &sTimeSpecRem);

        system("echo 0 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
    }
    else
    {
        /* FAILURE: Toggle every 250ms for 2 sec. */
        sTimeSpecReq.tv_sec = 0;
        sTimeSpecReq.tv_nsec = PHDRIVER_LED_FAILURE_DELAY_MS * 1000 * 1000;
        for(bCnt = 0; bCnt < PHDRIVER_LED_FAILURE_FLICKER; bCnt++)
        {
            system("echo 1 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
            nanosleep(&sTimeSpecReq, &sTimeSpecRem);
            system("echo 0 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
            nanosleep(&sTimeSpecReq, &sTimeSpecRem);
        }
    }
#endif /* !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD) */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
