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

/** @ file
 *
 * Contact Interface : Protocol Abstraction Layer
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-17 15:55:08 +0530 (Thu, 17 Dec 2015) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 17033 $ (v04.13.00)
 *
 */

/* *******************************************************************************************************************
 * Includes
 * ****************************************************************************************************************** */
#include <ph_Datatypes.h>
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHPAL_CT
#include <ph_Status.h>
#include "phUser.h"
#include "phpalCt.h"
#include "phpalCt_T0.h"
#include "phpalCt_T1.h"

/* ******************************************************************************************************************
 * Global Defines
 * ***************************************************************************************************************** */

/* ******************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/**
 *
 */
typedef struct phpalCt_FactTable{
    uint8_t bFiDiValue;
    uint16_t wFactCorr;
}phpalCt_FactTable_t;

/* ******************************************************************************************************************
 * GLOBAL AND STATIC VARIABLES
 * ***************************************************************************************************************** */
/**
 * Final protocol selected
 */
uint8_t gphpalCt_ProtSel;
uint8_t gphpalCt_BIFSC; /* Global variable to store IFSC value from ATR for PAL.*/
uint8_t gphpalCt_BEmvEn;/* Global variable to store EMVCo or NonEMVCo profile for PAL.*/
uint8_t gphpalCt_BSadDad;/* NAD for  card to terminal communication.*/
uint8_t gphpalCt_BDadSad;/* NAD for terminal to card communication.*/


/* *******************************************************************************************************************
 * Private Function Prototypes
 * *******************************************************************************************************************/

/* This Function is used to find out Class and clock according to Atr's class clock stop indicator byte.*/
static phStatus_t phpalCt_NegotiateClassClock(phhalCt_ProtocolParams_t *psAtrPALParams,uint8_t *bClass);
/* This function is used for processing activation process further after successful card activation.*/
static phStatus_t phPalCt_ProcessActivation(phpalCt_DataParams_t *pDataParams,
        phhalCt_ProtocolParams_t *psAtrPalParams);

/* *******************************************************************************************************************
 *   Public Functions
 * *******************************************************************************************************************/

/**
 * @brief This Api is used to initialize the CT IP and is called from the upper layer
 * @param psCtInitParams   Pointer to Structure which contains Ct initialization parameters.
 *         PH_ERR_FAILED - If resetting the CP IP is unsuccessful
 */
phStatus_t phpalCt_Init(phhalCt_InitParam_t *psCtInitParams)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;

    eStatus = phhalCt_Init(psCtInitParams);
    phpalCt_T1_Init();
    gphpalCt_BEmvEn = TRUE;
    return eStatus;
}

/**
 * Set the CT pal configurations to the desired value.
 */
phStatus_t phpalCt_SetConfig(phhalCt_Configs_t eConfig, uint32_t dwValue)
{
    phStatus_t wStatus;

    switch(eConfig)
    {
    case E_EMV_ENABLE:
        gphpalCt_BEmvEn =  (uint8_t)((dwValue>0)? 1: 0);
        wStatus = phhalCt_SetConfig(E_EMV_ENABLE, dwValue);
        break;
    default:
        wStatus = PH_ERR(INVALID_PARAMETER, HAL_CT);
    }

    return wStatus;
}


/**
 *@brief   phpalHwCt_ActivateCard, This Function does a cold reset/activate of the card for ATR reception,
 *         the API performs three iterations for different vcc selection internally.
 *
 *@param   pDataParams - Pointer to the context structure of the PAL layer
 *         dwSlotNumber - Slot number of of which the card is present and activation has to be carried out
 *@return  PH_ERR_SUCCESS - If the cold activation is success and ATR is received
 *
 */
