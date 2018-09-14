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
    @file adi_osal_noos_mutex.c

    Operating System Abstraction Layer - OSAL for no-OS - Mutex functions

    This file contains the Mutex APIs for the no-OS implementation of OSAL

    @internal

    @note  
         In the no-OS implementation there is only one "thread".  Since
         pending is not allowed in ISR levels, the "thread" can always acquire
         it. So mutexes are implemented as all returning success unless there
         is a programming error releasing a mutex that was not acquired.

    SUPRESSED MISRA RULES ARE:
        See adi_osal_noos.c for the list of the MISRA Rules that have been
        suppressed for the no-OS OSAL.

    @endinternal
*/


/*=============  I N C L U D E S   =============*/

/* stdlib.h is included to find NULL definition */
#include <stdlib.h>
#include "adi_osal.h"
#include "osal_noos.h"
#include "osal_common.h"


#ifdef _MISRA_RULES
/*Rule 14.7 indicates that a function shall have a single exit point */
#pragma diag(suppress:misra_rule_14_7:"Allowing several point of exit (mostly for handling parameter error checking) increases the code readability and therefore maintainability")

/* Rule-5.1 indicates that all identifiers shall not rely on more than 31 characters of significance */
#pragma diag(suppress:misra_rule_5_1:"prefixes added in front of identifiers quickly increases their size. In order to keep the code self explanatory, and since ADI tools are the main targeted tools, this restriction is not really a necessity")

/* Rule-11.3 indicates that typecast of a integer value into pointer is invalid */
#pragma diag(suppress:misra_rule_11_3 : "typecasting is necessary every time a predefine value is written to a return pointer during error conditions")

/* Rule-16.7 indicates that an argument which is a pointer argument should be
 * const if the pointer is not used to modify the object. This is a common API
 * even if the argument is not used in this implementation it could be in
 * others
 */
#pragma diag(suppress:misra_rule_16_7 : "sometimes the pointer argument is not used but cannot be declared const")
#endif


/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=any")
#pragma file_attr(  "prefersMemNum=50")
#endif


/* since returning NULL means error, this value is used to define a "valid" mutex handle. */
#define DUMMY_MUTEX_HANDLE  ((ADI_OSAL_MUTEX_HANDLE) 0xAD10ul)

/*=============  D A T A  =============*/

#ifdef OSAL_DEBUG
/* in the debug case we keep track of how many times mutexes have been acquired
 * and released and give an error if the number of releases is greater than
 * acquires.
 * In order not to have to allocate memory a single count is used for all mutexes.
 */ 

static uint32_t snMutexAcquireCount = 0u;
#endif


/*=============  C O D E  =============*/

/*!
  ****************************************************************************
    @brief Creates a mutex.

    @param[out] phMutex  - Pointer to a location to write the returned mutex
                           handle

    @return ADI_OSAL_SUCCESS      - If mutex is created successfully
    @return ADI_OSAL_FAILED       - If failed to create mutex
    @return ADI_OSAL_MEM_ALLOC_FAILED  - If memory could not be allocated
    @return ADI_OSAL_CALLER_ERROR - If the function is invoked from an invalid
                                    location


    @note On the non-OS implementation of OSAL this function always succeeds
          and returns a dummy mutex unless it is called from ISR or phMutex is
          NULL.

    "phMutex" points to NULL if mutex creation is failed.

 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_MutexCreate(ADI_OSAL_MUTEX_HANDLE *phMutex)
{
#ifdef OSAL_DEBUG
    if (phMutex == NULL)
    {
        return(ADI_OSAL_FAILED);
    }

    if(_adi_osal_IsCurrentLevelISR())
    {
        *phMutex = ADI_OSAL_INVALID_MUTEX;
        return ADI_OSAL_CALLER_ERROR;
    }
#endif  /* OSAL_DEBUG */

    return (adi_osal_MutexCreateStatic(NULL, 0u, phMutex));
}

