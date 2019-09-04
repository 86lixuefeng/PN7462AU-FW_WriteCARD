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
#include "ph_Status.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalCt.h"
#include "phhalCt_Int.h"
#include "phUser.h"
#include "phhalNvic.h"
#include "phhalPmu.h"
#include "phhalPcr.h"
#include "ph_Platform.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_ctip.h"
#include "phhalCt_Event.h"
#include "ph_Reg.h"
#include "phhalSysSer.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
/*
 * ROM code is returning this code for main card absence, which is equivalent to class change indicated error code in flash.
 * So this macro is created.
 */
#define PHHAL_CT_ROM_CARD_ABSENCE_RETURN   0xC093

/* System service call for disabling the DCDC */
#define PHSYSSV_CTIF_DCDC_DISABLE               0x06

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/**
 * CT HAL Event Reporting
 * @warning Can be Set only by CT ISR. Setting this at any other place would cause problems and driver crash.
 */
PH_NOINIT phhalCt_EventType_t gphhalCt_InEvent;
phhalCt_SlotType_t  gphhalCt_ESlotType;    /**< Used to check current slot enabled[auxillary or main]*/

PH_NOINIT uint8_t  gphhalCt_DriverBuff[PHHAL_CT_MAXBUFSIZE];/**< CT HAL internal 261 bytes software buffer for ATR
                                                                                                and APDU reception.*/
PH_NOINIT uint8_t gphhalCt_BEmvEn;  /**< Used to set flag for EMVCo profile.*/
PH_NOINIT uint8_t  gphhalCt_BActivationState;                /**< Used to check the activation status in Card Activate*/
PH_NOINIT uint8_t  gphhalCt_BTimerCount;                   /**< 8 bit timer reg is used for 24000 etu ART timer,
                                                                                this variable is used for count */
PH_NOINIT uint8_t  gphhalCt_BFirstOfferedProt;     /**< Used to update first offered protocol according to TD1 in Atr.*/

/* Following variables are used to hold the waiting times.*/
PH_NOINIT uint32_t gphhalCt_DwCharacterWaitingTime;   /**< Used for inter-character delay time for APDU reception in T=1 protocol.*/
PH_NOINIT uint32_t gphhalCt_DwWaitingTime;            /**< In T=0 APDU used for inter byte delay timer  for character reception.*/
PH_NOINIT uint32_t gphhalCt_DwBlockWaitingTime;           /**< Used for the Block waiting timer[max block waiting time].*/

/* Following variables are used to control the transmit and receive.*/
PH_NOINIT uint8_t gphhalCt_BTransmitComplete;  /**< Used to set after transmission has successfully completed.*/
PH_NOINIT uint16_t gphhalCt_WPendingBytes;     /**< Used to store number of remaining byte in fifo which are not read still.*/
PH_NOINIT uint8_t gphhalCt_BCWTFlag;        /**< Used to set only in T1 protocol when CWT is going to be use.*/

PH_NOINIT uint8_t gphhalCt_BFiDi;      /**< FiDi value as per TA1 byte in Atr.*/
PH_NOINIT uint8_t gphhalCt_BCurrentFiDi; /**< Updated FiDi value in code according to negotiable or specific mode,
                                                                    this only will be used for setting baud rate.*/

/* Following variables are used to count the number of bytes in activation, transmit, receive */
PH_NOINIT uint16_t gphhalCt_WReceiveOffset;    /**< Used for keeping a count of the offset value in the APDU reception*/
PH_NOINIT volatile  uint16_t gphhalCt_WDataCount;         /**< Used as index for storing data to HAL buffer in ISR context */
PH_NOINIT uint16_t gphhalCt_WReceiveSize;

static pphhalCt_CallbackFunc_t gpphhalCt_CallbackFunc;
PH_NOINIT static uint32_t gdwphhalCtRegIntrpts;

