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
 * This file contains Hal apis for Ct synchronous profile.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-11-30 11:55:53 +0530 (Wed, 30 Nov 2016) $
 * $Author: Stevenson James (NXP66090) $
 * $Revision: 18761 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SYNCCT
#include "phhalPmu.h"
#include "phhalPcr.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "ph_Reg.h"
#include "phhalSyncCt.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/* Macro for selection of 5v operating voltage for card.*/
#define PHHAL_CT_VCC5                     0x00


/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static uint8_t gbActivationType;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phhalSyncCt_Class1Activation(uint8_t *pbAtrHeaderBuffer);
static phStatus_t phhalSyncCt_Class2Activation(uint8_t *pbAtrHeaderBuffer);
static void phhalSyncCt_SendClock(void);


/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/**
 * This function is used to initialize the CT IP for synchronus card profile.
 * @param     uint8_t bPullupSetStatus;        If logic set to 1 Pull up else  pull down config.
 * @param    uint8_t bCardPresConnectorStatus;  If Logic 1 Normally open else normally close config.
 * @param    uint8_t bSlewRateConfig;           To rise from 0 to its selected value (see ct_pcr_reg register), the time
                                                    needed by VCC depends on the capacitance.
 * @retval    #PH_ERR_SUCCESS - Initialization of CT IP is successful.
 * @retval    #PH_ERR_FAILED  - Initialization of CT IP failed.
 */
phStatus_t phhalSyncCt_Init(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,uint8_t bSlewRateConfig)
{
    /* Enables the Contact Interface */
    PH_REG_SET_BIT(PCR_SYS_REG, ENABLE_CT);

    /* Enables the CLOCK for CT IP */
    PH_REG_SET_BIT(PCR_CLK_CFG_REG, CLOCK_CTIF_ENABLE);

    /* Enables the CLOCK Gating for CT IP*/
    PH_REG_SET_BIT(PCR_CLK_CFG_REG, IPCLOCK_CTIF_ENABLE);

    /* Refer to the function header for the meaning of this setting */
    if (bPullupSetStatus)
    {
        PH_REG_SET_BIT(CT_SSR_REG, PRES_PUP_EN);
    }

    /* Refer to the function header for the meaning of this setting */
    if ( bCardPresConnectorStatus)
    {
        PH_REG_SET_BIT(CT_SSR_REG, PRES_CON_NO);
    }


    /* Added for the slew rate configuration */
    PH_REG_SET(CT_SRR_REG, bSlewRateConfig);
    /* Enabling Synchronous card profiling in Ct.*/
    PH_REG_SET_BIT(CT_CCRx_REG, SAN);
    /* Clearing bit SHL And Resetin for further use.*/
    PH_REG_CLEAR_BIT(CT_CCRx_REG, SHL);
    PH_REG_CLEAR_BIT(CT_PCR_REG, RSTIN);
    return PH_ERR_SUCCESS ;
}


/**
 * This function is used to check if card is present in the main slot.
 * @maskedret #PH_ERR_CARD_PRESENT - If Main slot is chosen and card is present.
 * @maskedret #PH_ERR_CARD_ABSENT - If Main slot is chosen and card is absent.
 */
phStatus_t phhalSyncCt_CheckCardPres(void)
{
    phStatus_t eStatus = PH_ERR_FAILED;

    if (PH_REG_TEST_BIT(CT_MSR_REG, PRES))
    {
        eStatus = PH_ERR(SYNC_CT_CARD_PRESENT,HAL_SYNC_CT);
    }
    else
    {
        eStatus = PH_ERR(SYNC_CT_CARD_ABSENT,HAL_SYNC_CT);
    }
   return eStatus;
}