/*!
  ****************************************************************************
    @brief Creates a mutex using user allocated memory for the mutex

    THIS FUNCTION IS THE SAME AS adi_osal_MutexCreate except that memory is
    passed to it instead of relying on dynamic memory.

    @param[out] phMutex      - Pointer to a location to write the returned
                               mutex handle
    @param[in] nMutexObjSize - Size of the memory passed for the creation of
                               the mutex
    @param[in] pMutexObject  - Area of memory provided to us for the mutex

    @return ADI_OSAL_SUCCESS      - If mutex is created successfully
    @return ADI_OSAL_FAILED       - If failed to create mutex
    @return ADI_OSAL_MEM_ALLOC_FAILED - If there isn't sufficient memory to create
                                    the mutex
    @return ADI_OSAL_CALLER_ERROR - If the function is invoked from an invalid
                                    location

    @note On the non-OS implementation of OSAL this function always succeeds
          and returns a dummy mutex unless it is called from ISR or phMutex is
          NULL.

    @see adi_osal_MutexCreate
    @see adi_osal_MutexDestroyStatic
 *****************************************************************************/


ADI_OSAL_STATUS adi_osal_MutexCreateStatic(void* const pMutexObject, uint32_t nMutexObjSize, ADI_OSAL_MUTEX_HANDLE *phMutex)
{
#ifdef OSAL_DEBUG
    if (phMutex == NULL)
    {
        return(ADI_OSAL_FAILED);
    }

    if(_adi_osal_IsCurrentLevelISR())
    {
		*phMutex = ADI_OSAL_INVALID_MUTEX;
        return ADI_OSAL_CALLER_ERROR;
    }
#endif  /* OSAL_DEBUG */
    
    *phMutex = DUMMY_MUTEX_HANDLE;
    return ADI_OSAL_SUCCESS;
}


/*!
  ****************************************************************************
  @brief Returns the size of a mutex object.

  This function can be used by the adi_osal_MutexCreateStatic function to
  determine what the object size should be for this particular RTOS
  implementation.

  Parameters:
      None

    @return size of a mutex object in bytes.

    @note On the non-OS implementation of OSAL the size is 0

    @see adi_osal_MutexCreateStatic

*****************************************************************************/


uint32_t adi_osal_MutexGetObjSize(void)
{
    return ( 0ul );
}


/*!
  ****************************************************************************
    @brief Deletes a mutex without freeing memory

    This API is designed to destroy a mutex that has been allocated with
    adi_osal_MutexCreateStatic().

    @param[in] hMutex      - Handle of the mutex to be deleted

    @return ADI_OSAL_SUCCESS      - If mutex is deleted successfully
    @return ADI_OSAL_FAILED       - If failed to delete mutex
    @return ADI_OSAL_BAD_HANDLE   - If the specified mutex handle is invalid
    @return ADI_OSAL_CALLER_ERROR - If function is invoked from an invalid
                                    location
    @return ADI_OSAL_THREAD_PENDING-If one or more threads are pending on the
                                    specified mutex.

  Notes:
      Only owner is authorised to release the acquired mutex. But it
      can "destroyed" by  other task.

    @see adi_osal_MutexCreateStatic
    @see adi_osal_MutexDestroy

*****************************************************************************/


ADI_OSAL_STATUS adi_osal_MutexDestroyStatic(ADI_OSAL_MUTEX_HANDLE const hMutex)
{
#ifdef OSAL_DEBUG
    if(_adi_osal_IsCurrentLevelISR())
    {
        return ADI_OSAL_CALLER_ERROR;
    }

    if (DUMMY_MUTEX_HANDLE != hMutex )
    {
        return ADI_OSAL_BAD_HANDLE;
    }
#endif  /* OSAL_DEBUG */

    return  ADI_OSAL_SUCCESS ;
}



/*!
  ****************************************************************************
    @brief This function is used to delete a mutex.

    @param[in] hMutex      - Handle of the mutex to be deleted

    @return ADI_OSAL_SUCCESS      - If mutex is deleted successfully
    @return ADI_OSAL_FAILED       - If failed to delete mutex
    @return ADI_OSAL_BAD_HANDLE   - If the specified mutex handle is invalid
    @return ADI_OSAL_CALLER_ERROR - If function is invoked from an invalid
                                    location
    @return ADI_OSAL_THREAD_PENDING - one or more thread is pending on the
                                    specified mutex.

    @note 
          Only owner is authorised to release the acquired mutex. But it 
          can "destroyed" by  other task.

*****************************************************************************/


