/*********************************************************************************

Copyright (c) 2013-2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.  By using 
this software you agree to the terms of the associated Analog Devices Software 
License Agreement (See /LICENSES/ADI.LABLAB.LICENSE). 

Modified from the Analog Devices original.
Portions Copyright (c) 2018 Andrew Grosser, Moritz von Buttlar.

*********************************************************************************/

/*!
 *****************************************************************************
 * @file:    FlashTest.c
 * @brief:   Flash Device Example for ADuCM350
 * @version: $Revision: 28544 $
 * @date:    $Date: 2014-11-13 13:09:44 -0500 (Thu, 13 Nov 2014) $
 *****************************************************************************/

/** \addtogroup FlashTest_Example Flash Example
 *  Example code demonstrating use of the flash memory driver.
 *  @{
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//AG
//#ifndef _ILONG
//#error Needs _ILONG
//#endif

#include "wut.h"

#include "test_common.h"
#include <flash.h>

#include <ADuCM350.h>



#if defined ( __ICCARM__ )  // IAR compiler...
/* Apply ADI MISRA Suppressions */
#define ASSERT_ADI_MISRA_SUPPRESSIONS
#include "misra.h"
#endif

/* The symbol USE_TIMERS controls whether timer-driven Flash aborts are
 * generated during the test run. This exercises the ability of the driver to
 * cope with flash writes being aborted. Set to zero to disable the timer aborts.
 */
#define USE_TIMERS 1

#if (1 == USE_TIMERS)
static void InitializeWUT(void);
/* pick some 16-bit comparator values for the
   WUT (even though the WUT is 32-bit), because
   the GPT resolution is only 16-bit */
#define LOADB   0x80
#define LOADC  0x800
#define LOADD 0x8000

#define COUNT 5
static uint32_t tickCount = 0u;
static uint32_t wakeCount = 0u;
#endif /* (1 == USE_TIMERS) */

/* CRC polynomial for signature checking */
#define IEEE_802_3_POLYNOMIAL      0x04C11DB7

static bool_t parityErrorOccurred = false;

/* device handles */
static ADI_WUT_DEV_HANDLE hWUT;
static ADI_FEE_DEV_HANDLE  hAbortFlashDevice = NULL;

static char strbuf[256]; /* buffer for logging messages */
static uint32_t totalCount = 0; /* count of test iterations */

/* skipCount is a debugging feature. It is a module-static variable that
 * causes test passes to be skipped up to the specified number. This
 * allows the test cycle to be quickly advanced to a known failing case.
 */
static uint32_t skipCount = 0;

static void writeFlash(ADI_FEE_DEV_HANDLE  hDevice, bool_t useDMA, uint32_t addr, uint32_t length, uint8_t *pBuffer)
{
    ADI_FEE_RESULT_TYPE feeResult = ADI_FEE_ERR_UNSUPPORTED;
    char *psFnName = "adi_FEE_SubmitTxBuffer";

#if (1 == ADI_FEE_CFG_GPF_DMA_SUPPORT)
    if (useDMA)
    {
        // Try to use DMA, if available
        feeResult = adi_FEE_SubmitTxBuffer(hDevice,
                                addr,
                                pBuffer,
                                length);
    }

    if ((ADI_FEE_ERR_UNSUPPORTED != feeResult) &&
        (ADI_FEE_ERR_INVALID_ADDRESS != feeResult) &&
        (ADI_FEE_ERR_INVALID_PARAMETER != feeResult))
    {
        void *pRetBuff = NULL;

        feeResult = adi_FEE_GetTxBuffer(hDevice, &pRetBuff);

        if (pRetBuff != pBuffer)
        {
            test_Fail("pRetBuff != pBuffer after adi_FEE_GetTxBuffer()");
        }
    }
    else
#endif /* (1 == ADI_FEE_CFG_GPF_DMA_SUPPORT) */
    {
        psFnName = "adi_FEE_Write";

        feeResult = adi_FEE_Write(hDevice,
                                addr,
                                pBuffer,
                                length);
    }

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "%s(addr = 0x%08x, length = %ld) failed with %d\n", psFnName, addr, length, feeResult);
        test_Fail(strbuf);
    }
}

