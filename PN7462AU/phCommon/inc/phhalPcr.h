/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors 2013, 2014
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** @file
 *
 * PCR Hal implementation for ARM platform.
 *
 * Project:  PN7462AU
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 18406 $ (v04.13.00)
 * $Date: 2016-05-18 12:38:59 +0530 (Wed, 18 May 2016) $
 *
 */

#ifndef PHHALPCR_H
#define PHHALPCR_H
/**
 * \addtogroup phhalPcr
 *
 * The Power Control and Reset APIs
 *
 * @{
 *
 */
/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_PCR
#include "PN7462AU/PN7462AU_pcr.h"
#include "ph_Status.h"
#include "PN7462AU_ROM3.h"

/* ******************************************************************************************************************
 * MACROS/Defines
 * **************************************************************************************************************** */
/**
 * Macro for soft reset
 */
#define PHHAL_PCR_DO_SOFT_RESET  do {         \
        PH_REG_SET(PCR_CTRL_REG,PCR_CTRL_REG_CLR_BOOT_REGS_MASK);\
        PH_REG_SET_BIT_WO(PCR_CTRL_REG,SOFT_RESET); \
    } while (0)
/**
 * Macro to get the VBUS status
 */
#define PHHAL_PCR_USB_VBUS_STATUS()           PH_REG_GET_FIELD_NS(PCR_BOOT_REG, USB_VBUS_OK)
/**
 * Macro to check PVDD OK
 */
#define PHHAL_PCR_CHECKPVDDOK()               PH_REG_GET_FIELD_NS(PCR_BOOT_REG, POK_PVDD)

/**
 * Macro to check PVDD 3V OK
 */
#define PHHAL_PCR_CHECKPVDD_3V_OK()               PH_REG_GET_FIELD_NS(PCR_BOOT_REG, POK_PVDD_3V)

/**
 * Macro to select the host interface
 */
#define PHHAL_PCR_SELECT_HIF(bHif)            PH_REG_SET_FIELD(PCR_SYS_REG, HIF_SELECTION, bHif)
/**
 * Maximum wake up time value 2000 milisecond
 */
#define PHHAL_PCR_MAX_WAKEUP_TIMER_VALUE   2000
/**
 * Minimum wake up time value 1 milisecond
 */
#define PHHAL_PCR_MIN_WAKEUP_TIMER_VALUE   1
/**
 * Macro for the Host Interface - I2C
 */
#define PHHAL_PCR_HIF_I2C          0x01
/**
 * Macro for the Host Interface - SPI
 */
#define PHHAL_PCR_HIF_SPI          0x02
/**
 * Macro for the Host Interface - HSUART
 */
#define PHHAL_PCR_HIF_HSUART       0x03
/**
 * Macro for the Host Interface - USB
 */
#define PHHAL_PCR_HIF_USB          0x04
/**
 * Macro for the Host Interface - SMBUS
 */
#define PHHAL_PCR_HIF_SMBUS        0x05


/* This is the default value of the PCR_CLK_CFG_REG set during the clock intialization
 * Customer can change the following bits only of the value in PHHAL_PCR_CLK_CONFIGURATION
 * if required to save power
 * CLOCK_CTIF_ENABLE                     -> Bit 23 ON  : Clock source for SPIM enabled
 * CLOCK_SPIM_ENABLE                     -> Bit 19 ON  : Clock source for SPIM enabled
 * CLOCK_HOSTIF_ENABLE                   -> Bit 17 ON  : Clock source for HOSTIF enabled
 * CLOCK_TIMER_ENABLE                    -> Bit 16 ON  : Clock source for TIMER enabled
 * CLOCK_CRC_ENABLE                      -> Bit 15 ON  : Clock source for CRC enabled
 * CLOCK_CLKGEN_ENABLE                   -> Bit 14 ON  : Clock source for CLKGEN enabled
 * CLOCK_RNG_ENABLE                      -> Bit 12 ON  : Clock source for RNG enabled
 * CLOCK_CLIF_ENABLE                     -> Bit 11 ON  : Clock source for CLIF enabled
 * */
