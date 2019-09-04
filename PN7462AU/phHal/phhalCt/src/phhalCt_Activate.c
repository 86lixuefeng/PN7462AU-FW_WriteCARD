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
 * Implements CT HAL which directly interacts with CT IP registers.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-08-08 18:10:10 +0530 (Mon, 08 Aug 2016) $
 * $Author: Stevenson James (NXP66090) $
 * $Revision: 18579 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalCt.h"
#include "phhalCt_Int.h"
#include "phUser.h"
#include "ph_Status.h"
#include "phhalPmu.h"
#include "phhalPcr.h"
#include "ph_Platform.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "phhalCt_Event.h"
#include "ph_Reg.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_CT_CHARACTER_TA           0
#define PHHAL_CT_CHARACTER_TB           1
#define PHHAL_CT_CHARACTER_TC           2
#define PHHAL_CT_CHARACTER_TD           3


/* Wait time after the de activation sequence is started
 * This value is calculated on the CT software timer with default baudrate
 * 82.5 uS * 62 =  5084 uS */
#define PHHAL_CT_DCDC_WAIT_TIME               62

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
/**
 * This is helper/common function to process the ATR after cold or warm activation.
 */
static phStatus_t phhalCt_ProcessActivation(uint8_t *pbAtrBuffer, uint8_t *pbAtrSize,
                                            phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * Waiting for the ATR bytes.
 */
static phStatus_t phhalCt_WaitForAtrBytes(uint8_t bFirstByte,phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function parses the ATR bytes and checks the correctness of received ATR characters.
 */
static phStatus_t phhalCt_AtrParser( phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function parses the ATR bytes of interface characters TA1, TA2, TA3 and so on.
 */
static phStatus_t phhalCt_ProcessTA(uint8_t bTAbyte, uint8_t bATRSwitchCount,
                                    phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function parses the ATR bytes of interface characters TB1, TB2, TB3 and so on.
 */
static phStatus_t phhalCt_ProcessTB(uint8_t bTBbyte,uint8_t bATRSwitchCount, phhalCt_AtrParameterType_t  *psAtrParams);

/**
 * This function parses the ATR bytes of interface characters TC1, TC2, TC3 and so on.
 */
static phStatus_t phhalCt_ProcessTC(uint8_t bTCbyte, uint8_t bATRSwitchCount, phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function parses the ATR bytes of interface characters TD1, TD2, TD3 and so on.
 */
static phStatus_t phhalCt_ProcessTD(uint8_t bTDbyte, uint8_t bATRSwitchCount, phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function checks the Lrc and the strcuture of the ATR is correct or not.
 */
static phStatus_t phhalCt_ProcessLrc( uint8_t bHistoBytes, phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function is used to handle TA2 byte during ATR parser.
 */
static void phhalCt_ProcessTA2(uint8_t bTAbyte, phhalCt_AtrParameterType_t *psAtrParams);
/**
 * This function is used to handle TB3 byte Error scenario (specifies character and block waiting time for T=1 protocol)
 */
static void phhalCt_HandleWaitingByteError(uint8_t bTBbyte ,phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function is used to handle absent character during ATR .
 */
static phStatus_t phhalCt_HandleAbsentChars(uint8_t bATRSwitchCount, uint8_t bCharacters, uint8_t bHistoBytes,
                                            phhalCt_AtrParameterType_t  *psAtrParams);
/**
 * This function is used to validate the received TCK character in the ATR.
 */
static uint8_t phhalCt_CheckLRC(void);

/**
 * This function is used to perform the Firmware Deactivation procedure
 */
static phStatus_t phhalCt_CardDeactivateConfig(void);

/**
 * This table is used in the ATR parser. This contains the 4 function pointers each for different interface byte
 * and respective interface byte MASK value
 */
static const
phhalCt_AtrType TypeABCD_Table[]={  {&phhalCt_ProcessTA,0x10},   /**< Process TA. */
                                    {&phhalCt_ProcessTB,0x20},   /**< Process TB. */
                                    {&phhalCt_ProcessTC,0x40},   /**< Process TC. */
                                    {&phhalCt_ProcessTD,0x80}};  /**< Process TD. */

typedef phStatus_t (phRomhalCt_ColdActivateConfig_t)(uint8_t bVccSel,uint8_t bEmvEn);
typedef phStatus_t (phRomhalCt_SetActivationConfig_t)(uint8_t bEmvEn, uint8_t bWarmResetState);

#define phRomhalCt_ColdActivateConfig(bVccSel, bEmvEn)     \
    ((phRomhalCt_ColdActivateConfig_t *) PH_HAL_CT_COLDACTIVATECONFIG_ADDR)(bVccSel, bEmvEn)

#define phRomhalCt_SetActivationConfig(bEmvEn, bWarmResetState)     \
    ((phRomhalCt_SetActivationConfig_t *) PH_HAL_CT_SETACTIVATIONCONFIG_ADDR)(bEmvEn, bWarmResetState)

/* Artf189696: Deactivation sequence is not performed completely if card presence is not sensed. This ROM3 API will
 * no longer be used. phhalCt_CardDeactivateConfig will be used instead. */
#if 0
typedef phStatus_t (phRomhalCt_CardDeactivateConfig_t)(void);
#define phRomhalCt_CardDeactivateConfig()  ((phRomhalCt_CardDeactivateConfig_t *) PH_HAL_CT_CARDDEACTIVATECONFIG_ADDR)()
#endif
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalCt_CardActivate(uint8_t bVccSel, uint8_t* pbAtrBuffer, uint8_t* pbAtrSize,
                                phhalCt_ProtocolParams_t  *psAtrPalParameters)
{
    phhalCt_AtrParameterType_t sAtrParams;
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;

    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers.*/
        if ((pbAtrBuffer == NULL) || (pbAtrSize == NULL) || (psAtrPalParameters == NULL))
        {
            break;
        }
        /* Vcc select should be 1.8V, 3V or 5V as per the input.*/
        if( (PHHAL_CT_VCC1M8 != bVccSel) &&
            (PHHAL_CT_VCC3   != bVccSel) &&
            (PHHAL_CT_VCC5   != bVccSel) )
        {
            break;
        }
#endif

        /* Consume pending Card Removal events, if any(artf187980) */
        (void)phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_CARD_REMOVED);

        /* Check if the card is present */
        eStatus = phhalCt_CheckCardPres();
        if(gphhalCt_ESlotType == E_AUXILIARY_SLOT)
        {
            /* To check if Card in auxiliary slot is present and not active .*/
        }
        else
        {
            if(eStatus != PH_ERR(CT_MAIN_CARD_PRESENT,HAL_CT))
            {
                break;
            }
            /* Check if the card is already activated */
            if (PH_REG_TEST_BIT(CT_PCR_REG, START))
            {
                eStatus = PH_ERR_CT_CARD_ALREADY_ACTIVATED;
                break;
            }
        }

        sAtrParams.psAtrPalParams = psAtrPalParameters;

        /* Clear global variables and Set default values before receiving Warm atr */
        phhalCt_ClearGLobalVar();
        (void)phhalCt_SetDefaultValues(&sAtrParams);
        /* Set the activation state */
        gphhalCt_BActivationState = TRUE;
        /* Clear the global count of ATR bytes in ISR */
        gphhalCt_WDataCount = 0x00;
        /* artf555048: Explicitly Clear PEC to ensure that Parity error will be detected during ATR */
        PH_REG_SET_FIELD(CT_FCR_REG ,PEC2_PEC0,PHHAL_CT_RESET_PARITY_ERR_COUNT);
        /* Clear All Events */
        (void) phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL));
        gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

        /* Call the ROM3 Api */
        eStatus = phRomhalCt_ColdActivateConfig(bVccSel, gphhalCt_BEmvEn);
        PH_BREAK_ON_FAILURE(eStatus);

        /* Wait for ATR bytes and process */
        eStatus = phhalCt_ProcessActivation(pbAtrBuffer,pbAtrSize,&sAtrParams);

        if(eStatus == PH_ERR_CT_ATR_WARM_RESET_INDICATED)
        {
            /* In EMVCo if warm ATR required is indicated in the cold ATR bytes,
             * start warm activation and wait for the bytes */
            if(gphhalCt_BEmvEn)
            {
                /* In case of warm ATR issue request from the parser, call the warm ATR from here.*/
                eStatus = phhalCt_WarmReset(pbAtrBuffer,pbAtrSize,psAtrPalParameters);
            }
        }
    }while (0);

    /* If the ATR is not returned with success then clear the return structure parameter */
    if((eStatus != PH_ERR_SUCCESS) &&
       (eStatus != PH_ERR_CT_CLASS_CHANGE_INDICATED) &&
       (eStatus != PH_ERR_CT_EARLY_ERROR))
    {
        if(psAtrPalParameters != NULL)
            phUser_MemSet(psAtrPalParameters,0x00,sizeof(phhalCt_ProtocolParams_t));
    }
    /* Clear all Events */
    (void) phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL));
    gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

    /* Clear all the global variables */
    phhalCt_ClearGLobalVar();
    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}


phStatus_t phhalCt_WarmReset(uint8_t *pbAtrBuffer, uint8_t *pbAtrSize, phhalCt_ProtocolParams_t  *psAtrPalParameters)
{
    phhalCt_AtrParameterType_t  sAtrParams;
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the null pointers */
        if ((pbAtrBuffer == NULL) || (pbAtrSize == NULL) || (psAtrPalParameters == NULL))
        {
            break;
        }
#endif
        /* Consume pending Card Removal events, if any */
        (void)phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_CARD_REMOVED);

        /* Check if the card is present */
        eStatus = phhalCt_CheckCardPres();
        if(gphhalCt_ESlotType == E_AUXILIARY_SLOT)
        {
            /* To check if Card in auxiliary slot is present and not active .*/
        }
        else
        {
            if(eStatus != PH_ERR(CT_MAIN_CARD_PRESENT,HAL_CT))
            {
                break;
            }
        }

        /* Flush the FIFO */
        PH_REG_SET_BIT(CT_UCR2x_REG,FIFO_FLUSH);

        sAtrParams.psAtrPalParams = psAtrPalParameters;

        /* Clear global variables and Set default values before receiving Warm atr */
        phhalCt_ClearGLobalVar();
        (void)phhalCt_SetDefaultValues(&sAtrParams);

        /* Set the warm reset flag, activation flag */
        sAtrParams.bWarmResetState  = 0x01;
        /* Set the activation state */
        gphhalCt_BActivationState = TRUE;
        /* Clear the global count of ATR bytes in ISR */
        gphhalCt_WDataCount = 0x00;

        /* Clear All Events */
        (void) phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL));
        gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

        /* Call the ROM3 Api */
        eStatus = phRomhalCt_SetActivationConfig(gphhalCt_BEmvEn, sAtrParams.bWarmResetState);
        PH_BREAK_ON_FAILURE(eStatus);

        /* Wait for ATR bytes and process */
        eStatus = phhalCt_ProcessActivation(pbAtrBuffer,pbAtrSize,&sAtrParams);

    }while (0);
    /* Clear all Events */
   (void) phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL));
   gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;

   /* Clear all global variables */
   phhalCt_ClearGLobalVar();
   return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}


