/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors 2013,2014
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
 * This file implements the Api's for PMU
 *
 * Project:  PN7462AU
 * $Author: Sundaresan Swaminathan (nxp36013) $
 * $Revision: 17621 $ (v04.13.00)
 * $Date: 2016-01-29 12:13:35 +0530 (Fri, 29 Jan 2016) $
 *
 *
 */

#ifndef PHHALPMU_H
#define PHHALPMU_H

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_PMU
#include "ph_Status.h"

/** @addtogroup phhalPmu
 *
 * The Power Management Unit APIs
 *
 *
 * @{
 */
/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/**
 * TxLdo Wait time
 *
 * LFO                       Delay in EEPROM (us)
 * 1 (00)                    140-279
 * 2 (01)                    280-559
 * 4 (10)                    560-1119
 * 8 (11)                   >= 1120
 *
 * If delay < 140us then use default 280us
 *
 */
#define PHHAL_PMU_LFO1_TXLDO_MIN_US       140
#define PHHAL_PMU_LFO1_TXLDO_MAX_US       279
#define PHHAL_PMU_LFO2_TXLDO_MIN_US       280
#define PHHAL_PMU_LFO2_TXLDO_MAX_US       559
#define PHHAL_PMU_LFO4_TXLDO_MIN_US       560
#define PHHAL_PMU_LFO4_TXLDO_MAX_US       1119
#define PHHAL_PMU_LFO8_TXLDO_MIN_US       1120

/**
 * TXLDO clock division select. 00->LFO/1 , 01-> LFO/2 , 10->LFO/4 , 11->LFO/8.
 */
#define PHHAL_PMU_TXLDO_CLK_DIV1           00
#define PHHAL_PMU_TXLDO_CLK_DIV2           01
#define PHHAL_PMU_TXLDO_CLK_DIV4           02
#define PHHAL_PMU_TXLDO_CLK_DIV8           03

#define PHHAL_PMU_TXLDO_LFO_VALUE_MASK		   0xC000
#define PHHAL_PMU_TXLDO_WAITTIME_VALUE_MASK	   0x3FFF
#define PHHAL_PMU_TXLDO_LFO_VALUE_POS		   14

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/**
 * Enumeration for TXLDO configuration
 */
typedef enum
{
    /** \b 0x00 */ E_TXLDO_CONFIG_INVALID, /**< Invalid TXLDO Configuration */
    /** \b 0x01 */ E_TXLDO_CONFIG_1,       /**< TXLDO Configuration 1 battery powered only VUP_TX = VBUS = 2.3V to 5.5V */
    /** \b 0x02 */ E_TXLDO_CONFIG_2,       /**< TXLDO Configuration 2 powered by an external 5V, 5V on VUP_TX; VBUS = 2.3V to 5.5V */
    /** \b 0x03 */ E_TXLDO_CONFIG_3,       /**< TXLDO Configuration 3 5V on TVDD; VUP_TX = VBUS = 2.3V to 5.5V */
    /** \b 0x04 */ E_TXLDO_CONFIG_4        /**< TXLDO Configuration 4 5V directly on TVDD without TxLdo */
} phhalPmu_TxLdoConfig_t;

/**
 * Enumeration to select the TxLdo to start in different modes.
 */
typedef enum
{
    /** \b 0x00 */ E_FULL_POWER,    /**< Start the TXLDO in full power  */
    /** \b 0x01 */ E_LOW_POWER,     /**< Start the TXLDO in low power   */
    /** \b 0x02 */ E_DRIVER_INVALID /**< Invalid TXLDO Configuration    */
} phhalPmu_DriverConfig_t;

/**
 * Contact interface class options to start the Dcdc
 */
typedef enum
{
    /** \b 0x00 */ E_CLASS_A,  /**< Selection for classA 5V */
    /** \b 0x01 */ E_CLASS_B,  /**< Selection for classB 3.3V */
    /** \b 0x02 */ E_CLASS_C,  /**< Selection for classC 1.8V */
    /** \b 0x03 */ E_CLASS_INVALID /**< Invalid value. */
} phhalPmu_CtClassSelect_t;

