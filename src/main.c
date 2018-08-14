/*
 * Echem Readout test software
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Timer.h"

#include "test_common.h"
#include <stdio.h>
#include <uart.h>

#include "afe.h"
#include "afe_lib.h"
#include "uart.h"

#include <DEVICE.h>
#include "system_ADuCM350.h"



// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Wmissing-declarations"
// #pragma GCC diagnostic ignored "-Wreturn-type"

#define BUFFER_SIZE     20
ADI_UART_HANDLE hUartDevice;
static uint8_t RxBuffer[BUFFER_SIZE];
static uint8_t TxBuffer[BUFFER_SIZE];

// #define USE_UART_FOR_DATA 1

/****************************************************************************/
/*  <----------- DURL1 -----------><----------- DURL2 ----------->          */
/*                  <-- DURIVS1 --><-- DURIVS2 -->                          */
/*                                 _______________________________ <--- VL2 */
/*                                |                                         */
/*  ______________________________|                                <--- VL1 */
/*                                                                          */
/*  <---- dur1 ----><--- dur2 ---><---- dur3 ----><---- dur4 ---->          */
/****************************************************************************/

/* DC Level 1 voltage in mV (range: -0.8V to 0.8V) */
#define VL1                         (032)
/* DC Level 2 voltage in mV (range: -0.8V to 0.8V) */
#define VL2                         (232)
/* The duration (in us) of DC Level 1 voltage */
#define DURL1                       ((uint32_t)(1000000))
/* The duration (in us) of DC Level 2 voltage */
#define DURL2                       ((uint32_t)(1000000))
/* The duration (in us) which the IVS switch should remain closed (to shunt */
/* switching current) before changing the DC level.                         */
#define DURIVS1                     ((uint32_t)(100))
/* The duration (in us) which the IVS switch should remain closed (to shunt */
/* switching current) after changing the DC level.                          */
#define DURIVS2                     ((uint32_t)(100))
/* Is shunting of the switching current required? Required: 1, Not required: 0 */
#define SHUNTREQD                   (1)

/* RCAL value, in ohms                                              */
/* Default value on ADuCM350 Switch Mux Config Board Rev.0 is 1k    */
#define RCAL                        (100000)
/* RTIA value, in ohms                                              */
/* Default value on ADuCM350 Switch Mux Config Board Rev.0 is 7.5k  */
#define RTIA                        (7500)

/* DO NOT EDIT: DAC LSB size in mV, before attenuator (1.6V / (2^12 - 1)) */
#define DAC_LSB_SIZE                (0.39072)
/* DO NOT EDIT: DC Level 1 in DAC codes */
#define DACL1                       ((uint32_t)(((float)VL1 / (float)DAC_LSB_SIZE) + 0x800))
/* DO NOT EDIT: DC Level 2 in DAC codes */
#define DACL2                       ((uint32_t)(((float)VL2 / (float)DAC_LSB_SIZE) + 0x800))

/* DO NOT EDIT: Number of samples to be transferred by DMA, based on the duration of */
/* the sequence.                                                                     */
/* SAMPLE_COUNT = (Level 1 Duration + Level 2 Duration)us * (160k/178)samples/s      */
#define SAMPLE_COUNT                (uint32_t)((2 * (DURL1 + DURL2)) / 2225)

/* Size limit for each DMA transfer (max 1024) */
#define DMA_BUFFER_SIZE             (300u)

/* DO NOT EDIT: Maximum printed message length. Used for printing only. */
#define MSG_MAXLEN                  (50)



#define UART0_TX_PORTP0_MUX  ((uint16_t) ((uint16_t) 2<<12))
#define UART0_RX_PORTP0_MUX  ((uint16_t) ((uint16_t) 2<<14))



// #pragma location="volatile_ram"
uint16_t        dmaBuffer[DMA_BUFFER_SIZE * 2];