phStatus_t phhalCt_CardDeactivate(void)
{
    if(gphhalCt_ESlotType == E_AUXILIARY_SLOT)
    {

    }
    else
    {
        phhalCt_CardDeactivateConfig();

        /* Enter Critical section */
        __phUser_EnterCriticalSection();

        /* Set the context variable for ISR */
        gphhalCt_BDisableDCDC = TRUE;

        /* Exit Critical section */
        __phUser_ExitCriticalSection();

        /* Start the timer in the  software triggered mode */
        phhalCt_SetTimer(PHHAL_CT_SW_TRIGGER, PHHAL_CT_DCDC_WAIT_TIME);

    }

    /* Clear all the global variables */
    phhalCt_ClearGLobalVar();

    return PH_ERR_SUCCESS;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
/**
 *Function Name     : phhalCt_CardDeactivateConfig
 *Description       : This function is used to perform the Firmware Deactivation procedure
 *                    This function replaces phRomhalCt_CardDeactivateConfig (See Artf189696)
 *Output Parameters : PH_ERR_SUCCESS
 */
static phStatus_t phhalCt_CardDeactivateConfig(void)
{
    /* Clear the start bit to start a de-activation sequence */
    PH_REG_CLEAR_BIT(CT_PCR_REG, START);

    /* De-configure the Clock Configuration for the sequencer */
    PH_REG_SET_BIT(PCR_CLK_CFG2_REG, CTSEQ_CLKSEL);

    /* Flush the FIFO */
    PH_REG_SET_BIT(CT_UCR2x_REG,FIFO_FLUSH);

    /* Firmware workaround for the hardware bug */
    PH_REG_RESET(CT_UCR1x_REG);
    PH_REG_RESET(CT_UCR2x_REG);

    return PH_ERR_SUCCESS;
}

/**
 *Function Name    : phhalCt_WaitForAtrBytes
 *Description     :  This Api is used to wait receive,timer elapse,hardware fault related events for atr bytes reception
 *
 *Input Parameters  :  bFirstByte - Set to logic 1 if it is first byte of atr,Mute event can only come for first byte.
 *                                  Set to logic 0 if it is first byte of atr.
 *
 *Input Parameters  :  psAtrParams - Pointer to Atr Parameter Structure.
 *
 *Output Parameters : PH_ERR_SUCCESS - If Atr byte has received in fifo and Receive event hass received successfully.
 *
 */
static phStatus_t phhalCt_WaitForAtrBytes(uint8_t bFirstByte, phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the null pointers */
        if (psAtrParams == NULL)
        {
            break;
        }
#endif
        /* Wait for atr bytes
         * The RTOS wait timer is set to 10 seconds until then at least one byte should come or
         * any of these event bits should be set.
         */
        if(bFirstByte)
        {
            eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_RX | E_PH_HALCT_EVENT_TO1 |
                                            E_PH_HALCT_EVENT_TO3 | E_PH_HALCT_EVENT_MUTE | E_PHHAL_CT_ERROR_EVENTS),
                                            10000,&gphhalCt_InEvent,TRUE);
        }
        else
        {
            eStatus = phhalCt_Event_WaitAny((phhalCt_EventType_t)(E_PH_HALCT_EVENT_RX | E_PH_HALCT_EVENT_TO1 |
                                            E_PH_HALCT_EVENT_TO3 | E_PHHAL_CT_ERROR_EVENTS),
                                            10000,&gphhalCt_InEvent,TRUE);
        }

        PH_BREAK_ON_FAILURE_WITH_ERROR(eStatus, OPERATION_TIMEDOUT);
        if(gphhalCt_InEvent & E_PH_HALCT_EVENT_RX )
        {
            if((gphhalCt_InEvent & E_PH_HALCT_EVENT_EARLY) == E_PH_HALCT_EVENT_EARLY)
            {
                psAtrParams->bEarlyEventFlag = 0x01;
            }
            (void) phhalCt_Event_Consume(E_PH_HALCT_EVENT_RX);
            gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_MUTE) == E_PH_HALCT_EVENT_MUTE)
        {
            eStatus = PH_ERR_CT_MUTE_ERROR;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_TO1) == E_PH_HALCT_EVENT_TO1)
        {
            eStatus = PH_ERR_CT_TIME_OUT_ATR_20160ETU;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_TO3) == E_PH_HALCT_EVENT_TO3)
        {
            eStatus = PH_ERR_CT_TIME_OUT_ATR_10080ETU;
            break;
        }
        else
        {
            /* Call the generic wait handler */
            eStatus = phhalCt_HandleCommonEvent();
            if(eStatus == PH_ERR_CT_PARITY_ERROR)
            {
                if(gphhalCt_BEmvEn)
                {
                    /* According to EMVCo we have to deactivate if parity error occurs in receiving ATR
                     * EMVCo specification section 8.3 */
                    (void)phhalCt_CardDeactivate();
                }
            }
        }
    }while(0);
    return eStatus;
}
/**
 *Function Name    : phhalCt_ProcessActivation
 *Description      : This is helper/common function to process the ATR after cold or warm activation..
 *
 *Input Parameters  :pbAtrBuffer - pointer to Atr buffer.
 *Input Parameters  :pbAtrSize - pointer to Atr buffer size.
 *Input Parameters  :psAtrParams - Pointer to Atr Parameter Structure.
 *
 *Output Parameters : PH_ERR_SUCCESS - If all Atr bytes are correct according to Specifications.
 *                    PH_ERR_CT_ATR_WARM_RESET_INDICATED -If  wrong cold atr bytes received in EMVCO profile,
 *                                                                                          then warm reset is needed.
 *                    PH_ERR_ATR_PARSER_ERROR - If in EMVCo profile wrong warm atr bytes are received  or
 *                                                                               in 7816-3 Wrong Atr bytes are received.
 */
