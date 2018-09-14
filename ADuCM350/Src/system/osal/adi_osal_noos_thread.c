/*********************************************************************************

Copyright (c) 2008-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement.

*********************************************************************************/

 /* $Date: 2014-11-12 16:08:36 -0500 (Wed, 12 Nov 2014) $
*****************************************************************************/

/*!
    @file adi_osal_noos_thread.c

    Operating System Abstraction Layer - OSAL for no OS- Thread
    functions

    This file contains the Thread APIs for the no OS implementation of
    OSAL

*/

/*=============  I N C L U D E S   =============*/

#include "adi_osal.h"
#include "osal_noos.h"
#include "osal_common.h"
#include <string.h>


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
#pragma file_attr(  "prefersMem=any")
#pragma file_attr(  "prefersMemNum=50")
#endif


/*=============  D A T A  =============*/




/*=============  C O D E  =============*/


/*!
  ****************************************************************************
    @brief Returns the name of the currently executing thread


    @param[out] pszTaskName     - Pointer to the location to return the thread
                                  name
    @param[in]  nNumBytesToCopy - Number of bytes from the name to copy into
                                  the memory supplied as output argument

    @return ADI_OSAL_FAILED     - Unable to copy the task name
    @return ADI_OSAL_SUCCESS    - Successfully copied
    @return ADI_OSAL_BAD_HANDLE - The specified string pointer is invalid.

  *****************************************************************************/

ADI_OSAL_STATUS adi_osal_ThreadGetName(char_t *pszTaskName,
                                       uint32_t nNumBytesToCopy)
{

#ifdef OSAL_DEBUG
    if (NULL == pszTaskName)
    {
        return(ADI_OSAL_BAD_HANDLE);
    }
#endif

    /* for a no-OS port we need to return an empty string */
    *pszTaskName = '\0';
    return (ADI_OSAL_SUCCESS);
}