ADI_OSAL_STATUS adi_osal_MutexDestroy(ADI_OSAL_MUTEX_HANDLE const hMutex)
{
    return (adi_osal_MutexDestroyStatic(hMutex));
}


/*!
  ****************************************************************************
  @brief Acquires a mutex with a timeout

  This function is used to lock a mutex (acquire a resource)

    @param[in] hMutex            - Handle of the mutex which need to be acquired
    @param[in] nTimeoutInTicks   - Specify the number of system ticks for
                                   acquiring the mutex

      Valid timeouts are:

        ADI_OSAL_TIMEOUT_NONE       -  No wait. Results in an immediate return
                                       from this service  regardless of whether
                                       or not it was successful

        ADI_OSAL_TIMEOUT_FOREVER    -  Wait option for calling task to suspend
                                       indefinitely until a specified  mutex is
                                       obtained

        1 ....0xFFFFFFFE            -  Selecting a numeric value specifies the
                                       maximum time limit (in system ticks) for
                                       obtaining specified mutex

    @return ADI_OSAL_SUCCESS      - If the specified mutex is locked
                                    successfully
    @return ADI_OSAL_FAILED       - If failed to lock the specified mutex
    @return ADI_OSAL_BAD_HANDLE   - If the specified mutex ID is invalid
    @return ADI_OSAL_TIMEOUT      - If the specified time limit expired.
    @return ADI_OSAL_CALLER_ERROR - If the function is invoked from an invalid
                                    location

    @note On the non-OS implementation of OSAL this API always succeeds in 
          release mode.

*****************************************************************************/

ADI_OSAL_STATUS adi_osal_MutexPend(ADI_OSAL_MUTEX_HANDLE const hMutex, ADI_OSAL_TICKS nTimeoutInTicks)
{

#ifdef OSAL_DEBUG
    if (_adi_osal_IsCurrentLevelISR())
    {
        return ADI_OSAL_CALLER_ERROR;
    }

    if (DUMMY_MUTEX_HANDLE != hMutex )
    {
        return ADI_OSAL_BAD_HANDLE;
    }

    /* Increase the number of times that a mutex has been acquired. This is
     * for error checking only */
    snMutexAcquireCount ++;

#endif /* OSAL_DEBUG */

    return ADI_OSAL_SUCCESS;
}

/*!
  ****************************************************************************

    @brief Unlocks a mutex.

    @param[in] hMutex      - Handle of the mutex which needs to be unlocked

    @return ADI_OSAL_SUCCESS          - If mutex is un locked successfully
    @return ADI_OSAL_FAILED           - If failed unlock mutex
    @return ADI_OSAL_BAD_HANDLE       - If the specified mutex ID is invalid
    @return ADI_OSAL_NOT_MUTEX_OWNER  - If it is called from a task which does
                                        not own it.
    @return ADI_OSAL_CALLER_ERROR     - The function is invoked from an invalid
                                        location

    @note 
         Mutex can be successfully released by its owner : Only the task which
         acquired it can release it. Any attempt to release it by non-owner will
         result in error.

    @note On the non-OS implementation of OSAL this API always succeeds in 
          release mode.
*****************************************************************************/


ADI_OSAL_STATUS adi_osal_MutexPost(ADI_OSAL_MUTEX_HANDLE const hMutex)
{

#ifdef OSAL_DEBUG
    if (_adi_osal_IsCurrentLevelISR())
    {
        return ADI_OSAL_CALLER_ERROR;
    }

    if ((hMutex == NULL) ||
        (hMutex == ADI_OSAL_INVALID_MUTEX ))
    {
        return ADI_OSAL_BAD_HANDLE;
    }

    /* Check the number of times that mutexes has been acquired. The mutex
     * owner must release it so if the count is 0 or less there is an error */

    if (0u >= snMutexAcquireCount)
    {
        return ADI_OSAL_NOT_MUTEX_OWNER;
    }
    snMutexAcquireCount --;

#endif /* OSAL_DEBUG */

    return ADI_OSAL_SUCCESS;
}


/*
**
** EOF: 
**
*/

