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
 * Registers access.
 * $Author: Anish Ahammed (nxp86397) $
 * $Revision: 18643 $ (v05.19.00)
 * $Date: 2016-09-15 12:18:12 +0530 (Thu, 15 Sep 2016) $
 *
 * History:
 * CBE 02/03/2011 File creation.
 *
 * \deprecated These macros use full names + full mask names + full position
 *             names of the target resisters. And hence are some times prone to
 *             developer errors.  It is recommended to use helper macros as
 *             defined in \ref ph_Reg.h with PH_REG_* prefix
 */

#ifndef PH_REGISTERS_H
#define PH_REGISTERS_H

/** \defgroup phRegisters Registers access
 * \ingroup types
 * \brief Contains various macros to read/write HW 32 bits registers.
 *
 *
 * @{
 */

/*********************************************************************************************************************/
/*   INCLUDES                                                                                                        */
/*********************************************************************************************************************/
#ifndef PH_DATATYPES_H
#   include <ph_Datatypes.h>
#endif

/*********************************************************************************************************************/
/*   TOOLS                                                                                                           */
/*********************************************************************************************************************/

/**
 * \brief Create a boolean for testing
 * !(!(0)) -> !(TRUE) -> FALSE
 * !(!(non zero)) -> !(FALSE) -> TRUE
 */
#define PH_HALREG_BOOL(x) (!(!(x)))

/**
 * \brief Bit position shift
 */
#define PH_HALREG_BITMSK(bit) (1UL << (bit))

/*********************************************************************************************************************/
/*   REGISTERS  MANIPULATION                                                                                         */
/*********************************************************************************************************************/
/**
 * \brief Read a 32 bit value from the HW register
 */
#define PH_HALREG_GETREG(regAddr) ( (regAddr ## _R) | *((pReg32_t)(regAddr)) )
#define PH_HALREG_GETREG_X(regAddr) ( *((pReg32_t)(regAddr)) )

/**
 * \brief  Read a register and mask the requested fields
 */
#define PH_HALREG_GETFIELD(regAddr, regMask) ( (regAddr ## _R | PH_HALREG_GETREG_X(regAddr)) & ((uint32_t)(regMask)) )
#define PH_HALREG_GETFIELD_X(regAddr, regMask) ( (PH_HALREG_GETREG_X(regAddr)) & ((uint32_t)(regMask)) )

/**
 * \brief Read a register and mask the requested fields then shift to lsb
 */
#define PH_HALREG_GETFIELDSHIFT(regAddr, regMask, bitPos) ( ( (regAddr ## _R | PH_HALREG_GETREG_X(regAddr)) & ((uint32_t)(regMask)) ) >> (bitPos) )
#define PH_HALREG_GETFIELDSHIFT_X(regAddr, regMask, bitPos) ( ( (PH_HALREG_GETREG_X(regAddr)) & ((uint32_t)(regMask)) ) >> (bitPos) )

/**
 * \brief Write a 32 bit value to the HW register
 */
#define PH_HALREG_SETREG(regAddr,regValue) ((PH_HALREG_GETREG_X(regAddr)) = (regAddr ## _W | (uint32_t)(regValue)))
/* _X => Exclusive Skip read write check */
#define PH_HALREG_SETREG_X(regAddr,regValue) ((PH_HALREG_GETREG_X(regAddr)) = ((uint32_t)(regValue)))

/**
 * \brief  Write a 32 bit value to the HW register and preserve out-of-field bits
 */
#define PH_HALREG_SETFIELD(regAddr,regMask, regValue) ((PH_HALREG_GETREG_X(regAddr)) =(((regAddr ## _W | regAddr ## _R | PH_HALREG_GETREG_X(regAddr)) & (~((uint32_t)(regMask)))) | (((uint32_t)(regValue)) & ((uint32_t)(regMask)))))
#define PH_HALREG_SETFIELD_X(regAddr,regMask, regValue) ((PH_HALREG_GETREG_X(regAddr)) =((( PH_HALREG_GETREG_X(regAddr)) & (~((uint32_t)(regMask)))) | (((uint32_t)(regValue)) & ((uint32_t)(regMask)))))

/**
 * \brief  Write a 32 bit lsb positioned value to the HW register and preserve out-of-field bits
 */