/* Sequence for Amperometric measurement */
uint32_t seq_afe_ampmeas[] = {
    0x00150065,   /*  0 - Safety Word, Command Count = 15, CRC = 0x1C                                       */
    0x84007818,   /*  1 - AFE_FIFO_CFG: DATA_FIFO_SOURCE_SEL = 0b11 (LPF)                                   */
    0x8A000030,   /*  2 - AFE_WG_CFG: TYPE_SEL = 0b00                                                       */
    0x88000F00,   /*  3 - AFE_DAC_CFG: DAC_ATTEN_EN = 0 (disable DAC attenuator)                            */
    0xAA000800,   /*  4 - AFE_WG_DAC_CODE: DAC_CODE = 0x800 (DAC Level 1 placeholder, user programmable)    */
    0xA0000002,   /*  5 - AFE_ADC_CFG: MUX_SEL = 0b00010, GAIN_OFFS_SEL = 0b00 (TIA)                        */
    0xA2000000,   /*  6 - AFE_SUPPLY_LPF_CFG: BYPASS_SUPPLY_LPF = 0 (do not bypass)                         */
    0x86005533,   /*  7 - DMUX_STATE = 5, PMUX_STATE = 5, NMUX_STATE = 6, TMUX_STATE = 6                    */
    0x0001A900,   /*  8 - Wait: 6.8ms (based on load RC = 6.8kOhm * 1uF)                                    */
    0x80024EF0,   /*  9 - AFE_CFG: WG_EN = 1                                                                */
    0x00000C80,   /* 10 - Wait: 200us                                                                       */
    0x80034FF0,   /* 11 - AFE_CFG: ADC_CONV_EN = 1, SUPPLY_LPF_EN = 1                                       */
    0x00090880,   /* 12 - Wait: 37ms  for LPF settling                                                      */
    0x00000000,   /* 13 - Wait: (DAC Level 1 duration - IVS duration 1) (placeholder, user programmable)    */
    0x86015533,   /* 14 - IVS_STATE = 1 (close IVS switch, user programmable)                               */
    0x00000000,   /* 15 - Wait: IVS duration 1 (placeholder, user programmable)                             */
    0xAA000800,   /* 16 - AFE_WG_DAC_CODE: DAC_CODE = 0x800 (DAC Level 2 placeholder, user programmable)    */
    0x00000000,   /* 17 - Wait: IVS duration 2 (placeholder, user programmable)                             */
    0x86005533,   /* 18 - IVS_STATE = 0 (open IVS switch)                                                   */
    0x00000000,   /* 19 - Wait: (DAC Level 2 duration - IVS duration 2) (placeholder, user programmable)    */
    0x80020EF0,   /* 20 - AFE_CFG: WAVEGEN_EN = 0, ADC_CONV_EN = 0, SUPPLY_LPF_EN = 0                       */
    0x82000002,   /* 21 - AFE_SEQ_CFG: SEQ_EN = 0                                                           */
};

/* Function prototypes */
void                    test_print                  (char *pBuffer);
ADI_UART_RESULT_TYPE    uart_Init                   (void);
ADI_UART_RESULT_TYPE    uart_UnInit                 (void);
void        			RxDmaCB         (void *hAfeDevice, uint32_t length, void *pBuffer);
int32_t adi_initpinmux(void);

typedef struct {
    ADI_GPIO_PORT_TYPE Port;
    ADI_GPIO_DATA_TYPE Pins;
} PinMap;

PinMap gain_sw  = {ADI_GPIO_PORT_2, ADI_GPIO_PIN_8 | ADI_GPIO_PIN_9};
ADI_GPIO_PORT_TYPE gainPort = ADI_GPIO_PORT_2;
ADI_GPIO_DATA_TYPE gain200 = ADI_GPIO_PIN_9;
ADI_GPIO_DATA_TYPE gain8k = ADI_GPIO_PIN_8;

int switchGain() {
// P2.8
// P2.9
   /* Enable GPIO output drivers */
 if (ADI_GPIO_SUCCESS != adi_GPIO_SetOutputEnable(gain_sw.Port, gain_sw.Pins, true))
    {
        FAIL("adi_GPIO_SetOutputEnable (led_DISPLAY)");
    }

 adi_GPIO_SetLow(gainPort,  gain200);
 adi_GPIO_SetHigh(gainPort,  gain8k);
 return 0;
}