static void verifyFlash(uint32_t baseAddr, uint32_t flashSize, uint32_t relAddr, uint32_t length, uint8_t startVal)
{
    uint8_t *pByte = (uint8_t*)baseAddr;
    uint32_t byteCount;

    for (byteCount = 0u;
         byteCount < relAddr;
         byteCount += 1)
    {
        if (0xFFu != *pByte)
        {
            sprintf(strbuf, "verifyFlash: [0x%08x] != 0xFF", pByte);
            test_Fail(strbuf);
        }

        ++pByte;
    }

    for (;
         byteCount < (relAddr + length);
         byteCount += 1)
    {
        if (startVal != *pByte)
        {
            sprintf(strbuf, "verifyFlash: [0x%08x] != 0x%02x, == 0x%02x", pByte, startVal, *pByte);
            test_Fail(strbuf);
        }

        ++pByte;
        startVal = (startVal + 1) % 255; /* stay within 0-254 range */
    }

    for (;
         byteCount < flashSize;
         byteCount += 1)
    {
        if (0xFFu != *pByte)
        {
            sprintf(strbuf, "verifyFlash: [0x%08x] != 0xFF", pByte);
            test_Fail(strbuf);
        }

        ++pByte;
    }
}

static void erasePages(ADI_FEE_DEV_HANDLE  hDevice, uint32_t addr, uint32_t length)
{
    uint32_t lastAddr = addr + length - 1u;
    uint32_t page, firstPage, lastPage;
    ADI_FEE_RESULT_TYPE feeResult;

     feeResult = adi_FEE_GetPageNumber(hDevice,
                                      addr,
                                      &firstPage);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_GetPageNumber(addr = 0x%08x) failed with %d\n", addr, feeResult);
        test_Fail(strbuf);
    }

     feeResult = adi_FEE_GetPageNumber(hDevice,
                                      lastAddr,
                                      &lastPage);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_GetPageNumber(addr = 0x%08x) failed with %d\n", lastAddr, feeResult);
        test_Fail(strbuf);
    }

    for (page = firstPage;
         page <= lastPage;
         ++page)
    {
        feeResult = adi_FEE_PageErase(hDevice, page);

      if (ADI_FEE_SUCCESS != feeResult)
      {
          sprintf(strbuf, "adi_FEE_PageErase(page = %ld) failed with %d\n", page, feeResult);
          test_Fail(strbuf);
      }
    }
}

static void eraseSignaturePage(ADI_FEE_DEV_HANDLE  hDevice, uint32_t baseAddr, uint32_t flashSize)
{
    uint32_t lastAddr = baseAddr + flashSize - 1u;
    uint32_t page;
    ADI_FEE_RESULT_TYPE feeResult;

    feeResult = adi_FEE_GetPageNumber(hDevice,
                                      lastAddr,
                                      &page);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_GetPageNumber(addr = 0x%08x) failed with %d\n", lastAddr, feeResult);
        test_Fail(strbuf);
    }

    feeResult = adi_FEE_PageErase(hDevice, page);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_PageErase(page = %ld) failed with %d\n", page, feeResult);
        test_Fail(strbuf);
    }
}

static void massErase(ADI_FEE_DEV_HANDLE  hDevice)
{
      ADI_FEE_RESULT_TYPE feeResult;

      feeResult = adi_FEE_MassErase(hDevice);

      if (ADI_FEE_SUCCESS != feeResult)
      {
          test_Fail("mass erase failed");
      }
}

static void checkAndErase(ADI_FEE_DEV_HANDLE  hDevice, uint32_t baseAddr, uint32_t flashSize)
{
    uint32_t *pWord = (uint32_t*)baseAddr;
    uint32_t byteCount;
    ADI_FEE_RESULT_TYPE feeResult;

    for (byteCount = 0u;
         byteCount < flashSize;
         byteCount += 4)
    {
        if (0xFFFFFFFFu != *pWord)
        {
            uint32_t pageNum;

            feeResult = adi_FEE_GetPageNumber(hDevice,
                                        (uint32_t)pWord,
                                        &pageNum);

            if (ADI_FEE_SUCCESS != feeResult)
            {
                sprintf(strbuf, "adi_FEE_GetPageNumber(addr = 0x%08x) failed with %d\n", pWord, feeResult);
                test_Fail(strbuf);
            }

            sprintf(strbuf, "cleaning up page %ld, address 0x%08x", pageNum, pWord);
            test_Perf(strbuf);
            feeResult = adi_FEE_PageErase(hDevice, pageNum);

            if (ADI_FEE_SUCCESS != feeResult)
            {
                sprintf(strbuf, "adi_FEE_PageErase(page = %ld) failed with %d\n", pageNum, feeResult);
                test_Fail(strbuf);
            }
        }

        ++pWord;
    }

}

