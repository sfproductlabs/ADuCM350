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
#include "osal_noos.h"
#include "osal_common.h"


#ifdef _MISRA_RULES
/*Rule 14.7 indicates that a function shall have a single exit point */
#pragma diag(suppress:misra_rule_14_7:"Allowing several point of exit (mostly for handling parameter error checking) increases the code readability and therefore maintainability")

/* Rule-5.1 indicates that all identifiers shall not rely on more than 31 characters of significance */
#pragma diag(suppress:misra_rule_5_1:"prefixes added in front of identifiers quickly increases their size. In order to keep the code self explanatory, and since ADI tools are the main targeted tools, this restriction is not really a necessity")
#endif

/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=internal")
#pragma file_attr(  "prefersMemNum=30")
#endif


/* since IMASK is a 16-bits value, 0xFFFFFFFF is invalid and can be used for testing */
#define INVALID_IMASK       (0xFFFFFFFFul)


/*=============  D A T A  =============*/

/*=============  C O D E  =============*/

/*!
  ****************************************************************************
    @brief Prevents rescheduling until adi_osal_SchedulerUnlock is called.

    After this function is called, the current thread does not become
    de-scheduled , even if a high-priority thread becomes ready to run.

    Note that calls to adi_osal_SchedulerLock may be nested. A count is
    maintained to ensure that a matching number of calls to
    adi_osal_SchedulerUnlock are made before scheduling is re-enabled.

    In a non-OS port this function just returns

    @see adi_osal_SchedulerUnlock
*****************************************************************************/

void adi_osal_SchedulerLock( void )
{
#ifdef OSAL_DEBUG
    _adi_osal_gnSchedulerLockCnt++;
#endif
    return;
}


/*!
  ****************************************************************************
    @brief Re-enables thread scheduling.

    This function decrements the internal count which tracks how many times
    adi_osal_SchedulerLock was called. If the internal count is 0 then the
    scheduler is re-enabled.

    @return ADI_OSAL_SUCCESS - If thread scheduling was enabled successfully
    @return ADI_OSAL_FAILED  - If the function call does not match a call to
                               adi_osal_SchedulerLock

    @note In a non-OS port this function always returns ADI_OSAL_SUCCESS

    @see adi_osal_SchedulerLock
*****************************************************************************/

ADI_OSAL_STATUS adi_osal_SchedulerUnlock( void )
{
#ifdef OSAL_DEBUG
    if (0u == _adi_osal_gnSchedulerLockCnt)
    {
        return (ADI_OSAL_FAILED);
    }
    _adi_osal_gnSchedulerLockCnt--;
#endif
    return (ADI_OSAL_SUCCESS);
}



/*
**
** EOF:
**
*/
