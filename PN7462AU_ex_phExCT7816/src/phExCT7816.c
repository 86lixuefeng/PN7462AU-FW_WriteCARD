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
 * phExCT7816.c:
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
#include "phExCT7816.h"
#include "phLED.h"
#include "ph_Log.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXCT7816_MAX_ATR_SIZE          33
#define PH_EXCT7816_MAX_NO_APDU          6
#define PH_EXCT7816_APDU_LENGTH          13
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

static const uint8_t gkphCT7816_ApduCommandArray[PH_EXCT7816_MAX_NO_APDU][22] =
{
/* CreateMF */
{0x00,0xe0,0x00,0x00,0x09,0x62,0x07,0x82,0x01,0x38,0x83,0x02,0x3f,0x00},
/* CreateEF */
{0x00,0xe0,0x00,0x00,0x11,0x62,0x0f,0x80,0x02,0x00,0xff,0x82,0x02,0x01,0x61,0x83,0x02,0x01,0x01,0x88,0x01,0x08},
/* SelectEF */
{0x00,0xa4,0x00,0x04,0x02,0x01,0x01,0x00},
/* Write binary */
{0x00,0xD0,0x00,0x00,0x0f,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff},
/* Read binary */
{0x00,0xb0,0x00,0x00,0x0F},
/* DeleteEF */
{0x00,0xE4,0x00,0x04,0x02,0x01,0x01,0x00}
};

/* The length of the each Apdu in the above Apdu table */
static const uint8_t gkphCT7816_ApduLength[PH_EXCT7816_MAX_NO_APDU] =
{
    14,22,8,20,5,8
};

static const char gkphCT7816_ApduInfo[PH_EXCT7816_MAX_NO_APDU][15]=
{
    ("Create MF "),
    ("Create EF "),
    ("Select EF "),
    ("Write binary "),
    ("Read binary "),
    ("Delete EF "),
};

PH_NOINIT static uint8_t gbphCT7816_ApduRespBuffer[64];

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
/** phExCT7816 entry point. */
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
    (void)phpalCt_SetConfig( E_EMV_ENABLE, FALSE);
    /** Start the application */
    while(1)
    {
        phExCT7816_Scosta_Card_App();
    }
    return 0;
}

/**
 * Check card presence and start example.
 *
 * @return Status of the operation.
 */
phStatus_t phExCT7816_Scosta_Card_App()
{
    phStatus_t eStatus = PH_ERR_FAILED;
    /** Instance for  CT pal parameter's structure */
    phpalCt_DataParams_t pDataParams ;
    uint8_t bAtrFlag = FALSE;
    uint16_t wReceiveSize = 0x00;
    uint8_t bApduCount = 0x00;

    do
    {
        LOG_TXT("Waiting for CT card insertion.\n");
        /** Check the card presence */
        while((PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
        {
        /** Wait here until there is a presence of card seen */
        }

        LOG_TXT("CT card detected\n");
        /** Card Detected Blue Led should glow.*/
        phLED_SetPattern(gkphLED_Ct_Detect);

        /* The response buffer is used to optimize the code */
        pDataParams.pbAtrBuffer = (uint8_t *)(&gbphCT7816_ApduRespBuffer);
        pDataParams.bSizeOfATRbuffer = PH_EXCT7816_MAX_ATR_SIZE;

        /* Activate the card */
        eStatus = phpalCt_ActivateCard(&pDataParams);
        if(PH_ERR_SUCCESS != eStatus)
        {
            if((PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT) == eStatus)
            {
                LOG_TXT("Early Atr is received..\n");
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_TXT("Card Activation failed.\n");
                LOG_X32("Error Status", eStatus);
                bAtrFlag = TRUE;
                break;
            }
        }

        phLED_SetPattern(gkphLED_Ct_Ongoing);
        LOG_TXT("Card Activation Successful.\n");
        LOG_AU8("ATR:\n",pDataParams.pbAtrBuffer, pDataParams.bAtrReceivedLength);

        for(bApduCount=0x00; bApduCount<PH_EXCT7816_MAX_NO_APDU; bApduCount++)
        {
            /* Start the transactions */
            eStatus = phpalCt_Transceive( (uint8_t *)gkphCT7816_ApduCommandArray[bApduCount],
                                                     gkphCT7816_ApduLength[bApduCount],
                                                     gbphCT7816_ApduRespBuffer,&wReceiveSize);
            if(PH_ERR_SUCCESS != eStatus)
            {
                break;
            }
            if((gbphCT7816_ApduRespBuffer[wReceiveSize-2] == 0x90) && (gbphCT7816_ApduRespBuffer[wReceiveSize-1] == 0x00))
            {
                LOG_TXT(gkphCT7816_ApduInfo[bApduCount]);
                LOG_TXT("successful\n");
            }
            else if((gbphCT7816_ApduRespBuffer[wReceiveSize-2] == 0x6A) && (gbphCT7816_ApduRespBuffer[wReceiveSize-1] == 0x89))
            {
                if(bApduCount == 0x00)
                {
                    LOG_TXT("MF File Already Exist.\n");
                }
                else
                {
                    if(bApduCount == 0x01)
                    {
                        LOG_TXT("EF File Already Exist.\n");
                    }
                }
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_TXT(gkphCT7816_ApduInfo[bApduCount]);
                LOG_TXT("not successful\n");
                break;
            }
        } /* End of for loop */

        /* When all operations have been done successfully.*/
        phLED_SetPattern(gkphLED_Ct_Pass);

    }while(0);

    if(PH_ERR_SUCCESS != eStatus )
    {
        if(!bAtrFlag)
        {
            LOG_TXT("Error in transceiving Apdu.\n");
            LOG_X32("Error Status   :", eStatus);
            phLED_SetPattern(gkphLED_Ct_Fail);
        }
    }
    phhalCt_CardDeactivate();
    LOG_TXT("Remove CT card.\n");

    /** Check the card absence */
    while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
    {
      /** Wait here until there is removal of card */
    }

    LOG_TXT("CT card removed.\n");
    phLED_SetPattern(gkphLED_BootUp);
    return eStatus;

}