static phStatus_t phhalCt_ProcessActivation(uint8_t *pbAtrBuffer, uint8_t *pbAtrSize,
                                            phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    do
    {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /* Check for the null pointers */
        if(psAtrParams == NULL)
        {
            break;
        }
#endif
        eStatus = phhalCt_AtrParser(psAtrParams);

        /* Stop all the timers */
        PHHAL_CT_STOP_TIMER;

        /* This wait is necessary. System is fast. */
        phUser_Wait(700);

        if(eStatus == PH_ERR_SUCCESS)
        {
            phUser_MemCpy((uint8_t *) pbAtrBuffer, (uint8_t *) gphhalCt_DriverBuff,(uint32_t)gphhalCt_WDataCount);
            *pbAtrSize = (uint8_t)gphhalCt_WDataCount;

            /* Set the baud rate */
            phhalCt_SetBaudRate(gphhalCt_BCurrentFiDi, psAtrParams->psAtrPalParams->bWI,
                                psAtrParams->psAtrPalParams->bBWI, psAtrParams->psAtrPalParams->bCWI);

            /* Set the guard time value */
            PH_REG_SET(CT_GTRx_REG, psAtrParams->bValueofNInTC1);

            /* Set the protocol */
            phhalCt_SetTransmissionProtocol(gphhalCt_BFirstOfferedProt);

            /* If the early event is set then indicate a warning to the user instead of success
             * and copy the ATR bytes  */
            if(psAtrParams->bEarlyEventFlag)
            {
                eStatus = PH_ERR_CT_EARLY_ERROR;
                break;
            }

            /* Indicate the user that TA is present with T=15, hence there is a chance of change of class */
            if(psAtrParams->bFlagT15TAPresent)
            {
                eStatus = PH_ERR_CT_CLASS_CHANGE_INDICATED;
                break;
            }
        }
        else if(eStatus == PH_ERR_CT_ATR_WARM_RESET_INDICATED)
        {
            break;
        }
        else
        {
            /* In EMVCo for any ATR parser error even in warm reset then do the de activation and
             * copy the faulty ATR bytes(if any) for indication */
            if(gphhalCt_BEmvEn)
            {
                if((eStatus == PH_ERR_CT_ATR_PARSER_ERROR) && (gphhalCt_WDataCount > 0))
                {
                    phUser_MemCpy((uint8_t *) pbAtrBuffer, (uint8_t *) gphhalCt_DriverBuff,(uint32_t)gphhalCt_WDataCount);
                    *pbAtrSize = (uint8_t)gphhalCt_WDataCount;
                }
                (void) phhalCt_CardDeactivate();
            }
        }
    }while(0);
    return eStatus;
}
/**
 *Function Name     : phhalCt_AtrParser
 *Description       : This Api is used to parse the received ATR from card, this is specific to EMVCo mode
 *
 *Input Parameters  :  psAtrParams - Pointer to Atr Parameter Structure.
 *
 *Output Parameters : PH_ERR_CT_PARITY_ERROR - If parity error is set during the ATR reception
 *Output              PH_ERR_CT_ATR_PARSER_ERROR - If wrong ATR bytes are received
 *Output              PH_ERR_SUCCESS - If ATR parsing is successful
 */
