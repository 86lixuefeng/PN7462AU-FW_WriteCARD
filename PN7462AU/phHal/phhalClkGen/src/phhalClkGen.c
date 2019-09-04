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
 * Clock generation HAL implementation for the CLKGEN module
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-11 18:18:28 +0530 (Fri, 11 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16950 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_CLKGEN
#include "PN7462AU/PN7462AU_pcr.h"
#include "phhalClkGen.h"
#include "ph_Reg.h"
#include "PN7462AU/PN7462AU_clkgen.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "phUser.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_CLKGEN_CLIF_PLL_OUTPUT_CLOCK2                      0   /* CLIF PLL_CLOCK_SELECT */
#define PHHAL_CLKGEN_CLIF_PLL_INPUT_CLOCK                        3   /* CLIF PLL_CLOCK_SELECT */
#define PHHAL_CLKGEN_CLIF_PLL_XTAL_CLOCK                         1   /* CLIF PLL_CLOCK_SELECT */
#define PHHAL_CLKGEN_USB_PLL_OUTPUT_CLOCK                        0   /* USB PLL_CLOCK_SELECT */
#define PHHAL_CLKGEN_XTAL_DELAY_USECS                           20
#define PHHAL_CLKGEN_PLL_DELAY_USECS                            10
#define PHHAL_CLKGEN_XTAL_CHECK_UNIT_USECS                      100
#define PHHAL_CLKGEN_DELAY_10_MICRO_SECS                        10
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static phhalClkGen_Source_t geClockSource;
PH_NOINIT static uint8_t gbClifMode;
PH_NOINIT static uint8_t gbKickOnError;
PH_NOINIT static uint16_t gwXtalActivationTimeOut;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

#define PHHAL_CLKGEN_CHECK_XTAL_STATUS() do {                                       \
while((!PH_REG_TEST_BIT(CLKGEN_STATUS_REG,XTAL_ACTIVATION_TIMEOUT_ERROR))           \
     && (!PH_REG_TEST_BIT(CLKGEN_STATUS_REG,XTAL_OSC_OK)))                          \
{                                                                                   \
    phUser_Wait(PHHAL_CLKGEN_XTAL_CHECK_UNIT_USECS);                                \
}                                                                                   \
} while(0)

static phStatus_t phhalClkGen_XtalStart(void);

static phStatus_t phhalClkGen_XtalSwKick(void);

static phStatus_t phhalClkGen_XtalOscCheck(void);

static void phhalClkGen_WaitBitSetInStatusReg(uint32_t bBit);

static phStatus_t phhalClkGen_ClifPllClkInDetect(phhalClkGen_ClifPllClkDet_t bClifPllDetFlag);