/**
 * Enumeration for TVDD voltage selection
 */
typedef enum
{
    /** \b 0x00 */ E_TVDD_3M0_V = 0,   /**< TVDD 3V selection */
    /** \b 0x01 */ E_TVDD_3M3_V,       /**< TVDD 3.3V selection */
    /** \b 0x02 */ E_TVDD_3M6_V,       /**< TVDD 3.6V selection */
    /** \b 0x03 */ E_TVDD_4M5_V,       /**< TVDD 4.5V selection */
    /** \b 0x04 */ E_TVDD_4M75_V,      /**< TVDD 4.75V selection */
    /** \b 0x05 */ E_TVDD_MAX          /**< TVDD MAX selection */
} phhalPmu_TvddSel_t;

/**
 * Enumeration to select the source for the low power modes .
 */
typedef enum
{
    /** \b 0x00 */ E_TVDD_IN_SRC,       /**< Start the TXLDO in low power with TVDD supply  */
    /** \b 0x01 */ E_VUP_SRC,           /**< Start the TXLDO in low power with VUP supply  */
    /** \b 0x02 */ E_VBUS_SRC,          /**< Start the TXLDO in low power with VBUS supply (LP2) */
    /** \b 0x03 */ E_INVALID_SRC        /**< Invalid TXLDO Configuration    */
} phhalPmu_LowPower_TvddSrc_t;

/**
 * Enumeration to select the source for the TXLDO 5V monitor.
 */
typedef enum
{
    /** \b 0x00 */ E_5V_MON_SRC_VUP,               /**< Source for the monitor is VUP */
    /** \b 0x01 */ E_5V_MON_SRC_TVDD,              /**< Source for the monitor is TVDD */
    /** \b 0x02 */ E_5V_MON_SRC_INVALID            /**< Invalid source for monitor */
} phhalPmu_Monitor_Src_t;

/**
 * Enumeration to select the threshold for the TXLDO 5V monitor.
 */
typedef enum
{
    /** \b 0x00 */ E_5V_MON_THRES_3V3,                 /**< Threshold for the monitor is 3.3V */
    /** \b 0x01 */ E_5V_MON_THRES_4M0V,                /**< Threshold for the monitor is 4.0V */
    /** \b 0x02 */ E_5V_MON_THRES_INVALID              /**< Invalid Threshold for monitor */
} phhalPmu_Monitor_Threshold_t;

/** \addtogroup grpErr_HalPMU
 *
 * Error codes for PMU HAL
 *
 * @{
 */
/**
 *  PMU error enums
 */
typedef enum
{
    /** \b 0x81 */    PH_ERR_TXLDO_INVALID_TVDD_SEL = (PH_ERR_CUSTOM_BEGIN+0x01),    /**< TVDD Selection is not valid for this config */
    /** \b 0x82 */    PH_ERR_TXLDO_TVDD_NOT_OK,         /**< TXLDO Started but TVDD Not Ok */
    /** \b 0x83 */    PH_ERR_TXLDO_5VMON_NOT_OK,        /**< 5V not available  */
    /** \b 0x84 */    PH_ERR_TXLDO_ERROR,               /**< Error when the TXLdo does not start within the specified time */
    /** \b 0x85 */    PH_ERR_DCDCLDO_VBUSP_LOW,         /**< VBUSP Monitor returned low */
    /** \b 0x86 */    PH_ERR_PVDD_ERROR,                /**< PVDD could not be started */
    /** \b 0x87 */    PH_ERR_VBUSMON2_ERROR                 /**< Vbus 2 monitoring  */
} phhalPmu_ErrorCodes_t;

/** @} */

/**
 * PMU ISR Call back function signature.
 */
typedef void ( *pphhalPmu_CallbackFunc_t )( uint32_t );
/**
 * PMU DcdcLdoStart function signature.
 */
