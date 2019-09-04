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
 * This file contains Apis for S9  synchronous card.
 *
 * Project:  PN7462AU
 *
 * $Date: 2015-05-04 16:39:24 +0530 (Mon, 04 May 2015) $
 * $Author: Purnank H G (ing05193) $
 * $Revision: 12183 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SYNCCT

#include "ph_Status.h"

#include "phhalSyncCt_S9.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_SYNC_CT_MAX_BUFFER_SIZE 512

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static uint8_t gbPinVerify;
/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* This function is used to receive byte from card.*/
static uint32_t phhalSyncCt_S9_RcvByte(void);
/* This function is used to read bit from card.*/
static uint8_t  phhalSyncCt_S9_Readbit(void);
/* This function is used to send command to card.*/
static void phhalSyncCt_S9_SendCommand(uint8_t bCodeCommand, uint32_t dwCardAddress ,
                                                                uint32_t dwOffset ,uint8_t *pbDataBuffer);

/* This function is used to read protected bit from card.*/
static uint8_t phhalSyncCt_S9_ReadbitPB(uint8_t *pbDataBuffer ,uint32_t dwCardAddress );


/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * This function is used to power up S9 card including CtIP initializationa and card activation.
 */
phStatus_t phhalSyncCt_S9Powerup(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,
                                                                        uint8_t bSlewRateConfig ,uint8_t *pbAtrBuffer)
{
    phStatus_t eStatus = PH_ERR_FAILED;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbAtrBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif

    eStatus = phhalSyncCt_Init(bPullupSetStatus, bCardPresConnectorStatus, bSlewRateConfig);

    /* wait for connector type stability.*/
    (void)phUser_Wait(10000);

    eStatus =phhalSyncCt_CardActivate(pbAtrBuffer ,E_TYPE1);
    if(eStatus != PH_ERR_SUCCESS)
    {
        if(eStatus == PH_ADD_COMPCODE(PH_ERR_SYNC_CT_ACTIVATION_FAILED,PH_COMP_HAL_SYNC_CT))
        {
            eStatus =phhalSyncCt_CardActivate(pbAtrBuffer ,E_TYPE2);
        }
        return eStatus;
    }
    return PH_ERR_SUCCESS;
}
/**
 * This function is used to read byte in 8 bit mode(unprotected mode).
 */
phStatus_t phhalSyncCt_S9_Read8bits(uint32_t dwCardAddress, uint32_t dwByteCount ,uint8_t *pbDataBuffer)
{
    uint32_t bCount ;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbDataBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif

    if (dwByteCount > PHHAL_SYNC_CT_MAX_BUFFER_SIZE)
    {
        return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_BUFFER_TOO_SMALL,PH_COMP_HAL_SYNC_CT);
    }
    phhalSyncCt_S9_SendCommand(0x0E, dwCardAddress, 0 ,pbDataBuffer);
    for (bCount=0; bCount<dwByteCount;bCount++)
    {
        pbDataBuffer[bCount] = (uint8_t )phhalSyncCt_S9_RcvByte();
    }
    phhalSyncCt_SetReset(1);
    phUser_Wait(10000);
    return  PH_ERR_SUCCESS;
}


/**
 * This function is used to read byte in 9 bit mode(protected mode).
 */

phStatus_t phhalSyncCt_S9_Read9bits(uint32_t dwCardAddress, uint32_t dwByteCount ,uint8_t *pbDataBuffer)
{
    uint32_t dwLoopCount ;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbDataBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif
    if (dwByteCount > (PHHAL_SYNC_CT_MAX_BUFFER_SIZE>>1))
    {
        return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_BUFFER_TOO_SMALL,PH_COMP_HAL_SYNC_CT);
    }
    phhalSyncCt_S9_SendCommand(0x0C, dwCardAddress, 0 ,pbDataBuffer);
    for (dwLoopCount=0;dwLoopCount != (dwByteCount<<1); dwLoopCount++)
    {
        pbDataBuffer[dwLoopCount] = (uint8_t)phhalSyncCt_S9_RcvByte();
        if (phhalSyncCt_S9_Readbit())
            pbDataBuffer[dwLoopCount] = 0x01;
         else
             pbDataBuffer[dwLoopCount] = 0x00;
    }
    phhalSyncCt_SetReset(1);
    return PH_ERR_SUCCESS;
}


/**
 * This external function is use to receive a byte.
 **/
static uint32_t phhalSyncCt_S9_RcvByte(void)
{
   uint8_t bcharacter;
   uint8_t bCount = 0x00;

   bcharacter = 0;
   phhalSyncCt_SetTRMode(0);                    /*select receive mode.*/
   (void)phUser_Wait(5000);
   for (bCount=0; bCount<8; bCount++)
   {
      bcharacter = bcharacter>>1;
      if (phhalSyncCt_S9_Readbit() == 1)
         bcharacter += (uint8_t)(0x80);            /*input lsb first.*/
   }
   return(bcharacter);
}
/**
 * This external function is use to read bit.
 **/
