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
    @file adi_osal_noos_event.c

    Operating System Abstraction Layer - OSAL for non-OS - Eventgroup
    functions

    This file contains the Eventgroup APIs for the non-OS implementation of
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
    @brief Creates an event group with the memory which has already been
            provided.

    @param[in] nEventObjSize - Size of the memory passed for the creation of
                               the event group
    @param[in] pEventObject  - Area of memory provided to us for the event group

    This is typically used to synchronize threads with events that happen in
    the system.

    @param[out] phEventGroup - Pointer to a location to write the returned
                               event group handle

    @return ADI_OSAL_SUCCESS      - If event group is created successfully
    @return ADI_OSAL_FAILED       - If failed to create event group
    @return ADI_OSAL_CALLER_ERROR - If function is invoked from an invalid
                                    location

*****************************************************************************/

ADI_OSAL_STATUS
adi_osal_EventGroupCreateStatic(
		void* const pEventObject,
		uint32_t nEventObjSize,
		ADI_OSAL_EVENT_HANDLE *phEventGroup
		)
{
    OSAL_EVENT *pEventNative = pEventObject;

    ADI_OSAL_STATUS eRetStatus, eHeapResult;

#ifdef OSAL_DEBUG
    if (NULL == phEventGroup)
    {
        return(ADI_OSAL_FAILED);
    }

    if (CALLED_FROM_AN_ISR)
    {
        *phEventGroup = ADI_OSAL_INVALID_EVENT_GROUP;
        return (ADI_OSAL_CALLER_ERROR);
    }
    if ( nEventObjSize < sizeof(OSAL_EVENT) )
    {
        *phEventGroup = ADI_OSAL_INVALID_EVENT_GROUP;
        return (ADI_OSAL_MEM_TOO_SMALL);
    }

    if ((false == _adi_osal_IsMemoryAligned(pEventObject)) || (NULL == pEventObject) )
    {
        *phEventGroup = ADI_OSAL_INVALID_EVENT_GROUP;
        return (ADI_OSAL_BAD_MEMORY);
    }

#endif /* OSAL_DEBUG */

    /* Flags are initially all created as unset (0x0) */
    pEventNative->eventBits = 0u;
    pEventNative->bActiveE = true;

    *phEventGroup = (ADI_OSAL_EVENT_HANDLE)pEventNative;

    return (ADI_OSAL_SUCCESS);
}


/*!
  ****************************************************************************
    @brief Creates a event group.

    @param[out] phEventGroup    - Pointer to a location to write the returned
                             event group ID

    @return ADI_OSAL_SUCCESS      - If event is created successfully
    @return ADI_OSAL_FAILED       - If failed to create event
    @return ADI_OSAL_CALLER_ERROR - If the call is made from an invalid location
                                    (i.e an ISR)

  Note:
      phEventGroup set to "ADI_OSAL_INVALID_EVENT_GROUP" if event creation is failed.
 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_EventGroupCreate(ADI_OSAL_EVENT_HANDLE *phEventGroup)
{
    ADI_OSAL_STATUS eRetStatus;
    OSAL_EVENT *pEventObj;

#ifdef OSAL_DEBUG
    if (NULL == phEventGroup)
    {
        return(ADI_OSAL_FAILED);
    }

    if (CALLED_FROM_AN_ISR)
    {
        *phEventGroup = ADI_OSAL_INVALID_EVENT_GROUP;
        return (ADI_OSAL_CALLER_ERROR);
    }
#endif

#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle type into a pointer to a useful structure")
    eRetStatus =  _adi_osal_MemAlloc((void**) &pEventObj, sizeof(OSAL_EVENT));
    if (ADI_OSAL_SUCCESS != eRetStatus)
    {
        *phEventGroup = ADI_OSAL_INVALID_EVENT_GROUP;
        return (eRetStatus);
    }
#pragma diag(pop)

    eRetStatus = adi_osal_EventGroupCreateStatic(pEventObj, sizeof(OSAL_EVENT), phEventGroup);

    if (eRetStatus != ADI_OSAL_SUCCESS)
    {
        _adi_osal_MemFree(pEventObj);
    }

    return (eRetStatus);
}
/*!
  ****************************************************************************
  @brief Returns the size of a event group object.

  This function can be used by the adi_osal_EventGroupCreateStatic function to
  determine what the object size should be for this particular RTOS
  implementation.

  Parameters:
      None

    @return size of a event group object in bytes.

    @see adi_osal_EventGroupCreateStatic

*****************************************************************************/