//#define PHHAL_PCR_CLK_CONFIGURATION                     0x00B3D800

/**
 * This is the default value of the PCR_CLK_CFG_REG set during the clock intialization
 * Customer can change the following bits only of the value in PHHAL_PCR_CLK_CONFIGURATION
 * if required to save power */

/* = 0x70FBD800 */
#ifdef NXPBUILD__PHHAL_CT
#define PHHAL_PCR_CLK_CT                                                \
    PH_FIELD(PCR_CLK_CFG_REG, IPCLOCK_CTIF_ENABLE, 0)                   \
    | PH_FIELD(PCR_CLK_CFG_REG, CLOCK_CTIF_ENABLE, 1)
#else /* NXPBUILD__PHHAL_CT */
#define PHHAL_PCR_CLK_CT        0
#endif /* NXPBUILD__PHHAL_CT */

#ifdef NXPBUILD__PHHAL_HIF
#define PHHAL_PCR_CLK_HIF                                               \
    PH_FIELD(PCR_CLK_CFG_REG, IPCLOCK_HSUART_ENABLE, 0)                 \
    | PH_FIELD(PCR_CLK_CFG_REG, CLOCK_HOSTIF_ENABLE, 1)
#else /* NXPBUILD__PHHAL_HIF */
#define PHHAL_PCR_CLK_HIF       0
#endif /* NXPBUILD__PHHAL_HIF */

#ifdef NXPBUILD__PHHAL_SPIM
#define PHHAL_PCR_CLK_SPIM                                              \
    PH_FIELD(PCR_CLK_CFG_REG, IPCLOCK_SPIM_ENABLE, 0)                   \
    | PH_FIELD(PCR_CLK_CFG_REG, CLOCK_SPIM_ENABLE, 1)
#else /* NXPBUILD__PHHAL_SPIM */
#define PHHAL_PCR_CLK_SPIM      0
#endif /* NXPBUILD__PHHAL_SPIM */

#ifdef NXPBUILD__PHHAL_I2CM
#define PHHAL_PCR_CLK_I2CM                                              \
    PH_FIELD(PCR_CLK_CFG_REG, IPCLOCK_I2CM_ENABLE, 0)                 \
    | PH_FIELD(PCR_CLK_CFG_REG, I2CM_CLOCK_GATING_ENABLE, 0)
#else /* NXPBUILD__PHHAL_I2CM */
#define PHHAL_PCR_CLK_I2CM      0
#endif /* NXPBUILD__PHHAL_I2CM */

#ifdef NXPBUILD__PHHAL_TIMER
#define PHHAL_PCR_CLK_TIMER     PH_FIELD(PCR_CLK_CFG_REG, CLOCK_TIMER_ENABLE, 1)
#else /* NXPBUILD__PHHAL_TIMER */
#define PHHAL_PCR_CLK_TIMER     0
#endif /* NXPBUILD__PHHAL_TIMER */

#ifdef NXPBUILD__PHHAL_CRC
#define PHHAL_PCR_CLK_CRC   PH_FIELD(PCR_CLK_CFG_REG, CLOCK_CRC_ENABLE, 1)
#else /* NXPBUILD__PHHAL_CRC */
#define PHHAL_PCR_CLK_CRC     0
#endif /* NXPBUILD__PHHAL_CRC */

#ifdef NXPBUILD__PHHAL_CLKGEN
#define PHHAL_PCR_CLK_CLKGEN    PH_FIELD(PCR_CLK_CFG_REG, CLOCK_CLKGEN_ENABLE, 1)
#else /* NXPBUILD__PHHAL_CLKGEN */
#define PHHAL_PCR_CLK_CLKGEN    0
#endif /* NXPBUILD__PHHAL_CLKGEN */

