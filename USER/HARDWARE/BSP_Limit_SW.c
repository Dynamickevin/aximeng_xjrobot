
#include "BSP_limit_SW.h"


/**
  ******************************************************************************
  * @file    BSP_limit_SW.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   ��λ����
  ******************************************************************************
  * @attention
  *		LIMIT_SW1��λ����1  ��Ӧ J38
  * 	LIMIT_SW2��λ����2 	��Ӧ J39
  *		NEST_SW�鳲��λ����	��Ӧ J40
  *		����EXTI_ON	�����ж�
  *		
  *
  ******************************************************************************
  */

 /*
  * @brief  ��λ���س�ʼ����������λ������ʹ��ǰ�����ȵ��ô˺���
  * @param  ��
  * @retval ��
*/
void  bsp_Limit_SW_Init(void)
{
	

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( LIMIT_SW1_GPIO_CLK|LIMIT_SW2_GPIO_CLK|NEST_SW_GPIO_CLK, ENABLE); 
	

	GPIO_InitStructure.GPIO_Pin = LIMIT_SW1_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_Init(LIMIT_SW1_GPIO_PORT, &GPIO_InitStructure);	
															   
	GPIO_InitStructure.GPIO_Pin = LIMIT_SW2_PIN;
	GPIO_Init(LIMIT_SW2_GPIO_PORT, &GPIO_InitStructure);
															   
	GPIO_InitStructure.GPIO_Pin = NEST_SW_PIN;	
	GPIO_Init(NEST_SW_GPIO_PORT, &GPIO_InitStructure);	

	
#ifdef EXTI_ON
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
		
	SYSCFG_EXTILineConfig(LIMIT_SW1_EXTI_PORT, LIMIT_SW1_EXTI_PIN);
	SYSCFG_EXTILineConfig(LIMIT_SW2_EXTI_PORT, LIMIT_SW2_EXTI_PIN);
	SYSCFG_EXTILineConfig(NEST_SW_EXTI_PORT  , NEST_SW_EXTI_PIN  );

	
	EXTI_InitStructure.EXTI_Line = LIMIT_SW1_EXTI_LINE;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����
		
	EXTI_InitStructure.EXTI_Line = LIMIT_SW2_EXTI_LINE;//LINE0
	EXTI_Init(&EXTI_InitStructure);//����
		
	EXTI_InitStructure.EXTI_Line = NEST_SW_EXTI_LINE;//LINE0
	EXTI_Init(&EXTI_InitStructure);//����
#endif

}

#ifdef	EXTI_ON

void NEST_SW_EXTI_Handler(void)
{
	if(EXTI_GetITStatus(NEST_SW_EXTI_LINE) != RESET) 
	{
		if(NEST_SW_ReadBit()){
			Nest_SW_Flag = LIMIT_SW_RISING;
		}else{
			Nest_SW_Flag = LIMIT_SW_FALLING;
		}
    //����жϱ�־λ
		EXTI_ClearITPendingBit(NEST_SW_EXTI_LINE);     
	} 
}
void LIMIT_SW1_EXTI_Handler(void)
{
	if(EXTI_GetITStatus(LIMIT_SW1_EXTI_LINE) != RESET) 
	{
		if(Limit_SW1_ReadBit()){
			Limit_SW1_Flag = LIMIT_SW_RISING;
		}else{
			Limit_SW1_Flag = LIMIT_SW_FALLING;
		}
    //����жϱ�־λ
		EXTI_ClearITPendingBit(LIMIT_SW1_EXTI_LINE);     
	}
}
void LIMIT_SW2_EXTI_Handler(void)
{
	if(EXTI_GetITStatus(LIMIT_SW2_EXTI_LINE) != RESET) 
	{
		if(Limit_SW2_ReadBit()){
			Limit_SW2_Flag = LIMIT_SW_RISING;
		}else{
			Limit_SW2_Flag = LIMIT_SW_FALLING;
		}
    //����жϱ�־λ
		EXTI_ClearITPendingBit(LIMIT_SW2_EXTI_LINE);     
	}
}

 
 /*
  * @brief  ��ȡ��λ����1�¼���������
  * @param  ��
  * @retval ���ص�ǰ�¼�
*/
LIMIT_SW_Flag_Typedef Limit_SW1_Get_Event(void)
{
	LIMIT_SW_Flag_Typedef event = Limit_SW1_Flag;
	Limit_SW1_Flag = LIMIT_SW_NOTHING;
	return event;
}

 /*
  * @brief  ��ȡ��λ����2�¼���������
  * @param  ��
  * @retval ���ص�ǰ�¼�
*/
LIMIT_SW_Flag_Typedef Limit_SW2_Get_Event(void)
{
	LIMIT_SW_Flag_Typedef event = Limit_SW2_Flag;
	Limit_SW2_Flag = LIMIT_SW_NOTHING;
	return event;
}

 /*
  * @brief  ��ȡ�鳲��λ�����¼���������
  * @param  ��
  * @retval ���ص�ǰ�¼�
*/
LIMIT_SW_Flag_Typedef Nest_SW_Get_Event(void)
{
	LIMIT_SW_Flag_Typedef event = Nest_SW_Flag;
	Nest_SW_Flag = LIMIT_SW_NOTHING;
	return event;
}
#endif

#define true  1
#define false 0


#ifndef bool
#define bool unsigned char
#endif

 static __inline bool  SW_GpioGet(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET )
    {
        return true;
    }
    else
    {
        return false;
    }
}







