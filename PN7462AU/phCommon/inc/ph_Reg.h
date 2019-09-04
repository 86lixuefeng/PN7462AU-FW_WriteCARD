/*
 *                  Copyright (c), NXP Semiconductors
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/** \file
 *
 * Utility and helper to read / write / set APB registers
 *
 * \note  These macros only work with the APB Registers.  It expects _MASK and _POS values
 *        to clearly set and get fields.
 *
 * $Author: Purnank G (ing05193) $
 * $Revision: 17073 $ (v04.13.00)
 * $Date: 2015-12-18 16:54:55 +0530 (Fri, 18 Dec 2015) $
 *
 * History:
 *
 * */

#ifndef PH_REG_H
#define PH_REG_H

#ifndef PH_DATATYPES_H
#   include <ph_Datatypes.h>
#endif

/**
 * \ingroup phRegisters
 * @{
 */

/** \def PH_REG_GET
 *
 * Get the value of REG_NAME Register */
/** \def PH_REG_GET_SC
 *
 * Same as #PH_REG_GET but Skip check if the register is read only or not */
#define PH_REG_GET(REG_NAME)                    \
    (REG_NAME ## _R | *((pReg32_t)(REG_NAME)))

#define PH_REG_GET_SC(REG_NAME)                 \
    (                 *((pReg32_t)(REG_NAME)))

/** \def PH_REG_SET
 *
 * Set the value of REG_NAME Register */
/** \def PH_REG_SET_SC
 *
 * Same as #PH_REG_SET but do not check if the register is read only or not */
#define PH_REG_SET(REG_NAME,REG_VAL)                            \
    (PH_REG_GET_SC(REG_NAME) = ( REG_NAME ## _W | (REG_VAL)))

#define PH_REG_SET_SC(REG_NAME,REG_VAL)                         \
    (PH_REG_GET_SC(REG_NAME) = (                  (REG_VAL)))

/** \def PH_REG_RESET
 * Reset the REG_NAME to it's reset values */
#define PH_REG_RESET(REG_NAME)                                          \
    (PH_REG_GET_SC(REG_NAME)                                            \
     = (REG_NAME ## _W | REG_NAME ## __RESET_VALUE ))

/** \def PH_REG_GET_FIELD
 * Get the value of REG_NAME Register's FIELD_NAME field. Shifting _is_ done */
/** \def PH_REG_GET_FIELD_NS
 * Same as #PH_REG_GET_FIELD but do not shift */
#define PH_REG_GET_FIELD(REG_NAME,FIELD_NAME)                          \
    (                        \
        ((REG_NAME ## _ ## FIELD_NAME ## _MASK)                         \
         & (REG_NAME ## _R | PH_REG_GET_SC(REG_NAME)))                  \
        >> (REG_NAME ## _ ## FIELD_NAME ## _POS))

#define PH_REG_GET_FIELD_NS(REG_NAME,FIELD_NAME)                        \
    (                     \
        ((REG_NAME ## _ ## FIELD_NAME ## _MASK)                         \
         & (REG_NAME ## _R | PH_REG_GET_SC(REG_NAME))))

/** \def PH_REG_GET_FIELD_NS_SC
 * Get the value of REG_NAME Register's FIELD_NAME field. Shifting _is_not_ done */
#define PH_REG_GET_FIELD_NS_SC(REG_NAME,REG_MASK)                          \
    (                        \
        ((PH_REG_GET_SC(REG_NAME)))                  \
        & (REG_MASK))

/** \def PH_REG_SET_FIELD
 * Set the value of REG_NAME Register's FIELD_NAME field. Shifting of VALUE _is_ done
 * Values of other BITS is retained
 */
#define PH_REG_SET_FIELD(REG_NAME,FIELD_NAME, VALUE)                    \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME))    \
         & (~(REG_NAME ## _ ## FIELD_NAME ## _MASK)))                   \
        | ((REG_NAME ## _ ## FIELD_NAME ## _MASK) \
           & (((uint32_t)(VALUE)) << (REG_NAME ## _ ## FIELD_NAME ## _POS )))))

#define PH_REG_SET_FIELD_NS(REG_NAME,FIELD_NAME, VALUE_SHIFTED)              \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME))    \
         & (~(REG_NAME ## _ ## FIELD_NAME ## _MASK)))                   \
        | ((REG_NAME ## _ ## FIELD_NAME ## _MASK) \
           & (VALUE_SHIFTED))))

/**
 * Set multiple fields.
 * \sa PH_FIELD
 */
#define PH_REG_SET_FIELDS_NS(REG_NAME,FIELDS_MASK, VALUE_SHIFTED)        \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME))    \
         & (~(FIELDS_MASK)))                                             \
        | ((FIELDS_MASK)                                                 \
           & ((VALUE_SHIFTED)))))

/**
 * Set multiple fields.
 * \sa PH_FIELD
 */
#define PH_REG_SET_FIELDS_NS_SC(REG_NAME,FIELDS_MASK, VALUE_SHIFTED)     \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((PH_REG_GET_SC(REG_NAME)) & (~(FIELDS_MASK)))                   \
        | ((FIELDS_MASK) & ((VALUE_SHIFTED)))))

/** \def PH_REG_SET_BIT
 * Set the value of REG_NAME Register's BIT_NAME bit.
 * Values of other BITS is retained
 */
#define PH_REG_SET_BIT(REG_NAME,BIT_NAME)                               \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME) ))  \
        | (REG_NAME ## _ ## BIT_NAME ## _MASK)))

/** \def PH_REG_SET_BITN
 * Set the value of REG_NAME Register's But Number bit.
 * Values of other BITS is retained
 */
#define PH_REG_SET_BITN(REG_NAME,BIT_NUMBER)                            \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME) ))  \
        | (0x01U << (BIT_NUMBER))))

/** \def PH_REG_SET_BITN_SC
 * Set the value of REG_NAME Register's But Number bit.
 * Values of other BITS is retained
 */
#define PH_REG_SET_BITN_SC(REG_NAME,BIT_NUMBER)                            \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        (PH_REG_GET_SC(REG_NAME) )  \
        | (0x01U << (BIT_NUMBER))))


/** \def PH_REG_SET_BIT_WO
 * Set the bit of REG_NAME Register's BIT_NAME.
 * REG_NAME should be write only.  Use this macro when read of REG_NAME is forbidden.
 */
#define PH_REG_SET_BIT_WO(REG_NAME,BIT_NAME)                           \
    (PH_REG_GET_SC(REG_NAME) =                                         \
        ((REG_NAME ## _WO) \
         | (REG_NAME ## _ ## BIT_NAME ## _MASK)))

/** Same as @ref PH_REG_SET_BIT_WO, but use number */
#define PH_REG_SET_BITN_WO(REG_NAME,BIT_NUMBER)                        \
    (PH_REG_GET_SC(REG_NAME) =                                         \
        ((REG_NAME ## _WO) \
         | (0x01U << (BIT_NUMBER))))

/** \def PH_REG_CLEAR_BIT
 * Clear the value of REG_NAME Register's BIT_NAME bit.
 * Values of other BITS is retained
 */

#define PH_REG_CLEAR_BIT(REG_NAME,BIT_NAME)                           \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME))    \
         & (~(REG_NAME ## _ ## BIT_NAME ## _MASK)))))

/** \def PH_REG_CLEAR_BITN
 * Clear the value of REG_NAME Register's BIT_NUMBER bit.
 * Values of other BITS is retained
 */

#define PH_REG_CLEAR_BITN(REG_NAME,BIT_NUMBER)                           \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W | PH_REG_GET_SC(REG_NAME))    \
         & (~(0x01U << (BIT_NUMBER))))))
/**
 * Same as @ref PH_REG_CLEAR_BITN, but the case where REG_NAME is not
 * part of standard registers of APIs
 */
#define PH_REG_CLEAR_BITN_SC(REG_NAME,BIT_NUMBER)                           \
    (PH_REG_GET_SC(REG_NAME) = (                                        \
        ((PH_REG_GET_SC(REG_NAME))    \
         & (~(0x01U << (BIT_NUMBER))))))

#define PH_REG_FLIP_BIT(REG_NAME,BIT_NAME)                               \
    (PH_REG_GET_SC(REG_NAME) ^= (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W |                              \
          REG_NAME ## _ ## BIT_NAME ## _MASK))))

#define PH_REG_FLIP_BITN(REG_NAME,BIT_NUMBER)                            \
    (PH_REG_GET_SC(REG_NAME) ^= (                                        \
        ((REG_NAME ## _R | REG_NAME ## _W |                              \
          0x01U << (BIT_NUMBER)))))

#define PH_REG_FLIP_BITN_SC(REG_NAME,BIT_NUMBER)                            \
    (PH_REG_GET_SC(REG_NAME) ^= (                                        \
        (( 0x01U << (BIT_NUMBER)))))

/** \def PH_REG_TEST_BIT
 * Check if the value of REG_NAME Register's BIT_NAME bit is set
 */

#define PH_REG_TEST_BIT(REG_NAME,BIT_NAME)                           \
    ( 0 != (                                        \
        ((REG_NAME ## _R | PH_REG_GET_SC(REG_NAME))    \
         & (REG_NAME ## _ ## BIT_NAME ## _MASK))))

#define PH_REG_TEST_BITN(REG_NAME,BIT_NUMBER)                           \
    ( 0 != (                                        \
        ((REG_NAME ## _R | PH_REG_GET_SC(REG_NAME))    \
         & (0x01U << (BIT_NUMBER)))))

#define PH_REG_TEST_BITN_SC(REG_NAME,BIT_NUMBER)                           \
    ( 0 != (                                        \
        ((PH_REG_GET_SC(REG_NAME))    \
         & (0x01U << (BIT_NUMBER)))))

/*
 * \def PH_FIELD
 *
 * User macro to get/set field parameter.  Shifting of VALUE _is_ done
 *
 * Sample usage:
 *
 * @code
 *
 *   PH_REG_SET_SC(PCR_CTRL_REG, 0
 *           | PH_FIELD(PCR_CTRL_REG,CLR_BOOT_REGS,1)
 *           | PH_FIELD(PCR_CTRL_REG,SUSPEND,1)
 *           | PH_FIELD(PCR_CTRL_REG,STANDBY,1)
 *           | PH_FIELD(PCR_CTRL_REG,SOFT_RESET,1)
 *           );
 *
 * @endcode
 *
 */
#define PH_FIELD(REG_NAME,FIELD_NAME,VALUE) \
        ((REG_NAME ## _ ## FIELD_NAME ## _MASK) & ((VALUE) << (REG_NAME ## _ ## FIELD_NAME ## _POS) ))


 /* @} */

#endif /* PH_REG_H */
