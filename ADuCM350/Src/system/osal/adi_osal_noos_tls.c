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
    @file adi_osal_noos_tls.c

    Operating System Abstraction Layer - OSAL for non-OS - TLS
    functions

    This file contains the Thread Local Storage APIs for the non-OS 
    implementation of OSAL

    @internal

    SUPRESSED MISRA RULES ARE:
        See adi_osal_noos.c for the list of the MISRA Rules that have been
        suppressed for the no-OS OSAL.

    @endinternal
*/


/*=============  I N C L U D E S   =============*/

#include <stdlib.h>                                                             /* for NULL definition */
#include "adi_osal.h"
#include "osal_noos.h"
#include "osal_common.h"


#ifdef _MISRA_RULES
/*Rule 14.7 indicates that a function shall have a single exit point */
#pragma diag(suppress:misra_rule_14_7:"Allowing several point of exit (mostly for handling parameter error checking) increases the code readability and therefore maintainability")
/* Rule-11.3 indicates that typecast of a integer value into pointer is invalid */
#pragma diag(suppress:misra_rule_11_3 : "typecasting is necessary to convert the OSAL Slot Value void* type to the underlying OS_REG integral type")
#endif

/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=any")
#pragma file_attr(  "prefersMemNum=50")
#endif


/* Thread local storage numbers/keys consist of two values:
 *  - The signature, indicating that the key refers to a TLS slot 0xNNNNNNXX
 *  - The index of the slot, from 0 to Max Slots 0xXXXXXXNN
 */
#define TLS_SIGNATURE ((uint32_t)(0x544C5300))

/* Masks used to extract either the signature component, or the slot index component
 * of a TLS Key.
 */
#define TLS_MASK_SIG ((uint32_t)(0xFFFFFF00))
#define TLS_MASK_NUM ((uint32_t)(0x000000FF))

/*=============  D A T A  =============*/

/*!
    @internal
    @var _adi_osal_gnNumSlots
         stores the number of TLS slots requested by the user during
         initialization.
    @endinternal
*/
uint32_t _adi_osal_gnNumSlots = ADI_OSAL_MAX_NUM_TLS_SLOTS;


/*!
    @internal
    @var _adi_osal_gTLSUsedSlots
         Word used in the management of allocated TLS slots.
         Bits are used to represent the status of the slot. Bit 0 corresponds
         to slot number 0 and slot number 30 corresponds bit number 30. 
         A slot is free if the corresponding bit is clear and a
         slot is acquired if the corresponding bit is set. Initially all
         the slot bits are clear.
    @endinternal
*/
static uint32_t _adi_osal_gTLSUsedSlots = 0u;

/*
    variable: saTLSSlotsTable
    Hold the TLS values for each slot. Since in the noOS implementation there are
    only ONE 'thread', this array can be allocated statically.
    Make the array the maximum supported size (ADI_OSAL_MAX_NUM_TLS_SLOTS)
*/
static ADI_OSAL_SLOT_VALUE saTLSSlotTable[ADI_OSAL_MAX_NUM_TLS_SLOTS];



/*=============  C O D E  =============*/

#pragma inline;
#if defined (__ECC__)
#pragma always_inline;
#elif defined (__ICCARM__)
#pragma inline=forced
#endif
bool IsValidTLSKey(ADI_OSAL_TLS_SLOT_KEY key)
{
    return (TLS_SIGNATURE == (key & TLS_MASK_SIG));
}