static phStatus_t phhalClkGen_UsbPllClkInDetect(uint8_t bClkEdge, uint8_t bClkWindowLen, uint16_t wTimeoutMicrosec);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalClkGen_Init(phhalClkGen_Source_t eSource, uint32_t dwXtalActivationTimeOut, uint8_t bKickOnError)
{

    /* Save the input parameters in static context*/
    geClockSource = eSource;
    gwXtalActivationTimeOut = 0;
    gbKickOnError = 0;
    gbClifMode = E_CLIFPLL_MAX;

    /* Enable ClkGen clock. */
    PH_REG_SET_BIT(PCR_CLK_CFG_REG, CLOCK_CLKGEN_ENABLE);

    /* Common Setting: Disable PLL Input Buffer Bypass for both USB and CLIF PLL */
    PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,PLL_INPUT_BUFFER_BYPASS);

    /* Common Setting: Enable PLL Input Buffer for both USB and CLIF PLL */
    PH_REG_SET_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,PLL_INPUT_BUFFER_ENABLE);

    /* Configure the Activation Timeout if XTAL is used */
    if(geClockSource == E_CLKSRC_XTAL)
    {
        /* Convert the microseconds in to LFO Clock cycles (1/380Khz = 2.6 ~~ 5/2) */
        gwXtalActivationTimeOut = (uint16_t) (((dwXtalActivationTimeOut*2)/5) & 0xFFF) ;
        gbKickOnError = bKickOnError;
        PH_REG_SET_FIELD(CLKGEN_HFO_XTAL_REG,XTAL_ACTIVATION_TIMEOUT,gwXtalActivationTimeOut);
    }
    else
    {
        /* SW take control of XTAL Oscillator */
        PH_REG_SET_BIT(CLKGEN_HFO_XTAL_REG, XTAL_CONTROL_SW);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalClkGen_DeInit(void)
{

    /* Reset the static context*/
    geClockSource = E_CLKSRC_MAX;
    gwXtalActivationTimeOut = 0;
    gbKickOnError = 0;
    gbClifMode = E_CLIFPLL_MAX;

    /* Reset the XTAL control register to default */
    PH_REG_RESET(CLKGEN_HFO_XTAL_REG);

    /* Disable ClkGen clock. */
    PH_REG_CLEAR_BIT(PCR_CLK_CFG_REG, CLOCK_CLKGEN_ENABLE);

    return PH_ERR_SUCCESS;
}

phStatus_t phhalClkGen_Clk27m12Detect(void)
{

    phStatus_t status = PH_ERR_CLKIN_DETECTION_ERROR;

    do
    {
        /* If clock source is XTAL, check of XTAL OSC OK is set */
        if(geClockSource == E_CLKSRC_XTAL)
        {
            status =  phhalClkGen_XtalOscCheck();

            /* If Crystal Oscillator activation is failure break */
            if(status != PH_ERR_SUCCESS)
            {
                break;
            }
        }

        /*Disable CLIF PLL */
        PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_PLL_ENABLE);

        /*Disable USB PLL */
        PH_REG_SET_BIT(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_PD);

        /*Set the default value of detection window length and input clock edges */
        PH_REG_RESET(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG);

        /* Enable the CLIF Input Clock Detector. The DETECTION_WINDOW_LENGTH & INPUT_CLOCK_EDGES
         * should be their reset value for 27.12Mhz*/
        PH_REG_SET_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_CLK_DETECT_ENABLE);

        /* Enable the USB Input Clock Detector. The DETECTION_WINDOW_LENGTH & INPUT_CLOCK_EDGES
         * should be their reset value for 27.12Mhz*/
        PH_REG_SET_BIT(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG, USB_CLK_DETECT_ENABLE);

        phUser_Wait(PHHAL_CLKGEN_CLIF_PLL_DELAY_USECS);

        /* Wait till CLIF Clock In Detection is done */
        phhalClkGen_WaitBitSetInStatusReg(CLKGEN_STATUS_REG_CLIF_CLK_IN_DETECT_DONE_MASK);


        /* Wait for clk_in_ok = 1 */
        phhalClkGen_WaitBitSetInStatusReg(CLKGEN_STATUS_REG_CLIF_CLK_IN_OK_MASK);

        /* Wait till USB Clock In Detection is done */
        phhalClkGen_WaitBitSetInStatusReg(CLKGEN_STATUS_REG_CLK_IN_DETECT_DONE_MASK);

        /* Wait for clk_in_ok = 1 */
        phhalClkGen_WaitBitSetInStatusReg(CLKGEN_STATUS_REG_CLK_IN_OK_MASK);

        /* Check if there is a clock of a frequency higher or equals to the 27.12MHz at the input of the PLL. */
        if(PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLIF_CLK_IN_OK) && PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLK_IN_OK))
        {
            status = PH_ERR_SUCCESS;
            break;
        }

    } while(0);

    /* Disable the CLIF Input Clock Detector */
    PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_CLK_DETECT_ENABLE);

    /* Disable the USB Input Clock Detector */
    PH_REG_CLEAR_BIT(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG, USB_CLK_DETECT_ENABLE);


    return PH_ADD_COMPCODE(status,PH_COMP_CLKGEN);
}


