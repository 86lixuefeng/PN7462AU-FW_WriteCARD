/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2013,2014
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

/**
 *
 * @file
 * This file implements the Api's for PMU wrapper IP
 * Project:  PN7462AU
 *
 * $Date: 2016-06-22 11:25:52 +0530 (Wed, 22 Jun 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 18485 $ (v04.13.00)
 *
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_PMU
#include "phhalNvic.h"
#include "phhalPmu.h"
#include "phhalPcr.h"
#include "phUser.h"
#include "PN7462AU/PN7462AU_pmu.h"
#include "PN7462AU/PN7462AU_pcr.h"
#include "PN7462AU/PN7462AU_ctip.h"

#ifdef NXPBUILD__PHHAL_CT
#include "phhalCt.h"
#endif
#ifdef NXPBUILD__PHHAL_RF
#include "phhalRf.h"
#endif

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_PMU_MAX_TVDD_SUPPLY                   4
#define PHHAL_PMU_TXLDO_5VMON_DELAY                 50

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
/* Both the variables must be zero initialized. */
static pphhalPmu_CallbackFunc_t gpphhalPmu_CallbackFunc = NULL;
static uint32_t gdwphhalPmuRegIntrpts = 0;

static uint8_t  gbphhalPmuTxLdoInt;
static phhalPmu_TvddSel_t  gephhalPmuFullPowerTvddSel;
static phhalPmu_LowPower_TvddSrc_t  gephhalPmuTvddSrc;
static uint8_t  gbphhalPmuOverCurrentEn;
static uint16_t gwphhalPmuTxLdoWait;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phhalPmu_TxLdoFullPower(phhalPmu_TvddSel_t bTvddSel, uint8_t bOverCurrentEn);
static void phhalPmu_TxLdoLowPower(phhalPmu_TvddSel_t bTvddSel);
static void phhalPmu_TxLdoLowPower_2(void);
static uint16_t phhalPmu_TxLdoClkConfig(uint16_t wWaitTime);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

phStatus_t phhalPmu_TxLdoInit(uint8_t bUseTxLdo, phhalPmu_TvddSel_t eFullPowerTvddSel,
                              phhalPmu_LowPower_TvddSrc_t eLowPowerTvddSrc,
                              uint16_t wWaitTime, uint8_t bOverCurrentEn)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;

    do
    {

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    if(eLowPowerTvddSrc > E_VBUS_SRC)
    {
        break;
    }
    if(eFullPowerTvddSel > E_TVDD_4M75_V)
    {
        break;
    }
#endif

        /* Store all the values in local context for future use */
        gbphhalPmuTxLdoInt = bUseTxLdo;
        gephhalPmuFullPowerTvddSel = eFullPowerTvddSel;
        gephhalPmuTvddSrc = eLowPowerTvddSrc;
        gbphhalPmuOverCurrentEn = bOverCurrentEn;


        if(bUseTxLdo)
        {
        	/* Exit the TxLdo Standby mode and Enter TxLdo Low Power mode */
        	phhalPmu_TxLdoStandby(0x00);

        	/* Configure the TxLdo Clock based on TxLdo Startup time */
            gwphhalPmuTxLdoWait = phhalPmu_TxLdoClkConfig(wWaitTime);
        }
        else
        {
            gwphhalPmuTxLdoWait = 0;
        }

        eStatus = PH_ERR_SUCCESS;
    }while(0);

    return eStatus;
}