PH_NOINIT volatile uint8_t gphhalCt_BParityErr;
PH_NOINIT uint8_t gphhalCt_BLastByteTransmit;
PH_NOINIT uint8_t gphhalCt_BDisableDCDC;

/**
 * This table will be used for all valid possible baud rate selection.
 */
const phhalCt_ClockUart_t gkphhalCt_BPreScalar[]={
        {  0x0BA0,    0x01,     6,   1},
        {  0x05D0,    0x02,     6,   2},
        {  0x02E8,    0x03,     6,   4},
        {  0x0174,    0x04,     6,   8},
        {  0x00BA,    0x05,     6,   16},
        {  0x00B0,    0x06,     7,   32},
        {  0x005D,    0x07,     7,   64},
        {  0x00F8,    0x08,     6,   12},
        {  0x0120,    0x09,     7,   20},

        {  0x08B8,    0x11,     5,   1},
        {  0x045C,    0x12,     5,   2},
        {  0x022E,    0x13,     5,   4},
        {  0x0117,    0x14,     5,   8},
        {  0x00BA,    0x15,     6,   16},
        {  0x00B0,    0x16,     7,   32},
        {  0x005D,    0x17,     7,   64},
        {  0x00BA,    0x18,     5,   12},
        {  0x0120,    0x19,     7,   20},

        {  0x0AE6,    0x21,     4,   1},
        {  0x0573,    0x22,     4,   2},
        {  0x02B9,    0x23,     4,   4},
        {  0x015C,    0x24,     4,   8},
        {  0x00AA,    0x25,     4,   16},
        {  0x0110,    0x26,     7,   32},
        {  0x0080,    0x27,     7,   64},
        {  0x00E8,    0x28,     4,   12},
        {  0x01B0,    0x29,     7,   20},

        {  0x0BA0,    0x31,     3,   1},
        {  0x05D0,    0x32,     3,   2},
        {  0x02E8,    0x33,     3,   4},
        {  0x0174,    0x34,     3,   8},
        {  0x00BA,    0x35,     3,   16},
        {  0x005D,    0x36,     3,   32},
        {  0x00B0,    0x37,     7,   64},
        {  0x00F8,    0x38,     3,   12},
        {  0x0094,    0x39,     3,   20},

        {  0x0D14,    0x41,     2,   1},
        {  0x068A,    0x42,     2,   2},
        {  0x0345,    0x43,     2,   4},
        {  0x01A2,    0x44,     2,   8},
        {  0x00D1,    0x45,     2,   16},
        {  0x0066,    0x46,     2,   32},
        {  0x0110,    0x47,     7,   64},
        {  0x0117,    0x48,     2,   12},
        {  0x00A7,    0x49,     2,   20},

        {  0x0BA0,    0x51,     1,   1},
        {  0x05D0,    0x52,     1,   2},
        {  0x02E8,    0x53,     1,   4},
        {  0x0174,    0x54,     1,   8},
        {  0x00BA,    0x55,     1,   16},
        {  0x005D,    0x56,     1,   32},
        {  0x005D,    0x57,     3,   64},
        {  0x00F8,    0x58,     1,   12},
        {  0x0094,    0x59,     1,   20},

        {  0x0E88,    0x61,     1,   1},
        {  0x0744,    0x62,     1,   2},
        {  0x03A2,    0x63,     1,   4},
        {  0x01D1,    0x64,     1,   8},
        {  0x00E8,    0x65,     1,   16},
        {  0x0074,    0x66,     1,   32},
        {  0x00AE,    0x67,     5,   64},
        {  0x0136,    0x68,     1,   12},
        {  0x00BA,    0x69,     1,   20},

        {  0x0C00,    0x91,     5,   1},
        {  0x0600,    0x92,     5,   2},
        {  0x0300,    0x93,     5,   4},
        {  0x0180,    0x94,     5,   8},
        {  0x00C0,    0x95,     5,   16},
        {  0x0060,    0x96,     5,   32},
        {  0x0080,    0x97,     7,   64},
        {  0x0100,    0x98,     5,   12},
        {  0x0096,    0x99,     5,   20},

        {  0x0C00,    0xA1,     3,   1},
        {  0x0600,    0xA2,     3,   2},
        {  0x0300,    0xA3,     3,   4},
        {  0x0180,    0xA4,     3,   8},
        {  0x00C0,    0xA5,     3,   16},
        {  0x0060,    0xA6,     3,   32},
        {  0x003C,    0xA7,     4,   64},
        {  0x0100,    0xA8,     3,   12},
        {  0x0098,    0xA9,     3,   20},

        {  0x0C00,    0xB1,     2,   1},
        {  0x0600,    0xB2,     2,   2},
        {  0x0300,    0xB3,     2,   4},
        {  0x0180,    0xB4,     2,   8},
        {  0x00C0,    0xB5,     2,   16},
        {  0x0060,    0xB6,     2,   32},
        {  0x0040,    0xB7,     3,   64},
        {  0x0100,    0xB8,     2,   12},
        {  0x0098,    0xB9,     2,   20},

        {  0x0C00,    0xC1,     1,   1},
        {  0x0600,    0xC2,     1,   2},
        {  0x0300,    0xC3,     1,   4},
        {  0x0180,    0xC4,     1,   8},
        {  0x00C0,    0xC5,     1,   16},
        {  0x0060,    0xC6,     1,   32},
        {  0x0048,    0xC7,     2,   64},
        {  0x0100,    0xC8,     1,   12},
        {  0x0099,    0xC9,     1,   20},

        {  0x1000,    0xD1,     1,   1},
        {  0x0800,    0xD2,     1,   2},
        {  0x0400,    0xD3,     1,   4},
        {  0x0200,    0xD4,     1,   8},
        {  0x0100,    0xD5,     1,   16},
        {  0x0080,    0xD6,     1,   32},
        {  0x0040,    0xD7,     1,   64},
        {  0x0154,    0xD8,     1,   12},
        {  0x00CC,    0xD9,     1,   20},

    };

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
typedef phStatus_t (phRomhalCt_RegInit_t)(uint8_t bPullupSetStatus, uint8_t bCardPresConnectorStatus,
    uint8_t bAutoCTDeactivationEnable, uint8_t bSlewRateConfig);