phStatus_t phhalClkGen_ClifClockStart(phhalClkGen_ClifMode_t eClifMode,phhalClkGen_ClifPllClkDet_t bClifPllDetFlag)
{
    phStatus_t status;

    if(eClifMode == gbClifMode)
    {
        return PH_ERR_SUCCESS;
    }
    do
    {
        status = phhalClkGen_ClifPllClkInDetect(bClifPllDetFlag);
        PH_BREAK_ON_FAILURE(status);

        gbClifMode = eClifMode;

        /* Configure PLL_REF_CLK_SEL=00b always */
        PH_REG_SET_FIELD(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_PLL_REF_CLK_SELECT, 0);

        if(eClifMode == E_CLIFPLL_NORMAL)
        {
        	/* PLL Lock Override without enabling PLL  */
        	/* Clock_presence_Bypass_val = 1 */
        	PH_REG_SET_BIT(CLKGEN_CLOCK_PRESENCE_BYPASS_REG, CLOCK_PRESENCE_BYPASS_VAL);


        	/* Clock_presence_Bypass_enable = 1 */
        	PH_REG_SET_BIT(CLKGEN_CLOCK_PRESENCE_BYPASS_REG,
        			CLOCK_PRESENCE_BYPASS_ENABLE);

/*            //(e.g. for CLK=19.2Mhz CLKGEN_PLL2_CONTROL_REG=0x02E121E0  )
            PH_REG_SET(CLKGEN_CLIF_PLL2_CONTROL_REG, 0x02e121e0);

            //(e.g. for CLK=19.2Mhz CLKGEN_PLL1_CONTROL_REG=0x02E3B190  )
          PH_REG_SET(CLKGEN_CLIF_PLL1_CONTROL_REG, 0x02e3b190);*/

            /* If the Clif Mode is Initiator or Active mode or Target PLM/DL-ALM mode then assign the O/P of PLL input
             * buffer to CLIF Clock */
             PH_REG_SET_FIELD(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,CLIF_PLL_CLOCK_SELECT,
                 PHHAL_CLKGEN_CLIF_PLL_INPUT_CLOCK);
/*            PH_REG_SET_FIELD(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,CLIF_PLL_CLOCK_SELECT,
                PHHAL_CLKGEN_CLIF_PLL_XTAL_CLOCK);
             */
        }
        else
        {
            /*CLIF Mode is PICC SL-ALM Enable CLIF PLL(FracNPLL) */
            PH_REG_SET_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,CLIF_PLL_ENABLE );
            phUser_Wait(PHHAL_CLKGEN_PLL_DELAY_USECS);
            /**********************************************************************************/
            PH_REG_SET(CLKGEN_CLIF_PLL1_CONTROL_REG, 0x02A2E220);
            PH_REG_SET(CLKGEN_CLIF_PLL2_CONTROL_REG, 0x02E20160);
            PH_REG_SET_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_PLL_DIVN1);
            /**********************************************************************************/
            PH_REG_SET_FIELD(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,CLIF_PLL_CLOCK_SELECT,
                            PHHAL_CLKGEN_CLIF_PLL_OUTPUT_CLOCK2);

        }

    } while(0);

    return status;
}

phStatus_t phhalClkGen_ClifClockStop(void)
{
    gbClifMode = 0;

    PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_CLK_DETECT_ENABLE);
    PH_REG_RESET(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG);

    return PH_ERR_SUCCESS;

}

phStatus_t phhalClkGen_UsbClockStart(uint8_t bClkEdge, uint8_t bClkWindowLen, uint16_t wTimeoutMicrosec)
{
    phStatus_t status;
    uint32_t dwTimeElapsed;

    do
    {
        status = phhalClkGen_UsbPllClkInDetect(bClkEdge,bClkWindowLen, wTimeoutMicrosec);
        PH_BREAK_ON_FAILURE(status);

        /* Configure PLL_REF_CLK_SEL=00b always */
        PH_REG_SET_FIELD(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_REF_CLK_SELECT, 0);

        /*Configure the USB PLL Clock Out */
        PH_REG_SET_FIELD(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_CLKOUT_SELECT, PHHAL_CLKGEN_USB_PLL_OUTPUT_CLOCK);

        /* Exit the PLL from the Power Down Mode. */
        PH_REG_CLEAR_BIT(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_PD);

        /* Enable the PLL. */
        PH_REG_SET_BIT(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_CLKEN);

        /* Poll with timeout to confirm the lock status of the PLL. */
        dwTimeElapsed = 0;
        while(!PH_REG_TEST_BIT(CLKGEN_STATUS_REG, USB_PLL_LOCK))
        {
            if(dwTimeElapsed >= wTimeoutMicrosec)
            {
                break;
            }
            else
            {
                phUser_Wait(PHHAL_CLKGEN_DELAY_10_MICRO_SECS);
                dwTimeElapsed += PHHAL_CLKGEN_DELAY_10_MICRO_SECS;
            }
        }

        /* Check if timeout occurred */
        if(!PH_REG_TEST_BIT(CLKGEN_STATUS_REG, USB_PLL_LOCK))
        {
            /* USB PLL Lock Error */
            /* Put the PLL in Power Down Mode. */
            PH_REG_SET_BIT(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_PD);

            /* Disable the USB_PLL Output Clock. */
            PH_REG_CLEAR_BIT(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_CLKEN);

            status = PH_ERR(USB_PLL_LOCK_ERROR,CLKGEN);
        }

    } while(0);

    return status;
}

