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
 * $Date: 2016-01-29 10:21:08 +0530 (Fri, 29 Jan 2016) $
 * $Author: Prasad Hegde (nxp72764) $
 * $Revision: 17615 $ (v04.13.00)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalCt.h"
#include "phhalCt_Int.h"
#include "ph_Status.h"
#include "phhalPmu.h"
#include "ph_Platform.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "phhalCt_Event.h"
#include "ph_Reg.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
/**
 *  Default value for IFSC.
 */
#define PHHAL_CT_DEFAULT_IFSC                   0x20
/**
 *  Default value for waiting integer.
 */
#define PHHAL_CT_7816_DEFAULT_BWI               0x04
/**
 *  Default value for waiting integer.
 */
#define PHHAL_CT_7816_DEFAULT_CWI               0x0D
/**
 *  Default value for waiting integer.
 */
#define PHHAL_CT_DEFAULT_WI                     0x0A
/**
 *  Default value for extra guard time.
 */
#define PHHAL_CT_DEFAULT_EXTRAGUARD_TIME        0x00

/** Macro for Default clock value 4.92MHz for activation.*/
#define PHHAL_CT_CARD_DEFAULT_CLOCK      0x05

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
 * This function calculates WWT,BWT and CWT.
 */
static void phhalCt_CalculateWWTBWTCWT(uint8_t bTableCount,uint8_t bWI,uint8_t bBWI ,uint8_t bCWI);

typedef phStatus_t (phRomhalCt_SetTimer_t)(uint32_t dwMode, uint32_t dwWTX, uint32_t dwBlockWaitingTime,
    uint32_t dwWaitingTime,uint8_t bEmvEn);

#define phRomhalCt_SetTimer(dwMode, dwWTX, dwBlockWaitingTime, dwWaitingTime, bEmvEn)    \
    ((phRomhalCt_SetTimer_t *) PH_HAL_CT_SETTIMER_ADDR)(dwMode, dwWTX, dwBlockWaitingTime, dwWaitingTime, bEmvEn)

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phhalCt_SetTimer(uint32_t dwMode, uint32_t dwWTX)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    if(dwMode == PHHAL_CT_PPSMODE_9600_ETU )
    {
        /*Stop the timers before loading the new values*/
        PH_REG_SET(CT_TOC_REG, (uint8_t)(0));

         /* Load the value of initial WWT value for PPS exchange.*/
        PH_REG_SET(CT_TOR2_REG, PHHAL_CT_LSB_ETU9600_VALUE);
        PH_REG_SET(CT_TOR3_REG, PHHAL_CT_MSB_ETU9600_VALUE);
        /* Timer 1 will remain stop, Timer 2 and 3 starts on every start bit on I/O line.*/
        PH_REG_SET(CT_TOC_REG, PHHAL_CT_T1STOP_T23STARTONIO_CONFIG);
    }
    else if(dwMode == PHHAL_CT_SW_TRIGGER)
    {
    	/* Load the default values of ETU */
        /* Set the card clock default value to 5 Mhz max */
        PH_REG_SET_FIELD(CT_CCRx_REG, ACC2_ACC0 ,PHHAL_CT_CARD_DEFAULT_CLOCK);
        /* The PDR default value 372 is multiplied by 8 will result in 0xBA0, Since we are dividing the Card clock
         * frequency by 8 [ACC2- ACC0 = clock/6], we have to multiply the default 372 etu value with 8 for sampling and
         * hence the value 0x8B8 is loaded as the default value in the PDR register */
        PH_REG_SET(CT_PDRx_LSB_REG, 0xB8);
        PH_REG_SET(CT_PDRx_MSB_REG, 0x08);

        /*Stop the timers before loading the new values*/
        PH_REG_SET(CT_TOC_REG, (uint8_t)(0));

        PH_REG_SET(CT_TOR2_REG, (uint8_t)(dwWTX ));
        PH_REG_SET(CT_TOR3_REG, (uint8_t)(dwWTX >> 8));
        /* Timer 1 will remain stop, Timer 2 and 3 starts now */
        PH_REG_SET(CT_TOC_REG, PHHAL_CT_T23_SOFTWARE_TRIG_CONFIG);
    }
    else
    {
        eStatus = phRomhalCt_SetTimer(dwMode, dwWTX, gphhalCt_DwBlockWaitingTime,
                                             gphhalCt_DwWaitingTime, gphhalCt_BEmvEn);
    }
    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}

