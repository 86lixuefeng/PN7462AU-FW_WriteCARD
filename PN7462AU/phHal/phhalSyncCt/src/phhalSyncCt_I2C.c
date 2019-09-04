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
 * This file contains all apis for I2C profile synchronous card.
 *
 * Project:  PN7462AU
 *
 * $Date: 2016-11-30 11:55:53 +0530 (Wed, 30 Nov 2016) $
 * $Author: Stevenson James (NXP66090) $
 * $Revision: 18761 $ (v04.13.00)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "ph_Datatypes.h"
#include "ph_NxpBuild.h"

#ifdef NXPBUILD__PHHAL_SYNCCT

#include "ph_Status.h"
#include "phhalSyncCt_I2C.h"
#include "ph_Reg.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHHAL_SYNC_CT_MAX_BUFFER_SIZE 512
#define PHHAL_CT_VCC5  0x00

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */
typedef void (*pFunctPtr)(void) ;
/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
PH_NOINIT static uint8_t gbI2CTxRxState; /* used to tell about Transmit or receive state.*/
PH_NOINIT static uint8_t gbI2CTxRxActive; /* used to tell about I2C communication is going to happen.*/
PH_NOINIT static uint32_t gdwI2CDataCount; /* Used to indicate number of byte to send or receive from I2C Card.*/
PH_NOINIT static uint8_t gbI2CExtended; /* Used ot tell about I2C card supports or not extended mode.*/
PH_NOINIT static uint8_t gbTypeRxTx;     /*Used to tell whcih type of operation is going to happen.*/
PH_NOINIT static uint8_t gbSlaveAddress; /* Used to store slave address for communication.*/
PH_NOINIT static uint8_t gbI2CNoAck; /* Used to tell status for ack from card.*/
PH_NOINIT static uint8_t *gpbSyncCtBuffer;  /* pointer to static buffer ,which we can use for data or receive buffer whichever
                                        is going to use.*/

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* This function is used as state machine for transmit or receive process.*/
void static phhalSyncCt_I2CStateMachine(uint8_t bSendReceive ,uint8_t *pbDataBuffer);

/*This function writes string of bytes ot card.*/
static phStatus_t  phhalSyncCt_I2CWriteStringByte(uint8_t *pbDataBuffer ,uint32_t dwDataNumber);

/* This function is used to get slave address stored in user data buffer for I2C communication.*/
static uint8_t  phhalSyncCt_GetSlaveAddress(uint8_t *pbDataBuffer);

/* This function is used to read bytes from random address location provided by user buffer.*/
static phStatus_t phhalSyncCt_I2CReadRandom(uint8_t *pbDataBuffer ,uint8_t *pbReceiveBuf);

/* This function is used to get slave address from provided information by user data buffer.*/
static uint8_t  phhalSyncCt_GetSlaveAddress(uint8_t *pbDataBuffer);

/* This function is used to read bytes from Current address location.*/
static phStatus_t  phhalSyncCt_I2CReadFromCurrentAddress(uint8_t *pbDataBuffer,uint8_t *pbReceiveBuf);

/* This function is used to write bytes at card address location.*/
static uint8_t phhalSyncCt_I2CWriteByte(uint8_t bTransmitByte);

/* This function is used to read bytes from card.*/
static uint8_t phhalSyncCt_I2CReadbyte(uint8_t bLastbyte);

/* This function is used to get acknowledgment from card for read or write operation start.*/
static uint8_t phhalSyncCt_I2CTxSlv_Addr(uint8_t byte, uint8_t bRWN);

/* This function is used to send header to synchronous card.*/
static uint8_t phhalSyncCt_I2CTxHeader(uint8_t *pbDataBuffer);

/* This function is used to start condition for communication with synchronous card.*/
static void phhalSyncCt_I2CTxStart(void);

/* This function is used to send data to synchronous card.*/
static void phhalSyncCt_I2CTxDatas(void);

/* This function is used to decide whether to transmit or receive from card.*/
static void phhalSyncCt_I2CRxSlv_TxAddr(void);

/* This function is used to receive data to synchronous card.*/
static void phhalSyncCt_I2CRxDatas(void);

/* This function is used to stop condition for communication with synchronous card.*/
static void phhalSyncCt_I2CTxStop(void);

/* This tabel consists I2C function in sequence for transmit or receive data from card.*/
static const
pFunctPtr I2C_Table[]={  &(phhalSyncCt_I2CTxStart),            /* state 0.*/
        &(phhalSyncCt_I2CTxDatas),            /* state 1.*/
        & (phhalSyncCt_I2CRxSlv_TxAddr),      /* state 2.*/
        & (phhalSyncCt_I2CRxDatas),            /* state 3.*/
        & (phhalSyncCt_I2CTxStop)  };           /* state 4.*/


