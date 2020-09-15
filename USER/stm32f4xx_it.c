/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include <includes.h>				//支持OS时，使用

//#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
//#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
//#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数
//#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
//#endif

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
	
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

 
////systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{
    static uint16 counter;
	INT8U  err;
	DATA_CONTROL_BLOCK  *dp0,*dp1,*dp2,*dp8; 
	
	//OS_EVENT *Semaphore1;

	CPU_SR         cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
  OSIntNesting++;
  OS_EXIT_CRITICAL();

  OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
	
	counter++;

	if ( counter>=200 )		//400ms
	
	{
		counter = 0;

	}
	
	if ( (counter&0x01)==0 )//4ms
	{
		dp1 = OSMemGet(p_msgBuffer,&err);
		if(OS_NO_ERR == err)
		{
			((DATA_CONTROL_BLOCK *)dp1)->type = SENSOR_MSG; 
			if(OS_NO_ERR != OSQPost(timerQueue,(void*)dp1))     
			{
				OSMemPut(p_msgBuffer,dp1);
			}
		}
	}

	if ( (counter&0x07)==0 )//16ms
	{
		dp2 = OSMemGet(p_msgBuffer,&err);
		if(OS_NO_ERR == err)
		{
			((DATA_CONTROL_BLOCK *)dp2)->type = MOTOR_MSG; 
			if(OS_NO_ERR != OSQPost(LocalQueue,(void*)dp2))     
			{
				OSMemPut(p_msgBuffer,dp2);
			}
		}
	}
	
	if ( (counter&0x000F)==0 )//32ms
	{
		dp8= OSMemGet(p_msgBuffer,&err);
		if(OS_NO_ERR == err)
		{
			((DATA_CONTROL_BLOCK *)dp8)->type = WHL_MSG; 
			if(OS_NO_ERR != OSQPost(timerQueue,(void*)dp8))      
			{
				OSMemPut(p_msgBuffer,dp8);
			}
		}	
	}
	
	if((counter&0x0080)==0)//256ms
	{

    dp0 = OSMemGet(p_msgBuffer,&err);
		if(OS_NO_ERR == err)
		{
			((DATA_CONTROL_BLOCK *)dp0)->type = BATTERY_MSG; 
			if(OS_NO_ERR != OSQPost(timerQueue,(void*)dp0))      
			{
				OSMemPut(p_msgBuffer,dp0);
			}
		}	
	}
	
    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}


/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

extern void USART1_ISR(void);
void USART1_IRQHandler(void)
{       
	CPU_SR   cpu_sr;

	OS_ENTER_CRITICAL();                         
  OSIntNesting++;
  OS_EXIT_CRITICAL();
	
	#if ( OS_VIEW_MODULE == DEF_ENABLED )
		OSView_RxTxISRHandler();
	#endif
	
	USART1_ISR();
	
	OSIntExit();
}

extern void BRD_POWER_ISR(void);
void BRD_UART_IRQHandler(void)    //USART2_IRQHandler
{       
	CPU_SR    cpu_sr;

	OS_ENTER_CRITICAL();                         
  OSIntNesting++;
  OS_EXIT_CRITICAL();
	
	#if ( OS_VIEW_MODULE == DEF_ENABLED )
		OSView_RxTxISRHandler();
	#endif
	
	BRD_POWER_ISR();
	
	OSIntExit();
}

extern void USART3_ISR(void);
void USART3_IRQHandler(void)
{       
	CPU_SR    cpu_sr;

	OS_ENTER_CRITICAL();                         
  OSIntNesting++;
  OS_EXIT_CRITICAL();
	
	#if ( OS_VIEW_MODULE == DEF_ENABLED )
		OSView_RxTxISRHandler();
	#endif
	
	USART3_ISR();
	
	OSIntExit();
}

extern void UART4_ISR(void);
void UART4_IRQHandler(void)
{       
	CPU_SR    cpu_sr;

	OS_ENTER_CRITICAL();                         
  OSIntNesting++;
  OS_EXIT_CRITICAL();
	
	#if ( OS_VIEW_MODULE == DEF_ENABLED )
		OSView_RxTxISRHandler();
	#endif
	
	UART4_ISR();
	
	OSIntExit();
}

extern void GPS_UART_ISR(u8 dataTemp);
void GPS_RS232_UART_IRQHandler(void)
{       
	CPU_SR   cpu_sr;
	u8 tmp;
	OS_ENTER_CRITICAL();                         
  OSIntNesting++;
  OS_EXIT_CRITICAL();
	
	#if ( OS_VIEW_MODULE == DEF_ENABLED )
		OSView_RxTxISRHandler();
	#endif
	
	if(USART_GetITStatus(GPS_RS232_UART, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(GPS_RS232_UART);
		GPS_UART_ISR(tmp);
	}
	
	OSIntExit();
}



/*
  * @brief  EXTI9_5，中断处理函数
  * @param  无
  * @retval 无
*/
void EXTI9_5_IRQHandler(void)
{
	//LIMIT_SW2_EXTI_Handler();
	ENCCODER_AB_IRQHandler ();
	ENCCODER_Z_EXTI_Handler();	
	
}



////extern void EXTI15_10_IRQ_ISR(void);
//void EXTI15_10_IRQHandler(void)
//{
//	CPU_SR         cpu_sr;
//	
//	OS_ENTER_CRITICAL();                         
//	OSIntNesting++;
//	OS_EXIT_CRITICAL();

//  #if ( OS_VIEW_MODULE == DEF_ENABLED )
//		OSView_RxTxISRHandler();
//	#endif
//	
//	EXTI15_10_IRQ_ISR();
//	
//	OSIntExit();	
//	
//}





/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
