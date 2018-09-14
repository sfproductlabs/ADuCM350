/*********************************************************************************

Copyright (c) 2008-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement.

*********************************************************************************/

/* $Revision: 28530 $
 * $Date: 2014-11-12 16:10:52 -0500 (Wed, 12 Nov 2014) $
******************************************************************************

Title:      OSAL interrupt table definition on Cortex-M3

Description:
           This is the definition of the OSAL interrupt table in which pointers
           to handlers written in C for the given interrupt are stored.

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include "adi_osal.h"
#include <adi_osal_arch.h>

/*=============  D E F I N E S  =============*/

/*!
    @internal
    @var _adi_osal_gHandlerTable
	This is the OSAL dispatch table. It is an array of function pointers and 
    callback arguments, of the type corresponding to OSAL's plain C interrupt 
    handlers (i.e. the high-level handlers). The table needs to be large enough 
    for any index that we can get back from adi_nvic_RegisterHandler(). 
    @endinternal
 */

#define OSAL_HANDLER_TABLE_SIZE 255u

uint32_t _adi_osal_gHandlerTableSize = OSAL_HANDLER_TABLE_SIZE;

/*!
    @internal
    @var _adi_osal_gHandlerTableSize
    The size of the OSAL dispatch table. The size needs to be large enough for 
    any index that we can get back from adi_nvic_RegisterHandler(). 
    @endinternal
 */

ADI_OSAL_HANDLER_ENTRY _adi_osal_gHandlerTable[OSAL_HANDLER_TABLE_SIZE];


/*
**
** EOF: 
**
*/