/**
  *
  * This function is used to performs activation.Configures the Setting according to Type 1 first if not successful then
  * according to Type 2.If success, returns the ATR to the user.
  *
  * @param   pbAtrBuffer - Pointer to the ATR buffer where the ATR header bytes will be stored
  *
  * @retval    #PH_ERR_SUCCESS -  Activation successful and ATR bytes are received successfully
  * @retval    #PH_ERR_FAILED - Activation not successful.
  * @maskedret #PH_ERR_SYNC_CT_CARD_ABSNET - Card is absent in the slot.
  * @maskedret #PH_ERR_SYNC_CT_CARD_PRESENT - Card is present in slot.
  */
phStatus_t phhalSyncCt_CardActivate(uint8_t* pbAtrBuffer , phhalSyncCt_ActivationType_t eActivationType)
{
    phStatus_t eStatus = PH_ERR_FAILED;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if ((pbAtrBuffer == NULL)||((E_TYPE1 != eActivationType) &&(E_TYPE2 != eActivationType)))
        {
            eStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }
#endif

        eStatus = phhalSyncCt_CheckCardPres();
        if(eStatus != PH_ERR(SYNC_CT_CARD_PRESENT,HAL_SYNC_CT))
        {
            break;
        }
        PH_REG_SET(CT_UTR_URR_REG_adr1,CT_UTR_URR_REG_adr1__RESET_VALUE);
        /* power up the card.*/

        /* Selecting 5 volt for synchronous card activation.because in spec operating voltage is written as 5v.*/
        PH_REG_SET_FIELD(CT_PCR_REG, VCCSEL1_VCCSEL0 , PHHAL_CT_VCC5);

        PH_REG_SET_BIT(CT_PCR_REG, START);
        /* wait for To stable Vcc .*/
        (void)phUser_Wait(5000);
        if(E_TYPE1 == eActivationType)
        {
            eStatus = phhalSyncCt_Class1Activation(pbAtrBuffer);
            gbActivationType = E_TYPE1;
        }
        else
        {
            eStatus = phhalSyncCt_Class2Activation(pbAtrBuffer);
            gbActivationType = E_TYPE2;
        }

        if((pbAtrBuffer[0] == 0x00)||(pbAtrBuffer[0] == 0xFF))
        {
            eStatus = phhalSyncCt_CardDeactivate();
            eStatus = PH_ERR_SYNC_CT_ACTIVATION_FAILED;
            break;
        }
        return PH_ERR_SUCCESS;
    }while(0);
    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_SYNC_CT);
}
/**
  *
  * This function is used to get bit from card.
  * @param     pbReceivedbit  ->  point to received bit from card.
  *            0 -> if received bit is 0.
  *            1 -> if received bit is 1.
  * @retval    #PH_ERR_SUCCESS
  *
  */

phStatus_t phhalSyncCt_SetClock(uint8_t bClockValue)
{
    if(bClockValue)
    {
        /* Setting SHL pin at high.*/
        PH_REG_SET_BIT(CT_CCRx_REG, SHL);
    }
    else
    {
        /* Setting SHL pin at low.*/
        PH_REG_CLEAR_BIT(CT_CCRx_REG, SHL);
    }
    return PH_ERR_SUCCESS;
}
/**
  *
  * This function is used to get bit from card.
  * @param     pbReceivedbit  ->  point to received bit from card.
  *            0 -> if received bit is 0.
  *            1 -> if received bit is 1.
  * @retval    #PH_ERR_SUCCESS
  *
  */
phStatus_t  phhalSyncCt_GetSyncBit(uint8_t *pbReceivedbit)
{

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbReceivedbit == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif
    if((uint8_t) PH_REG_GET(CT_UTR_URR_REG_adr1) & 0x01)
    {
        *pbReceivedbit = 1;
    }
    else
    {
        *pbReceivedbit = 0 ;
    }
    return PH_ERR_SUCCESS;
}


/**
  * This function is used to set bit in uart for sending it to card.
  * @param   uint8_t bTransmitBit -> it indicates the bit to transmit.
  * @retval    #PH_ERR_SUCCESS
  */
phStatus_t phhalSyncCt_SetSyncBit(uint8_t bTransmitBit)
{
    PH_REG_SET(CT_UTR_URR_REG_adr1,bTransmitBit);
    return PH_ERR_SUCCESS;
}