static void verifyFlashErased(uint32_t baseAddr, uint32_t flashSize)
{
    uint32_t *pWord = (uint32_t*)baseAddr;
    uint32_t byteCount;

    for (byteCount = 0u;
         byteCount < flashSize;
         byteCount += 4)
    {
        if (0xFFFFFFFFu != *pWord)
        {
            sprintf(strbuf, "flash address 0x%08x != 0xFFFFFFFF", pWord);
            test_Fail(strbuf);
        }

        ++pWord;
    }
}

static void checkSignatures(ADI_FEE_DEV_HANDLE  hDevice, uint32_t baseAddr, uint32_t flashSize, ADI_FEE_SIGN_DIR direction, uint32_t *pSignature)
{
    ADI_FEE_RESULT_TYPE feeResult;
    uint32_t startPage, endPage;
    uint32_t signature = 0u;
    uint32_t endAddr = baseAddr + flashSize - 4u;

    feeResult = adi_FEE_GetPageNumber(hDevice,
                            baseAddr,
                            &startPage);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_GetPageNumber(addr = 0x%08x) failed with %d\n", baseAddr, feeResult);
        test_Fail(strbuf);
    }

    feeResult = adi_FEE_GetPageNumber(hDevice,
                            endAddr,
                            &endPage);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_GetPageNumber(addr = 0x%08x) failed with %d\n", endAddr, feeResult);
        test_Fail(strbuf);
    }

    feeResult = adi_FEE_VerifySignature(hDevice,
                                        direction,
                                        startPage,
                                        endPage,
                                        &signature);

    if (ADI_FEE_ERR_READ_VERIFY_ERROR != feeResult)
    {
        sprintf(strbuf, "adi_FEE_Sign(%ld, %ld) != ADI_FEE_ERR_READ_VERIFY_ERROR\n", startPage, endPage);
        test_Fail(strbuf);
    }

    if (ADI_FEE_SIGN_REVERSE == direction)
    {
        endAddr -= 4u;
    }

    feeResult = adi_FEE_Write(hDevice,
                            endAddr,
                            (uint8_t*)&signature,
                            4u);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_Write() of signature word at 0x%08x failed with %d\n", endAddr, feeResult);
        test_Fail(strbuf);
    }

    feeResult = adi_FEE_VerifySignature(hDevice,
                                        direction,
                                        startPage,
                                        endPage,
                                        &signature);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_Sign(%ld, %ld) != ADI_FEE_SUCCESS\n", startPage, endPage);
        test_Fail(strbuf);
    }

    *pSignature = signature;
}


#if 1
#include "crc32.h"     /* include the header file generated with pycrc */

/*
 * Software implementation of the signature algorithm, used
 * for checking the hardware-produced signatures. This version uses a
 * module (crc32.c) which was generated by pycrc (http://www.tty1.net/pycrc/)
 * using the following command lines:
 *
 * python pycrc.py --generate=h --output=crc32.h --model=crc-32 --algorithm=tbl
 *  --xor-out=0 --reflect-in=true --reflect-out=false
 * python pycrc.py --generate=c --output=crc32.c --model=crc-32 --algorithm=tbl
 *  --xor-out=0 --reflect-in=true --reflect-out=false
 *
 * Assumes start and end addresses are word-aligned.
 */
