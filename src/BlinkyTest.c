/*********************************************************************************

Copyright (c) 2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using
this software you agree to the terms of the associated Analog Devices Software
License Agreement (See /LICENSES/ADI.LABLAB.LICENSE). 

Modified from the Analog Devices original.
Portions Copyright (c) 2018 Andrew Grosser, Moritz von Buttlar.

*********************************************************************************/

/*****************************************************************************
 * @file:   Blinky.c
 * @brief:  LED blinking example.
 *          Blinks the DISPLAY and DISPLAY1 LEDs on the Eval-ADUCM350EBZ board
 *****************************************************************************/

#include <stdio.h>
#include "test_common.h"
#include "gpio.h"

typedef struct {
    ADI_GPIO_PORT_TYPE Port;
    ADI_GPIO_DATA_TYPE Pins;
} PinMap;

PinMap led_DISPLAY  = {ADI_GPIO_PORT_0, ADI_GPIO_PIN_11};
PinMap led_DISPLAY1 = {ADI_GPIO_PORT_4, ADI_GPIO_PIN_2};

int BlinkyTest(void) {
    volatile uint32_t   i;
	uint32_t            count = 0;

    /* Initialize system */
    SystemInit();

    /* Test initialization */
    test_Init();

    /* Initialize GPIO */
    if (ADI_GPIO_SUCCESS != adi_GPIO_Init())
    {
        FAIL("adi_GPIO_Init");
    }

    /* Enable GPIO output drivers */
    if (ADI_GPIO_SUCCESS != adi_GPIO_SetOutputEnable(led_DISPLAY.Port, led_DISPLAY.Pins, true))
    {
        FAIL("adi_GPIO_SetOutputEnable (led_DISPLAY)");
    }
    if (ADI_GPIO_SUCCESS != adi_GPIO_SetOutputEnable(led_DISPLAY1.Port, led_DISPLAY1.Pins, true))
    {
        FAIL("adi_GPIO_SetOutputEnable (led_DISPLAY1)");
    }

	/* Loop indefinitely */
    while (1)
    {
		/* Delay */
		for (i = 0; i < 50000; i++)
            ;

		/* Flash the count using the LEDs */
		switch (count++)
        {
			case 3:
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetLow(led_DISPLAY.Port,  led_DISPLAY.Pins))
                {
                    FAIL("adi_GPIO_SetLow (led_DISPLAY)");
                }
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetLow(led_DISPLAY1.Port, led_DISPLAY1.Pins))
                {
                    FAIL("adi_GPIO_SetLow (led_DISPLAY1)");
                }
				break;

			case 2:
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetHigh(led_DISPLAY.Port,  led_DISPLAY.Pins))
                {
                    FAIL("adi_GPIO_SetHigh (led_DISPLAY)");
                }
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetLow(led_DISPLAY1.Port, led_DISPLAY1.Pins))
                {
                    FAIL("adi_GPIO_SetLow (led_DISPLAY1)");
                }
				break;

			case 1:
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetLow(led_DISPLAY.Port,  led_DISPLAY.Pins))
                {
                    FAIL("adi_GPIO_SetLow (led_DISPLAY)");
                }
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetHigh(led_DISPLAY1.Port, led_DISPLAY1.Pins))
                {
                    FAIL("adi_GPIO_SetHigh (led_DISPLAY1)");
                }
				break;

			case 0:
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetHigh(led_DISPLAY.Port,  led_DISPLAY.Pins))
                {
                    FAIL("adi_GPIO_SetHigh (led_DISPLAY)");
                }
				if (ADI_GPIO_SUCCESS != adi_GPIO_SetHigh (led_DISPLAY1.Port, led_DISPLAY1.Pins))
                {
                    FAIL("adi_GPIO_SetHigh (led_DISPLAY1)");
                }
				break;

		}

		/* Apply module */
		count %= 4;
	}
}
