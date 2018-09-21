/*********************************************************************************

Copyright (c) 2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement.

*********************************************************************************/

/*!
 *****************************************************************************
 * @file:    Uart_CharEcho.c
 * @brief:   Uart_CharEcho Device Example for ADuCxxx
 * @version: $Revision: 29228 $
 * @date:    $Date: 2014-12-15 09:59:54 -0500 (Mon, 15 Dec 2014) $
 *****************************************************************************/

/** \addtogroup Uart_Example UART Example
 *  Example code demonstrating use of the UART device driver.
 *  @{
 */

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"suppress all MISRA rules for test")
#endif /* _MISRA_RULES */


#include "test_common.h"
#include <stdio.h>
#include <uart.h>

#include <ADuCM350.h>

#if defined ( __ICCARM__ )  // IAR compiler...
/* Apply ADI MISRA Suppressions */
#define ASSERT_ADI_MISRA_SUPPRESSIONS
#include "misra.h"
#endif

/* Macro to enable the returning of AFE data using the UART */
/*      1 = return AFE data on UART                         */
/*      0 = return AFE data on SW (Std Output)              */
#define USE_UART_FOR_DATA           (1)

/* Size of Tx and Rx buffers */
#define BUFFER_SIZE     2

/* UART Handle */
ADI_UART_HANDLE      hDevice;

/* Rx and Tx buffers */
static uint8_t RxBuffer[BUFFER_SIZE];
static uint8_t TxBuffer[BUFFER_SIZE];

void test_print(char *pBuffer);
extern int32_t adi_initpinmux(void);

/* Helper function for printing a string to UART or Std. Output */
void test_print(char *pBuffer)
{
#if (1 == USE_UART_FOR_DATA)
    int16_t size;
    /* Print to UART */
    size = strlen(pBuffer);
    adi_UART_BufTx(hDevice, pBuffer, &size);

#elif (0 == USE_UART_FOR_DATA)
    /* Print  to console */
    printf(pBuffer);

#endif /* USE_UART_FOR_DATA */
}

int UartEcho(void)
{
    /* UART return code */
    ADI_UART_RESULT_TYPE uartResult;
    ADI_UART_INIT_DATA   initData;
    ADI_UART_GENERIC_SETTINGS_TYPE  Settings;
    int16_t  rxSize;
    int16_t  txSize;
    
    /* Flag which indicates whether to stop the program */
    _Bool bStopFlag = false;


    /* NVIC initialization */
    NVIC_SetPriorityGrouping(12);

    /* Use static pinmuxing */
    adi_initpinmux();

    /*
     * Initialize UART
     */

    initData.pRxBufferData = RxBuffer;
    initData.RxBufferSize = BUFFER_SIZE;
    initData.pTxBufferData = TxBuffer;
    initData.TxBufferSize = BUFFER_SIZE;

    /* Open UART driver */
    uartResult = adi_UART_Init(ADI_UART_DEVID_0, &hDevice, &initData);
    if (ADI_UART_SUCCESS != uartResult)
    {
        test_Fail("adi_UART_Init() failed");
    }

    Settings.BaudRate = ADI_UART_BAUD_9600;
    Settings.bBlockingMode = true;
    Settings.bInterruptMode = true;
    Settings.Parity = ADI_UART_PARITY_NONE;
    Settings.WordLength = ADI_UART_WLS_8;
    Settings.bDmaMode = false;
          

    /* config UART */
    uartResult =  adi_UART_SetGenericSettings(hDevice, &Settings);
    if (ADI_UART_SUCCESS != uartResult)
    {
        test_Fail("adi_UART_SetGenericSettings() failed");
    }

    /* enable UART */
    uartResult = adi_UART_Enable(hDevice, true);
    if (ADI_UART_SUCCESS != uartResult)
    {
        test_Fail("adi_UART_Enable(true) failed");
    }

    /* UART processing loop */
    while(bStopFlag == false)
    {
        rxSize = 1;
        txSize = 1;

        /* Read a character */
        uartResult = adi_UART_BufRx(hDevice, RxBuffer, &rxSize);
        if (ADI_UART_SUCCESS != uartResult)
        {
            test_Fail("adi_UART_BufRx() failed");
        }

        /* copy receive character to transmit */
        TxBuffer[0] = RxBuffer[0];
        
        /* If return character is pressed, write back \n character along with \r */
        if(RxBuffer[0] == '\r')
        {
            TxBuffer[1] = '\n';

            /* Transmit two characters in this case */
            txSize     = 2u;

            /* Stop the program upon receiving carriage return */
            bStopFlag = true;
        }          

        /* transmit the character */
        uartResult = adi_UART_BufTx(hDevice, TxBuffer, &txSize);
        if (ADI_UART_SUCCESS != uartResult)
        {
             test_Fail("adi_UART_BufTx() failed");
        }
     }

    /* Close the UART */
    uartResult = adi_UART_UnInit(hDevice);
    if (ADI_UART_SUCCESS != uartResult)
    {
        test_Fail("adi_UART_UnInit");
    }

    /* If we've got here then the test has passed */
    test_Pass();
}

#if defined ( __ICCARM__ )  // IAR compiler...
/* Revert ADI MISRA Suppressions */
#define REVERT_ADI_MISRA_SUPPRESSIONS
#include "misra.h"
#endif