int32_t adi_initpinmux(void) {
    /* Port Control MUX registers */
    *((volatile uint32_t *)REG_GPIO0_GPCON) = UART0_TX_PORTP0_MUX | UART0_RX_PORTP0_MUX;


    return 0;
}

int main4(void) {
    ADI_AFE_DEV_HANDLE  hAfeDevice;
    uint32_t            dur1;
    uint32_t            dur2;
    uint32_t            dur3;
    uint32_t            dur4;

    /* Initialize system */
    SystemInit();
    // set UART pins
    adi_initpinmux();

    /* Change the system clock source to HFXTAL and change clock frequency to 16MHz     */
    /* Requirement for AFE (ACLK)                                                       */

    SystemTransitionClocks(ADI_SYS_CLOCK_TRIGGER_MEASUREMENT_ON);

    /* SPLL with 32MHz used, need to divide by 2 */
    SetSystemClockDivider(ADI_SYS_CLOCK_UART, 2);
    if (ADI_UART_SUCCESS != uart_Init())
    {
        FAIL("uart_Init");
    }


    // write test data via UART

    while(1) {

    	test_print("Test 123 \n");

    }


   /* Initialize GPIO (interrupts etc) */
    if (ADI_GPIO_SUCCESS != adi_GPIO_Init())
    {
        FAIL("adi_GPIO_Init");
    }
    // switch gain to 8k | 200k
    switchGain();


    /* Initialize the AFE API */
    if (ADI_AFE_SUCCESS != adi_AFE_Init(&hAfeDevice))
    {
        FAIL("Init");
    }

    /* Set RCAL Value */
    if (ADI_AFE_SUCCESS != adi_AFE_SetRcal(hAfeDevice, RCAL))
    {
        FAIL("Set RCAL");
    }

    /* Set RTIA Value */
    if (ADI_AFE_SUCCESS != adi_AFE_SetRtia(hAfeDevice, RTIA))
    {
        FAIL("Set RTIA");
    }

    /* AFE power up */
    if (ADI_AFE_SUCCESS != adi_AFE_PowerUp(hAfeDevice))
    {
        FAIL("PowerUp");
    }

    /* Excitation Channel Power-Up */
    if (ADI_AFE_SUCCESS != adi_AFE_ExciteChanPowerUp(hAfeDevice))
    {
        FAIL("ExciteChanCalAtten");
    }

    // while (1) {
   //   PRINT("Test\n");

  //  }

    /* TIA Channel Calibration */
    if (ADI_AFE_SUCCESS != adi_AFE_TiaChanCal(hAfeDevice))
    {
        FAIL("TiaChanCal");
    }

    /* Excitation Channel (no attenuation) Calibration */
    if (ADI_AFE_SUCCESS != adi_AFE_ExciteChanCalNoAtten(hAfeDevice))
    {
            FAIL("adi_AFE_ExciteChanCalNoAtten");
        }

    /* Amperometric Measurement */
    /* Set the user programmable portions of the sequence */
    /* Set the duration values */
    if (SHUNTREQD)
    {
    dur1 = DURL1 - DURIVS1;
    dur2 = DURIVS1;
    dur3 = DURIVS2;
    dur4 = DURL2 - DURIVS2;
    }
    else
    {
        dur1 = DURL1;
        dur2 = 0;
        dur3 = 0;
        dur4 = DURL2;
    }

    /* Set durations in ACLK periods */
        seq_afe_ampmeas[13] = dur1 * 16;
        seq_afe_ampmeas[15] = dur2 * 16;
        seq_afe_ampmeas[17] = dur3 * 16;
        seq_afe_ampmeas[19] = dur4 * 16;

    /* Set DAC Level 1 */
    seq_afe_ampmeas[4]  = SEQ_MMR_WRITE(REG_AFE_AFE_WG_DAC_CODE, DACL1);
    /* Set DAC Level 2 */
    seq_afe_ampmeas[16] = SEQ_MMR_WRITE(REG_AFE_AFE_WG_DAC_CODE, DACL2);

    if (!SHUNTREQD)
    {
        /* IVS switch remains open */
        seq_afe_ampmeas[14] &= 0xFFFEFFFF;
    }

#if (ADI_AFE_CFG_ENABLE_RX_DMA_DUAL_BUFFER_SUPPORT == 1)
    /* Set the Rx DMA buffer sizes */
    if (ADI_AFE_SUCCESS != adi_AFE_SetDmaRxBufferMaxSize(hAfeDevice, DMA_BUFFER_SIZE, DMA_BUFFER_SIZE))
    {
        FAIL("adi_AFE_SetDmaRxBufferMaxSize");
    }
#endif /* ADI_AFE_CFG_ENABLE_RX_DMA_DUAL_BUFFER_SUPPORT == 1 */

    /* Register Rx DMA Callback */
    if (ADI_AFE_SUCCESS != adi_AFE_RegisterCallbackOnReceiveDMA(hAfeDevice, RxDmaCB, 0))
    {
        FAIL("adi_AFE_RegisterCallbackOnReceiveDMA");
    }

    /* Recalculate CRC in software for the amperometric measurement */
    adi_AFE_EnableSoftwareCRC(hAfeDevice, true);

    /* Perform the Amperometric measurement(s) */
    if (ADI_AFE_SUCCESS != adi_AFE_RunSequence(hAfeDevice, seq_afe_ampmeas, (uint16_t *) dmaBuffer, SAMPLE_COUNT))
    {
        FAIL("adi_AFE_RunSequence");
    }



    /* Restore to using default CRC stored with the sequence */
    adi_AFE_EnableSoftwareCRC(hAfeDevice, false);


}