typedef phStatus_t (phRomhalCt_SwitchSlotConfig_t)(phhalCt_SlotType_t eSlot_Index);
typedef phStatus_t (phRomhalCt_CheckCardPres_t)(void);
typedef phStatus_t (phRomhalCt_ClockStartStopConfig_t)(uint8_t bClockStop, uint8_t bLowHigh,
    phhalCt_SlotType_t eSlot_Index);

#define phRomhalCt_RegInit(bPullupSetStatus, bCardPresConnectorStatus, bAutoCTDeactivationEnable, bSlewRateConfig)   \
    ((phRomhalCt_RegInit_t *) PH_HAL_CT_REGINIT_ADDR)(bPullupSetStatus, bCardPresConnectorStatus, \
        bAutoCTDeactivationEnable, bSlewRateConfig)

#define phRomhalCt_SwitchSlotConfig(eSlot_Index)        \
    ((phRomhalCt_SwitchSlotConfig_t *) PH_HAL_CT_SWITCHSLOTCONFIG_ADDR)(eSlot_Index)

#define phRomhalCt_CheckCardPres()   ((phRomhalCt_CheckCardPres_t *) PH_HAL_CT_CHECKCARDPRES_ADDR)()

#define phRomhalCt_ClockStartStopConfig(bClockStop, bLowHigh, eSlot_Index) \
    ((phRomhalCt_ClockStartStopConfig_t *) PH_HAL_CT_CLOCKSTARTSTOPCONFIG_ADDR)(bClockStop, bLowHigh, eSlot_Index)

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalCt_Init(phhalCt_InitParam_t *psCtInitParams)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /**Check for the null pointers*/
        if ( psCtInitParams == NULL)
        {
            return PH_ERR(INVALID_PARAMETER,HAL_CT);
        }
