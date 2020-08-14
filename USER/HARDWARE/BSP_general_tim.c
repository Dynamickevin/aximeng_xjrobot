/**
  ******************************************************************************
  * @file    bsp_general_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器定时范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_general_tim.h"

uint8_t TIME_1ms,TIME_10ms,TIME_100ms,TIME_1s;

uint32_t Time100ms;
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x[6,7] 
	RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_ARR;       
	
	TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_PSC;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(GENERAL_TIM, ENABLE);		
}

/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void GENERAL_TIM_Configuration(void)
{
	TIM_Mode_Config();
}



void  GENERAL_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		TIME_1ms++;
		if(TIME_1ms == 10){
			TIME_1ms = 0;
			TIME_10ms++;
			if(TIME_10ms == 10){
				TIME_10ms = 0;
				TIME_100ms++;
				if(TIME_100ms == 10){
					TIME_100ms = 0;
					TIME_1s ++;
					TIME_1s = TIME_1s%200;
					LED1(TIME_1s%2);
					LED2((TIME_1s+1)%2);
/**********************************************************************/					
					
/****************************************************************///之上为1S					
				}			
				bsp_HI3521_Heater();	
				Time100ms++;
/****************************************************************///之上为100mS					
			}
			
			
/****************************************************************///之上为10mS			
		}

		if(RF_RX_Buf.Timeout)RF_RX_Buf.Timeout--;
		else{
			if(RF_RX_Buf.Status != UART_READY)
				RF_RX_Buf.Status = UART_READY;			
		}
		
		if(BRD_RX_Buf.Timeout)BRD_RX_Buf.Timeout--;
		else{
			if(BRD_RX_Buf.Status != UART_READY)
				BRD_RX_Buf.Status = UART_END;			
		}
/****************************************************************///之上为1msS	
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_IT_Update);  		 
	}		 	
}


/*********************************************END OF FILE**********************/
