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
    @file adi_osal_noos_sem.c

    Operating System Abstraction Layer - OSAL for non-OS - Semaphore
    functions

    This file contains the Semaphore APIs for the non-OS implementation of
    OSAL

    @internal

    SUPRESSED MISRA RULES ARE:
        See adi_osal_noos.c for the list of the MISRA Rules that have been
        suppressed for the non-OS OSAL.

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
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the structure into an opaque pointer")
#endif

/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=any")
#pragma file_attr(  "prefersMemNum=50")
#endif





/*=============  D A T A  =============*/



/*=============  C O D E  =============*/

/*!
  ****************************************************************************
    @brief Creates a counting semaphore with the memory which has already been 
            provided.

    @param[in] nSemObjSize - Size of the memory passed for the creation of
                               the semaphore
    @param[in] pSemObject  - Area of memory provided to us for the semaphore
    
    @param[out] phSem      - Pointer to a location to write the returned
                             semaphore ID
    @param[in]  nInitCount - Initial value for the creation of a counting
                             semaphore.
                             Semaphore will be created "unavailable" state if
                             "nInitCount" is equal to zero.

    @return ADI_OSAL_SUCCESS      - If semaphore is created successfully
    @return ADI_OSAL_FAILED       - If failed to create semaphore
    @return ADI_OSAL_BAD_COUNT    - The value specified in nInitCount is too
                                    large for the RTOS
    @return ADI_OSAL_CALLER_ERROR - If the call is made from an invalid location
                                    (i.e an ISR)

  Note:
      phSem  set  to "ADI_OSAL_INVALID_SEM" if semaphore creation is failed.
 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_SemCreateStatic(void* const pSemObject, uint32_t nSemObjSize, ADI_OSAL_SEM_HANDLE *phSem, uint32_t nInitCount)

{
    ADI_OSAL_STATUS    eRetStatus;
    OSAL_SEMAPHORE    *pSem = pSemObject;

#ifdef OSAL_DEBUG
    if (NULL == phSem)
    { 
        return (ADI_OSAL_FAILED);
    }
    if (_adi_osal_IsCurrentLevelISR())
    {
        *phSem = ADI_OSAL_INVALID_SEM;
        return (ADI_OSAL_CALLER_ERROR);
    }
    if ( nSemObjSize < sizeof(OSAL_SEMAPHORE) )
    {
        *phSem = ADI_OSAL_INVALID_SEM;
        return (ADI_OSAL_MEM_TOO_SMALL);
    }

    if (nInitCount > ADI_OSAL_SEM_MAX_COUNT)
    {
        *phSem = ADI_OSAL_INVALID_SEM;
        return (ADI_OSAL_BAD_COUNT);
    }

    if ((false == _adi_osal_IsMemoryAligned(pSemObject)) || (NULL == pSemObject) )
    {
        *phSem = ADI_OSAL_INVALID_SEM;
        return (ADI_OSAL_BAD_MEMORY);
    }
    
#endif /* OSAL_DEBUG */

    pSem->nCount = nInitCount;
    pSem->bActiveS = true;

    *phSem = (ADI_OSAL_SEM_HANDLE) pSem; /* hits MISRA 11.4 */

    return (ADI_OSAL_SUCCESS);
}


/*!
  ****************************************************************************
    @brief Creates a counting semaphore.

    @param[out] phSem      - Pointer to a location to write the returned
                             semaphore ID
    @param[in]  nInitCount - Initial value for the creation of a counting
                             semaphore.
                             Semaphore will be created "unavailable" state if
                             "nInitCount" is equal to zero.

    @return ADI_OSAL_SUCCESS      - If semaphore is created successfully
    @return ADI_OSAL_FAILED       - If failed to create semaphore
    @return ADI_OSAL_BAD_COUNT    - The value specified in nInitCount is too
                                    large for the RTOS
    @return ADI_OSAL_CALLER_ERROR - If the call is made from an invalid location
                                    (i.e an ISR)

  Note:
      phSem  set  to "ADI_OSAL_INVALID_SEM" if semaphore creation is failed.
 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_SemCreate(ADI_OSAL_SEM_HANDLE *phSem, uint32_t nInitCount)
{
    ADI_OSAL_STATUS eRetStatus;
    OSAL_SEMAPHORE_PTR pSemObj;

#ifdef OSAL_DEBUG
    if (NULL == phSem)
    {
        return (ADI_OSAL_FAILED);
    }
    if (_adi_osal_IsCurrentLevelISR())
    {
        *phSem = ADI_OSAL_INVALID_SEM;
        return (ADI_OSAL_CALLER_ERROR);
    }
#endif

    eRetStatus = _adi_osal_MemAlloc((void**) &pSemObj, sizeof(OSAL_SEMAPHORE));
    if (ADI_OSAL_SUCCESS != eRetStatus)
    {
        *phSem = ADI_OSAL_INVALID_SEM;
        return (eRetStatus);
    }

    eRetStatus = adi_osal_SemCreateStatic(pSemObj, sizeof(OSAL_SEMAPHORE), phSem, nInitCount);

    if (eRetStatus != ADI_OSAL_SUCCESS)
    {
        _adi_osal_MemFree(pSemObj);
    }

    return (eRetStatus);
}

/*!
  ****************************************************************************
  @brief Returns the size of a semaphore object.

  This function can be used by the adi_osal_SemCreateStatic function to
  determine what the object size should be for this particular RTOS
  implementation.

  Parameters:
      None

    @return size of a semaphore object in bytes.

    @see adi_osal_SemCreateStatic

*****************************************************************************/