static uint32_t
crcBlock(
    uint32_t addr,
    uint32_t endAddr,
    const int32_t stride,
    const uint32_t polynomial,
    const uint32_t crc)
{
    uint32_t pyCrc = crc_init();

    /* Outer loop - iterates over data words */
    for(;;) /* middle-breaking loop */
    {
        uint32_t *pWord = ((uint32_t  *)addr);
        uint32_t  word = __RBIT(*pWord);

        pyCrc = crc_update(pyCrc, (uint8_t*)&word, 4);

        if (addr == endAddr) break; /* loop breaks here */

        addr += stride;
    }

    return crc_finalize(pyCrc);
}
#else
/*
 * Software implementation of the signature algorithm, used
 * for checking the hardware-produced signatures.
 * Assumes start and end addresses are word-aligned.
 */
static uint32_t
crcBlock(
    uint32_t addr,
    uint32_t endAddr,
    const int32_t stride,
    const uint32_t polynomial,
    uint32_t crc)
{
    /* Outer loop - iterates over data words */
    for(;;) /* middle-breaking loop */
    {
        uint32_t data = *((uint32_t *)addr);
        uint32_t i;

        /* Inner loop - updates CRC from current data word */
        for (i = 0;     i < 32; i++)
        {
            uint32_t xor = data ^ crc;

            crc  <<= 1;
            data <<= 1;

            if (xor & 0x80000000)
            {
                crc ^= polynomial;
            }
        }

        if (addr == endAddr) break; /* loop breaks here */

        addr += stride;
    }

    return crc;
}
#endif

static void testParityChecking(ADI_FEE_DEV_ID_TYPE const devID, uint32_t flashBase, uint32_t flashStart, uint32_t nWords, uint32_t parityBase)
{
    ADI_FEE_RESULT_TYPE feeResult;
    ADI_FEE_DEV_HANDLE  hDevice;
    uint32_t   offset = (flashStart & 0x1Fu);
    uint32_t   addr   = (flashStart >> 3u) | parityBase;
    uint32_t *pFlashWord = (uint32_t*)flashStart;
    uint32_t wordsProcessed = 0u;

    test_Perf("Testing parity checking");

    feeResult = adi_FEE_Init(devID, true, &hDevice);

    if (ADI_FEE_SUCCESS != feeResult)
    {
      test_Fail("adi_FEE_Init() failed\n");
    }

    /* Erase the parity region first, so we know that it will fail checking */
    erasePages(hDevice, addr,  nWords/8);

    feeResult = adi_FEE_SetParityChecking(hDevice, ADI_FEE_PARITY_ENABLE_INTERRUPT);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        test_Fail("adi_FEE_SetParityChecking(ON) failed\n");
    }

    /* If we're executing from this flash memory then a parity error should occur immediately,
     * otherwise we need to force one with a data read. */
    if (!parityErrorOccurred)
    {
        volatile uint32_t dummy = *(uint32_t*)flashBase;
    }

    if (parityErrorOccurred)
    {
        pADI_FEE0->FEEPARSTA = 0x1u; /* clear the parity error indication */
    }
    else
    {
        test_Fail("No parity error detected");
    }

    feeResult = adi_FEE_SetParityChecking(hDevice, ADI_FEE_PARITY_DISABLE);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        test_Fail("adi_FEE_SetParityChecking(OFF) failed\n");
    }

    NVIC_ClearPendingIRQ(PARITY_IRQn); /* Clear any latched parity interrupt */

    /* Now generate the correct parity information and write it to flash.
    */
    while (wordsProcessed < nWords)    /* For each word in specified region */
    {
        uint32_t nParityWord = 0xFFFFFFFFu;       /* aligned word for write */

        /* Prepare the next aligned word to write to flash */
        while((offset < 32u) && (wordsProcessed < nWords))
        {
            uint32_t dataWord = *pFlashWord++;
            uint32_t parityBit = 1u;

            /* For every set bit in dataWord, toggle the parity bit */
            while(0u != dataWord)
            {
                parityBit ^= (1u & dataWord); /* no-op if bottom bit is zero */
                dataWord >>= 1;               /* shift the next bit down */
            }

            /* Now use the parity bit to toggle the appropriate bit in the parity word.
             * The parity word starts out as all-1s (so that unchanged bits have no effect
             * when written) and gets toggled to zero if parityBit is 1. This is why
             * parityBit is initialised to 1, above. If there are an even number of set bits
             * dataWord then parityBit will still be 1 and will toggle the bit in nParityWord
             * to zero, i.e. the hardware expects *even* parity.
             */
            nParityWord ^= (parityBit << offset);
            wordsProcessed++;
            offset++;
        }

        offset = 0; /* start at the beginning of the next word */

        /* Write the word to the flash memory */
        writeFlash(hDevice, false, addr, 4, (uint8_t*)&nParityWord);

        /* If the word has been successfully written to flash then increment the destination address */
        addr += 4;
    }

    /* Turn parity checking back on for the remainder of program execution */
    feeResult = adi_FEE_SetParityChecking(hDevice, ADI_FEE_PARITY_ENABLE_INTERRUPT);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        test_Fail("adi_FEE_SetParityChecking(ON) failed\n");
    }

    feeResult = adi_FEE_UnInit(hDevice);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        test_Fail("adi_FEE_UnInit() failed\n");
    }

    test_Perf("Parity checking test complete, parity checking now enabled");
}