/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
/**
 * This function is used to power up I2C card including CtIP initializationa and card activation.
 */
phStatus_t phhalSyncCt_I2CPowerup(uint8_t bPullupSetStatus,uint8_t bCardPresConnectorStatus,uint8_t bSlewRateConfig)
{
    phhalSyncCt_Init(bPullupSetStatus, bCardPresConnectorStatus, bSlewRateConfig);

    /* wait for connector type stability.*/
    (void)phUser_Wait(10000);

    /* Selecting 5 volt for synchronous card activation.because in spec operating voltage is written as 5v.*/
    PH_REG_SET_FIELD(CT_PCR_REG, VCCSEL1_VCCSEL0 , PHHAL_CT_VCC5);

    PH_REG_SET_BIT(CT_PCR_REG, START);
    /* wait for To stable Vcc .*/
    (void)phUser_Wait(5000);
    phhalSyncCt_SetClock(1);
    return PH_ERR_SUCCESS;
}
/**
 * This Api is used to Send Data to card through interface device.
 */
phStatus_t phhalSyncCt_I2CSendData(uint8_t *pbDataBuffer ,uint32_t dwDataNumber)
{
    phStatus_t phStatus= PH_ERR_FAILED;
    phStatus = phhalSyncCt_I2CWriteStringByte(pbDataBuffer, dwDataNumber);
    if(PH_ERR_SUCCESS != phStatus)
    {
        return phStatus;
    }
    return PH_ERR_SUCCESS;
}

static phStatus_t  phhalSyncCt_I2CWriteStringByte(uint8_t *pbDataBuffer ,uint32_t dwDataNumber)
{
    gbI2CTxRxState = E_STATE_0;
    gbI2CTxRxActive = 0x01;
    gdwI2CDataCount = dwDataNumber;
    if(gdwI2CDataCount > PHHAL_SYNC_CT_MAX_BUFFER_SIZE)
    {
        return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL,PH_COMP_HAL_SYNC_CT);
    }
    phhalSyncCt_I2CStateMachine(0,pbDataBuffer);

    if (gbI2CNoAck)
       return PH_ERR_FAILED;

    return PH_ERR_SUCCESS;
}
/**
* DESCRIPTION : This function start state machine with value given in parametter:
*                  0 means send data.
*                  1 means receive data.
*/

void static phhalSyncCt_I2CStateMachine(uint8_t bSendReceive ,uint8_t *pbDataBuffer)
{
    gbTypeRxTx = bSendReceive;
    gpbSyncCtBuffer = pbDataBuffer;
    do
    {
        if(gbI2CTxRxState <= E_LAST_STATE)
            I2C_Table[gbI2CTxRxState]();
        else
        {
            gbI2CTxRxActive = 0;
            gbI2CTxRxState = E_STATE_0;
        }
    } while (gbI2CTxRxActive);
}
/**
 DESCRIPTION : Slave address = pbDataBuffer[0]
*               I2C Add   	 = pbDataBuffer[1]
*               Data nr      = pbDataBuffer[2]
*               Once this routine is over, the data read are
*               added to the first place of buffer:
*
*               Read datas at the current address in not extended mode
*               This function return a booleen if all datas have been
*               read corectly.
 *
 */

phStatus_t phhalSyncCt_I2CRequestData(uint8_t *pbReceiveBuf ,uint8_t *pbDataBuffer)
{
    phStatus_t phStatus = PH_ERR_FAILED;
    gbI2CExtended  = 0;

    phhalSyncCt_I2CWriteStringByte(pbDataBuffer, 2);	// Write [I2C add] [Memory Add] to set the memory read start address

    gbSlaveAddress = phhalSyncCt_GetSlaveAddress(pbDataBuffer);
    phStatus = phhalSyncCt_I2CReadFromCurrentAddress(pbDataBuffer ,pbReceiveBuf);
    if (PH_ERR_SUCCESS != phStatus )
    {
        return phStatus;
    }
    /*all data read.*/
    return PH_ERR_SUCCESS;
}

static uint8_t  phhalSyncCt_GetSlaveAddress(uint8_t *pbDataBuffer)
{
     gbSlaveAddress = pbDataBuffer[0] ;
     if(!(gbI2CExtended))
     {
         gbSlaveAddress += 2*(pbDataBuffer[1] & 0x01);
     }
     return gbSlaveAddress;
}

