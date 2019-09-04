/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  RS: Generated 24. Jan 2017
*
*/



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include "phDriver.h"
#include "BoardSelection.h"

#ifdef PHDRIVER_LINUX_USER_SPI



#include <stdint.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <poll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define PHBAL_REG_LINUX_USER_SPI_ID               0x12U       /**< ID for Linux User Spi BAL component */

#define PHBAL_REG_LINUX_ERROR                    0xFFFFFFFF

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

typedef struct
{
    /* Pin/bus numbers */
    uint32_t spiBus;
    uint32_t spiCsPinNum;

    /* File descriptors */
    uint32_t spiFd;
}phbalReg_LinuxUserSpiObj_t;


/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */
static phbalReg_LinuxUserSpiObj_t sSpiObj;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static phStatus_t phbalReg_LinuxUserSpi_ClosePort(phbalReg_LinuxUserSpiObj_t * psSpiObj);
static phStatus_t phbalReg_LinuxUserSpi_OpenPort(phbalReg_LinuxUserSpiObj_t * psSpiObj);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phbalReg_Init(void * pDataParams, uint16_t wSizeOfDataParams)
{
    phStatus_t status;

    if ((pDataParams == NULL) || (sizeof(phbalReg_Type_t) != wSizeOfDataParams))
    {
        return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
    }

    ((phbalReg_Type_t *)pDataParams)->wId        = PH_COMP_DRIVER | PHBAL_REG_LINUX_USER_SPI_ID;
    ((phbalReg_Type_t *)pDataParams)->bBalType   = PHBAL_REG_TYPE_USER_SPI;

    sSpiObj.spiFd          = PHBAL_REG_LINUX_ERROR;

    /* Set default bus/pin numbers */
    sSpiObj.spiBus         = PHDRIVER_USER_SPI_BUS;
    sSpiObj.spiCsPinNum    = PHDRIVER_USER_SPI_CS;

    status = phbalReg_LinuxUserSpi_OpenPort(&sSpiObj);

    return status;
}


phStatus_t phbalReg_Exchange(void * pDataParams, uint16_t wOption,
    uint8_t * pTxBuffer, uint16_t wTxLength, uint16_t wRxBufSize, uint8_t * pRxBuffer, uint16_t * pRxLength)
{
    uint32_t dwStatus;
    struct spi_ioc_transfer sSpiTransfer = {0};

    if(wTxLength == 0)
    {
        return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
    }

    sSpiTransfer.tx_buf = (uintptr_t)pTxBuffer;
    sSpiTransfer.rx_buf = (uintptr_t)pRxBuffer;
    sSpiTransfer.len = wTxLength;
    sSpiTransfer.speed_hz = PHDRIVER_USER_SPI_FREQ;
    sSpiTransfer.bits_per_word = PHDRIVER_USER_SPI_CFG_BITS_PER_WORD;
    sSpiTransfer.delay_usecs = 0;

    dwStatus = ioctl(sSpiObj.spiFd, SPI_IOC_MESSAGE(1), &sSpiTransfer);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if( pRxLength != NULL )
    {
        *pRxLength = sSpiTransfer.len;
    }

    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_SetConfig(void * pDataParams, uint16_t wConfig,
    uint16_t wValue)
{
    return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
}

phStatus_t phbalReg_GetConfig(void * pDataParams, uint16_t wConfig,
    uint16_t * pValue)
{
    return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */


static phStatus_t phbalReg_LinuxUserSpi_ClosePort(phbalReg_LinuxUserSpiObj_t * psSpiObj)
{
    if(psSpiObj->spiFd != PHBAL_REG_LINUX_ERROR)
    {
        close(psSpiObj->spiFd);
    }

    psSpiObj->spiFd = PHBAL_REG_LINUX_ERROR;

    return PH_DRIVER_SUCCESS;
}

static phStatus_t phbalReg_LinuxUserSpi_OpenPort(phbalReg_LinuxUserSpiObj_t * psSpiObj)
{
    uint32_t dwStatus;
    char spi[16];

    uint32_t mode = PHDRIVER_USER_SPI_CFG_MODE;
    uint8_t bits = PHDRIVER_USER_SPI_CFG_BITS_PER_WORD;
    uint32_t speed = PHDRIVER_USER_SPI_FREQ;

    snprintf(spi, sizeof(spi), PHDRIVER_USER_SPI_CFG_DIR "%lu.%lu",
        (long) psSpiObj->spiBus, (long)psSpiObj->spiCsPinNum);

    psSpiObj->spiFd = open(spi, O_RDWR);
    if(psSpiObj->spiFd == PHBAL_REG_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    /* Set mode */
    dwStatus = ioctl(psSpiObj->spiFd, SPI_IOC_WR_MODE, &mode);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserSpi_ClosePort(psSpiObj);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    /* Set Bits per Word */
    dwStatus = ioctl(psSpiObj->spiFd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserSpi_ClosePort(psSpiObj);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    /* Set Speed. */
    dwStatus = ioctl(psSpiObj->spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserSpi_ClosePort(psSpiObj);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    return PH_DRIVER_SUCCESS;
}

#endif /* PHDRIVER_LINUX_USER_SPI */
