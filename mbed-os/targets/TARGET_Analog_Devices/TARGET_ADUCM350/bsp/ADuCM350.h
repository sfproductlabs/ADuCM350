/*
** ADuCM350.h
**
** Copyright (C) 2016 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef ADUCM350_H
#define ADUCM350_H

#include <ADuCM350_cdef.h>
#include <ADuCM350_device.h>

#define __CM3_REV              0x0001U /*!< CM4 Core Revision r0p1                */
#define __MPU_PRESENT          1u      /*!< MPU present                           */
#ifndef __FPU_PRESENT
#define __FPU_PRESENT          0u      /*!< FPU present                           */
#endif
#define __NVIC_PRIO_BITS       3u      /*!< Number of Bits for Priority Levels    */
#define __Vendor_SysTickConfig 0       /*!< 1 if different SysTick Config is used */

#include <core_cm3.h>

#include "system_ADuCM350.h"

#endif /* ADUCM350_H */