uint32_t adi_osal_EventGroupGetObjSize(void)
{
    return ( sizeof(OSAL_EVENT) );
}


/*!
  ****************************************************************************
    @brief Destroys the specified Event group without freeing memory

    @param[in] hEventGroup - handle of the event group to be destroyed

    @return ADI_OSAL_SUCCESS         - If event group is destroyed
                                       successfully
    @return ADI_OSAL_FAILED          - If failed to create event group
    @return ADI_OSAL_CALLER_ERROR    - If function is invoked from an
                                       invalid location
    @return ADI_OSAL_THREAD_PENDING  - If the destruction of the Event Group
                                       is not possible because threads are
                                       pending on events from that group
    @return ADI_OSAL_BAD_HANDLE      - Invalid event flag group ID

*****************************************************************************/

ADI_OSAL_STATUS adi_osal_EventGroupDestroyStatic(ADI_OSAL_EVENT_HANDLE const hEventGroup)
{
    ADI_OSAL_STATUS eRetStatus;
#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle type into a native uCOS pointer")
    OSAL_EVENT* pEventNative = (OSAL_EVENT*) hEventGroup;
#pragma diag(pop)


#ifdef OSAL_DEBUG
    if (CALLED_FROM_AN_ISR)
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if ((NULL == hEventGroup) ||
    	(ADI_OSAL_INVALID_EVENT_GROUP == hEventGroup) ||
        (pEventNative->bActiveE == false))
    {
        return(ADI_OSAL_BAD_HANDLE);
    }

    /* even though the memory is getting freed, this flag is set so that future operations
       on that handle will fail */
    pEventNative->bActiveE = false;
#endif /* OSAL_DEBUG */

    _adi_osal_MemFree(pEventNative);

    return ( ADI_OSAL_SUCCESS );
}


/*!
  ****************************************************************************
    @brief Deletes a specified event group.

    @param[in]  hEvent      - The handle of the event group which need to be deleted

    @return ADI_OSAL_SUCCESS          - If event group is deleted successfully
    @return ADI_OSAL_FAILED           - If failed to delete event group
    @return ADI_OSAL_BAD_HANDLE       - If the specified event group handle is
                                        invalid
    @return ADI_OSAL_CALLER_ERROR     - If the call is made from an invalid
                                        location (i.e an ISR).
    @return ADI_OSAL_THREAD_PENDING   - If a thread is pending on the specified
                                        event group

 *****************************************************************************/

ADI_OSAL_STATUS adi_osal_EventGroupDestroy( ADI_OSAL_EVENT_HANDLE const hEventGroup )
{

    ADI_OSAL_STATUS eRetStatus;
#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle into a pointer to a useful structure")
    OSAL_EVENT* pEventNative = (OSAL_EVENT*) hEventGroup;
#pragma diag(pop)

    eRetStatus = adi_osal_EventGroupDestroyStatic(hEventGroup);
    if (eRetStatus == ADI_OSAL_SUCCESS )
    {
        _adi_osal_MemFree(pEventNative);
    }

    return (eRetStatus);
}