phStatus_t phpalCt_ActivateCard(phpalCt_DataParams_t *pDataParams)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    uint8_t bcount=0,bActivateSequence[]={PHHAL_CT_VCC5,PHHAL_CT_VCC3,PHHAL_CT_VCC1M8};
    uint8_t bClass;
    uint8_t bEarlyFlag = FALSE;

    phhalCt_ProtocolParams_t sAtrPalParams;


    do
    {

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if(NULL == pDataParams)
            return PH_ERR(INVALID_PARAMETER,PAL_CT);
        if(gphpalCt_BEmvEn)
        {
            if(pDataParams->bSizeOfATRbuffer < PHPAL_CT_EMV_MAX_ATR_BYTES)
                return PH_ERR(INVALID_PARAMETER,PAL_CT);
        }
        else
        {
            if(pDataParams->bSizeOfATRbuffer < PHPAL_CT_7816_MAX_ATR_BYTES)
                return PH_ERR(INVALID_PARAMETER,PAL_CT);
        }
#endif

        eStatus = phhalCt_CardActivate(bActivateSequence[bcount], pDataParams->pbAtrBuffer,
                                         &(pDataParams->bAtrReceivedLength),&sAtrPalParams);
        /* if in 7816,Atr having class indicator comes.*/
        while(eStatus == (PH_ERR_CT_CLASS_CHANGE_INDICATED | PH_COMP_HAL_CT))
        {
            bClass = bActivateSequence[bcount];
            eStatus = phpalCt_NegotiateClassClock(&sAtrPalParams ,&bClass);
            if(bClass != bActivateSequence[bcount] )
            {
                (void)phhalCt_CardDeactivate();
                (void)phUser_Wait(10000);
                eStatus = phhalCt_CardActivate(bClass, pDataParams->pbAtrBuffer,
                                                   &(pDataParams->bAtrReceivedLength),&sAtrPalParams);
                bActivateSequence[bcount] = bClass;
            }
            else
            {
                eStatus = PH_ERR_SUCCESS;
            }

        }

        if((eStatus == PH_ERR_SUCCESS)||(eStatus == (PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT)))
        {
            if(eStatus == (PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT))
            {
                bEarlyFlag = TRUE;
            }

            eStatus = phPalCt_ProcessActivation(pDataParams, &sAtrPalParams);
            PH_BREAK_ON_FAILURE(eStatus);
            /* Assign the handle to the context structure */

            pDataParams->bSupportedClass = bActivateSequence[bcount];
            if(bEarlyFlag)
            {
                eStatus = (PH_ERR_CT_EARLY_ERROR| PH_COMP_HAL_CT);
            }
            break;
        }
        /* Card not present error or MUTE error we should check */
        else if((eStatus == (PH_ERR_CT_MUTE_ERROR | PH_COMP_HAL_CT))||
                (eStatus == (PH_ERR_CT_MAIN_CARD_ABSENT | PH_COMP_HAL_CT)))
        {
            (void)phhalCt_CardDeactivate();
            (void)phUser_Wait(10000);
            bcount++;
        }
        else
        {
            /* Any other error break from the loop, no need to activate further */
            break;
        }
    }while(bcount<3);
    return eStatus;
}

/**
 *@brief   This Function is used to set the Transmission protocol to T=0 or T=1 from the upper layer
 *@param   bProtocolType - bProtocolType or T=0 or T=1 to be passed from the upper layer
 *
 *@return  PH_ERR_SUCCESS - If the protocol set successfully
 *         PH_ERR_CT_PROTOCOL_NOT_SUPPORTED - If invalid protocol parameter is passed
 */