/*!
  ****************************************************************************
    @brief Allocates a thread slot and returns the slot number

    This function allocates the slot (i.e memory location of a word size ) 
    from the thread local storage buffer and returns the slot number.

    @param[out] pnThreadSlotKey       - Pointer to return the slot number if a
                                        free slot is found.  Must be populated with
                                        ADI_OSAL_TLS_UNALLOCATED.  If a valid slot
                                        is already present in the supplied address
                                        this API returns success.

    @param[in] pTerminateCallbackFunc - Pointer to a function that gets called 
                                        when the slot is freed.  Can be NULL
                                        if the callback function is not required.

    @return ADI_OSAL_SUCCESS       - If the function successfully allocated the 
                                     slot.
    @return ADI_OSAL_FAILED        - If the function failed to allocate the 
                                     slot.
    @return ADI_OSAL_CALLER_ERROR  - If the function is invoked from an invalid 
                                     location (i.e an ISR)

    Note:
     "pnThreadSlotKey"  will be set to  ADI_OSAL_INVALID_THREAD_SLOT   if
      failed to find a free slot.
  @ingroup thread local storage
*****************************************************************************/
ADI_OSAL_STATUS
adi_osal_ThreadSlotAcquire(ADI_OSAL_TLS_SLOT_KEY     *pnThreadSlotKey,
                           ADI_OSAL_TLS_CALLBACK_PTR pTerminateCallbackFunc)
{
    uint32_t nSlotIndex;
    uint32_t nSlotBit;

    ADI_OSAL_STATUS  eRetStatus = ADI_OSAL_FAILED;

#ifdef OSAL_DEBUG
    if (CALLED_FROM_AN_ISR)
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if(NULL == pnThreadSlotKey)
    {
    	/* The pointer is NULL */
    	return ADI_OSAL_FAILED;
    }
#endif /* OSAL_DEBUG */


    /* If the passed-in slot key has already been allocated, then we return
     * successfully.  We check that -
     *  - It has the correct TLS signature
     *  - The slot has been (and is still) allocated.
     */
    if(IsValidTLSKey(*pnThreadSlotKey))
    {
        /* Extract the slot number from the TLS key, and convert to a bit
         * position */
        nSlotBit = 1ul << (*pnThreadSlotKey & TLS_MASK_NUM);
        if(0u != (_adi_osal_gTLSUsedSlots & nSlotBit))
        {
                /* The slot has previously been allocated using the supplied
                 * address */
                return(ADI_OSAL_SUCCESS);
        }
    }

    /* Before we allocate a slot, the address to be written-to must have
     * an "unallocated" key.
     */
    if(*pnThreadSlotKey != ADI_OSAL_TLS_UNALLOCATED)
    {
        *pnThreadSlotKey = ADI_OSAL_INVALID_THREAD_SLOT;
        return(eRetStatus);
    }

    /* We start looking for a an available slot from the g_TLSIndexBase.  Anything
     * below this index will be ignored.
     */
    for (nSlotIndex = 0u; nSlotIndex < _adi_osal_gnNumSlots; nSlotIndex++)
    {
        /* Convert the slot index into its bit position */
        nSlotBit = ((uint32_t) 1 << (nSlotIndex));

        /* if the corresponding bit in the available slots global is not set, then we
         * use it as the free slot.
         */
        if (0u == (_adi_osal_gTLSUsedSlots & nSlotBit))
        {
            /* a free slot has been found. */

            /*
             * Set the slot bit, indicating it's in use.
             */
        	_adi_osal_gTLSUsedSlots      |= nSlotBit;

        	/* We do not need to store the callback function, as
        	 * we only have a single thread of execution that does
        	 * not get destroyed.
        	 */
        	eRetStatus = ADI_OSAL_SUCCESS;
            break;
        }
    }

    if ( eRetStatus == ADI_OSAL_SUCCESS )
    {
        /* Combine the signature and the slot number to form a slot key */
        *pnThreadSlotKey = (nSlotIndex | TLS_SIGNATURE);
    }
    else
    {
        /* only write result in case of failure */
        *pnThreadSlotKey = ADI_OSAL_INVALID_THREAD_SLOT;
    }

    return(eRetStatus);
}

/*!
  ****************************************************************************
    @brief Frees the specified slot in the local storage buffer.

    @param[in] nThreadSlotKey     - slot which needs to be freed.

    @return ADI_OSAL_SUCCESS      - If the function successfully freed the
                                    slot.
    @return ADI_OSAL_FAILED       - If the function tried to free a slot
                                    which was not allocated.
    @return ADI_OSAL_BAD_SLOT_KEY - If the specified slot key is invalid.
    @return ADI_OSAL_CALLER_ERROR - If the function is invoked from an
                                    invalid location (i.e an ISR)
*****************************************************************************/
ADI_OSAL_STATUS
adi_osal_ThreadSlotRelease(ADI_OSAL_TLS_SLOT_KEY nThreadSlotKey)
{

    ADI_OSAL_STATUS eRetStatus = ADI_OSAL_FAILED;
    uint32_t nSlotBit;
    uint32_t nSlotIndex;

    /* Remove the TLS signature from the slot key */
    nSlotIndex = (nThreadSlotKey & TLS_MASK_NUM);

#ifdef OSAL_DEBUG
    if (CALLED_FROM_AN_ISR)
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if( (!IsValidTLSKey(nThreadSlotKey)) || (nSlotIndex >= _adi_osal_gnNumSlots) )
    {
        /* The slot number does not contain the TLS signature */
        return ADI_OSAL_BAD_SLOT_KEY;
    }
#endif /* OSAL_DEBUG */


	nSlotBit = (uint32_t)1u << nSlotIndex;

	/* Check for the bit status : it should be set (allocated) */
	if( 0u != (nSlotBit & _adi_osal_gTLSUsedSlots) )
	{
		/* Make this TLS slot available for reuse. */
		_adi_osal_gTLSUsedSlots &= ~(nSlotBit);
		eRetStatus = ADI_OSAL_SUCCESS;
	}

    return(eRetStatus);
}