static uint8_t  phhalSyncCt_S9_Readbit(void)
{
    uint8_t bReceivedbit = 0x00;
    /*DESCRIPTION : This function is used to read a bit in URR^0.*/
    phhalSyncCt_Send_Nbclocks(0x01);
    phhalSyncCt_SetReset(0);
    (void)phUser_Wait(1000);
    phhalSyncCt_GetSyncBit(&bReceivedbit);
    return(bReceivedbit);
}
/**
 * This function is used to send command to card .
 * @param bCodeCommand -> Command Byte
 * @param dwCardAddress -> Address for card .
 * @param dwOffset -> this indicates offset for user buffer.
 * @param pbDataBuffer -> points to user buffer.
 */
static void phhalSyncCt_S9_SendCommand(uint8_t bCodeCommand, uint32_t dwCardAddress ,
                                                    uint32_t dwOffset ,uint8_t *pbDataBuffer)
{

    uint8_t byte;
    phhalSyncCt_SetReset(1);
    (void)phUser_Wait(10000);
    byte = (uint8_t)(dwCardAddress>>8);
    byte = (byte<<6) + bCodeCommand;
    phhalSyncCt_TransmitByte(byte);                     /*byte 1 : command.*/
    byte = (uint8_t)(dwCardAddress);
    phhalSyncCt_TransmitByte(byte);                     /* byte 2 : address.*/
    phhalSyncCt_TransmitByte(pbDataBuffer[dwOffset]);   /* byte 1 : data (no effect in case of read)*/
    phhalSyncCt_SetReset(0);
    (void)phUser_Wait(10000);
    phhalSyncCt_SetTRMode(0);
    (void)phUser_Wait(4000);
}
/**
 *This external function is used to send write data command in 9 bits mode.
 *@param -> bWriteType -> specify for type of write.
 *          - 0x31 : write 9 bits (protected mode)
 *          - 0x33 : write 8 bits (unprotected mode)
 *@param -> dwOffset :  offset position in auxiliary ram
 *@param  -> dwByteCount : Number of byte count to be write in auxillary ram from offset position.
 */
phStatus_t phhalSyncCt_S9_Write(uint8_t bWriteType, uint32_t dwOffset,uint32_t dwCardAddress,
                                                        uint32_t dwByteCount ,uint8_t *pbDataBuffer)
{

   uint32_t dwLoopCount;
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbDataBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif

   if (gbPinVerify == 0)
   {
      /* Writing attempt without pin code verification done.*/
       return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_WRITING_WITHOUT_PIN_VERIFIED,PH_COMP_HAL_SYNC_CT);
   }

   for (dwLoopCount=0; dwByteCount!=0; dwByteCount--,dwLoopCount++)
   {
      if (phhalSyncCt_S9_ReadbitPB(pbDataBuffer ,(dwCardAddress + dwLoopCount)) == 0)
      {
         /*  if PB bit is 0 then protected byte else unprotected.*/
          return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_PROTECTED_BYTE,PH_COMP_HAL_SYNC_CT);
      }

      /* write data stocked in buffer exchange.*/
      phhalSyncCt_S9_SendCommand(bWriteType, (dwCardAddress+dwLoopCount), (dwOffset+dwLoopCount) , pbDataBuffer);
      phhalSyncCt_Send_Nbclocks(203);

      /* read and compare the written byte.*/
      phhalSyncCt_S9_SendCommand(0x0E, (dwCardAddress+dwLoopCount), dwOffset ,pbDataBuffer);

      if (phhalSyncCt_S9_RcvByte() != pbDataBuffer[dwOffset+dwLoopCount])
      {
          phhalSyncCt_SetClock(0);

         phhalSyncCt_SetReset(1);
         /* Write Error.*/;
         return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_WRITE_ERROR,PH_COMP_HAL_SYNC_CT);
      }
   }
   return PH_ERR_SUCCESS;
}

/**
 *  DESCRIPTION : This external function is use to verify PIN CODE.
 **/