/**
* DESCRIPTION : This function read bytes from i2c synchronous card
*               in a random way.
*               The slave address, the word address and the data
*               number must be store in the data exchange buffer
*               before calling this function:
*
*               Slave address        = data_exch_buff[offset + 0]
*               Data nr msb          = data_exch_buff[offset + 1]
*               Data nr lsb          = data_exch_buff[offset + 2]
*               Once this routine is over, the data read are
*               added to the first place of the buffer:
*               Data 1               = data_exch_buff[offset + 0]
*               Data 2               = data_exch_buff[offset + 1]
*               .....
*               Data N               = data_exch_buff[offset + N -1]
*
*               This function return a booleen if Data nr have been
*               read corectly.
*/
static phStatus_t  phhalSyncCt_I2CReadFromCurrentAddress(uint8_t *pbDataBuffer,uint8_t *pbReceiveBuf)
{
   gbI2CTxRxState       = E_STATE_0;
   gbI2CTxRxActive        = 0x01;

//   gdwI2CDataCount      = (uint32_t)(pbDataBuffer[1]*256);
   gdwI2CDataCount     = pbDataBuffer[2];						// Only one byte for read length
   if(gdwI2CDataCount > PHHAL_SYNC_CT_MAX_BUFFER_SIZE)
   {
       return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL,PH_COMP_HAL_SYNC_CT);
   }

   /*start receive data.*/

   phhalSyncCt_I2CStateMachine(1,pbReceiveBuf);

   if (gbI2CNoAck)
      return PH_ERR_FAILED;
   return PH_ERR_SUCCESS;
}
/**
* DESCRIPTION : Slave address    = data_exch_buff[offset + 0]
*               Word address msb = data_exch_buff[offset + 1]
*               Word address lsb = data_exch_buff[offset + 2]
*               Data nr msb      = data_exch_buff[offset + 3]
*               Data nr lsb      = data_exch_buff[offset + 4]
*               Once this routine is over, the data read are
*               added to the previous string i.e.:
*               Data 1           = data_exch_buff[offset + 0]
*               Data 2           = data_exch_buff[offset + 0]
*               .....
*               Data N           = data_exch_buff[offset + N - 1]
*
*               This function return a boolean if all datas have been read *
*/
phStatus_t phhalSyncCt_I2CExtendedRequestData(uint8_t bI2CExtended ,uint8_t *pbReceiveBuf ,uint8_t *pbDataBuffer)
{
    phStatus_t phStatus = PH_ERR_FAILED;
    gbI2CExtended  = bI2CExtended;
    gbSlaveAddress = phhalSyncCt_GetSlaveAddress(pbDataBuffer);
    phStatus=phhalSyncCt_I2CReadRandom(pbDataBuffer,pbReceiveBuf);
    if (PH_ERR_SUCCESS != phStatus)
    {

        return phStatus;
    }
    /*all data read.*/
    return PH_ERR_SUCCESS;
}
/**
* DESCRIPTION : this function read bytes from i2c synchronous card
*               in a random way.
*               The slave address, the word address and the data
*               number must be store in the data exchange buffer
*               before calling this function:
*               Slave address        = data_exch_buff[offset + 0]
*               Word address msb     = data_exch_buff[offset + 1] (extmode)
*               Word address lsb     = data_exch_buff[offset + 2]
*               Data nr msb          = data_exch_buff[offset + 3]
*               Data nr lsb          = data_exch_buff[offset + 4]
*               Once this routine is over, the data read are added
*               to the previous string i.e.:
*               Data 1               = data_exch_buff[offset + 0]
*               Data 2               = data_exch_buff[offset + 1]
*               .....
*               Data N               = data_exch_buff[offset + N - 1]
*
*               This function return a booleen if all datas have been read
*/
static phStatus_t phhalSyncCt_I2CReadRandom(uint8_t *pbDataBuffer ,uint8_t *pbReceiveBuf)
{
    gbI2CTxRxActive      = 1;

    gdwI2CDataCount  = (uint32_t)(pbDataBuffer[3]*256);
    gdwI2CDataCount += pbDataBuffer[4];

    if(gdwI2CDataCount > PHHAL_SYNC_CT_MAX_BUFFER_SIZE)
    {
        return PH_ADD_COMPCODE(PH_ERR_SYNC_CT_I2C_BUFFER_TOO_SMALL,PH_COMP_HAL_SYNC_CT);
    }
    if(phhalSyncCt_I2CTxHeader(pbDataBuffer))
    {
        /*start receive data.*/
        gbI2CTxRxState = E_STATE_0;
        phhalSyncCt_I2CStateMachine(1 ,pbReceiveBuf);
    }

    if (gbI2CNoAck)
        return PH_ERR_FAILED;
    return PH_ERR_SUCCESS;
}