#ifdef NXPBUILD__PHHAL_RNG
#define PHHAL_PCR_CLK_RNG   PH_FIELD(PCR_CLK_CFG_REG, CLOCK_RNG_ENABLE, 1)
#else /* NXPBUILD__PHHAL_RNG */
#define PHHAL_PCR_CLK_RNG   0
#endif /* NXPBUILD__PHHAL_RNG */

#ifdef NXPBUILD__PHHAL_RF
#define PHHAL_PCR_CLK_RF   PH_FIELD(PCR_CLK_CFG_REG, CLOCK_CLIF_ENABLE, 1)
#else /* NXPBUILD__PHHAL_RF */
#define PHHAL_PCR_CLK_RF    0
#endif /* NXPBUILD__PHHAL_RF */

#define PHHAL_PCR_CLK_CONFIGURATION ( 0                                 \
    | PHHAL_PCR_CLK_CT                                                  \
    | PHHAL_PCR_CLK_HIF                                                 \
    | PHHAL_PCR_CLK_SPIM                                                \
    | PHHAL_PCR_CLK_I2CM                                                \
    | PHHAL_PCR_CLK_TIMER                                               \
    | PHHAL_PCR_CLK_CRC                                                 \
    | PHHAL_PCR_CLK_CLKGEN                                              \
    | PHHAL_PCR_CLK_RNG                                                 \
    | PHHAL_PCR_CLK_RF                                                  \
    | PH_FIELD(PCR_CLK_CFG_REG, EECTRL_SYS_GATING_ENABLE, 1)            \
    | PH_FIELD(PCR_CLK_CFG_REG, EECTRL_PF_GATING_ENABLE, 1)             \
    | PH_FIELD(PCR_CLK_CFG_REG, EECTRL_EEPROM_GATING_ENABLE, 1)         \
    | PH_FIELD(PCR_CLK_CFG_REG, CPU_CLKREQ_ENABLE, 1)                   \
    | PH_FIELD(PCR_CLK_CFG_REG, AUTOMATIC_CLOCKSTOP_AT_IDLE_ENABLE, 1)  \
    | PH_FIELD(PCR_CLK_CFG_REG, CLOCK_DFT_ENABLE, 0)                    \
    | PH_FIELD(PCR_CLK_CFG_REG, CLOCK_SWP_ENABLE, 0)                    \
    | PH_FIELD(PCR_CLK_CFG_REG, LFO_EN, 0)                              \
    | PH_FIELD(PCR_CLK_CFG_REG, LFO_TRIMM, 0)                           \
    | PH_FIELD(PCR_CLK_CFG_REG, EN_SWIO_CLK, 0)                         \
    | PH_FIELD(PCR_CLK_CFG_REG, SELECT_SCR_CTSEQ, 0)                    \
)


/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/** \addtogroup grpErr_HalPCR
 *
 * Error codes for PCR HAL
 *
 * @{
 *
 */
/**
 *  PMU/PCR error enums
 */
typedef enum
{
    /** \b 0x81 */ PH_ERR_PCR_NO_HIF_SELECTED = (PH_ERR_CUSTOM_BEGIN+0x01),    /**< No HIF is selected */
    /** \b 0x82 */ PH_ERR_PCR_WUC_PARAM_ERROR,      /**< Wakeup counter not set correctly */
    /** \b 0x83 */ PH_ERR_PCR_NO_SVDD_VBAT_ENABLED, /**< SVDD error */
    /** \b 0x84 */ PH_ERR_PCR_NO_TVDD_MON_ENABLED,  /**< TVDD monitoring is not enabled */
    /** \b 0x85 */ PH_ERR_PCR_NO_VBUS_MON_ENABLED,  /**< VBUS monitoring is not enabled */
    /** \b 0x86 */ PH_ERR_PCR_NOWAKEUP_ENABLED,     /**< None of the wakeup enabled */
    /** \b 0x87 */ PH_ERR_PCR_INVALID_GPIO_CONFIG,  /**< GPIO configuration is invalid */
    /** \b 0x88 */ PH_ERR_USB_SUSPEND_PREVENTED,    /**< Could not enter into the suspend */
    /** \b 0x89 */ PH_ERR_PCR_PVDD_ERROR,           /**< Wake up configuration is HIF
        or Gpio but PVDD is not OK */
    /** \b 0xFF */ PH_ERR_PCR_INVALID = 0xFF
}phhalPcr_ErrorCodes_t;
/** @} */
/**
 * Enumeration for the PCR/PMU system events
 */
