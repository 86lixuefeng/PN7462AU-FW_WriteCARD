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

#ifndef PHPLATFORM_PORT_LINUXGPIO_H_
#define PHPLATFORM_PORT_LINUXGPIO_H_

#include <phDriver.h>


#include <stddef.h>
#include <stdbool.h>

phStatus_t PiGpio_is_exported(size_t gpio);
phStatus_t PiGpio_export(size_t gpio);
phStatus_t PiGpio_unexport(size_t gpio);
phStatus_t PiGpio_set_direction(size_t gpio, bool output);
phStatus_t PiGpio_set_edge(size_t gpio, bool rising, bool falling);
phStatus_t PiGpio_Write(size_t gpio, int value);
phStatus_t PiGpio_read(size_t gpio, uint8_t *pGpioVal);
phStatus_t PiGpio_poll(size_t gpio, int highOrLow, int timeOutms);


#endif /* PHPLATFORM_PORT_LINUXGPIO_H_ */