/*!
  ****************************************************************************
    @brief Waits for event flags

    @param[in] hEventGroup      - handle of the event group to use
    @param[in] nRequestedEvents - Specifies requested event flags.
    @param[in] eGetOption       - Specifies whether all bits need to be
                                  set/cleared OR any of the bits to be set.
    @param[in] nTimeoutInTicks  - Timeout for the event flag in system ticks.
    @param[in] pnReceivedEvents - Pointer to destination of where the retrieved
                                  event flags are placed.


         The following options are valid for setting flag eGetOption.

            ADI_OSAL_EVENT_FLAG_SET_ANY  - check any of the bits specified by
                                           the nRequestedEvents is set
            ADI_OSAL_EVENT_FLAG_SET_ALL  - check all the bits specified by the
                                           nRequestedEvents are set.

         Valid options for nTimeoutInTicks  are:

           ADI_OSAL_TIMEOUT_NONE     -  No wait. Results in an immediate return
                                        from this service regardless of whether
                                        or not it was successful
           ADI_OSAL_TIMEOUT_FOREVER  -  Wait option for calling task to suspend
                                        indefinitely until the required flags are
                                        set.
           1 ... 0XFFFFFFFE          -  Other timeout values not currently supported

    @return ADI_OSAL_SUCCESS      -  If there is no error while retrieving the
                                     event flags. This does not indicate event
                                     flag condition - the user must read the
                                     flags separately.
    @return ADI_OSAL_FAILED       -  If an error occurred while retrieving event
                                     flags.
    @return ADI_OSAL_BAD_HANDLE   -  If the specified event group is invalid.
    @return ADI_OSAL_TIMEOUT      -  If the function failed to get the specified
                                     event flag(s) due to timeout.
    @return ADI_OSAL_CALLER_ERROR -  If the function is invoked from an invalid
                                     location (i.e an ISR)
    @return ADI_OSAL_BAD_OPTION   -  If "eGetOption" specifies a wrong option.
*****************************************************************************/

ADI_OSAL_STATUS adi_osal_EventPend (ADI_OSAL_EVENT_HANDLE const hEventGroup,
                                    ADI_OSAL_EVENT_FLAGS        nRequestedEvents,
                                    ADI_OSAL_EVENT_FLAG_OPTION  eGetOption,
                                    ADI_OSAL_TICKS              nTimeoutInTicks,
                                    ADI_OSAL_EVENT_FLAGS        *pnReceivedEvents)
{
    ADI_OSAL_EVENT_FLAGS nRetValue;
#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle type into a native uCOS pointer")
    OSAL_EVENT     *pEventNative = (OSAL_EVENT*) hEventGroup;
#pragma diag(pop)


#ifdef OSAL_DEBUG
    if (( nTimeoutInTicks != ADI_OSAL_TIMEOUT_NONE )   &&
        ( nTimeoutInTicks != ADI_OSAL_TIMEOUT_FOREVER))
    {
         return (ADI_OSAL_BAD_TIME);
    }

    if (CALLED_FROM_AN_ISR)
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if ( (eGetOption != ADI_OSAL_EVENT_FLAG_ANY) &&
         (eGetOption != ADI_OSAL_EVENT_FLAG_ALL) )
    {
        return (ADI_OSAL_BAD_OPTION);
    }

    if ((NULL == hEventGroup) ||
    	(ADI_OSAL_INVALID_EVENT_GROUP == hEventGroup) ||
        (pEventNative->bActiveE == false))
    {
        return(ADI_OSAL_BAD_HANDLE);
    }
#endif /* OSAL_DEBUG */

	/* We don't need to disable interrupts here, because we
	 * aren't modifying the contents of eventBits. Because
	 * eventBits is volatile we will see any change in its
	 * state.
	 */
	if (ADI_OSAL_EVENT_FLAG_ANY == eGetOption)
	{
		if (ADI_OSAL_TIMEOUT_FOREVER == nTimeoutInTicks)
		{
			do
			{
				/* Mask out the bits that we aren't interested in */
				nRetValue = (pEventNative->eventBits & nRequestedEvents);

				/* For the 'any' case we only need to test for the masked
				 * bits being non-zero.
				 */
			} while (0u == nRetValue);
		}
		else /* zero timeout - don't wait */
		{
			nRetValue = (pEventNative->eventBits & nRequestedEvents);

			if (0u == nRetValue)
			{
				return ADI_OSAL_TIMEOUT;
			}
		}
	}
	else
	{
		if (ADI_OSAL_TIMEOUT_FOREVER == nTimeoutInTicks)
		{
			do
			{
				/* Mask out the bits that we aren't interested in */
				nRetValue = (pEventNative->eventBits & nRequestedEvents);

				/* For the 'all' case we need to test
				 * for the masked bits being equal to the requested bits.
				 */
			} while (nRequestedEvents != nRetValue);
		}
		else /* zero timeout - don't wait */
		{
			nRetValue = (pEventNative->eventBits & nRequestedEvents);

			if (nRequestedEvents != nRetValue)
			{
				return ADI_OSAL_TIMEOUT;
			}
		}
	}

    *pnReceivedEvents = nRetValue;

    return( ADI_OSAL_SUCCESS );
}