phStatus_t phpalCt_SetProtocol(phpalCt_ProtocolType_t eProtocolType)
{
    phStatus_t eStatus = PH_ERR_CT_PROTOCOL_NOT_SUPPORTED;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if((eProtocolType != E_PHPAL_CT_T0) && (eProtocolType  != E_PHPAL_CT_T1) &&
           (eProtocolType  != E_PHPAL_CT_BOTH_T0_T1))
        {
            return PH_ERR(INVALID_PARAMETER,PAL_CT);
        }
#endif
    do
    {
        if((eProtocolType != (phpalCt_ProtocolType_t)(gphpalCt_ProtSel)) &&
           ( E_PHPAL_CT_BOTH_T0_T1 != (phpalCt_ProtocolType_t)(gphpalCt_ProtSel)))
        {
            return PH_ERR(CT_PROTOCOL_NOT_SUPPORTED, PAL_CT);
        }

        /**HAL API used to set the transmission protocol*/
        eStatus = phhalCt_SetTransmissionProtocol((uint8_t)(eProtocolType));
        gphpalCt_ProtSel = eProtocolType;
        phpalCt_T1_Init();
    }while(0);
    return eStatus;
}

phStatus_t phpalCt_SetNAD(uint8_t bDADSAD)
{
    uint8_t btempDADSAD;
    uint8_t btempSADDAD;

    btempDADSAD = bDADSAD;
    btempSADDAD = (uint8_t)(((btempDADSAD &0xF0)>>4)+((btempDADSAD &0x0F)<<4));
    if (  ((btempSADDAD == btempDADSAD) && (bDADSAD != 0)) ||
          ((btempDADSAD & 0x88) != 0 )
       )
     {
        return PH_ERR(CT_DADSAD_NOT_SUPPORTED, PAL_CT);
     }
    gphpalCt_BDadSad = btempDADSAD;
    gphpalCt_BSadDad = btempSADDAD;
    return PH_ERR_SUCCESS;
}
/**
 *
 * @brief   This Api calls the appropriate APi's of the underlying protocol for a T0 or T1 related transcieve.
 * @param   pbTransmitBuff - Pointer to the transmit buffer
 * @param   dwTransmitSize - Number of the bytes to be transmitted
 * @param   pbReceiveBuff - Pointer to the receive buffer
 * @param   dwReceiveSize - Pointer to the receive buffer size
 * @return  PH_ERR_INVALID_PARAMETER - If the parameters passed has any errors
 *          PH_ERR_CT_MAIN_CARD_ABSENT - If the card is absent in the main slot
 *          PH_ERR_SUCCESS - If the transaction with the card is successful
 */
phStatus_t phpalCt_Transceive(uint8_t* pbTransmitBuff, uint32_t dwTransmitSize,
                              uint8_t* pbReceiveBuff,  uint16_t* pwReceiveSize)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if((pbTransmitBuff == NULL) || (pbReceiveBuff == NULL) || (pwReceiveSize == NULL))
        {
            return PH_ERR(INVALID_PARAMETER,PAL_CT);
        }
#endif

        /* If the protocol set by the user is T=1 then call the T=1 Transceive api */
        if(gphpalCt_ProtSel == E_PHPAL_CT_T1)
        {
            eStatus = phpalCt_T1_Transcieve(pbTransmitBuff, dwTransmitSize,pbReceiveBuff,  pwReceiveSize);
        }
        else
        {
            /* Max buffer size is 261 only  and minimum size should be greater than 3 */
            if((dwTransmitSize > PHPAL_CT_MAX_APDU_SIZE_T0)  || (dwTransmitSize < PHPAL_CT_MIN_APDU_SIZE_T0))
            {
                return PH_ERR(INVALID_PARAMETER,PAL_CT);
            }
            /* If the protocol set by the user is T=0 then call the T=0 Transceive api */
            eStatus = phpalCt_T0_Transcieve(pbTransmitBuff, dwTransmitSize, pbReceiveBuff,  pwReceiveSize);
        }
    }while(0);
    return eStatus;
}

