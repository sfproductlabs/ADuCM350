/*********************************************************************************

Copyright (c) 2008-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement.

*********************************************************************************/

/* $Revision: 28527 $
 * $Date: 2014-11-12 16:08:36 -0500 (Wed, 12 Nov 2014) $
******************************************************************************

Title:      OSAL for no-OS

Description:
              Operating System Abstraction Layer for the environment where
              there is no OS(Operating System) running.

SUPRESSED MISRA RULES ARE:

            1.1  [R]    :  "All code shall conform to ISO 9899:1990"
            5.1  [R]    :  "Identifier shall not rely on the significance of more than 31 characters"
            5.7  [A]    :  "No identifier name should be reused"
            11.1 [R]    :  "Conversions shall not be performed between a pointer to function and
                           any type other than an integral type"
            11.3 [A]    :  "A cast should not be performed between a pointer type and an integral type"
            11.4 [A]    :  "A cast should not be performed between a pointer to object type and a
                           different pointer to object type"
            14.7 [R]    :  "a function shall have a single point of exit at the end of the function"
            16.2 [R]    :  "Functions shall not call themselves, either directly or indirectly"
            17.5 [A]    :  "The declaration of objects should contain no more than 2 levels of pointer indirection"
			20.8 [R]    :  "The signal handling facilities of <signal.h> shall not be used. "
			20.4 [R]    :  "Dynamic heap memory allocation shall not be used." 


*****************************************************************************/

/*=============  I N C L U D E S   =============*/


#ifdef _MISRA_RULES
/*Rule 14.7 indicates that a function shall have a single exit point */
#pragma diag(suppress:misra_rule_14_7:"Allowing several point of exit (mostly for handling parameter error checking) increases the code readability and therefore maintainability")
/* Rule-5.1 indicates that all identifiers shall not rely on more than 31 characters of significance */
#pragma diag(suppress:misra_rule_5_1:"prefixes added in front of identifiers quickly increases their size. In order to keep the code self explanatory, and since ADI tools are the main targeted tools, this restriction is not really a necessity")
#endif

#include <stdlib.h>                                                             /* for NULL definition */
#include <limits.h>
#include "adi_osal.h"
#include "osal_common.h"
#include "osal_noos.h"

/* "version.h" contains the macro "ADI_OSAL_SVN_REV" which gives the build version
   for OSAL. It is a generated file.
*/
#include "version.h"


/*=============  D E F I N E S  =============*/

#if defined (__ECC__)
#pragma file_attr(  "libGroup=adi_osal.h")
#pragma file_attr(  "libName=libosal")
#pragma file_attr(  "prefersMem=external")
#pragma file_attr(  "prefersMemNum=70")
#endif


/*=============  D A T A  =============*/

/*=============  C O D E  =============*/

/*!
  ****************************************************************************
    @brief Returns code base version information
    
    @details This API function call is used to access the code-base version
             information.  The code-base version differs for each target
             operating system of the OSAL although the major and minor revs are
             the same for all OS variants.

    @param[out] pVersion - Location to store the retrieved version information.

    @return ADI_OSAL_SUCCESS - if able to successfully return the version
    @return ADI_OSAL_FAILED  - if version information could not be obtained.
    @note Version number is mentioned in the format major.minor.patch.build.
          For example,Version "1.0.2.2022" means

          => major  = 1.
          => minor  = 0.
          => patch  = 2.
          => build  = 2022.

          Members of structure ADI_OSAL_VERSION_PTR are also declared in above 
          order.
*****************************************************************************/

ADI_OSAL_STATUS adi_osal_GetVersion(ADI_OSAL_VERSION* pVersion)
{
#ifdef OSAL_DEBUG
    if (NULL == pVersion)
    {
        return (ADI_OSAL_FAILED);
    }
#endif

    pVersion->nMajor = ADI_OSAL_MAJOR_VER;
    pVersion->nMinor = ADI_OSAL_MINOR_VER;
    pVersion->nPatch = ADI_OSAL_PATCH_VER;
    pVersion->nBuild = ADI_BUILD_VER;
    return(ADI_OSAL_SUCCESS);
}


