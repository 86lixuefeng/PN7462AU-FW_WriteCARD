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

#include "phDriver.h"

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <poll.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define PHDRIVER_LINUX_CFG_DIR                      "/sys/class/gpio"
#define PHDRIVER_LINUX_FILE_EXPORT_RETRY_COUNT      10
#define PHDRIVER_LINUX_FILE_EXPORT_TIMEOUT          100
#define PHDRIVER_LINUX_ERROR                        0xFFFFFFFF

static void phDriver_Linux_IntDelay(uint32_t dwDelay);

phStatus_t PiGpio_is_exported(size_t gpio)
{
    uint32_t fd = 0;
    char bGpio[64] = {0};

    snprintf(bGpio, sizeof(bGpio), PHDRIVER_LINUX_CFG_DIR "/gpio%lu/direction", (long unsigned int) gpio);

    fd = open(bGpio, O_WRONLY);
    if (fd != PHDRIVER_LINUX_ERROR)
    {
        close(fd);
        return PH_DRIVER_SUCCESS;
    }

    return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
}


phStatus_t PiGpio_export(size_t gpio)
{
    uint32_t fd;
    uint32_t dwTimeoutLoop;
    char buf[64] = {0};
    size_t len = 0;

    if(PiGpio_is_exported(gpio) == PH_DRIVER_SUCCESS)
    {
        return PH_DRIVER_SUCCESS;
    }

    fd = open(PHDRIVER_LINUX_CFG_DIR "/export", O_WRONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    len = snprintf(buf, sizeof(buf), "%lu", (long unsigned int)gpio);
    write(fd, buf, len);
    close(fd);

    /* wait until file is actually available in user space OR TimeOut. */
    for (dwTimeoutLoop = 0; dwTimeoutLoop < PHDRIVER_LINUX_FILE_EXPORT_RETRY_COUNT; dwTimeoutLoop++)
    {
        if( PiGpio_is_exported(gpio) == 0 )
        {
            return PH_DRIVER_SUCCESS;
        }

        phDriver_Linux_IntDelay(PHDRIVER_LINUX_FILE_EXPORT_TIMEOUT);
    }

    return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
}

phStatus_t PiGpio_unexport(size_t gpio)
{
    uint32_t fd = 0;
    char buf[64] = {0};
    uint32_t dwLen = 0;

    fd = open(PHDRIVER_LINUX_CFG_DIR "/unexport", O_WRONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    dwLen = snprintf(buf, sizeof(buf), "%lu", (long unsigned int)gpio);
    write(fd, buf, dwLen);
    close(fd);

    return PH_DRIVER_SUCCESS;
}

phStatus_t PiGpio_set_direction(size_t gpio, bool output)
{
    uint32_t fd = 0;
    char buf[64] = {0};

    snprintf(buf, sizeof(buf), PHDRIVER_LINUX_CFG_DIR "/gpio%lu/direction", (long unsigned int)gpio);
    fd = open(buf, O_WRONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if(output)
    {
        write(fd, "out", 3);
    }
    else
    {
        write(fd, "in", 2);
    }

    close(fd);

    return PH_DRIVER_SUCCESS;
}

phStatus_t PiGpio_set_edge(size_t gpio, bool rising, bool falling)
{
    uint32_t fd = 0;
    char buf[64] = {0};

    snprintf(buf, sizeof(buf), PHDRIVER_LINUX_CFG_DIR "/gpio%lu/edge", (long unsigned int)gpio);

    fd = open(buf, O_WRONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if(rising && falling)
    {
        write(fd, "both", 4);
    }
    else if(rising)
    {
        write(fd, "rising", 6);
    }
    else
    {
        write(fd, "falling", 7);
    }

    close(fd);

    return PH_DRIVER_SUCCESS;
}

phStatus_t PiGpio_read(size_t gpio, uint8_t *pGpioVal)
{
    char path[30];
    char cValue;
    uint32_t fd;
    struct pollfd pollfd;

    *pGpioVal = 0;

    snprintf(path, 30, PHDRIVER_LINUX_CFG_DIR "/gpio%lu/value", (long unsigned int)gpio);
    fd = open(path, O_RDONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    pollfd.fd = fd;
    lseek(pollfd.fd, 0, SEEK_SET);

    if ((read(fd, &cValue, 1)) == PHDRIVER_LINUX_ERROR)
    {
        close(fd);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    close(fd);

    *pGpioVal = (cValue == '0') ? 0 : 1;

    return PH_DRIVER_SUCCESS;
}

phStatus_t PiGpio_Write(size_t gpio, int value)
{
    const char s_values_str[] = "01";

    char path[30];
    uint32_t fd;

    snprintf(path, 30, PHDRIVER_LINUX_CFG_DIR "/gpio%lu/value", (long unsigned int)gpio);
    fd = open(path, O_WRONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if (1 != write(fd, &s_values_str[(0 == value) ? 0 : 1], 1))
    {
        close(fd);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    close(fd);
    return PH_DRIVER_SUCCESS;
}

phStatus_t PiGpio_poll(size_t gpio, int highOrLow, int timeOutms)
{
    char path[30];
    char cValue;
    uint32_t fd;
    uint32_t ret;
    struct pollfd pollfd;

    pollfd.events = POLLPRI; /* look for GPIO status change. */

    snprintf(path, 30, PHDRIVER_LINUX_CFG_DIR "/gpio%lu/value", (long unsigned int)gpio);
    fd = open(path, O_RDONLY);
    if (fd == PHDRIVER_LINUX_ERROR)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    pollfd.fd = fd; /* GPIO file descriptor. */

    ret = read(fd, &cValue, 1);
    if (ret == PHDRIVER_LINUX_ERROR)
    {
        close(fd);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if((highOrLow) == ((cValue == '0')? 0 : 1))
    {
        close(fd);
        return PH_DRIVER_SUCCESS;
    }

    /* Reposition the read/write file offset to 0 bytes. */
    lseek(pollfd.fd, 0, SEEK_SET);
    /* Poll until the GPIO state changes.
     * Num of Items in pollFd is 1. */
    ret = poll( &pollfd, 1, timeOutms );
    if( ret != 1 )
    {
        close(fd);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    /* Check for GPIO change OR Timeout. */
    lseek(pollfd.fd, 0, SEEK_SET);

    ret = read(fd, &cValue, 1);
    if (ret == PHDRIVER_LINUX_ERROR)
    {
        close(fd);
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if((highOrLow) == ((cValue == '0')? 0 : 1))
    {
        close(fd);
        return PH_DRIVER_SUCCESS;
    }else{
        close(fd);
        return (PH_DRIVER_TIMEOUT | PH_COMP_DRIVER);
    }
}


static void phDriver_Linux_IntDelay(uint32_t dwDelay)
{
    struct timespec sTimeSpecReq;
    struct timespec sTimeSpecRem;

    sTimeSpecReq.tv_sec = (dwDelay / 1000);
    sTimeSpecReq.tv_nsec = (dwDelay % 1000) * 1000 * 1000;

    nanosleep(&sTimeSpecReq, &sTimeSpecRem);
}