phStatus_t phhalClkGen_UsbClockStop(void)
{
    PH_REG_RESET(CLKGEN_USB_PLL_CONTROL_REG);
    return PH_ERR_SUCCESS;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static phStatus_t phhalClkGen_XtalStart()
{
    phStatus_t status = PH_ERR_SUCCESS;

    do
    {
        /* This loop will exit when either on activation timeout error or xtal osc ok success */
        PHHAL_CLKGEN_CHECK_XTAL_STATUS();

        /* If Clock is not available and Manual start is enabled */
        if(!PH_REG_TEST_BIT(CLKGEN_STATUS_REG,XTAL_OSC_OK))
        {
            status =  PH_ERR(XTAL_ACT_TO_ERROR,CLKGEN);

            if(gbKickOnError)
            {
                status = phhalClkGen_XtalSwKick();
            }
            break;
        }

    }while(0);

    return status;
}

static phStatus_t phhalClkGen_XtalSwKick(void)
{
    phStatus_t  status;
    uint32_t dwActivationTimeoutLoop;

    /* enable XTAL by sw */
    PH_REG_SET_BIT(CLKGEN_HFO_XTAL_REG, XTAL_CONTROL_SW);

    /*Reset Spare to 0 (reset value is 0) to allow
    the crystal output clock to the rest of the IC */
    PH_REG_CLEAR_BIT(CLKGEN_HFO_XTAL_REG, XTAL_SPARE0);

    /*wait atleast 10us (typical), set to 20 uSecs */
    phUser_Wait(PHHAL_CLKGEN_XTAL_DELAY_USECS);

    /*set xtal_enable_kick to 1, reset xtal_bypass_reg to 0, reset xtal_sel_external_clock to 0*/
    PH_REG_CLEAR_BIT(CLKGEN_HFO_XTAL_REG,XTAL_BYPASS);
    PH_REG_CLEAR_BIT(CLKGEN_HFO_XTAL_REG,XTAL_SEL_EXTERNAL_CLOCK);
    PH_REG_SET_BIT(CLKGEN_HFO_XTAL_REG,XTAL_ENABLE_KICK);

    /*wait atleast 10us (typical), set to 20 uSecs*/
    phUser_Wait(PHHAL_CLKGEN_XTAL_DELAY_USECS);

    /*set enable_xtal to 1*/
    PH_REG_SET_BIT(CLKGEN_HFO_XTAL_REG, XTAL_ENABLE);

   /* Convert the timeout into 20us units */
    dwActivationTimeoutLoop = gwXtalActivationTimeOut/8;
    do
    {
        /*Check every 20us */
        dwActivationTimeoutLoop--;
        phUser_Wait(PHHAL_CLKGEN_XTAL_DELAY_USECS);

    }while((dwActivationTimeoutLoop) && (!PH_REG_TEST_BIT(CLKGEN_STATUS_REG,XTAL_OSC_OK)));

    if(PH_REG_TEST_BIT(CLKGEN_STATUS_REG,XTAL_OSC_OK))
    {
        status = PH_ERR_SUCCESS;
    }
    else
    {
        status =  PH_ERR(XTAL_ACT_TO_ERROR,CLKGEN);

    }

    return status;
}


static phStatus_t phhalClkGen_ClifPllClkInDetect(phhalClkGen_ClifPllClkDet_t bClifPllDetFlag)
{

    phStatus_t status;

    /* Check if there is a clock of a frequency higher or equals to the 27.12MHz at the input of the PLL. */
    if(PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLIF_CLK_IN_OK))
    {
        return PH_ERR_SUCCESS;
    }

    /*Disable CLIF PLL */
    PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_PLL_ENABLE);

    /* Enable the CLIF Input Clock Detector. The DETECTION_WINDOW_LENGTH & INPUT_CLOCK_EDGES
     * should be their reset value for 27.12Mhz*/
    PH_REG_SET_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_CLK_DETECT_ENABLE);

    phUser_Wait(PHHAL_CLKGEN_PLL_DELAY_USECS);

    /* Wait till clock detection procedure is finished. */
    while(!PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLIF_CLK_IN_DETECT_DONE))
    {
        __NOP();
    }

    /* Check if there is a clock of a frequency higher or equals to the 27.12MHz at the input of the PLL. */
    if(PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLIF_CLK_IN_OK))
    {
        status = PH_ERR_SUCCESS;
    }
    else
    {
        /* Clock error - supply clock doesn't exist/less than 27.12MHz. */
        status =  PH_ERR(CLIF_CLK_IN_ERROR,CLKGEN);
    }

    if(bClifPllDetFlag == E_CLIFPLL_CLKDET_SINGLE)
    {
        /* Disable the CLIF Input Clock Detector. */
        PH_REG_CLEAR_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG, CLIF_CLK_DETECT_ENABLE);
    }

    return status;
}