/**
 * DESCRIPTION : This function write a byte on I2C port and return Acknoledge of slave.
 */
static uint8_t phhalSyncCt_I2CWriteByte(uint8_t bTransmitByte)
{
   uint8_t bLoopcount;
   uint8_t bTransmitBit;
  uint8_t bAcknoledge;

    /* Set ISO-UART in transmission mode.*/
   phhalSyncCt_SetTRMode(1);
   for (bLoopcount=8; bLoopcount>0; bLoopcount--)
   {
      /* Send MSB bit first.*/
      bTransmitBit = (bTransmitByte>>(bLoopcount-1)) & 0x01;
      phhalSyncCt_SetSyncBit(bTransmitBit);
      //phhalSyncCt_Send_Nbclocks(0x01);

      (void)phUser_Wait(40);
      PH_REG_SET_BIT(CT_CCRx_REG, SHL);
      (void)phUser_Wait(40);
      PH_REG_CLEAR_BIT(CT_CCRx_REG, SHL);
   }

   /* Set ISO-UART in reception mode.*/
   phhalSyncCt_SetTRMode(0);

   //delay min 5us
   (void)phUser_Wait(40);
   phhalSyncCt_SetClock(1);
   phhalSyncCt_GetSyncBit(&bAcknoledge);             /*input Acknoledge.*/
   //delay min 5us
   (void)phUser_Wait(40);
   phhalSyncCt_SetClock(0);
   return (bAcknoledge);
}
/**
* DESCRIPTION : This function read a byte on I2C port.
*              Bit ack is set if no acknoledge must be sent.
*              UART must be in receipt mode before calling this function.
*/
static uint8_t phhalSyncCt_I2CReadbyte(uint8_t bLastbyte)
{
   uint8_t bLoopCount, bReceivedByte = 0,bReceivedBit;

   for (bLoopCount=0; bLoopCount<8; bLoopCount++)
   {
      /* MSB bit first.*/

       bReceivedByte = bReceivedByte << 1;
       phhalSyncCt_SetClock(1);
       phhalSyncCt_GetSyncBit(&bReceivedBit);
       bReceivedByte |= bReceivedBit;
       phhalSyncCt_SetClock(0);
   }

   if (!(bLastbyte))
   {
      phhalSyncCt_SetSyncBit(0);       /* send Acknoledge if set.*/
      phhalSyncCt_SetTRMode(1);             /* Set ISO-UART in transmission mode.*/
      phhalSyncCt_Send_Nbclocks(0x01);
      phhalSyncCt_SetTRMode(0);            /* set ISO-UART in receiption mode.*/
   }
   else
   {
      phhalSyncCt_Send_Nbclocks(0x01);
      phhalSyncCt_SetTRMode(1);
   }

   return (bReceivedByte);
}

/**
 * DESCRIPTION : This function send slave address in case of read data
               strored in data_exchange_buffer[offset] on I2C port.
 */
static uint8_t phhalSyncCt_I2CTxSlv_Addr(uint8_t byte, uint8_t bRWN)
{
   /* Set or Reset R/W.*/

   byte |= bRWN;
   byte = phhalSyncCt_I2CWriteByte(byte);
   return(byte);
}

/**
* DESCRIPTION : This function send header to synchronous i2c card.
*               Slave address           = data_exch_buff[offset + 0]
*               Word address msb        = data_exch_buff[offset + 1]
*               Word address lsb        = data_exch_buff[offset + 2]
*               Data nr msb             = data_exch_buff[offset + 3]
*               Data nr lsb             = data_exch_buff[offset + 4]
*
*               This function return 1 if header have been transmit
*               without errors.
*/
static uint8_t phhalSyncCt_I2CTxHeader(uint8_t *pbDataBuffer)
{
    phhalSyncCt_I2CTxStart();
    gbI2CNoAck=phhalSyncCt_I2CTxSlv_Addr(gbSlaveAddress, 0);
    if(gbI2CNoAck==0)   /* slave address with R/W=0. */
    {
        if (gbI2CExtended)
            gbI2CNoAck=phhalSyncCt_I2CWriteByte(pbDataBuffer[1]);
        if (!gbI2CNoAck)
            gbI2CNoAck=phhalSyncCt_I2CWriteByte(pbDataBuffer[2]);
    }
    return (!gbI2CNoAck);
}

