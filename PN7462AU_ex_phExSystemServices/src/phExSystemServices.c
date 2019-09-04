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
 * Project:  PN7462AU
 *
 * $Date$
 * $Author$
 * $Revision$ (v05.19.00)
 */

#include <stdio.h>

#include "phhalSysSer.h"
#include "ph_Status.h"
#include "phExSystemServices.h"

#ifndef DEBUG
#warning This application requires user interface for performing the operations. Please USE DEBUG mode and NOT RELEASE mode.
#endif

/*
 * 0 ==> Do not allow this example to run IR-Reversible commands
 * 1 ==> Allow this example to run IR-Reversible commands, but still ask User for confirmation
 */
#define ENABLE_IR_REVERSIBLE_COMMANDS 0

static int AskUserForConfirmation(void);
#define USER_AGREES                     0xA5
#define USER_DOES_NOT_AGREE             0x5A
#define IRREVERSIBLE_TXT "(Careful! Irreversible! UNDO Not Possible!!!)"
int main(void) {

    uint32_t i = 0, dwCommand = 0 ;
    phStatus_t wStatus = PH_ERR_SUCCESS;
    phhalSysSer_Secrow_Config_t sSecrowConfig;
    phhalSysSer_USB_Primary_Dnld_Config_t sPriDwnlCfg;
    phhalSysSer_USBInfo_t sUsbInfo;
    phhalSysSer_USB_PVDD_Config_t sPvddCfg;

    while(1)
    {

        printf("\n\nWelcome to PN7462AU SystemServices Example.\n\n");
        printf("Select one of the following commands:\n");
        printf("1) Perform SECROW Lock" IRREVERSIBLE_TXT "\n");
        printf("2) Enable Code write protection" IRREVERSIBLE_TXT "\n");
        printf("3) Block SWD debugging" IRREVERSIBLE_TXT "\n");
        printf("4) Disable primary download" IRREVERSIBLE_TXT "\n");
        printf("5) Update Product ID\n");
        printf("6) Update Vendor ID\n");
        printf("7) Perform InApplication Programming\n");
        printf("8) Set internal PVDD\n");
        printf("9) Get Rom version\n");
        printf("Others) Invalid\n");
        printf("Input: ");
        fflush(stdin);

        scanf("%d", &dwCommand);

        if((dwCommand) && (dwCommand<=PH_EXSYS_GET_ROM_VERSION))
        {
            printf("\nSelected : %s", gkcArray[dwCommand]);
        }

        switch(dwCommand)
        {
        case PH_EXSYS_SERV_SECROW_LOCK:
            /* SECROW LOCK. */
            if (USER_AGREES == AskUserForConfirmation()) {
                wStatus = phhalSysSer_OTP_SetSecrowLock();
            }
            else
            {
                wStatus = PH_ERR_ABORTED;
            }
            break;

        case PH_EXSYS_SERV_ENABLE_CWP:
            wStatus = phhalSysSer_OTP_SecrowConfig(PH_SYSRV_GET_DATA, &sSecrowConfig);
            PH_BREAK_ON_FAILURE(wStatus);

            /* Enable Code Write Protection. */
            if (USER_AGREES == AskUserForConfirmation()) {
                sSecrowConfig.bEnableCWP[0] = 1;
                wStatus = phhalSysSer_OTP_SecrowConfig(PH_SYSRV_SET_DATA, &sSecrowConfig);
            }
            else
            {
                wStatus = PH_ERR_ABORTED;
            }
            break;

        case PH_EXSYS_SERV_BLOCK_SWD:
            wStatus = phhalSysSer_OTP_SecrowConfig(PH_SYSRV_GET_DATA, &sSecrowConfig);
            PH_BREAK_ON_FAILURE(wStatus);

            if (USER_AGREES == AskUserForConfirmation()) {
                /* Block SWD. */
                sSecrowConfig.bBlockSWD[0] = 1;
                wStatus = phhalSysSer_OTP_SecrowConfig(PH_SYSRV_SET_DATA, &sSecrowConfig);
            }
            else
            {
                wStatus = PH_ERR_ABORTED;
            }
            break;

        case PH_EXSYS_SERV_DIS_PRI_DWNLD:
            wStatus = phhalSysSer_USB_PrimaryDnldConfig(PH_SYSRV_GET_DATA, &sPriDwnlCfg);
            PH_BREAK_ON_FAILURE(wStatus);

            if (USER_AGREES == AskUserForConfirmation()) {
                /* Disable primary download. */
                sPriDwnlCfg.Primary_Dnld_Disable[0] = 1;
                wStatus = phhalSysSer_USB_PrimaryDnldConfig(PH_SYSRV_SET_DATA, &sPriDwnlCfg);
            }
            else
            {
                wStatus = PH_ERR_ABORTED;
            }
            break;

        case PH_EXSYS_SERV_UPDATE_PID:
            wStatus = phhalSysSer_USB_Config(PH_SYSRV_GET_DATA, &sUsbInfo);
            PH_BREAK_ON_FAILURE(wStatus);

            /* Update Product ID. */
            sUsbInfo.USBDesc_PID[0] = 0x17;
            sUsbInfo.USBDesc_PID[1] = 0x01;
            wStatus = phhalSysSer_USB_Config(PH_SYSRV_SET_DATA, &sUsbInfo);
            break;

        case PH_EXSYS_SERV_UPDATE_VID:
            wStatus = phhalSysSer_USB_Config(PH_SYSRV_GET_DATA, &sUsbInfo);
            PH_BREAK_ON_FAILURE(wStatus);

            /* Update Vendor ID. */
            sUsbInfo.USBDesc_VID[0] = 0xC9;
            sUsbInfo.USBDesc_VID[1] = 0x1F;
            wStatus = phhalSysSer_USB_Config(PH_SYSRV_SET_DATA, &sUsbInfo);
            break;

        case PH_EXSYS_SERV_IAP:
            printf("Enter the Flash page number.\n");
            printf("(One page is 128bytes long. For 158k, 1263 is the last Page): ");
            scanf("%d", &i);
            /* IAP */
            wStatus = phhalSysSer_SetFlashProgram(i, gkbaFlashPageData);
            break;

        case PH_EXSYS_SET_INTERNAL_PVDD:
            wStatus = phhalSysSer_USB_PVDD_Config(PH_SYSRV_GET_DATA, &sPvddCfg);
            PH_BREAK_ON_FAILURE(wStatus);

            /* Pvdd source is set to internal.
             * AA- External; 55 - Internal; others - Auto. */
            sPvddCfg.PVDD_Source[0] = 0x55;
            wStatus = phhalSysSer_USB_PVDD_Config(PH_SYSRV_SET_DATA, &sPvddCfg);
            break;

        case PH_EXSYS_GET_ROM_VERSION:
            /* Get Rom version */
            i = 0;
            wStatus = phhalSysSer_GetROM_Version(PH_SYSRV_GET_DATA, (uint32_t *)&i);
            PH_BREAK_ON_FAILURE(wStatus);

            printf("ROM Version: 0x%08X\n", i);
            break;

        default:
            printf("Invalid command!!!\n");
            break;
        }

        if(((dwCommand) && (dwCommand<=PH_EXSYS_GET_ROM_VERSION)))
        {
            if(wStatus)
            {
                printf("Requested operation failed. Error Code 0x%04X.\n", wStatus);
            }
            else
            {
                printf("Operation success.\n");
            }
        }
    }

    /* It is not expected to reach here, if so then Hang. */
    while(1){
        __WFI();
    }
    return 0 ;
}

static int AskUserForConfirmation(void) {
#if ENABLE_IR_REVERSIBLE_COMMANDS
    char input_cmd[10];
    printf("Are you really sure?  This is ir-reversible.\n");
    printf("There is no way this can be reverted. UNDO is not possible.\n");
    printf("The changes are one time programmed to IC.\n");
    printf("This will directly influence the behavior of the IC.\n");

    printf("Type 'YES' To continue. (All Capitals): ");
    scanf("%s",input_cmd);
    if ( input_cmd[0] == 'Y'
        && input_cmd[1] == 'E'
        && input_cmd[2] == 'S'
        && input_cmd[3] == '\0'
        )
    {
        return USER_AGREES;
    }
    else
    {
        printf("Aborted!\n");
        return USER_DOES_NOT_AGREE;
    }
#else
    printf("By default, IR-Reversible commands are not enabled.\n");
    printf("Updated %s and enable ENABLE_IR_REVERSIBLE_COMMANDS to 1\n",(__FILE__));
    return USER_DOES_NOT_AGREE;
#endif

}