uint32_t adi_osal_SemGetObjSize(void)
{
    return ( sizeof(OSAL_SEMAPHORE) );
}

/*!
  ****************************************************************************
    @brief Deletes a specified semaphore without freeing memory.

    @param[in]  hSem      - The handle of the semaphore which need to be deleted

    @return ADI_OSAL_SUCCESS          - If semaphore is deleted successfully
    @return ADI_OSAL_FAILED           - If failed to delete semaphore
    @return ADI_OSAL_BAD_HANDLE       - If the specified semaphore handle is
                                        invalid
    @return ADI_OSAL_CALLER_ERROR     - If the call is made from an invalid
                                        location (i.e an ISR).
    @return ADI_OSAL_THREAD_PENDING   - If a thread is pending on the specified
                                        semaphore

 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_SemDestroyStatic( ADI_OSAL_SEM_HANDLE const hSem )
{

    OSAL_SEMAPHORE* pSemNative = (OSAL_SEMAPHORE*) hSem; /* hits MISRA 11.4 */

#ifdef OSAL_DEBUG
    if (_adi_osal_IsCurrentLevelISR())
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if ((hSem == ADI_OSAL_INVALID_SEM) ||
        (hSem == NULL ))
    {
        return (ADI_OSAL_BAD_HANDLE);
    }

    /* even though the memory is getting freed, this flag is set so that future operations
       on that handle will fail */
    pSemNative->bActiveS = false;
#endif /* OSAL_DEBUG */


    return ( ADI_OSAL_SUCCESS );

}
/*!
  ****************************************************************************
    @brief Deletes a specified semaphore.

    @param[in]  hSem      - The handle of the semaphore which need to be deleted

    @return ADI_OSAL_SUCCESS          - If semaphore is deleted successfully
    @return ADI_OSAL_FAILED           - If failed to delete semaphore
    @return ADI_OSAL_BAD_HANDLE       - If the specified semaphore handle is
                                        invalid
    @return ADI_OSAL_CALLER_ERROR     - If the call is made from an invalid
                                        location (i.e an ISR).
    @return ADI_OSAL_THREAD_PENDING   - If a thread is pending on the specified
                                        semaphore

 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_SemDestroy( ADI_OSAL_SEM_HANDLE const hSem )
{

    ADI_OSAL_STATUS eRetStatus;
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle into a pointer to a useful structure")
#endif
    OSAL_SEMAPHORE* pSemNative = (OSAL_SEMAPHORE*) hSem;
#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

    eRetStatus = adi_osal_SemDestroyStatic(hSem);
    if (eRetStatus == ADI_OSAL_SUCCESS )
    {
        _adi_osal_MemFree(pSemNative);
    }

    return (eRetStatus);
}


/*!
  ****************************************************************************
    @brief Waits for access to a semaphore

    If the specified semaphore is acquired, its count will be decremented.


    @param[in]  hSem             - Handle of the semaphore to obtain
    @param[in]  nTimeoutInTicks  - Specify the number of system ticks after
                                   which obtaining the semaphore will return.

            Valid timeouts are:

            ADI_OSAL_TIMEOUT_NONE     No wait. Results in an immediate return
                                      from this service regardless of whether
                                      or not it was successful
            ADI_OSAL_TIMEOUT_FOREVER  Wait option for calling task to suspend
                                      indefinitely  until a semaphore instance
                                      is obtained
            1....0xFFFFFFFE           Selecting a numeric value specifies the
                                      maximum time limit (in system ticks ) for
                                      obtaining a semaphore


    @return ADI_OSAL_SUCCESS    -  If semaphore acquired successfully
    @return ADI_OSAL_FAILED     -  If an error occured while acquiring the
                                   semaphore
    @return ADI_OSAL_TIMEOUT    -  If the API failed to acquire the semaphore
                                   within the specified time limit
    @return ADI_OSAL_BAD_HANDLE -  If the specified semaphore handle is invalid
    @return ADI_OSAL_BAD_TIME   -  If the specified time is invalid for the RTOS
    @return ADI_OSAL_CALLER_ERROR- If the function is invoked from an invalid
                                   location

 *****************************************************************************/

