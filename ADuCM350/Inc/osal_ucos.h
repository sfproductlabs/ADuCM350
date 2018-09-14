/*
***************************************************************************
Copyright (c), 2009-2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
***************************************************************************

Title:   UCOS-specific definitions

         This file contains the uCOS specific defines.

*****************************************************************************/

#ifndef __OSAL_UCOS_H__
#define __OSAL_UCOS_H__


#ifdef _MISRA_RULES
#pragma diag(push)
/* Rule-5.1 indicates that all identifiers shall not rely on more than 31 characters of significance */
#pragma diag(suppress:misra_rule_5_1:"prefixes added in front of identifiers quickly increases their size. In order to keep the code self explanatory, and since ADI tools are the main targeted tools, this restriction is not really a necessity")
#endif /* _MISRA_RULES */


/*=============  I N C L U D E S   =============*/

#include <adi_types.h>
#include "osal_common.h"
#include <adi_osal_arch_internal.h>

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all)
#endif /*  _MISRA_RULES */
#include <ucos_ii.h>
#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

/*==============  D E F I N E S  ===============*/
#if defined(__STDC__)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
  Patch version of  OSAL for uCOS.  Please refer "adi_osal.h" for
  ADI_OSAL_MAJOR_VER and ADI_OSAL_MINOR_VER since these fields are
  also used for the code-base version
*/
#define ADI_OSAL_PATCH_VER              3u


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

/* Semaphores are used in place of Mutexes in this implementation */
#define INIT_COUNT_FOR_EMULATED_MUTEX   1u

/*
    Defines the minimum number of bytes that the user has to pass during OSAL
    Initialization to create the OSAL heap.  For now in uCOS, there are no real
    requirements, so we just check that it is not 0
*/
#define OSAL_MINIMUM_HEAP_SIZE          ((uint32_t) 0u)


/* The scheduler and critical region locks counters can be used to verify if
  the user calls a function that can potentially create a deadlock situation */

#define CALLED_IN_SCHED_LOCK_REGION     (OSLockNesting > 0u)


/*
    uC/OS reserves the first 3 priority levels, so we offset them in the OSAL
    (see note #3 in the uCOS Reference Manual under OSTaskCreate)
*/
#define ADI_OSAL_UCOS_BASE_PRIO         3u

/*
    uC/OS maximum lowest priority is 255 (as of v2.85). It is really defined by
    the build option OS_LOWEST_PRIO.  This macro is defined for error checking
    purposes
*/
#define UCOS_ABSOLUTE_MAXIMUM_PRIORITY  (0xFFu)

/*
    The current code has been developed and tested with uC/OS version 2.86
*/
#define COMPATIBLE_OS_VERSION           (286u)


/*  Structure:  ADI_OSAL_THREAD_INFO
    This structure provides a mapping between thread handle and thread priority
    it also stores the thread local storage since uCOS does not provide it.
*/
typedef struct __AdiOsalThreadInfo
{
    /* Variable : nThreadSignature
     * Stores the thread signature to determine if a pointer is an OSAL thread
     */
    uint32_t nThreadSignature;

    /* Variable : nThreadPrio
       Stores the thread  priority (which is the ID of a ucos-ii task), ucos
       only support 255 priorities, so although 8 bits are enough, the type does
       not exist on SHARC */
    ADI_OSAL_PRIORITY nThreadPrio;
} ADI_OSAL_THREAD_INFO, *ADI_OSAL_THREAD_INFO_PTR;

/* signature added to OSAL threads on creation */
#define ADI_OSAL_THREAD_SIGNATURE (0x41444954u)

/* signature added to OSAL threads on destruction */
#define ADI_OSAL_INVALID_THREAD_SIGNATURE (0x6e746872u)

typedef struct __AdiOsalMutexInfo
{
    /* Variable : hMutexID
       Stores the mutex  handle return from uCOS
    */
    OS_EVENT*               hMutexID;

    /* Variable : hOwnerThread
       Holds the task handle which acquires the mutex. This is a OS_TCB*
       because we don't know whether the thread was created with OSAL or not.
    */
    OS_TCB*         hOwnerThread;

    /* Variable : nAcquisitionCount
       counter for mutex ownership.
    */
    uint32_t                nAcquisitionCount;

    /* Variable : bDynAlloc
        indicates if this structure has been dynamically allocated.
    */
    bool                    bDynAlloc;

} ADI_OSAL_MUTEX_INFO, *ADI_OSAL_MUTEX_INFO_PTR;



/*
    This definition is used to allocate enough memory for the worth case TCB structure size
    The 20 words have been computed using the worth case definition based on ucos v2.85
    This value is verified in the Init function.
*/
#define ADI_OSAL_UCOS_MAX_TCB_SIZE          ((uint16_t) (20u*4u + (uint16_t) ADI_OSAL_MAX_THREAD_NAME))


#if defined (__ECC__)
/*  Utility Macro to do compile time assertion */
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_19_13:"## is used so that identifier names are unique")
#pragma diag(suppress:misra_rule_19_7:"The gains from this compile time check macro outweight the suppression of the Adv rules ")
#pragma diag(suppress:misra_rule_19_4:"In this case we don't want this to take space in the code, so it is defined as a typedef")
#endif
#endif

#define COMPILE_TIME_ASSERT(condition, name) typedef uint8_t name##_failed[(condition)?1:-1]


#if defined (__ECC__)
#ifdef _MISRA_RULES
#pragma diag(pop)
#endif
#endif
/*=============  E X T E R N A L S  =============*/

/* Local global variables that are shared across files */
extern uint32_t _adi_osal_gnNumSlots;
extern uint32_t _adi_osal_gnTickPeriod;
extern uint32_t _adi_osal_gkLowestPrio;
extern ADI_OSAL_THREAD_INFO _adi_osal_oStartupVirtualThread;
extern uint32_t _adi_osal_gnSchedulerLockCnt;
extern int32_t _adi_osal_gnCriticalRegionNestingCnt;


#pragma inline
bool _adi_osal_IsOSALThread(ADI_OSAL_THREAD_INFO const *hThread)
{
    return ( hThread->nThreadSignature == ADI_OSAL_THREAD_SIGNATURE);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#else /* assembly language specific macros and declarations*/


#endif  /* if !defined(_LANGUAGE_C) */


#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */



#endif /*__OSAL_UCOS_H__ */

/*
**
** EOF:
**
*/