phStatus_t phhalSyncCt_S9_VerifyPin(uint8_t *pbPinCodeBuffer)
{

    phStatus_t eStatus = PH_ERR_SYNC_CT_PIN_NOT_VERIFIED ;
    uint8_t bReceivedByte = 0x00;
    uint8_t bDataBuffer[5] = {0x00, 0x00,0x00,0x00,0x00};
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbPinCodeBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif
    do{
        /*check PB of the error counter.*/
        if (phhalSyncCt_S9_ReadbitPB(bDataBuffer ,0x3FD) == 0)
        {
            /*error counter protected.*/

            eStatus =  PH_ERR_SYNC_CT_ERROR_COUNTER_PROTECTED;
            break;
        }

        phhalSyncCt_SetReset(1);             /* write error counter with mask.*/
        (void)phUser_Wait(10000);
        phhalSyncCt_TransmitByte(0xF2);               /* send byte 1.*/
        phhalSyncCt_TransmitByte(0xFD);               /* send byte 2 : address 253.*/
        phhalSyncCt_TransmitByte(0x01);  /* send byte 3 : bit mask.*/
        phhalSyncCt_SetReset(0);

        phhalSyncCt_Send_Nbclocks(103);

        phhalSyncCt_SetReset(1);             /* compare PIN1.*/
        (void)phUser_Wait(10000);
        phhalSyncCt_TransmitByte(0xCD);
        phhalSyncCt_TransmitByte(0xFE);
        phhalSyncCt_TransmitByte(pbPinCodeBuffer[0]);
        phhalSyncCt_SetReset(0);
        (void)phUser_Wait(40);
        phhalSyncCt_Send_Nbclocks(2);

        phhalSyncCt_SetTRMode(0);                    /* ISO-UART in reception mode.*/
        (void)phUser_Wait(40);
        phhalSyncCt_GetSyncBit(&bReceivedByte);
        if(bReceivedByte == 0x01)
        break;                     /* verify pin error.*/

        phhalSyncCt_SetReset(1);             /*compare PIN2.*/
        (void)phUser_Wait(10000);
        phhalSyncCt_TransmitByte(0xCD);
        phhalSyncCt_TransmitByte(0xFF);
        phhalSyncCt_TransmitByte(pbPinCodeBuffer[1]);
        phhalSyncCt_SetReset(0);
        phhalSyncCt_Send_Nbclocks(2);

        phhalSyncCt_SetTRMode(0);                   /*ISO-UART in reception mode.*/
        phhalSyncCt_GetSyncBit(&bReceivedByte);
        if( bReceivedByte == 0x01)
        break;                     /*verify pin error.*/

        phhalSyncCt_SetReset(1);            /* write error counter.*/
        (void)phUser_Wait(10000);
        phhalSyncCt_TransmitByte(0xF3);
        phhalSyncCt_TransmitByte(0xFD);
        phhalSyncCt_TransmitByte(0xFF);
        phhalSyncCt_SetReset(0);
        phhalSyncCt_Send_Nbclocks(103);

        phhalSyncCt_S9_SendCommand(0x0E,0x3FD,0,pbPinCodeBuffer);     /*verify error counter has been erased.*/
        bReceivedByte = (uint8_t)phhalSyncCt_S9_RcvByte();
        if (bReceivedByte != 0xFF)
        {
            eStatus = PH_ERR_SYNC_CT_PIN_CODE_ERROR;
            break;
        }
        eStatus = PH_ERR_SUCCESS;
        gbPinVerify = 1;
    }while(0);
    return PH_ADD_COMPCODE(eStatus,PH_COMP_HAL_SYNC_CT);
}

/**
 * DESCRIPTION : This external function is use to compare PIN CODE.
 * @param - dwCardAddress -> Address of byte to compare.
 * @param - dwAuxRamAddress -> offset position of data to compare in auxiliary ram.
 * @param - pbDataBuffer -> points to data buffer having bytes to compare.
 */

phStatus_t phhalSyncCt_S9_Compare(uint32_t dwCardAddress,uint32_t dwOffset ,uint8_t *pbDataBuffer)
{
#if PHFL_PARAM_CHECK >= PH_PARAM_CHECK_VERBOSE
        /*Check for the null pointers*/
        if (pbDataBuffer == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER,PH_COMP_HAL_SYNC_CT);
        }
#endif
   if (phhalSyncCt_S9_ReadbitPB(pbDataBuffer,dwCardAddress) == 0)
   {
      /* protected bit already set.*/
      return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_PROTECTED_BIT_ALREADY_SET,PH_COMP_HAL_SYNC_CT);
   }

   phhalSyncCt_S9_SendCommand (0x30, dwCardAddress, dwOffset ,pbDataBuffer);
   while(1)
   {
      if (phhalSyncCt_S9_Readbit() == 0)
      break;
   }
   if (phhalSyncCt_S9_ReadbitPB(pbDataBuffer,dwCardAddress) == 1)
   {
      /*verify pin error.*/
      return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_PIN_NOT_VERIFIED,PH_COMP_HAL_SYNC_CT);
   }
   return PH_ERR_SUCCESS;
}
/**
 *DESCRIPTION : This external function is use to read bit PB.
 * @param - dwCardAddress -> Address of byte to compare.
 */
static uint8_t phhalSyncCt_S9_ReadbitPB(uint8_t *pbDataBuffer ,uint32_t dwCardAddress)

{
   uint8_t bCount;

   phhalSyncCt_SetTRMode(0);
   phhalSyncCt_S9_SendCommand(0x0C, dwCardAddress, 0,pbDataBuffer);
   for (bCount=0; bCount<8; bCount++)
      phhalSyncCt_Send_Nbclocks(0x01);

   if (phhalSyncCt_S9_Readbit() == 0)
      return(0);
   return(1);
}

#endif /* NXPBUILD__PHHAL_SYNCCT */