phStatus_t phhalPmu_TxLdoStart(phhalPmu_DriverConfig_t eConfig)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;
    uint8_t bLdoStarted = FALSE;

    do
    {
        /* Start the TxLdo only if the TxLdo is internal otherwise just return success for the External LDO */
        if(E_FULL_POWER == eConfig && gbphhalPmuTxLdoInt)
        {
            phhalPmu_TxLdoFullPower(gephhalPmuFullPowerTvddSel,gbphhalPmuOverCurrentEn);
            bLdoStarted = TRUE;
        }
        else if(E_LOW_POWER == eConfig && gephhalPmuTvddSrc == E_VUP_SRC)
        {
        	/* If the TxlDo is in Full power and transitions to low power, only enable the low power regulator @ 3V and need not wait */
        	if(PH_REG_TEST_BIT(PMU_TXLDO_CONTROL_REG,TXLDO_ENABLE) && !PH_REG_TEST_BIT(PMU_TXLDO_CONTROL_REG,TXLDO_LOW_POWER_EN))
        	{
        	    PH_REG_SET_FIELD(PMU_TXLDO_CONTROL_REG, TXLDO_SELECT,E_TVDD_3M0_V);
        		PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LOW_POWER_EN);
        	}
        	else
        	{
        		/*If the TxLdo was in low power from boot, again transition to low power and need to wait */
                phhalPmu_TxLdoLowPower(E_TVDD_3M0_V);
                bLdoStarted = TRUE;
        	}
        }
        else if(E_LOW_POWER == eConfig && gephhalPmuTvddSrc == E_VBUS_SRC)
        {
            phhalPmu_TxLdoLowPower_2();
            bLdoStarted = TRUE;
        }
        else if(E_LOW_POWER == eConfig && gephhalPmuTvddSrc == E_TVDD_IN_SRC)
        {
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
            PH_BREAK_IF_TRUE_WITH_ERROR((gbphhalPmuTxLdoInt != 0x0),eStatus,INVALID_PARAMETER);
#endif

            /* Stop the internal TXLDo since the TVDD is powered externally */
            phhalPmu_TxLdoStop();
        }
        else
        {
            //QL Warning
        }

        if(bLdoStarted)
        {
            phUser_Wait(gwphhalPmuTxLdoWait);
            if(FALSE == PH_REG_TEST_BIT(PMU_STATUS_REG, TXLDO_TVDD_OK))
            {
               return PH_ERR(TXLDO_ERROR,PMU);
            }
        }
        eStatus = PH_ERR_SUCCESS;
    }while(0);

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PMU);
}

phStatus_t phhalPmu_TxLdoStop(void)
{
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LOW_POWER_EN);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG,TXLDO_ENABLE_LP2);
    PH_REG_SET_FIELD(PMU_TXLDO_CONTROL_REG, TXLDO_SELECT,0);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE);

    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_OVERCURRENT_EN);
    PH_REG_SET_BIT_WO(PMU_INTERRUPT_CLR_ENABLE_REG,TXLDO_OVERCURRENT_IRQ_CLEAR_ENABLE);
    return PH_ERR_SUCCESS;
}
phStatus_t phhalPmu_TxLdoStandby(uint8_t bEnableStandBy)
{

    do{
        /* Check whether int TVDD is used? */
        if(!gbphhalPmuTxLdoInt){
            break;
        }

        if(bEnableStandBy)
        {
            if(!(PH_REG_GET_FIELD(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE)
            		|| PH_REG_GET_FIELD(PMU_TXLDO_CONTROL_REG, TXLDO_LOW_POWER_EN)))
            {
                /* Discard the error status. */
                (void)phhalPmu_TxLdoStart(E_FULL_POWER);
            }

            __phUser_EnterCriticalSection();

            /* Wait for the TVDD Cap to settle */
            phUser_Wait(gwphhalPmuTxLdoWait);

            /* Stop the TxlDO */
            phhalPmu_TxLdoStop();

            PH_REG_SET_BIT(PCR_PMU_REG, TXLDO_ENABLE_STANDBY);

            __phUser_ExitCriticalSection();

        }
        else
        {
        	/* Always transition to TxLdo Low Power mode */
        	phhalPmu_TxLdoFullPower(gephhalPmuFullPowerTvddSel, gbphhalPmuOverCurrentEn);
        }

    }while(0);

    return PH_ERR_SUCCESS;
}

