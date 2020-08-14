/**
  ******************************************************************************
  * @file    bsp_general_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ͨ�ö�ʱ����ʱ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_general_tim.h"

uint8_t TIME_1ms,TIME_10ms,TIME_100ms,TIME_1s;

uint32_t Time100ms;
/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ����TIMx_CLK,x[6,7] 
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
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
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
					
/****************************************************************///֮��Ϊ1S					
				}			
				bsp_HI3521_Heater();	
				Time100ms++;
/****************************************************************///֮��Ϊ100mS					
			}
			
			
/****************************************************************///֮��Ϊ10mS			
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
/****************************************************************///֮��Ϊ1msS	
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_IT_Update);  		 
	}		 	
}


/*********************************************END OF FILE**********************/