ADI_OSAL_STATUS  adi_osal_SemPend(ADI_OSAL_SEM_HANDLE const hSem, ADI_OSAL_TICKS nTimeoutInTicks)
{
    OSAL_SEMAPHORE* pSemNative = (OSAL_SEMAPHORE*) hSem; /* hits MISRA 11.4 */
    ADI_OSAL_STATUS eRetStatus = ADI_OSAL_FAILED;
    uint32_t nIntState;

#ifdef OSAL_DEBUG
     if (( nTimeoutInTicks != ADI_OSAL_TIMEOUT_NONE )   &&
         ( nTimeoutInTicks != ADI_OSAL_TIMEOUT_FOREVER))
     {
          return (ADI_OSAL_BAD_TIME);
     }

     if (_adi_osal_IsCurrentLevelISR())
     {
         return (ADI_OSAL_CALLER_ERROR);
     }

    if ((hSem == ADI_OSAL_INVALID_SEM) ||
        (hSem == NULL) ||
        (pSemNative->bActiveS == false))
    {
        return (ADI_OSAL_BAD_HANDLE);
    }
#endif /* OSAL_DEBUG */

    /* if the timeout is none we return immediately decrementing the count if
       it was greater than one
     */
    if (ADI_OSAL_TIMEOUT_NONE == nTimeoutInTicks)
    {
        nIntState = _adi_osal_InterruptsDisable();
        if (pSemNative->nCount > 0u)
        {
            pSemNative->nCount--;
            eRetStatus = ADI_OSAL_SUCCESS;
        }
        else
        {
            eRetStatus = ADI_OSAL_TIMEOUT;
        }

        _adi_osal_InterruptsEnable(nIntState);
        return(eRetStatus);
    }
    
    /* here we know that the timeout is forever - otherwise we would have 
     * got an error in the debug case
     */

    while ( eRetStatus == ADI_OSAL_FAILED )
    {
        nIntState = _adi_osal_InterruptsDisable();
        if (pSemNative->nCount > 0u)
        {
            pSemNative->nCount--;
            eRetStatus = ADI_OSAL_SUCCESS;
        }
        _adi_osal_InterruptsEnable(nIntState);
    }

    /* the semaphore was acquired */
    return ( ADI_OSAL_SUCCESS );
}


/*!
  ****************************************************************************
    @brief Releases a semaphore

    The semaphore count will be incremented if the specified semaphore is
    released successfully and there were no threads pending on it

    @param[in]  hSem - Handle of the semaphore to be released

    @return ADI_OSAL_SUCCESS     - If semaphore released successfully
    @return ADI_OSAL_FAILED      - If an error occured while releasing the
                                   specified semaphore
    @return ADI_OSAL_BAD_HANDLE  - If the specified semaphore handle is invalid

*****************************************************************************/

ADI_OSAL_STATUS adi_osal_SemPost(ADI_OSAL_SEM_HANDLE const hSem)
{
    OSAL_SEMAPHORE* pSemNative = (OSAL_SEMAPHORE*) hSem; /* hits MISRA 11.4 */
    ADI_OSAL_STATUS eRetStatus = ADI_OSAL_FAILED;
    uint32_t nIntState;

#ifdef OSAL_DEBUG
    if ((hSem == ADI_OSAL_INVALID_SEM) ||
        (hSem == NULL) ||
        (pSemNative->bActiveS == false))
    {
        return (ADI_OSAL_BAD_HANDLE);
    }
#endif /* OSAL_DEBUG */

    nIntState = _adi_osal_InterruptsDisable();
    pSemNative->nCount++;
    _adi_osal_InterruptsEnable(nIntState);

    return( ADI_OSAL_SUCCESS );
}

/*
**
** EOF: 
**
*/