typedef phStatus_t (phhalPmu_DcdcLdoStart_t)(phhalPmu_CtClassSelect_t eClass);
/**
 * PMU DcdcLdoStop function signature.
 */
typedef phStatus_t (phhalPmu_DcdcLdoStop_t)(void);
/**
 * PMU PvddLdoStart function signature.
 */
typedef phStatus_t (phhalPmu_PvddLdoStart_t)(void);
/**
 * PMU PvddLdoStop function signature.
 */
typedef phStatus_t (phhalPmu_PvddLdoStop_t)(void);
/**
 * PMU PvddLdoLowPowee function signature.
 */
typedef phStatus_t (phhalPmu_PvddLdoLowPower_t)(void);

/* *****************************************************************************************************************
 * Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Function Prototypes
 * ***************************************************************************************************************** */
/**
 * This function initializes the TxLdo parameters.
 * @param[in] bUseTxLdo Flag to indicate to use internal TxLDo or External TxLDo
 *                      0 -> External TxLDo configuration
 *                      1 -> Internal TxLDo configuration
 * @param[in] eFullPowerTvddSel Tvdd Selection values, Selection of Tvdd supply for full power mode
 *                       0          3.0V
 *                       1          3.3V
 *                       2          3.6V
 *                       3          4.5V
 *                       4+         4.75V
 * @param[in] eLowPowerTvddSrc Tvdd source Selection values. Refer #phhalPmu_LowPower_TvddSrc_t
 *
 * @param[in] wWaitTime Time to wait for LDO to start in uSeconds.
 *                          0 -> Do not wait
                            Minimum value should be at least 250 uS, and Maximum value should be less than 500 uS
 * @param[in] bOverCurrentEn To enable the over current protection in Full power mode
 *                      0 -> Do not enable the over current protection
 *                      1 -> Enable the over current protection
 * @retval    #PH_ERR_SUCCESS
 * @maskedret #PH_ERR_INVALID_PARAMETER
 */
phStatus_t phhalPmu_TxLdoInit(uint8_t bUseTxLdo, phhalPmu_TvddSel_t eFullPowerTvddSel,
                              phhalPmu_LowPower_TvddSrc_t eLowPowerTvddSrc,
                              uint16_t wWaitTime, uint8_t bOverCurrentEn);
/**
 * This function starts the TxLdo in requested mode as specified by eConfig.
 * User should also provide the Tvdd selection value as well.
 * @param[in] eConfig Configuration to start the TxLdo in #E_FULL_POWER or #E_LOW_POWER.
 * @retval    #PH_ERR_SUCCESS
 * @maskedret #PH_ERR_INVALID_PARAMETER
 * @maskedret #PH_ERR_TXLDO_ERROR
 */
phStatus_t phhalPmu_TxLdoStart(phhalPmu_DriverConfig_t eConfig);

/**
 * This function stops the TxLdo
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalPmu_TxLdoStop(void);

/**
 * This function puts the TxLdo in standby
 * @param[in] bEnableStandBy To ut the TxLdo in stand by or not
 *                      0 -> Do not enable standby
 *                      1 -> Enable standby
 * @retval    #PH_ERR_SUCCESS
 */
phStatus_t phhalPmu_TxLdoStandby(uint8_t bEnableStandBy);

/**
 * This function enables or disables the 5V TXLDO monitor along with the monitor source and threshold.
 * @param[in] bEnableMonitor - Enables or disables the monitor
 *                              0 -> Disables the monitor
 *                              1 -> Enables the monitor
 * @param[in] eMonSource - Monitor source , please refer #phhalPmu_Monitor_Src_t for further details
 * @param[in] eMonThreshold - Monitor threshold , please refer #phhalPmu_Monitor_Threshold_t for further details
 * @retval    #PH_ERR_SUCCESS
 * @maskedret #PH_ERR_INVALID_PARAMETER
 */