static phStatus_t phhalCt_AtrParser(phhalCt_AtrParameterType_t  *psAtrParams)
{
    uint8_t     bAtrIndex         = 0x00;
    uint8_t     bReadIfByte       = 0x00;
    uint8_t     bCharacters       = 0x00;
    uint8_t     bHistoBytes       = 0x00;
    uint8_t     bATRSwitchCount   = 0x00;
    phStatus_t  eStatus           = PH_ERR_CT_ATR_PARSER_ERROR;

    if(gphhalCt_BEmvEn)
    {
        /* Disabling parity error event for first byte as per EMVCo Compliance.*/
        PH_REG_SET_BIT(CT_UCR2x_REG,DISPE);
    }

    /* Receive the first TS byte */
    eStatus = phhalCt_WaitForAtrBytes(0x01, psAtrParams);
    if(PH_ERR_SUCCESS != eStatus)
    {
        /* Again resetting parity error event.*/
        PH_REG_CLEAR_BIT(CT_UCR2x_REG,DISPE);
        return eStatus;
    }
    /* Check for the TS character */
    if((gphhalCt_DriverBuff[bAtrIndex] != 0x3B) && (gphhalCt_DriverBuff[bAtrIndex] != 0x3F))
    {
        /* Again resetting parity error event. */
        PH_REG_CLEAR_BIT(CT_UCR2x_REG,DISPE);
        return PH_ERR_CT_ATR_PARSER_ERROR;
    }

    /* Receive the T0 character */
    eStatus = phhalCt_WaitForAtrBytes(0x00,psAtrParams);
    PH_RETURN_ON_FAILURE(eStatus);

    bAtrIndex++;
    bReadIfByte = gphhalCt_DriverBuff[bAtrIndex];
    bHistoBytes = bReadIfByte & PHHAL_CT_LSB_NIBBLE_MASK;
    /* for the characters 0 to 8 (bATRSwitchCount) */
    for(bATRSwitchCount=0; bATRSwitchCount<0x08; bATRSwitchCount++)
    {
        /* TA(bCharacters),TB(bCharacters),TC(bCharacters),TD(bCharacters) */
        for (bCharacters=0; bCharacters<0x04; bCharacters++)
        {
            /* Check for TA TB TC TD presence */
            if((TypeABCD_Table[bCharacters].T & bReadIfByte) != 0)
            {
                eStatus = phhalCt_WaitForAtrBytes(0x00 ,psAtrParams);
                PH_RETURN_ON_FAILURE(eStatus);

                bAtrIndex++;
                /* Process the recognized character */
                eStatus = TypeABCD_Table[bCharacters].InterfaceChars(gphhalCt_DriverBuff[bAtrIndex],
                                                                     bATRSwitchCount ,psAtrParams);
                /* Check character TD */
                if(bCharacters == PHHAL_CT_CHARACTER_TD)
                {
                    bReadIfByte = gphhalCt_DriverBuff[bAtrIndex];
                }
            }
            else
            {
                eStatus = phhalCt_HandleAbsentChars(bATRSwitchCount,bCharacters,bHistoBytes, psAtrParams);
                /* If no more interface characters present exit from the loop */
                if(bCharacters == PHHAL_CT_CHARACTER_TD)
                {
                    return eStatus;
                }
            }
        } /* End of inside for loop for TA,TB,TC,TD presence */
    }
    return PH_ERR_SUCCESS;
}

/**
 *Function Name     : phhalCt_ProcessTA
 *Description       : This Api is used to process Interface character TA1,TA2,TA3,TA4 bytes for atr, and sets
 *                    psAtrParams->bInvalidAtr flag if wrong value comes for them.
 *
 *Input Parameters  : bTAbyte - value of received TA byte.
 *Input Parameters  : bATRSwitchCount - it is used to tell whether it is TA1 or TA2 or TA3.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *
 */
static phStatus_t phhalCt_ProcessTA(uint8_t bTAbyte, uint8_t bATRSwitchCount,phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    uint8_t bCount = 0x00;
    switch(bATRSwitchCount)
    {
        case 0:
            /* PROCESS TA1 */
            /*
             *  If TA1 is absent from the ATR, the default values of D = 1 and F = 372 shall be used during all
             *  subsequent exchanges.
             */
            if(gphhalCt_BEmvEn)
            {
                /* The decision of whether ATR must be rejected, based on invalid TA1, should be taken
                 * when checking byte TA2
                 * For now simply save the TA1byte in global variable */
                if((bTAbyte > PHHAL_CT_EMVCO_FIDI_MAX_VAL) ||
                   (bTAbyte < PHHAL_CT_EMVCO_FIDI_MIN_VAL))
                {
                    gphhalCt_BFiDi = bTAbyte;
                    break;
                }
            }
            else
            {
                /* Search in the prescalar table for the incoming TA1 byte */
                for(bCount=0x00; bCount<PHHAL_CT_FIDI_TAB_LEN;bCount++)
                {
                    if(bTAbyte == gkphhalCt_BPreScalar[bCount].bFiDi)
                    {
                        break;
                    }
                }
                if((bCount == PHHAL_CT_FIDI_TAB_LEN))
                {
                    /* In 7816 TA1 value decision  will depend upon TA2 byte.*/
                    psAtrParams->bInvalidTA1 = 0x01;
                    break;
                }
            }
            /* Everything is OK update the global values */
            gphhalCt_BFiDi = bTAbyte;
            gphhalCt_BCurrentFiDi = bTAbyte;
            psAtrParams->psAtrPalParams->bFiDi = bTAbyte;
        break;
        case 1:
            /* PROCESS TA2 */
            phhalCt_ProcessTA2(bTAbyte,psAtrParams);
        break;
        case 2:
            /* PROCESS TA3 */
            if(gphhalCt_BEmvEn)
            {
                if( (psAtrParams->psAtrPalParams->bProtSelT1))
                {
                    /* Only if T=1 protocol is offered then only treat TA3 as IFSC.*/
                    if((bTAbyte > PHHAL_CT_MAX_IFSC) ||(bTAbyte < PHHAL_CT_EMVCO_MIN_IFSC))
                    {
                        psAtrParams->bInvalidAtr = 0x01;
                        break;
                    }
                    psAtrParams->psAtrPalParams->bIFSC = bTAbyte;
                }
            }
            else
            {
                /* Check for flag 15 presence, as it is necessary because in that case this byte will not be IFSC.*/
                if(psAtrParams->bFlagT15)
                {
                    psAtrParams->bFlagT15TAPresent = 0x01;
                    /* First TA for flag 15 will tell for class and clock stop config ,not IFSC .*/
                    psAtrParams->psAtrPalParams->bFlagT15TAValue = bTAbyte;
                    break;
                }
                else
                {
                    /* Only if T=1 protocol is offered then only treat TA3 as IFSC.*/
                    if( (psAtrParams->psAtrPalParams->bProtSelT1))
                    {
                        if((bTAbyte > PHHAL_CT_MAX_IFSC) ||(bTAbyte < PHHAL_CT_7816_MIN_IFSC))
                        {
                            psAtrParams->bInvalidAtr = 0x01;
                            break;
                        }
                        psAtrParams->psAtrPalParams->bIFSC = bTAbyte;
                    }
                }
            }

        break;
        default:
            /* PROCESS TA4 */
            /* For EMVCo profile flag 15 we are not setting and it should execute the following code */
            if((psAtrParams->bFlagT15) && (!(psAtrParams->bFlagT15TAPresent)))
            {
                psAtrParams->bFlagT15TAPresent = 0x01;
                psAtrParams->psAtrPalParams->bFlagT15TAValue = bTAbyte;
            }

       break;
    }
    return eStatus;
}
/**
 *Function Name     : phhalCt_ProcessTA2
 *Description       : This Api is used to process Interface character TA2.
 *
 *Input Parameters  : bTAbyte - Value of received TA2 byte.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *Output Parameters : None
 */