static int testFlash(char *psDesc,
                     ADI_FEE_DEV_ID_TYPE const devID,
                     bool_t useDMA,
                     uint32_t const baseAddr,
                     uint32_t kiloBytes,
                     uint32_t iterations)
{
    ADI_FEE_RESULT_TYPE feeResult;
    ADI_FEE_DEV_HANDLE  hDevice;
    uint32_t flashSize = kiloBytes*1024u;
    uint32_t rnd = RAND_MAX;;
    uint32_t count = 0u;
    static uint8_t buffer[4096];
    uint32_t i;
    uint32_t forwardSig, reverseSig;

    /* Fill source buffer */
    for (i = 0u;
         i < sizeof(buffer);
         ++i)
    {
        buffer[i] = (uint8_t)(i % 255);
    }

    test_Perf(psDesc);

    /* Initialize flash driver */
    feeResult = adi_FEE_Init(devID, true, &hDevice);

    if (ADI_FEE_SUCCESS != feeResult)
    {
      test_Fail("adi_FEE_Init() failed\n");
    }

    /* Note: parity checking is turned off, for the specified flash controller,
     * by adi_FEE_Init(). If not then we would need to explicitly disable parity
     * checking here to avoid parity errors on data reads from the flash, as
     * we do not update the parity bits when writing to the flash region for
     * testing.
     */

#if (1 == USE_TIMERS)
    hAbortFlashDevice = hDevice;
    feeResult = adi_FEE_SetInterruptAbort(hDevice, WUT_IRQn, 1);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        sprintf(strbuf, "adi_FEE_SetInterruptAbort(%ld) failed with %d\n", SysTick_IRQn, feeResult);
        test_Fail(strbuf);
    }