typedef enum
{
    /** \b 0x000 */ E_SYSTEM_EVENT_WAITING             = 0, /**< Event is waiting */
    /** \b 0x001 */ E_SYSTEM_EVENT_RFLD_ACT            = (1 << 0), /**< Rfld activated event occurred */
    /** \b 0x002 */ E_SYSTEM_EVENT_VBUSP_MON_LOW       = (1 << 1), /**< VBUSP monitor low event occurred */
    /** \b 0x004 */ E_SYSTEM_EVENT_VBUSP_MON_HIGH      = (1 << 2), /**< VBUSP monitor high event occurred */
    /** \b 0x008 */ E_SYSTEM_EVENT_PVDD_CURLIM_ACT     = (1 << 3), /**< PVDD current limitation is active event */
    /** \b 0x010 */ E_SYSTEM_EVENT_DCDC_OVERLOAD       = (1 << 4), /**< DCDC overload event */
    /** \b 0x020 */ E_SYSTEM_EVENT_PVDD_THRESHOLD_LOW  = (1 << 5), /**< PVDD has fallen below the threshold */
    /** \b 0x040 */ E_SYSTEM_EVENT_PVDD_THRESHOLD_HIGH = (1 << 6), /**< PVDD has fallen below the threshold */
    /** \b 0x080 */ E_SYSTEM_EVENT_TXLDO_OVERCURRENT   = (1 << 7), /**< TXLDO over current occurred */
    /** \b 0x100 */ E_SYSTEM_EVENT_VCC_ILIM_ACT        = (1 << 8), /**< Vcc LDO current limiter */
    /** \b 0x200 */ E_SYSTEM_EVENT_USB_RESUME          = (1 << 9)  /**< USB resume event */
} phhalPcr_SystemEventType_t;

/**
 * Standby prevention status enum
 */
typedef enum
{
    /** \b 0x00 */ E_STANDBY_SUCCESS, /**< Standby applied */
    /** \b 0x01 */ E_HOSTCOMM_ONGOING, /**< Host communication ongoing */
    /** \b 0x02 */ E_WUC_EQUAL_ZERO, /**< Wake up counter is enabled but wake up counter value is equal to zero */
    /** \b 0x03 */ E_RFLDT_EVENT, /**< Either RF Level detector is activated as wake up source and RF
        level Detector is not enabled or RF Field is already present */
    /** \b 0x04 */ E_NO_PVDD_ON, /**< Host Interface is selected as wake up source and no PVDD is available */
    /** \b 0x05 */ E_PVDD_CURRENT_LIMITER, /**< Pvdd current limiter input has seen a 0 to 1 */
    /** \b 0x06 */ E_TEMP_SENSOR0_ERROR_LOW, /**< Temperature sensor 0 Error is selected as wake up source
        AND the temp_error_irq signal is low */
    /** \b 0x07 */ E_TEMP_SENSOR1_ERROR_LOW, /**< Temperature sensor 1 Error is selected as wake up source
        AND the temp_error_irq signal is low */
    /** \b 0x08 */ E_HIF_INTERFACE_ERROR, /**< HIF not selected */
    /** \b 0x09 */ E_GPIO_INTERRUPT, /**< Gpio gave an interrupt */
    /** \b 0x0A */ E_TVDD_ERROR, /**< If en_svdd_limiter is set but svdd_vbat is NOT set. */
    /** \b 0x0B */ E_CT_CARD_PRESENCE_SEEN, /**< Contact card presence is seen. */
    /** \b 0x0C */ E_AUX_INTERRUPT, /**< Ct uart int_aux pin has given an interrupt. */
    /** \b 0x0D */ E_CT_DEACTIVATION              = 13 /**< Ct deactivation is in progress. */
}phhalPcr_StandbyPrevReason_t;