static void phhalCt_ProcessTA2(uint8_t bTAbyte, phhalCt_AtrParameterType_t  *psAtrParams)
{
    if(gphhalCt_BEmvEn)
    {
       /* For EMVCo TA2, offered protocol should be equal to first offered protocol by TD1 */
       /* 5th bit must be zero if it is present.
        * EMVCo specification 8.3.3.5 TA2 */
       if(((bTAbyte & PHHAL_CT_LSB_NIBBLE_MASK) != gphhalCt_BFirstOfferedProt) ||
          ((bTAbyte & PHHAL_CT_BIT5_MASK)       != 0x00))
       {
           psAtrParams->bInvalidAtr = 0x01;
       }

       /* If TA2 is present and Bit 5 is zero, TA1 byte is allowed to have only '11' to '13' */
       if((gphhalCt_BFiDi > PHHAL_CT_EMVCO_FIDI_MAX_VAL) ||
          (gphhalCt_BFiDi < PHHAL_CT_EMVCO_FIDI_MIN_VAL))
       {
           psAtrParams->bInvalidAtr = 0x01;
       }
    }
    else
    {

       /* AS per 7816 standard.*/
       if((  (bTAbyte & PHHAL_CT_LSB_NIBBLE_MASK) > PHHAL_CT_PROTOCOL_T1) ||
          ( ((bTAbyte & PHHAL_CT_BIT5_MASK) == 0x00) && (psAtrParams->bInvalidTA1) )
         )
       {
           if((bTAbyte & PHHAL_CT_LSB_NIBBLE_MASK) > PHHAL_CT_PROTOCOL_T1)
           {
               psAtrParams->bInvalidTD1 = 0x01;
           }
           /*If bit 8 is set to 1 then go for deactivation otherwise for warm reset.*/
           /* But if after warm reset same atr comes then will do deactivation.*/
           if((bTAbyte & PHHAL_CT_BIT8_MASK)||(psAtrParams->bWarmResetState))
           {
               /*
                *  just to differentiate between deactivation and warm reset in end for unsupported protocol
                *  or baud rate,this parameter is set to 0xFF.
                */
               psAtrParams->psAtrPalParams->bNegotiableMode = 0xFF;
           }
           psAtrParams->bInvalidAtr = 0x01;
       }
       else
       {
           /* If bit 5 is 1 then ignore TA1 byte offered baud rate and work with default baud rate.*/
           if((bTAbyte & PHHAL_CT_BIT5_MASK))
           {
               gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;
               psAtrParams->psAtrPalParams->bFiDi = PHHAL_CT_DEFAULT_FIDI;
               psAtrParams->bInvalidTA1 = 0x00;
           }
           /* Ignore TD1 byte offered protocol and work according to specified protocol by TA2
            * So ignore the flag set by Unsupported TD1 or Unsupported TA1.*/
           psAtrParams->bInvalidTD1  = 0x00;
           gphhalCt_BFirstOfferedProt = (bTAbyte & PHHAL_CT_LSB_NIBBLE_MASK);
           if(gphhalCt_BFirstOfferedProt == PHHAL_CT_PROTOCOL_T1)
           {
               psAtrParams->psAtrPalParams->bProtSelT1 = 0x01;
               psAtrParams->psAtrPalParams->bProtSelT0 = 0x00;
               psAtrParams->bTCKByte = 0x01;
           }
           else
           {
               psAtrParams->psAtrPalParams->bProtSelT1 = 0x00;
               psAtrParams->psAtrPalParams->bProtSelT0 = 0x01;
               psAtrParams->bTCKByte = 0x00;
           }
       }
    }
}
/**
 *Function Name     : phhalCt_ProcessTB
 *Description       : This Api is used to process Interface character TB1,TB2,TB3,TB4 bytes for atr, and sets
 *                    psAtrParams->bInvalidAtr flag if wrong value comes for them.
 *
 *Input Parameters  : bTBbyte - value of received TB byte.
 *Input Parameters  : bATRSwitchCount - it is used to tell whether it is TB1 or TB2 or TB3.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *
 */
static phStatus_t phhalCt_ProcessTB(uint8_t bTBbyte, uint8_t bATRSwitchCount,
                                    phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    switch(bATRSwitchCount)
    {
        case 0:
            /* PROCESS TB1 */
            /* Reject the cold ATR if TB1 byte is a non zero value
             * else in warm ATR accept any value (EMVCo only) */
            if(gphhalCt_BEmvEn)
            {
                if ((bTBbyte != 0x00) && (psAtrParams->bWarmResetState == 0x00))
                {
                    psAtrParams->bInvalidAtr = 0x01;
                }
            }
        break;
        case 1:
            /* PROCESS TB2: In ATR TB2 byte should never be present (EMVCo only) */
            if(gphhalCt_BEmvEn)
            {
                psAtrParams->bInvalidAtr = 0x01;
            }
        break;
        case 2:
        {
            /* PROCESS TB3 */
            /* If T=1 protocol is indicated and if  T=15 protocol is not indicated then only
             * TB3 byte is considered to have CWI or BWI. */
            if((psAtrParams->psAtrPalParams->bProtSelT1) && (!(psAtrParams->bFlagT15)))
            {
                phhalCt_HandleWaitingByteError(bTBbyte,psAtrParams);
                if(gphhalCt_BEmvEn)
                {
                    if((psAtrParams->psAtrPalParams->bBWI > PHHAL_CT_EMVCO_BWI_MAX) ||
                       (psAtrParams->psAtrPalParams->bCWI > PHHAL_CT_EMVCO_CWI_MAX))
                    {
                        psAtrParams->bInvalidAtr = 0x01;
                    }
                }
                else
                {
                    if(psAtrParams->psAtrPalParams->bBWI > PHHAL_CT_7816_BWI_MAX)
                    {
                        psAtrParams->bInvalidAtr = 0x01;
                    }
                }
            }
        }
        break;
        default:
            /* PROCESS TB4 : Do nothing */
        break;
    }

    return eStatus;
}
/**
 *Function Name     : phhalCt_ProcessTC
 *Description       : This Api is used to process Interface character TC1,TC2,TC3,TC4 bytes for atr, and sets
 *                    psAtrParams->bInvalidAtr flag for their wrong values.
 *
 *Input Parameters  : bTCbyte - value of received TC byte.
 *Input Parameters  : bATRSwitchCount - it is used to tell whether it is TC1 or TC2 or TC3.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *
 */
