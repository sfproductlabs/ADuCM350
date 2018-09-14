#ifndef __ADI_OSAL_ARCH_INTERNAL__
#define __ADI_OSAL_ARCH_INTERNAL__

#include <adi_osal.h> 
#include <adi_osal_arch.h>
#include <device.h>

/* external data (shared across OSAL environments) */

/*!
    @internal
    @var _adi_osal_gHandlerTable
	The OSAL dispatch table. It is an array of function pointers and callback arguments, 
    of the type corresponding to OSAL's plain C interrupt handlers (i.e. the high-level
	handlers).
    @endinternal
 */
extern ADI_OSAL_HANDLER_ENTRY _adi_osal_gHandlerTable[];

/*!
    @internal
    @var _adi_osal_gHandlerTableSize
	The size of the OSAL dispatch table. The size needs to be large enough for 
    any index that we can get back from adi_nvic_RegisterHandler(). 
    @endinternal
 */
extern uint32_t _adi_osal_gHandlerTableSize;

/* external code (shared across OSAL environments) */
extern ADI_OSAL_STATUS _adi_osal_HeapInstall(uint32_t *pHeapMemory, uint32_t nHeapMemorySize);
extern ADI_OSAL_STATUS _adi_osal_MemAlloc(void** ppData, uint32_t nSize);
extern void            _adi_osal_MemFree(void* pData);
extern bool            _adi_osal_IsCurrentLevelISR( void );
extern bool            _adi_osal_IsMemoryAligned(const void *pMemory);

extern void _adi_osal_stdWrapper(void);

#pragma inline
static uint32_t _adi_osal_InterruptsDisable(void)
{
    uint32_t state = __get_PRIMASK();
    if(state == 0)
    {
        __disable_irq();     
    }
    return state;
}

#pragma inline
static void _adi_osal_InterruptsEnable(uint32_t previousState)
{
   if(previousState == 0)
   {
        __enable_irq();
   }
}

/*!
  ****************************************************************************

   @internal

   @brief Describes whether the API is called at interrupt level or not

   @return true  - if the current execution level is greater than 0
   @return false - if the level is 0

   @endinternal
*****************************************************************************/
#pragma inline
bool _adi_osal_IsCurrentLevelISR( void )
{
    return (0ul != __get_IPSR()) ;
}

/*!
  ****************************************************************************
   @internal

   @brief This function indicates whether a pointer is aligned and can be used
          to store variables in the particular architecture

   @param[in] pMemory - Pointer to the allocated memory

   @return true    - if the memory was aligned
   @return false   - if the memory was not aligned

   @endinternal

*****************************************************************************/

#pragma inline
bool _adi_osal_IsMemoryAligned(const void *pMemory)
{
    return( 0u == ((uint32_t)(pMemory) & 0x3u));
}

#endif /* __ADI_OSAL_ARCH_INTERNAL__ */