/**
 * @brief   This Api implements Tranceive Operation with Split Chaining support.
 * @param[in] pbTransmitBuff  Pointer to the transmit buffer
 * @param[in] dwTransmitSize  Size of the bytes to be transmitted
 * @param[out] pbReceiveBuff  Pointer to the receive buffer
 * @param[out] pwReceiveSize  Pointer to the receive buffer size
 * @param [in] eOption  Enum Constant indicating the expected behaviour of the Tranceive operation.

 * @retval #PH_ERR_SUCCESS  The Apdu transmitted successfully and received the response successfully.
 * @retval #PH_ERR_INVALID_PARAMETER  Parameters are invalid.
 *
 * @maskedret    #PH_ERR_CT_RETRY_COUNT_FAILURE    If Retry count has been exceeded than allowable value.
 * @maskedret    #PH_ERR_CT_INS_COMMAND_ERROR    Improper INS is received from card.
 * @maskedret    #PH_ERR_CT_RESYNCH_SUCCESS    If Resynchronisation happened for 7816 profile.
 * @maskedret    #PH_ERR_CT_CHAINING_ABORT_REQUESTED    If Chaining Abortion request is received from card
 * @maskedret    #PH_ERR_CT_EXT_APDU_NOT_SUPPORTED    If Extended command Apdu is passed for T0 protocol.
 * @maskedret    #PH_ERR_CT_PAL_SUCCESS_RX_CHAINING    RX Chaining ongoing. Call phpalCt_Transceive_SplitChaining() with
 *                                                     eOption "E_PHPAL_CT_RX_CHAINING" to receive next block.
 * @maskedret    #PH_ERR_CT_PAL_SUCCESS_TX_CHAINING    TX Chaining Ongoing. Call phpalCt_Transceive_SplitChaining() with
 *                                                     eOption "E_PHPAL_CT_TX_CHAINING" to send next block.
 */
phStatus_t phpalCt_Transceive_SplitChaining(uint8_t* pbTransmitBuff, uint32_t dwTransmitSize,
                                            uint8_t* pbReceiveBuff,  uint16_t* pwReceiveSize,
                                            phpalCt_TransceiveOption_t eOption
                                           )
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        if((pbTransmitBuff == NULL) || (pbReceiveBuff == NULL) || (pwReceiveSize == NULL))
        {
            return PH_ERR(INVALID_PARAMETER,PAL_CT);
        }

        if((eOption < E_PHPAL_CT_TXRX_DEFAULT) || (eOption > E_PHPAL_CT_RX_CHAINING))
        {
            return PH_ERR(INVALID_PARAMETER,PAL_CT);
        }
#endif

        /* Reset the number of bytes received */
        *pwReceiveSize = 0;

        /* If the protocol set by the user is T=1 then call the T=1 Transceive api */
        if(gphpalCt_ProtSel == E_PHPAL_CT_T1)
        {
            eStatus = phpalCt_T1_Transcieve_SplitChaining(pbTransmitBuff, dwTransmitSize,pbReceiveBuff,  pwReceiveSize, eOption);
        }
        else
        {
            /* Max buffer size is 261 only  and minimum size should be greater than 3 */
            if((dwTransmitSize > PHPAL_CT_MAX_APDU_SIZE_T0)  || (dwTransmitSize < PHPAL_CT_MIN_APDU_SIZE_T0))
            {
                return PH_ERR(INVALID_PARAMETER,PAL_CT);
            }
            /* If the protocol set by the user is T=0 then call the T=0 Transceive api */
            eStatus = phpalCt_T0_Transcieve(pbTransmitBuff, dwTransmitSize, pbReceiveBuff,  pwReceiveSize);
        }
    }while(0);
    return eStatus;
}

/**
 *@brief   This function is used to deinitialize the CT IP and disable the NVIC for contact interface
 *@param   void
 *@return  #PH_ERR_SUCCESS If Card Deactivate is done successfully
 *@retval #PH_ERR_CT_MAIN_CARD_PRESENT If the card is absent in the slot
 */
phStatus_t phpalCt_DeInit(void)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    eStatus = phpalCt_T1_DeInit();
    eStatus = phhalCt_DeInit();
    return eStatus;
}

/* *******************************************************************************************************************
 * Private Functions
 * ****************************************************************************************************************** */