/*!
 * @brief       AFE Rx DMA Callback Function.
 *
 * @param[in]   hAfeDevice  Device handle obtained from adi_AFE_Init()
 *              length      Number of U16 samples received from the DMA
 *              pBuffer     Pointer to the buffer containing the LPF results
 *
 *
 * @details     16-bit results are converted to bytes and transferred using the UART
 *
 */
void RxDmaCB(void *hAfeDevice, uint32_t length, void *pBuffer)
{
#if (1 == USE_UART_FOR_DATA)
    char                    msg[MSG_MAXLEN];
    uint32_t                i;
    uint16_t                *ppBuffer = (uint16_t*)pBuffer;

    /* Check if there are samples to be sent */
    if (length)
    {
        for (i = 0; i < length; i++)
        {
            sprintf(msg, "%u\r\n", *ppBuffer++);
            PRINT(msg);
    }
    }

#elif (0 == USE_UART_FOR_DATA)
    FAIL("Std. Output is too slow for ADC/LPF data. Use UART instead.");

#endif /* USE_UART_FOR_DATA */
}

/* Helper function for printing a string to UART or Std. Output */
void test_print (char *pBuffer) {
#if (1 == USE_UART_FOR_DATA)
    int16_t size;
    /* Print to UART */
    size = strlen(pBuffer);
    adi_UART_BufTx(hUartDevice, pBuffer, &size);

#elif (0 == USE_UART_FOR_DATA)
    /* Print  to console */
    printf(pBuffer);

#endif /* USE_UART_FOR_DATA */
}

/* Initialize the UART, set the baud rate and enable */
ADI_UART_RESULT_TYPE uart_Init (void) {
    ADI_UART_RESULT_TYPE    result = ADI_UART_SUCCESS;

    /* Open UART in blocking, non-intrrpt mode by supplying no internal buffs */
    if (ADI_UART_SUCCESS != (result = adi_UART_Init(ADI_UART_DEVID_0, &hUartDevice, NULL)))
    {
        return result;
    }

    /* Set UART baud rate to 115200 */
    if (ADI_UART_SUCCESS != (result = adi_UART_SetBaudRate(hUartDevice, ADI_UART_BAUD_115200)))
    {
        return result;
    }

    /* Enable UART q */
    if (ADI_UART_SUCCESS != (result = adi_UART_Enable(hUartDevice,true)))
    {
        return result;
    }

    return result;
}

