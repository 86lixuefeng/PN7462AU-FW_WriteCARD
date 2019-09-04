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
 * phExMain_Ct.c:  Implements the CT task and the logical operations related to contact interface.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-08-26 11:34:37 +0530 (Wed, 26 Aug 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 14749 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"


#include "phRtos.h"

#include "phExMain_Ct.h"
#include "phFlashBoot_Event.h"
#include "phhalNvic.h"
#include "phExMain_Cfg.h"
#include "ph_Log.h"
#include "phCfg_EE.h"
#include "phLED.h"
#include "phExMain.h"
#include "phhalTimer.h"
#include "phExMain_Clif.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PH_EXMAIN_CT_MAX_ATR_SIZE          33
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static const uint8_t Create_MF[] = {0x00,0xe0,0x00,0x00,0x09,0x62,0x07,0x82,0x01,0x38,0x83,0x02,0x3f,0x00};
static const uint8_t Create_EF[] = {0x00,0xe0,0x00,0x00,0x11,0x62,0x0f,0x80,0x02,0x00,0xff,0x82,0x02,0x01,0x61,
                                    0x83,0x02,0x01,0x01,0x88,0x01,0x08};
static const uint8_t Select_EF[] = {0x00,0xa4,0x00,0x04,0x02,0x01,0x01,0x00};
static const uint8_t Write_binary[] = {0x00,0xD0,0x00,0x00,0x0f,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,
                                       0xbb,0xcc,0xdd,0xee,0xff};
static const uint8_t Read_binary[] = {0x00,0xb0,0x00,0x00,0x0F};
static const uint8_t Delete_EF[] = {0x00,0xE4,0x00,0x04,0x02,0x01,0x01,0x00};

PH_NOINIT static uint8_t bApduRespBuffer[64];

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Contact based card operations entry point.
 *
 * @return Status of the operation.
 */
phStatus_t phExMain_Ct_Example_App(void)
{
    phStatus_t eStatus = PH_ERR_FAILED;
    /* Local instance for  CT pal parameter's structure */
    phpalCt_DataParams_t sPalCtDataParams ;
    uint16_t wReceiveSize=0;
    uint8_t bAtrFlag = FALSE;

    do
    {
        sPalCtDataParams.pbAtrBuffer = (uint8_t *)(&bApduRespBuffer);
        sPalCtDataParams.bSizeOfATRbuffer = PH_EXMAIN_CT_MAX_ATR_SIZE;

        phLED_SetPattern(gkphLED_Ct_Ongoing);

        /** Perform Card Activation. */
        eStatus = phpalCt_ActivateCard(&sPalCtDataParams);
        if(PH_ERR_SUCCESS != eStatus)
        {
            if((PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT)== eStatus)
            {
                LOG_TXT("Early Atr is received..\n");
            }
            else
            {
                phLED_SetPattern(gkphLED_Ct_Fail);
                LOG_TXT("Card Activation failed.\n");
                LOG_X32("Error Status",eStatus);
                bAtrFlag = TRUE;
                break;
            }
        }
        LOG_TXT("Card Activation Successful.\n");
        LOG_AU8("ATR:\n",sPalCtDataParams.pbAtrBuffer, sPalCtDataParams.bAtrReceivedLength);

        /** Create MF. */
        eStatus = phpalCt_Transceive( (uint8_t *)Create_MF, sizeof(Create_MF),bApduRespBuffer,&wReceiveSize);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }
        if((bApduRespBuffer[wReceiveSize-2] == 0x90) && (bApduRespBuffer[wReceiveSize-1] == 0x00))
        {
            LOG_TXT("Create MF done.\n");
        }
        else if((bApduRespBuffer[wReceiveSize-2] == 0x6A) && (bApduRespBuffer[wReceiveSize-1] == 0x89))
        {
            LOG_TXT("MF File Already Exist.\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("MF can not be created.\n");
            break;
        }

        /** Create binary EF. */
        eStatus = phpalCt_Transceive( (uint8_t *)Create_EF, sizeof(Create_EF),bApduRespBuffer,&wReceiveSize);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }
        if((bApduRespBuffer[wReceiveSize-2] == 0x90) && (bApduRespBuffer[wReceiveSize-1] == 0x00))
        {
            LOG_TXT("Create EF done.\n");
        }
        else if((bApduRespBuffer[wReceiveSize-2] == 0x6A) && (bApduRespBuffer[wReceiveSize-1] == 0x89))
        {
            LOG_TXT("EF File Already Exist.\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("EF can not be created.\n");
            break;
        }

        /* Select EF */
        eStatus = phpalCt_Transceive( (uint8_t *)Select_EF, sizeof(Select_EF),bApduRespBuffer,&wReceiveSize);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }
        if((bApduRespBuffer[wReceiveSize-2] == 0x90) && (bApduRespBuffer[wReceiveSize-1] == 0x00))
        {
            LOG_TXT("Select EF done.\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("Could not select EF.\n");
            break;
        }

        /** Perform Write binary. */
        eStatus = phpalCt_Transceive( (uint8_t *)Write_binary, sizeof(Write_binary),bApduRespBuffer,&wReceiveSize);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }
        if((bApduRespBuffer[wReceiveSize-2] == 0x90) && (bApduRespBuffer[wReceiveSize-1] == 0x00))
        {
            LOG_TXT("Write binary done.\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("Write binary command failed.\n");
            break;
        }

        /* Perform read binary */
        eStatus = phpalCt_Transceive( (uint8_t *)Read_binary, sizeof(Read_binary),bApduRespBuffer,&wReceiveSize);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }
        if((bApduRespBuffer[wReceiveSize-2] == 0x90) && (bApduRespBuffer[wReceiveSize-1] == 0x00))
        {
            LOG_TXT("Read binary done.\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("Read binary command failed.\n");
            break;
        }

        /** Delete EF. */
        eStatus = phpalCt_Transceive(  (uint8_t *)Delete_EF, sizeof(Delete_EF),bApduRespBuffer,&wReceiveSize);
        if(PH_ERR_SUCCESS != eStatus)
        {
            break;
        }
        if((bApduRespBuffer[wReceiveSize-2] == 0x90) && (bApduRespBuffer[wReceiveSize-1] == 0x00))
        {
            phLED_SetPattern(gkphLED_Ct_Pass);
            LOG_TXT("Delete EF done.\n");
        }
        else
        {
            phLED_SetPattern(gkphLED_Ct_Fail);
            LOG_TXT("Could not delete EF.\n");
        }
    }while(0);

    if(PH_ERR_SUCCESS != eStatus )
    {
        if(!bAtrFlag)
        {
            LOG_TXT("Error in transceiving Apdu.\n");
            LOG_X32("Error Status   :", eStatus);
        }
        phLED_SetPattern(gkphLED_Ct_Fail);
    }
    /** Card Deactivation. */
    phhalCt_CardDeactivate();
    LOG_TXT("Remove CT card.\n");
    return eStatus;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