static phStatus_t phhalClkGen_UsbPllClkInDetect(uint8_t bClkEdge, uint8_t bClkWindowLen, uint16_t wTimeoutMicrosec)
{
    phStatus_t status;
    uint32_t dwTimeElapsed;

    do
    {
        /* Put the PLL in Power Down Mode. */
        PH_REG_SET_BIT(CLKGEN_USB_PLL_CONTROL_REG, USB_PLL_PD);

        PH_REG_SET_BIT(CLKGEN_CLIF_PLL_GLOBAL_CONTROL_REG,PLL_INPUT_BUFFER_ENABLE);

        PH_REG_SET_FIELD(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG,
                         INPUT_USB_CLOCK_EDGES_NUMBER,
                         bClkEdge);

        PH_REG_SET_FIELD(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG,
                         DETECTION_WINDOW_LENGTH,
                         bClkWindowLen);

        /* Enable the Input Clock Detector. The DETECTION_WINDOW_LENGTH & INPUT_CLOCK_EDGES
         * should be their reset value for 27.12Mhz*/
        PH_REG_SET_BIT(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG, USB_CLK_DETECT_ENABLE);

        /* Poll with timeout for completion of clock detection procedure. */
        dwTimeElapsed = 0;
        while(!PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLK_IN_DETECT_DONE))
        {
            if(dwTimeElapsed >= wTimeoutMicrosec)
            {
                break;
            }
            else
            {
                phUser_Wait(PHHAL_CLKGEN_DELAY_10_MICRO_SECS);
                dwTimeElapsed += PHHAL_CLKGEN_DELAY_10_MICRO_SECS;
            }
        }

        /* Check if clock detection procedure finished. */
        if(!PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLK_IN_DETECT_DONE))
        {
            /* clock detection procedure timed out */
            status =  PH_ERR(CLKIN_DETECTION_ERROR,CLKGEN);
            break;
        }

        /* Check if there is a clock of a frequency higher or equals to the 27.12MHz at the input of the PLL. */
        if(PH_REG_TEST_BIT(CLKGEN_STATUS_REG, CLK_IN_OK))
        {
            status = PH_ERR_SUCCESS;
        }
        else
        {
            /* Clock error - supply clock doesn't exist/less than 27.12MHz. */
            status =  PH_ERR(USB_CLK_IN_ERROR,CLKGEN);
        }

    }while(0);

    /* Disable the Input Clock Detector. */
    PH_REG_CLEAR_BIT(CLKGEN_INPUT_CLOCK_DETECTOR_CONTROL_REG, USB_CLK_DETECT_ENABLE);

    return status;
}


static void phhalClkGen_WaitBitSetInStatusReg(uint32_t bBit)
{
   uint8_t     bCounter = 0;

   while (0 == PH_REG_GET_FIELD_NS_SC(CLKGEN_STATUS_REG, bBit))
   {
      phUser_Wait(20);

      if (++bCounter == 100)
      break;
   }
}

/*
 * This function is not thread safe.
 * It can be called from Clif context or Usb context.
 */
static phStatus_t phhalClkGen_XtalOscCheck(void)
{

    /* If the clock source is external, OSC OK Signal will not be coming and should not be used */
    if(geClockSource == E_CLKSRC_EXTERNAL)
    {
        return PH_ERR_SUCCESS;
    }
    else
    {
        return phhalClkGen_XtalStart();
    }
}

#endif /* NXPBUILD__PHHAL_CLKGEN */