/* Uninitialize the UART */
ADI_UART_RESULT_TYPE uart_UnInit (void) {
    ADI_UART_RESULT_TYPE    result = ADI_UART_SUCCESS;

  /* Uninitialize the UART API */
    if (ADI_UART_SUCCESS != (result = adi_UART_UnInit(hUartDevice)))
    {
        return result;
    }

    return result;

}



int main(void)
{
    /* UART return code */
    ADI_UART_RESULT_TYPE uartResult;
    ADI_UART_INIT_DATA   initData;
    ADI_UART_GENERIC_SETTINGS_TYPE  Settings;
    int16_t  rxSize;
    int16_t  txSize;

    /* Flag which indicates whether to stop the program */
    _Bool bStopFlag = false;

    /* Clock initialization */
    SystemInit();
  //  pADI_SYSCLK->CLKCON1=0x0101; // from internet forum (2-wire JTAG Programming....)

    /* NVIC initialization */
    NVIC_SetPriorityGrouping(12);

    /* Change HCLK clock divider to 1 for a 16MHz clock */
    if (ADI_SYS_SUCCESS != SetSystemClockDivider(ADI_SYS_CLOCK_CORE, 1))
    {
       // test_Fail("SetSystemClockDivider() failed");
    }
    /* Change PCLK clock divider to 1 for a 16MHz clock */
    if (ADI_SYS_SUCCESS != SetSystemClockDivider (ADI_SYS_CLOCK_UART, 1))
    {
       // test_Fail("SetSystemClockDivider() failed");
    }

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
    uartResult = adi_UART_Init(ADI_UART_DEVID_0, &hUartDevice, &initData);
    if (ADI_UART_SUCCESS != uartResult)
    {
       // test_Fail("adi_UART_Init() failed");
    }

    Settings.BaudRate = ADI_UART_BAUD_115200;
    Settings.bBlockingMode = true;
    Settings.bInterruptMode = true;
    Settings.Parity = ADI_UART_PARITY_NONE;
    Settings.WordLength = ADI_UART_WLS_8;
    Settings.bDmaMode = false;

    /* config UART */
    uartResult =  adi_UART_SetGenericSettings(hUartDevice, &Settings);
    if (ADI_UART_SUCCESS != uartResult)
    {
       // test_Fail("adi_UART_SetGenericSettings() failed");
    }

    /* enable UART */
    uartResult = adi_UART_Enable(hUartDevice, true);
    if (ADI_UART_SUCCESS != uartResult)
    {
       // test_Fail("adi_UART_Enable(true) failed");
    }

    // reduce baud rate to lower limit
    uint32_t currentCOMFBR;

    /* UART processing loop */
    while(bStopFlag == false)
    {
        rxSize = 1;
        txSize = 1;
        // add something to DIVN; thereby increase the baudrate
       //hUartDevice->pUartRegs->COMFBR=hUartDevice->pUartRegs->COMFBR+10;
       // currentCOMFBR=currentCOMFBR+10;
       // pADI->COMFBR=currentCOMFBR;
        /* Read a character */
        uartResult = adi_UART_BufRx(hUartDevice, RxBuffer, &rxSize);
        if (ADI_UART_SUCCESS != uartResult)
        {
  //          test_Fail("adi_UART_BufRx() failed");
        }//

        // TxBuffer[0] = RxBuffer[0];


        /* copy receive character to transmit */
        if (RxBuffer[0]!='E')
        {
        	TxBuffer[0] = RxBuffer[0];

        /* transmit the character */
        uartResult = adi_UART_BufTx(hUartDevice, TxBuffer, &txSize);
        if (ADI_UART_SUCCESS != uartResult)
        {
     //        test_Fail("adi_UART_BufTx() failed");
        }
        }

     }

 // return (1);
}