#define PH_HALREG_SETFIELDSHIFT(regAddr,regMask,bitPos,regValue) ((PH_HALREG_GETREG_X(regAddr)) =(((regAddr ## _W | regAddr ## _R | PH_HALREG_GETREG_X(regAddr)) & (~((uint32_t)(regMask)))) | (((uint32_t)(regValue) << (bitPos)) & ((uint32_t)(regMask)))))
#define PH_HALREG_SETFIELDSHIFT_X(regAddr,regMask,bitPos,regValue) ((PH_HALREG_GETREG_X(regAddr)) =((( PH_HALREG_GETREG_X(regAddr)) & (~((uint32_t)(regMask)))) | (((uint32_t)(regValue) << (bitPos)) & ((uint32_t)(regMask)))))

/*********************************************************************************************************************/
/*   REGISTERS MANIPULATION AT A PROVIDED BIT POSITION                                                               */
/*********************************************************************************************************************/
/**
 * \brief Set bit at bitPos to 1
 */
#define PH_HALREG_SETBITN(regAddr,bitPos) ( (PH_HALREG_GETREG_X(regAddr)) |= (regAddr ## _W |regAddr ## _R | PH_HALREG_BITMSK(bitPos) ))
#define PH_HALREG_SETBITN_X(regAddr,bitPos) ( (PH_HALREG_GETREG_X(regAddr)) |= ( PH_HALREG_BITMSK(bitPos) ))

/**
 * \brief Set bit at bitPos to 1
 */
#define PH_HALREG_SETBITW(regAddr,bitPos) ( (PH_HALREG_GETREG_X(regAddr)) = (regAddr ## _W | regAddr ## _WO | PH_HALREG_BITMSK(bitPos) ))

/**
 * \brief Clear bit at bitPos (set to 0)
 */
#define PH_HALREG_CLEARBITN(regAddr,bitPos) ( (PH_HALREG_GETREG_X(regAddr)) &= (regAddr ## _W |regAddr ## _R | (~PH_HALREG_BITMSK(bitPos) )))
#define PH_HALREG_CLEARBITN_X(regAddr,bitPos) ( (PH_HALREG_GETREG_X(regAddr)) &= ((~PH_HALREG_BITMSK(bitPos) )))


/**
 * \brief Flip bit at bitPos
 */
#define PH_HALREG_FLIPBITN(regAddr,bitPos) ( (PH_HALREG_GETREG_X(regAddr)) ^= (regAddr ## _W |regAddr ## _R | PH_HALREG_BITMSK(bitPos) ))

/**
 * \brief Test bit at bitPos : if 1 return TRUE else return FALSE
 */
#define PH_HALREG_TESTBITN(regAddr,bitPos) PH_HALREG_BOOL( (regAddr ## _R | PH_HALREG_GETREG_X(regAddr)) & PH_HALREG_BITMSK(bitPos) )
#define PH_HALREG_TESTBITN_X(regAddr,bitPos) PH_HALREG_BOOL( ( PH_HALREG_GETREG_X(regAddr)) & PH_HALREG_BITMSK(bitPos) )


/**
 * \brief These macros report the lowest bit position set
 * \note  ASSUMPTION is that at least one 1-bit is set.
 */
#define PH_HALREG_LWSTBITPOS2(value)  (((value) & 0x1)    ? 0 : 1)
#define PH_HALREG_LWSTBITPOS4(value)  (((value) & 0x3)    ?    PH_HALREG_LWSTBITPOS2(value)  : (2  + PH_HALREG_LWSTBITPOS2(((value) >> 2))))
#define PH_HALREG_LWSTBITPOS8(value)  (((value) & 0xF)    ?    PH_HALREG_LWSTBITPOS4(value)  : (4  + PH_HALREG_LWSTBITPOS4(((value) >> 4))))
#define PH_HALREG_LWSTBITPOS16(value) (((value) & 0xFF)   ?    PH_HALREG_LWSTBITPOS8(value)  : (8  + PH_HALREG_LWSTBITPOS8(((value) >> 8))))
#define PH_HALREG_LWSTBITPOS32(value) (((value) & 0xFFFF) ?    PH_HALREG_LWSTBITPOS16(value) : (16 + PH_HALREG_LWSTBITPOS16(((value) >> 16))))



/** @} */


#endif /* PH_REGISTERS_H */