static phStatus_t phhalCt_ProcessTC(uint8_t bTCbyte, uint8_t bATRSwitchCount,
                                    phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    switch(bATRSwitchCount)
    {
        case 0:
            /* PROCESS TC1 : Do nothing just update the value so that in TB3 processing this value is required */
            psAtrParams->bValueofNInTC1 = bTCbyte;
        break;

        case 1:
            /* PROCESS TC2 */
            /* Reject an ATR containing TC2 having any value other than 0x0A */
            /* EMVCo specification 8.3.3.7 */
            if (gphhalCt_BEmvEn)
            {
                if(bTCbyte != PHHAL_CT_EMVCO_SUPPORTED_TC2)
                {
                    /* Set the error */
                    psAtrParams->bInvalidAtr = 0x01;
                    break;
                }
            }
            else /* Reject an ATR containing TC2=0x00 value in ISO7816 */
            {
                if(bTCbyte == PHHAL_CT_UNSUPPORTED_TC2)
                {
                    /* Set the error */
                    psAtrParams->bInvalidAtr = 0x01;
                    break;
                }
            }
            /* Everything is OK . Apply the TC2 value as WI index*/
            psAtrParams->psAtrPalParams->bWI = bTCbyte;
        break;

        case 2:
            /* Process TC3 only if T=1 protocol is supported */

            if(psAtrParams->psAtrPalParams->bProtSelT1)
            {
                /* According to EMVCo TC3 MUST be = 0x00 which indicates only LRC computation is allowed
                 * EMVCo specification 8.3.3.11 TC3 */
                if (gphhalCt_BEmvEn)
                {
                    if(bTCbyte != PHHAL_CT_EMVCO_SUPPORTED_TC3)
                    {
                        /* Set the error */
                        psAtrParams->bInvalidAtr = 0x01;
                    }
                }
                else /* ISO7816 */
                {
                    if(bTCbyte & PHHAL_CT_CRC_PRESENCE_CHECK_MASK)
                    {
                        /* TC3 byte 0th bit will decide whether LRC or CRC will be used for further communication.*/
                        psAtrParams->psAtrPalParams->bCRCPresent = 0x01;
                    }
                }
            }
        break;
        default :
            /* PROCESS TC4 : Do nothing */
        break;
    }
    return eStatus;
}
/**
 *Function Name     : phhalCt_ProcessTD
 *Description       : This Api is used to process Interface character TD1,TD2,TD3,TD4 bytes for atr, and sets
 *                    psAtrParams->bInvalidAtr flag for their wrong values.
 *
 *Input Parameters  : bTDbyte - value of received TD byte.
 *Input Parameters  : bATRSwitchCount - it is used to tell whether it is TD1 or TD2 or TD3.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *
 */
