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
    @file adi_osal_noos_prof.c

    Operating System Abstraction Layer - OSAL for no OS- Profiling functions

    This file contains the Profiling APIs for the no OS implementation of 
    OSAL

*/


/*=============  I N C L U D E S   =============*/

#include "adi_osal.h"
#include "osal_noos.h"
#include "osal_common.h"
#include <stdlib.h>


#ifdef _MISRA_RULES
/*Rule 14.7 indicates that a function shall have a single exit point */
#pragma diag(suppress:misra_rule_14_7:"Allowing several point of exit (mostly for handling parameter error checking) increases the code readability and therefore maintainability")
#pragma diag(suppress:misra_rule_5_1:"prefixes added in front of identifiers quickly increases their size. In order to keep the code self explanatory, and since ADI tools are the main targeted tools, this restriction is not really a necessity")
#endif

/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=any")
#pragma file_attr(  "prefersMemNum=50")
#endif




/*=============  D A T A  =============*/

static bool sbIsProfilingEnabled = false;


/*=============  C O D E  =============*/



/*!
  ****************************************************************************
    @brief Enables profiling for all threads

    @description adi_osal_ThreadEnableCycleCounting is an API used to enable 
                 thread-specific cycle counting for profiling purposes, if
                 supported by the underlying operating system.  It enables
                 cycle counting for all threads.

    @return ADI_OSAL_SUCCESS      - If cycle counting has been successfully 
                                    enabled.
    @return ADI_OSAL_FAILED       - If cycle counting is not supported by the 
                                    OS, or could not be enabled

 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_ThreadEnableCycleCounting(void)
{
#ifdef OSAL_DEBUG
    sbIsProfilingEnabled = true;
#endif
    return ADI_OSAL_SUCCESS;
}

/*!
  ****************************************************************************
    @brief Returns the cycle count of the current thread

    @description adi_osal_ThreadGetCycleCount is an API used to retrieve the 
                 thread-specific cycle count of the currently executing thread.
                 If the thread-specific cycle counting has not been
                 successfully enabled using adi_osal_ThreadEnableCycleCounting,
                 then adi_osal_ThreadGetCycleCount will return a failure code.
                 Whether or not the count includes scheduler cycles is
                 implementation (and operating system) dependent.


    @param[out] pnCycleCount        - Pointer to the location to write the 
                                      returned cycle count for the currently
                                      executing thread.

    @return ADI_OSAL_SUCCESS        - If cycle counting has been successfully 
                                      enabled.
    @return ADI_OSAL_FAILED         - If cycle counting is not supported by the 
                                      OS, or could not be enabled
    @return ADI_OSAL_BAD_HANDLE  [D]- If the pointer argument is NULL.
    @return ADI_OSAL_CALLER_ERROR[D]- If the API was called from ISR level or
                                      cycle counting was not enabled.

    @note In the case of a failure the content of the pointer argument is set
          to 0.
 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_ThreadGetCycleCount(uint64_t *pnCycles)
{
#ifdef OSAL_DEBUG
    if (NULL == pnCycles )
    {
        return (ADI_OSAL_BAD_HANDLE);
    }

    if (_adi_osal_IsCurrentLevelISR())
    {
        *pnCycles = 0u;
        return (ADI_OSAL_CALLER_ERROR);
    }

    if (!sbIsProfilingEnabled)
    {
        *pnCycles = 0u;
        return (ADI_OSAL_CALLER_ERROR);
    }
#endif
    *pnCycles = __builtin_emuclk();
    return (ADI_OSAL_SUCCESS);
}

/*
**
** EOF: 
**
*/
