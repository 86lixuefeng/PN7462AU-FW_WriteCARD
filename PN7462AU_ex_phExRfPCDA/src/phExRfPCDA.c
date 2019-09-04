/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2015
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
 * phExRfPCDA.c:  General Boot up tasks for phExRfPCDA
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-09-29 11:26:13 +0530 (Thu, 29 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18681 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"

#include "phExRfPCDA.h"
#include "phhalRf.h"
#include "PN7462AU/PN7462AU_clif.h"
#include "phUser.h"
#include "phhalTimer.h"
#include "phExRfPCDA_A.h"
#include "ph_Log.h"
#include "phhalGpio.h"
#include "phhalPcr.h"
#include "ph_Reg.h"
#include "phFlashBoot.h"
#include "phLED.h"
#include <phRtos.h>
#include "phhalSysSer.h"
#include "phCfg_EE.h"
#include "phhalPmu.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
#define PH_EXRFPCDA_TESTBUS1_SELECT                                   0x01
#define PH_EXRFPCDA_TESTBUS2_SELECT                                   0x02

#define PH_EXRfPCDA_USETXLDO_EXTERNAL               0x0
#define PH_EXRfPCDA_TXLDO_MONITOR_ENABLE            0x1
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT_ALIGNED static uint8_t gbaphExRfPCDA_TxBuf[PH_EXRFPCDA_TX_BUFSIZE];
PH_NOINIT_ALIGNED static uint8_t gbaphExRfPCDA_RxBuf[PH_EXRFPCDA_RX_BUFSIZE];

static phhalTimer_Timers_t *pLedTimer;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phEXRfPCDA_TxLdoMonitorEnable();
static phStatus_t phEXRfPCDA_TxLdoMonitorCheck();

/** Main Polling function */
phStatus_t phExRfPCDA_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer);
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