#endif

    /** By default CT IP is initialized to main slot */
    gphhalCt_ESlotType = E_MAIN_SLOT;
    gpphhalCt_CallbackFunc = NULL;
    gdwphhalCtRegIntrpts = 0;
    gphhalCt_BDisableDCDC = 0x00;

    /* Enabling EMVCo profile by default.*/
    gphhalCt_BEmvEn = TRUE;

    /* Clear/ put to default of all the global variables. */
    phhalCt_ClearGLobalVar();

    gphhalCt_InEvent = E_PH_HALCT_EVENT_WAITING;
    eStatus = phhalCt_Event_Init();
    PH_RETURN_ON_FAILURE(eStatus);
    /* Clear the HAL buffer. */
    (void) phUser_MemSet(gphhalCt_DriverBuff, 0x00, PHHAL_CT_MAXBUFSIZE);

    /* Call the ROM3 Api to configure these parameters */
    eStatus = phRomhalCt_RegInit(psCtInitParams->bPullupSetStatus,
    psCtInitParams->bCardPresConnectorStatus,
    psCtInitParams->bAutoCTDeactivationEnable,
    psCtInitParams->bSlewRateConfig);

   return eStatus ;
}

phStatus_t phhalCt_SetConfig(phhalCt_Configs_t eConfig, uint32_t dwValue)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    switch(eConfig)
    {
    case E_EMV_ENABLE:
        gphhalCt_BEmvEn =  (uint8_t)((dwValue>0)? 1: 0);
        break;
    default:
        wStatus = PH_ERR(INVALID_PARAMETER, HAL_CT);
    }

    return wStatus;
}

phStatus_t phhalCt_SwitchSlot(phhalCt_SlotType_t eSlot_Index)
{
    phStatus_t eStatus = PH_ERR_SUCCESS ;
    gphhalCt_ESlotType = eSlot_Index;

    phRomhalCt_SwitchSlotConfig(eSlot_Index);

    return eStatus;
}

phStatus_t phhalCt_CheckCardPres(void)
{
    phStatus_t eStatus = PH_ERR(CT_MAIN_CARD_ABSENT,HAL_CT);

    if(gphhalCt_ESlotType == E_AUXILIARY_SLOT)
    {

    }
    else
    {
        /* Check if card is present in the main slot */
        eStatus = phRomhalCt_CheckCardPres();
        if(eStatus == PHHAL_CT_ROM_CARD_ABSENCE_RETURN)
        {
            eStatus = PH_ERR(CT_MAIN_CARD_ABSENT,HAL_CT);
        }
        else
        {
            eStatus = PH_ERR(CT_MAIN_CARD_PRESENT,HAL_CT);
        }
    }
    return eStatus;
}


phStatus_t phhalCt_ClockStartStop(uint8_t bClockStop, uint8_t bLowHigh)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    if(gphhalCt_ESlotType == E_AUXILIARY_SLOT)
    {
        /* For Aux. Slot.*/
    }
    else
    {
        eStatus = phRomhalCt_ClockStartStopConfig(bClockStop, bLowHigh, E_MAIN_SLOT);
    }

    return eStatus;
}

