
#include "BSP_limit_SW.h"


/**
  ******************************************************************************
  * @file    BSP_limit_SW.c
  * @version V1.0
  * @date    2020-07-13
  * @brief   限位开关
  ******************************************************************************
  * @attention
  *		LIMIT_SW1限位开关1  对应 J38
  * 	LIMIT_SW2限位开关2 	对应 J39
  *		NEST_SW归巢限位开关	对应 J40
  *		定义EXTI_ON	开启中断
  *		
  *
  ******************************************************************************
  */

 /*
  * @brief  限位开关初始化函数，限位开关在使用前必须先调用此函数
  * @param  无
  * @retval 无
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
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
		
	SYSCFG_EXTILineConfig(LIMIT_SW1_EXTI_PORT, LIMIT_SW1_EXTI_PIN);
	SYSCFG_EXTILineConfig(LIMIT_SW2_EXTI_PORT, LIMIT_SW2_EXTI_PIN);
	SYSCFG_EXTILineConfig(NEST_SW_EXTI_PORT  , NEST_SW_EXTI_PIN  );

	
	EXTI_InitStructure.EXTI_Line = LIMIT_SW1_EXTI_LINE;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
		
	EXTI_InitStructure.EXTI_Line = LIMIT_SW2_EXTI_LINE;//LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
		
	EXTI_InitStructure.EXTI_Line = NEST_SW_EXTI_LINE;//LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
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
    //清除中断标志位
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
    //清除中断标志位
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
    //清除中断标志位
		EXTI_ClearITPendingBit(LIMIT_SW2_EXTI_LINE);     
	}
}

 
 /*
  * @brief  获取限位开关1事件，并清零
  * @param  无
  * @retval 返回当前事件
*/
LIMIT_SW_Flag_Typedef Limit_SW1_Get_Event(void)
{
	LIMIT_SW_Flag_Typedef event = Limit_SW1_Flag;
	Limit_SW1_Flag = LIMIT_SW_NOTHING;
	return event;
}

 /*
  * @brief  获取限位开关2事件，并清零
  * @param  无
  * @retval 返回当前事件
*/
LIMIT_SW_Flag_Typedef Limit_SW2_Get_Event(void)
{
	LIMIT_SW_Flag_Typedef event = Limit_SW2_Flag;
	Limit_SW2_Flag = LIMIT_SW_NOTHING;
	return event;
}

 /*
  * @brief  获取归巢限位开关事件，并清零
  * @param  无
  * @retval 返回当前事件
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







