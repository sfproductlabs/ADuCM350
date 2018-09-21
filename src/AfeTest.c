/*********************************************************************************

Copyright (c) 2012-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement (See /LICENSES/ADI.LABLAB.LICENSE). 

Modified from the Analog Devices original.
Portions Copyright (c) 2018 Andrew Grosser, Moritz von Buttlar.

*********************************************************************************/

/*****************************************************************************
 * @file:    AfeTest.c
 * @brief:   AFE Test for ADuCM350
 * @version: $Revision: 28525 $
 * @date:    $Date: 2014-11-12 14:51:26 -0500 (Wed, 12 Nov 2014) $
 *****************************************************************************/

/*! \addtogroup AFE_Test AFE Test
 *  .
 *  @{
 */

/* Apply ADI MISRA Suppressions */
#define ASSERT_ADI_MISRA_SUPPRESSIONS
#include "misra.h"

#include <stddef.h>		/* for 'NULL' */

#include "test_common.h"

#include "afe.h"
#include "afe_lib.h"
#include "dma.h"


/*!
 * @brief       Function 'main' for AFE example program
 *
 * @param       none
 * @return      int (Zero for success, non-zero for failure).
 *
 * @details     Simple application that calls the example functions in sequence.
 *
 */

int AfeTest(void)
{
    ADI_AFE_DEV_HANDLE hDevice;
    int32_t measResult;
    // Used for AC measurement
    int16_t dftResults[4];
    // Used for DC measurement
    // There are more than 100 samples produced, only 100 read in this example
    uint16_t dcMeasBuffer[100];
    uint16_t dcMeasBufferLength = 100;

    // Initialize system
    SystemInit();


    /* Change the system clock source to HFXTAL and change clock frequency to 16MHz     */
    /* Requirement for AFE (ACLK)                                                       */
    SystemTransitionClocks(ADI_SYS_CLOCK_TRIGGER_MEASUREMENT_ON);

    // Test initialization
    test_Init();

    // The following functions correspond to the use cases
    // outlined in chapter "AFE Example Use Cases" from HRM

    // Initialize the AFE API
    if (adi_AFE_Init(&hDevice)) {
        FAIL("Init");
    }

    // Step 1: AFE power up
    if (adi_AFE_PowerUp(hDevice)) {
        FAIL("PowerUp");
    }

    // Step 2: Auxiliary Channel Calibration
    if (adi_AFE_AuxChanCal(hDevice)) {
        FAIL("AuxChanCal");
    }

    // Step 3: Auxiliary Channel Measurement
    if (adi_AFE_AuxChanMeasFixed(hDevice, &measResult)) {
        FAIL("AuxChanMeas");
    }

    // Step 4: Internal Temperature Sensor Channel Calibration
    if (adi_AFE_TempSensChanCal(hDevice)) {
        FAIL("TempSensChanCal");
    }

    // Step 5: Internal Temperature Sensor Measurement
    if (adi_AFE_TempSensMeas(hDevice, &measResult)) {
        FAIL("TempSensMeas");
    }

    // Step 6: Excitation Channel Power-Up
    if (adi_AFE_ExciteChanPowerUp(hDevice)) {
        FAIL("ExciteChanCalAtten");
    }

    // Step 7: TIA Channel Calibration
    if (adi_AFE_TiaChanCal(hDevice)) {
        FAIL("TiaChanCal");
    }

    // Step 8: Excitation Channel Calibration (Attenuation Enabled)
    if (adi_AFE_ExciteChanCalAtten(hDevice)) {
        FAIL("ExciteChanCalAtten");
    }

    // Step 9: Excitation Channel Calibration (No Attenuation)
    if (adi_AFE_ExciteChanCalNoAtten(hDevice)) {
        FAIL("ExciteChanCalNoAtten");
    }

    // Step 10: AC Measurement Example
     if (adi_AFE_AcMeas10khz(hDevice, dftResults)) {
        FAIL("AcMeas10khz");
     }

     // Step 11: Attenuation Change - TO BE ADDED

     // Step 12: DC Measurement Example
     if (adi_AFE_DcMeas(hDevice, &dcMeasBuffer[0], dcMeasBufferLength)) {
         FAIL("DcMeas");
     }

     // Step 13: AFE Power Down
     if (adi_AFE_PowerDown(hDevice)) {
        FAIL("PowerDown");
    }

    PASS();

}


/* Revert ADI MISRA Suppressions */
#define REVERT_ADI_MISRA_SUPPRESSIONS
#include "misra.h"

/*
** EOF
*/

/*@}*/