/**
 * This function is used to store values for class and clock stop according to ATR.
 * @param  psAtrPALParams - points toward structure for ATR's parameter which will be use in PAL.
 * @param  bClass - points to Class which is supported by Card through atr.
 */
static phStatus_t phpalCt_NegotiateClassClock(phhalCt_ProtocolParams_t *psAtrPALParams ,uint8_t *bClass)
{

    phStatus_t eStatus = PH_ERR_SUCCESS;
    uint8_t bOfferedClass = 0x00;
    bOfferedClass = (uint8_t)(psAtrPALParams->bFlagT15TAValue & (0x3F));

   switch(bOfferedClass)
   {
       case 1:
           *bClass = PHHAL_CT_VCC5;
       break;
       case 2:
       case 3:
           *bClass = PHHAL_CT_VCC3;
       break;
       case 4:
       case 6:
       case 7:
           *bClass = PHHAL_CT_VCC1M8;
       break;
       default:
           eStatus = PH_ERR_INVALID_PARAMETER;
       break;
   }


   return eStatus;
}

static phStatus_t phPalCt_ProcessActivation(phpalCt_DataParams_t *pDataParams,
                                                                        phhalCt_ProtocolParams_t *psAtrPalParams)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    if(psAtrPalParams->bNegotiableMode)
    {
        phhalCt_PPSParamType_t sPPSParam;
        sPPSParam.bBWI = psAtrPalParams->bBWI;
        sPPSParam.bCWI = psAtrPalParams->bCWI;
        sPPSParam.bWI = psAtrPalParams->bWI;
        sPPSParam.bProtSelT0 = psAtrPalParams->bProtSelT0;
        sPPSParam.bProtSelT1 = psAtrPalParams->bProtSelT1;

        if(!(psAtrPalParams->bProtSelT0))
        {
            sPPSParam.bNegotiableProtocol = E_PHPAL_CT_T1;
        }
        else
        {
            sPPSParam.bNegotiableProtocol = E_PHPAL_CT_T0 ;
        }
        sPPSParam.bFiDi  = psAtrPalParams->bFiDi;

         /* Delay of 10 Etu is required for BGT.*/
         phUser_Wait(1000);
         (void)phhalCt_SetTimer(PHHAL_CT_PPSMODE_9600_ETU,0x00);
         eStatus = phhalCt_PPSRequestHandling( &sPPSParam);
         PHHAL_CT_STOP_TIMER;
         if(PH_ERR_SUCCESS != eStatus)
         {
             if((PH_COMP_HAL_CT|PH_ERR_PPS_EXCHANGE_NOT_REQUIRED) == eStatus)
             {
                 eStatus = PH_ERR_SUCCESS;
             }
             else
             {
                 /* Call deactivation  */
                 (void) phhalCt_CardDeactivate();
                 return eStatus;
             }
         }
         psAtrPalParams->bProtSelT0 = sPPSParam.bProtSelT0;
         psAtrPalParams->bProtSelT1 = sPPSParam.bProtSelT1;
     }



     gphpalCt_BIFSC = psAtrPalParams->bIFSC;


     if(!(psAtrPalParams->bProtSelT0))
     {
         pDataParams->bCardSupportedProt = E_PHPAL_CT_T1;
         gphpalCt_ProtSel = E_PHPAL_CT_T1;
         phpalCt_T1_Init();
     }
     else if((psAtrPalParams->bProtSelT0)&& (psAtrPalParams->bProtSelT1))
     {
         pDataParams->bCardSupportedProt = E_PHPAL_CT_BOTH_T0_T1;
         gphpalCt_ProtSel = E_PHPAL_CT_BOTH_T0_T1;
     }
     else
     {
         pDataParams->bCardSupportedProt = E_PHPAL_CT_T0;
         gphpalCt_ProtSel = E_PHPAL_CT_T0;
     }

     /* Wait atleast 10 etu for not violating BGT */
     phUser_Wait(1000);
     return eStatus;

}

#endif /* NXPBUILD__PHPAL_CT */