static phStatus_t phhalCt_ProcessTD(uint8_t bTDbyte, uint8_t bATRSwitchCount,
                                    phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    bTDbyte &= PHHAL_CT_LSB_NIBBLE_MASK;
    switch(bATRSwitchCount)
    {
        case 0:
            /* PROCESS TD1 */
            /* The terminal shall accept an ATR containing TD1 with the m.s. nibble having any value */
            if (bTDbyte > PHHAL_CT_PROTOCOL_T1)
            {
                /* In EMVCo other protocols are not allowed except T=0 or T=1 */
                if(gphhalCt_BEmvEn)
                {
                    psAtrParams->bInvalidAtr = 0x01;
                    break;
                }
                else
                {
                    /* In ISO7816 decision will be based on TA2 byte.*/
                    psAtrParams->bInvalidTD1 = 0x01;
                }
            }
            gphhalCt_BFirstOfferedProt = bTDbyte;
            /* If the protocol is T=1 offered update the flags */
            if(bTDbyte == PHHAL_CT_PROTOCOL_T1)
            {
                psAtrParams->psAtrPalParams->bProtSelT1 = 0x01;
                psAtrParams->psAtrPalParams->bProtSelT0 = 0x00;
                psAtrParams->bTCKByte = 0x01;
            }
        break;
        case 1:
            /* PROCESS TD2 */
            /* If the offered protocol is in descending order then reject that ATR (Both EMVCo and ISO7816) */
            if((gphhalCt_BFirstOfferedProt > bTDbyte))
            {
                psAtrParams->bInvalidAtr = 0x01;
                break;
            }
            /* EMVCo specification 8.3.3.8 TD2 */
            if(gphhalCt_BEmvEn)
            {
                /* If the offered protocol neither T=1 nor T=14 then reject the ATR */
               if((bTDbyte != PHHAL_CT_PROTOCOL_T1) && (bTDbyte != PHHAL_CT_PROTOCOL_T14))
               {
                   psAtrParams->bInvalidAtr = 0x01;
                   /* To handle 7816 (Non EMVCo) card for T=0 and T=15 combination,so Atr parser can handle TCK
                    * and return correct length for Atr.*/
                    if(bTDbyte == PHHAL_CT_PROTOCOL_T15)
                    {
                        psAtrParams->bTCKByte = 0x01;
                    }
                    break;
               }
               else if(bTDbyte == PHHAL_CT_PROTOCOL_T1)
               {
                   psAtrParams->psAtrPalParams->bProtSelT1 = 0x01;
                   psAtrParams->bTCKByte = 0x01;
               }
               else
               {
                   /* This case is when T=14 is offered.
                    * And If the first offered protocol is T=1 then this combination is not allowed, reject the ATR */
                   if(gphhalCt_BFirstOfferedProt == PHHAL_CT_PROTOCOL_T1)
                   {
                       psAtrParams->bInvalidAtr = 0x01;
                       break;
                   }
                   else
                   {
                       /* Update the TCK byte it can be present as T=0 and T=14 combination has TCK presence. */
                       psAtrParams->bTCKByte = 0x01;
                   }
               }
            }
            else
            {
                /* If nonzero protocol comes,then update TCK byte flag.*/
                if(bTDbyte >= PHHAL_CT_PROTOCOL_T1)
                {
                    psAtrParams->bTCKByte = 0x01;
                    /* Set protocol 15 flag,if protocol 15 is offered.*/
                    if(bTDbyte == PHHAL_CT_PROTOCOL_T1)
                    {
                        psAtrParams->psAtrPalParams->bProtSelT1 = 0x01;
                    }
                    else
                    {
                         if(bTDbyte == PHHAL_CT_PROTOCOL_T15)
                         {
                             psAtrParams->bFlagT15 = 0x01;
                         }
                    }
                }
            }
            psAtrParams->bLastOfferedProt = bTDbyte;
        break;
        default:
            /* PROCESS TD3 */
            /* Offered protocol are not allowed in descending order.*/
            if(psAtrParams->bLastOfferedProt > bTDbyte)
            {
                psAtrParams->bInvalidAtr = 0x01;
                break;
            }
            else
            {
                /* If offered protocol is T=1.*/
                if(bTDbyte == PHHAL_CT_PROTOCOL_T1)
                {
                    psAtrParams->psAtrPalParams->bProtSelT1 = 0x01;
                    psAtrParams->bTCKByte = 0x01;
                }
                else
                {
                    if ((!(gphhalCt_BEmvEn)))
                    {
                        /* If offered protocol has nonzero value then set flag for TCK.*/
                        if(bTDbyte > 0x00)
                        {
                            if(bTDbyte == PHHAL_CT_PROTOCOL_T15)
                            {
                                /* Set flag 15,if protocol T = 15 present.*/
                                psAtrParams->bFlagT15 = 0x01;
                            }
                            psAtrParams->bTCKByte = 0x01;
                        }
                    }
                    else
                    {
                        /* For T=0 and T= 15/T=14 combination,TCK byte will be present.*/
                        if((bTDbyte == PHHAL_CT_PROTOCOL_T15)||(bTDbyte == PHHAL_CT_PROTOCOL_T14))
                        {
                            psAtrParams->bTCKByte = 0x01;
                        }
                    }
                }
                psAtrParams->bLastOfferedProt = bTDbyte;
            }

        break;
    }

    return eStatus;
}
/**
 *Function Name     : phhalCt_HandleAbsentChars
 *Description       : This Api is used to handle the absent characters in the ATR.
 *
 *Input Parameters  : bATRSwitchCount - Indicator for which interface characters we are processing.
 *                                      bATRSwitchCount = 0x00 -> T0 characters
 *                                      bATRSwitchCount = 0x01 -> TD1 characters
 *                                      bATRSwitchCount = 0x02 -> TD2 characters
 *                                      bATRSwitchCount = 0x03 -> TD3 characters
 *Input Parameters  : bCharacters - Specifically tells which characters TA, TB or TC
 *Input Parameters  : bHistoBytes - Number of historical characters present
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *
 *Output Parameters :  eStatus
 */
static phStatus_t phhalCt_HandleAbsentChars(uint8_t bATRSwitchCount, uint8_t bCharacters, uint8_t bHistoBytes,
                                            phhalCt_AtrParameterType_t  *psAtrParams)
{
    phStatus_t  eStatus = PH_ERR_SUCCESS;

    if(gphhalCt_BEmvEn)
    {
        /* If the TA2 byte is absent, no negotiation can be done, hence put the default value,
         * since we can't use the TA1 value as Fi and Di */
        if((bATRSwitchCount == 0x01) && (bCharacters == PHHAL_CT_CHARACTER_TA))
        {
            gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;
            psAtrParams->psAtrPalParams->bFiDi = PHHAL_CT_DEFAULT_FIDI;
            gphhalCt_BFiDi = PHHAL_CT_DEFAULT_FIDI;
        }
        /* TB character absence check */
        else if(bCharacters == PHHAL_CT_CHARACTER_TB)
        {
            /* Check for TB1 presence, this is mandatory in EMVCo specification */
            if((bATRSwitchCount == 0x00) && (psAtrParams->bWarmResetState == 0x00))
            {
                psAtrParams->bInvalidAtr = 0x01;
            }
            else
            {
                /* Check for TB3 presence it should be mandatory if T=1 protocol used */
                if((bATRSwitchCount == 0x02) && (psAtrParams->psAtrPalParams->bProtSelT1 == 0x01))
                {
                    psAtrParams->bInvalidAtr = 0x01;
                }
            }
        }
        else /* Check for the TD character absence check */
        {
            if(bCharacters == PHHAL_CT_CHARACTER_TD)
            {
                /* If TD1 is absent then TA2 will also be absent and it will work on default baud rate.*/
                if(bATRSwitchCount == 0x00)
                {
                    gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;
                    psAtrParams->psAtrPalParams->bFiDi = PHHAL_CT_DEFAULT_FIDI;
                }
                else if (bATRSwitchCount == 0x01)
                {
                    /* TD2 character presence is mandatory if the first offered protcol is T=1 */
                    if(gphhalCt_BFirstOfferedProt == 0x01)
                    {
                        psAtrParams->bInvalidAtr = 0x01;
                    }
                }
                else
                {
                    /* To avoid QA warnings.*/
                }
            }
        }
    }
    else /* ISO 7816 cases where the absent characters are handled */
    {
        /* If TA2 absent in 7816 ,negotiable mode is set to 1.*/
        if((bATRSwitchCount == 0x01) && (bCharacters == PHHAL_CT_CHARACTER_TA))
        {
            psAtrParams->psAtrPalParams->bNegotiableMode = 0x01;
            /* For this case when TA1 or TD1 has invalid values and TA2 absent, need to return ATR parser error.*/
            if((psAtrParams->bInvalidTA1)||(psAtrParams->bInvalidTD1))
            {
                psAtrParams->bInvalidTA1 = 0x00;
                psAtrParams->bInvalidTD1 = 0x00;
                psAtrParams->bInvalidAtr = 0x01;
            }
            gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;
        }
        /* TD1 itself is absent then TA2 will also absent  */
        else if((bATRSwitchCount == 0x00) && (bCharacters == PHHAL_CT_CHARACTER_TD))
        {
            psAtrParams->psAtrPalParams->bNegotiableMode = 0x01;
            /* For this case when TA1 has invalid values and TA2 absent, need to return ATR parser error.*/
            if(psAtrParams->bInvalidTA1)
            {
                psAtrParams->bInvalidTA1 = 0x00;
                psAtrParams->bInvalidAtr = 0x01;
            }
            /*
             * The case where even the TD1 is absent, indicates no TA2 byte.
             * In this case put the FiDi value to the default.
             */
            gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;
        }
        else
        {
            /* Just for QA */
        }
    }
    /* Check for TD interface byte.*/
    if(bCharacters == PHHAL_CT_CHARACTER_TD)
    {
        eStatus = phhalCt_ProcessLrc(bHistoBytes,psAtrParams);
    }

    return eStatus;
}
/**
 *Function Name     : phhalCt_ProcessLrc
 *Description       : This Api is used to receive and process historical bytes, LRC byte.
 *
 *Input Parameters  : bHistoBytes -  Number of historical bytes,fetched from T0 byte.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *Output Parameters : PH_ERR_SUCCESS - If everything is OK and no error flags are set.
 *
 */
