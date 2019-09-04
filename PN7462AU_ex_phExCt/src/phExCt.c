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
 * $Date: 2016-09-19 12:01:19 +0530 (Mon, 19 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18660 $ (v05.19.00)
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
#include "phhalCt.h"
#include "phExCt.h"
#include "phLED.h"
#include "ph_Log.h"
#include "phhalTimer.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXCT_MAX_ATR_SIZE           32
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/** Command Apdu for T0 protocol.*/
static  uint8_t SelectMasterCard_T0Apdu[] = {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x10,0x10,0x00};
/** Command Apdu for T1 protocol with Prilog and Epilog.*/
static  uint8_t SelectMasterCard_T1Apdu[] =  {0x00, 0x00, 0x0D,0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x10,
                                              0x10,0x00,0x0E};
/** Application receive buffer */
static uint8_t bApduRespBuffer[128];

static phhalTimer_Timers_t *pLedTimer;


/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/** Main entry point for examples  */
int main(void)
{
	phFlashBoot_Main();

    phhalCt_InitParam_t  sInitParams;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;
    sInitParams.bAutoCTDeactivationEnable= 0;

    /** Initialize delay loop variable based on CPU speed */
    phUser_WaitInit((phUser_CpuSpeed_t)E_COMMON_CPUSPEED_20MHZ);
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);

    /** Initialize the LED module */
    phLED_Init();
    phLED_SetPattern(gkphLED_BootUp);
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);
    phhalCt_Init(& sInitParams);

    /** Start the application */
    while(1)
    {
        phExCt_HalApp();
    }
    return 0;
}