phStatus_t phhalPmu_TxLdoMonitorEnable(uint8_t bEnableMonitor, phhalPmu_Monitor_Src_t eMonSource,
                                        phhalPmu_Monitor_Threshold_t eMonThreshold);

/**
 * This function enables the 5V monitor and returns if there is 5V available or not.
 * @retval    #PH_ERR_SUCCESS
 * @maskedret #PH_ERR_TXLDO_5VMON_NOT_OK
 */
phStatus_t phhalPmu_TxLdoMonitorCheck(void);

/**
 * This function starts the Dcdc in follower or doubler mode for each different Class selection depending on the VBUSP
 * monitor.
 * @param[in] eClass to provide the different class. Different classes possible are CLASS A or CLASS B or
 *                   CLASS C
 * @return Success if the Dcdc is started correctly.
 * @retval    #PH_ERR_SUCCESS
 * @maskedret #PH_ERR_DCDCLDO_VBUSP_LOW
 * @maskedret #PH_ERR_INVALID_PARAMETER
 */
#define phhalPmu_DcdcLdoStart(eClass)      \
    ((phhalPmu_DcdcLdoStart_t *) \
        PH_HAL_PMU_DCDCLDOSTART_ADDR)\
        (eClass)
/**
 * This function shuts down the Dcdc LDO and disables the Dcdc overload IRQ
 * @retval    #PH_ERR_SUCCESS
 */
#define phhalPmu_DcdcLdoStop()      \
    ((phhalPmu_DcdcLdoStop_t *) \
            PH_HAL_PMU_DCDCLDOSTOP_ADDR)()

/**
 * @maskedret #PH_ERR_DCDCLDO_VBUSP_LOW Dcdc is already in the doubler mode or the current activation is Class C
 * @retval    #PH_ERR_SUCCESS The CT can be activated again in the class B doubler mode
 */
phStatus_t phhalPmu_DcdcLdoCheck(void);
/**
 * This function starts the Pvdd LDO. It starts the Pvdd in soft start first and then again with the normal mode
 * Checks if the Pvdd is OK, it OK sets the PVDD threshold to 0V,enable the interrupt. If PVDD is not OK then
 * set the threshold to 1.8V,enable the interrupt and return error. Also sets the VBUSMON2 threshold and enables the
 * interrupt.
 *
 * @return successfully Pvdd LDO is turned on
 * @retval    #PH_ERR_SUCCESS
 * @maskedret #PH_ERR_PVDD_ERROR
 */
#define phhalPmu_PvddLdoStart()          \
    ((phhalPmu_PvddLdoStart_t *) \
        PH_HAL_PMU_PVDDLDOSTART_ADDR)()
/**
 * This function powers down the LDO in Pvdd LDO and turns off the monitors
 */
#define phhalPmu_PvddLdoStop()           \
    ((phhalPmu_PvddLdoStop_t *) \
        PH_HAL_PMU_PVDDLDOSTOP_ADDR)()
/**
 * This function puts the PvddLDO to LowPowerDown mode
 */
#define phhalPmu_PvddLdoLowPower()       \
    ((phhalPmu_PvddLdoLowPower_t *) \
        PH_HAL_PMU_PVDDLDOLOWPOWER_ADDR)()

/**
 * @brief   This function registers the call back function to be called from the
 * ISR on the specified interrupts.
 * @param[in] pCallBackFunc Function to be called from the ISR.
 * @param[in] dwInterrupts Interrupt bits on which the call back function shall be called.
 * @return Status of the operation.
 * @retval    #PH_ERR_SUCCESS Call back registered is successful
 * @maskedret #PH_ERR_INVALID_PARAMETER Parameter has not been sent correctly
 */
phStatus_t phhalPmu_RegCallBack(pphhalPmu_CallbackFunc_t pCallBackFunc, uint32_t dwInterrupts);


/**
 * This API is the ISR for PMU
 *
 */
PH_INTERRUPT(PMU_IRQHandler);
/** @}.*/
#endif /* NXPBUILD__PHHAL_PMU */
#endif /* PHHALPMU_H */
