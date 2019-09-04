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
 * phExMain_Log.c : Facilitates the LOG feature while running Debug sessions.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-14 11:42:13 +0530 (Mon, 14 Dec 2015) $
 * $Author: Rajesh Sugasi (nxp69678) $
 * $Revision: 16972 $ (v05.19.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "phExMain_Log.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define LOG_CASE_ENUM(BR,STR) \
    case((BR)): LOG_TXT(STR "\n"); break

#define LOG_CASE_ENUM_EVT_SOURCE(BR,STR) \
    case((BR)): LOG_TXT("\tEvent Source: " STR "\n"); break

#define LOG_CASE_ENUM_BOOT_REASON(BR,STR) \
    case((BR)): LOG_TXT("Boot Reason: " STR "\n"); break

#define LOG_CASE_ENUM_DISCLOOP(BR,STR) \
    case((BR)): LOG_TXT(STR "\n"); break


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

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
#if PHFL_LOG_ENABLE

void phExMain_Log_BootReason(phhalPcr_BootReason_t br) {
    switch(br) {
        LOG_CASE_ENUM(E_BOOT_NONE,"NONE");
        LOG_CASE_ENUM(E_STARTUP_POR,"STARTUP_POR");
        LOG_CASE_ENUM(E_RFLDT_BOOT,"RFLDT_BOOT");
        LOG_CASE_ENUM(E_WUC_CNT,"WUC_CNT");
        LOG_CASE_ENUM(E_INT_AUX_BOOT,"INT_AUX_BOOT");
        LOG_CASE_ENUM(E_CT_PRESENCE,"CT_PRESENCE");
        LOG_CASE_ENUM(E_I2C_ADDR,"I2C_ADDR");
        LOG_CASE_ENUM(E_SMBUS_ADDR,"SMBUS_ADDR");
        LOG_CASE_ENUM(E_SPI_ADDR,"SPI_ADDR");
        LOG_CASE_ENUM(E_SWP_DET,"SWP_DET");
        LOG_CASE_ENUM(E_SOFT_RESET,"SOFT_RESET");
        LOG_CASE_ENUM(E_WATCH_DOG,"WATCH_DOG");
        LOG_CASE_ENUM(E_TVDD_MON,"TVDD_MON");
        LOG_CASE_ENUM(E_HIF_RESET,"HIF_RESET");
        LOG_CASE_ENUM(E_TEMP_SENSOR0,"TEMP0");
        LOG_CASE_ENUM(E_TEMP_SENSOR1,"TEMP1");
        LOG_CASE_ENUM(E_NO_PVDD,"NO_PVDD");
        LOG_CASE_ENUM(E_PVDD_ILIM,"PVDD_ILIM");
        LOG_CASE_ENUM(E_GPIO_INT,"GPIO_INT");
        LOG_CASE_ENUM(E_HSU_INT,"HSU_INT");
        LOG_CASE_ENUM(E_DFT_RESET,"DFT_RESET");
        LOG_CASE_ENUM(E_ACTIVE_HPD,"ACTIVE_HPD");
        LOG_CASE_ENUM(E_VBUS_LOW,"VBUS_LOW");
        default:
            LOG_TXT("\n\t\tINVALID_phhalPcr_BootReason\n");
    }
}

void phExMain_Log_DiscLoop(
    phStatus_t st, uint8_t bPollingLoopEntryCount)
{
    switch (st)
    {
        LOG_CASE_ENUM_DISCLOOP(PHAC_DISCLOOP_COLLISION_PENDING, "Collision\n");
        LOG_CASE_ENUM_DISCLOOP(PHAC_DISCLOOP_MULTI_TECH_DETECTED,"Multi tech detected\n");

        case PHAC_DISCLOOP_DEVICE_ACTIVATED:
        case PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED:
        case PHAC_DISCLOOP_EXTERNAL_RFOFF:
        case PHAC_DISCLOOP_MERGED_SEL_RES_FOUND:
        case PH_ERR_SUCCESS:
            break;
        case PHAC_DISCLOOP_NO_TECH_DETECTED:
        case PHAC_DISCLOOP_NO_DEVICE_RESOLVED:
        case PHAC_DISCLOOP_FAILURE:
            break;
        LOG_CASE_ENUM_DISCLOOP(PHAC_DISCLOOP_ACTIVATED_BY_PEER,"Peer detected\n");
        LOG_CASE_ENUM_DISCLOOP(PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED,"Act Peer detected\n");
        LOG_CASE_ENUM_DISCLOOP(PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED,"LPCD success but NO card detected\n");
        LOG_CASE_ENUM_DISCLOOP(PH_ERR_INVALID_PARAMETER,"Parameter error\n");
        default:
            LOG_X32("\n\t\t\tINVALID_phacDiscLoop_Fw_Status:",st);
        break;
    }
}


void phExMain_Log_FlashBootEventIds(phFlashBoot_Event_Ids evtId) {
    switch (evtId)
    {
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_NONE,"NONE");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_BOOT,"BOOT");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_CT,"CT");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_CT_TASK,"CT_TASK");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_HIF,"HIF");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_I2CM,"I2CM");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_PCR,"PCR");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_PMU,"PMU");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_SPIM,"SPIM");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_USB,"USB");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_CLIF,"CLIF");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_TIMER,"TIMER");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_SYS,"SYS");
        LOG_CASE_ENUM_EVT_SOURCE(E_PH_ERROR,"ERROR");
        default:
            LOG_U32("phFlashBoot_Event_Ids",(unsigned int)evtId);
            LOG_TXT("\t\tINVALID_FlashBootEventIds\n");
            break;
    }
}



#endif /* PHFL_LOG_ENABLE */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