/*!
  ****************************************************************************
    @brief Sets one or more event flags.

    @param[in] hEventGroup      - handle of the event group to use
    @param[in] nEventFlags      - Specifies the event flags to set.
                                  'ORed' into the current event flags.

    @return ADI_OSAL_SUCCESS    - If the event flag(s) are posted successfully.
    @return ADI_OSAL_FAILED     - If the function failed to post the event
                                  flags.
    @return ADI_OSAL_BAD_HANDLE - If the specified event group is invalid
    @return ADI_OSAL_BAD_EVENT  - If the events presented in the nEventFlags
                                  variable are not valid

*****************************************************************************/

ADI_OSAL_STATUS  adi_osal_EventSet( ADI_OSAL_EVENT_HANDLE const hEventGroup,
                                     ADI_OSAL_EVENT_FLAGS nEventFlags)
{
    ADI_OSAL_STATUS eRetStatus;
    uint32_t nIntState;
#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle type into a native uCOS pointer")
    OSAL_EVENT *pEventNative = (OSAL_EVENT*) hEventGroup;
#pragma diag(pop)

#ifdef OSAL_DEBUG
    if ((NULL == hEventGroup) ||
    	(ADI_OSAL_INVALID_EVENT_GROUP == hEventGroup) ||
        (pEventNative->bActiveE == false))
    {
        return(ADI_OSAL_BAD_HANDLE);
    }
#endif /* OSAL_DEBUG */

    nIntState = _adi_osal_InterruptsDisable();
    pEventNative->eventBits |= nEventFlags;
    _adi_osal_InterruptsEnable(nIntState);

    return( ADI_OSAL_SUCCESS );
}



/*!
  ****************************************************************************
    @brief Clears one or more event flags.

    @param[in] hEventGroup      - Handle of the event group to use
    @param[in] nEventFlags      - Specifies the event flags to cleared.

    @return ADI_OSAL_SUCCESS    - If the event flag(s) are cleared successfully.
    @return ADI_OSAL_FAILED     - If the function failed to clear the event
                                  flags.
    @return ADI_OSAL_BAD_HANDLE - If the specified event group is invalid
    @return ADI_OSAL_BAD_EVENT  - If the events presented in the nEventFlags
                                  variable are not valid

*****************************************************************************/

ADI_OSAL_STATUS  adi_osal_EventClear(ADI_OSAL_EVENT_HANDLE const hEventGroup, ADI_OSAL_EVENT_FLAGS nEventFlags)
{
    ADI_OSAL_STATUS eRetStatus;
    uint32_t nIntState;
#pragma diag(push)
#pragma diag(suppress:misra_rule_11_4 : "typecasting is necessary to convert the handle type into a native uCOS pointer")
    OSAL_EVENT *pEventNative = (OSAL_EVENT*) hEventGroup;
#pragma diag(pop)

#ifdef OSAL_DEBUG
    if ((NULL == hEventGroup) ||
    	(ADI_OSAL_INVALID_EVENT_GROUP == hEventGroup) ||
        (pEventNative->bActiveE == false))
    {
        return(ADI_OSAL_BAD_HANDLE);
    }
#endif /* OSAL_DEBUG */

    nIntState = _adi_osal_InterruptsDisable();
    pEventNative->eventBits &= ~nEventFlags;
    _adi_osal_InterruptsEnable(nIntState);

    return( ADI_OSAL_SUCCESS );
}


/*
**
** EOF: 
**
*/