/**
  * This function is used to set transmission or reception mode for uart.
  * @param  uint8_t bValue -> if set to logic 1 ,transmit mode.
  *                            if set to logic 0,Reception mode.
  */
phStatus_t  phhalSyncCt_SetTRMode(uint8_t bValue)
{
    if(bValue)
    {
        /* Set for transmission mode.*/
        PH_REG_SET_BIT(CT_UCR1x_REG, T_R);
    }
    else
    {
        /* Set for Receive mode.*/
        PH_REG_CLEAR_BIT(CT_UCR1x_REG,T_R);
    }
    return PH_ERR_SUCCESS;
}

/**
  * This function is used to Receive byte from card.
  * @param pbReceiveByte - Pointer to Byte received from card.
  * @retval #PH_ERR_SUCCESS - Byte have been received successfully
*/
phStatus_t phhalSyncCt_ReceiveByte(uint8_t *pbReceivedByte)
{
    uint8_t bCount = 0x00;
    uint8_t bReceivedbit = 0x00;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbReceivedByte == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif

    /* Set for Receive mode.*/
    phhalSyncCt_SetTRMode(0);
    (void)phUser_Wait(1000);
    for(bCount = 0 ; bCount <8 ;bCount++)
    {
        (*pbReceivedByte) = (*pbReceivedByte) >> 1;
        phhalSyncCt_SetClock(1);
        phhalSyncCt_GetSyncBit(&bReceivedbit);
        if(bReceivedbit)
        {
            *pbReceivedByte += (uint8_t)(0x80);
        }
        (void)phUser_Wait(40);
        phhalSyncCt_SetClock(0);
        (void)phUser_Wait(40);
    }
    return PH_ERR_SUCCESS;
}

/**
  * This function is used to send byte  to card.
  * @param  uint8_t bSendByte-> it indicates the byte to transmit.
  * @retval #PH_ERR_SUCCESS - Bytes have been transmitted successfully
*/
phStatus_t phhalSyncCt_TransmitByte(uint8_t bTransmitByte)
{
    uint8_t bCount = 0x00;
    /* Set for Transmit mode.*/
    phhalSyncCt_SetTRMode(1);
    (void)phUser_Wait(1000);
    for(bCount = 0 ; bCount <8 ;bCount++)
    {
       /* Send LSB Bit first.*/
        if(bTransmitByte & 0x01)
        {
            phhalSyncCt_SetSyncBit(1);
        }
        else
        {
            phhalSyncCt_SetSyncBit(0);
        }
        phhalSyncCt_SendClock();
        bTransmitByte = bTransmitByte>>1;
    }
    return PH_ERR_SUCCESS;
}


/**
 *This function is used to set value for reset contact pin  for contact card.
 *@param - uint8_t bValue - it indicates value to be set for reset pin.
 *@retval #PH_ERR_SUCCESS
 */
phStatus_t phhalSyncCt_Reset(void)
{
    /* Perform a Soft reset this will reset the whole CT UART IP */
    PH_REG_CLEAR_BIT(CT_SSR_REG, NOT_SOFTRESET);
    /* Delay to wait for one clock cycle */
    (void)phUser_Wait(100);
    if (PH_REG_TEST_BIT(CT_SSR_REG, NOT_SOFTRESET))
    {
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ERR_FAILED;
    }
}

/**
 * This Static function is used to activate card according to Type 1 operating condition.
 */
static phStatus_t phhalSyncCt_Class1Activation(uint8_t *pbAtrHeaderBuffer)
{
    uint8_t bCount = 0x00;

    (void)phUser_Wait(4000);
    PH_REG_SET_BIT(CT_PCR_REG, RSTIN);
    (void)phUser_Wait(10000);
    phhalSyncCt_SetClock(1);
    (void)phUser_Wait(4000);
    phhalSyncCt_SetClock(0);
    (void)phUser_Wait(4000);
    PH_REG_CLEAR_BIT(CT_PCR_REG, RSTIN);
    (void)phUser_Wait(4000);
    for(bCount = 0x00 ; bCount< 4 ;bCount++)
    {
       phhalSyncCt_ReceiveByte(&pbAtrHeaderBuffer[bCount]);
    }
    return PH_ERR_SUCCESS;
}