/**
 * Different boot reasons
 */
typedef enum
{
    /** \b 0x00 */ E_BOOT_NONE, /**< No Boot */
    /** \b 0x01 */ E_STARTUP_POR, /**< Power on reset */
    /** \b 0x02 */ E_RFLDT_BOOT, /**< Boot due to RFLD detection */
    /** \b 0x03 */ E_WUC_CNT, /**< Boot due to wake up counter */
    /** \b 0x04 */ E_INT_AUX_BOOT, /**< Boot due to auxillary interrupt */
    /** \b 0x05 */ E_CT_PRESENCE, /**< Boot due to contact card presence seen */
    /** \b 0x06 */ E_I2C_ADDR, /**< Boot due to I2C address is seen on the bus */
    /** \b 0x07 */ E_SMBUS_ADDR, /**< Boot due to SMBUS address is seen on the bus */
    /** \b 0x08 */ E_SPI_ADDR, /**< Boot due to SPI transaction */
    /** \b 0x09 */ E_SWP_DET, /**< Boot due to SWP detection */
    /** \b 0x0A */ E_SOFT_RESET, /**< Boot due to soft reset */
    /** \b 0x0B */ E_WATCH_DOG, /**< Boot due to watch dog timer */
    /** \b 0x0C */ E_TVDD_MON, /**< Boot due to TVDD monitoring */
    /** \b 0x0D */ E_HIF_RESET, /**< Boot due to HIF is reset */
    /** \b 0x0E */ E_TEMP_SENSOR0, /**< Boot due to temperature has gone below the threshold */
    /** \b 0x0F */ E_TEMP_SENSOR1, /**< Boot due to temperature has gone below the threshold */
    /** \b 0x10 */ E_NO_PVDD, /**< Boot due to no PVDD */
    /** \b 0x11 */ E_PVDD_ILIM, /**< Boot due to PVDD current limitation */
    /** \b 0x12 */ E_GPIO_INT, /**< Boot due to gpio gave interrupt */
    /** \b 0x13 */ E_HSU_INT, /**< Boot due to HSU module interrupt */
    /** \b 0x14 */ E_DFT_RESET, /**< Boot due to DFT reset */
    /** \b 0x15 */ E_ACTIVE_HPD, /**< Boot due to active HPD */
    /** \b 0x16 */ E_VBUS_LOW /**< Boot due to VBUS has gone low */
}phhalPcr_BootReason_t;

/**
 * Enumeration for specifying a setting for reducing power consumption - A pad to be pulled down, or a module or
 * Clock is to be disabled.
 */