phStatus_t phhalCt_SetBaudRate(uint8_t bFidiValue,uint8_t bWI,uint8_t bBWI ,uint8_t bCWI)
{
    phStatus_t eStatus = PH_ERR_FAILED;
    uint8_t bCount = 0x00;
    do
    {
        /* Search for the supported FiDi value from the table */
        for(bCount=0;bCount <PHHAL_CT_FIDI_TAB_LEN;bCount++)
        {
            if(bFidiValue == gkphhalCt_BPreScalar[bCount].bFiDi)
            {
                break;
            }
        }
        if(bCount == PHHAL_CT_FIDI_TAB_LEN) /* End of table reached no FiDi found */
        {
            eStatus = PH_ERR_INVALID_PARAMETER;
            break;
        }

        /* Set the clock divider value from the table */
        PH_REG_SET_FIELD(CT_CCRx_REG, ACC2_ACC0, gkphhalCt_BPreScalar[bCount].bClockDivider);
        /* Set the PDR register value from the table */
        PH_REG_SET(CT_PDRx_LSB_REG, (uint8_t )(gkphhalCt_BPreScalar[bCount].wPdrRegValue));
        PH_REG_SET(CT_PDRx_MSB_REG, (uint8_t )(gkphhalCt_BPreScalar[bCount].wPdrRegValue >> 8));

        /* Update the current FiDi value */
        gphhalCt_BCurrentFiDi = bFidiValue;
        /* Calculate the timing parameters based on the table index */
        (void)phhalCt_CalculateWWTBWTCWT(bCount, bWI,bBWI ,bCWI);

        eStatus = PH_ERR_SUCCESS;
    }while(0);
    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_CT);
}

/* *****************************************************************************************************************
 * Internal Functions
 * ***************************************************************************************************************** */

/**
 *Function Name     : phhalCt_ClearGLobalVar
 *Description       : This Api is used to clear the global variables used within CT HAL except the slot index.
 *
 *Input Parameters  : None
 *
 *Output Parameters : None
 *
 */
void phhalCt_ClearGLobalVar(void)
{
    gphhalCt_BActivationState = 0x00;
    gphhalCt_BTimerCount = 0x00;

    gphhalCt_BTransmitComplete = 0x00;
    gphhalCt_WPendingBytes = 0x00;
    gphhalCt_BCWTFlag = 0x00;
    gphhalCt_WDataCount = 0x00;
    gphhalCt_WReceiveOffset = 0x00;
}

/**
 *Function Name     : phhalCt_SetDefaultValues
 *Description       : This Api is used to set the default values to the  global variables used for protocol parameters
 *
 *Input Parameters  : psAtrParams - pointer to Atr parameter structure.
 *
 *Output Parameters : None
 *
 */
void phhalCt_SetDefaultValues(phhalCt_AtrParameterType_t  *psAtrParams)
{

    gphhalCt_BFiDi     = PHHAL_CT_DEFAULT_FIDI;
    gphhalCt_BCurrentFiDi = PHHAL_CT_DEFAULT_FIDI;

    psAtrParams->bValueofNInTC1 = PHHAL_CT_DEFAULT_EXTRAGUARD_TIME;
    psAtrParams->psAtrPalParams->bCWI   = PHHAL_CT_7816_DEFAULT_CWI;
    psAtrParams->psAtrPalParams->bBWI    = PHHAL_CT_7816_DEFAULT_BWI;
    psAtrParams->psAtrPalParams->bWI = PHHAL_CT_DEFAULT_WI;
    gphhalCt_BFirstOfferedProt = 0x00;

    psAtrParams->bFlagT15 = 0x00;
    psAtrParams->bFlagT15TAPresent = 0x00;
    psAtrParams->bEarlyEventFlag = 0x00;

    psAtrParams->bTCKByte = 0x00;
    psAtrParams->bInvalidAtr = 0x00;
    psAtrParams->bInvalidTD1 = 0x00;
    psAtrParams->bInvalidTA1 = 0x00;
    psAtrParams->bLastOfferedProt = 0x00;
    psAtrParams->bWarmResetState = 0x00;

    psAtrParams->psAtrPalParams->bIFSC = PHHAL_CT_DEFAULT_IFSC;
    psAtrParams->psAtrPalParams->bFiDi = PHHAL_CT_DEFAULT_FIDI;
    psAtrParams->psAtrPalParams->bProtSelT1 = 0x00;
    psAtrParams->psAtrPalParams->bProtSelT0 = 0x01;
    psAtrParams->psAtrPalParams->bNegotiableMode = 0x00;
    psAtrParams->psAtrPalParams->bFlagT15TAValue = 0x00;
    psAtrParams->psAtrPalParams->bCRCPresent = 0x00;

    gphhalCt_WReceiveSize = 0xFFFF;
}

/**
 *Function Name    : phhalCt_HandleCommonEvent
 *Description      : This function is used to process all common hardware fault events like parity ,overrun,protl,prot.
 *
 *Input Parameters  :void
 *Output Parameters :PH_ERR_SUCCESS - If any of the hardware fault event is not generated.\
 *                   PH_ERR_CT_MUTE_ERROR - Card is muted
 *                   PH_ERR_CT_EARLY_ERROR - Card has answered early
 *                   PH_ERR_CT_PARITY_ERROR - Card has parity error while receiving the ATR
 *                   PH_ERR_CT_OVERUN_ERROR - Fifo is over run while receiving the ATR
 *                   PH_ERR_CT_FRAMING_ERROR - Framing error while receiving the ATR
 */