#endif /* (1 == USE_TIMERS) */

    checkAndErase(hDevice, baseAddr, flashSize);

    //AG
    //assert(0x3FFFFFFF == RAND_MAX);

    /* Check that the flash array is all 0xFFFFFFFF */
    verifyFlashErased(baseAddr, flashSize);

    for(count = 1; count <= iterations; ++count)
    {
        uint32_t relAddr = rand() % (flashSize - 8);
        uint32_t limit = flashSize - relAddr - 8;
        uint32_t length = rnd;

        if (limit > 4095)
            limit = 4095;

        length = (rand() % limit);
        length >>= (rand() % 12);
        length += 1;

        if (ADI_FEE_DEVID_GP == devID)
        {
            /* Round down/up to 4-byte boundaries, so we can use DMA */
            relAddr &= ~3u;
            length = (length + 3) & ~3u;
        }

        assert(relAddr < 0x20000u);
        assert(length > 0u);
        assert(length <= 4096u);

        if (++totalCount <= skipCount)
        {
            continue;
        }

        sprintf(strbuf, "%3d: Testing absAddr = 0x%08lX, length = %lu",
                totalCount, baseAddr + relAddr, length);
        test_Perf(strbuf);

        writeFlash(hDevice, useDMA, baseAddr + relAddr, length, buffer);
        verifyFlash(baseAddr, flashSize - 8u, relAddr, length, 0u);

        /* check the flash signature */
        checkSignatures(hDevice, baseAddr, flashSize, ADI_FEE_SIGN_FORWARD, &forwardSig);

        if (forwardSig != crcBlock(baseAddr, baseAddr + flashSize - 12u, +4, IEEE_802_3_POLYNOMIAL, 0xFFFFFFFFu))
        {
            test_Fail("Forward signature doesn't match calculated CRC");
        }

        checkSignatures(hDevice, baseAddr, flashSize, ADI_FEE_SIGN_REVERSE, &reverseSig);

        if (reverseSig != crcBlock(baseAddr + flashSize - 12u, baseAddr, -4, IEEE_802_3_POLYNOMIAL, 0xFFFFFFFFu))
        {
            test_Fail("Reverse signature doesn't match calculated CRC");
        }

        /* We expect different signatures for the forward and reverse directions. It is
         * Possible for them to be the same, but this is unlikely enough that we should
         * be able to discount the possibility.
        */
        if (forwardSig == reverseSig)
        {
            test_Fail("Forward and reverse signatures are the same");
        }

        if (ADI_FEE_DEVID_0 == devID)
        {
            /* For flash controller 0 we don't use mass erase,
             * as our own code will be in the lower part of it.
             */
            erasePages(hDevice, baseAddr + relAddr, length);
            eraseSignaturePage(hDevice, baseAddr, flashSize);
        }
        else
        {
            /* For flash controller 1 or the GPF controller we
             * can use the mass erase.
             */
            massErase(hDevice);
        }

        verifyFlashErased(baseAddr, flashSize);
    }

    /* If we've just finished testing flash controller 0 then turn parity checking back
     * on for the remainder of program execution. Parity checking has to remain off during
     * testing, to avoid parity errors on data reads from the flash, but turning it on
     * here allows subsequent instruction fetches from flash 0 to be parity-checked.
     */
    if (ADI_FEE_DEVID_0 == devID)
    {
        feeResult = adi_FEE_SetParityChecking(hDevice, ADI_FEE_PARITY_ENABLE_INTERRUPT);

        if (ADI_FEE_SUCCESS != feeResult)
        {
            test_Fail("adi_FEE_SetParityChecking(ON) failed\n");
        }
    }

    feeResult = adi_FEE_UnInit(hDevice);

    if (ADI_FEE_SUCCESS != feeResult)
    {
        test_Fail("adi_FEE_UnInit() failed\n");
    }

    test_Perf("testFlash finished.");

    return 0;
}

void HardFault_Handler(void)
{
    test_Fail("Hard fault");
}

void BusFault_Handler(void)
{
    test_Fail("Bus fault");
}

void Parity_Int_Handler(void)
{
    if (parityErrorOccurred)
    {
         uint32_t errAddrLo = pADI_FEE0->FEEPARADRL;
         uint32_t errAddrHi = pADI_FEE0->FEEPARADRH;

         /* The first parity error was expected, this one isn't */
         sprintf(strbuf, "FLASH parity error, address = 0x%04x%04x", errAddrHi, errAddrLo);
         test_Fail(strbuf);
    }
    else
    {
        /* This is an expected parity error */
        parityErrorOccurred = true;
        NVIC_DisableIRQ(PARITY_IRQn);
   }
}

extern void initialise_monitor_handles(void);

/**
 * @brief  Function 'main' for flash memory test program.
 *
 * @return int (Zero for success, non-zero for failure).
 *
 *
 *
 *
 */
