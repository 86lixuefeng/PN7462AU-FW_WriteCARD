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
 * phExPos_Poll.c: Contains the core logic for the Clif Reader part.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-15 12:16:28 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18642 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phRtos.h"
#include "phacDiscLoop.h"
#include "phExPos_Poll.h"
#include "phExPos_Clif.h"
#include "phExPos_TypeAEMV.h"
#include "ph_Log.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p3b.h"
#include "phpalSli15693.h"
#include "phUser.h"
#include "phExPos_Common.h"
#include "phExPos_Utils.h"
#include "phExPos_SPIHIF.h"
#include "phpalI14443p4.h"
#include "phUser.h"
#include "phLED.h"

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
extern phExPos_Posfw_Data_t gphExPos_UtilsPosParama;
uint32_t bRxLen;
uint16_t wOption=0;
uint16_t wTxLen;
uint32_t *dpRxBuf;
uint8_t bNumSlots = 0;
uint8_t bAfi = 0x00;
uint8_t bAtqbLen = 0;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * Example application's Reader mode operations entry point.
 *
 * @param psDiscLoopParams : Discovery loop data params.
 */
void phExPos_Poll_Main(phacDiscLoop_Sw_DataParams_t *psDiscLoopParams)
 {
 	static  EventBits_t uxBits;
 	static phStatus_t status = PH_ERR_FAILED;

     if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_A))
     {
    	 /*Check whether the inserted card is a banking card or non banking card*/
    	 if ((psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak!=PH_EXPOS_COMMON_MIFARECLASSIC_1K_SAK) &&
    			 (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak!=PH_EXPOS_COMMON_MIFARECLASSIC_4K_SAK) &&
    			 (psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak!=PH_EXPOS_COMMON_MIFAREULTRALIGHT_SAK))
    	{
        uxBits = 0;
        status = phExPos_TypeAEMV(psDiscLoopParams);
        if (status == PH_ERR_SUCCESS)
        {
          gphExPos_UtilsPosParama.Uid_Length = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;
     	  phUser_MemCpy (&gphExPos_UtilsPosParama.Uid, &psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExPos_UtilsPosParama.Uid_Length);

           /*card L3 and L4 activation success, update event in POS structure and notify to host*/
     	   phExPos_UtilsGenerateATRFromAts(psDiscLoopParams->sTypeATargetInfo.sTypeA_I3P4.pAts);

           gphExPos_UtilsPosParama.bAtrValid = true;

           gphExPos_UtilsPosParama.eCurrEvent = E_BANKING_CARD_INSERTION_EVENT_TYPEA;
           gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CL_CHANNEL_NO;

           /* stop the polling timer */
           phhalTimer_Stop(gpphExPos_Clif_PollTimer);

           /* Raise the interrupt to Host */
           phExPos_Utils_Raise_Interrupt();
           /*Glow Yellow LED to indicate the contact less card insertion*/
           phExPos_UtilsGlow_CLIF_Card_Detection_LED();

             /*Infinitely wait for APDU commands from host */
           while (true)
           {
        	   uxBits = phRtos_EventGroupWaitBits(gphExPos_UtilsPosParama.xCL_Events,
                                           PH_EXPOS_COMMON_CL_TRNSP_EX_CMD |
                                           PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD |
                                           PH_EXPOS_COMMON_CL_ABORT_CMD,
                                           pdTRUE, /* status bits should be cleared before returning. */
                                           pdFALSE, /* wait for any status bit being set. */
                                           portMAX_DELAY); /* wait until the time expires. */

              /*card deactivate command from host, deactivate card, update event in
               *  POS structure and notify host*/
              if ((uxBits & (uint32_t) PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD))
              {
            	 phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
                 /* Card Removal process. */
                 gphExPos_UtilsPosParama.bAtrValid = false;
                 gphExPos_UtilsPosParama.bTxDataValid = 1;
                 gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
                 gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
                 gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
                 gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
                 gphExPos_UtilsPosParama.bpTxBuf[4] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0xFF00) >> 0x08;
                 gphExPos_UtilsPosParama.bpTxBuf[5] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0x00FF);

                 phExPos_UtilsSendResponseToHost();
                 if (gphExPos_UtilsPosParama.Trans_complete)
                 {
                	 /*If all commands are through including Generate AC command then glow LEDs to indicate successful transaction*/
                	 phExPos_UtilsGlow_Success_LEDS();
                 }
                 else
                 {
                	 phExPos_UtilsGlow_Error_LEDS();
                 }
                 /* Card Removal process. */
                 status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
                      psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);

                 status = status & PH_ERR_MASK;
                 while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR)))
                 {
                     (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
                     phUser_Wait(500000);
                     status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
                         psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);
                     status = status & PH_ERR_MASK;
                 }
                /*card removed from field, notify host  through
                 * interrupt*/
                gphExPos_UtilsPosParama.eCurrEvent = E_CARD_REMOVAL_EVENT;
                gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CHANNEL_NONE;
                phExPos_UtilsAll_LEDS_OFF();
                phExPos_Utils_Raise_Interrupt();
                break;
              }
              /*APDU command from HOST, do transparent exchange and send
              * response to host*/
              else if (uxBits & (uint32_t) PH_EXPOS_COMMON_CL_TRNSP_EX_CMD)
              {
                 wTxLen = (uint16_t)gphExPos_UtilsPosParama.aRxBuff[0];
                 gphExPos_UtilsPosParama.bTxDataValid = 1;
                 status = phpalI14443p4_Exchange(psDiscLoopParams->pPal14443p4DataParams,
                          wOption,
                          (uint8_t *)&gphExPos_UtilsPosParama.aRxBuff[1],
                          wTxLen,
						  (uint8_t **)&dpRxBuf,
                          (uint16_t *)&bRxLen);

                 if (status == PH_ERR_SUCCESS)
                 {
                    /*APDU exchange success, send response to host*/
                    gphExPos_UtilsPosParama.bTxDataValid = 1;
                    gphExPos_UtilsPosParama.aTxBuff[0] = bRxLen;
                    /*Copy response from card to response buffer of host*/
                    phUser_MemCpy(&gphExPos_UtilsPosParama.aTxBuff[1], dpRxBuf, bRxLen);

                    phExPos_UtilsSendResponseToHost();

                 /*Glow LED if generate AC command is successful*/
                 if(((gphExPos_UtilsPosParama.bpRxBuf[5] == 0xAE)) &&
                     (gphExPos_UtilsPosParama.bpTxBuf[bRxLen + 2] == 0x90))
                 {
                	 gphExPos_UtilsPosParama.Trans_complete = 1;
                 }

                 }
                 else
                 {
                    /*Transparent exchange failed, reason could be card removal
                     * send return status to host*/
                    phExPos_UtilsGlow_Error_LEDS();

                    /* Card Removal process. */
                    status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
                         psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);

                    status = status & PH_ERR_MASK;
                    while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR)))
                    {
                        (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
                        phUser_Wait(500000);
                        status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
                            psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);
                        status = status & PH_ERR_MASK;
                    }
                   /*card removed from field, notify host  through
                    * interrupt*/
                   gphExPos_UtilsPosParama.eCurrEvent = E_CARD_REMOVAL_EVENT;
                   gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CHANNEL_NONE;
                   phExPos_UtilsAll_LEDS_OFF();

                    gphExPos_UtilsPosParama.bpTxBuf[0] = 2;
                    gphExPos_UtilsPosParama.bpTxBuf[1] = 0;
                    gphExPos_UtilsPosParama.bpTxBuf[2] = 0;
                    gphExPos_UtilsPosParama.bpTxBuf[3] = 0;
                    gphExPos_UtilsPosParama.bpTxBuf[4] = status & 0x00ff; // error code
                    gphExPos_UtilsPosParama.bpTxBuf[5] = ((0xff00 & status) >> 8);

                    phExPos_UtilsSendResponseToHost();

                   break;
                 }
              }
              else if ((uxBits & (uint32_t) PH_EXPOS_COMMON_CL_ABORT_CMD))
            	  /*Abort command will not be used*/
                 {
                  phExPos_UtilsSendResponseToHost();
                    break;
                 }
                 else
                 {
                    gphExPos_UtilsPosParama.eCurrEvent = E_STS_NO_EVENT;
                 }
              } // end of while
           }
    	}


    	else
 		   /*as per SAK value card does not support ISO14443-4
 			* indicate to host as non banking card*/
    	{
    		 gphExPos_UtilsPosParama.Uid_Length = psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;
       	     phUser_MemCpy (&gphExPos_UtilsPosParama.Uid, &psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid, gphExPos_UtilsPosParama.Uid_Length);

			  gphExPos_UtilsPosParama.eCurrEvent = E_NONBANKING_CARD_INSERTION_EVENT_TYPEA;
			  gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CL_CHANNEL_NO;

			  /* stop the polling timer */
			  phhalTimer_Stop(gpphExPos_Clif_PollTimer);

			  phExPos_UtilsAll_LEDS_OFF();
			  /*Glow Yellow LED to indicate the contact less card insertion*/
			  phExPos_UtilsGlow_CLIF_Card_Detection_LED();
			  phExPos_UtilsGlow_Error_LEDS();

			  phExPos_Utils_Raise_Interrupt();
			  phExPos_SPIHIFGPIOLineIdle();

			  phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
              /* Card Removal process. */
              status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
                   psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);

              status = status & PH_ERR_MASK;
              while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR)))
              {
                  (void)phpalI14443p3a_HaltA(psDiscLoopParams->pPal1443p3aDataParams);
                  phUser_Wait(500000);
                  status = phpalI14443p3a_WakeUpA(psDiscLoopParams->pPal1443p3aDataParams,
                      psDiscLoopParams->sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);
                  status = status & PH_ERR_MASK;
              }

			  /*card removed from field, notify host  through interrupt*/
			  phExPos_UtilsAll_LEDS_OFF();
			  phExPos_Utils_Raise_Interrupt();

			  gphExPos_UtilsPosParama.eCurrEvent = E_CARD_REMOVAL_EVENT;
			  gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CHANNEL_NONE;

    	 }
       }

    if (psDiscLoopParams->bDetectedTechs & (1 << PHAC_DISCLOOP_TECH_TYPE_B))
    {
    	 gphExPos_UtilsPosParama.Uid_Length = 4;
  	     phUser_MemCpy (&gphExPos_UtilsPosParama.Uid, &psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aPupi, gphExPos_UtilsPosParama.Uid_Length);

         gphExPos_UtilsPosParama.eCurrEvent = E_BANKING_CARD_INSERTION_EVENT_TYPEB;
         gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CL_CHANNEL_NO;

		  /* stop the polling timer */
		  phhalTimer_Stop(gpphExPos_Clif_PollTimer);

		  phExPos_UtilsAll_LEDS_OFF();
		  phExPos_UtilsGlow_CLIF_Card_Detection_LED();
		  phExPos_UtilsGlow_Error_LEDS();

		  phExPos_Utils_Raise_Interrupt();
		  phExPos_SPIHIFGPIOLineIdle();

          while (true)
          {
			   uxBits = phRtos_EventGroupWaitBits(gphExPos_UtilsPosParama.xCL_Events,
											  PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD,
											  pdTRUE, /* status bits should be cleared before returning. */
											  pdFALSE, /* wait for any status bit being set. */
											  portMAX_DELAY); /* wait until the time expires. */

			 /*card deactivate command from host, deactivate card, update event in
			   *  POS structure and notify host*/
			  if ((uxBits & (uint32_t) PH_EXPOS_COMMON_CL_DEACTIVATE_CARD_CMD))
			  {
				 phpalI14443p4_Deselect(psDiscLoopParams->pPal14443p4DataParams);
				 /* Card Removal process. */
				 gphExPos_UtilsPosParama.bAtrValid = false;
				 gphExPos_UtilsPosParama.bTxDataValid = 1;
				 gphExPos_UtilsPosParama.bpTxBuf[0] = 0x02; /* Buffer Len*/
				 gphExPos_UtilsPosParama.bpTxBuf[1] = 0x00;
				 gphExPos_UtilsPosParama.bpTxBuf[2] = 0x00; /* Buffer Len*/
				 gphExPos_UtilsPosParama.bpTxBuf[3] = 0x00;
				 gphExPos_UtilsPosParama.bpTxBuf[4] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0xFF00) >> 0x08;
				 gphExPos_UtilsPosParama.bpTxBuf[5] = (PH_EXPOS_COMMON_CMD_SUCCESS_CODE & 0x00FF);

                 phExPos_UtilsSendResponseToHost();

                /* Card Removal process. */
                status = phpalI14443p3b_WakeUpB(psDiscLoopParams->pPal1443p3bDataParams, bNumSlots, bAfi,
						psDiscLoopParams->sTypeBTargetInfo.bExtendedAtqBbit, (uint8_t *)&(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB), &bAtqbLen);

                status = status & PH_ERR_MASK;
                while(((status  == PH_ERR_SUCCESS) || (status == PH_ERR_COLLISION_ERROR)))
                {
                    (void)phpalI14443p3b_HaltB(psDiscLoopParams->pPal1443p3bDataParams);
                    phUser_Wait(500000);
                    status = phpalI14443p3b_WakeUpB(psDiscLoopParams->pPal1443p3bDataParams, bNumSlots, bAfi,
    						psDiscLoopParams->sTypeBTargetInfo.bExtendedAtqBbit, (uint8_t *)&(psDiscLoopParams->sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB), &bAtqbLen);
                    status = status & PH_ERR_MASK;
                }
			    /*card removed from field, notify host  through interrupt*/
			    phExPos_UtilsAll_LEDS_OFF();
			    phExPos_Utils_Raise_Interrupt();

			    gphExPos_UtilsPosParama.eCurrEvent = E_CARD_REMOVAL_EVENT;
			    gphExPos_UtilsPosParama.bActiveSlot = PH_EXPOS_COMMON_CHANNEL_NONE;
                break;
			    }

              }
        }
 }


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