static phStatus_t phhalCt_ProcessLrc(uint8_t bHistoBytes,phhalCt_AtrParameterType_t *psAtrParams)
{
    phStatus_t eStatus   = PH_ERR_CT_ATR_PARSER_ERROR;
    uint8_t bLrcReceived = 0x00;
    uint8_t bLoopCount   = 0x00;
    do
    {
        /* Receive all the historical bytes and TCK byte if present */
        for(bLoopCount=0x00;bLoopCount<(bHistoBytes + psAtrParams->bTCKByte);bLoopCount++)
        {
            eStatus = phhalCt_WaitForAtrBytes(0x00 ,psAtrParams);
            PH_RETURN_ON_FAILURE(eStatus);
        }

        /* Check LRC is correct if present */
        if (psAtrParams->bTCKByte == 0x01)
        {
            bLrcReceived = phhalCt_CheckLRC();
            if(bLrcReceived != 0x00)
            {
                /* In EMVCo,If TCK byte comes wrong then de activation is required without any warm reset.*/
                eStatus = PH_ERR_CT_ATR_PARSER_ERROR;
                break;
            }
        }

        /* Check if the ATR bytes are more than the maximum allowed */
        if(gphhalCt_BEmvEn)
        {
            if(gphhalCt_WDataCount > PHHAL_CT_EMVCO_MAX_ATR_BYTE)
            {
                psAtrParams->bInvalidAtr = 0x01;
            }
        }
        else
        {
            if(gphhalCt_WDataCount > PHHAL_CT_7816_MAX_ATR_BYTE)
            {
                psAtrParams->bInvalidAtr = 0x01;
            }
        }
        /* Handling of the Error scenarios if occurred during the ATR parsing by checking the following flag */
        if(psAtrParams->bInvalidAtr == 0x01)
        {
            eStatus = PH_ERR_CT_ATR_PARSER_ERROR;
            if(gphhalCt_BEmvEn)
            {
                if(psAtrParams->bWarmResetState == 0x00)
                {
                    eStatus = PH_ERR_CT_ATR_WARM_RESET_INDICATED;
                }
            }
            else
            {
                /* Specific error code is need to return  because as per spec deactivation or warm
                 * reset is must for these condition.*/
                if((psAtrParams->bInvalidTA1) || (psAtrParams->bInvalidTD1))
                {
                    /* Just to differentiate between return error code ,this flag was set as 0xFF.*/
                    if(psAtrParams->psAtrPalParams->bNegotiableMode == 0xFF)
                    {
                        eStatus = PH_ERR_CT_ATR_SPECIFIC_PARAMETER_UNSUPPORTED;
                    }
                    else
                    {
                        eStatus = PH_ERR_CT_ATR_WARM_RESET_INDICATED;
                    }
                }
            }
            break;
        }
        eStatus = PH_ERR_SUCCESS;
    }while(0);
    return eStatus;
}
/**
 *Function Name     : phhalCt_HandleWaitingByteError
 *Description       : This Api is used to verify the TB3 byte is valid.
 *
 *Input Parameters  : bTBbyte -  TB3 byte value which has BWI and CWI values.
 *Input Parameters  : psAtrParams - Pointer to Atr Parameter Structure.
 *Output Parameters : None.
 *
 */
static void phhalCt_HandleWaitingByteError(uint8_t bTBbyte, phhalCt_AtrParameterType_t *psAtrParams)
{
    uint8_t bTC1CheckVal = 0x00; /* Default check value */
    uint32_t dwCwiCheckVal = 0x01; /* Default check value */

    /* TB3 (if T=1 is indicated in TD2) indicates the values of the CWI and the BWI
       used to compute the CWT and BWT respectively */
    /* EMVCo specification 8.3.3.10 */
    /* L.S.B of TB3 byte indicates the CWI */
    psAtrParams->psAtrPalParams->bCWI = (uint8_t)(bTBbyte & PHHAL_CT_LSB_NIBBLE_MASK);

    /* M.S.B of TB3 byte indicates the BWI */
    psAtrParams->psAtrPalParams->bBWI = (uint8_t)(( bTBbyte >> 4 ) & PHHAL_CT_LSB_NIBBLE_MASK );

    /*
     * As CWT should be always greater than GT so (2^CWI)>(ExtraGuardtime+1) ,
     * extra guard time is offered by TC1 byte.
     */
    if(psAtrParams->psAtrPalParams->bCWI != PHHAL_CT_MIN_CWI)
    {
        dwCwiCheckVal = (psAtrParams->psAtrPalParams->bCWI -1);
        dwCwiCheckVal = (uint32_t)(2 <<dwCwiCheckVal);
    }

    if(psAtrParams->bValueofNInTC1 != PHHAL_CT_SPECIAL_TC1_VALUE)
    {
        bTC1CheckVal = psAtrParams->bValueofNInTC1+0x01;
    }

    /* Guard time always should be lesser than CWT for both profiles.*/
    if(dwCwiCheckVal <= bTC1CheckVal)
    {
        psAtrParams->bInvalidAtr = 0x01;
    }
}

/**
 *Function Name     : phhalCt_CheckLRC
 *Description       : This Api is used to calculate LRC byte for validating the TCK received during ATR reception
 *
 *Input Parameters  : None
 *
 *Output Parameters : bLrc - Returns the LRC calculated from the received ATR
 */
static uint8_t phhalCt_CheckLRC(void)
{
    uint8_t bByteCount = 0x00;
    uint8_t bLrc = 0x00;

    /* Calculate the LRC except the TS byte */
    for(bByteCount = 1; bByteCount < gphhalCt_WDataCount; bByteCount++)
    {
        bLrc ^= gphhalCt_DriverBuff[bByteCount];
    }
    return bLrc;
}

#endif /* NXPBUILD__PHHAL_CT */
