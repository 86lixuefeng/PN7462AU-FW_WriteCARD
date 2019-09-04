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
 * phExCTEMVCO.c:
 *
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-06-20 15:49:09 +0530 (Mon, 20 Jun 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 18464 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include <stdio.h>
#include "ph_Datatypes.h"
#include "phFlashBoot_Event.h"
#include "phCfg_EE.h"
#include "phUser.h"
#include "phFlashBoot.h"
#include "phpalCt.h"
#include "phhalCt.h"
#include "phExCTEMVCo.h"
#include "phLED.h"
#include "ph_Log.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define PH_EXCTEMVCO_MAX_ATR_SIZE           32
#define PH_EXCTEMVCO_MAX_NO_APDU             9
#define PH_EXCTEMVCO_APDU_LENGTH             13

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static const uint8_t bApduCommandArray[PH_EXCTEMVCO_MAX_NO_APDU][32] =
{
    /** SELECT MasterCard credit or debit command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT Visa credit or debit card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT Maestro (debit card) command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT Cirrus (interbank network) ATM card only command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT Maestro UK card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x05,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT Visa Electron card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT V PAY command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT VISA Plus card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x80,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /** SELECT Amex card */
    {0x00,0xA4,0x04,0x00,0x06,0xA0,0x00,0x00,0x00,0x25,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

static const char gkphCTEMVCo_ApduInfo[PH_EXCTEMVCO_MAX_NO_APDU][50]=
{
   {"Master Card : Credit or Debit\n"},
   {"Visa Card : Credit or Debit\n"},
   {"Master Card : Maestro(debit card)\n"},
   {"Master Card : Cirrus(interbank network)\n"},
   {"Master Card : Maestro UK\n"},
   {"Visa Card : Electron card\n"},
   {"Visa Card : V PAY card\n"},
   {"Visa Card : VISA Plus card\n"},
   {"Amex Card\n"}
};

PH_NOINIT static uint8_t gbphCTEMVCo_ApduRespBuffer[256];

PH_NOINIT static phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
/** phExCTEMVCo entry point. */
int main(void)
{
    phhalCt_InitParam_t  sInitParams;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;
    sInitParams.bAutoCTDeactivationEnable= 0;

    phFlashBoot_Main();

    /** Initialize delay loop variable based on CPU speed */
    phUser_WaitInit((phUser_CpuSpeed_t)E_COMMON_CPUSPEED_20MHZ);

    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);

    /** Initialize the LED module */
    phLED_Init();

    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

    phpalCt_Init(& sInitParams);

    /** Start the application */
    while(1)
    {
        phExCTEMVCo_EMV_Card_App();
    }
    return 0;
}

/**
 * Check card presence and start example.
 *
 * @return Status of the operation.
 */
phStatus_t phExCTEMVCo_EMV_Card_App()
{
    phStatus_t phStatus = PH_ERR_FAILED;
    /** Global instance for  CT pal parameter's structure */
    phpalCt_DataParams_t pDataParams ;
    uint8_t bApduCount = 0x00;
    uint16_t wNoOfResponseBytes = 0x00;

    do
    {
        LOG_TXT("Waiting for CT card insertion.\n");
        /** Check the card presence */
        while((PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
        {
            /** Wait here until there is a presence of card seen */
        }

        /** Card Detected Blue Led should glow.*/
        phLED_SetPattern(gkphLED_Ct_Detect);

        LOG_TXT("CT card detected.\n");

        /* The response buffer is used to optimize the code */
        pDataParams.pbAtrBuffer = (uint8_t *)&gbphCTEMVCo_ApduRespBuffer;
        pDataParams.bSizeOfATRbuffer = PH_EXCTEMVCO_MAX_ATR_SIZE;

        phStatus = phpalCt_ActivateCard(&pDataParams );
        if((PH_ERR_SUCCESS != phStatus))
        {

            if((PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT) == phStatus)
            {
                LOG_TXT("Early Atr is received..\n");
            }
            else if((PH_ERR_CT_ATR_PARSER_ERROR|PH_COMP_HAL_CT)== phStatus)
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                if(pDataParams.bAtrReceivedLength <= 32)
                {
                    LOG_AU8("ATR:\n",gbphCTEMVCo_ApduRespBuffer, pDataParams.bAtrReceivedLength);
                }
                LOG_TXT("Non EMVCo Card detected.\n");
                break;
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_TXT("Card Activation failed.\n");
                LOG_X32("Error Status", phStatus);
                break;
            }
        }

        phLED_SetPattern(gkphLED_Ct_Ongoing);
        LOG_AU8("ATR:\n",pDataParams.pbAtrBuffer, pDataParams.bAtrReceivedLength);
        LOG_TXT("\nEMVCo payment card detected. Checking embedded VISA or Master Card applications..\n");

        for(bApduCount = 0x00; bApduCount < PH_EXCTEMVCO_MAX_NO_APDU; bApduCount++)
        {

            /** This logic only works for the Hard Coded CASE 4 APDUs */
            uint32_t bCmdAPDULength = 4 /** Header */
                    + 1 /** Self */
                    + bApduCommandArray[bApduCount][4]
                                                    + 1 /** LE */ ;
            /** Send the Apdu */
            phStatus = phpalCt_Transceive(
                    (uint8_t *)bApduCommandArray[bApduCount],
                    bCmdAPDULength,
                    gbphCTEMVCo_ApduRespBuffer,&wNoOfResponseBytes);
            if(phStatus != PH_ERR_SUCCESS)
            {
                LOG_TXT("Error transceiving Apdu\n");
                LOG_X32("Error Status :  ", phStatus);
                phLED_SetPattern(gkphLED_Ct_Fail);
                break;
            }
            /** Check the status of the command execution */
            if((gbphCTEMVCo_ApduRespBuffer[wNoOfResponseBytes-2] == 0x90) && (gbphCTEMVCo_ApduRespBuffer[wNoOfResponseBytes-1] == 0x00))
            {
                LOG_TXT(gkphCTEMVCo_ApduInfo[bApduCount]);
                phStatus = PH_ERR_SUCCESS;
                phLED_SetPattern(gkphLED_Ct_Pass);
                break;
            }
            else
            {
                phStatus = PH_ERR_PROTOCOL_ERROR;
            }

        }
        if(PH_ERR_PROTOCOL_ERROR == phStatus)
        {

            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("Neither Master nor Visa application detected\n");
        }

    } while(0);

    phhalCt_CardDeactivate();

    LOG_TXT("Remove CT card.\n");

    /** Check the card absence */
    while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
    {
        /** Wait here until there is removal of card */
    }
    LOG_TXT("CT card removed.\n");
    phLED_SetPattern(gkphLED_BootUp);
    return phStatus;
}