phStatus_t phhalCt_DeInit(void)
{
    phStatus_t eStatus;
    /* Perform a soft reset */
    if (phhalCt_Reset() != PH_ERR_SUCCESS)
    {
        return PH_ERR(FAILED,HAL_CT);
    }

    /* De-configuring the CT specific PCR Registers */
    PH_REG_CLEAR_BIT(PCR_SYS_REG, ENABLE_CT);
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG, CLOCK_CTIF_ENABLE);
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG, IPCLOCK_CTIF_ENABLE);
    PH_REG_CLEAR_BIT(PCR_SYS_REG, AUTOMATIC_CT_DEACT);

    /* Disables the NVIC for Contact Interface */
    PH_REG_CLEAR_BITN(NVIC_ISER_REG,PHHAL_NVIC_CTIF);

    /* Globals specific to the current session of card has to be cleared*/
    phhalCt_ClearGLobalVar();
    gphhalCt_BDisableDCDC = 0x00;
    /* Clear Events */
    eStatus = phhalCt_Event_Consume((phhalCt_EventType_t)(E_PH_HALCT_EVENT_ALL | E_PH_HALCT_EVENT_CARD_REMOVED));
    PH_RETURN_ON_FAILURE(eStatus);
    /* Deinitialize all event group.*/
    eStatus = phhalCt_Event_Deinit();

    return eStatus;
}


phStatus_t phhalCt_Reset(void)
{
    /* Perform a Soft reset this will reset the whole CT UART IP */
    PH_REG_CLEAR_BIT(CT_SSR_REG,NOT_SOFTRESET);
    /* Delay to wait for one clock cycle */
    phUser_Wait(100);
    if (PH_REG_TEST_BIT(CT_SSR_REG, NOT_SOFTRESET))
    {
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ERR(FAILED,HAL_CT);
    }
}


void phhalCt_AsyncShutDown(void)
{
    (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_ASYNC);
}

