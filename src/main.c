/*********************************************************************************

Copyright (c) 2013-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using
this software you agree to the terms of the associated Analog Devices Software
License Agreement (See /LICENSES/ADI.LABLAB.LICENSE). 

Modified from the Analog Devices original.
Portions Copyright (c) 2018 Andrew Grosser, Moritz von Buttlar.

*********************************************************************************/



#include <time.h>
#include <stddef.h>  // for 'NULL'
#include <stdio.h>   // for scanf
#include <string.h>  // for strncmp
#include <stdint.h>

#include "test_common.h"


/* Helper macro for printing strings to UART or Std. Output */
#define PRINT(s)                    test_print(s)

#if defined (DEBUG) && defined (__GNUC__) && !defined (NDEBUG)
	extern void initialise_monitor_handles(void);
#endif

int main(void) {
#if defined (DEBUG) && defined (__GNUC__) && !defined (NDEBUG)
	initialise_monitor_handles();
#endif

    /* Initialize system */
    SystemInit();

    // Change HCLK clock divider to 1 for a 16MHz clock
    if (ADI_SYS_SUCCESS != SetSystemClockDivider(ADI_SYS_CLOCK_CORE, 1))
    {
            test_Fail("SetSystemClockDivider() failed");
    }

    // Change PCLK clock divider to 1 for a 16MHz clock
    if (ADI_SYS_SUCCESS != SetSystemClockDivider (ADI_SYS_CLOCK_UART, 1))
    {
            test_Fail("SetSystemClockDivider() failed");
    }

    /* test system initialization */
    test_Init();


    /* Change the system clock source to HFXTAL and change clock frequency to 16MHz     */
    /* Requirement for AFE (ACLK)                                                       */

    SystemTransitionClocks(ADI_SYS_CLOCK_TRIGGER_MEASUREMENT_ON);

    /* SPLL with 32MHz used , need to divide by 2 */
    SetSystemClockDivider(ADI_SYS_CLOCK_UART, 2);

    

    BlinkyTest();
    //FlashTest();
    //AfeTest();
    //UartEcho();

}