typedef enum
{
    /** \b 0x00000000 */ E_PD_GPIO1 = (1UL << 0), /**< pull down GPIO 1 */
    /** \b 0x00000001 */ E_PD_GPIO2 = (1UL << 1), /**< pull down GPIO 2 */
    /** \b 0x00000002 */ E_PD_GPIO3 = (1UL << 2), /**< pull down GPIO 3 */
    /** \b 0x00000004 */ E_PD_GPIO4 = (1UL << 3), /**< pull down GPIO 4 */
    /** \b 0x00000008 */ E_PD_GPIO5 = (1UL << 4), /**< pull down GPIO 5 */
    /** \b 0x00000010 */ E_PD_GPIO6 = (1UL << 5), /**< pull down GPIO 6 */
    /** \b 0x00000020 */ E_PD_GPIO7 = (1UL << 6), /**< pull down GPIO 7 */
    /** \b 0x00000040 */ E_PD_GPIO8 = (1UL << 7), /**< pull down GPIO 8 */
    /** \b 0x00000080 */ E_PD_GPIO9 = (1UL << 8), /**< pull down GPIO 9 */
    /** \b 0x00000100 */ E_PD_GPIO10 = (1UL << 9),  /**< pull down GPIO 10 */
    /** \b 0x00000200 */ E_PD_GPIO11 = (1UL << 10), /**< pull down GPIO 11 */
    /** \b 0x00000400 */ E_PD_GPIO12 = (1UL << 11), /**< pull down GPIO 12 */
    /** \b 0x00000800 */ E_PD_ATXA = (1UL << 12), /**< pull down ATX A */
    /** \b 0x00001000 */ E_PD_ATXB = (1UL << 13), /**< pull down ATX B */
    /** \b 0x00002000 */ E_PD_ATXC = (1UL << 14), /**< pull down ATX C */
    /** \b 0x00004000 */ E_PD_ATXD = (1UL << 15), /**< pull down ATX D */
    /** \b 0x00008000 */ E_PD_INT_AUX = (1UL << 16), /**< pull down INT_AUX */
    /** \b 0x00010000 */ E_PD_IO_AUX = (1UL << 17), /**< pull down IO_AUX */
    /** \b 0x00020000 */ E_PD_CLK_AUX = (1UL << 18), /**< pull down CLK_AUX */
    /** \b 0x00040000 */ E_PD_SPIM = (1UL << 19), /**< pull down All SPIM PADS - SCK, NSS, MOSI, MISO */
    /** \b 0x00080000 */ E_PD_IICM = (1UL << 20), /**< pull down All IICM PADS - SCL, SDA */
    /** \b 0x00100000 */ E_DISABLE_CT_CLK = (1UL << 21), /**< Disable CT Clock */
    /** \b 0x00200000 */ E_DISABLE_CLIF = (1UL << 22), /**< Disable CLIF modules and clocks */
    /** \b 0x00400000 */ E_PCR_ANA_TX_STANDBY_GSN = (1UL << 23), /**< GSN value in Standby mode made 0xF */
    E_APPLY_ALL_SETTNGS = 0x7FFFFFFFUL /* Apply all the settings for reducing Power consumption */
} phhalPcr_PwrDown_Setting_t;


/**
 * Structure for the temperature sensor settings
 */
typedef struct {
    /* WARNING: This structure is highly mapped with EEPROM. Do not change it. */
    /* The temperature values are taken from the PN548 code */
    uint8_t bUseTempSensor0;  /**< Flag to indicate to use temperature sensor 0 or not */
    uint8_t bUseTempSensor1;  /**< Flag to indicate to use temperature sensor 1 or not */
    uint8_t bLowTempTarget0;  /**< Possible values  0 ==> 135     1 ==> 130     2 ==> 125 3==> 120 */
    uint8_t bLowTempTarget1;  /**< Possible values  0 ==> 135     1 ==> 130     2 ==> 125 3==> 120 */
    uint8_t bHighTempTarget0; /**< Possible values  0 ==> 135     1 ==> 130     2 ==> 125 3==> 120 */
    uint8_t bHighTempTarget1; /**< Possible values  0 ==> 135     1 ==> 130     2 ==> 125 3==> 120 */
}phhalPcr_PwrTempConfig_t; /**< Temperature sensor settings */

/**
 * Structure for the wake up sources selection
 */
