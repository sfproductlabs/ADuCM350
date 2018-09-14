/*********************************************************************************

Copyright (c) 2009-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement.

*********************************************************************************/

/* $Revision: 28527 $
 * $Date: 2014-11-12 16:08:36 -0500 (Wed, 12 Nov 2014) $
***************************************************************************

Title:   specific definitions non OS environment.

         This file contains the non OS specific definitions.

*****************************************************************************/
#ifndef __OSAL_NOOS_H__
#define __OSAL_NOOS_H__


/*=============  I N C L U D E S   =============*/


#if defined(__STDC__)

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"This module gains clarity by using some identifier names longer than 31 characters.")
#endif

#include <adi_osal_arch_internal.h>

#include "adi_osal.h"
#include "osal_common.h"


/*==============  D E F I N E S  ===============*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
  Patch version of  OSAL for non-OS.  Please refer "adi_osal.h" for
  ADI_OSAL_MAJOR_VER and ADI_OSAL_MINOR_VER since these fields are
  also used for the code-base version
*/
#define ADI_OSAL_PATCH_VER                      2u


#ifdef DEBUG
/* in case we ever want to turn on the checks in 'release' mode */
#define OSAL_DEBUG
#endif /* DEBUG */


/*
    This preprocessor constant is used to indicate is the OSAL has been initialized.
   This is used instead of boolean flag in order to minimize the risk of match when
   the memory is uninitialized after processor boot
*/
#define OSAL_INITIALIZED                (0xAD10ul)


/*******************************************************************************

       Data Structures semaphore and eventgroup management.

 ********************************************************************************/

typedef struct osal_semaphore {
    bool               bActiveS;            /* Indicates if the semaphore is active, this avoid operations on deleted semaphores */
    volatile uint32_t  nCount;             /* count for the semaphore  */
} OSAL_SEMAPHORE, *OSAL_SEMAPHORE_PTR;

typedef struct osal_event {
    bool               bActiveE;            /* Indicates if the semaphore is active, this avoid operations on deleted eventgroups */
    volatile ADI_OSAL_EVENT_FLAGS  eventBits;           /* flag state of the eventgroup  */
} OSAL_EVENT, *OSAL_EVENT_PTR;


/* external data (shared across OSAL files) */
extern  uint32_t     _adi_osal_gnSchedulerLockCnt;
extern  int32_t      _adi_osal_gnCriticalRegionNestingCnt;
extern  uint32_t     _adi_osal_gnCriticalRegionState;
extern  uint32_t     _adi_osal_gnTickPeriod;
extern  uint32_t     _adi_osal_gnTickCnt;
extern  uint32_t     _adi_osal_gnNumSlots;
extern  uint32_t     _adi_osal_gTLSAvailableSlots;
extern  ADI_OSAL_SLOT_VALUE       _adi_osal_gaTLSSlotTable[ADI_OSAL_MAX_NUM_TLS_SLOTS];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

#else /* assembly language specific macros and declarations*/

#endif  /* if !defined(__GNUC__) && !defined(__STDC__) */

#endif /*__OSAL_NOOS_H__ */
/*
**
** EOF: 
**
*/

