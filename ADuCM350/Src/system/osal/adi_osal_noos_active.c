/*********************************************************************************

Copyright (c) 2008-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement.

*********************************************************************************/

/* $Revision: 28527 $
 * $Date: 2014-11-12 16:08:36 -0500 (Wed, 12 Nov 2014) $ 
*****************************************************************************/

/*!
    @file adi_osal_noos_critical.c

    Operating System Abstraction Layer - OSAL for no-OS - Critical section
    functions

    This file contains the critical section & scheduler locking APIs for the
    no-OS implementation of OSAL

    @note 
         Since no scheduler is present in the no-OS implementation,
         SchedulerLock and SchedulerUnlock are equivalent to NOPs.
*/

/*=============  I N C L U D E S   =============*/


#if defined (__ADSP21000__)
#include <sysreg.h>                                                   
#endif

#include "adi_osal.h"

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=internal")
#pragma file_attr(  "prefersMemNum=30")
#endif

/*=============  D A T A  =============*/

/*
   variable     : _adi_osal_gbSchedulerActive
   A boolean flag to indicate whether scheduler is active.
*/
const bool _adi_osal_gbSchedulerActive = false;

/*=============  C O D E  =============*/

/*!
  ****************************************************************************
    @brief Determines whether the scheduler is running.

    @return true  - If the scheduler is running,
    @return false - If the scheduler is not running

    @note In the no-OS version of OSAL the function returns always false as
          there is no scheduler active.

*****************************************************************************/

bool adi_osal_IsSchedulerActive(void)
{
    return(false);
}


/*
**
** EOF:
**
*/