phStatus_t phhalCt_HandleCommonEvent(void)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
    do
    {
        if((gphhalCt_InEvent & E_PH_HALCT_EVENT_PARITY) == E_PH_HALCT_EVENT_PARITY)
        {
            gphhalCt_BParityErr = 0x00;
            phhalCt_Event_Consume((phhalCt_EventType_t)PH_ERR_CT_PARITY_ERROR);
            eStatus = PH_ERR_CT_PARITY_ERROR;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_OVR_ERR) == E_PH_HALCT_EVENT_OVR_ERR)
        {
            phhalCt_Event_Consume((phhalCt_EventType_t)PH_ERR_CT_OVERUN_ERROR);
            eStatus = PH_ERR_CT_OVERUN_ERROR;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_FRM_ERR) == E_PH_HALCT_EVENT_FRM_ERR)
        {
            phhalCt_Event_Consume((phhalCt_EventType_t)PH_ERR_CT_FRAMING_ERROR);
            eStatus = PH_ERR_CT_FRAMING_ERROR;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_PTL_ERR) == E_PH_HALCT_EVENT_PTL_ERR)
        {
            phhalCt_Event_Consume((phhalCt_EventType_t)PH_ERR_CT_TEMPERATURE_LATCHED);
            eStatus = PH_ERR_CT_TEMPERATURE_LATCHED;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_ASYNC) == E_PH_HALCT_EVENT_ASYNC)
        {
            phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_ASYNC);
            (void)phhalCt_CardDeactivate();
            /* Stop the DCDC LDO */
            phhalPmu_DcdcLdoStop();
            eStatus = PH_ERR_CT_ASYNCH_SHUTDOWN;
            break;
        }
        else if((gphhalCt_InEvent & E_PH_HALCT_EVENT_CARD_REMOVED) == E_PH_HALCT_EVENT_CARD_REMOVED)
        {
            phhalCt_Event_Consume((phhalCt_EventType_t)E_PH_HALCT_EVENT_CARD_REMOVED);
            (void)phhalCt_CardDeactivate();
            /* Stop the DCDC LDO */
            phhalPmu_DcdcLdoStop();
            eStatus = PH_ERR_CT_CARD_REMOVED;
            break;
        }
        else
        {
            if((gphhalCt_InEvent & E_PH_HALCT_EVENT_PROTL_ERR) == E_PH_HALCT_EVENT_PROTL_ERR)
            {
                phhalCt_Event_Consume((phhalCt_EventType_t)PH_ERR_CT_PROTECTION_LATCHED);
                eStatus = PH_ERR_CT_PROTECTION_LATCHED;
            }
        }
    }while(0);
    return eStatus;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

/**
 *Function Name     : phhalCt_CalculateWWTBWTCWT
 *Description       : This Api is used to calculate WWT ,BWT and CWT values according to atr provided parameters.
 *
 *Input Parameters  : bTableCount - index for particular clock uart structure from clock uart table.
 */
static void phhalCt_CalculateWWTBWTCWT(uint8_t bTableCount,uint8_t bWI,uint8_t bBWI ,uint8_t bCWI)
{
    uint16_t wFactoCorr = (uint16_t)(gkphhalCt_BPreScalar[bTableCount].bDValue * 1000);

    /* WWT calculation */
    /*  WWT = (960 x D x WI)
    *  Final WWT = {WWT + (D x 480)} EMVCO Specification 9.2.2.1
    * */
    gphhalCt_DwWaitingTime = (uint32_t)(960 * bWI);
    gphhalCt_DwWaitingTime = (uint32_t)(gphhalCt_DwWaitingTime * gkphhalCt_BPreScalar[bTableCount].bDValue);
    if(gphhalCt_BEmvEn)
    {
        gphhalCt_DwWaitingTime = (uint32_t)((gphhalCt_DwWaitingTime) + (uint32_t)(480 * gkphhalCt_BPreScalar[bTableCount].bDValue));
    }
    gphhalCt_DwWaitingTime += 1;

    /* CWT calculation.*/
     /* EMVCO Specification 9.2.4.2.2.*/
    gphhalCt_DwCharacterWaitingTime = (uint16_t)( 1 << bCWI);
    if(gphhalCt_BEmvEn)
    {
        gphhalCt_DwCharacterWaitingTime += 16;
    }
    else
    {
        gphhalCt_DwCharacterWaitingTime += 12;
    }

    /* BWT calculation.*/
    /* BWT = 11 ETU + 2^BWI*960* (372D/F).*/
    /* Final BWT = {BWT + (D x 960)} EMVCO Specification 9.2.4.2.2.*/
    gphhalCt_DwBlockWaitingTime  = (uint32_t)(1 << bBWI);
    gphhalCt_DwBlockWaitingTime *= wFactoCorr;
    gphhalCt_DwBlockWaitingTime /= 1000;
    gphhalCt_DwBlockWaitingTime *= 960;
    gphhalCt_DwBlockWaitingTime += 12;
    if(gphhalCt_BEmvEn)
    {
        gphhalCt_DwBlockWaitingTime += (uint32_t)(960*gkphhalCt_BPreScalar[bTableCount].bDValue);
    }
}

#endif /* NXPBUILD__PHHAL_CT */
