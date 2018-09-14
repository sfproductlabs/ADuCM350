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
    @file adi_osal_noos_timing.c

    Operating System Abstraction Layer - OSAL for non-OS - Timing
    functions

    This file contains the Timing APIs for the non-OS implementation of
    OSAL

    @internal

    SUPRESSED MISRA RULES ARE:
        See adi_osal_noos.c for the list of the MISRA Rules that have been
        suppressed for the no-OS OSAL.

    @endinternal
*/

/*=============  I N C L U D E S   =============*/

#include "adi_osal.h"
#include "osal_noos.h"
#include "osal_common.h"



#ifdef _MISRA_RULES
/*Rule 14.7 indicates that a function shall have a single exit point */
#pragma diag(suppress:misra_rule_14_7:"Allowing several point of exit (mostly for handling parameter error checking) increases the code readability and therefore maintainability")
#endif


/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=any")
#pragma file_attr(  "prefersMemNum=50")
#endif



/*=============  D A T A  =============*/
/*!
    @internal
    @var _adi_osal_gnTickPeriod
         defines the length of system ticks in microseconds
    @endinternal
 */
uint32_t _adi_osal_gnTickPeriod = ADI_OSAL_DEFAULT_SYS_TIMER_PERIOD;

/*
   variable     : _adi_osal_gnTickCnt
   System ticks counter. Each tick represents '_adi_osal_gnTickPeriod'
   microseconds.
 */
uint32_t _adi_osal_gnTickCnt = 0u ;



/*=============  C O D E  =============*/


/*!
  ****************************************************************************
    @brief Returns the duration of a tick period in microseconds.

    @param[out] pnTickPeriod - pointer to a location to write the tick period
                               in microseconds.

    @return ADI_OSAL_SUCCESS - If the function successfully returned the
                               duration of the tick period in microseconds.

  Notes:
      This function  helps to convert  time units to system ticks which is
      needed by the pend APIs of message-Q,semaphore,mutex,event  and to
      put the task in "sleep" mode.

                                                   No. Microsec in one second
      Duration of the tick period (in micro second) =  -------------------------
                                                   No of ticks in one second
*****************************************************************************/

ADI_OSAL_STATUS  adi_osal_TickPeriodInMicroSec(uint32_t *pnTickPeriod)
{
    *pnTickPeriod = _adi_osal_gnTickPeriod;  
    return(ADI_OSAL_SUCCESS);
}


/*!
  ****************************************************************************
  @brief Processes a clock tick

  This indicates to the OS that a tick period is completed.

  Parameters:
      None

  Returns:
      None

*****************************************************************************/
void adi_osal_TimeTick ( void )
{
    _adi_osal_gnTickCnt++;
    return;
}


/*!
  ****************************************************************************
    @brief Returns the current value of the continuously incrementing timer
           tick counter.

    The counter increments once for every timer interrupt.

    @param[out] pnTicks - pointer to a location to write the current value of
                          the tick counter.

    @return ADI_OSAL_SUCCESS - If the function successfully returned the tick
                               counter value

*****************************************************************************/
ADI_OSAL_STATUS adi_osal_GetCurrentTick(uint32_t *pnTicks )
{
    *pnTicks = _adi_osal_gnTickCnt;
    return(ADI_OSAL_SUCCESS);
}


/*
**
** EOF: 
**
*/
