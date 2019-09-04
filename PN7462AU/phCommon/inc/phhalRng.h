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
 * RNG HAL Implementation for RNG Module
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 */

/** \addtogroup phhalRng
 *
 * The Random Number Generator APIs
 *
 * This module implements RNG interface Flash code HAL, specific to RNG module.
 *
 * @warning If the build Macro \ref PHFL_PARAM_CHECK is less than \ref PH_PARAM_CHECK_VERBOSE, then the input parameters
 *                   will not be checked for erroneous values in the HAL API. In such a build configuration use the
 *                   API with caution and pass valid parameters only. Invalid parameters will likely result in
 *                   hardfaults or other failures.
 *  @{
 *
 *
 * \dot
 *      digraph "Sample Usage"{
 *          node [shape=oval, fontname="Helvetica", fontsize=10];
 *          End;
 *          Start;
 *
 *          node [shape=record, fontname="Helvetica", fontsize=10];
 *
 *      //Nodes
 *          phhalRng_Init [URL ="\ref phhalRng_Init"];
 *          phhalRng_GenerateRng [URL ="\ref phhalRng_GenerateRng"];
 *          phhalRng_DeInit [URL ="\ref phhalRng_DeInit"];
 *
 *
 *      //Logical Flow
 *          Start -> phhalRng_Init -> phhalRng_GenerateRng -> phhalRng_DeInit -> End;
 *
 *      //optionals
 *          edge [ style="dashed", color="green" ];
 *
 *      //error/abort cases
 *          edge [ style="dashed", color="red" ];
 *
 *      }
 * \enddot
 *
 *
 */

#ifndef PHHAL_RNG
#define PHHAL_RNG

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_RNG
#include "ph_Status.h"
#include "PN7462AU_ROM3.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/** The default TRNG feed timeout 0x26 translates to the default feed timeout of 15.2us*/
#define PHHAL_RNG_DEFAULT_TRNG_FEED_TIMEOUT       (0x26)
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
/**
 * Function Pointer to an API that Generates Random Number
 */
typedef phStatus_t (phhalRng_GenerateRng_t)(uint8_t *pbBuf, uint32_t dwLen);
/**
 * Function Pointer to an API that Stops the RNG Module
 */
typedef phStatus_t (phhalRng_DeInit_t)(void);
/**
 * Function Pointer to an API that Starts the RNG Module
 */
typedef phStatus_t (phhalRng_Init_t)(uint8_t bTrngFeedTimeout);

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */


/**
 * This API starts the RNG HW Module
 * @param[in] bTrngFeedTimeout Programmable wait time to release gated clocks feeding TRNG
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS on success
 *
 */
#define phhalRng_Init(bTrngFeedTimeout)        \
	((phhalRng_Init_t *)\
			PH_HAL_RNG_INIT_ADDR)\
			(bTrngFeedTimeout)

/**
 * This API generates a stream of random numbers
 *
 * @param[in] dwLen Defines the length of random buffer
 * @param[out] pbBuf  Defines the pointer to the buffer to store the random numbers
 * @return Status of the Operation
 *
 * @retval    #PH_ERR_SUCCESS If random numbers are successfully generated.
 * @maskedret #PH_ERR_INVALID_PARAMETER in case of wrong parameters.
 * @maskedret #PH_ERR_OPERATION_TIMEDOUT in case of IP Errors
 */
#define phhalRng_GenerateRng(pbBuf, dwLen)     \
	((phhalRng_GenerateRng_t *)\
			PH_HAL_RNG_GENERATERNG_ADDR)\
			(pbBuf, dwLen)

/**
 * This API stops the RNG HW Module
 *
 * @retval    #PH_ERR_SUCCESS on success
 */
#define phhalRng_DeInit()             \
	((phhalRng_DeInit_t *)\
			PH_HAL_RNG_DEINIT_ADDR)()

#endif /* NXPBUILD__PHHAL_RNG */
/** @} */
#endif /* PHHAL_RNG */