phStatus_t phhalCt_RegCallBack(pphhalCt_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts)
{
    if ((pCallBackFunc == NULL) || (dwInterrupts == 0)){
        return PH_ERR(INVALID_PARAMETER,HAL_CT);
    }

    gpphhalCt_CallbackFunc = pCallBackFunc;
    gdwphhalCtRegIntrpts = dwInterrupts;

    return PH_ERR_SUCCESS;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

void CTIF_IRQHandler(void)
{
    uint8_t dwDummy;
    /* Read the Status of USR1 Register and check if any error bit is enabled and set corresponding error flag*/
    uint8_t bUartStatus1 = (uint8_t)PH_REG_GET(CT_USR1_REG);
    uint8_t bUartStatus2 = (uint8_t)PH_REG_GET(CT_USR2_REG);

    if (bUartStatus2 & CT_USR2_REG_PRESL_MASK)
    {
        if (!PH_REG_TEST_BIT(CT_MSR_REG, PRES))
        {
            /* Post the event */
            (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_CARD_REMOVED);
            /* Call the deactivate also even though the HW deactivation takes place */
            phhalCt_CardDeactivate();
        }
        else /* Card is inserted */
        {
            /* Sanity only, if the card is inserted before the DCDC timer elapses then
             * check that DCDC system services call is pending */
            if(gphhalCt_BDisableDCDC) /* The timer is triggered in the software mode */
            {
                /* Disable the DCDC through the system services */
                phhalSysSer_SetSystemServiceExtension(PHSYSSV_CTIF_DCDC_DISABLE,(void *)(&dwDummy));
                gphhalCt_BDisableDCDC = 0x00;
                PHHAL_CT_STOP_TIMER;
            }
        }

        if((gpphhalCt_CallbackFunc) && (gdwphhalCtRegIntrpts & bUartStatus2))
        {
            gpphhalCt_CallbackFunc(bUartStatus2);
        }

        return;
    }

    if (bUartStatus1 & CT_USR1_REG_MUTE_MASK)
    {
        /* Post the event */
        (void) phhalCt_Event_Post(E_PH_HALCT_EVENT_MUTE);
        return;
    }
    if (bUartStatus1 & CT_USR1_REG_EARLY_MASK)
    {
        /* Post the event */
        (void) phhalCt_Event_Post(E_PH_HALCT_EVENT_EARLY);
        return;
    }
    if (bUartStatus1 & CT_USR1_REG_PE_MASK)
    {
        /* During the T=0 transmit if the last byte in the Fifo is naked by the card for more than
         * PEC retry counter this error is captured here and updated */
        if(!gphhalCt_BActivationState)
        {
            if((!(PH_REG_TEST_BIT(CT_UCR1x_REG,PROT))) && (PH_REG_TEST_BIT(CT_UCR1x_REG,T_R)))
            {
                /* Flush the Fifo */
                PH_REG_SET_BIT(CT_UCR2x_REG,FIFO_FLUSH);
                gphhalCt_BParityErr = 0x01;
            }
        }

        /* Post the event */
        (void) phhalCt_Event_Post(E_PH_HALCT_EVENT_PARITY);
        if ((!(bUartStatus1 & CT_USR1_REG_FT_MASK))||(gphhalCt_BActivationState))
        {
            return;
        }
    }
    if (bUartStatus1 & CT_USR1_REG_OVR_MASK)
    {
        /* Post the event */
        (void) phhalCt_Event_Post(E_PH_HALCT_EVENT_OVR_ERR);
        return;
    }
    if (bUartStatus1 & CT_USR1_REG_FER_MASK)
    {
        /* Post the event */
        (void) phhalCt_Event_Post(E_PH_HALCT_EVENT_FRM_ERR);
        return;
    }
    if (bUartStatus1 & CT_USR1_REG_FT_MASK)
    {
        /* If at least one byte is received[ATR byte] and card activate is called restart the timer **/
        if (gphhalCt_BActivationState)
        {
            gphhalCt_DriverBuff[gphhalCt_WDataCount++] = (uint8_t) PH_REG_GET(CT_UTR_URR_REG_adr1);

            /* Handle Atr parsing parity error.*/
            if((gphhalCt_WDataCount == 0x01))
            {
                /* Again Enabling Parity error event after first byte.*/
                PH_REG_CLEAR_BIT(CT_UCR2x_REG,DISPE);
            }

            /* Post the event */
            (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_RX);
            bUartStatus1 = (bUartStatus1 &  (uint8_t)~(CT_USR1_REG_FT_MASK));
        }
        else
        {
            if(gphhalCt_BTransmitComplete == 0x00)
            {


                /*Disabling the Fifo threshold interrupt for last character so no interrupt will be generated after
                 *last character transmission.
                 */
                if(PH_REG_GET_FIELD(CT_FCR_REG, FTC4_FTC0) == 0x00)
                {
                    /*
                     * Last character is put in Fifo here ,to overcome CWT violation due to RTOS context switching time.
                     */
                     PH_REG_SET_BIT(CT_UCR2x_REG,DISFT);
                     PH_REG_SET_BIT(CT_UCR1x_REG,LCT);
                     PH_REG_SET(CT_UTR_URR_REG_adr1, gphhalCt_BLastByteTransmit);
                }
                /* Post the event */
                (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_TX);
                bUartStatus1 = (bUartStatus1 &  (uint8_t)~(CT_USR1_REG_FT_MASK));
                return;
            }
                if(gphhalCt_BCWTFlag == 0x00)
                {
                    if(PH_REG_TEST_BIT(CT_UCR1x_REG,PROT))
                    {
                         /* Change the count value to character waiting time Dirty
                         * fix for CWT waiting time not able to achieve in RTOS env */
                    PH_REG_SET(CT_TOR1_REG, (uint8_t)(gphhalCt_DwCharacterWaitingTime));
                    PH_REG_SET(CT_TOR2_REG, (uint8_t)(gphhalCt_DwCharacterWaitingTime >> 8));
                    PH_REG_SET(CT_TOR3_REG, (uint8_t)(gphhalCt_DwCharacterWaitingTime >> 16));
                        /* Timer 1,2,3 forms a 24 bit timer and they start on each subsequent start bit */
                    PH_REG_SET(CT_TOC_REG, PHHAL_CT_T123START_BIT_CONFIG);

                        gphhalCt_BCWTFlag = 0x01;
                    }

                }

                /* At higher baud rate receive events are missed during processing of last byte,
                 * thats why for doing copy in hal buffer as soon as bytes comes in fifo  this logic is needed.
                 */
                while(PH_REG_GET(CT_FSR_REG))
                {
                    gphhalCt_DriverBuff[gphhalCt_WDataCount++] = (uint8_t) PH_REG_GET(CT_UTR_URR_REG_adr1);
                    gphhalCt_WPendingBytes++;

                }
                if(gphhalCt_WPendingBytes >= gphhalCt_WReceiveSize)
                {
                    /* Post the event */
                    (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_RX);

                    /* Place a very high value so that even if some more bytes come in the Fifo
                     * this check will never get called*/
                    gphhalCt_WReceiveSize = 0xFFFF;
                }
            }
        return;
    }
    /* Check the status of USR2 REGISTER*/
    if (bUartStatus2 & CT_USR2_REG_TO3_MASK)
    {
        if (gphhalCt_BActivationState) /*For the case for ATR reception*/
        {
            /* All ATR Bytes Received */
            gphhalCt_BActivationState = 0;
            /* Post the event */
            (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_TO3);
            bUartStatus2 = (bUartStatus2 &  (uint8_t)~(CT_USR2_REG_TO3_MASK));
            return;
        }
        else if(gphhalCt_BDisableDCDC) /* The timer is triggered in the software mode */
        {
            /* Disable the DCDC through the system services */
            phhalSysSer_SetSystemServiceExtension(PHSYSSV_CTIF_DCDC_DISABLE,(void *)(&dwDummy));
            gphhalCt_BDisableDCDC = 0x00;
            PHHAL_CT_STOP_TIMER;
        }
        else /* For BWT or CWT violation */
        {
            /* Post the event */
            (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_TO3);
            bUartStatus2 = (bUartStatus2 &  (uint8_t)~(CT_USR2_REG_TO3_MASK));
            return;
        }
    }
    if (bUartStatus2 & CT_USR2_REG_TO1_MASK)
    {
        /* Increment the software counter */
        gphhalCt_BTimerCount++;
        /* Count is reached for 20160 value */
        if(gphhalCt_BTimerCount == PHHAL_CT_ATR_TOR1_TIMER_MAX_COUNT)
        {
            PHHAL_CT_STOP_TIMER;
            gphhalCt_BTimerCount = 0;
            gphhalCt_BActivationState = 0;
            /* Post the event */
            (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_TO1);
        }
        return;
    }
    if (bUartStatus2 & CT_USR2_REG_INTAUXL_MASK)
    {
        /* If TDA INT AUX is registered by the caller call the call back  */
        if((gpphhalCt_CallbackFunc) && (gdwphhalCtRegIntrpts & bUartStatus2))
        {
            gpphhalCt_CallbackFunc(bUartStatus2);
        }
        bUartStatus2 = (bUartStatus2 &  (uint8_t)~(CT_USR2_REG_INTAUXL_MASK));
    }
    if (bUartStatus2 & CT_USR2_REG_PROTL_MASK)
    {
        /* Post the event */
        (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_PROTL_ERR);

        if((gpphhalCt_CallbackFunc) && (gdwphhalCtRegIntrpts & bUartStatus2))
        {
            gpphhalCt_CallbackFunc(bUartStatus2);
        }

        bUartStatus2 = (bUartStatus2 &  (uint8_t)~(CT_USR2_REG_PROTL_MASK));
    }
    if (bUartStatus2 & CT_USR2_REG_PTL_MASK)
    {
        /* Post the event */
        (void)phhalCt_Event_Post(E_PH_HALCT_EVENT_PTL_ERR);

        if((gpphhalCt_CallbackFunc) && (gdwphhalCtRegIntrpts & bUartStatus2))
        {
            gpphhalCt_CallbackFunc(bUartStatus2);
        }

        bUartStatus2 = (bUartStatus2 &  (uint8_t)~(CT_USR2_REG_PTL_MASK));
    }

}

#endif /* NXPBUILD__PHHAL_CT */