/*!
  ****************************************************************************
    @brief Stores the given value in the specified TLS slot.

    @param[out] nThreadSlotKey     - Slot key for the Thread Local Buffer in
                                     which "SlotValue" to be stored.
    @param[in] slotValue           - Value to be stored.

    @return ADI_OSAL_SUCCESS       - If the function successfully stored the 
                                     value in the specified slot.
    @return ADI_OSAL_FAILED        - If there was an error trying to store
                                     the value in the slot which is not
                                     acquired
    @return ADI_OSAL_BAD_SLOT_KEY  - If the specified slot key is invalid.
    @return ADI_OSAL_CALLER_ERROR  - If the function is invoked from an
                                     invalid location (i.e an ISR)
*****************************************************************************/
ADI_OSAL_STATUS
adi_osal_ThreadSlotSetValue(ADI_OSAL_TLS_SLOT_KEY nThreadSlotKey,
                            ADI_OSAL_SLOT_VALUE   slotValue)
{
    uint32_t nSlotBit;
    uint32_t nSlotIndex;

    ADI_OSAL_STATUS eRetStatus = ADI_OSAL_FAILED;

    /* Remove the TLS signature from the slot number */
    nSlotIndex = (nThreadSlotKey & TLS_MASK_NUM);

#ifdef OSAL_DEBUG
    if( CALLED_FROM_AN_ISR )
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if( (!IsValidTLSKey(nThreadSlotKey)) || (nSlotIndex >= _adi_osal_gnNumSlots) )
    {
        /* The slot number does not contain the TLS signature */
        return ADI_OSAL_BAD_SLOT_KEY;
    }
#endif /* OSAL_DEBUG */

	nSlotBit = (uint32_t)1u << nSlotIndex;

	/* Make sure the thread slot has been allocated first */
	if( 0u != (_adi_osal_gTLSUsedSlots & nSlotBit) )
	{
		/* We have a valid task in place, so set the value. */
		saTLSSlotTable[nSlotIndex] = slotValue;
		eRetStatus = ADI_OSAL_SUCCESS;
	}

    return(eRetStatus);
}

/*!
  ****************************************************************************

    @brief Gets a value for the specified TLS slot from the current thread.

    @param[in] nThreadSlotKey     - Slot key, from which the data needs 
                                    to be retrieved.
    @param[out] pSlotValue        - Pointer to store the retrieved value from 
                                    TLS.

    @return ADI_OSAL_SUCCESS      - If the function successfully retrieved 
                                    data from the specified slot.
    @return ADI_OSAL_FAILED       - If there was an error trying to retrieve 
                                    data a slot which is not acquired, or the
                                    supplied slot value pointer is NULL.
    @return ADI_OSAL_BAD_SLOT_KEY - If the specified slot key is invalid.
    @return ADI_OSAL_CALLER_ERROR - If the function is invoked from an invalid 
                                    location (i.e an ISR)
    @note
         "pnSlotValue"  will be set to ADI_OSAL_INVALID_THREAD_SLOT if
          failed to retrieve the data from the specified slot.

*****************************************************************************/

ADI_OSAL_STATUS  
adi_osal_ThreadSlotGetValue(ADI_OSAL_TLS_SLOT_KEY nThreadSlotKey, 
                            ADI_OSAL_SLOT_VALUE   *pSlotValue)
{
    uint32_t nSlotBit;
    uint32_t nSlotIndex;

    ADI_OSAL_STATUS eRetStatus = ADI_OSAL_FAILED;

    /* Remove the TLS signature from the slot number */
    nSlotIndex = (nThreadSlotKey & TLS_MASK_NUM);

#ifdef OSAL_DEBUG
    if( CALLED_FROM_AN_ISR )
    {
        return (ADI_OSAL_CALLER_ERROR);
    }

    if( (!IsValidTLSKey(nThreadSlotKey)) || (nSlotIndex >= _adi_osal_gnNumSlots) )
    {
        /* The slot key does not contain the TLS signature */
        return ADI_OSAL_BAD_SLOT_KEY;
    }

    if(NULL == pSlotValue)
    {
        /* A null pointer has been passed */
        return (ADI_OSAL_CALLER_ERROR);
    }
#endif /* OSAL_DEBUG */

    nSlotBit = (uint32_t)1u << nSlotIndex;

    /* Make sure that the slot has been previously allocated */
    if( 0u != (_adi_osal_gTLSUsedSlots & nSlotBit) )
    {
        /* We have a valid task in place, so set the value. */
   		*pSlotValue = saTLSSlotTable[nSlotIndex];
        eRetStatus = ADI_OSAL_SUCCESS;
    }

    return(eRetStatus);
}


/*
**
** EOF: 
**
*/