typedef struct {
    /* WARNING: This structure is highly mapped with EEPROM. Do not change it. */
    uint16_t wWakeUpTimerVal; /**< Timer value for the wake up in miliseconds */
    uint8_t bEnableHIFWakeup; /**< Flag to know the host interface wake up */
    uint8_t bSmBusAddrResp;   /**< Flag to know the SMBUS address */
    uint8_t bI2CAddr;         /**< I2C address if the wake up is configured for HIF */
    uint8_t bWakeUpTimer;     /**< Flag to enable the wake up timer as wake up source */
    uint8_t bWakeUpRfLdt;     /**< Flag to enable the RfLdt as wake up source */
    uint8_t bWakeUpTemp;      /**< Flag to enable temperature sensor 0/1 as wake up source when
                                   it goes below the lower threshold */
    uint8_t bWakeUpPvddLim;   /**< Flag to enable the Pvdd current limitation as wake up source */
    uint8_t bWakeUpCtPr;      /**< Flag to enable the CT presence as wake up source */
    uint8_t bWakeUpIntAux;    /**< Flag to enable the auxillary interrupt as wake up source */
    uint8_t bWakeUpTvddMon;   /**< Flag to enable the Tvdd monitoring as wake up source */
    uint8_t bWakeUpGpio;      /**< Flag to enable the Gpio as wake up source.
                                   Any of the gpio configured for wake up will wake up the chip */
    uint8_t blWakeupVbusLow;  /**< Flag to enable the Vbus low as wake up. */
} phhalPcr_WakeUpConfig_t; /**< Wake up sources selection */

/**
 * RfLd detection settings
 */
typedef struct phhalPcr_RfLdtParams {
    /* WARNING: This structure is highly mapped with EEPROM. Do not change it. */
    uint8_t bRfldRefLo;  /**< Higher Reference Value for RF Level Detector */
    uint8_t bRfldRefHi;  /**< Lower Reference Value for RF Level Detector*/
    uint8_t bEnableAdvancedRFLD;    /**< Should we used advanced RFLD Feature or normal RFLD Feature */
    uint8_t bPadding;    /**< Structure Padding */
}phhalPcr_RfLdtParams_t; /**< RfLd detection settings */
/**
 * TxAna register settings for standby
 */
typedef struct phhalPcr_TxAnaStandByConfig {
    /* WARNING: This structure is highly mapped with EEPROM. Do not change it. */
    uint32_t dwAnaTxStandByValue; /**< To hold CLIF standby GSN value selection */
    uint32_t dwAnaTxProtStandByValue; /**< To hold the CLIF configuration related to powerdown*/
}phhalPcr_TxAnaStandByConfig_t; /**< TxAna register settings for standby */

/**
 * PCR ISR Call back function signature.
 */
typedef void ( *pphhalPcr_CallbackFunc_t )( uint32_t );

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 *PCR  phhalPcr_GetBootReason_t function signature.
 *
 */
typedef phStatus_t (phhalPcr_GetBootReason_t)(uint32_t *pdwBootReason );
/**
 *PCR  phhalPcr_SaveContext_t function signature.
 */
typedef phStatus_t (phhalPcr_SaveContext_t)(uint32_t * pdwContextBuffer);
/**
 *PCR  phhalPcr_RestoreContext_t function signature.
 */
typedef phStatus_t (phhalPcr_RestoreContext_t)(uint32_t * pdwContextBuffer);

/**
 * @brief PCR HAL Initialization Function
 * This function initializes the global variable, temperature sensors and the required interrupts.
 *
 * @param[in] phhalPcr_PwrTempConfig Pointer to the temperature configuration structure
 * @param[in] psTxAnaStandByConfig Standby values to be applied for the TX ANA GSN value and the PROT value
 * @param[in] dwPwrDownSettingSelect 32 bit value bit-file created by ORing enums of type phhalPcr_PwrDown_Setting_t
 *            used to select which settings must be applied to reduce power consumption.
 * @return None

 *
 *
 */
phStatus_t phhalPcr_Init(phhalPcr_PwrTempConfig_t * phhalPcr_PwrTempConfig,
                     phhalPcr_TxAnaStandByConfig_t* psTxAnaStandByConfig,
                     uint32_t dwPwrDownSettingSelect);

