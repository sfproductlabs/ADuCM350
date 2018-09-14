#include <stdlib.h>     /* for NULL definition */
#include <limits.h>
#include "adi_osal.h"
#include "osal_ucos.h"
#include "adi_osal_arch.h"
#include "adi_osal_arch_internal.h"
#include "osal_common.h"
#include  <ucos_ii.h>
#include <cpu.h>
/*!
    @internal
    @var snCriticalRegionState
         Holds the state of the interrupt mask as of the first call to
         adi_osal_EnterCriticalRegion
    @endinternal
*/
static uint32_t snCriticalRegionState = 0u;

void _adi_osal_stdWrapper (void)
{
    /* Get the interrupt number */
    uint32_t nIntNum = __get_IPSR();
    
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    /* Tell uC/OS-II that we are starting an ISR */
    OSIntNesting++; 
    CPU_CRITICAL_EXIT();

     /* Call the higher level callback */
    _adi_osal_gHandlerTable[nIntNum].pfOSALHandler(ADI_NVIC_IRQ_SID(nIntNum),  _adi_osal_gHandlerTable[nIntNum].pOSALArg);

    /* Tell uC/OS-III that we are leaving the ISR */
    OSIntExit(); 
   
    return;
}
/*!
  ****************************************************************************
    @brief Disables interrupts to enable atomic execution of a critical region
    of code.

    Note that critical regions may be nested. A count is maintained to ensure a
    matching number of calls to adi_ExitCriticalRegion are made before
    restoring interrupts. Each critical region is also (implicitly) a scheduler
    lock.

    @see adi_osal_ExitCriticalRegion
*****************************************************************************/

void adi_osal_EnterCriticalRegion( void )
{

    /* accessing the global count variable needs to be protected from thread
     * switches so interrupt disable is called at the very begining. The value
     * of the interrupts state is only saved the first time. */

    uint32_t istate = _adi_osal_InterruptsDisable();

    if (0==_adi_osal_gnCriticalRegionNestingCnt)
    {
        /* only lock the interrupts the first time the function is called */
        snCriticalRegionState = istate;
    }

    _adi_osal_gnCriticalRegionNestingCnt++;


    return;
}

/*!
  ****************************************************************************
    @brief Re-enables interrupts and restores the interrupt status.

    This function decrements the count of nested critical regions. Use it as a
    closing bracket to adi_osal_EnterCriticalRegion.
    The Enter/ExitCriticalRegion sequence must be nestable

    @see adi_osal_EnterCriticalRegion
*****************************************************************************/

void adi_osal_ExitCriticalRegion( void )
{
    _adi_osal_gnCriticalRegionNestingCnt--;

    /* when the last nesting level is reached, reenable the interrupts */
    if (_adi_osal_gnCriticalRegionNestingCnt <= 0)
    {
        _adi_osal_gnCriticalRegionNestingCnt = 0;
        _adi_osal_InterruptsEnable(snCriticalRegionState);
    }

    return;
}