/* DESCRIPTION : This function send start condition*/
static void phhalSyncCt_I2CTxStart(void)
{
    uint8_t bReceivedBit;
    gbI2CNoAck = 0;
    phhalSyncCt_SetSyncBit(1);                        /* set SDA in high state.*/
    phhalSyncCt_SetTRMode(1);
    phhalSyncCt_SetClock(1);                          /* set CLK in high state.*/
    phhalSyncCt_SetTRMode(0);
    phhalSyncCt_GetSyncBit(&bReceivedBit);

    if (bReceivedBit)                     /* verify SDA in high state.*/
    {
        phhalSyncCt_SetSyncBit(0);                     /* set SDA in low state.*/
        phhalSyncCt_SetTRMode(1);                          /* ISO-UART in output mode.*/
        phhalSyncCt_SetClock(0);                       /* set CLK in low state.*/ // FIXME comment

            gbI2CTxRxState = E_STATE_1;                /* send data.*/
        if (gbTypeRxTx)
            gbI2CTxRxState = E_STATE_2;             /* receive data.*/
    }
    else
    {
        phhalSyncCt_SetClock(0);                       /* try to unlock SDA.*/

        if (!(phhalSyncCt_I2CWriteByte(0xFF)))      /* by sending 9 clock pulses.*/
        {
            gbI2CTxRxState = E_STATE_1;             /* send data.*/
            if (gbTypeRxTx)
            gbI2CTxRxState = E_STATE_2;          /* receive data.*/
        }
        else
        {
            gbI2CTxRxState = E_LAST_STATE;      /* state stop because io line locked.*/
        }
    }
}

/**
* DESCRIPTION : This function send data stored in data exchange buffer
*               on I2C port.
*               First byte is slave address.
*/
static void phhalSyncCt_I2CTxDatas(void)

{
   uint32_t  dWLoopCount;
   for (dWLoopCount=0; dWLoopCount<(gdwI2CDataCount); dWLoopCount++)
   {
      /* Send data to stop condition.*/
       gbI2CNoAck=phhalSyncCt_I2CWriteByte(gpbSyncCtBuffer[dWLoopCount]);
      if (gbI2CNoAck)
         /* gbI2CNoAck is set.*/
         break;
      /* if not ACK, wait 5 us between each byte.*/
      phUser_Wait(5000);
    }

    if (gbI2CNoAck)
    {
       gbI2CTxRxState  = E_STATE_0;
       /* if not ACKnoledge return to state zero and do not send stop condition.*/
       gbI2CTxRxActive = 0;
    }
    else
    {
       gbI2CTxRxState = E_LAST_STATE;         /* anything wrong, send stop condition.*/
    }
}


/*DESCRIPTION : This function read data from synchronous i2c card.*/
static void phhalSyncCt_I2CRxSlv_TxAddr(void)
{
    gbI2CNoAck=phhalSyncCt_I2CTxSlv_Addr(gbSlaveAddress, 1);
   if (gbI2CNoAck == 0)
      /* slave address with R/W=1,  anything wrong, read datas.*/

      gbI2CTxRxState = E_STATE_3;
   else
   {
      gbI2CTxRxState = E_STATE_0;
      gbI2CTxRxActive = 0;
   }
}

/**
* DESCRIPTION : This function read data from synchronous i2c card.
*               Data 1   = data_exch_buff[offset]
*               Data 2   = data_exch_buff[offset + 1]
*             .....
*               Data N  = data_exch_buff[offset + N - 1]
*/

static void phhalSyncCt_I2CRxDatas(void)
{
   uint32_t dwLoopCount;
   for (dwLoopCount=0; dwLoopCount<(gdwI2CDataCount-1); dwLoopCount++)
   {
      /* read n-1 datas.*/
      /*Received data to stop condition read with ack.*/
       gpbSyncCtBuffer[dwLoopCount]=phhalSyncCt_I2CReadbyte(0);
   }
   /* last data to receive then not ack.*/
   gpbSyncCtBuffer[dwLoopCount]=phhalSyncCt_I2CReadbyte(1);
   gbI2CTxRxState = E_LAST_STATE;
}


/*DESCRIPTION : This function send stop condition .*/
static void phhalSyncCt_I2CTxStop(void)
{
    phhalSyncCt_SetTRMode(1);              /* set ISO-UART in transmission mode.*/
    phhalSyncCt_SetSyncBit(0);         /*be sure SDA is Low.*/
    phhalSyncCt_SetClock(1);           /*clock line high.*/
    phhalSyncCt_SetTRMode(0);              /* stop condition leaving line.*/
    gbI2CTxRxActive = 0;         /* in high impedance.*/
}

#endif