/*!
  *****************************************************************************
  Function: adi_osal_Config
   @brief Initializes OSAL

   @details Configuration function for OSAL. This function will be called from system
            startup.  

   @param[in] pConfig - Pointer to ADI_OSAL_OS_INIT_ATTR structure.

   @return  ADI_OSAL_SUCCESS - Initialization is done successfully.
   @return  ADI_OSAL_FAILED  - OSAL has already been initialized
   @return  ADI_OSAL_BAD_SLOT_KEY - Number of thread local storage slots 
                                    specified greater than the maximum allowed.

   @return  ADI_OSAL_BAD_PRIO_INHERIT - Priority inheritance specified when it 
                                        is not supported

   @return ADI_OSAL_MEM_ALLOC_FAILED - Error initializing dynamic memory heap
   @return ADI_OSAL_INVALID_ARGS     - The arguments do not describe a viable
                                        configuration

*****************************************************************************/

ADI_OSAL_STATUS adi_osal_Config( const ADI_OSAL_CONFIG *pConfig )
{

/* static variable    : snOsalConfigurationState 
 * Flag to record if OSAL has already been configured
 */
    static uint32_t snOsalConfigurationState = 0u ; 

    uint32_t   i;                    /* indexing through interrupts table */
    uint32_t*  pHeapMemory = NULL;   /* ptr to the memory to use for the heap */
    uint32_t   nHeapMemorySize = 0u; /* size of memory pointed by pHeapMemory */

#ifdef OSAL_DEBUG
    /* Check if already initialized. If the parameters are the same then the
     * call succeeds. Otherwise the call fails. The priority inheritance
     * setting is not recorded so we cannot check it
     */
    if (OSAL_INITIALIZED == snOsalConfigurationState) 
    {
        if (NULL == pConfig)
        {
            return (ADI_OSAL_SUCCESS);
        }
        else
        {
            if ( (pConfig->nNumTLSSlots != _adi_osal_gnNumSlots) ||
                (pConfig->nSysTimerPeriodInUsec != _adi_osal_gnTickPeriod) ||
                (pConfig->pHeap != pHeapMemory) || 
                (pConfig->nHeapSizeBytes != nHeapMemorySize)) 
            {
                return (ADI_OSAL_FAILED);
            }
            else
            {
                return (ADI_OSAL_SUCCESS);
            }
        }
    }
#endif
    /* checks that arguments are all valid */
    if (NULL != pConfig)
    {
#ifdef OSAL_DEBUG
        if ( (ADI_OSAL_PRIO_INHERIT_ENABLED != pConfig->eEnablePrioInherit) &&
             (ADI_OSAL_PRIO_INHERIT_DISABLED!= pConfig->eEnablePrioInherit) &&
             (ADI_OSAL_PRIO_INHERIT_AUTO    != pConfig->eEnablePrioInherit))
        {
            /* incorrect value for priority inheritance */
            return(ADI_OSAL_BAD_PRIO_INHERIT);
        }

        if (pConfig->nNumTLSSlots > (uint32_t) ADI_OSAL_MAX_NUM_TLS_SLOTS)
        {
            return (ADI_OSAL_BAD_SLOT_KEY);
        }
#endif

        /* automatically sets the tick period based on the "OS" settings : 
         * considered not supported */
        _adi_osal_gnTickPeriod = pConfig->nSysTimerPeriodInUsec;
        _adi_osal_gnNumSlots = pConfig->nNumTLSSlots;

        pHeapMemory = pConfig->pHeap;
        nHeapMemorySize = pConfig->nHeapSizeBytes;

    } 

    /* Create a heap with the information provided. If pHeapMemory was NULL
       then _adi_osal_HeapInstall will set its heap to the default
     */

    if ((NULL != pHeapMemory) && (0u == nHeapMemorySize))
    {
        return (ADI_OSAL_INVALID_ARGS);
    }

    if (ADI_OSAL_SUCCESS != _adi_osal_HeapInstall(pHeapMemory,nHeapMemorySize))
    {
        return (ADI_OSAL_MEM_ALLOC_FAILED);
    }
        
    snOsalConfigurationState = OSAL_INITIALIZED;

    return( ADI_OSAL_SUCCESS);
}



/*
**
** EOF: 
**
*/