/** Entry point of the Example */
int main(void)
{
    phStatus_t status;
    /* GPIO4 = tx_active.
     * GPIO5 = rx_active. */
    uint8_t aTestBus1Config[] = {0, /* Updated to gpkphCfg_EE_HW_RfTestBus->bTbDigi1 at run time below */
         PH_EXRFPCDA_TESTBUS1_SELECT};
    uint8_t aTestBus2Config[] = {0, /* Updated to gpkphCfg_EE_HW_RfTestBus->bTbDigi2 at run time below */
        PH_EXRFPCDA_TESTBUS2_SELECT};

    LOG_TXT("BootUp:phExRf\n");
    LOG_TXT(__DATE__ "\n");
    LOG_TXT(__TIME__ "\n\n");

    phFlashBoot_Main();

    phEXRfPCDA_TxLdoMonitorEnable();

    /** <b>LED Configuraiton </b>. */
    /** - Configure and use LED + Timer, to trigger a blinking pattern on LEDs. */
    phLED_Init();
    /** - Set the pattern to be a Normal bootup Pattern. */
    phLED_SetPattern(gkphLED_BootUp);
    /** - Request an LED Timer in units of Milli Seconds. */
    PH_USER_ASSERT(phhalTimer_RequestTimer(E_TUNIT_MILLI_SECS,  &pLedTimer) == PH_ERR_SUCCESS);
    /** - Configure the timer to be be free running with 100ms cycle */
    phhalTimer_Configure(pLedTimer, 100 , &phLED_TimerCallback);
    phhalTimer_Start(pLedTimer, E_TIMER_FREE_RUNNING);

    status = phEXRfPCDA_TxLdoMonitorCheck();
		if (status != PH_ERR_SUCCESS)
		{
			phLED_SetStatus(LED_R);
			LOG_TXT("phExRfPCDA: No External TVDD Present \n");
			return 1;
		}

    /** <b>TestBus Configuraiton </b>. */
    /** - Enable+Configure the Digital Test Bus 1. */
    aTestBus1Config[0] = gpkphCfg_EE_HW_RfTestBus->bTbDigi1;
    status = phhalSysSer_CLIF_Config_DigitalTestBus((void *)aTestBus1Config);
    if (status != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
    }

    /** - Enable+Configure the Digital Test Bus 2. */
    aTestBus2Config[0] = gpkphCfg_EE_HW_RfTestBus->bTbDigi2;
    status = phhalSysSer_CLIF_Config_DigitalTestBus((void *)aTestBus2Config);
    if (status != PH_ERR_SUCCESS)
    {
        LOG_TXT("Test Bus Configuration failed.");
    }

    while(1)
    {
        /** <b>Main Polling Loop </b>. */
        if(phhalRf_GetExtFieldStatus() != PH_ERR_SUCCESS)
        {
            /** - If we don't have External Field, Poll For Cards */
            phExRfPCDA_Poll(gbaphExRfPCDA_TxBuf, gbaphExRfPCDA_RxBuf);
            /** - Switch OFF RF once done */
            phhalRf_FieldOff();
            /** - Wait before next iteration */
            phUser_Wait(5100);
            LOG_TXT_D("RF OFF\n");
        }
        else
        {
            LOG_TXT("Ext Field ON\n");
            /* Not handled for this example */
            LOG_TXT("Target Mode not supported. Hence waiting for external field to go off.\n");
            while(phhalRf_GetExtFieldStatus() == PH_ERR_SUCCESS)
            {
                __WFI();
            }
        }
        /* 300 milli secs */
        phUser_Wait(300*1000);
    }

    return 0;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
phStatus_t phExRfPCDA_Poll(uint8_t *pTxBuffer, uint8_t *pRxBuffer)
{
    phStatus_t status = PH_ERR_INVALID_PARAMETER;

    do{

#ifdef NXPBUILD__CLIF_RM

        LOG_TXT("\nPolling Start\n");

        /** <b>Polling Function </b> */
        /** - Load Protocol for Type A PCD 106 for Tx and Rx */
        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_A_106_P);
        PH_BREAK_ON_FAILURE(status)

        /** - Switch on RF Field.  It is mandatory to Atleast do one Load any Protocol before
         *    switching on any RF Field. */
        status = phhalRf_FieldOn();
        PH_BREAK_ON_FAILURE(status)
        phUser_Wait(5100);
        /** - Poll for Type A Cards */
        status = phExRfPCDA_A_Poll(pTxBuffer, pRxBuffer);
        if(status == PH_ERR_SUCCESS){
            LOG_TXT_D("Success for A\n");
            break;
        }
        phLED_SetPattern(gkphLED_BootUp);

#endif /* NXPBUILD__CLIF_RM */

    }while(0);

    return status;
}

static void phEXRfPCDA_TxLdoMonitorEnable()
{
    /* Check the configured TxLdo is External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXRfPCDA_USETXLDO_EXTERNAL)
    {
#if defined (PHFL_ENABLE_EXTERNAL_TVDD_3V_MON)
		phhalPmu_TxLdoMonitorEnable(PH_EXRfPCDA_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_3V3);
#else
		phhalPmu_TxLdoMonitorEnable(PH_EXRfPCDA_TXLDO_MONITOR_ENABLE, E_5V_MON_SRC_TVDD, E_5V_MON_THRES_4M0V);
#endif
    }
}

static phStatus_t phEXRfPCDA_TxLdoMonitorCheck()
{
	phStatus_t eStatus;
	/* Check if the TxLDO is configured as External */
	if (gpkphCfg_EE_Boot_TxLdoParams->bUseTxLdo == PH_EXRfPCDA_USETXLDO_EXTERNAL)
	{
		/* Perform the Monitor Check of TxLdo */
		eStatus = phhalPmu_TxLdoMonitorCheck();
	}
	else
	{
		return PH_ERR_SUCCESS;
	}
	return eStatus;
}
