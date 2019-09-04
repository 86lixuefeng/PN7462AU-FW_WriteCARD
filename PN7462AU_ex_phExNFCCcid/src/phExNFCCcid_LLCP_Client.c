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
 * $Date: 2016-09-12 10:26:49 +0530 (Mon, 12 Sep 2016) $
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18634 $ (v05.19.00)
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
#include "phUser.h"
#include "ph_Log.h"
#include "phExNFCCcid_LLCP_Client.h"
#include "phExNFCCcid_UsbCcid.h"
#include "phExNFCCcid_LLCP_NDEFMsg.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#ifdef PHFL_LLCP_SNEP_ENABLE
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static phnpSnep_Sw_DataParams_t           snpSnep;
static const uint8_t  baSnepAppBuf[] =  PH_EXNFCCCID_LLCP_NDEFMSG;

extern phlnLlcp_Sw_DataParams_t        slnLlcp;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
void phExNFCCcid_LLCP_Client(void * pLlcp)
{
	phlnLlcp_Transport_Socket_t    ClientSocket;
	phStatus_t                     status = 0;
    phStatus_t                     wInitstatus = 0;
    uint8_t                        bClientRxBuffer[64] = {0x0};
    uint32_t   dwDataLen = sizeof(baSnepAppBuf);
    uint32_t   dwClientRxBuffLength = sizeof(bClientRxBuffer);

    do
    {
        status = phnpSnep_Sw_Init(&snpSnep, sizeof(phnpSnep_Sw_DataParams_t), &slnLlcp, &ClientSocket);

        status = phlnLlcp_WaitForActivation(pLlcp);

        phOsal_EventClear(slnLlcp.LlcpEventObj.EventHandle,  E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_LLCP_ACTIVATED, NULL);

        wInitstatus = phnpSnep_ClientInit(&snpSnep, phnpSnep_Default_Server, NULL, bClientRxBuffer,dwClientRxBuffLength);
        /* LOG_ON_FAILURE(wInitstatus, "Client Init"); */

        if(wInitstatus == PH_ERR_SUCCESS)
        {
            status = phnpSnep_Put(&snpSnep, (uint8_t *)baSnepAppBuf, dwDataLen);
            ClientSocket.fReady = true;

            if(((status & PH_ERR_MASK) != PH_ERR_LLCP_DEACTIVATED) && ((status & PH_ERR_MASK) != PH_ERR_PEER_DISCONNECTED))
            {
                status = phnpSnep_ClientDeInit(&snpSnep);
                status = phlnLlcp_Deactivate(pLlcp);
                /* LOG_ON_FAILURE(status, "LLCP DeInit"); */
            }
            else
            {
                /* LOG_TXT("UnSoc\n"); */
            	status = phlnLlcp_Transport_Socket_Unregister(snpSnep.plnLlcpDataParams, snpSnep.psSocket);
                /* LOG_ON_FAILURE(status, "Client UnSoc"); */
            }
        }
        else if((wInitstatus & PH_ERR_MASK) != PH_ERR_LLCP_SAP_EXCEEDED)
        {
        	status = phlnLlcp_Transport_Socket_Unregister(snpSnep.plnLlcpDataParams, snpSnep.psSocket);
            LOG_ON_FAILURE(status, "Client UnSoc");
        }
        else
        {

        }
    }while(1);
    phRtos_TaskDelete(NULL);
}
#endif
/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