/**
 * This Static function is used to activate card according to Type 2 operating condition.
 */
static phStatus_t phhalSyncCt_Class2Activation(uint8_t *pbAtrHeaderBuffer)
{
    uint8_t bCount = 0x00;

    phhalSyncCt_SetClock(1);
    (void)phUser_Wait(20);
    phhalSyncCt_SetClock(0);
    (void)phUser_Wait(10);
    /* FCB is C4 contact for card.*/
    PH_REG_SET_BIT(CT_PCR_REG, C4);

    (void)phUser_Wait(20);
    for(bCount = 0x00 ; bCount< 4 ;bCount++)
    {
        phhalSyncCt_ReceiveByte(&pbAtrHeaderBuffer[bCount]);
    }
    return PH_ERR_SUCCESS;
}

/**
  * This function is used to Send n number of  Clock cycles to card.
  * @param  uint32_t dwCount - it indicates the number of clock cycle to be send.
  * @retval #PH_ERR_SUCCESS
 */
phStatus_t phhalSyncCt_Send_Nbclocks(uint32_t dwCount)
{

    for(;dwCount != 0x00; dwCount--)
    {
        (void)phhalSyncCt_SendClock();
    }
    return PH_ERR_SUCCESS;
}

/**
 * This static function is used to send one clock pulse.
 */
static void phhalSyncCt_SendClock(void)
{
    PH_REG_SET_BIT(CT_CCRx_REG, SHL);
    (void)phUser_Wait(40);
    PH_REG_CLEAR_BIT(CT_CCRx_REG, SHL);
    (void)phUser_Wait(40);
}
/**
  * This function is used to deactivate the card.
  *
  * @retval    #PH_ERR_SUCCESS - Card is deactivated successfully
  */
phStatus_t phhalSyncCt_CardDeactivate(void)
{
    /* Clock at low stage.*/
    PH_REG_CLEAR_BIT(CT_CCRx_REG, SHL);
    if(E_TYPE2 == gbActivationType)
    {
        PH_REG_CLEAR_BIT(CT_PCR_REG, C4);
    }
    /*Reset pin already low and remove power supply.*/
    PH_REG_CLEAR_BIT(CT_PCR_REG, START);
    (void)phUser_Wait(10000);
    return PH_ERR_SUCCESS;
}


/**
  * This function is used to reset CT-IP for synchronous card profile.
  * @retval    #PH_ERR_SUCCESS - if IP is reset successfully.
  */
phStatus_t phhalSyncCt_SetReset(uint8_t bValue)
{
    if(bValue)
    {
        PH_REG_SET_BIT(CT_PCR_REG, RSTIN);
    }
    else
    {
        PH_REG_CLEAR_BIT(CT_PCR_REG, RSTIN);
    }
    return PH_ERR_SUCCESS;
}

/**
 * This function does the DeInitialization of the CT IP for synchronus card profile.
 * @retval    #PH_ERR_SUCCESS De-Initialization operation completed successfully.
 * @retval    #PH_ERR_FAILED De-Initialization failed
 */
phStatus_t phhalSyncCt_DeInit(void)
{

    if (phhalSyncCt_Reset() != PH_ERR_SUCCESS)
    {
        return PH_ERR(FAILED,HAL_CT);
    }

    /* De-configuring the CT specific PCR Registers */
    PH_REG_CLEAR_BIT(PCR_SYS_REG, ENABLE_CT);
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG,CLOCK_CTIF_ENABLE);
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG, IPCLOCK_CTIF_ENABLE);
    PH_REG_CLEAR_BIT(PCR_SYS_REG, AUTOMATIC_CT_DEACT);
    return PH_ERR_SUCCESS;
}


#endif /* NXPBUILD__PHHAL_SYNCCT */