/** Main test application for CT HAL */
phStatus_t phExCt_HalApp( void)
{
    phStatus_t phStatus = PH_ERR_FAILED;
    uint8_t bCount=0,bActivateSequence[]={PHHAL_CT_VCC5,PHHAL_CT_VCC3,PHHAL_CT_VCC1M8};
    uint8_t bAtrFlag = FALSE;
    uint8_t bAtrReceivedLength = 0x00;
    uint16_t wLength = 0x00;
    uint8_t  bProtocol = 0x00;
    uint8_t bGetResposeBuf[5] = {0x00,0xC0,0x00,0x00,0x00};
    phhalCt_ProtocolParams_t sAtrPALParams;

    LOG_TXT("Waiting for CT card insertion.\n");

    /** Check the card presence */
    while((PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
    {
        /** Wait until there is a presence of card seen. */
    }

    LOG_TXT("CT card detected.\n");
    phLED_SetPattern(gkphLED_Ct_Detect);

    do
    {
        /** Card Detected,Activation process will start.*/
        phStatus = phhalCt_CardActivate( bActivateSequence[bCount],
                (uint8_t *)(bApduRespBuffer),&bAtrReceivedLength,&sAtrPALParams);

        /* Early error is considered as just a warning.*/
        if((PH_ERR_SUCCESS != phStatus))
        {
            if((PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT) == phStatus)
            {
                LOG_TXT("Early Atr is received..\n");
                phStatus = PH_ERR_SUCCESS;
                break;
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                if((PH_ERR_CT_ATR_PARSER_ERROR|PH_COMP_HAL_CT)== phStatus)
                {
                    LOG_AU8("ATR:\n",bApduRespBuffer, bAtrReceivedLength);
                    LOG_TXT("Non EMVCo Card detected.\n");
                    bAtrFlag = TRUE;
                    break;
                }
                /** Check for the MUTE errors */
                else if((PH_ERR_CT_MUTE_ERROR|PH_COMP_HAL_CT)== phStatus)
                {
                    phhalCt_CardDeactivate();
                    phUser_Wait(10000);
                    bCount++;
                    /* If Activation is failed for all three classes.*/
                    if(bCount == 3)
                    {
                        LOG_TXT("Card Activation failed.\n");
                        LOG_X32("Error Status", phStatus);
                        bAtrFlag = TRUE;
                    }
                }
                else
                {
                    LOG_TXT("Card Activation failed.\n");
                    LOG_X32("Error Status", phStatus);
                    bAtrFlag = TRUE;
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }while(bCount<3);

    do
    {

        if(PH_ERR_SUCCESS == phStatus)
        {
            phLED_SetPattern(gkphLED_Ct_Ongoing);
            LOG_AU8("ATR:\n",bApduRespBuffer, bAtrReceivedLength);
            LOG_TXT("\nEMVCo payment card detected. Checking Master Card application..\n");

            /** Card is activated successfully.*/

            if(!(sAtrPALParams.bProtSelT0))
            {
                /** If Card supports T1 protocol ,protocol T1 is selected using set transmission protocol api.*/
                bProtocol = 0x01;

                /** Calling phhalCt_SetTransmissionProtocol api is optional. Because in hal  card activate api ,
                 * this api is called itself after activation with first offered protocol.
                 *If card supports both protocol then you can explicitly choose your protocol by calling this api.
                 */
                phStatus =  phhalCt_SetTransmissionProtocol(bProtocol);

                /** Set timer in BWT mode for T1 protocol, by default WTX value should be 1.*/
                phStatus =  phhalCt_SetTimer(PHHAL_CT_APDUMODE_BWT, 1);

                phStatus =  phhalCt_Transmit(SelectMasterCard_T1Apdu,sizeof(SelectMasterCard_T1Apdu));
                PH_BREAK_ON_FAILURE(phStatus);

                /** Receive starting 3 byte for block.*/
                phStatus = phhalCt_Receive(bApduRespBuffer,3);
                PH_BREAK_ON_FAILURE(phStatus);

                /** Get Block length information from Len byte of Block.*/
                wLength = (uint16_t )bApduRespBuffer[0x02];
                /** Length with LRC byte.*/
                wLength++;

                /** Receive remaining bytes for block.*/
                phStatus = phhalCt_Receive(&bApduRespBuffer[3],wLength);
                PH_BREAK_ON_FAILURE(phStatus);

                /** Stopp the timer before new transaction.*/
                PHHAL_CT_STOP_TIMER;

                /** Check for last status byte.
                 * If last status byte 0x9000 then master card detected. */
                if((bApduRespBuffer[wLength] == 0x90)&&(bApduRespBuffer[wLength+1] == 0x00))
                {
                    phLED_SetPattern(gkphLED_Ct_Pass);
                    LOG_TXT(" Master Card Detected.\n");
                }
                else
                {
                    phLED_SetPattern(gkphLED_Ct_Fail);
                    LOG_TXT(" Master Card not detected.\n");
                }
            }
            else
            {
                /** If Card supports T1 protocol ,protocol T1 is selected using set transmission protocol api.*/
                /** If both the protocol is supported by card then by default will prefer protocol T0.*/
                bProtocol = 0x00;

                /** Calling phhalCt_SetTransmissionProtocol api is optional. Because in hal  card activate api ,
                 * this api is called itself after activation with first offered protocol.
                 *If card supports both protocol then you can explicitly choose your protocol by calling this api.
                 */
                phStatus =  phhalCt_SetTransmissionProtocol(bProtocol);

                /** To set timer in BWT mode for T1 protocol, by default WTX value should be 1.*/
                phStatus =  phhalCt_SetTimer(PHHAL_CT_APDUMODE_WWT, 1);

                phStatus =  phhalCt_Transmit(SelectMasterCard_T0Apdu,5);
                PH_BREAK_ON_FAILURE(phStatus);
                /** Receiving procedure byte for block.*/
                phStatus = phhalCt_Receive(bApduRespBuffer,1);
                PHHAL_CT_STOP_TIMER;
                PH_BREAK_ON_FAILURE(phStatus);

                phStatus =  phhalCt_SetTimer(PHHAL_CT_APDUMODE_WWT, 1);

                wLength = SelectMasterCard_T0Apdu[0x04];
                /** Sending remaining bytes from card.*/
                phStatus =  phhalCt_Transmit((&(SelectMasterCard_T0Apdu[5])),wLength);
                PH_BREAK_ON_FAILURE(phStatus);
                /** Wait for two status byte either '0x61XX' or some errorneous status bytes.*/
                phStatus = phhalCt_Receive(bApduRespBuffer,2);
                PHHAL_CT_STOP_TIMER;
                PH_BREAK_ON_FAILURE(phStatus);

                if(bApduRespBuffer[0] == 0x61)
                {
                    phStatus =  phhalCt_SetTimer(PHHAL_CT_APDUMODE_WWT, 1);

                    bGetResposeBuf[4]= bApduRespBuffer[1];
                    /** Send Get Response command to card*/
                    phStatus =  phhalCt_Transmit(bGetResposeBuf,0x05);
                    PH_BREAK_ON_FAILURE(phStatus);
                    wLength = bApduRespBuffer[1];
                    /** Receive procedure byte.*/
                    phStatus = phhalCt_Receive(bApduRespBuffer,1);
                    PH_BREAK_ON_FAILURE(phStatus);
                    /** Receive byte from card.*/
                    phStatus = phhalCt_Receive(bApduRespBuffer,wLength);
                    PH_BREAK_ON_FAILURE(phStatus);
                    /** Receive status byte from card.*/
                    phStatus = phhalCt_Receive(&(bApduRespBuffer[wLength]),2);
                    PHHAL_CT_STOP_TIMER;
                    PH_BREAK_ON_FAILURE(phStatus);

                    phLED_SetPattern(gkphLED_Ct_Pass);
                    LOG_TXT("Master Card detected.\n");
                }
                else
                {
                    phLED_SetPattern(gkphLED_Ct_Fail);
                    LOG_TXT("Master Card not detected.\n");
                }
            }
        }
    }while(0);
    if(PH_ERR_SUCCESS != phStatus)
    {
        if(!bAtrFlag)
        {
            LOG_TXT("Error in transceiving Apdu.\n");
            LOG_X32("Error Status   :", phStatus);
            phLED_SetPattern(gkphLED_Ct_Fail);
        }
    }
    phhalCt_CardDeactivate();
    /** Check the card absence */
    LOG_TXT("Remove CT card.\n");
    while((PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT) != phhalCt_CheckCardPres())
    {
        /* Wait here until there is removal of card */
    }
    LOG_TXT("CT card removed.\n");
    phLED_SetPattern(gkphLED_BootUp);
    return phStatus ;
}