int FlashTest()
{

    /* NVIC initialization */
    NVIC_SetPriorityGrouping(12);

#if (1 == USE_TIMERS)
    /* SysTick initialization */
    SysTick_Config(1000000);

    /* SysTick_Config() hardcodes priority. We override this. */
    NVIC_SetPriority(SysTick_IRQn, 14);

    /* Wakeup timer initialization */
    InitializeWUT();

    if( adi_WUT_SetTimerEnable(hWUT, true) != 0 ) {
        test_Fail("adi_WUT_SetTimerEnable failed");
    }
#endif /* (1 == USE_TIMERS) */

    testParityChecking(ADI_FEE_DEVID_0, 0x00000000u, 0x00000000u, 0x8000u, 0x0003E000u);
    testParityChecking(ADI_FEE_DEVID_1, 0x00040000u, 0x00040000u, 0x7C00u, 0x0005F000u);


    /* We *want* a repeatable sequence of psuedo-random numbers, so that
     * the flash address ranges tested are the same on each run. Since
     * the rand seed starts at zero by default this call only serves to
     * make this explicit and visible.
     */
    srand(0);

    /* Test flash controller 0 (upper half only, minus the parity area = 120k) */
    testFlash("Testing flash controller 0", ADI_FEE_DEVID_0, false, 0x00020000u, 120, 10);

    /* Test flash controller 1 (all) */
    testFlash("Testing flash controller 1", ADI_FEE_DEVID_1, true, 0x00040000u, 128, 10);

    /* Test GP flash controller (all) using PIO*/
    testFlash("Testing GP flash controller (PIO)", ADI_FEE_DEVID_GP, false, 0x20080000u, 16, 10);

#if (1 == ADI_FEE_CFG_GPF_DMA_SUPPORT)
    /* Test GP flash controller (all) using DMA*/
    testFlash("Testing GP flash controller (DMA)", ADI_FEE_DEVID_GP, true, 0x20080000u, 16, 10);
#endif /* (1 == ADI_FEE_CFG_GPF_DMA_SUPPORT) */

    /* If we got here then nothing has failed */
    test_Pass();

    return 0;
}


#if (1 == USE_TIMERS)
/**
 * @brief  Interrupt handler for SysTick example program
 *
 * @return none
 *
 * SysTick Handler is called whenever SysTick interrupt occurs.
 * In this example, the handler increments the interrupt count
 */
void SysTick_Handler(void)
{
    /* bump Count up to COUNT  */
    if (COUNT != tickCount)
        tickCount += 1;

    if (NULL != hAbortFlashDevice)
    {
        adi_FEE_Abort(hAbortFlashDevice);
    }
}

static void wutCallback(void* hWut, uint32_t Event, void* pArg)
{
    ++wakeCount;
    pADI_WUT->T2CLRI |= T2CLRI_WUFB_CLR;
}

static void InitializeWUT(void)
{
    /* Initialise the wakeup timer */
    if( adi_WUT_Init(ADI_WUT_DEVID_0, &hWUT) != 0 ) {
        FAIL("adi_WUT_Init failed");
    }

    /* install WUT callback on all interrupts */
    if (adi_WUT_RegisterCallback(hWUT, wutCallback, ADI_WUT_TARGET_MASK)) {
        FAIL("adi_WUT_RegisterCallback failed");
    }

    /* Enable Wakeup timer in NVIC */
    NVIC_EnableIRQ(WUT_IRQn);

    /* select UCLK clock source */
    if( adi_WUT_SetClockSelect(hWUT, ADI_WUT_CLK_PCLK) != 0 ) {
        FAIL("adi_WUT_SetClockSelect failed");
    }

    /* No pre-scaling */
    if( adi_WUT_SetPrescaler(hWUT, (ADI_WUT_PRESCALER_TYPE)T2CON_PRE_DIV256) != 0 ) {
        FAIL("adi_WUT_SetPrescaler failed");
    }

    /* select free-running mode */
    if( adi_WUT_SetTimerMode(hWUT, ADI_WUT_MODE_PERIODIC) != 0 ) {
        FAIL("adi_WUT_SetTimerMode failed");
    }

    /* preload B comparator */
    if( adi_WUT_SetComparator(hWUT, ADI_WUT_COMPB, LOADB) != 0) {
        FAIL("adi_WUT_SetComparator failed");
    }
    /* enable Binterrupt */
    if( adi_WUT_SetInterruptEnable(hWUT, ADI_WUT_COMPB, true) != 0 ) {
        FAIL("adi_WUT_SetInterruptEnable failed");
    }
}

#endif /* (1 == USE_TIMERS) */

#if defined ( __ICCARM__ )  // IAR compiler...
/* Revert ADI MISRA Suppressions */
#define REVERT_ADI_MISRA_SUPPRESSIONS
#include "misra.h"
#endif

/*
** EOF
*/

/*@}*/