/**
 * @brief Entering into the low power mode Function
 * This function is used to enter into the low power mode.
 * The low power mode can be either standby or suspend. The needed wake up sources can be enabled by the
 * wake up config structure.
 * @warning At least one wake up source has to be selected before calling this function.
 * @warning If any of the HIF interfaces selected as wake up source user must make sure that
 * the same HIF is selected before.
 *
 * @param[in] psRfLdtParams Parameters to be set during the RfLd wake up
 * @param[in] psWakeUpConfig Pointer to the structure of the wake up selection sources
 * @param[out] psPreventionReason Pointer to the standby/Suspend prevention reason in case
 *             entering into the low power mode fails
 * @return  Return Value
 * @warning This function will not return if it enters successfully into the standby
 * @retval    #PH_ERR_SUCCESS Entering the low power mode success
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 * @maskedret #PH_ERR_PCR_NO_HIF_SELECTED No HIF is selected, but wake up source selected as one of the HIF
 * @maskedret #PH_ERR_PCR_WUC_PARAM_ERROR  Wake up source selected is wake up timer but the wake up timer value is zero
 * @maskedret #PH_ERR_PCR_NO_TVDD_MON_ENABLED Wake up source selected is TVDD MON, but TVDD monitoring is not enabled
 * @maskedret #PH_ERR_PCR_NOWAKEUP_ENABLED No wake up source is selected
 */
phStatus_t phhalPcr_EnterLowPowerMode(phhalPcr_RfLdtParams_t* psRfLdtParams,
                                        phhalPcr_WakeUpConfig_t* psWakeUpConfig,
                                        phhalPcr_StandbyPrevReason_t* psPreventionReason);

/**
 * This function gets the boot reason
 * @param[out] pdwBootReason Boot reason
 * @return Reading the boot value
 */
#define phhalPcr_GetBootReason(pdwBootReason)           \
    ((phhalPcr_GetBootReason_t *) \
        PH_HAL_PCR_GETBOOTREASON_ADDR)\
        (pdwBootReason)

/**
 * @brief To save any context before entering into standby
 * This function is used to save context into the GPREG register.
 * @warning Only saving 7 double word possible, even though we have 8 GPREG's.
 *          GPREG0 is used for the boot reason internally.
 * @param[in] pdwContextBuffer Context buffer
 * @return Status of operation
 * @retval    #PH_ERR_SUCCESS Context saving is successful
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 */
#define phhalPcr_SaveContext(pdwContextBuffer)          \
    ((phhalPcr_SaveContext_t *) \
        PH_HAL_PCR_SAVECONTEXT_ADDR)\
        (pdwContextBuffer)

/**
 * @brief To restore any context after coming out of standby
 * @param[in] pdwContextBuffer Context buffer
 * @return Status of operation
 * @retval    #PH_ERR_SUCCESS Context restoring is successful
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been set correctly
 */
#define phhalPcr_RestoreContext(pdwContextBuffer)       \
    ((phhalPcr_RestoreContext_t *) \
        PH_HAL_PCR_RESTORECONTEXT_ADDR)\
        (pdwContextBuffer)

/**
 * @brief   This function registers the call back function to be called from the
 * ISR on the specified interrupts.
 * @param[in] pCallBackFunc Function to be called from the ISR.
 * @param[in] dwInterrupts Interrupt bits on which the call back function shall be called.
 * @return Status of the operation.
 * @retval    #PH_ERR_SUCCESS Call back registered is successful
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been sent correctly
 */
phStatus_t phhalPcr_RegCallBack(pphhalPcr_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts);

/**
 * PCR ISR handler
 */
void PCR_IRQHandler(void);
#endif /* NXPBUILD__PHHAL_PCR */
/** @} */
#endif /* PHHALPCR_H */
