

#include <includes.h>

//20200806 caigz add


/*
*********************************************************************************************************
*                                         OS_CPU_SysTickHandler()
*
* Description: Handle the system tick (SysTick) interrupt, which is used to generate the uC/OS-II tick
*              interrupt.
*
* Arguments  : none.
*
* Note(s)    : 1) This function MUST be placed on entry 15 of the Cortex-M3 vector table.
*********************************************************************************************************
*/

void  OS_CPU_SysTickHandler (void)
{
    OS_CPU_SR  cpu_sr;


    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}



/*
*********************************************************************************************************
*                                          OS_CPU_SysTickClkFreq()
*
* Description: 
*
* Arguments  : none.
*
* Note(s)    : 1) 
*********************************************************************************************************
*/
INT32U OS_CPU_SysTickClkFreq(void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*
*********************************************************************************************************
*                                          OS_CPU_SysTickInit()
*
* Description: Initialize the SysTick.
*
* Arguments  : none.
*
* Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
*********************************************************************************************************
*/

void  OS_CPU_SysTickInit (void)
{
	/* SYSTICK分频--时钟节拍为:OS_TICKS_PER_SEC	*/
	if ( SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC) )
  	{ 
  	  	/* Capture error */ 
    	while (1);
  	}
    
}

