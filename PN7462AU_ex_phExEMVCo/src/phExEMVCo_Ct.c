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
 * phExEMVCo_Ct.c:  Implements the CT task and the logical operations related to contact interface.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-11-18 18:56:23 +0530 (Wed, 18 Nov 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 16557 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phExEMVCo_Ct.h"
#include "phFlashBoot_Event.h"
#include "phhalNvic.h"
#include "phUser.h"
#include "phExEMVCo_Cfg.h"
#include "ph_Log.h"
#include "phCfg_EE.h"
#include "phLED.h"
#include "phExEMVCo.h"
#include "phhalTimer.h"
#include "phExEMVCo_Clif.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXEMVCO_CT_MAX_ATR_SIZE          32
#define PH_EXEMVCO_MAX_NO_APDU 9
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
phRtos_QueueHandle_t gphExEMVCo_Ct_Queue = NULL;

static const uint8_t bApduCommandArray[PH_EXEMVCO_MAX_NO_APDU][32] =
{
    /* SELECT MasterCard credit or debit command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT Visa credit or debit card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT Maestro (debit card) command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT Cirrus (interbank network) ATM card only command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x04,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT Maestro UK card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x05,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT Visa Electron card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT V PAY command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT VISA Plus card command */
    {0x00,0xA4,0x04,0x00,0x07,0xA0,0x00,0x00,0x00,0x03,0x80,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

    /* SELECT Amex card */
    {0x00,0xA4,0x04,0x00,0x06,0xA0,0x00,0x00,0x00,0x25,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

#if PHFL_LOG_ENABLE
static const char bApduInfo[PH_EXEMVCO_MAX_NO_APDU][50]=
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

#endif
static uint8_t bApduRespBuffer[256];

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#ifdef PHFL_HALAPI_WITH_RTOS
static void phExMain_CtCallBack(uint32_t dwIRQMessage);
#endif
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Performs callback function for the CT IRQ's.
 *
 * @param dwIRQMessage : Presence IRQ from the CT
 */
#ifdef PHFL_HALAPI_WITH_RTOS
static void phExMain_CtCallBack(uint32_t dwIRQMessage)
{
    phExEMVCo_EventCtMsg_t sCtMsg;
    phStatus_t status = 0x00;

    if(dwIRQMessage & CT_USR2_REG_PRESL_MASK)
    {
        status = phhalCt_CheckCardPres();
    }

    if(status == (PH_ERR_CT_MAIN_CARD_PRESENT | PH_COMP_HAL_CT) )
    {
        sCtMsg.dwMsg = PH_ERR_CT_MAIN_CARD_PRESENT;
    }
    else
    {
        if(status == (PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT))
        {
            sCtMsg.dwMsg = PH_ERR_CT_MAIN_CARD_ABSENT;
        }
    }
    /** wait indefinitely until post to ct task is success. */
    sCtMsg.eSrcId = E_PH_CT;

    PH_USER_ASSERT(phRtos_QueueSend( gphExEMVCo_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );
}

/**
 * Performs message posting to the CT task.
 *
 * @param eCtCmd : CT commands \ref phExEMVCo_EventCtCmds_t
 * @param pDes : Destination message queue handler.
 */
void phExEMVCo_CtPostMsg(phExEMVCo_EventCtCmds_t eCtCmd, void *pDes)
{
    phExEMVCo_EventCtMsg_t sCtMsg;
    /** wait indefinitely until post to ct task is success. */
    sCtMsg.eSrcId = E_PH_SYS;
    sCtMsg.pvDes = pDes;
    sCtMsg.eCommands = eCtCmd;
    PH_USER_ASSERT(phRtos_QueueSend( gphExEMVCo_Ct_Queue, &sCtMsg, portMAX_DELAY) == phRtos_Success );

}

/**
 * Initializes CT Queue & task.
 *
 * @param pSysMsgQueue : Destination system message queue handler.
 * @return Status of the operation.
 */
phStatus_t phExEMVCo_CtTaskInit(void *pSysMsgQueue)
{
    phRtos_TaskHandle_t pHandle = NULL;

    /** create the Clif Task and Clif queue and post the msg. */
    gphExEMVCo_Ct_Queue  = phRtos_QueueCreate(PHFL_EVENT_CT_Q_LEN, sizeof(phExEMVCo_EventCtMsg_t));
    PH_USER_ASSERT(gphExEMVCo_Ct_Queue);

    /** Create the ct task. */
    pHandle = NULL;
    phRtos_TaskCreate( &phExEMVCo_CtTask, "CtTask", PH_EXEMVCO_CFG_RTOS_CT_TASK_STACK_SIZE, gphExEMVCo_Ct_Queue,
        PH_EXEMVCO_CFG_RTOS_CT_TASK_PRIORITY, &pHandle );
    PH_USER_ASSERT(pHandle);

    return PH_ERR_SUCCESS;
}

/**
 * CT Task which performs all the Contact interface operations.
 *
 * @param pvParams : Handler to the System message queue.
 */
void phExEMVCo_CtTask(void *pvParams)
{
    phStatus_t eStatus;
    phExEMVCo_EventCtMsg_t sCtQueue;
    uint32_t dwIRQMessage = CT_USR2_REG_PRESL_MASK;

#ifdef PHFL_ENABLE_STANDBY
    phFlashBoot_Event_SysMsg_t sSysTaskMsg;
#endif
    phhalCt_InitParam_t  sInitParams;
    sInitParams.bAutoCTDeactivationEnable= 0;
    sInitParams.bCardPresConnectorStatus= gpkphCfg_EE_Boot_CT->bConnectorType;
    sInitParams.bPullupSetStatus = gpkphCfg_EE_Boot_CT->bPullUp;
    sInitParams.bSlewRateConfig = gpkphCfg_EE_Boot_CT->bSlewRate;

    /** Initialize the CT PAL layer */
     /* With the performance board configuration as pull down enabled and normally open connector type */
    /* The init parameters should come from the EEPROM */
    PH_USER_ASSERT(phpalCt_Init(& sInitParams) == PH_ERR_SUCCESS);

    /* Register the call back */
    phhalCt_RegCallBack(&phExMain_CtCallBack,dwIRQMessage);

    while(1)
    {
        /** Check for the message from the Ct hal . */
        phRtos_QueueReceive( gphExEMVCo_Ct_Queue, (void *)&sCtQueue, portMAX_DELAY );

        if(sCtQueue.eSrcId == E_PH_SYS)
        {
            LOG_TXT("System message to shut down\n");
            break;
        }
        switch((phhalCt_ErrorCodes_t)sCtQueue.dwMsg)
        {
            case PH_ERR_CT_MAIN_CARD_PRESENT:
                LOG_TXT("CT card detected.\n");
                phLED_SetPattern(gkphLED_Ct_Detect);
                /* LOG_TXT("Starting the contact card example application"); */
                eStatus = phExEMVCo_Ct_Example_App();
                if(PH_ERR_SUCCESS != eStatus)
                {
                    phLED_SetPattern(gkphLED_Ct_Fail);
                }
                else
                {
                    phLED_SetPattern(gkphLED_Ct_Pass);

                }

                gphExEMVCo_Clif_PollLoopLOGCount = 0;
            break;
            case PH_ERR_CT_MAIN_CARD_ABSENT:
                LOG_TXT("CT card removed.\n");
                phLED_SetPattern(gkphLED_BootUp);
#ifdef PHFL_ENABLE_STANDBY
                /* Wait indefinitely until post to System task is success. */
                sSysTaskMsg.eSrcId = E_PH_CT;
                sSysTaskMsg.pvDes = gphExEMVCo_Ct_Queue;
                sSysTaskMsg.dwMsg[0] = PH_ERR_CT_MAIN_CARD_ABSENT;
                (void)phRtos_QueueSend( sCtQueue.pvDes, (void *)&sSysTaskMsg, portMAX_DELAY);
#endif
                gphExEMVCo_Clif_PollLoopLOGCount = 0;
                phLED_SetPattern(gkphLED_BootUp);
            break;
            default:
                gphExEMVCo_Clif_PollLoopLOGCount = 0;
                /** It is not expected to reach here. */
                PH_USER_ASSERT(0);
        }

    }

}
#endif

/**
 * This is an example application running on PN7462AU on JCOP card with PPSE Applet installed
 * @return
 */
phStatus_t phExEMVCo_Ct_Example_App(void)
{
    phStatus_t phStatus = PH_ERR_FAILED;
    /* Global instance for  CT pal parameter's structure */
    phpalCt_DataParams_t pDataParams ;
    uint8_t bApduCount = 0x00;
    uint16_t wNoOfResponseBytes = 0x00;

    do
    {
        /* Use the receive buffer itself as ATR buffer */
        pDataParams.pbAtrBuffer = (uint8_t *)&bApduRespBuffer;
        pDataParams.bSizeOfATRbuffer = PH_EXEMVCO_CT_MAX_ATR_SIZE;
        phStatus = phpalCt_ActivateCard(&pDataParams);
        if((PH_ERR_SUCCESS != phStatus))
        {
            if((PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT) == phStatus)
            {
                LOG_TXT("Early Atr is received..\n");
            }
            else if((PH_ERR_CT_ATR_PARSER_ERROR|PH_COMP_HAL_CT)== phStatus)
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_AU8("ATR:\n",pDataParams.pbAtrBuffer, pDataParams.bAtrReceivedLength);
                LOG_TXT("Non EMVCo Card detected\n");
                break;
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_TXT("Card Activation failed.\n");
                LOG_X32("Error Status",phStatus);
                break;
            }
        }

        phLED_SetPattern(gkphLED_Ct_Ongoing);
        LOG_AU8("ATR:\n",pDataParams.pbAtrBuffer, pDataParams.bAtrReceivedLength);
        LOG_TXT("\nEMVCo payment card detected. Checking embedded VISA or Master Card applications..\n");

        for(bApduCount = 0x00; bApduCount < PH_EXEMVCO_MAX_NO_APDU; bApduCount++)
        {
            /* This logic only works for the Hard Coded CASE 4 APDUs */
            uint32_t bCmdAPDULength = 4 /* Header */
                    + 1 /* Self */
                    + bApduCommandArray[bApduCount][4]
                                                    + 1 /* LE */ ;
            /* Send the Apdu */
            phStatus = phpalCt_Transceive(
                    (uint8_t *)(bApduCommandArray[bApduCount]),
                    bCmdAPDULength,
                    bApduRespBuffer,&wNoOfResponseBytes);
            if(phStatus != PH_ERR_SUCCESS)
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_TXT("Error transceiving Apdu\n");
                break;
            }
            if((bApduRespBuffer[wNoOfResponseBytes-2] == 0x90) && (bApduRespBuffer[wNoOfResponseBytes-1] == 0x00))
            {
                phLED_SetPattern(gkphLED_Ct_Pass);
                LOG_TXT(bApduInfo[bApduCount]);
                phStatus = PH_ERR_SUCCESS;
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
    return phStatus;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