phStatus_t phhalPmu_TxLdoMonitorEnable(uint8_t bEnableMonitor,phhalPmu_Monitor_Src_t eMonSource,
                                        phhalPmu_Monitor_Threshold_t eMonThreshold)
{
    phStatus_t eStatus = PH_ERR_INVALID_PARAMETER;

    do
    {

#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    if(eMonSource > E_5V_MON_SRC_TVDD || eMonThreshold > E_5V_MON_THRES_4M0V)
    {
        break;
    }
#endif

        if(bEnableMonitor)
        {
            /* Configure 5V Monitor Source - TVDD */
            PH_REG_SET_FIELD(PCR_TXLDO_MON_REG,SRC_5V_MONITOR,eMonSource);

            /* Configure 5V Monitor threshold */
            PH_REG_SET_FIELD(PCR_TXLDO_MON_REG,THRES_5V_MONITOR,eMonThreshold);

            /* Enable the monitor and wait for 50uS delay */
            PH_REG_SET_BIT(PCR_TXLDO_MON_REG, EN_5V_MONITOR);
            phUser_Wait(PHHAL_PMU_TXLDO_5VMON_DELAY);
        }
        else
        {
            PH_REG_CLEAR_BIT(PCR_TXLDO_MON_REG, EN_5V_MONITOR);
        }
        eStatus = PH_ERR_SUCCESS;
    }while(0);

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PMU);
}

phStatus_t phhalPmu_TxLdoMonitorCheck(void)
{
    phStatus_t eStatus = PH_ERR_SUCCESS;

    if(!(PH_REG_TEST_BIT(PMU_INTERRUPT_STATUS_REG, TXLDO_5V_MON_IRQ_STATUS)))
    {
        eStatus = PH_ERR_TXLDO_5VMON_NOT_OK;
    }

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PMU);
}

phStatus_t phhalPmu_DcdcLdoCheck(void)
{
    uint8_t bClass;
    phStatus_t eStatus = PH_ERR_DCDCLDO_VBUSP_LOW;
    bClass = (uint8_t)(PH_REG_GET_FIELD(CT_PCR_REG,VCCSEL1_VCCSEL0));
    PH_RETURN_IF_TRUE_WITH_ERROR(((bClass == E_CLASS_A) || (bClass == E_CLASS_C)), DCDCLDO_VBUSP_LOW, PMU);

    /* For Class B check the LDO in follower mode or doubler mode */
    /* If already in doubler mode nothing can be done return error */
    if((PH_REG_TEST_BIT(PMU_LDO_CONTROL_REG,VBUSP_HI)))
    {
        /* Inform the user that he can start the activation again in doubler mode Class B*/
        eStatus = PH_ERR_SUCCESS;
    }

    return PH_ADD_COMPCODE(eStatus,PH_COMP_PMU);
}


phStatus_t phhalPmu_RegCallBack(pphhalPmu_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
    if ((pCallBackFunc == NULL) || (dwInterrupts == 0)){
        return PH_ERR(INVALID_PARAMETER,PMU);
    }
#endif

    gpphhalPmu_CallbackFunc = pCallBackFunc;
    gdwphhalPmuRegIntrpts = dwInterrupts;

    return PH_ERR_SUCCESS;
}



/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */


static void phhalPmu_TxLdoFullPower(phhalPmu_TvddSel_t bTvddSel, uint8_t bOverCurrentEn)
{
    __phUser_EnterCriticalSection();
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LOW_POWER_EN);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG,TXLDO_ENABLE_LP2);
    PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LIMITER_EN);
    if(bOverCurrentEn)
    {
        PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_OVERCURRENT_EN);
        PH_REG_SET(PMU_INTERRUPT_SET_ENABLE_REG,
                   PMU_INTERRUPT_SET_ENABLE_REG_TXLDO_OVERCURRENT_IRQ_SET_ENABLE_MASK);
    }
    else
    {
        PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_OVERCURRENT_EN);
        PH_REG_SET_BIT_WO(PMU_INTERRUPT_CLR_ENABLE_REG,TXLDO_OVERCURRENT_IRQ_CLEAR_ENABLE);
    }

    PH_REG_SET_FIELD(PMU_TXLDO_CONTROL_REG, TXLDO_SELECT,bTvddSel);
    PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE);
    __phUser_ExitCriticalSection();

    return;
}

static void phhalPmu_TxLdoLowPower(phhalPmu_TvddSel_t bTvddSel)
{
	__phUser_EnterCriticalSection();
	PH_REG_CLEAR_BIT(PCR_PMU_REG, TXLDO_ENABLE_STANDBY);
	PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LIMITER_EN);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_OVERCURRENT_EN);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG,TXLDO_ENABLE_LP2);
    PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LOW_POWER_EN);
    PH_REG_SET_FIELD(PMU_TXLDO_CONTROL_REG, TXLDO_SELECT,bTvddSel);
    PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE);
    __phUser_ExitCriticalSection();

    return;
}

