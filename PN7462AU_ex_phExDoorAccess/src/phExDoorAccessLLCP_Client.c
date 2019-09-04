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
 * This file contains the Snep Client.
 *
 * Project:  phExLlcp
 *
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 */


/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_Status.h"
#include "phhalRf.h"
#include <phOsal.h>
#include <phlnLlcp.h>
#include <phnpSnep.h>
#include <phRtos.h>
#include "ph_Log.h"
#include "phLED.h"
#include "phExDoorAccessLLCP_Client.h"
#include "phExDoorAccessLLCP_NDEFMsg.h"

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
#ifdef PHFL_ENABLE_LLCPSNEP
static phnpSnep_Sw_DataParams_t           snpSnep;
static const uint8_t  baSnepAppBuf[] = PH_EXLLCP_NDEFMSG;

extern phlnLlcp_Sw_DataParams_t        slnLlcp;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

void phExLLCP_Client(void * pLlcp)
{
  phlnLlcp_Transport_Socket_t        ClientSocket;
  phStatus_t                          status = 0;
    phStatus_t                          wInitstatus = 0;
    uint8_t                             bClientRxBuffer[64];
    uint32_t   dwDataLen = sizeof(baSnepAppBuf);
    uint32_t   dwClientRxBuffLength = sizeof(bClientRxBuffer);

    do{
        status = phnpSnep_Sw_Init(&snpSnep, sizeof(phnpSnep_Sw_DataParams_t), pLlcp, &ClientSocket);
        status = phlnLlcp_WaitForActivation(pLlcp);

        phOsal_EventClear(slnLlcp.LlcpEventObj.EventHandle,  E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);
        phLED_TimerCallback();

        //LOG_TXT_D("LLCP Client\n");
        wInitstatus = phnpSnep_ClientInit(&snpSnep, phnpSnep_Default_Server, NULL, bClientRxBuffer,dwClientRxBuffLength);

        if(wInitstatus == PH_ERR_SUCCESS)
        {
            status = phnpSnep_Put(&snpSnep, (uint8_t *)baSnepAppBuf, dwDataLen);
            ClientSocket.fReady = true;
            //LOG_TXT_D("Snep PUT\n");

            if(((status & PH_ERR_MASK) != PH_ERR_LLCP_DEACTIVATED) && ((status & PH_ERR_MASK) != PH_ERR_PEER_DISCONNECTED))
            {
                status = phnpSnep_ClientDeInit(&snpSnep);
                status = phlnLlcp_Deactivate(pLlcp);
                //LOG_TXT_D("SNEP PUT;LLCP Deact\n");
            }
            else
            {
                status = phlnLlcp_Transport_Socket_Unregister(snpSnep.plnLlcpDataParams, snpSnep.psSocket);
                //LOG_TXT_D("SENP PUT; Link lost\n");
                /* status = phlnLlcp_Deactivate(pLlcp); */
            }
        }
        else if((wInitstatus & PH_ERR_MASK) != PH_ERR_LLCP_SAP_EXCEEDED)
        {
            status = phlnLlcp_Transport_Socket_Unregister(snpSnep.plnLlcpDataParams, snpSnep.psSocket);
            //LOG_TXT_D("UnSoc\n");
        }
        else
        {
            /* Do nothing. */
        }

    }while(1);

    phRtos_TaskDelete(NULL);
}



/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#endif
