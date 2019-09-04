/*
 * phOsal_Cortex_Port.h
 *
 *  Created on: Mar 8, 2017
 *      Author: nxf18375
 */

#ifndef PHOSAL_CORTEX_PORT_H_
#define PHOSAL_CORTEX_PORT_H_

/*
 * Below Defines are applicable for Cortex Series i.e. Cortex-M0, Cortex_M3 and Cortex-M4
 * Cortex-M3: LPC1769
 * Cortex-M4: K82
 * Cortex-M0: PN640
 */


/** \ingroup    CMSIS_core_register
    \defgroup   CMSIS_core_base     Core Definitions
    \brief      Definitions for base addresses, unions, and structures.
  @{
 */

/* Memory mapping of Cortex-M Hardware */
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address              */
#define SysTick             ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct       */


/* IO definitions (access restrictions to peripheral registers) */
/**
    \brief IO definitions to specify the access to peripheral variables.
*/

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */


/** \brief  Structure type to access the System Timer (SysTick).
 */
typedef struct
{
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO uint32_t LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IO uint32_t VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __I  uint32_t CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
} SysTick_Type;


#endif /* PHOSAL_CORTEX_PORT_H_ */