static void phhalPmu_TxLdoLowPower_2()
{
	__phUser_EnterCriticalSection();
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_ENABLE);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LIMITER_EN);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_OVERCURRENT_EN);
    PH_REG_CLEAR_BIT(PMU_TXLDO_CONTROL_REG, TXLDO_LOW_POWER_EN);
    PH_REG_SET_BIT(PMU_TXLDO_CONTROL_REG,TXLDO_ENABLE_LP2);
	__phUser_ExitCriticalSection();
    return;
}

static uint16_t phhalPmu_TxLdoClkConfig(uint16_t wWaitTime)
{

	/* Assign the EEPROM value derived from wWaitTime to TXLDO Clock */
	PH_REG_SET_FIELD(PCR_CLK_CFG2_REG, CTRL_TXLDO_CLK, (wWaitTime & PHHAL_PMU_TXLDO_LFO_VALUE_MASK) >> PHHAL_PMU_TXLDO_LFO_VALUE_POS);

	wWaitTime = wWaitTime & PHHAL_PMU_TXLDO_WAITTIME_VALUE_MASK;

	/* Customers shall not set less than 140us or greater than 1.12 ms. The below code is for error management */
    if(wWaitTime < PHHAL_PMU_LFO1_TXLDO_MIN_US)
    {
        wWaitTime = PHHAL_PMU_LFO2_TXLDO_MIN_US;
    }
    else if(wWaitTime > PHHAL_PMU_LFO8_TXLDO_MIN_US)
    {
    	wWaitTime = PHHAL_PMU_LFO8_TXLDO_MIN_US;
    }
    else
    {
    	/* Do Nothing */
    }
    return wWaitTime;

}

/**
 *
 * @param phhalPmu_Isr
 */
PH_INTERRUPT(PMU_IRQHandler)
{
    uint32_t dwIrqStatus;

    /* Get the ISR Status */
    dwIrqStatus = (PH_REG_GET(PMU_INTERRUPT_STATUS_REG));
    /* Clear the Pending Interrupt Status */
    PH_REG_SET(PMU_INTERRUPT_CLR_STATUS_REG, dwIrqStatus);
    dwIrqStatus &= (PH_REG_GET(PMU_INTERRUPT_ENABLE_REG));

    /* Check if the current limitation of the Vcc is risen */
    if (dwIrqStatus & PMU_INTERRUPT_STATUS_REG_VCC_ILIM_ACT_IRQ_STATUS_MASK)
    {
#ifdef NXPBUILD__PHHAL_CT
        phhalCt_AsyncShutDown();
        /* Shut down the CT in case it is activated or auto deactivation is called  */
        phhalCt_CardDeactivate();
#endif
    }

    /* Check if the DCDC overload has happened */
    if (dwIrqStatus & PMU_INTERRUPT_STATUS_REG_DCDC_OVERLOAD_IRQ_STATUS_MASK)
    {
#ifdef NXPBUILD__PHHAL_CT
        phhalCt_AsyncShutDown();
        /* Shut down the CT in case it is activated or auto deactivation is called  */
        phhalCt_CardDeactivate();
#endif
    }
    /* Check if the PVDD has fallen below the threshold voltage */
    if (dwIrqStatus & PMU_INTERRUPT_STATUS_REG_PVDD_IRQ_STATUS_MASK)
    {
        PH_REG_FLIP_BIT(PCR_SYS_REG,PVDD_IRQ_VAL);
    }
    /* Check if the TXLDO over current has happened */
    if (dwIrqStatus & PMU_INTERRUPT_STATUS_REG_TXLDO_OVERCURRENT_IRQ_STATUS_MASK)
    {
#ifdef NXPBUILD__PHHAL_RF
        /* Call the async api of field off */
        phhalRf_SetIdleState(E_PH_HALRF_ENABLE);
        phhalRf_FieldOff();
#endif
    }

    if(gpphhalPmu_CallbackFunc && (gdwphhalPmuRegIntrpts & dwIrqStatus))
    {
        gpphhalPmu_CallbackFunc(dwIrqStatus);
    }

}
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif /* NXPBUILD__PHHAL_PMU */
